# Trabajo Práctico – Comunicaciones

## 1. Descripción general

Este trabajo práctico consiste en el diseño e implementación de una **red de sensores basada en LoRa**, orientada a la adquisición remota de variables ambientales y al control de actuadores desde una plataforma centralizada.

El sistema permite:

* Medir **humedad** y **nivel de luz**.
* Accionar **dos actuadores**:

  * Un **servo** que simula un motor de cortina.
  * Una **válvula** que se abre o cierra en función de la humedad.
* Visualizar datos en **Home Assistant**, **Grafana** e **InfluxDB**.
* Enviar comandos de control desde Home Assistant hacia el nodo LoRa mediante **downlinks**.

El enfoque es didáctico y apunta a comprender el flujo completo de datos en una arquitectura IoT real basada en LoRaWAN.

---

## 2. Arquitectura del sistema

### 2.1 Componentes principales

* **Nodo LoRa**

  * Lectura de sensores de humedad y luz.
  * Envío de datos vía LoRaWAN.
  * Recepción de comandos (downlinks) por puertos específicos.

* **Sensores**

  * Sensor de humedad.
  * Sensor de luz (LDR).

* **Actuadores**

  * Servo motor (cortina).
  * Válvula on/off controlada por humedad.

* **Arduino esclavo**

  * Controla los servos.
  * Recibe comandos desde el nodo LoRa.
  * Se utiliza porque el nodo LoRa:

    * No soporta simultáneamente librerías de **LMIC + Servo**.
    * No puede alimentar servos de forma segura.

* **Infraestructura IoT**

  * The Things Network (TTN).
  * MQTT.
  * Node-RED.
  * Home Assistant.
  * InfluxDB.
  * Grafana.

---

## 3. Flujo de datos (uplink)

El recorrido de los datos desde los sensores hasta la visualización es el siguiente:

1. El **nodo LoRa** lee los sensores de humedad y luz.
2. Los datos se envían por **LoRaWAN** hacia **The Things Network (TTN)**.
3. TTN publica automáticamente los mensajes en su **broker MQTT interno**.
4. Un **servidor MQTT local** se suscribe a TTN.
5. Los datos son consumidos por:

   * **Home Assistant** (automatización y control).
   * **Telegraf → InfluxDB** (almacenamiento histórico).
   * **Grafana** (visualización).

---

## 4. Flujo de control (downlink)

### 4.1 Problema encontrado

El objetivo inicial era enviar comandos directamente desde **Home Assistant → MQTT → TTN**. Sin embargo, surgieron dos problemas:

1. **Incompatibilidad de versiones** entre:

   * El broker MQTT de TTN.
   * La integración MQTT utilizada por Home Assistant.

2. **Limitaciones del bridge MQTT**:

   * TTN v3 utiliza autenticación, topics y estructura diferentes a un broker MQTT clásico.
   * Las versiones de Home Assistant y Mosquitto utilizadas no permitían un bridge limpio y estable.

Debido a esto, no fue posible implementar un flujo directo Home Assistant → TTN mediante MQTT.

### 4.2 Solución adoptada: Node-RED

Se utilizó **Node-RED** como capa intermedia para la comunicación de retorno:

1. Home Assistant envía el comando a Node-RED.
2. Node-RED formatea el mensaje según el esquema requerido por TTN.
3. Node-RED publica el **downlink** en TTN vía MQTT.
4. TTN envía el downlink al nodo LoRa.

Esta solución resultó:

* Simple.
* Funcional.
* Fácil de depurar.

---

## 5. Downlinks y control de actuadores

Los comandos de control se envían mediante **downlinks** usando distintos puertos:

* **Puerto 1**: control del servo (cortina).
* **Puerto 2**: control de la válvula.

El nodo LoRa:

1. Recibe el downlink.
2. Interpreta el puerto.
3. Envía el comando correspondiente al **Arduino esclavo**.

El Arduino:

* Maneja los servos.
* Ejecuta la acción solicitada (abrir/cerrar cortina o válvula).

---

## 6. Justificación del diseño

### 6.1 Separación LoRa – Arduino

Se decidió separar funciones por las siguientes razones:

* El nodo LoRa:

  * Tiene memoria limitada.
  * No soporta librerías de servo junto con LMIC.
  * No puede alimentar servos directamente.

* El Arduino:

  * Maneja sin problemas librerías de servo.
  * Permite una lógica de control más flexible.

Esta separación mejora:

* Estabilidad.
* Claridad del diseño.
* Facilidad de depuración.

---

## 7. Selección de hardware y justificación

En esta sección se justifica la elección del hardware utilizado para el nodo LoRa del sistema, comparando dos plataformas disponibles: **LoRa32u4 II** y **Heltec WiFi LoRa 32**. La decisión se basa en criterios de compatibilidad con LoRaWAN, estabilidad, complejidad del sistema y adecuación al uso con TTN.

---

### 7.1 LoRa32u4 II

La placa **LoRa32u4 II** integra un microcontrolador **ATmega32u4** junto con un transceptor LoRa (SX1276/SX1278). Su arquitectura es simple y está orientada a aplicaciones LoRaWAN clásicas.

Características relevantes:

* Microcontrolador de 8 bits sin sistema operativo.
* Arquitectura *bare-metal*, con control determinístico del tiempo.
* Compatibilidad directa con el stack **LMIC**, ampliamente utilizado con TTN.
* Consumo energético reducido.
* Alta estabilidad en procesos de *join*, uplinks y downlinks.

Esta placa se ajusta al modelo de funcionamiento para el cual fue diseñado LMIC, lo que resulta en una implementación confiable y predecible de LoRaWAN.

---

### 7.2 Heltec WiFi LoRa 32

La **Heltec WiFi LoRa 32** combina un microcontrolador **ESP32** con conectividad WiFi, Bluetooth y un transceptor LoRa. Si bien ofrece mayor potencia de procesamiento, su arquitectura introduce una complejidad considerable.

Características relevantes:

* Microcontrolador de 32 bits con **FreeRTOS**.
* Ejecución concurrente de múltiples tareas.
* Presencia de stacks WiFi y Bluetooth activos.
* Uso de abstracciones de alto nivel y múltiples interrupciones.

Estas características interfieren con los requisitos temporales estrictos del protocolo LoRaWAN, especialmente en lo relativo a las ventanas de recepción RX1 y RX2.

---

### 7.3 Limitaciones de la Heltec WiFi LoRa 32 con TTN

La ausencia de una librería estable y oficial para implementar LoRaWAN con TTN en la Heltec WiFi LoRa 32 no se debe a una falta de desarrollo, sino a incompatibilidades estructurales:

* El stack LMIC fue diseñado para microcontroladores simples y con control exclusivo del tiempo.
* El uso de FreeRTOS introduce latencias no determinísticas.
* El stack WiFi del ESP32 puede bloquear interrupciones y temporizadores críticos.
* Pequeños desvíos temporales provocan fallos en el *join* o pérdida de downlinks.

Como consecuencia, las implementaciones LoRaWAN sobre ESP32 presentan comportamientos erráticos y baja confiabilidad cuando se utilizan con TTN.

---

### 7.4 Comparación entre plataformas

| Característica                    | LoRa32u4 II        | Heltec WiFi LoRa 32 |
| --------------------------------- | ------------------ | ------------------- |
| Microcontrolador                  | ATmega32u4         | ESP32               |
| Arquitectura                      | Simple, bare-metal | Compleja, RTOS      |
| WiFi / Bluetooth                  | No                 | Sí                  |
| Compatibilidad con LMIC           | Alta               | Baja / inestable    |
| Estabilidad con TTN               | Alta               | Variable            |
| Consumo energético                | Bajo               | Alto                |
| Adecuación para LoRaWAN académico | Alta               | Limitada            |

---

### 7.5 Justificación de la elección

Aunque la Heltec WiFi LoRa 32 ofrece mayores prestaciones, su complejidad arquitectónica dificulta una implementación estable de LoRaWAN con TTN. En cambio, la LoRa32u4 II proporciona un entorno controlado y compatible con los supuestos del stack LMIC, resultando más adecuada para un trabajo práctico académico centrado en comunicaciones y redes LoRaWAN.

Por estas razones, se optó por utilizar la **LoRa32u4 II** como nodo LoRa del sistema.

## 8. Servicios utilizados

### 7.1 Mosquitto (MQTT)

```bash
sudo apt update
sudo apt install -y mosquitto mosquitto-clients
sudo systemctl enable mosquitto
sudo systemctl start mosquitto
```

### 7.2 Node-RED

```bash
sudo apt install -y nodejs npm
sudo npm install -g --unsafe-perm node-red
node-red-start
```

### 7.3 InfluxDB

```bash
docker run -d \
  --name influxdb \
  -p 8086:8086 \
  influxdb:2.7
```

### 7.4 Grafana

```bash
docker run -d \
  --name grafana \
  -p 3000:3000 \
  grafana/grafana
```

### 7.5 Home Assistant

```bash
docker run -d \
  --name homeassistant \
  --network=host \
  ghcr.io/home-assistant/home-assistant:stable
```

---

## 8. Implementación de The Things Network (TTN)

Esta sección describe paso a paso la implementación del backend LoRaWAN utilizando **The Things Network (TTN)**, incluyendo la creación de la aplicación, el registro del dispositivo y la integración mediante MQTT.

### 8.1 Creación de cuenta y acceso a la consola

1. Acceder a la consola de TTN:

   ```
   https://console.cloud.thethings.network
   ```
2. Iniciar sesión o crear una cuenta.
3. Seleccionar el **cluster** correspondiente a la región (por ejemplo, `eu1` o `nam1`).

> El cluster elegido define el endpoint MQTT que se utilizará posteriormente.

---

### 8.2 Creación de la Application

1. Ir a **Applications → Create application**.
2. Definir:

   * **Application ID** (ejemplo: `sensores-lora`).
   * Descripción libre.
3. Crear la aplicación.

La Application actúa como contenedor lógico de todos los nodos LoRa asociados al sistema.

---

### 8.3 Registro del End Device (Nodo LoRa)

1. Dentro de la Application, ir a **End devices → Add end device**.
2. Seleccionar *Enter end device specifics manually*.
3. Configurar:

   * Plan de frecuencia según la región.
   * Versión de LoRaWAN compatible con el stack LMIC utilizado.
   * **Device ID** (ejemplo: `nodo-humedad-luz`).

Se utilizó el método de activación **OTAA (Over-The-Air Activation)** por razones de seguridad y escalabilidad.

---

### 8.4 Credenciales OTAA

Al registrar el dispositivo, TTN genera automáticamente:

* **DevEUI**
* **JoinEUI / AppEUI**
* **AppKey**

Estas credenciales deben ser cargadas en el firmware del nodo LoRa. Cualquier error en estos valores impide el proceso de *join* a la red.

---

### 8.5 Payload Formatter (Uplink)

Para facilitar la visualización de los datos en la consola de TTN:

1. Ir a **Payload formatters → Uplink**.
2. Seleccionar **Custom JavaScript**.
3. Implementar un parser simple para los bytes recibidos, por ejemplo:

```js
return {
  humedad: bytes[0],
  luz: bytes[1]
};
```

Este formateo es únicamente para depuración y visualización; no afecta el contenido del mensaje MQTT.

---

### 8.6 Creación de API Key para MQTT

1. En la Application, ir a **API keys → Add API key**.
2. Habilitar los permisos:

   * Read application traffic.
   * Write downlink traffic.
3. Guardar la API Key generada.

La API Key se utiliza como contraseña para acceder al broker MQTT de TTN.

---

### 8.7 Acceso MQTT desde la computadora (uplink)

Con las credenciales configuradas, es posible consumir uplinks directamente desde una computadora utilizando `mosquitto_sub`:

```bash
mosquitto_sub \
  -h eu1.cloud.thethings.network \
  -p 1883 \
  -u sensores-lora@ttn \
  -P NNSXS.XXXXXXXXXXXX \
  -t 'v3/sensores-lora@ttn/devices/+/up'
```

Si se reciben mensajes en formato JSON, la integración con TTN es correcta.

---

### 8.8 Envío de downlinks y uso de puertos

Los comandos hacia el nodo se envían mediante **downlinks**, utilizando distintos puertos LoRa como canales lógicos:

* `f_port = 1`: control del servo (cortina).
* `f_port = 2`: control de la válvula.

Ejemplo de envío de downlink desde la computadora:

```bash
mosquitto_pub \
  -h eu1.cloud.thethings.network \
  -u sensores-lora@ttn \
  -P NNSXS.XXXXXXXX \
  -t 'v3/sensores-lora@ttn/devices/nodo-humedad-luz/down/push' \
  -m '{
    "downlinks": [{
      "f_port": 1,
      "frm_payload": "AQ==",
      "priority": "NORMAL"
    }]
  }'
```

El downlink es recibido por el nodo únicamente cuando este transmite y abre su ventana de recepción.

---

### 8.9 Integración con infraestructura local

TTN no se integra como un broker MQTT clásico. En su lugar, los servicios locales (Node-RED, Telegraf, scripts) actúan como **clientes MQTT de TTN**, consumiendo uplinks y republicándolos en el broker local.

Esta arquitectura evita el uso de bridges MQTT bidireccionales y asegura compatibilidad con el backend LoRaWAN.

---

## 9. Conclusiones

Este trabajo permitió comprender:

* El funcionamiento real de una red LoRaWAN.
* La integración de TTN con MQTT.
* El uso de Node-RED como middleware para downlinks.
* Las limitaciones prácticas del hardware.
* La importancia de separar responsabilidades entre dispositivos.

Además, la arquitectura implementada deja abierta la posibilidad de **integrar interfaces externas adicionales**, ampliando las capacidades del sistema sin modificar el nodo LoRa.

Entre las integraciones posibles se destaca **Telegram**, que puede utilizarse como:

* Canal de **notificaciones** (alertas de humedad baja, estado de válvula o cortina).
* Interfaz de **control remoto**, permitiendo enviar comandos simples que luego son procesados por Home Assistant o Node-RED.

Esta integración se realiza a nivel de software (Home Assistant o Node-RED), manteniendo a TTN como backend LoRaWAN y evitando exponer directamente el nodo a servicios externos. De esta forma, el sistema conserva su modularidad, seguridad y escalabilidad.

La arquitectura propuesta permite incorporar nuevas interfaces de usuario o servicios de visualización sin alterar el funcionamiento básico de la red LoRaWAN.

---

## 10. Notas finales

Este repositorio contiene:

* Código del nodo LoRa.
* Código del Arduino esclavo.

Cualquier mejora futura puede realizarse sobre esta arquitectura base.
