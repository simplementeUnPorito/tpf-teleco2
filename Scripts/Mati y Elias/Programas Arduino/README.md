# Programas IR (reenvío de señales capturadas)

Este directorio contiene **dos programas** para trabajar con señales de un **control remoto infrarrojo (IR)** usando la librería **IRremote**.

> Aclaración importante: estos programas **no detectan/capturan** señales. **Reenvían (emiten)** tramas IR ya capturadas (listas de tiempos) mediante `sendRaw()`. Para **detectar/capturar** señales necesitás un **receptor IR** y un sketch de lectura.

---

## ¿Para qué sirven estos 2 programas?

Ambos sketches sirven para **reproducir** una señal IR (por ejemplo, *ON* y/o *OFF*) enviando una secuencia de tiempos **mark/space** a una portadora típica de **38 kHz**.

Esto es útil cuando:

* Querés **replicar** el comando de un control remoto sin decodificar protocolo (NEC/Sony/etc.).
* Ya tenés los tiempos capturados y querés probar si el equipo responde.

---

## Programa 1 — Envío de ON y OFF

Este programa envía en un bucle:

* Un comando **ON** (un frame principal + un frame corto)
* Luego un comando **OFF** (un frame principal + un frame corto)

Incluye mensajes por Serial (115200) para ver cuándo se envía cada comando.

**Objetivo:** probar rápidamente el encendido/apagado (o dos comandos distintos) de un equipo IR.

---

## Programa 2 — Envío de ON (dos frames)

Este programa envía únicamente el comando **ON**, en dos partes:

* `frame_on_1` (frame principal)
* `frame_on_2` (frame corto)

También imprime por Serial cuándo se envía cada frame.

**Objetivo:** probar el envío repetido de un comando específico (por ejemplo, ON) y validar que el receptor lo tome de forma consistente.


---

## Requisitos

* Placa Arduino compatible
* **LED IR** (recomendado con resistencia y, si se busca alcance, con transistor driver)
* Librería **IRremote** instalada
* Monitor Serial a **115200**

---

## Conexión (hardware)

* Conectar el **LED IR** al pin de salida configurado en el sketch (si el sketch lo define) o al pin que use tu configuración actual.
* Colocar una **resistencia en serie** con el LED IR.

> Si el alcance es bajo, usar un **transistor** como driver para aumentar corriente del LED IR.

---

## Nota sobre `sendRaw()`

Las tramas (`uint16_t frame[]`) contienen tiempos alternados (en microsegundos):

* **mark**: LED IR emitiendo a la frecuencia indicada (por ejemplo 38 kHz)
* **space**: LED IR apagado

La llamada:

```cpp
irsend.sendRaw(frame, length, khz);
```

reproduce exactamente esa temporización.

---

## Programa 3 — Sensor HC-SR04

Este conjunto de scripts permite medir la distancia de objetos utilizando un sensor de ultrasonido **HC-SR04** conectado a un **ESP8266**, enviando los datos en tiempo real a un broker **MQTT** a través de Wi-Fi.

### Funcionamiento
* **Sensor HC-SR04**: Emite una señal ultrasónica y mide el tiempo que tarda en retornar tras rebotar en un objeto para calcular la distancia.
* **Conectividad**: El ESP8266 se conecta a la red local mediante Wi-Fi.
* **Protocolo MQTT**: Los datos de distancia se publican en un "topic" específico, permitiendo que Home Assistant u otros servicios se suscriban para visualizar o actuar según la medición.

### Archivos
* **`sensor_hcsr04.ino`**: Código principal que gestiona la conexión Wi-Fi, el cliente MQTT y el ciclo de lectura/envío.
* **`sensor.h`** (Recomendado): Archivo separado para definir las credenciales de Wi-Fi y la dirección del servidor MQTT, manteniendo el código principal limpio y seguro.
