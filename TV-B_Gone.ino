// TV-B-Gone — IR TV Killer — Uno (3 LED version)
// Wiring:
//   IR LED anode (+) → pin 3, 5, 6 (through 100Ω resistor each)
//   IR LED cathode (–) → GND

#include <IRremote.hpp>

#define IR_SEND_PIN1 3
#define IR_SEND_PIN2 5
#define IR_SEND_PIN3 6

#define CYCLE_PAUSE 300000

void setup() {
    Serial.begin(9600);
    IrSender.begin(IR_SEND_PIN1);
    Serial.println("TV-B-Gone starting...");
    sendAllCodes();
}

void loop() {
    delay(CYCLE_PAUSE);
    Serial.println("--- Cycling again ---");
    sendAllCodes();
}

void sendOnAllPinsNEC(uint16_t address, uint8_t command) {
    IrSender.begin(IR_SEND_PIN1);
    IrSender.sendNEC(address, command, 2);
    delay(20);
    IrSender.begin(IR_SEND_PIN2);
    IrSender.sendNEC(address, command, 2);
    delay(20);
    IrSender.begin(IR_SEND_PIN3);
    IrSender.sendNEC(address, command, 2);
}

void sendOnAllPinsSamsung(uint16_t address, uint16_t command) {
    IrSender.begin(IR_SEND_PIN1);
    IrSender.sendSamsung(address, command, 2);
    delay(20);
    IrSender.begin(IR_SEND_PIN2);
    IrSender.sendSamsung(address, command, 2);
    delay(20);
    IrSender.begin(IR_SEND_PIN3);
    IrSender.sendSamsung(address, command, 2);
}

void sendOnAllPinsSony(uint16_t address, uint8_t command, uint8_t nbits) {
    IrSender.begin(IR_SEND_PIN1);
    IrSender.sendSony(address, command, 2, nbits);
    delay(20);
    IrSender.begin(IR_SEND_PIN2);
    IrSender.sendSony(address, command, 2, nbits);
    delay(20);
    IrSender.begin(IR_SEND_PIN3);
    IrSender.sendSony(address, command, 2, nbits);
}

void sendOnAllPinsPanasonic(uint16_t address, uint8_t command) {
    IrSender.begin(IR_SEND_PIN1);
    IrSender.sendPanasonic(address, command, 2);
    delay(20);
    IrSender.begin(IR_SEND_PIN2);
    IrSender.sendPanasonic(address, command, 2);
    delay(20);
    IrSender.begin(IR_SEND_PIN3);
    IrSender.sendPanasonic(address, command, 2);
}

void sendOnAllPinsRC5(uint8_t address, uint8_t command) {
    IrSender.begin(IR_SEND_PIN1);
    IrSender.sendRC5(address, command, 2);
    delay(20);
    IrSender.begin(IR_SEND_PIN2);
    IrSender.sendRC5(address, command, 2);
    delay(20);
    IrSender.begin(IR_SEND_PIN3);
    IrSender.sendRC5(address, command, 2);
}

void sendOnAllPinsRC6(uint8_t address, uint8_t command) {
    IrSender.begin(IR_SEND_PIN1);
    IrSender.sendRC6(address, command, 2);
    delay(20);
    IrSender.begin(IR_SEND_PIN2);
    IrSender.sendRC6(address, command, 2);
    delay(20);
    IrSender.begin(IR_SEND_PIN3);
    IrSender.sendRC6(address, command, 2);
}

void pr(const char* name) {
    Serial.print("  -> ");
    Serial.println(name);
}

void sendAllCodes() {
    pr("Hisense POWER OFF (0x04FB)");
    sendOnAllPinsNEC(0x20, 0x4E);
    delay(100);

    pr("Hisense POWER TOGGLE (0x04FB)");
    sendOnAllPinsNEC(0x20, 0x0E);
    delay(100);

    pr("Hisense POWER OFF (0x00FD)");
    sendOnAllPinsNEC(0x00, 0xD1);
    delay(100);

    pr("Hisense POWER TOGGLE (0x00FD)");
    sendOnAllPinsNEC(0x00, 0x10);
    delay(100);

    pr("Hisense POWER TOGGLE alt (0x00FD)");
    sendOnAllPinsNEC(0x00, 0x0D);
    delay(100);

    pr("Hisense VIDAA POWER OFF");
    sendOnAllPinsNEC(0x00, 0x1A);
    delay(100);

    pr("Hisense VIDAA POWER TOGGLE");
    sendOnAllPinsNEC(0x00, 0x12);
    delay(100);

    pr("Samsung");
    sendOnAllPinsSamsung(0x0707, 0x02);
    delay(100);

    pr("Samsung alt");
    sendOnAllPinsSamsung(0x0707, 0x98);
    delay(100);

    pr("LG");
    sendOnAllPinsNEC(0x04, 0x08);
    delay(100);

    pr("LG alt");
    sendOnAllPinsNEC(0x04, 0xC4);
    delay(100);

    pr("Sony 12-bit");
    sendOnAllPinsSony(0x01, 0x15, 12);
    delay(100);

    pr("Sony 15-bit");
    sendOnAllPinsSony(0x01, 0x15, 15);
    delay(100);

    pr("Sony 20-bit");
    sendOnAllPinsSony(0x01, 0x15, 20);
    delay(100);

    pr("Panasonic");
    sendOnAllPinsPanasonic(0x4004, 0x3D);
    delay(100);

    pr("Vizio");
    sendOnAllPinsNEC(0x04, 0x08);
    delay(100);

    pr("Vizio alt");
    sendOnAllPinsNEC(0x00, 0x15);
    delay(100);

    pr("Philips RC5");
    sendOnAllPinsRC5(0x00, 0x0C);
    delay(100);

    pr("Philips RC6");
    sendOnAllPinsRC6(0x00, 0x0C);
    delay(100);

    pr("Sharp");
    sendOnAllPinsNEC(0x02, 0x45);
    delay(100);

    pr("Toshiba");
    sendOnAllPinsNEC(0x40, 0x12);
    delay(100);

    pr("Toshiba alt");
    sendOnAllPinsNEC(0x44, 0x12);
    delay(100);

    pr("TCL/Roku");
    sendOnAllPinsNEC(0x04, 0x18);
    delay(100);

    pr("Sanyo");
    sendOnAllPinsNEC(0x38, 0x12);
    delay(100);

    pr("RCA");
    sendOnAllPinsNEC(0xF0, 0x0F);
    delay(100);

    pr("JVC");
    sendOnAllPinsNEC(0x03, 0x17);
    delay(100);

    pr("Haier");
    sendOnAllPinsNEC(0x98, 0x67);
    delay(100);

    pr("Insignia");
    sendOnAllPinsNEC(0x04, 0x08);
    delay(100);

    pr("Magnavox");
    sendOnAllPinsNEC(0x38, 0x0A);
    delay(100);

    pr("Emerson");
    sendOnAllPinsNEC(0x86, 0x0F);
    delay(100);

    pr("Hitachi");
    sendOnAllPinsNEC(0x50, 0x17);
    delay(100);

    pr("Pioneer");
    sendOnAllPinsNEC(0xA5, 0x1C);
    delay(100);

    pr("Westinghouse");
    sendOnAllPinsNEC(0x04, 0x08);
    delay(100);

    pr("Funai");
    sendOnAllPinsNEC(0x07, 0x02);
    delay(100);

    Serial.println("=== Cycle complete ===");
}
void sendOnAllPinsRC5(uint8_t address, uint8_t command) {
    IrSender.begin(IR_SEND_PIN1);
    IrSender.sendRC5(address, command, 2);
    delay(20);
    IrSender.begin(IR_SEND_PIN2);
    IrSender.sendRC5(address, command, 2);
    delay(20);
    IrSender.begin(IR_SEND_PIN3);
    IrSender.sendRC5(address, command, 2);
}

void sendOnAllPinsRC6(uint8_t address, uint8_t command) {
    IrSender.begin(IR_SEND_PIN1);
    IrSender.sendRC6(address, command, 2);
    delay(20);
    IrSender.begin(IR_SEND_PIN2);
    IrSender.sendRC6(address, command, 2);
    delay(20);
    IrSender.begin(IR_SEND_PIN3);
    IrSender.sendRC6(address, command, 2);
}

void pr(const char* name) {
    Serial.print("  -> ");
    Serial.println(name);
}

void sendAllCodes() {
    pr("Hisense POWER OFF (0x04FB)");
    sendOnAllPinsNEC(0x20, 0x4E);
    delay(100);

    pr("Hisense POWER TOGGLE (0x04FB)");
    sendOnAllPinsNEC(0x20, 0x0E);
    delay(100);

    pr("Hisense POWER OFF (0x00FD)");
    sendOnAllPinsNEC(0x00, 0xD1);
    delay(100);

    pr("Hisense POWER TOGGLE (0x00FD)");
    sendOnAllPinsNEC(0x00, 0x10);
    delay(100);

    pr("Hisense POWER TOGGLE alt (0x00FD)");
    sendOnAllPinsNEC(0x00, 0x0D);
    delay(100);

    pr("Hisense VIDAA POWER OFF");
    sendOnAllPinsNEC(0x00, 0x1A);
    delay(100);

    pr("Hisense VIDAA POWER TOGGLE");
    sendOnAllPinsNEC(0x00, 0x12);
    delay(100);

    pr("Samsung");
    sendOnAllPinsSamsung(0x0707, 0x02);
    delay(100);

    pr("Samsung alt");
    sendOnAllPinsSamsung(0x0707, 0x98);
    delay(100);

    pr("LG");
    sendOnAllPinsNEC(0x04, 0x08);
    delay(100);

    pr("LG alt");
    sendOnAllPinsNEC(0x04, 0xC4);
    delay(100);

    pr("Sony 12-bit");
    sendOnAllPinsSony(0x01, 0x15, 12);
    delay(100);

    pr("Sony 15-bit");
    sendOnAllPinsSony(0x01, 0x15, 15);
    delay(100);

    pr("Sony 20-bit");
    sendOnAllPinsSony(0x01, 0x15, 20);
    delay(100);

    pr("Panasonic");
    sendOnAllPinsPanasonic(0x4004, 0x3D);
    delay(100);

    pr("Vizio");
    sendOnAllPinsNEC(0x04, 0x08);
    delay(100);

    pr("Vizio alt");
    sendOnAllPinsNEC(0x00, 0x15);
    delay(100);

    pr("Philips RC5");
    sendOnAllPinsRC5(0x00, 0x0C);
    delay(100);

    pr("Philips RC6");
    sendOnAllPinsRC6(0x00, 0x0C);
    delay(100);

    pr("Sharp");
    sendOnAllPinsNEC(0x02, 0x45);
    delay(100);

    pr("Toshiba");
    sendOnAllPinsNEC(0x40, 0x12);
    delay(100);

    pr("Toshiba alt");
    sendOnAllPinsNEC(0x44, 0x12);
    delay(100);

    pr("TCL/Roku");
    sendOnAllPinsNEC(0x04, 0x18);
    delay(100);

    pr("Sanyo");
    sendOnAllPinsNEC(0x38, 0x12);
    delay(100);

    pr("RCA");
    sendOnAllPinsNEC(0xF0, 0x0F);
    delay(100);

    pr("JVC");
    sendOnAllPinsNEC(0x03, 0x17);
    delay(100);

    pr("Haier");
    sendOnAllPinsNEC(0x98, 0x67);
    delay(100);

    pr("Insignia");
    sendOnAllPinsNEC(0x04, 0x08);
    delay(100);

    pr("Magnavox");
    sendOnAllPinsNEC(0x38, 0x0A);
    delay(100);

    pr("Emerson");
    sendOnAllPinsNEC(0x86, 0x0F);
    delay(100);

    pr("Hitachi");
    sendOnAllPinsNEC(0x50, 0x17);
    delay(100);

    pr("Pioneer");
    sendOnAllPinsNEC(0xA5, 0x1C);
    delay(100);

    pr("Westinghouse");
    sendOnAllPinsNEC(0x04, 0x08);
    delay(100);

    pr("Funai");
    sendOnAllPinsNEC(0x07, 0x02);
    delay(100);

    Serial.println("=== Cycle complete ===");
}  pr("LG");
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
}  IrSender.sendSamsung(0x0707, 0x98, 2);
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
}}

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
