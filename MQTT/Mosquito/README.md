# Broker MQTT – Mosquitto

## Introducción

MQTT (Message Queuing Telemetry Transport) es un protocolo de mensajería ligero basado en el modelo **publicador/suscriptor**, ampliamente utilizado en sistemas IoT por su eficiencia y simplicidad.

En este proyecto, MQTT actúa como el **núcleo de comunicación** entre el dispositivo embebido y el resto de la infraestructura.

---

## Rol de Mosquitto en el sistema

Mosquitto cumple la función de:

- Recibir mensajes publicados por el ESP8266.
- Distribuir dichos mensajes a los consumidores suscriptos.
- Desacoplar el hardware de las capas de procesamiento y visualización.

---

## Justificación técnica

Se selecciona Mosquitto debido a:

- Implementación estándar y estable.
- Bajo consumo de recursos.
- Compatibilidad nativa con Telegraf y Home Assistant.
- Facilidad de despliegue mediante Docker.

---

## Concepto de tópicos MQTT

El ESP8266 publica mediciones en tópicos jerárquicos, por ejemplo:

