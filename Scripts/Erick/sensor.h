#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

// =======================
// PINES (ESP8266)
// =======================
#define TRIG_PIN 12   // D6
#define ECHO_PIN 14   // D5

// =======================
// CONSTANTES
// =======================
#define SOUND_SPEED 0.0343   // cm/us
#define TIMEOUT_US 25000     // ~4.3 m

// =======================
// CALIBRACIÓN
// =======================
// Si a 10 cm reales marca 11 → restamos 1
#define OFFSET_CM (-1)

// =======================
// INIT
// =======================
void sensorInit() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
}

// =======================
// MEDICIÓN SIMPLE
// =======================
long medirDistanciaCM() {

  // Pulso TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Medir duración del ECHO
  unsigned long duracion = pulseIn(ECHO_PIN, HIGH, TIMEOUT_US);

  if (duracion == 0)
    return -1;   // timeout / sin eco

  // Convertir a cm
  long distancia = (duracion * SOUND_SPEED) / 2;

  // Aplicar offset de calibración
  distancia += OFFSET_CM;

  if (distancia < 0)
    distancia = 0;

  return distancia;
}

#endif
