# **Scripts – Herramientas auxiliares del proyecto**

Esta carpeta contiene **scripts y archivos de apoyo** utilizados durante el desarrollo del TPF de Telecomunicaciones II.

Su propósito **no es ejecutar servicios en producción**, sino:

* Analizar señales
* Calibrar sensores
* Capturar y procesar códigos infrarrojos
* Verificar equivalencia entre señales
* Servir como puente entre hardware experimental y la configuración final en ESPHome

> ⚠️ **Importante**: no todos los scripts están pensados para uso continuo.
> Muchos son **herramientas de laboratorio** utilizadas para obtener parámetros o datos que luego se integran al firmware definitivo.

---

## 🧠 Clasificación general

Los archivos de esta carpeta pueden agruparse en tres categorías:

1. **Calibración y modelado de sensores**
2. **Procesamiento y verificación de señales infrarrojas (IR)**
3. **Firmware auxiliar en Arduino para captura/emisión IR**

---

## 📐 Calibración y modelado de sensores

### `calcularModelo.py`

Script en Python utilizado para **calibrar un sensor LDR** mediante un modelo físico-matemático.

Funcionalidad principal:

* Convierte mediciones de **voltaje** a **resistencia**
* Ajusta el modelo:

```
R = k · LUX^(-n)
```

* Estima los parámetros `k` y `n` mediante ajuste por mínimos cuadrados (`curve_fit`)

Uso típico:

* Se toman mediciones reales de voltaje vs lux
* Se ejecuta el script
* Los parámetros obtenidos se copian al bloque `substitutions` del YAML de ESPHome

Este enfoque permite **separar calibración experimental de firmware**, manteniendo el código embebido simple y reproducible.

---

## 🔍 Verificación de señales

### `comparar.py`

Script sencillo en Python que compara dos archivos mediante **hash SHA-256**.

Utilidad:

* Verificar si dos capturas IR son **idénticas**
* Detectar diferencias entre códigos ON/OFF
* Confirmar que una conversión de formato no alteró la señal

Uso típico:

```
python comparar.py archivo1 archivo2
```

Se utilizó principalmente durante la captura y limpieza de señales infrarrojas.

---

## 📡 Señales infrarrojas – Formatos y datos

Durante el proyecto fue necesario trabajar con señales IR en distintos formatos.

### Archivos en formato Pronto

* `on_pronto.txt`
* `off_pronto.txt`
* `off_prueba.txt`

Estos archivos contienen **códigos IR en formato Pronto**, obtenidos a partir de capturas reales.

Uso:

* Documentar los códigos originales
* Servir como entrada para conversión a otros formatos
* Comparar señales ON vs OFF

---

### Señales en formato RAW

* `raw.txt`

Contiene la señal IR en **formato RAW** (duraciones positivas y negativas en microsegundos), compatible directamente con:

* `remote_transmitter.transmit_raw` en ESPHome

Este formato fue el finalmente utilizado para el control del aire acondicionado.

---

### Conversión de formatos

* `pronto_to_raw`

Herramienta utilizada para convertir señales desde **formato Pronto a formato RAW**.

Este paso fue necesario porque:

* ESPHome trabaja nativamente con códigos RAW
* La marca de aire acondicionado no contaba con perfiles predefinidos

---

## ❄️ Control de aire acondicionado (caso real)

La marca de aire acondicionado utilizada (**Tokyo**) no se encuentra ampliamente documentada en bases de datos IR.

Por este motivo se siguió el siguiente flujo:

1. **Captura de señal IR** usando un receptor
2. Uso de un **Arduino UNO** como plataforma de lectura
3. Registro de la señal en formato Pronto
4. Conversión a formato RAW
5. Verificación de equivalencia con scripts Python
6. Integración final en ESPHome (`transmit_raw`)

Este proceso permitió:

* Controlar un equipo real no soportado
* Evitar dependencias externas
* Comprender el protocolo IR a bajo nivel

---

## 🔧 Firmware auxiliar en Arduino

Dentro de la carpeta también se incluyen sketches de Arduino utilizados como **herramientas de laboratorio**:

* Captura de señales IR
* Emisión de códigos ON/OFF
* Pruebas de repetibilidad

Estos sketches **no forman parte del sistema final**, pero fueron esenciales para obtener los datos que luego se integraron en ESPHome.


---

**Versión del documento:** 1.0
