#ifndef DISPENSADOR_H
#define DISPENSADOR_H

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <PubSubClient.h>

struct WifiNetwork {
  const char* ssid;
  const char* password;
};

extern WifiNetwork wifiList[];
extern const int WIFI_COUNT;

// LED_ON / LED_OFF para módulos con lógica invertida
#define LED_ON  HIGH   // LED apagado
#define LED_OFF LOW    // LED encendido

extern WiFiClientSecure secured_client;
extern UniversalTelegramBot bot;

extern WiFiClient wifiClient;
extern PubSubClient mqttClient;

// =======================================================
// VARIABLES
// =======================================================
extern int  nivel1;
extern int  nivel2;
extern int  nivel3;

extern unsigned long disp1;
extern unsigned long disp2;
extern unsigned long disp3;

extern bool sync1;
extern bool sync2;
extern bool sync3;

extern unsigned long lastPublishMs;
extern const unsigned long PUBLISH_INTERVAL_MS;

extern unsigned long lastTelegramCheck;
extern const int TELEGRAM_INTERVAL;

// Blinking
extern unsigned long lastBlink;
extern bool ledState;
extern const unsigned long BLINK_INTERVAL;

// =======================================================
// DECLARACIÓN DE FUNCIONES
// =======================================================
void connectWiFi();
void publicarNivel(int perfumeID, int nivel);
void publicarDispensado(int perfumeID, unsigned long count);
void mqttCallback(char* topic, byte* payload, unsigned int length);
void connectMQTT();
void esperarSincronizacionTelegram();
void simularDispensar(int perfumeID);
void procesarTelegram();

#endif
