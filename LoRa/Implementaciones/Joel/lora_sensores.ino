#define CFG_us915 1
#define CFG_sx1276 1
#define LMIC_FAILURE_TO_SERIAL 0
#define LMIC_USE_INTERRUPTS 0

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <DHT.h>

#define LDR_PIN A0
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define IR_PIN 3   // pin OUT del sensor IR


// APPEUI/DEVEUI LSB (invertidos), APPKEY normal
static const u1_t PROGMEM APPEUI[8]  = { 0,0,0,0,0,0,0,0 };
static const u1_t PROGMEM DEVEUI[8]  = { 0xBD,0x4D,0x07,0xD0,0x7E,0xD5,0xB3,0x70 };
static const u1_t PROGMEM APPKEY[16] = { 0x0D,0x70,0x5F,0x54,0xA6,0x4F,0xAA,0x4F,0xE6,0xDB,0x12,0xFC,0xF4,0xA6,0xD9,0x49 };

void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8); }
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8); }
void os_getDevKey (u1_t* buf) { memcpy_P(buf, APPKEY, 16); }

// Si NO te une, probá DIO1=6 o DIO1=3 cambiando el segundo valor
const lmic_pinmap lmic_pins = {
  .nss  = 8,
  .rxtx = LMIC_UNUSED_PIN,
  .rst  = 4,
  .dio  = {7, 5, LMIC_UNUSED_PIN}
};

static osjob_t sendjob;
const unsigned TX_INTERVAL = 10;

void do_send(osjob_t*) {
  if (LMIC.opmode & OP_TXRXPEND) return;

  int raw = analogRead(LDR_PIN);
  uint8_t luz = (uint8_t)constrain(raw / 10, 0, 255);
  uint8_t irState = digitalRead(IR_PIN);  // 0 = detectado, 1 = libre (normalmente)


  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!(h == h) || !(t == t)) { h = 0; t = 0; } // NaN check sin isnan()

  int16_t  t100 = (int16_t)(t * 100.0f);
  uint16_t h100 = (uint16_t)(h * 100.0f);

 // Payload: [LDR][IR][T_hi][T_lo][H_hi][H_lo]
uint8_t payload[6];

payload[0] = luz;
payload[1] = irState;   // 👈 nuevo
payload[2] = (t100 >> 8) & 0xFF;
payload[3] = t100 & 0xFF;
payload[4] = (h100 >> 8) & 0xFF;
payload[5] = h100 & 0xFF;

LMIC_setTxData2(1, payload, sizeof(payload), 0);
}

void onEvent(ev_t ev) {
  if (ev == EV_JOINED) {
    LMIC_setLinkCheckMode(0);
    do_send(&sendjob);
  } else if (ev == EV_TXCOMPLETE) {
    os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
  }
}

void setup() {
  delay(3000);
  Serial.begin(9600);
  pinMode(IR_PIN, INPUT);

  dht.begin();
  os_init();
  LMIC_reset();

  LMIC_selectSubBand(1);            // sub-band 2
  LMIC_setDrTxpow(DR_SF7, 14);
  LMIC_setClockError(MAX_CLOCK_ERROR * 10 / 100);

  LMIC_startJoining();
}

void loop() {
  os_runloop_once();
}
