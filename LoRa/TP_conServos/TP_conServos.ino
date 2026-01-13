#include <Servo.h>

// ====== ENTRADAS DESDE LORA ======
#define VALVE_CMD_PIN    4
#define CURTAIN_CMD_PIN  5

// ====== SALIDAS A SERVOS ======
#define VALVE_SERVO_PIN    9
#define CURTAIN_SERVO_PIN  10

// ====== SERVOS ======
Servo valveServo;
Servo curtainServo;

// ====== ESTADO PREVIO (para debug limpio) ======
int lastValveCmd   = -1;
int lastCurtainCmd = -1;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Arduino esclavo iniciado");

  // Entradas de control
  pinMode(VALVE_CMD_PIN, INPUT);
  pinMode(CURTAIN_CMD_PIN, INPUT);

  // Adjuntar servos
  valveServo.attach(VALVE_SERVO_PIN);
  curtainServo.attach(CURTAIN_SERVO_PIN);

  // Estado inicial seguro
  valveServo.write(0);    // válvula cerrada
  curtainServo.write(0);  // cortina cerrada

  Serial.println("Servos inicializados en CLOSED");
}

void loop() {

  int valveCmd   = digitalRead(VALVE_CMD_PIN);
  int curtainCmd = digitalRead(CURTAIN_CMD_PIN);

  // ----- VÁLVULA -----
  if (valveCmd != lastValveCmd) {
    Serial.print("VALVE_CMD_PIN = ");
    Serial.println(valveCmd ? "HIGH → OPEN" : "LOW → CLOSE");

    if (valveCmd == HIGH) {
      valveServo.write(90);   // abrir
    } else {
      valveServo.write(0);    // cerrar
    }
    lastValveCmd = valveCmd;
  }

  // ----- CORTINA -----
  if (curtainCmd != lastCurtainCmd) {
    Serial.print("CURTAIN_CMD_PIN = ");
    Serial.println(curtainCmd ? "HIGH → OPEN" : "LOW → CLOSE");

    if (curtainCmd == HIGH) {
      curtainServo.write(90); // abrir
    } else {
      curtainServo.write(0);  // cerrar
    }
    lastCurtainCmd = curtainCmd;
  }

  delay(20); // refresco típico de servo
}
