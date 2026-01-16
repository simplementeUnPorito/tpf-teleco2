# Trabajo Práctico – Telecomunicaciones II

## 1. Descripción general

Este trabajo práctico consiste en el diseño e implementación de una **red de sensores basada en LoRa**, orientada a la adquisición remota de variables ambientales y al control de actuadores desde una plataforma centralizada.

El sistema permite:
- Medir **humedad** (sensor **DTH11**) y **nivel de luz**(sensor**LDR**).
- Accionar **dos actuadores**:
  - Un **servo** que simula un motor de cortina.
  - Una **válvula** que se abre o cierra en función de la humedad.
- Visualizar datos en **Home Assistant**, **Grafana** e **InfluxDB**.
- Enviar comandos de control desde Home Assistant hacia el nodo LoRa mediante **downlinks**.

El enfoque es didáctico y apunta a comprender el flujo completo de datos en una arquitectura IoT real basada en LoRaWAN.

---

## 2. Arquitectura del sistema

### 2.1 Componentes principales

- **Nodo LoRa**
  - Lectura de sensores de humedad y luz.
  - Envío de datos vía LoRaWAN.
  - Recepción de comandos (downlinks) por puertos específicos.

- **Sensores**
  - Sensor de humedad.
  - Sensor de luz (LDR).

- **Actuadores**
  - Servo motor (cortina).
  - Válvula on/off controlada por humedad.

- **Arduino esclavo**
  - Controla los servos.
  - Recibe comandos desde el nodo LoRa.
  - Se utiliza porque el nodo LoRa:
    - No soporta simultáneamente librerías de **LMIC + Servo**.
    - No puede alimentar servos de forma segura.

- **Infraestructura IoT**
  - The Things Network (TTN).
  - MQTT.
  - Node-RED.
  - Home Assistant.
  - InfluxDB.
  - Grafana.

---

## 3. Flujo de datos (uplink)

El recorrido de los datos desde los sensores hasta la visualización es el siguiente:

1. El **nodo LoRa** lee los sensores de humedad y luz.
2. Los datos se envían por **LoRaWAN** hacia **The Things Network (TTN)**.
3. TTN publica automáticamente los mensajes en su **broker MQTT interno**.
4. Un **servidor MQTT local** se suscribe a TTN.
5. Los datos son consumidos por:
   - **Home Assistant** (automatización y control).
   - **Telegraf → InfluxDB** (almacenamiento histórico).
   - **Grafana** (visualización).

---

## 4. Flujo de control (downlink)

### 4.1 Problema encontrado

El objetivo inicial era enviar comandos directamente desde **Home Assistant → MQTT → TTN**. Sin embargo, surgieron dos problemas:

1. **Incompatibilidad de versiones** entre:
   - El broker MQTT de TTN.
   - La integración MQTT utilizada por Home Assistant.

2. **Limitaciones del bridge MQTT**:
   - TTN v3 utiliza autenticación, topics y estructura diferentes a un broker MQTT clásico.
   - Las versiones de Home Assistant y Mosquitto utilizadas no permitían un bridge limpio y estable.

Debido a esto, no fue posible implementar un flujo directo Home Assistant → TTN mediante MQTT.

### 4.2 Solución adoptada: Node-RED

Se utilizó **Node-RED** como capa intermedia para la comunicación de retorno:

1. Home Assistant envía el comando a Node-RED.
2. Node-RED formatea el mensaje según el esquema requerido por TTN.
3. Node-RED publica el **downlink** en TTN vía MQTT.
4. TTN envía el downlink al nodo LoRa.

Esta solución resultó:
- Simple.
- Funcional.
- Fácil de depurar.

---

## 5. Downlinks y control de actuadores

Los comandos de control se envían mediante **downlinks** usando distintos puertos:

- **Puerto 1**: control del servo (cortina).
- **Puerto 2**: control de la válvula.

El nodo LoRa:
1. Recibe el downlink.
2. Interpreta el puerto.
3. Envía el comando correspondiente al **Arduino esclavo**.

El Arduino:
- Maneja los servos.
- Ejecuta la acción solicitada (abrir/cerrar cortina o válvula).

---

## 6. Justificación del diseño

### 6.1 Separación LoRa – Arduino

Se decidió separar funciones por las siguientes razones:

- El nodo LoRa:
  - Tiene memoria limitada.
  - No soporta librerías de servo junto con LMIC.
  - No puede alimentar servos directamente.

- El Arduino:
  - Maneja sin problemas librerías de servo.
  - Permite una lógica de control más flexible.

Esta separación mejora:
- Estabilidad.
- Claridad del diseño.
- Facilidad de depuración.

---

## 7. Servicios utilizados

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

## 8. Conclusiones

Este trabajo permitió comprender:

- El funcionamiento real de una red LoRaWAN.
- La integración de TTN con MQTT.
- El uso de Node-RED como middleware.
- Las limitaciones prácticas del hardware.
- La importancia de separar responsabilidades entre dispositivos.

El sistema cumple los objetivos planteados y sirve como base para futuras ampliaciones.

---

## 9. Notas finales

Este repositorio contiene:
- Código del nodo LoRa.
- Código del Arduino esclavo.
- Flujos de Node-RED.
- Configuración de servicios.

Cualquier mejora futura puede realizarse sobre esta arquitectura base.

