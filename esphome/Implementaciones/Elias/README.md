# Nodo `aula_sensors` (ESPHome) – Implementación de Elías

Este nodo ESPHome fue desarrollado como **prueba conceptual** de un dispositivo WiFi (ESP32) capaz de:

* Medir **temperatura y humedad** (DHT22).
* Estimar **nivel de iluminación** a partir de un **LDR** (ADC + modelo calibrado).
* **Capturar** señales IR con un receptor (TL1838) y **reenviarlas** con un LED IR para controlar un aire acondicionado.

La lógica se define en un único archivo **YAML**. Abajo se explica **qué hace cada bloque**.

---

## 1) Identificación del nodo

```yaml
esphome:
  name: aula_sensors
```

* Define el **nombre del dispositivo** dentro de ESPHome y Home Assistant.
* También suele usarse como **hostname** en la red.

---

## 2) Plataforma de hardware

```yaml
esp32:
  board: esp32dev
  framework:
    type: arduino
```

* Indica que el firmware se compila para un **ESP32** (placa genérica `esp32dev`).
* Se usa el framework **Arduino**.

---

## 3) Servicios base (diagnóstico y actualización)

```yaml
logger:
ota:
  platform: esphome
```

* `logger`: habilita **logs** para depuración (serial/red según configuración).
* `ota`: habilita **actualizaciones inalámbricas** (OTA) una vez que el dispositivo ya fue flasheado inicialmente.

---

## 4) Conectividad WiFi + IP fija

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

* `!secret`: toma credenciales desde `secrets.yaml` (evita exponerlas en el repo).
* `ap`: crea un **punto de acceso de recuperación** si el nodo no logra conectarse.
* `manual_ip`: se usa **IP fija** para evitar problemas intermitentes de descubrimiento (mDNS) en la red del laboratorio.

---

## 5) API de integración con Home Assistant

```yaml
api:
  reboot_timeout: 0s
```

* `api`: habilita la integración nativa con Home Assistant.
* `reboot_timeout: 0s`: evita reinicios automáticos si se pierde la comunicación.

---

## 6) Parámetros calibrables (substitutions)

```yaml
substitutions:
  ldr_min_v: "0.150"
  ldr_max_v: "2.45"
  avg_window: "10"
  Rref: "10000.0"
  k_param: "218254.528..."
  n_param: "2.924417..."
```

Este bloque permite **ajustar el comportamiento sin cambiar la lógica**:

* `ldr_min_v` / `ldr_max_v`: límites de voltaje observados (usados para saturación/normalización).
* `avg_window`: tamaño del **promedio móvil** aplicado a señales.
* `Rref`, `k_param`, `n_param`: parámetros del **modelo empírico del LDR**.

> Estos parámetros se obtuvieron por calibración empírica (ver carpeta `Scripts/`).

---

## 7) Sensores: DHT22 (temperatura y humedad)

```yaml
sensor:
  - platform: dht
    pin: 4
    model: AM2302
    ...
```

* Lee un **DHT22 / AM2302** en el pin 4.
* Publica dos entidades:

  * `Aula Temperatura`
  * `Aula Humedad`
* A ambas se les aplica **sliding_window_moving_average** con `avg_window` para suavizar ruido.

---

## 8) LDR: ADC crudo + voltaje filtrado + lux estimado

### 8.1 Lectura ADC cruda (base de cálculo)

```yaml
- platform: adc
  pin: 34
  id: ldr_raw
  raw: true
  internal: true
```

* Lee el ADC y guarda el valor en `id: ldr_raw`.
* `internal: true`: oculta esta señal en Home Assistant (se usa solo como señal intermedia).

### 8.2 Voltaje “real” (único filtrado)

```yaml
- platform: template
  id: ldr_voltage
  lambda: |-
    float raw = id(ldr_raw).state;
    float V = raw/1000;
    ...
```

* Convierte la lectura del ADC a una magnitud interpretada como **voltaje**.
* Se aplica **saturación** entre `ldr_min_v` y `ldr_max_v`.
* Se aplica el **promedio móvil** acá (para no filtrar repetidamente varias señales derivadas).

**Observación técnica:** el significado exacto de `raw` puede variar según configuración/librería (cuentas vs mV vs V). En esta implementación se usa una **normalización empírica** (`raw/1000`) ajustada con valores observados en el osciloscopio para trabajar en voltios dentro de un rango coherente.

### 8.3 Lux estimado (hereda el suavizado)

```yaml
- platform: template
  id: luz_aula_lux
  lambda: |-
    float V = id(ldr_voltage).state;
    ...
    float logLux = (log(k) - log(Rref) - log(inner)) / n;
    float lux = exp(logLux);
```

* Calcula **lux** a partir del voltaje usando un **modelo empírico** del LDR.
* `k_param` y `n_param` son parámetros calibrados con un sensor de luminosidad del laboratorio.
* Incluye validaciones para evitar resultados inválidos (NaN, negativos, saturación).

> Los scripts de calibración y comparación están en `Scripts/` (ver más abajo).

---

## 9) IR: receptor + transmisor

### 9.1 Receptor IR (captura de códigos)

```yaml
remote_receiver:
  pin:
    number: GPIO14
    inverted: true
  dump: raw
  tolerance: 55%
```

* Se añadió un **receptor IR TL1838** para **capturar señales** de un control remoto.
* `dump: raw` imprime la señal en formato **RAW** (duraciones mark/space) para poder copiarla.
* `tolerance` ajusta la tolerancia de reconocimiento al capturar.

### 9.2 Transmisor IR (reenvío de códigos)

```yaml
remote_transmitter:
  pin: GPIO25
  carrier_duty_percent: 50%
```

* Emite la señal IR mediante un **LED infrarrojo** (en una plaquita/circuito simple provisto por el profesor).

---

## 10) Control del aire acondicionado (RAW)

```yaml
switch:
  - platform: template
    name: "AC Power"
    turn_on_action:
      - remote_transmitter.transmit_raw: ...
    turn_off_action:
      - remote_transmitter.transmit_raw: ...
```

* Se crea un `switch` que **prende/apaga** el aire enviando códigos IR en crudo (`transmit_raw`).
* Se usa una `carrier_frequency` cercana a **38 kHz** (aproximada a partir de la captura y validada por prueba: el equipo responde bien).

### ¿Por qué RAW?

Los aires acondicionados de marca **Tokyo** no se encontraron dentro de los perfiles IR “conocidos”/compatibles disponibles, por lo que se adoptó el enfoque:

1. Capturar el comando del control remoto en **RAW**.
2. Reenviarlo desde el ESP32 con `remote_transmitter.transmit_raw`.

### Extensión futura (máquina de estados)

En esta versión se implementa solo **Power** (simplificación para validar el concepto). Sin embargo, el enfoque RAW permite escalar a una **máquina de estados** del aire acondicionado, por ejemplo:

* Estados por temperatura: 16°C, 17°C, 18°C, …, 24°C
* Estados por modo: frío/calor/auto
* Estados por fan: low/med/high

Eso se logra guardando múltiples códigos RAW y seleccionándolos según el estado. La lógica puede implementarse:

* dentro de ESPHome (por ejemplo con `select`, `number`, `script`, etc.), o
* con apoyo de Home Assistant (automatizaciones/escenas) que cambien el estado.

---

## Integración en Home Assistant

De forma general, para agregar el nodo en Home Assistant:

1. Entrar a Home Assistant.
2. Ir a **Overview → + → Add device** (o alternativamente **Settings → Devices & Services**).
3. Seleccionar **ESPHome**.
4. Elegir el nodo detectado (o ingresarlo manualmente si hace falta).

Al integrarlo, Home Assistant crea entidades como:

* `Aula Temperatura`
* `Aula Humedad`
* `LDR Voltaje Real`
* `Luz Aula (Lux)`
* `AC Power`

---

## Herramientas auxiliares (carpeta `Scripts/`)

Para apoyar esta implementación se desarrollaron herramientas que ayudan a:

* **Procesar/corregir códigos IR** capturados y convertirlos a un formato utilizable.
* **Calibrar** el modelo del LDR y comparar resultados.

Ubicación (según estructura del repositorio):

* `Scripts/Mati y Elias/Python/`

  * `calcularModelo.py` (cálculo/ajuste de modelo LDR)
  * `comparar.py` (comparaciones/validación)
  * archivos de apoyo (`on_pronto.txt`, `off_pronto.txt`, `raw.txt`, etc.)

* `Scripts/Mati y Elias/Programas Arduino/`

  * `recibir_raw/` (captura de señales IR)
  * `enviarOnOFF/` (reenvío de señales IR)

> Para más detalle técnico sobre estas herramientas, leer el `README.md` dentro de `Scripts/Mati y Elias/`.
