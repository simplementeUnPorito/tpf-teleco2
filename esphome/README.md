# **ESPHome – Implementación en Docker**

ESPHome se utiliza para **crear, compilar y administrar** los dispositivos WiFi del proyecto (ESP8266 y ESP32). En la PC servidor de la facultad se ejecuta un **contenedor Docker dedicado**, permitiendo que todos los integrantes trabajen sobre un entorno común accesible desde la red local o mediante la VPN Tailscale.

Este documento describe:

* Cómo se desplegó ESPHome en el servidor
* Cómo acceder al panel
* Cómo levantar un entorno local opcional
* Problemas encontrados durante el semestre y las decisiones de diseño adoptadas

---

## 🧱 1. Contenedor ESPHome en el servidor de la facultad

El contenedor oficial de ESPHome fue creado con el siguiente comando:

```bash
docker run -d \
  --name esphome \
  --restart unless-stopped \
  -v /home/user/esphome:/config \
  --net=host \
  --privileged \
  --device=/dev/ttyUSB0 \
  --device=/dev/ttyACM0 \
  esphome/esphome
```

### 📌 Descripción de parámetros

* `-v /home/user/esphome:/config`
  Almacena de forma persistente todos los proyectos, YAMLs y compilaciones de ESPHome.

* `--net=host`
  Permite acceder directamente al panel web de ESPHome sin redirecciones de puertos:

  ```
  http://IP_DEL_SERVIDOR:6052/
  ```

* `--privileged` y `--device=/dev/...`
  Habilitan el acceso a dispositivos USB para poder flashear ESPs conectados físicamente al servidor.

* `--restart unless-stopped`
  Garantiza que el servicio se reinicie automáticamente ante reinicios del sistema o fallos del contenedor.

---

## 🌐 2. Acceso al panel ESPHome

Con el contenedor en ejecución, el panel se encuentra disponible en:

```
http://[IP_DEL_SERVIDOR]:6052/
```

Ejemplo real:

```
http://100.109.64.19:6052/
```

El acceso es posible desde:

* La red local de la facultad
* Cualquier dispositivo conectado a la VPN Tailscale

---

## 🔌 Flasheo del ESP (primer upload)

El **primer flasheo** de un dispositivo ESP (ESP32 / ESP8266) **debe realizarse obligatoriamente por USB**. A partir de ese primer upload, todas las actualizaciones posteriores pueden realizarse **vía OTA (Over-The-Air)** de forma inalámbrica.

Existen **dos formas válidas** de realizar este flasheo inicial en el contexto del proyecto:

---

### 🖥️ Opción 1: Flasheo directo desde el servidor de la facultad

Esta opción se utiliza cuando el ESP está **conectado físicamente por USB** a la PC servidor donde corre el contenedor ESPHome.

**Procedimiento:**

1. Conectar el ESP por USB al servidor
2. Acceder al panel de ESPHome del servidor
3. Compilar y flashear el nodo directamente desde el dashboard

Este método:

* Es el más directo
* No requiere herramientas adicionales
* Depende de la disponibilidad física del servidor

---

### 💻 Opción 2: Flasheo desde tu PC personal (ESPHome Web)

Si el ESP está conectado por USB a **tu propia PC** (por ejemplo, trabajando desde casa), el flasheo inicial se realiza mediante **ESPHome Web**.

**Procedimiento:**

1. Crear o compilar el proyecto ESPHome (en el servidor o localmente)
2. Conectar el ESP por USB a tu PC
3. Abrir en el navegador:

```
https://web.esphome.io/?dashboard_install
```

4. Seleccionar el archivo de firmware (`.bin`) generado
5. Flashear el dispositivo directamente desde el navegador

Este método:

* No requiere acceso USB al servidor
* Funciona desde cualquier PC
* Es ideal para trabajo remoto

---

### 📡 Actualizaciones posteriores (OTA)

Una vez realizado el flasheo inicial por USB (mediante cualquiera de las dos opciones):

* El dispositivo queda habilitado para **actualizaciones OTA**
* Los siguientes uploads se realizan **de forma inalámbrica** desde ESPHome
* Ya no es necesario volver a conectar el ESP por USB

---

## 💻 3. Entorno ESPHome local (opcional)

Para trabajar desde casa sin depender del servidor de la facultad, cada estudiante puede levantar su propio contenedor ESPHome local.

```bash
mkdir ~/esphome

docker run -d \
  --name esphome \
  --restart unless-stopped \
  -v ~/esphome:/config \
  -p 6052:6052 \
  --privileged \
  --device=/dev/ttyUSB0 \
  --device=/dev/ttyACM0 \
  esphome/esphome
```

### ✔ Ventajas de este enfoque

* Compilar y flashear dispositivos localmente
* No depender de disponibilidad del servidor
* Probar configuraciones de forma aislada
* Sincronizar YAMLs con el servidor solo cuando sea necesario

---

## ⚠️ Problema identificado: descubrimiento por mDNS

Durante el desarrollo del proyecto se identificó un problema recurrente relacionado con el **descubrimiento automático de nodos ESPHome mediante mDNS**.

### 🧠 Descripción del problema

* Cuando el ESP no tiene una **IP conocida** (por ejemplo, asignada dinámicamente por DHCP)
* ESPHome intenta resolver el dispositivo mediante **mDNS** (`.local`)
* En el entorno de red de la facultad, este mecanismo:

  * falla de forma intermitente
  * bloquea el proceso de conexión
  * impide realizar uploads OTA

En la práctica, esto provoca que **no sea posible acceder al dispositivo por vía inalámbrica**, aun cuando el ESP esté correctamente conectado a la red.

---

## ✅ Solución adoptada durante el semestre

Para evitar este problema, durante el semestre se optó por:

### ✔ Configurar **IP fija** en los nodos ESPHome

Esto permite:

* Evitar completamente la dependencia de mDNS
* Acceder siempre al dispositivo mediante una IP conocida
* Garantizar estabilidad en los uploads OTA

Esta solución fue simple, efectiva y compatible con la infraestructura existente.

---

## 🧩 Alternativas más elegantes (no implementadas)

Existen soluciones más robustas que podrían adoptarse en futuras iteraciones del proyecto:

* **Router propio para el laboratorio**
  Red separada con control total de DHCP y mDNS

* **VLAN dedicada para IoT**
  Aislamiento lógico del tráfico IoT del resto de la red institucional

* **Servidor mDNS correctamente configurado**
  (Avahi / reflector entre interfaces)

Estas opciones mejoran escalabilidad y control, pero requieren coordinación con la infraestructura de red de la facultad.


---
# **Estructura del archivo YAML**

Este documento explica la **estructura general de un archivo de configuración ESPHome** utilizado en el proyecto, tomando como ejemplo un nodo real desplegado durante el semestre.

El objetivo es que cualquier alumno pueda:

* Entender qué hace cada bloque del YAML
* Saber dónde modificar parámetros
* Reutilizar la estructura para nuevos dispositivos

---

## 🧩 Estructura general

Un archivo ESPHome se compone de **bloques lógicos**, cada uno con una responsabilidad clara:

1. Identificación del nodo
2. Plataforma de hardware
3. Servicios base (logger, OTA, API)
4. Conectividad de red
5. Parámetros ajustables (substitutions)
6. Sensores
7. Actuadores / periféricos especiales

A continuación se detalla cada sección usando un ejemplo real.

---

## 🏷️ Identificación del dispositivo

```yaml
esphome:
  name: aula_sensors
```

* `name` identifica **únicamente** al nodo dentro de ESPHome y Home Assistant
* Se recomienda usar nombres descriptivos y sin espacios
* Este nombre se utiliza como hostname del dispositivo

---

## 🧠 Plataforma de hardware

```yaml
esp32:
  board: esp32dev
  framework:
    type: arduino
```

Define:

* El microcontrolador utilizado (ESP32 en este caso)
* El modelo de placa
* El framework de desarrollo

> ⚠️ Elegir correctamente la placa es fundamental para evitar problemas de pines y flasheo.

---

## 🛠️ Servicios base

```yaml
logger:
ota:
  platform: esphome
```

* `logger`: habilita logs por puerto serie y red
* `ota`: permite actualizar el firmware **sin cable USB**

Estos servicios se consideran **obligatorios** en todos los nodos del proyecto.

---

## 🌐 Conectividad WiFi

```yaml
wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

  ap:
    ssid: "ESPHome Fallback"
    password: "fallback1234"

  manual_ip:
    static_ip: 10.50.101.19
    gateway: 10.50.0.1
    subnet: 255.255.0.0
```

* Las credenciales se obtienen desde `secrets.yaml`
* El **Access Point de fallback** permite recuperar el nodo si no se conecta
* Se utiliza **IP fija** para evitar problemas de descubrimiento por mDNS

> 🧠 Decisión de diseño: el uso de IP fija mejora la estabilidad de OTA en la red de la facultad.

---

## 🔌 API de integración con Home Assistant

```yaml
api:
  reboot_timeout: 0s
```

* Permite que Home Assistant se comunique directamente con el nodo
* `reboot_timeout: 0s` evita reinicios automáticos si se pierde la conexión

---

## 🔁 Parámetros ajustables (substitutions)

```yaml
substitutions:
  ldr_min_v: "0.150"
  ldr_max_v: "2.45"
  avg_window: "10"
  Rref: "10000.0"
  k_param: "218254.5284788324"
  n_param: "2.9244174253917654"
```

* Permiten ajustar parámetros **sin modificar la lógica del código**
* Facilitan la calibración y reutilización
* Se recomienda agrupar aquí todos los valores experimentales

---

## 🌡️ Sensores físicos (DHT22)

```yaml
sensor:
  - platform: dht
    pin: 4
    model: AM2302
```

* Define un sensor DHT22 (AM2302)
* Se aplican filtros de promedio móvil para reducir ruido
* El `update_interval` controla la tasa de muestreo

---

## 🔢 Lectura analógica cruda (ADC)

```yaml
- platform: adc
  pin: 34
  id: ldr_raw
  raw: true
  internal: true
```

* Se obtiene la lectura ADC **sin filtrar**
* Se marca como `internal` para que no aparezca en Home Assistant
* Sirve como base para cálculos posteriores

---

## ⚡ Sensor virtual: voltaje real

```yaml
- platform: template
  name: "LDR Voltaje Real"
```

* Convierte el valor ADC en voltaje
* Aplica saturación y filtros
* Centraliza el único filtrado del sistema

Este enfoque evita **filtrar múltiples veces la misma señal**.

---

## 💡 Sensor virtual: iluminación (Lux)

```yaml
- platform: template
  name: "Luz Aula (Lux)"
```

* Calcula el nivel de iluminación a partir del voltaje
* Reutiliza el voltaje ya suavizado
* Implementa validaciones para evitar valores inválidos

---

## 📡 Receptor y transmisor infrarrojo

```yaml
remote_receiver:
  pin:
    number: GPIO14
    inverted: true

remote_transmitter:
  pin: GPIO25
```

* Permiten capturar y emitir señales IR
* Se utilizan para el control de aire acondicionado

---

## ❄️ Control del aire acondicionado (IR RAW)

```yaml
switch:
  - platform: template
    name: "AC Power"
```

### 🧠 Consideración importante

La marca de aire acondicionado utilizada (**Tokyo**) no cuenta con perfiles IR ampliamente documentados.

Por este motivo:

* Se capturó el **código IR en formato RAW**
* Se utilizó:

  * Un receptor IR
  * Un Arduino UNO
  * Scripts en Python ubicados en la carpeta `Scripts/`

Los códigos capturados se integraron directamente en ESPHome mediante `transmit_raw`.

Este enfoque permitió:

* Controlar el aire acondicionado sin depender de bases de datos externas
* Adaptarse a marcas no soportadas
* Comprender el protocolo IR a bajo nivel
--- 
**Versión del documento:** 1.1

