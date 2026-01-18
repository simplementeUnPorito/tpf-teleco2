# Telegraf

## Introducción

Telegraf es un agente de recolección de métricas que permite consumir datos desde múltiples fuentes y reenviarlos a bases de datos temporales.

En este proyecto, actúa como intermediario entre MQTT e InfluxDB.

---

## Rol en la arquitectura

- Suscribirse a tópicos MQTT.
- Interpretar mensajes.
- Transformarlos en métricas.
- Enviarlos a InfluxDB.

---

## Instalación mediante Docker

### Preparación del entorno

1. Crear carpeta de trabajo:
mkdir telegraf


2. Crear archivo de configuración:
touch telegraf/telegraf.conf


---

### Configuración conceptual

El archivo `telegraf.conf` define:

- Entrada MQTT.
- Salida InfluxDB.
- Formato de datos.

Ejemplo conceptual:

[[inputs.mqtt_consumer]]
servers = ["tcp://mosquitto:1883"]
topics = ["dispensador/perfume/#"]
data_format = "value"

[[outputs.influxdb_v2]]
urls = ["http://influxdb:8086"]


---

### Despliegue

Telegraf se ejecuta como contenedor, compartiendo red con Mosquitto e InfluxDB.

---

## Verificación

- Confirmar conexión al broker.
- Verificar logs del contenedor.
- Confirmar escritura en InfluxDB.

---

## Enfoque académico

Telegraf permite analizar claramente la separación entre adquisición, procesamiento y almacenamiento de datos.
