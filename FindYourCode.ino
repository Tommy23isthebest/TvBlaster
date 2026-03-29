// IR Remote Reader — Point your remote and press buttons
// Open Serial Monitor at 9600 baud to see the codes
//
// Wiring:
//   IR receiver OUT pin → pin 11
//   IR receiver VCC → 5V
//   IR receiver GND → GND

#include <IRremote.hpp>

#define IR_RECEIVE_PIN 11

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("IR Reader ready — point remote and press buttons");
  Serial.println("================================================");
  Serial.println();
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.println("========== BUTTON PRESS ==========");
    Serial.print("Protocol:  ");
    Serial.println(getProtocolString(IrReceiver.decodedIRData.protocol));
    Serial.print("Address:   0x");
    Serial.println(IrReceiver.decodedIRData.address, HEX);
    Serial.print("Command:   0x");
    Serial.println(IrReceiver.decodedIRData.command, HEX);
    Serial.print("Raw Data:  0x");
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    Serial.print("Bits:      ");
    Serial.println(IrReceiver.decodedIRData.numberOfBits);
    Serial.println("==================================");
    Serial.println();
    IrReceiver.resume();
  }
}
