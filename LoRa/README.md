## Selección de hardware y justificación

En esta sección se justifica la elección del hardware utilizado para el nodo LoRa del sistema, comparando dos plataformas disponibles: **LoRa32u4 II** y **Heltec WiFi LoRa 32**. La decisión se basa en criterios de compatibilidad con LoRaWAN, estabilidad, complejidad del sistema y adecuación al uso con TTN.

---

### LoRa32u4 II

La placa **LoRa32u4 II** integra un microcontrolador **ATmega32u4** junto con un transceptor LoRa (SX1276/SX1278). Su arquitectura es simple y está orientada a aplicaciones LoRaWAN clásicas.

Características relevantes:

* Microcontrolador de 8 bits sin sistema operativo.
* Arquitectura *bare-metal*, con control determinístico del tiempo.
* Compatibilidad directa con el stack **LMIC**, ampliamente utilizado con TTN.
* Consumo energético reducido.
* Alta estabilidad en procesos de *join*, uplinks y downlinks.

Esta placa se ajusta al modelo de funcionamiento para el cual fue diseñado LMIC, lo que resulta en una implementación confiable y predecible de LoRaWAN.

---

### Heltec WiFi LoRa 32

La **Heltec WiFi LoRa 32** combina un microcontrolador **ESP32** con conectividad WiFi, Bluetooth y un transceptor LoRa. Si bien ofrece mayor potencia de procesamiento, su arquitectura introduce una complejidad considerable.

Características relevantes:

* Microcontrolador de 32 bits con **FreeRTOS**.
* Ejecución concurrente de múltiples tareas.
* Presencia de stacks WiFi y Bluetooth activos.
* Uso de abstracciones de alto nivel y múltiples interrupciones.

Estas características interfieren con los requisitos temporales estrictos del protocolo LoRaWAN, especialmente en lo relativo a las ventanas de recepción RX1 y RX2.

---

### Limitaciones de la Heltec WiFi LoRa 32 con TTN

La ausencia de una librería estable y oficial para implementar LoRaWAN con TTN en la Heltec WiFi LoRa 32 no se debe a una falta de desarrollo, sino a incompatibilidades estructurales:

* El stack LMIC fue diseñado para microcontroladores simples y con control exclusivo del tiempo.
* El uso de FreeRTOS introduce latencias no determinísticas.
* El stack WiFi del ESP32 puede bloquear interrupciones y temporizadores críticos.
* Pequeños desvíos temporales provocan fallos en el *join* o pérdida de downlinks.

Como consecuencia, las implementaciones LoRaWAN sobre ESP32 presentan comportamientos erráticos y baja confiabilidad cuando se utilizan con TTN.

---

### Comparación entre plataformas

| Característica                    | LoRa32u4 II        | Heltec WiFi LoRa 32 |
| --------------------------------- | ------------------ | ------------------- |
| Microcontrolador                  | ATmega32u4         | ESP32               |
| Arquitectura                      | Simple, bare-metal | Compleja, RTOS      |
| WiFi / Bluetooth                  | No                 | Sí                  |
| Compatibilidad con LMIC           | Alta               | Baja / inestable    |
| Estabilidad con TTN               | Alta               | Variable            |
| Consumo energético                | Bajo               | Alto                |
| Adecuación para LoRaWAN académico | Alta               | Limitada            |

---

### Justificación de la elección

Aunque la Heltec WiFi LoRa 32 ofrece mayores prestaciones, su complejidad arquitectónica dificulta una implementación estable de LoRaWAN con TTN. En cambio, la LoRa32u4 II proporciona un entorno controlado y compatible con los supuestos del stack LMIC, resultando más adecuada para un trabajo práctico académico centrado en comunicaciones y redes LoRaWAN.

Por estas razones, se optó por utilizar la **LoRa32u4 II** como nodo LoRa del sistema.

---

### Configuración del entorno de desarrollo

Para programar el LoRa32u4 II se utiliza el entorno Arduino IDE, aprovechando la compatibilidad del microcontrolador ATmega32u4 con las placas tipo Feather.

Es necesario agregar el gestor de placas de Adafruit en las preferencias del Arduino IDE, lo que permite acceder a las definiciones de placas compatibles. Una vez agregado este repositorio, se selecciona la placa “Adafruit Feather 32u4” desde el Board Manager.

Esta selección es correcta debido a que el LoRa32u4 II comparte el mismo microcontrolador y disposición básica de pines con dicha placa. Tras esta configuración, el proceso de compilación y carga del firmware se realiza de manera estándar, sin requerir herramientas adicionales.