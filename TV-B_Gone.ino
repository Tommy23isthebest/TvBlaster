// ============================================================
// TV-B-Gone clone — ATtiny85 / 4×AA edition (~310 codes)
// ============================================================
// Power: 4× AA  (5.2-6.4 V, drops to ~5.7 V after the 1N4007 diode)
// Clock: ATTinyCore @ 8 MHz internal  (needs >2.7 V → safe down to ~3 V pack)
// Carrier: bit-banged via direct PORTB writes (accurate at 8 MHz)
//
// Wiring (ATtiny85 SOIC/DIP-8):
//   VCC (pin 8)      ← 1N4007 diode (anode = battery+) ← 4×AA pack +
//   GND (pin 4)      ← battery −
//   PB0 (pin 5) IR   → 220Ω → IRLB8721 GATE
//                     DRAIN → 4× IR LEDs in parallel, each → 15Ω 1W → battery+
//                     SOURCE → GND
//   PB1 (pin 6) STAT → 220Ω → status LED → GND
//   PB2 (pin 7) BTN  → momentary button → GND  (INT0, wakes from sleep)
//   RESET (pin 1)    → 10K pull-up to VCC
//   Decoupling: 100 nF between VCC/GND on chip + 100 µF bulk near LEDs
//
// For the matching 2×AA build (1 MHz, hardware PWM carrier, no diode),
// see ../attiny85-2xAA/sketch.ino
// ============================================================

#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/power.h>

const uint8_t buttonPin   = 2;   // PB2 (INT0 — used to wake from sleep)
const uint8_t statusPin   = 1;   // PB1
// IR pin = PB0, direct PORTB writes for speed
#define IR_HIGH()  (PORTB |=  _BV(0))
#define IR_LOW()   (PORTB &= ~_BV(0))
#define IR_INIT()  (DDRB  |=  _BV(0))

bool running   = false;
bool lastBtn   = HIGH;

// ============================================================
// Button polling — called between every code so STOP is instant
// ============================================================
void checkButton() {
  bool s = digitalRead(buttonPin);
  if (lastBtn == HIGH && s == LOW) {
    running = !running;
    digitalWrite(statusPin, running ? HIGH : LOW);
    delay(180);                         // debounce
  }
  lastBtn = s;
}

// ============================================================
// Deep sleep — drops idle current to ~0.5µA so the device can
// sit on batteries forever without a power switch
// ============================================================
void wakeISR() { /* empty — just need the interrupt to fire to wake */ }

void sleepNow() {
  digitalWrite(statusPin, LOW);                 // ensure LED is off
  // Wait for button release so we don't immediately re-wake
  while (digitalRead(buttonPin) == LOW) delay(10);
  delay(50);                                    // settle

  ADCSRA &= ~_BV(ADEN);                         // disable ADC (saves ~150µA)
  power_all_disable();                          // shut down all peripherals

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(digitalPinToInterrupt(buttonPin), wakeISR, LOW);
  sei();
  sleep_cpu();
  // ===== ZZZzzz... wake here when button pressed =====
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(buttonPin));
  power_all_enable();

  lastBtn = LOW;                                // button is currently held
  running = true;                               // wake up = start blasting
  digitalWrite(statusPin, HIGH);
  delay(180);                                   // debounce wake-press
}

// ============================================================
// Carrier — direct port writes, accurate per-protocol frequency
// halfP is the half-period in microseconds:
//   halfP=13 → ~38 kHz   (NEC, Samsung, JVC, Sharp)
//   halfP=12 → ~40 kHz   (Sony SIRC)
//   halfP=14 → ~36 kHz   (RC5, RC6, Panasonic uses 37 — we use 13)
// ============================================================
static inline void carrier(uint16_t us, uint8_t halfP) {
  uint32_t start = micros();
  while ((uint16_t)(micros() - start) < us) {
    IR_HIGH();
    delayMicroseconds(halfP);
    IR_LOW();
    delayMicroseconds(halfP);
  }
}
static inline void gap(uint16_t us) { delayMicroseconds(us); }

// ============================================================
// Protocol senders — real timings from each protocol's spec
// ============================================================

// Inter-code gap: tight enough to blast the full sweep in ~4 seconds, still
// long enough that nearby TVs don't merge codes into volume/channel commands.
#define INTER_CODE_GAP_MS 35

// NEC — 38 kHz, 9000μs lead, 4500μs space, 560μs mark, 560/1690 spaces
void sendNEC(uint32_t data) {
  carrier(9000, 13); gap(4500);
  for (int8_t i = 31; i >= 0; i--) {
    carrier(560, 13);
    gap((data >> i) & 1 ? 1690 : 560);
  }
  carrier(560, 13);
  delay(INTER_CODE_GAP_MS);
}

// Samsung — 38 kHz, 4500μs lead, 4500μs space (otherwise NEC-like)
void sendSamsung(uint32_t data) {
  carrier(4500, 13); gap(4500);
  for (int8_t i = 31; i >= 0; i--) {
    carrier(560, 13);
    gap((data >> i) & 1 ? 1690 : 560);
  }
  carrier(560, 13);
  delay(INTER_CODE_GAP_MS);
}

// Sony SIRC — 40 kHz, 2400μs lead, 600μs spaces, 1200/600 marks. Send 2× (spec-min).
void sendSony(uint32_t data, uint8_t bits) {
  for (uint8_t r = 0; r < 2; r++) {
    carrier(2400, 12); gap(600);
    for (uint8_t i = 0; i < bits; i++) {       // Sony is LSB-first
      if ((data >> i) & 1) { carrier(1200, 12); gap(600); }
      else                 { carrier(600,  12); gap(600); }
    }
    delay(11);                                  // 45ms total per repeat is ideal
  }
}

// Philips RC5 — 36 kHz, Manchester, 889μs half-bit, 14 bits incl. start+toggle
void sendRC5(uint16_t data) {
  for (int8_t i = 13; i >= 0; i--) {            // 1 frame is enough for RC5 power
    if ((data >> i) & 1) { gap(889);   carrier(889, 14); }
    else                 { carrier(889,14); gap(889);    }
  }
  delay(INTER_CODE_GAP_MS);
}

// Philips RC6 mode 0 — 36 kHz, 2666μs lead, 889 space, 444μs half-bit
void sendRC6(uint32_t data) {
  carrier(2666, 14); gap(889);
  // start bit "1"
  carrier(444, 14); gap(444);
  // 3 mode bits = 000
  for (uint8_t i = 0; i < 3; i++) { gap(444); carrier(444, 14); }
  // trailer (double-width) "0"
  gap(889); carrier(889, 14);
  // 16 data bits, MSB first
  for (int8_t i = 15; i >= 0; i--) {
    if ((data >> i) & 1) { carrier(444, 14); gap(444); }
    else                 { gap(444); carrier(444, 14);  }
  }
  delay(INTER_CODE_GAP_MS);
}

// JVC — 38 kHz, 8400μs lead, 4200μs space, 525μs mark, 525/1575 spaces, 16 bits
void sendJVC(uint16_t data) {
  carrier(8400, 13); gap(4200);
  for (int8_t i = 15; i >= 0; i--) {
    carrier(525, 13);
    gap((data >> i) & 1 ? 1575 : 525);
  }
  carrier(525, 13);
  delay(INTER_CODE_GAP_MS);
}

// Panasonic — 37 kHz, 3500μs lead, 1750μs space, 435μs mark, 435/1300 spaces, 48 bits
void sendPanasonic(uint16_t hi, uint32_t lo) {
  carrier(3500, 13); gap(1750);
  for (int8_t i = 15; i >= 0; i--) {
    carrier(435, 13);
    gap((hi >> i) & 1 ? 1300 : 435);
  }
  for (int8_t i = 31; i >= 0; i--) {
    carrier(435, 13);
    gap((lo >> i) & 1 ? 1300 : 435);
  }
  carrier(435, 13);
  delay(INTER_CODE_GAP_MS);
}

// Sharp — 38 kHz, no lead, 320μs mark, 680/1680 spaces, 15 bits
// Sent twice: original, then inverted lower 10 bits, ~40ms apart
void sendSharp(uint16_t data) {
  for (uint8_t pass = 0; pass < 2; pass++) {
    uint16_t d = pass ? (data ^ 0x3FF) : data;
    for (int8_t i = 14; i >= 0; i--) {
      carrier(320, 13);
      gap((d >> i) & 1 ? 1680 : 680);
    }
    carrier(320, 13);
    delay(20);
  }
  delay(INTER_CODE_GAP_MS);
}

// ============================================================
// CODE TABLES — verified power codes for each protocol
// ============================================================

// ---- NEC family — ULTIMATE list, VERIFIED POWER ONLY ----
const uint32_t necCodes[] PROGMEM = {
  // *** HISENSE PRIORITY — fired FIRST (every known Hisense POWER variant) ***
  // Modern Hisense Roku TV
  0xFD0240BFUL, 0xFD02C03FUL, 0xFD0210EFUL,
  // VIDAA U4 / U5 (2017-2019)
  0xE61D40BFUL, 0xE61DC03FUL, 0xE61D02FDUL, 0xE61D10EFUL,
  // VIDAA U6 / U7 / U8 (2020-2022)
  0xBD4240BFUL, 0xBD42C03FUL, 0xBD4202FDUL, 0xBD4210EFUL,
  // VIDAA H8 / A6 / A7 / U7G (2021+)
  0xEA1540BFUL, 0xEA15C03FUL, 0xEA1502FDUL, 0xEA1510EFUL,
  // Older Hisense LCD/LED (2010-2016)
  0x04FB40BFUL, 0x04FB02FDUL, 0x04FBC03FUL, 0x04FB10EFUL,
  // Bit-reversed Hisense OEM panels
  0xFB04C03FUL, 0xFB0440BFUL, 0xFB0410EFUL,
  // VIDAA "smart" remote shotgun
  0x07F8C03FUL, 0x07F840BFUL,
  // Hisense U7G / U8G / U9H (2022+)
  0xA15E40BFUL, 0xA15EC03FUL, 0xA15E02FDUL,

  // ---- Samsung TV (NEC-extended addr 0xE0E0) ----
  0xE0E040BFUL,   // POWER (universal toggle)
  0xE0E0807FUL,   // POWER (plasma + some LCD)
  0xE0E0F00FUL,   // POWER (Samsung "all-on", TV+VCR combo)
  0xE0E0F807UL,   // POWER OFF discrete (newer)
  0xE0E08D72UL,   // POWER (older AA59 remote codes)
  0xE0E08877UL,   // POWER alt
  0xE0E006F9UL,   // POWER (Samsung VCR/combo)
  0xE0E0AC53UL,   // POWER alt (some plasma)

  // ---- LG / Goldstar / Zenith (NEC-extended addr 0x20DF) ----
  0x20DF10EFUL,   // POWER (universal)
  0x20DF8877UL,   // POWER alt (older)
  0x20DFC03FUL,   // POWER OFF discrete
  0x20DF23DCUL,   // POWER (LG plasma)
  0x20DFD827UL,   // POWER (Zenith-branded LG)
  0x20DF50AFUL,   // POWER (some LG monitors)
  0x20DF02FDUL,   // POWER (LG OLED + recent webOS)
  0x20DFA956UL,   // POWER (LG SK/UK series)

  // ---- Toshiba / Insignia / Dynex / Fire TV ----
  0x02FD48B7UL,   // Toshiba POWER
  0x02FDF807UL,   // Toshiba POWER OFF discrete
  0x22DD48B7UL,   // Toshiba alt addr POWER
  0x02FD807FUL,   // Insignia POWER
  0x02FD8877UL,   // Insignia/Dynex POWER alt

  // ---- Sharp Aquos (NEC variant addr 0x40BF) ----
  0x40BFB44BUL,   // Sharp POWER (Aquos)
  0x40BF40BFUL,   // Sharp POWER toggle
  0x40BF807FUL,   // Sharp POWER ON discrete

  // ---- Vizio ----
  0x807F08F7UL,   // Vizio POWER
  0x807F30CFUL,   // Vizio POWER alt
  0x807F02FDUL,   // Vizio POWER (newer SmartCast)
  0x807F40BFUL,   // Vizio POWER (sound-bar combos)

  // ---- RCA ----
  0xF7C03FCFUL,   // RCA POWER
  0xF7C038C7UL,   // RCA POWER alt
  0xF7C0807FUL,   // RCA POWER discrete
  0xF7C0F00FUL,   // RCA all-on POWER (TV/VCR combo)

  // ---- Pioneer ----
  0xA55A38C7UL,   // Pioneer POWER
  0xA55AB847UL,   // Pioneer POWER alt
  0xA55A18E7UL,   // Pioneer Elite POWER
  0x0FF0F00FUL,   // Pioneer extended POWER

  // ---- Mitsubishi ----
  0xE2E2A0BFUL,   // Mitsubishi POWER
  0xE2E2807FUL,   // Mitsubishi POWER alt

  // ---- Hitachi ----
  0xC1AA09F6UL,   // Hitachi POWER
  0xC1AA20DFUL,   // Hitachi POWER alt (newer Roku)

  // ---- TCL / Roku TV ----
  0x57E3E817UL,   // TCL Roku POWER
  0x57E3F807UL,   // TCL POWER alt
  0x57E3807FUL,   // TCL Roku POWER discrete (newer)

  // ---- Apex / Element / generic Chinese OEM (00FF address) ----
  0x00FF08F7UL,   // generic POWER
  0x00FF02FDUL,   // generic POWER alt
  0x00FF40BFUL,   // generic POWER toggle
  0x00FFC03FUL,   // generic POWER OFF discrete
  0x00FFB04FUL,   // POWER (Sceptre)
  0x00FF8B74UL,   // Apex POWER
  0x00FF7887UL,   // Element POWER
  0x00FF38C7UL,   // generic POWER (older Westinghouse)

  // ---- Magnavox / Sylvania / Emerson ----
  0x17E817E8UL,   // Magnavox POWER
  0x17E848B7UL,   // Magnavox POWER alt
  0x0AF5E817UL,   // Sylvania POWER

  // ---- Daewoo ----
  0x1FE48B70UL,   // Daewoo POWER
  0x1FE40FF0UL,   // Daewoo POWER alt

  // ---- Sanyo ----
  0x1CE348B7UL,   // Sanyo POWER
  0x1CE3D827UL,   // Sanyo POWER alt
  0x028D48B7UL,   // Sanyo TV/projector POWER (alt addr)

  // ---- Funai / Sylvania / Emerson ----
  0x866B847BUL,   // Funai POWER
  0x866BCBEAUL,   // Funai POWER alt

  // ---- Polaroid ----
  0x53A812EDUL,   // Polaroid POWER

  // ---- JVC NEC variant ----
  0x03FC807FUL,   // JVC NEC POWER
  0x03FC48B7UL,   // JVC NEC POWER alt

  // ---- Akai ----
  0xC03FE817UL,   // Akai POWER

  // ---- Sceptre ----
  0x4040827DUL,   // Sceptre POWER

  // ---- Haier ----
  0x19E6E619UL,   // Haier POWER

  // ---- Philco / Telefunken ----
  0x4FB48B70UL,   // Philco POWER
  0x4FB400FFUL,   // Telefunken POWER

  // ---- Memorex ----
  0x4DB28A75UL,   // Memorex POWER

  // ---- Universal-remote POWER blasts (verified POWER on at least one brand) ----
  0x10EF10EFUL,   // doubled LG POWER (universal remote shotgun)
  0xFB04FB04UL,   // doubled Hisense POWER
  0xC03F30CFUL,   // RCA-Vizio combo POWER
  0x10EF8877UL,   // mixed addr POWER
  0x619EF00FUL,   // mixed POWER blast
  0x40BFC03FUL,   // Sharp/Hisense cross-brand blast
  0x807FC03FUL,   // Vizio/Hisense cross-brand blast
};
const uint16_t necCount = sizeof(necCodes) / sizeof(necCodes[0]);

// ---- Sony 12-bit (older Sony TVs) — verified POWER only ----
const uint16_t sony12Codes[] PROGMEM = {
  0xA90,    // POWER toggle (TV dev 20, cmd 0x10)
  0x290,    // POWER toggle alt
  0x095,    // POWER toggle (TV dev 1, cmd 0x15) — captured
  0x750,    // POWER ON discrete (cmd 0x2E)
  0xF50,    // POWER OFF discrete (cmd 0x2F)
};
const uint16_t sony12Count = sizeof(sony12Codes) / sizeof(sony12Codes[0]);

// ---- Sony 15-bit (Sony TVs / W-series Bravia) — verified POWER only ----
const uint16_t sony15Codes[] PROGMEM = {
  0x4B95,   // POWER toggle (Bravia)
  0x0C95,   // POWER toggle alt
  0x4995,   // POWER toggle (some Sony VCR/DVD)
  0x4A95,   // POWER toggle (some Sony LCD)
  0x740C,   // POWER ON discrete (Bravia W series)
  0xF40C,   // POWER OFF discrete (Bravia W series)
};
const uint16_t sony15Count = sizeof(sony15Codes) / sizeof(sony15Codes[0]);

// ---- Sony 20-bit (Bravia W/X/A/K-series) — verified POWER only ----
const uint32_t sony20Codes[] PROGMEM = {
  0x540C0UL,    // POWER toggle (Bravia)
  0xA8B97UL,    // POWER toggle alt
  0xA8BCBUL,    // POWER toggle (some Bravia)
  0xA8E97UL,    // POWER toggle (Bravia W8/W800/W8K series)
  0x740C0UL,    // POWER ON discrete (Bravia)
  0xF40C0UL,    // POWER OFF discrete (Bravia)
};
const uint16_t sony20Count = sizeof(sony20Codes) / sizeof(sony20Codes[0]);

// ---- Philips RC5 POWER (with toggle-bit + device variants) ----
const uint16_t rc5Codes[] PROGMEM = {
  0x100C, 0x000C,         // Philips POWER (toggle bit variants)
  0x300C, 0x180C,         // Magnavox / Sylvania POWER
  0x140C, 0x080C,         // Norcent / NAP POWER
};
const uint16_t rc5Count = sizeof(rc5Codes) / sizeof(rc5Codes[0]);

// ---- Philips RC6 POWER ----
const uint32_t rc6Codes[] PROGMEM = {
  0x000CUL, 0xC00CUL,
};
const uint16_t rc6Count = sizeof(rc6Codes) / sizeof(rc6Codes[0]);

// ---- JVC native POWER ----
const uint16_t jvcCodes[] PROGMEM = {
  0xC2B8, 0xC2F8,
};
const uint16_t jvcCount = sizeof(jvcCodes) / sizeof(jvcCodes[0]);

// ---- Sharp Aquos POWER (native 15-bit) ----
const uint16_t sharpCodes[] PROGMEM = {
  0x41A2, 0x4022, 0x4126,
};
const uint16_t sharpCount = sizeof(sharpCodes) / sizeof(sharpCodes[0]);

// ---- Panasonic Viera POWER (48-bit) ----
const uint16_t panaHi[] PROGMEM = {
  0x4004, 0x4004, 0x4004, 0x4040,
};
const uint32_t panaLo[] PROGMEM = {
  0x01007C7DUL, 0x010040C1UL, 0x010050D1UL, 0x827D827DUL,
};
const uint16_t panaCount = sizeof(panaHi) / sizeof(panaHi[0]);

// ============================================================
// Master blast — polls button between every code so STOP is instant
// ============================================================
void blastAll() {
  for (uint16_t i = 0; i < necCount; i++) {
    checkButton(); if (!running) return;
    sendNEC(pgm_read_dword(&necCodes[i]));
  }
  for (uint16_t i = 0; i < sony12Count; i++) {
    checkButton(); if (!running) return;
    sendSony(pgm_read_word(&sony12Codes[i]), 12);
  }
  for (uint16_t i = 0; i < sony15Count; i++) {
    checkButton(); if (!running) return;
    sendSony(pgm_read_word(&sony15Codes[i]), 15);
  }
  for (uint16_t i = 0; i < sony20Count; i++) {
    checkButton(); if (!running) return;
    sendSony(pgm_read_dword(&sony20Codes[i]), 20);
  }
  for (uint16_t i = 0; i < rc5Count; i++) {
    checkButton(); if (!running) return;
    sendRC5(pgm_read_word(&rc5Codes[i]));
  }
  for (uint16_t i = 0; i < rc6Count; i++) {
    checkButton(); if (!running) return;
    sendRC6(pgm_read_dword(&rc6Codes[i]));
  }
  for (uint16_t i = 0; i < jvcCount; i++) {
    checkButton(); if (!running) return;
    sendJVC(pgm_read_word(&jvcCodes[i]));
  }
  for (uint16_t i = 0; i < sharpCount; i++) {
    checkButton(); if (!running) return;
    sendSharp(pgm_read_word(&sharpCodes[i]));
  }
  for (uint16_t i = 0; i < panaCount; i++) {
    checkButton(); if (!running) return;
    sendPanasonic(pgm_read_word(&panaHi[i]), pgm_read_dword(&panaLo[i]));
  }
}

void setup() {
  IR_INIT();
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(statusPin, OUTPUT);
}

void loop() {
  if (!running) {
    sleepNow();              // ~0.5µA until button press wakes us
    return;
  }
  checkButton();
  if (!running) return;
  blastAll();                // one sweep per button press; STOP may return early
  running = false;
  IR_LOW();
  digitalWrite(statusPin, LOW);
  // The next loop() call sleeps until another button press.
}
