# ESPHome

ESPHome se utiliza en el proyecto para **desarrollar y administrar nodos IoT WiFi** basados en **ESP8266** y **ESP32**.

A nivel conceptual, ESPHome permite:

* Definir el comportamiento del dispositivo mediante un archivo **YAML** (configuración declarativa)
* Compilar firmware a partir de ese YAML
* Cargar firmware al dispositivo (primer flasheo) y luego mantenerlo con **actualizaciones OTA**
* Exponer **entidades** (sensores/actuadores) para integrarlas en plataformas de automatización

> En este repositorio se documenta la estructura general y las decisiones de diseño. Los detalles de cada nodo (sensores, pines, calibración y lógica específica) se describen en cada implementación.

---

## ESPHome en el servidor del proyecto

En el servidor de la facultad, ESPHome se ejecuta dentro de un **contenedor Docker**, de modo que el equipo comparta un **entorno común** (mismas versiones y mismas herramientas) para trabajar sobre los nodos WiFi del proyecto.

---

## Acceso al panel (Dashboard)

Formato general:

```txt
http://[IP_DEL_SERVIDOR]:6052/
```

Ejemplo:

```txt
http://100.109.64.19:6052/
```

---

## Flujo de trabajo típico (alto nivel)

1. **Crear/editar** el YAML del nodo (nombre, WiFi, servicios base, sensores/actuadores).
2. **Compilar** el firmware desde el dashboard.
3. **Primer flasheo**: se realiza cargando el firmware al ESP.
4. **Mantenimiento**: una vez habilitado, se usan **actualizaciones OTA** para subir cambios sin cable.

---

## Estructura general de un YAML (ejemplo mínimo)

El YAML de ESPHome se organiza en bloques lógicos. Un ejemplo mínimo genérico:

```yaml
esphome:
  name: nodo_ejemplo

# Plataforma/placa
esp32:
  board: esp32dev
  framework:
    type: arduino

# Servicios base (comunes en el proyecto)
logger:
ota:
  platform: esphome
api:

# Red
wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

  # AP de fallback para recuperación
  ap:
    ssid: "ESPHome Fallback"
    password: "cambiar_esto"

# Ejemplo simple de entidad (sensor/actuador)
# (Cada implementación agrega sus bloques reales: dht, adc, i2c, switch, light, etc.)
# sensor:
#   - platform: ...
#
# switch:
#   - platform: ...
```

### Notas (generales)

* Se recomienda usar **`secrets.yaml`** para credenciales (SSID/password) y evitar exponerlas en el repositorio.
* Mantener **nombres claros** para nodos y entidades facilita la integración y el mantenimiento.

---

## Integración con Home Assistant (cómo añadir, general)

Los nodos ESPHome se integran en Home Assistant mediante la integración **ESPHome**.

Flujo general:

1. Entrar a Home Assistant.
2. Ir a **Settings → Devices & Services**.
3. Presionar **Add Integration**.
4. Seleccionar **ESPHome**.
5. Indicar el nodo detectado o su dirección en la red (si corresponde) y completar el alta.

Al finalizar, el nodo aparecerá como un dispositivo y se crearán sus **entidades** (sensores, switches, etc.) para usarlas en dashboards y automatizaciones.

---

## Alcance de esta documentación

Este README describe **qué es ESPHome** en el proyecto y la **estructura general** de configuración.

* Los detalles de cada nodo (sensores, pines, calibración, y lógica específica) se documentan en la carpeta/implementación del alumno correspondiente.
* La administración del servidor, acceso remoto y decisiones de red se documentan en el README raíz del repositorio.
