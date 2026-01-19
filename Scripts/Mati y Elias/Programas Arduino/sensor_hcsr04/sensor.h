#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

// =======================
// PINES (NodeMCU)
// =======================
// D1 = GPIO5
// D2 = GPIO4
#define TRIG_PIN 5    // D1
#define ECHO_PIN 4    // D2

// =======================
// CONSTANTES
// =======================
#define TIMEOUT_US 30000     // ~5 m

// =======================
// CALIBRACIÓN
// =======================
#define OFFSET_CM 0

// =======================
// INIT
// =======================
inline void sensorInit() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
}

// =======================
// MEDICIÓN SIMPLE
// =======================
inline long medirDistanciaCM() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duracion = pulseIn(ECHO_PIN, HIGH, TIMEOUT_US);

  if (duracion == 0) {
    return -1;
  }

  long distancia = duracion * 0.0343 / 2;
  distancia += OFFSET_CM;

  if (distancia < 0)
    distancia = 0;

  return distancia;
}

#endif
