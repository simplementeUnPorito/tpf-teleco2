#include "dispensador.h"
#include "sensor.h"

// =======================================================
// CONFIG TELEGRAM
// =======================================================
const char* TELEGRAM_TOKEN = "8533984322:AAE_KpqttUtisF1u5KsNg2Z4rDfHNLN4K6g";
String AUTH_CHAT = "6227838236";

// =======================================================
// INSTANCIAS
// =======================================================
WiFiClientSecure secured_client;
UniversalTelegramBot bot(TELEGRAM_TOKEN, secured_client);

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// =======================================================
// LISTA ORDENADA DE REDES WIFI
// =======================================================
WifiNetwork wifiList[] = {
  { "Junh0", "junh0erick" },
  { "VirusC", "qwepoi123098" },
  { "EstudiantesCYT", "" }
};

const int WIFI_COUNT = sizeof(wifiList) / sizeof(wifiList[0]);

// =======================================================
// CONFIG MQTT
// =======================================================
String dynamicBroker = "";

const uint16_t MQTT_PORT  = 1883;
const char* MQTT_CLIENT_ID = "esp8266_mqtt_test";

const char* TOPIC_P1 = "perfume/1/status";
const char* TOPIC_P2 = "perfume/2/status";
const char* TOPIC_P3 = "perfume/3/status";

const char* CMD_P1 = "perfume/1/cmd";
const char* CMD_P2 = "perfume/2/cmd";
const char* CMD_P3 = "perfume/3/cmd";

// =======================================================
// LED INDICADOR (GPIO2 → LED onboard)
// =======================================================
const int LED_PIN = 2;

// =======================================================
// VARIABLES DEFINIDAS
// =======================================================
int nivel1 = 100;
int nivel2 = 100;
int nivel3 = 100;

unsigned long disp1 = 0;
unsigned long disp2 = 0;
unsigned long disp3 = 0;

unsigned long lastSensorRead = 0;
long distanciaCM = 0;

bool sync1 = false;
bool sync2 = false;
bool sync3 = false;

unsigned long lastPublishMs = 0;
const unsigned long PUBLISH_INTERVAL_MS = 10000;

unsigned long lastTelegramCheck = 0;
const int TELEGRAM_INTERVAL = 800;

// LED Blinking
unsigned long lastBlink = 0;
bool ledState = false;
const unsigned long BLINK_INTERVAL = 150;


// =======================================================
// FUNCIONES
// =======================================================
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);   // fuerza borrado de redes antiguas
  delay(500);

  Serial.println("\n[WiFi] Iniciando conexión...");

  for (int i = 0; i < WIFI_COUNT; i++) {

    Serial.print("[WiFi] Intentando conectar a: ");
    Serial.println(wifiList[i].ssid);

    if (strlen(wifiList[i].password) == 0)
      WiFi.begin(wifiList[i].ssid);
    else
      WiFi.begin(wifiList[i].ssid, wifiList[i].password);

    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - start < 5000) {
      Serial.print(".");
      delay(250);
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("[WiFi] Conectado a ");
      Serial.println(wifiList[i].ssid);

      Serial.print("[WiFi] IP: ");
      Serial.println(WiFi.localIP());
      return;
    }

    Serial.print("[WiFi] Falló conexión con ");
    Serial.println(wifiList[i].ssid);
  }

  Serial.println("[WiFi] ❌ No se pudo conectar a ninguna red.");
  Serial.println("[WiFi] Reintentando en 5 segundos...");
  delay(5000);

  connectWiFi();
}

void seleccionarBroker() {

  IPAddress ip = WiFi.localIP();
  IPAddress gw = WiFi.gatewayIP();

  Serial.println("====== DEBUG RED ======");
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("IP: "); Serial.println(ip);
  Serial.print("Gateway: "); Serial.println(gw);
  Serial.println("=======================");

  // ===========================
  // CASA → VirusC (192.168.100.x)
  // ===========================
  if (gw[0] == 192 && gw[1] == 168 && gw[2] == 100) {
    dynamicBroker = "192.168.100.68";
    Serial.println("[BROKER] Red de casa detectada → broker local");
    return;
  }

  // ===========================
  // UCA → EstudiantesCYT (NO dependas del GW 10.20 vs 10.30)
  // ===========================
  if (WiFi.SSID() == "EstudiantesCYT") {
    dynamicBroker = "10.20.2.60";
    Serial.println("[BROKER] EstudiantesCYT → broker 10.20.2.60");
    return;
  }

  // ===========================
  // HOTSPOT (Junh0 / otros) → usar gateway como broker (más realista)
  // ===========================
  if (gw[0] == 172 || gw[0] == 192) {
    dynamicBroker = gw.toString();
    Serial.print("[BROKER] Hotspot detectado → broker en gateway ");
    Serial.println(dynamicBroker);
    return;
  }

  // ===========================
  // DESCONOCIDA
  // ===========================
  dynamicBroker = "";
  Serial.println("[BROKER] Red desconocida → MQTT deshabilitado");
}





void publicarNivel(int perfumeID, int nivel) {
  String payload = "{";
  payload += "\"nivel\":" + String(nivel) + ",";
  payload += "\"msg\":\"ok\"";
  payload += "}";

  const char* topic =
      (perfumeID == 1) ? TOPIC_P1 :
      (perfumeID == 2) ? TOPIC_P2 :
                         TOPIC_P3;

  mqttClient.publish(topic, payload.c_str());

  Serial.print("[MQTT] Publicado en ");
  Serial.print(topic);
  Serial.print(" => ");
  Serial.println(payload);
}

void publicarDispensado(int perfumeID, unsigned long count) {
  String payload = "{\"dispensado\":" + String(count) + "}";
  const char* topic =
      (perfumeID == 1) ? CMD_P1 :
      (perfumeID == 2) ? CMD_P2 :
                         CMD_P3;

  mqttClient.publish(topic, payload.c_str(), true);

  Serial.print("[MQTT DISP] ");
  Serial.print(topic);
  Serial.print(" => ");
  Serial.println(payload);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("[MQTT IN] "); 
  Serial.print(topic);
  Serial.print(" => "); 
  Serial.println(msg);

  StaticJsonDocument<128> doc;
  if (deserializeJson(doc, msg)) {
    Serial.println("[MQTT IN] JSON inválido");
    return;
  }

  if (String(topic) == CMD_P1 && doc.containsKey("dispensado")) {
    disp1 = doc["dispensado"];
    sync1 = true;
  }

  if (String(topic) == CMD_P2 && doc.containsKey("dispensado")) {
    disp2 = doc["dispensado"];
    sync2 = true;
  }

  if (String(topic) == CMD_P3 && doc.containsKey("dispensado")) {
    disp3 = doc["dispensado"];
    sync3 = true;
  }
}

void connectMQTT() {

  if (dynamicBroker == "") {
    Serial.println("[MQTT] MQTT deshabilitado en esta red.");
    return;
  }

  mqttClient.setServer(dynamicBroker.c_str(), MQTT_PORT);
  mqttClient.setCallback(mqttCallback);

  while (!mqttClient.connected()) {
    Serial.print("[MQTT] Conectando a ");
    Serial.print(dynamicBroker);
    Serial.print(" ... ");

    if (mqttClient.connect(MQTT_CLIENT_ID)) {
      Serial.println("OK");

      mqttClient.subscribe(CMD_P1);
      mqttClient.subscribe(CMD_P2);
      mqttClient.subscribe(CMD_P3);
    } else {
      Serial.print("Fallo rc=");
      Serial.print(mqttClient.state());
      Serial.println(" — reintentando...");
      delay(1000);
    }
  }
}


void esperarSincronizacionTelegram() {
  bot.sendMessage(AUTH_CHAT,
                  "⏳ *Sincronizando datos del dispensador...*",
                  "Markdown");

  Serial.println("[SYNC] Esperando retained de MQTT...");

  unsigned long start = millis();

  while (!(sync1 && sync2 && sync3)) {
    mqttClient.loop();
    yield();

    if (millis() - start > 7000) {
      bot.sendMessage(AUTH_CHAT,
                      "⚠️ *Advertencia:* No se recibieron todos los contadores retained.\n"
                      "Se continuará con los valores disponibles.",
                      "Markdown");
      break;
    }
  }

  bot.sendMessage(AUTH_CHAT, "✅ *Sincronización completa!*", "Markdown");
}

void simularDispensar(int perfumeID) {
  Serial.print("[SIM] Dispensando perfume ");
  Serial.println(perfumeID);

  if (perfumeID == 1) {
    disp1++;
    publicarDispensado(1, disp1);
    bot.sendMessage(AUTH_CHAT,
      "✨ Perfume 1 dispensado correctamente.\n"
      "Total acumulado: " + String(disp1) + " veces.");
  } 
  else if (perfumeID == 2) {
    disp2++;
    publicarDispensado(2, disp2);
    bot.sendMessage(AUTH_CHAT,
      "✨ Perfume 2 dispensado correctamente.\n"
      "Total acumulado: " + String(disp2) + " veces.");
  } 
  else if (perfumeID == 3) {
    disp3++;
    publicarDispensado(3, disp3);
    bot.sendMessage(AUTH_CHAT,
      "✨ Perfume 3 dispensado correctamente.\n"
      "Total acumulado: " + String(disp3) + " veces.");
  }
}

void procesarTelegram() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  while (numNewMessages) {

    for (int i = 0; i < numNewMessages; i++) {
      String chat_id = bot.messages[i].chat_id;
      String text    = bot.messages[i].text;

      if (chat_id != AUTH_CHAT) {
        bot.sendMessage(chat_id, "❌ No autorizado.");
        continue;
      }

      if (text == "/r1") {
          disp1 = 0;
          publicarDispensado(1, disp1);

          bot.sendMessage(chat_id,
            "🔄 Perfume 1 reseteado correctamente.\n"
            "Valor actual: 0"
          );
      }
      else if (text == "/r2") {
          disp2 = 0;
          publicarDispensado(2, disp2);

          bot.sendMessage(chat_id,
            "🔄 Perfume 2 reseteado correctamente.\n"
            "Valor actual: 0"
          );
      }
      else if (text == "/r3") {
          disp3 = 0;
          publicarDispensado(3, disp3);

          bot.sendMessage(chat_id,
            "🔄 Perfume 3 reseteado correctamente.\n"
            "Valor actual: 0"
          );
      }


      else if (text == "/resetall") {

        disp1 = 0;
        disp2 = 0;
        disp3 = 0;

        publicarDispensado(1, 0);
        publicarDispensado(2, 0);
        publicarDispensado(3, 0);

        bot.sendMessage(chat_id,
          "🧹 *Reset global realizado*\n"
          "Todos los contadores fueron puestos a 0.",
          "Markdown"
        );
      }


      else if (text == "/p1") simularDispensar(1);
      else if (text == "/p2") simularDispensar(2);
      else if (text == "/p3") simularDispensar(3);

      else if (text == "/niveles") {
        String msg =
          "📊 *Niveles actuales:*\n"
          "Perfume 1: " + String(nivel1) + "%\n"
          "Perfume 2: " + String(nivel2) + "%\n"
          "Perfume 3: " + String(nivel3) + "%\n";
        bot.sendMessage(chat_id, msg, "Markdown");
      }

      else if (text == "/stats") {
        String msg =
          "📈 *Estadísticas de uso:*\n"
          "P1 dispensado: " + String(disp1) + " veces\n"
          "P2 dispensado: " + String(disp2) + " veces\n"
          "P3 dispensado: " + String(disp3) + " veces\n";
        bot.sendMessage(chat_id, msg, "Markdown");
      }

      else if (text == "/comandos") {
        bot.sendMessage(
          chat_id,
          "Comandos disponibles:\n"
          "/p1 /p2 /p3  → dispensar\n"
          "/niveles     → ver niveles\n"
          "/stats       → ver estadísticas\n"
          "/r1 /r2 /r3  → reset de un perfume\n"
          "/resetall    → reset global\n"
          "/comandos    → mostrar esta lista"
        );
      }
    }

    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }
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
      Serial.println("[WiFi] ❌ No se encontraron redes.");
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

  secured_client.setInsecure();
  secured_client.setTimeout(1500);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  connectWiFi();
  seleccionarBroker();
  connectMQTT();

  sensorInit();

  bot.sendMessage(AUTH_CHAT,
                  "🤖 *Iniciando dispensador...*",
                  "Markdown");

  esperarSincronizacionTelegram();

  bot.sendMessage(
      AUTH_CHAT,
      "🤖 *Dispensador conectado y listo para usar*",
      "Markdown"
  );

  bot.sendMessage(
      AUTH_CHAT,
      "📋 *Comandos disponibles:*\n"
      "/p1 /p2 /p3  → dispensar\n"
      "/niveles     → ver niveles\n"
      "/stats       → ver estadísticas\n"
      "/r1 /r2 /r3  → reset de un perfume\n"
      "/resetall    → reset global\n"
      "/comandos    → mostrar esta lista",
      "Markdown"
  );
}

// =======================================================
// LOOP
// =======================================================
void loop() {

  long d = medirDistanciaCM();

  if (d > 0) {
    Serial.print("[SENSOR] ");
    Serial.print(d);
    Serial.println(" cm");
  } else {
    Serial.println("[SENSOR] ERROR / TIMEOUT");
  }

  delay(500);

  // LED siempre se ejecuta, no importa si MQTT o Telegram están lentos
  if (WiFi.status() != WL_CONNECTED) {
      // 🔴 Sin WiFi → LED fijo encendido
      digitalWrite(LED_PIN, LED_OFF);
  } else {
      // 🔵 Con WiFi → LED titila RÁPIDO y constante
      unsigned long now = millis();
      if (now - lastBlink >= BLINK_INTERVAL) {
          lastBlink = now;
          ledState = !ledState;
          digitalWrite(LED_PIN, ledState ? LED_OFF : LED_ON);
      }
  }


  if (!mqttClient.connected())
    connectMQTT();

  mqttClient.loop();

  unsigned long now = millis();

  if (now - lastPublishMs >= PUBLISH_INTERVAL_MS) {
    lastPublishMs = now;

    nivel1--; if (nivel1 <= 0) nivel1 = 100;
    nivel2 -= 2; if (nivel2 <= 0) nivel2 = 100;
    nivel3 -= 3; if (nivel3 <= 0) nivel3 = 100;

    publicarNivel(1, nivel1);
    publicarNivel(2, nivel2);
    publicarNivel(3, nivel3);
  }

  if (now - lastTelegramCheck >= TELEGRAM_INTERVAL) {
    lastTelegramCheck = now;
    procesarTelegram();
  }
}
