# InfluxDB

## Introducción

InfluxDB es una base de datos orientada a series temporales, optimizada para almacenar mediciones indexadas por tiempo.

---

## Rol en el sistema

InfluxDB almacena:

- Nivel de perfume medido.
- Marca temporal de cada medición.
- Posibles campos adicionales para expansión futura.

---

## Justificación técnica

InfluxDB es adecuada debido a:

- Alta eficiencia en datos temporales.
- Integración directa con Telegraf y Grafana.
- Lenguaje de consulta especializado.
- Soporte para retención de datos.

---

## Organización de datos

Los datos se estructuran en:

- **Buckets**: contenedores lógicos de datos.
- **Measurements**: tipo de medición.
- **Fields**: valores numéricos.
- **Tags**: metadatos descriptivos.

---

## Despliegue en Docker

InfluxDB se ejecuta en un contenedor con:

- Volúmenes persistentes.
- Configuración inicial automatizada.
- Acceso restringido por credenciales.

Este enfoque garantiza continuidad de datos ante reinicios.
