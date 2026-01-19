# 1. Resumen del proyecto

Este documento describe paso a paso la implementación de un nodo IoT basado en **LoRaWAN** utilizando la placa **Heltec LoRa32u4 v1.1**. El nodo mide nivel de luz (**LDR**), temperatura y humedad (**DHT11**) y detección por infrarrojo (sensor **IR emisor–receptor con salida digital**).  

Los datos se envían por **LoRaWAN mediante OTAA** a **The Things Network (TTN)** y luego se consumen por **MQTT**, integrándose en **Node-RED** para su visualización en un **dashboard**.

---

## 1.1 Alcance

El documento incluye:

- Hardware y sensores utilizados  
- Cableado y conexiones eléctricas  
- Configuración del **Arduino IDE**  
- Código del nodo LoRa  
- Configuración de **TTN** (aplicación, dispositivo y decoder)  
- Integración **MQTT**  
- Instalación y configuración de **Node-RED en Windows 11**  
- Creación del dashboard de visualización  

---

## 1.2 Arquitectura general

El flujo de datos del sistema es el siguiente:

Sensores → Heltec LoRa32u4 → LoRaWAN OTAA → TTN → MQTT → Node-RED → Dashboard


---

# 2. Materiales y componentes

Componentes principales del sistema:

- Placa **Heltec LoRa32u4 v1.1** (ATmega32u4 + SX1276)
- Sensor **LDR** (módulo o divisor resistivo) conectado a entrada analógica A0
- Sensor **DHT11** (3 pines: VCC, GND, DATA)
- Sensor **IR emisor–receptor** (3 pines: VCC, GND, OUT), tipo detección de obstáculo/barrera
- Cables dupont, protoboard, resistencia (si el LDR no viene en módulo) y fuente de alimentación
- PC con **Windows 11**, **Arduino IDE**, **Node.js** y **Node-RED**
- Gateway **LoRaWAN** compatible con **TTN** (US915 – Paraguay)

---

# 3. Conexiones eléctricas

Las conexiones propuestas a continuación están pensadas para no interferir con el módulo LoRa integrado en la placa.

| Dispositivo | Pin del sensor | Pin en Heltec LoRa32u4 v1.1 |
|------------|---------------|-----------------------------|
| LDR | OUT / Señal | A0 |
| LDR | VCC | 3.3 V o 5 V (según módulo) |
| LDR | GND | GND |
| DHT11 | DATA | D2 |
| DHT11 | VCC | 5 V (o 3.3 V si el módulo lo admite) |
| DHT11 | GND | GND |
| IR (3 pines) | OUT | D3 |
| IR (3 pines) | VCC | 5 V |
| IR (3 pines) | GND | GND |

---

## 3.1 Notas importantes

- Verificar el nivel lógico del pin **OUT** del sensor IR: muchos módulos entregan **LOW** cuando detectan un objeto.
- El sensor **DHT11** requiere un intervalo mínimo de lectura de **2 segundos**; se recomienda enviar datos por LoRaWAN cada **60 segundos o más** para evitar saturación y reducir consumo.
- Evitar el uso de pines reservados para el módulo LoRa, de acuerdo con el **pinmap configurado en LMIC**.

---

# 4. Heltec LoRa32u4 II

El **Heltec LoRa32u4 II** es una placa de desarrollo orientada a aplicaciones **IoT de largo alcance y bajo consumo**, que integra en un solo módulo un microcontrolador **ATmega32u4** y un transceptor **LoRa SX1276**, permitiendo implementar nodos LoRaWAN de forma simple y eficiente.

---

## 4.1 Microcontrolador

La placa está basada en el **ATmega32u4**, un microcontrolador de 8 bits de la familia **AVR**, ampliamente utilizado en plataformas compatibles con **Arduino**.  
Este microcontrolador incorpora **interfaz USB nativa**, lo que permite la programación directa desde el **Arduino IDE** sin necesidad de conversores externos.

**Características principales:**

- Arquitectura AVR de 8 bits  
- Frecuencia de operación: **16 MHz**  
- Memoria Flash: **32 KB**  
- SRAM: **2.5 KB**  
- EEPROM: **1 KB**

---

## 4.2 Transceptor LoRa

El módulo integra el **Semtech SX1276**, un transceptor de radio compatible con **LoRa y LoRaWAN**, diseñado para comunicaciones de largo alcance con muy bajo consumo energético.

**Características del SX1276:**

- Bandas de operación: **US902–928 MHz** (utilizada en Paraguay)
- Modulación **LoRa (Spread Spectrum)**
- Alta sensibilidad de recepción
- Soporte para distintos factores de expansión (**SF7–SF12**)

Este transceptor permite establecer enlaces confiables a varios kilómetros de distancia, dependiendo del entorno y la configuración del sistema.

---

## 4.3 Compatibilidad LoRaWAN

El Heltec LoRa32u4 II es totalmente compatible con **LoRaWAN**, pudiendo operar en los siguientes modos:

- **OTAA (Over-The-Air Activation)**
- **ABP (Activation By Personalization)**

En este proyecto se utiliza **OTAA**, gestionado mediante la librería **LMIC**, lo que permite la autenticación segura del dispositivo en **The Things Network (TTN)**.

---

## 4.4 Interfaz de sensores

La placa dispone de múltiples pines **GPIO digitales y analógicos**, lo que permite la conexión directa de sensores externos, tales como:

- Sensores analógicos (por ejemplo, LDR en A0)
- Sensores digitales (por ejemplo, DHT11 y sensor IR)
- Módulos adicionales mediante interfaces **SPI**, **I²C** o **UART**

---

## 4.5 Alimentación

El módulo puede alimentarse mediante:

- Conexión **USB**
- Fuente externa de **5 V**

La integración del SX1276 y el bajo consumo del ATmega32u4 lo hacen adecuado para aplicaciones alimentadas por batería.

---

## 4.6 Ventajas para proyectos IoT

- Integración de **microcontrolador y LoRa** en una sola placa
- Programación sencilla mediante **Arduino IDE**
- Compatibilidad directa con **TTN**
- Ideal para **prototipos y proyectos académicos**
- Amplio soporte comunitario

