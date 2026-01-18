# Grafana

## Introducción

Grafana es una plataforma de visualización de datos que permite crear dashboards interactivos a partir de múltiples fuentes.

---

## Rol en el proyecto

- Visualizar el nivel de perfume.
- Analizar tendencias.
- Facilitar la interpretación de datos.

---

## Instalación mediante Docker

### Preparación

1. Crear carpeta:
mkdir grafana

3. Crear subdirectorio de datos:
mkdir grafana/data

---

### Despliegue

Grafana se define como servicio en Docker Compose, exponiendo el puerto 3000.

---

### Configuración inicial

1. Acceder a:
http://localhost:3000

2. Credenciales iniciales:
- Usuario: admin
- Contraseña: admin

3. Agregar InfluxDB como Data Source.

---

## Creación de dashboards

- Seleccionar bucket.
- Definir consultas.
- Crear gráficos temporales.

---

## Enfoque académico

Grafana permite visualizar el impacto de la frecuencia de muestreo y la dinámica del sistema.
