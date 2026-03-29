// TV-B-Gone — IR TV Killer — Uno
// Cycles through power-off codes for 20+ TV brands.
// Point the IR LED at a TV and it turns off within seconds.
//
// Wiring:
//   IR LED anode (long leg) → pin 3 (through 100Ω resistor)
//   IR LED cathode (short leg) → GND
//   For more range: use a PN2222 transistor to drive the LED
//
// Board: Uno
// Library: IRremote v4.x (install via Library Manager)
//
// Hold the IR LED within 2-3 feet of the TV and point it
// directly at the IR sensor (usually bottom center of bezel).

#include <IRremote.hpp>

#define IR_SEND_PIN 3
#define CYCLE_PAUSE 5000

void setup() {
  Serial.begin(9600);
  IrSender.begin(IR_SEND_PIN);
  Serial.println("TV-B-Gone starting...");
  Serial.println("Point IR LED at TV sensor.");
  Serial.println();
  sendAllCodes();
}

void loop() {
  delay(CYCLE_PAUSE);
  Serial.println("--- Cycling again ---");
  sendAllCodes();
}

void pr(const char* name) {
  Serial.print("  -> ");
  Serial.println(name);
}

void sendAllCodes() {

  // ===== HISENSE (multiple remote variants) =====
  pr("Hisense EN2B27");
  IrSender.sendNEC(0x04, 0x08, 2);
  delay(100);

  pr("Hisense EN-33926A");
  IrSender.sendNEC(0x04, 0x09, 2);
  delay(100);

  pr("Hisense EN2A27");
  IrSender.sendNEC(0x04, 0x02, 2);
  delay(100);

  pr("Hisense alt1");
  IrSender.sendNEC(0x00, 0x08, 2);
  delay(100);

  pr("Hisense alt2");
  IrSender.sendNEC(0x00, 0x02, 2);
  delay(100);

  pr("Hisense ERF3A69");
  IrSender.sendNEC(0x01, 0x08, 2);
  delay(100);

  pr("Hisense Roku");
  IrSender.sendNEC(0x04, 0x18, 2);
  delay(100);

  // ===== SAMSUNG =====
  pr("Samsung");
  IrSender.sendSamsung(0x0707, 0x02, 2);
  delay(100);

  pr("Samsung alt");
  IrSender.sendSamsung(0x0707, 0x98, 2);
  delay(100);

  // ===== LG =====
  pr("LG");
  IrSender.sendNEC(0x04, 0x08, 2);
  delay(100);

  pr("LG alt");
  IrSender.sendNEC(0x04, 0xC4, 2);
  delay(100);

  // ===== SONY =====
  pr("Sony 12-bit");
  IrSender.sendSony(0x01, 0x15, 2, 12);
  delay(100);

  pr("Sony 15-bit");
  IrSender.sendSony(0x01, 0x15, 2, 15);
  delay(100);

  pr("Sony 20-bit");
  IrSender.sendSony(0x01, 0x15, 2, 20);
  delay(100);

  // ===== PANASONIC =====
  pr("Panasonic");
  IrSender.sendPanasonic(0x4004, 0x100BCBD, 2);
  delay(100);

  // ===== VIZIO =====
  pr("Vizio");
  IrSender.sendNEC(0x04, 0x08, 2);
  delay(100);

  pr("Vizio alt");
  IrSender.sendNEC(0x00, 0x15, 2);
  delay(100);

  // ===== PHILIPS =====
  pr("Philips RC5");
  IrSender.sendRC5(0x00, 0x0C, 2);
  delay(100);

  pr("Philips RC6");
  IrSender.sendRC6(0x00, 0x0C, 2);
  delay(100);

  // ===== SHARP =====
  pr("Sharp");
  IrSender.sendNEC(0x02, 0x45, 2);
  delay(100);

  // ===== TOSHIBA =====
  pr("Toshiba");
  IrSender.sendNEC(0x40, 0x12, 2);
  delay(100);

  pr("Toshiba alt");
  IrSender.sendNEC(0x44, 0x12, 2);
  delay(100);

  // ===== TCL / ROKU =====
  pr("TCL/Roku");
  IrSender.sendNEC(0x04, 0x18, 2);
  delay(100);

  // ===== SANYO =====
  pr("Sanyo");
  IrSender.sendNEC(0x38, 0x12, 2);
  delay(100);

  // ===== RCA =====
  pr("RCA");
  IrSender.sendNEC(0xF0, 0x0F, 2);
  delay(100);

  // ===== JVC =====
  pr("JVC");
  IrSender.sendNEC(0x03, 0x17, 2);
  delay(100);

  // ===== HAIER =====
  pr("Haier");
  IrSender.sendNEC(0x98, 0x67, 2);
  delay(100);

  // ===== INSIGNIA =====
  pr("Insignia");
  IrSender.sendNEC(0x04, 0x08, 2);
  delay(100);

  // ===== MAGNAVOX =====
  pr("Magnavox");
  IrSender.sendNEC(0x38, 0x0A, 2);
  delay(100);

  // ===== EMERSON =====
  pr("Emerson");
  IrSender.sendNEC(0x86, 0x0F, 2);
  delay(100);

  // ===== HITACHI =====
  pr("Hitachi");
  IrSender.sendNEC(0x50, 0x17, 2);
  delay(100);

  // ===== PIONEER =====
  pr("Pioneer");
  IrSender.sendNEC(0xA5, 0x1C, 2);
  delay(100);

  // ===== WESTINGHOUSE =====
  pr("Westinghouse");
  IrSender.sendNEC(0x04, 0x08, 2);
  delay(100);

  // ===== FUNAI =====
  pr("Funai");
  IrSender.sendNEC(0x07, 0x02, 2);
  delay(100);

  Serial.println("=== Cycle complete ===");
}
