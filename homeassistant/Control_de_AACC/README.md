# Control del aire acondiconado (en este caso apagado/encendido a 24grados).

## Procedimiento realizado:

Para lograr el control remoto de un aire acondicionado marca Tokyo, se decidió implementar una solución basada en señales infrarrojas en modo raw, debido a que dicho equipo no se encuentra tabulado ni soportado directamente por las librerías estándar de control IR.

En una primera etapa, utilizamos un receptor infrarrojo modelo TL1838, conectado a un Arduino Uno. Para garantizar un funcionamiento correcto del sensor, se siguieron las recomendaciones especificadas en el datasheet del TL1838, implementando el circuito acondicionador correspondiente, incluyendo la correcta alimentación, desacople y conexión de salida del receptor.

<img width="892" height="317" alt="ir_receiver_circuit" src="https://github.com/user-attachments/assets/6297fcb5-ada0-406f-b198-8f8015558512" />


Posteriormente, mediante el Arduino IDE, se desarrolló un programa que permitió capturar los pulsos infrarrojos emitidos por el control remoto original del aire acondicionado. El programa fue configurado para medir los tiempos de encendido y apagado de la señal (pulsos raw) y mostrarlos por el monitor serial, expresados en microsegundos. De esta forma, fue posible observar y registrar la secuencia completa de la señal infrarroja asociada al comando de encendido/apagado del equipo.


```cpp
#include <IRremote.h>

void setup() {
  Serial.begin(115200);
  IrReceiver.begin(11, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    IrReceiver.printIRResultRawFormatted(&Serial, true);
    Serial.println("-----");
    IrReceiver.resume();
  }
}
```


Una vez obtenidos los datos crudos de la señal, estos fueron copiados y analizados, realizando los ajustes necesarios para asegurar la correcta alternancia de pulsos positivos y negativos, así como la coherencia temporal de los valores. Luego, la señal procesada fue incorporada al archivo de configuración .yaml utilizado en ESPHome.

```yaml
remote_receiver:
  pin:
    number: GPIO14
    inverted: true
  dump: raw
  tolerance: 30%

remote_transmitter:
  pin: GPIO4
  carrier_duty_percent: 50%

switch:
  - platform: template
    name: "AC ON"
    turn_on_action:
      - remote_transmitter.transmit_raw:
          carrier_frequency: 38000
          code: [9059, -4465, 586, -1661, 586, -1661, 586, -523, 587, -524, 586, -524, 586, -524, 586, -1661, 587, -1660, 587, -1660, 586, -1660, 587, -1660, 587, -523, 587, -523, 587, -523, 586, -524, 586, -1660, 587, -525, 585, -524, 587, -523, 587, -523, 586, -523, 587, -1660, 587, -1659, 586, -1661, 587, -524, 585, -525, 585, -525, 586, -524, 586, -524, 585, -524, 586, -524, 587, -524, 586, -523, 587, -523, 586, -524, 587, -522, 587, -525, 585, -1660, 586, -525, 585, -1661, 586, -523, 587, -524, 587, -523, 585, -525, 586, -523, 587, -522, 587, -524, 586, -524, 586, -524, 586, -523, 587, -523, 586, -523, 587, -523, 587, -524, 585, -1661, 586, -524, 586, -524, 586, -524, 586, -524, 586, -524, 586, -523, 587, -523, 586, -524, 586, -524, 586, -523, 588, -523, 586, -523, 586, -524, 586, -524, 587, -522, 586, -524, 587, -523, 586, -524, 586, -524, 586, -524, 587, -522, 587, -523, 586, -1661, 586, -524, 586, -524, 586, -524, 586, -523, 587, -523, 586, -524, 586, -524, 586, -524, 586, -524, 585, -524, 586, -1661, 587, -523, 586, -1661, 585, -524, 587, -523, 586, -524, 586, -524, 586, -523, 587, -1660, 586, -1661, 586, -1661, 586, -1660, 586, -525, 585, -1661, 586, -524, 585, -525, 586]

  - platform: template
    name: "AC OFF"
    turn_on_action:
      - remote_transmitter.transmit_raw:
          carrier_frequency: 38000
          code: [9083, -4441, 559, -1688, 557, -1690, 558, -552, 557, -553, 558, -552, 558, -553, 556, -1689, 558, -1690, 558, -1689, 557, -1689, 559, -1688, 559, -551, 558, -552, 558, -553, 558, -551, 558, -1689, 558, -552, 557, -553, 558, -552, 558, -553, 557, -552, 558, -1688, 558, -1689, 558, -1689, 558, -552, 558, -552, 558, -552, 558, -552, 557, -553, 556, -554, 557, -553, 557, -552, 557, -554, 557, -553, 556, -554, 555, -555, 556, -553, 556, -1691, 557, -552, 556, -1692, 554, -555, 555, -555, 555, -555, 554, -556, 555, -555, 554, -556, 554, -556, 555, -555, 554, -556, 554, -556, 554, -556, 553, -557, 554, -556, 553, -556, 554, -1693, 554, -556, 554, -555, 555, -555, 555, -555, 555, -555, 554, -556, 554, -556, 554, -555, 555, -556, 554, -556, 553, -556, 555, -555, 554, -556, 554, -556, 554, -556, 553, -557, 553, -556, 555, -555, 554, -555, 555, -555, 554, -557, 554, -555, 554, -556, 554, -556, 553, -556, 554, -556, 554, -1693, 553, -557, 553, -1693, 554, -556, 553, -557, 553, -556, 554, -556, 554, -556, 554, -1692, 554, -1693, 553, -1693, 553, -1693, 554, -556, 554, -556, 554, -556, 554, -555, 554]

```

En la etapa final, se empleó un ESP32 junto con un módulo transmisor infrarrojo, configurado mediante ESPHome para reproducir la señal infrarroja capturada previamente. La transmisión se realizó utilizando el método transmit_raw, permitiendo emitir exactamente la misma secuencia de pulsos registrada desde el control original. De esta manera, se logró el encendido y apagado del aire acondicionado Tokyo de forma confiable.


![IMG_3996](https://github.com/user-attachments/assets/90e64cca-4caa-4659-a6e3-4d83fdfc1165)

![IMG_3996](https://github.com/user-attachments/assets/9cd6e67e-edf9-4156-bf52-66763c45bedc)




La decisión de trabajar con señales raw fue fundamental en este caso, ya que el protocolo utilizado por el aire acondicionado no corresponde a ninguno de los protocolos estándar soportados por las librerías IR convencionales. El uso de señales crudas permitió evitar incompatibilidades y garantizar el correcto funcionamiento del sistema de control.





## Observación: El uso de dump: raw vs all

Esta nota nace de la experiencia directa y de los múltiples intentos fallidos. Fue necesario pasar por varias pruebas antes de dar con el ajuste correcto en el archivo .yaml, donde descubrimos la diferencia crítica:

> **dump: all**: Fue el primer camino que tomamos. Se intentó inicialmente en el .yaml, pero al no pertenecer el aire Tokyo a un protocolo estándar, el sistema no devolvía datos útiles o intentaba encajarlos en protocolos erróneos, entregando basura.

> **dump: raw**: Fue la solución definitiva y el verdadero hallazgo técnico. Al cambiar este parámetro en el .yaml, obligamos al ESP32 a registrar únicamente los tiempos de los pulsos, permitiendo capturar la "huella digital" exacta de la señal sin importar el protocolo del fabricante.

