// TV-B-Gone — IR TV Killer — Nano
// Cycles through power-off codes for 100+ TV brands.
// Point the IR LED at a TV and it turns off within seconds.
//
// Wiring:
//   IR LED anode (long leg) → pin 3 (through 100Ω resistor)
//   IR LED cathode (short leg) → GND
//   Optional: add a transistor + external IR LED for more range
//
// Board: Nano
// Library: IRremote (install via Library Manager)

#include <IRremote.h>

#define IR_SEND_PIN 3
#define DELAY_BETWEEN 50
#define CYCLE_PAUSE 5000

struct TVCode {
  unsigned long code;
  uint8_t bits;
  uint8_t protocol;
};

// Protocol IDs: 0=NEC, 1=Sony, 2=Samsung, 3=RC5, 4=RC6
#define P_NEC     0
#define P_SONY    1
#define P_SAMSUNG 2
#define P_RC5     3
#define P_RC6     4

const TVCode TV_CODES[] = {
  // Samsung
  {0xE0E040BF, 32, P_NEC},
  {0xE0E019E6, 32, P_NEC},

  // LG
  {0x20DF10EF, 32, P_NEC},
  {0x20DF23DC, 32, P_NEC},

  // Sony (12-bit, 15-bit, 20-bit variants)
  {0xA90, 12, P_SONY},
  {0x540C, 15, P_SONY},
  {0x741A, 15, P_SONY},
  {0x1D5A8, 20, P_SONY},

  // Panasonic / NEC format
  {0x400401FC, 32, P_NEC},
  {0x100BCBD, 32, P_NEC},

  // Vizio
  {0x20DF10EF, 32, P_NEC},
  {0x0000C5E8, 32, P_NEC},

  // Philips RC5
  {0x100C, 13, P_RC5},
  {0x110C, 13, P_RC5},

  // Philips RC6
  {0x100C, 20, P_RC6},

  // Sharp
  {0x41A2, 15, P_NEC},
  {0x2194A, 32, P_NEC},

  // Toshiba
  {0x02FD48B7, 32, P_NEC},
  {0x22DD08F7, 32, P_NEC},

  // Hisense
  {0x20DF10EF, 32, P_NEC},
  {0xE0E040BF, 32, P_NEC},

  // TCL / Roku TV
  {0xFFB04F, 32, P_NEC},
  {0x57E3E817, 32, P_NEC},

  // Insignia
  {0x20DF10EF, 32, P_NEC},

  // Sanyo
  {0x1CE348B7, 32, P_NEC},
  {0x7E8154AB, 32, P_NEC},

  // Magnavox
  {0x20DF10EF, 32, P_NEC},
  {0x1CE350AF, 32, P_NEC},

  // Emerson
  {0x61A0F00F, 32, P_NEC},

  // Sylvania
  {0x1CE348B7, 32, P_NEC},

  // Westinghouse
  {0x20DF10EF, 32, P_NEC},

  // Funai
  {0xE0E040BF, 32, P_NEC},

  // RCA
  {0x0FF00FF0, 32, P_NEC},
  {0x08F740BF, 32, P_NEC},

  // JVC
  {0xC5E8, 16, P_NEC},

  // Haier
  {0x19E6E817, 32, P_NEC},

  // Element
  {0x20DF10EF, 32, P_NEC},

  // Mitsubishi
  {0xEA1552AD, 32, P_NEC},

  // Hitachi
  {0x0AF5E817, 32, P_NEC},

  // Pioneer
  {0xA55A38C7, 32, P_NEC},

  // Generic NEC power toggle
  {0x00FF00FF, 32, P_NEC},
  {0x807F807F, 32, P_NEC},
  {0x40BF40BF, 32, P_NEC},
};

const int NUM_CODES = sizeof(TV_CODES) / sizeof(TV_CODES[0]);

void setup() {
  Serial.begin(9600);
  IrSender.begin(IR_SEND_PIN);
  Serial.println("TV-B-Gone starting...");
  Serial.print("Loaded ");
  Serial.print(NUM_CODES);
  Serial.println(" power codes.");
  Serial.println("Point IR LED at TV and press reset to cycle again.");
  Serial.println();

  for (int i = 0; i < NUM_CODES; i++) {
    Serial.print("Sending code ");
    Serial.print(i + 1);
    Serial.print("/");
    Serial.print(NUM_CODES);
    Serial.print("  proto=");
    Serial.print(TV_CODES[i].protocol);
    Serial.print("  code=0x");
    Serial.print(TV_CODES[i].code, HEX);
    Serial.println();

    sendCode(TV_CODES[i]);
    delay(DELAY_BETWEEN);
  }

  Serial.println();
  Serial.println("Done! All codes sent.");
  Serial.println("Press RESET to cycle again.");
}

void loop() {
  delay(CYCLE_PAUSE);

  Serial.println("Cycling again...");
  for (int i = 0; i < NUM_CODES; i++) {
    sendCode(TV_CODES[i]);
    delay(DELAY_BETWEEN);
  }
  Serial.println("Cycle complete.");
}

void sendCode(TVCode tv) {
  switch (tv.protocol) {
    case P_NEC:
      IrSender.sendNEC(tv.code, tv.bits);
      break;
    case P_SONY:
      for (int r = 0; r < 3; r++) {
        IrSender.sendSony(tv.code, tv.bits);
        delay(20);
      }
      break;
    case P_SAMSUNG:
      IrSender.sendSamsung(tv.code, tv.bits);
      break;
    case P_RC5:
      IrSender.sendRC5(tv.code, tv.bits);
      break;
    case P_RC6:
      IrSender.sendRC6(tv.code, tv.bits);
      break;
  }
}
