#include <IRremote.h>

void setup() {
  Serial.begin(115200);
  IrReceiver.begin(11, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    IrReceiver.printIRResultRawFormatted(&Serial, true);
    Serial.println("-----");
    IrReceiver.resume();
  }
}
