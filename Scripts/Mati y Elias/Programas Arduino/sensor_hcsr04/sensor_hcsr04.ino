#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#define MQTT_MAX_PACKET_SIZE 512
#include <PubSubClient.h>

#include "sensor.h"

#define LED_ON  HIGH
#define LED_OFF LOW

const char* TOPIC_DIST_STATE = "arduino/ultrasonico/distancia";
const char* HA_DISCOVERY_TOPIC = "homeassistant/sensor/distancia_fisica_arduino/config";
const char* MQTT_CLIENT_ID = "esp8266_real_sensor";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// =======================================================
// CREDENCIALES WIFI
// =======================================================
const char* ssid = "EstudiantesCYT";
const char* password = "";  // red abierta, sin password
const int WIFI_COUNT = 5;

// =======================================================
// CONFIG MQTT
// =======================================================
String dynamicBroker = "";

const uint16_t MQTT_PORT  = 1883;

// =======================================================
// LED INDICADOR (GPIO2 → LED onboard)
// =======================================================
const int LED_PIN = 2;

// =======================================================
// VARIABLES DEFINIDAS
// =======================================================

unsigned long lastSensorRead = 0;
long distanciaCM = 0;

unsigned long lastPublishMs = 0;
const unsigned long PUBLISH_INTERVAL_MS = 10000;

// LED Blinking
unsigned long lastBlink = 0;
bool ledState = false;
const unsigned long BLINK_INTERVAL = 150;

const char* BROKER_IP = "10.20.2.60";  // IP real del Mosquitto

// =======================================================
// FUNCIONES
// =======================================================
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(500);

  Serial.println("\n[WiFi] Iniciando conexión...");
  WiFi.begin(ssid);  // como no hay password

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
      Serial.print(".");
      delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
      Serial.print("\n[WiFi] Conectado a ");
      Serial.println(ssid);
      Serial.print("[WiFi] IP: ");
      Serial.println(WiFi.localIP());
  } else {
      Serial.println("\n[WiFi] No se pudo conectar a la red.");
      delay(5000);
      connectWiFi();  // reintentar
  }
}


void publicarDiscoveryHA() {
  DynamicJsonDocument doc(512);
  doc["name"] = "Distancia Manual";
  doc["stat_t"] = TOPIC_DIST_STATE;
  doc["unit_of_meas"] = "cm";
  doc["uniq_id"] = "sensor_fisico_999";
  doc["val_tpl"] = "{{ value_json.distancia_cm }}";

  String payload;
  serializeJson(doc, payload);

  mqttClient.beginPublish(HA_DISCOVERY_TOPIC, payload.length(), true);
  mqttClient.print(payload);
  mqttClient.endPublish();
  
  Serial.println("[HA] Discovery enviado forzado");
}


void connectMQTT() {
    mqttClient.setServer(BROKER_IP, MQTT_PORT);

    while (!mqttClient.connected()) {
        Serial.print("[MQTT] Conectando... ");
        if (mqttClient.connect(MQTT_CLIENT_ID)) {
            Serial.println("¡CONECTADO!");
            // PUBLICAR DISCOVERY JUSTO AQUÍ
            publicarDiscoveryHA(); 
        } else {
            Serial.print("Error: ");
            Serial.println(mqttClient.state());
            delay(2000);
        }
    }
}




void publicarDistancia(long distancia) {
  StaticJsonDocument<128> doc;
  doc["distancia_cm"] = distancia; // Este nombre debe ser IGUAL al del template del discovery

  char payload[128];
  serializeJson(doc, payload);
  mqttClient.publish(TOPIC_DIST_STATE, payload, true);
}



// =======================================================
// SETUP
// =======================================================
void setup() {
  Serial.begin(115200);

  Serial.print("[ESP] MAC: ");
  Serial.println(WiFi.macAddress());

  Serial.println("\n[WiFi] Escaneando redes...");
  int n = WiFi.scanNetworks();

  if (n == 0) {
      Serial.println("[WiFi] No se encontraron redes.");
  } else {
      Serial.print("[WiFi] Redes encontradas: ");
      Serial.println(n);

      for (int i = 0; i < n; i++) {
          Serial.print("  - ");
          Serial.print(WiFi.SSID(i));
          Serial.print("  RSSI=");
          Serial.print(WiFi.RSSI(i));
          Serial.print("  CH=");
          Serial.println(WiFi.channel(i));
      }
  }


  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  connectWiFi();
  connectMQTT();
  //publicarDiscoveryHA(); 
  sensorInit();
}



// =======================================================
// LOOP
// =======================================================
void loop() {

  // =========================
  // LED (estado WiFi)
  // =========================
  if (WiFi.status() != WL_CONNECTED) {
    // Sin WiFi → LED fijo
    digitalWrite(LED_PIN, LED_OFF);
  } else {
    // Con WiFi → LED parpadea
    unsigned long nowLed = millis();
    if (nowLed - lastBlink >= BLINK_INTERVAL) {
      lastBlink = nowLed;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? LED_OFF : LED_ON);
    }
  }

  // =========================
  // MQTT
  // =========================
  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();

  // =========================
  // Publicar distancia
  // =========================
  unsigned long now = millis();

  if (now - lastPublishMs >= PUBLISH_INTERVAL_MS) {
    lastPublishMs = now;

    distanciaCM = medirDistanciaCM();

    if (distanciaCM >= 0) {
      Serial.print("[SENSOR] Distancia: ");
      Serial.print(distanciaCM);
      Serial.println(" cm");
    } else {
      Serial.println("[SENSOR] ERROR / TIMEOUT");
    }

    publicarDistancia(distanciaCM);
  }
}
