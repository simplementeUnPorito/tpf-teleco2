#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <DHT.h>

// ================== SENSORES ==================
#define DHTPIN A0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define LDR_PIN A1   // LDR en A1

// ================== GPIO COMANDOS ==================
#define LED_PIN         13
#define VALVE_CMD_PIN    9   // → Arduino externo (válvula)
#define CURTAIN_CMD_PIN 10   // → Arduino externo (cortina)

// ================== OTAA ==================
static const u1_t PROGMEM APPEUI[8] = {0};
static const u1_t PROGMEM DEVEUI[8] = {
  0x9D,0x22,0x07,0xD0,0x7E,0xD5,0xB3,0x70
};
static const u1_t PROGMEM APPKEY[16] = {
  0x7E,0x42,0xE5,0xA9,0xCA,0x60,0x38,0x95,
  0x38,0xD0,0x39,0x06,0xF1,0xEA,0xE8,0x02
};

void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8); }
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8); }
void os_getDevKey (u1_t* buf) { memcpy_P(buf, APPKEY, 16); }

// ================== LMIC ==================
static osjob_t sendjob;
static uint8_t payload[3];

const lmic_pinmap lmic_pins = {
  .nss  = 8,
  .rxtx = LMIC_UNUSED_PIN,
  .rst  = 4,
  .dio  = {7, 5, LMIC_UNUSED_PIN}
};

// ================== LUZ ==================
uint8_t lightLevelFromRaw(int raw) {
  if (raw < 350) return 1;   // tenue
  return 2;
}

// ================== ENVÍO UPLINK ==================
void do_send(osjob_t* j) {

  if (LMIC.opmode & OP_TXRXPEND) return;

  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("DHT error → no envio");
    return;
  }

  uint16_t hum = (uint16_t)(h * 100);
  payload[0] = highByte(hum);
  payload[1] = lowByte(hum);

  int ldrRaw = analogRead(LDR_PIN);
  payload[2] = lightLevelFromRaw(ldrRaw);

  LMIC_setTxData2(1, payload, 3, 0);

  Serial.print("TX | Hum=");
  Serial.print(h);
  Serial.print("% | LDR=");
  Serial.print(ldrRaw);
  Serial.print(" | Nivel=");
  Serial.println(payload[2]);
}

// ================== EVENTOS LMIC ==================
void onEvent(ev_t ev) {

  switch (ev) {

    case EV_JOINING:
      Serial.println("EV_JOINING");
      break;

    case EV_JOINED:
      Serial.println("EV_JOINED");
      LMIC_setLinkCheckMode(0);
      do_send(&sendjob);
      break;

    case EV_TXCOMPLETE:
      Serial.println("EV_TXCOMPLETE");

      // ====== DECISIÓN LOCAL: VÁLVULA ======
      {
        float h = dht.readHumidity();
        if (!isnan(h)) {
          digitalWrite(VALVE_CMD_PIN, (h < 50.0) ? HIGH : LOW);
          Serial.print("VALVE_CMD = ");
          Serial.println(h < 50.0 ? "OPEN" : "CLOSE");
        }
      }

      // ====== DOWNLINK: CORTINA ======
      if (LMIC.dataLen > 0) {

        uint8_t port = LMIC.frame[LMIC.dataBeg - 1];
        uint8_t cmd  = LMIC.frame[LMIC.dataBeg];

        Serial.print("Downlink | FPort=");
        Serial.print(port);
        Serial.print(" | CMD=");
        Serial.println(cmd, HEX);

        if (port == 2) {
          digitalWrite(CURTAIN_CMD_PIN, cmd ? HIGH : LOW);
        }

        if (port == 1) {
          digitalWrite(LED_PIN, cmd ? HIGH : LOW);
        }
      }

      os_setTimedCallback(
        &sendjob,
        os_getTime() + sec2osticks(60),
        do_send
      );
      break;
  }
}

// ================== SETUP ==================
void setup() {

  Serial.begin(115200);
  while (!Serial);

  dht.begin();

  pinMode(LED_PIN, OUTPUT);
  pinMode(VALVE_CMD_PIN, OUTPUT);
  pinMode(CURTAIN_CMD_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(VALVE_CMD_PIN, LOW);
  digitalWrite(CURTAIN_CMD_PIN, LOW);

  os_init();
  LMIC_reset();

  LMIC_selectSubBand(1);
  LMIC.dn2Freq = 923300000;
  LMIC.dn2Dr   = DR_SF12CR;

  LMIC_setClockError(MAX_CLOCK_ERROR * 10 / 100);
  LMIC_setDrTxpow(DR_SF10, 14);

  LMIC_startJoining();
}

// ================== LOOP ==================
void loop() {
  os_runloop_once();
}
