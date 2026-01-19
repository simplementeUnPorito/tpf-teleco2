# Control del aire acondiconado (en este caso apagado/encendido a 24grados).

## Procedimiento realizado:

Para lograr el control remoto de un aire acondicionado marca Tokyo, se decidió implementar una solución basada en señales infrarrojas en modo raw, debido a que dicho equipo no se encuentra tabulado ni soportado directamente por las librerías estándar de control IR.

En una primera etapa, utilizamos un receptor infrarrojo modelo TL1838, conectado a un Arduino Uno. Para garantizar un funcionamiento correcto del sensor, se siguieron las recomendaciones especificadas en el datasheet del TL1838, implementando el circuito acondicionador correspondiente, incluyendo la correcta alimentación, desacople y conexión de salida del receptor.

Posteriormente, mediante el Arduino IDE, se desarrolló un programa que permitió capturar los pulsos infrarrojos emitidos por el control remoto original del aire acondicionado. El programa fue configurado para medir los tiempos de encendido y apagado de la señal (pulsos raw) y mostrarlos por el monitor serial, expresados en microsegundos. De esta forma, fue posible observar y registrar la secuencia completa de la señal infrarroja asociada al comando de encendido/apagado del equipo.

Una vez obtenidos los datos crudos de la señal, estos fueron copiados y analizados, realizando los ajustes necesarios para asegurar la correcta alternancia de pulsos positivos y negativos, así como la coherencia temporal de los valores. Luego, la señal procesada fue incorporada al archivo de configuración .yaml utilizado en ESPHome.

En la etapa final, se empleó un ESP32 junto con un módulo transmisor infrarrojo, configurado mediante ESPHome para reproducir la señal infrarroja capturada previamente. La transmisión se realizó utilizando el método transmit_raw, permitiendo emitir exactamente la misma secuencia de pulsos registrada desde el control original. De esta manera, se logró el encendido y apagado del aire acondicionado Tokyo de forma confiable.

La decisión de trabajar con señales raw fue fundamental en este caso, ya que el protocolo utilizado por el aire acondicionado no corresponde a ninguno de los protocolos estándar soportados por las librerías IR convencionales. El uso de señales crudas permitió evitar incompatibilidades y garantizar el correcto funcionamiento del sistema de control.

