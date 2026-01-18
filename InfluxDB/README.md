# InfluxDB

## Introducción

InfluxDB es una base de datos optimizada para series temporales, ideal para almacenar métricas IoT.

---

## Rol en el sistema

- Almacenar mediciones del nivel de perfume.
- Asociar cada dato a una marca temporal.
- Permitir consultas históricas.

---

## Instalación mediante Docker

### Preparación

1. Crear carpeta:
mkdir influxdb

2. Crear subdirectorios:
mkdir -p influxdb/data influxdb/config


---

### Configuración inicial

InfluxDB se inicializa mediante variables de entorno:

- Usuario administrador
- Organización
- Bucket
- Token de acceso

Esto se define en `docker-compose.yml`.

---

### Despliegue

1. Iniciar el servicio:
docker compose up -d

2. Acceder a la interfaz web:
http://localhost:8086

---

## Persistencia de datos

El uso de volúmenes asegura que los datos no se pierdan al reiniciar los contenedores.

---

## Justificación técnica

InfluxDB es ampliamente utilizado en entornos industriales y académicos para el análisis de datos temporales.
