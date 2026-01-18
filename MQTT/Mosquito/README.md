# Broker MQTT – Mosquitto

## Introducción

MQTT es un protocolo de mensajería ligero basado en el modelo publicador/suscriptor, ampliamente utilizado en sistemas IoT por su eficiencia y bajo consumo de recursos.

En este proyecto, Mosquitto actúa como broker central de comunicaciones.

---

## Rol de Mosquitto en el sistema

- Recibir mensajes del ESP8266.
- Distribuirlos a Telegraf y otros clientes.
- Desacoplar hardware y software de backend.

---

## Instalación mediante Docker

### Preparación del entorno

1. Crear una carpeta para Mosquitto:
mkdir mosquitto

2. Crear subdirectorios necesarios:
mkdir -p mosquitto/config mosquitto/data mosquitto/log

---

### Archivo de configuración básico

Crear el archivo `mosquitto/config/mosquitto.conf`:

listener 1883
allow_anonymous true
persistence true
persistence_location /mosquitto/data
log_dest file /mosquitto/log/mosquitto.log

---

### Servicio en Docker Compose

En el archivo `docker-compose.yml` se define el servicio Mosquitto, exponiendo el puerto 1883 y utilizando volúmenes persistentes.

---

## Verificación de funcionamiento

1. Iniciar el contenedor:
docker compose up -d

2. Verificar que Mosquitto esté activo:
docker ps

3. Probar publicación/suscripción básica para validar conectividad.

---

## Justificación técnica

Mosquitto fue elegido por su estabilidad, compatibilidad con herramientas IoT y facilidad de despliegue en contenedores.
