#include <IRremote.h>

int IR_LED = 3;
IRsend irsend(IR_LED);

// Frecuencia del aire (38 kHz)
const int khz = 38;

// Frame ON principal (tu captura)
uint16_t frame_on_1[] = {
  8950, 4400,
  600,1650, 600,1600, 600,500, 600,500, 600,500, 600,500,
  600,1650, 600,1600, 600,1650, 600,1600, 600,1650,
  600,500, 600,500, 600,500, 600,500,
  600,1600, 600,500, 600,500, 600,500, 600,500, 600,500,
  600,500, 600,500, 600,500, 600,500, 600,500,
  600,500, 600,500, 600,500, 600,500, 600,500,
  600,500, 600,1650, 600,500, 600,1600, 600,500,
  600,500, 600,500, 600,500, 600,500, 600,500, 600,500
};

// Frame ON corto
uint16_t frame_on_2[] = {
  600,500, 600,500, 600,500, 600,500, 600,500, 600,1650,
  600,1600, 600,1650, 600,1600, 600,500, 600,500, 600,500,
  600,500, 600,500, 600,500, 600,500, 600,500
};

void setup() {
  Serial.begin(115200);
  irsend.begin();
  Serial.println("Probador NEC-modificado iniciado");
}

void loop() {

  Serial.println("Mandando ON frame 1...");
  irsend.sendRaw(frame_on_1, sizeof(frame_on_1)/2, khz);
  delay(30);

  Serial.println("Mandando ON frame 2...");
  irsend.sendRaw(frame_on_2, sizeof(frame_on_2)/2, khz);
  delay(2000);
}
