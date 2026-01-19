1. Resumen del proyecto
Este documento describe paso a paso la implementación de un nodo IoT basado en LoRaWAN utilizando la placa Heltec LoRa32u4 v1.1. El nodo mide nivel de luz (LDR), temperatura y humedad (DHT11) y detección por infrarrojo (emisor–receptor con salida digital). Los datos se envían por LoRaWAN mediante OTAA a The Things Network (TTN) y luego se consumen por MQTT e integración en Node-RED para visualización en dashboard.

1.1 Alcance
Incluye: hardware, cableado, configuración de Arduino IDE, código, TTN (aplicación + dispositivo + decoder), MQTT, Node-RED (instalación en Windows 11) y dashboard.

1.2 Arquitectura general
Flujo de datos: Sensores → Heltec LoRa32u4 → LoRaWAN OTAA → TTN → MQTT → Node-RED → Dashboard

2. Materiales y componentes
Componentes principales:
•	Placa Heltec LoRa32u4 v1.1 (ATmega32u4 + SX1276).
•	Sensor LDR (módulo o divisor resistivo) conectado a entrada analógica A0.
•	Sensor DHT11 (3 pines: VCC, GND, DATA).
•	Sensor IR emisor–receptor (3 pines: VCC, GND, OUT) – tipo detección de obstáculo/barrera.
•	Cables dupont, protoboard, resistencia (si el LDR no viene en módulo) y fuente/alimentación.
•	PC con Windows 11 + Arduino IDE + Node.js + Node-RED.
•	Gateway LoRaWAN compatible con TTN (US915 en Paraguay).

3. Conexiones eléctricas
Las conexiones propuestas a continuación están pensadas para no interferir con el módulo LoRa 
Dispositivo	Pin del sensor	Pin en Heltec LoRa32u4 v1.1
LDR	OUT/Señal	A0
LDR	VCC	3.3V o 5V (según módulo)
LDR	GND	GND
DHT11	DATA	D2
DHT11	VCC	5V (o 3.3V si el módulo lo admite)
DHT11	GND	GND
IR (3 pines)	OUT	D3
IR (3 pines)	VCC	5V
IR (3 pines)	GND	GND

3.1 Notas importantes
• Verificar el nivel lógico del pin OUT del IR: muchos módulos entregan LOW cuando detectan objeto.
• El DHT11 requiere un intervalo de lectura ≥ 2 s; se recomienda enviar por LoRaWAN cada 60 s o más para evitar saturación y reducir consumo.
• Evita usar pines reservados para LoRa (según el pinmap configurado en LMIC).
