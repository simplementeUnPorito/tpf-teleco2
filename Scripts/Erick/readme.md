# Puesta en marcha del sistema

## Requisitos previos

- Ubuntu Linux
- Docker y Docker Compose
- ESP8266
- Red WiFi funcional

---

## Procedimiento completo

### 1. Infraestructura base

1. Instalar Docker.
2. Instalar Docker Compose.
3. Crear estructura de carpetas.

---

### 2. Levantar servicios

1. Ejecutar:
docker compose up -d

2. Verificar contenedores activos:
docker ps


---

### 3. Configuración del ESP8266

1. Crear firmware en ESPHome.
2. Flashear el dispositivo.
3. Verificar conexión WiFi.

---

### 4. Verificación de datos

1. Confirmar mensajes MQTT.
2. Ver datos en InfluxDB.
3. Visualizar en Grafana.

---

### 5. Domótica

1. Integrar con Home Assistant.
2. Confirmar entidades.
3. Probar lectura del sensor.

---

## Pruebas básicas

- Publicación correcta de datos.
- Persistencia en base de datos.
- Visualización funcional.

---

## Consideraciones finales

El sistema es completamente reproducible y adecuado para evaluación académica en Telecomunicaciones II.

