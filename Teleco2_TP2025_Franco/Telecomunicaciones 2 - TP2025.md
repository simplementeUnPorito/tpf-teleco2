Telecomunicaciones 2 - TP2025


Layer/Scope		Tecnologia			Tarea
APP			WSN				Sensor de temperatura/humedad en aula
APP			WSN				Sensor de humedad del suelo
APP			WSN				Sensor de luz


APP			WIFI-HomeAssistant		Actuador emisor infrarrojo para controlar AACC


FIS			WSN-WIFI			WSN2MQTT Gateway



OFF:
rawIRTimings[200]: 
 -309950
 +9000,-4350
 + 650,-1600 + 650,-1600 + 600,- 500 + 600,- 450
 + 650,- 450 + 650,- 450 + 650,-1600 + 650,-1600
 + 600,-1600 + 650,-1600 + 600,-1600 + 650,- 450
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,-1600
 + 600,- 500 + 600,- 500 + 600,- 450 + 650,- 450
 + 650,- 450 + 650,-1600 + 650,-1600 + 600,-1600
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,- 450
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,- 450
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,- 450
 + 650,- 450 + 650,-1550 + 650,- 450 + 650,-1600
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,- 450
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,- 450
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,- 450
 + 650,- 450 + 650,- 450 + 650,-1550 + 650,- 450
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,- 450
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,- 450
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,- 450
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,- 450
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,- 450
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,- 450
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,- 450
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,- 450
 + 650,-1550 + 650,- 450 + 650,-1600 + 650,- 450
 + 650,- 450 + 650,- 450 + 650,- 450 + 650,- 450
 + 650,-1550 + 650,-1600 + 650
Duration=142050us


ON:
rawIRTimings[200]: 
 -3276750
 [9050,-4350,
 650,-1550,650,-1600,650,-450,650,-450,
 650,-450,650,-450,650,-1550,650,-1600,
 650,-1600,600,-1600,650,-1600,650,-450,
 600,-500,600,-500,600,-500,600,-1600,
 650,-450,650,-450,650,-450,650,-450,
 650,-450,650,-1600,600,-1600,650,-1600,
 650,-450,650,-450,600,-500,600,-500,
 600,-500,600,-500,600,-500,600,-500,
 600,-450,650,-450,650,-450,650,-450,
 650,-450,650,-1600,650,-450,650,-1550,
 650,-450,700,-400,650,-450,650,-450,
 650,-450,650,-450,700,-400,650,-450,
 650,-450,650,-450,650,-450,650,-450,
 650,-450,650,-450,650,-1600,650,-450,
 650,-450,650,-450,650,-450,650,-450,
 650,-450,650,-450,650,-450,650,-450,
 650,-450,650,-450,650,-450,650,-450,
 650,-450,650,-450,650,-450,650,-450,
 650,-450,650,-450,650,-450,650,-450,
 650,-450,650,-1550,650,-450,650,-450,
 650,-450,650,-450,650,-450,650,-450,
 650,-450,650,-450,650,-450,650,-450,
 650,-1600,650,-450,650,-1550,650,-450,
 650,-450,650,-450,650,-450,650,-450,
 650,-1600,600,-1600,650]
Duration=143200us








El sistema integra una red de sensores inalámbricos (WSN) cuyos datos son recolectados por un nodo sink basado en ESP8266, el cual actúa como gateway WSN–WiFi y publica las mediciones en un broker MQTT. Home Assistant consume estos datos para visualización y automatización, permitiendo accionar dispositivos mediante actuadores infrarrojos controlados vía WiFi.



La red de sensores se compone de múltiples nodos sink basados en ESP8266, cada uno encargado de recolectar datos de sensores locales y actuar como gateway WSN–WiFi, publicando directamente las mediciones en el broker MQTT.



Ambos ESP8266 pueden publicar directamente sus mediciones al broker MQTT sin necesidad de centralizar los datos en un único nodo. Cada ESP8266 actúa como nodo sink y gateway WSN–WiFi para los sensores conectados localmente, simplificando la arquitectura y permitiendo una solución escalable y robusta.






Justificación académica de la decisión:

1. Arquitectura adoptada

El sistema se diseñó utilizando una arquitectura distribuida, en la cual múltiples nodos basados en ESP8266 actúan como nodos sink de la red de sensores inalámbricos (WSN). Cada nodo sink se encarga de adquirir las mediciones de los sensores conectados localmente y de funcionar como gateway WSN–WiFi, publicando directamente los datos en un broker MQTT a través de la red IP.

2. Justificación técnica

Esta decisión permite simplificar la arquitectura del sistema, evitando la necesidad de un nodo concentrador adicional y reduciendo la complejidad del intercambio de datos entre dispositivos intermedios. Al disponer de conectividad WiFi nativa, cada ESP8266 puede comunicarse directamente con el broker MQTT, cumpliendo de forma autónoma el rol de gateway entre la WSN y la red IP.

3. Escalabilidad y robustez

La arquitectura distribuida favorece la escalabilidad del sistema, ya que nuevos nodos sensores pueden incorporarse sin modificar la infraestructura existente. Asimismo, mejora la robustez del sistema, dado que la falla de un nodo no compromete el funcionamiento del resto de la red.

4. Integración con la capa de aplicación

Los datos publicados en MQTT son consumidos por Home Assistant, donde se integran como entidades de sensores y se emplean en visualización y automatización. De este modo, se logra una separación clara entre la capa física y de enlace (WSN), la capa de transporte de datos (MQTT) y la capa de aplicación, cumpliendo con el enfoque por capas propuesto en el trabajo práctico.




“Pero entonces… ¿dónde está el gateway WSN–WiFi si hay varios ESP?”
Cada ESP8266 actúa como nodo sink y gateway WSN–WiFi para los sensores conectados localmente. El sistema no se basa en un único gateway centralizado, sino en múltiples gateways distribuidos, lo cual es una arquitectura válida y ampliamente utilizada en sistemas IoT basados en IP.


“¿No sería mejor que uno solo concentre todo?”
La centralización en un único nodo no es estrictamente necesaria dado que todos los nodos cuentan con conectividad WiFi. Centralizar implicaría mayor complejidad, mayor latencia y un punto único de falla, sin aportar beneficios significativos para los requerimientos del sistema.


“Entonces, ¿esto sigue siendo una WSN?”
Sí. La WSN está definida por la existencia de nodos sensores distribuidos que adquieren variables físicas y las transmiten a nodos sink. En este caso, cada ESP8266 cumple el rol de nodo sink para su subconjunto de sensores, manteniendo el concepto de red de sensores inalámbricos, aunque con múltiples puntos de acceso a la red IP.


“¿Por qué MQTT y no otro protocolo?”
MQTT fue elegido por su modelo publish/subscribe, bajo overhead y desacoplamiento entre productores y consumidores de datos, lo cual resulta especialmente adecuado para sistemas IoT y WSN. Además, facilita la integración con plataformas de automatización como Home Assistant.


“¿ESPHome no te está ocultando cosas?”
ESPHome se utiliza como herramienta de abstracción para acelerar el desarrollo en la capa de aplicación y facilitar la integración con Home Assistant. Sin embargo, la arquitectura del sistema, el flujo de datos y la comunicación mediante MQTT son definidos explícitamente, por lo que la solución no depende de mecanismos opacos sino de protocolos estándar.


“¿Qué pasa si un nodo se cae?”
La arquitectura distribuida evita un punto único de falla. La caída de un nodo afecta únicamente a los sensores asociados a ese nodo, mientras que el resto del sistema continúa operando normalmente.





graph TD
    %% Nodos de entrada
    subgraph Capa_Sensores [Nodos]
        N1[ESP8266 + Sensores]
        N2[ESP8266 - Nodo Virtual - ESPHome]
    end

    %% Infraestructura central
    subgraph Servidores [Servidor]
        Broker[Mosquitto MQTT]
        HA[Home Assistant]
    end

    %% Actuación
    subgraph Salida [Capa de Actuación]
        NodeIR[ESP32 con ESPHome]
        AC[Aire Acondicionado]
    end

    %% Flujo de datos corregido
    N1 & N2 -- "Publica (WiFi)" --> Broker
    Broker -- "Escucha/Recibe" --> HA
    
    HA -- "Habla (WiFi)" --> NodeIR
    NodeIR -- "Señal IR" --> AC

    %% Estilos para diferenciar protocolos
    %% Estilos
    style Broker fill:#3498db,color:white
    style HA fill:#f1c40f,color:black
    style NodeIR fill:#2ecc71,color:black
    style N1 fill:#eee,color:black
    style N2 fill:#eee,color:black
    style AC fill:#333,color:white





Control del aire acondiconado (en este caso apagado/encendido a 24grados).

Procedimiento realizado:

Para lograr el control remoto de un aire acondicionado marca Tokyo, se decidió implementar una solución basada en señales infrarrojas en modo raw, debido a que dicho equipo no se encuentra tabulado ni soportado directamente por las librerías estándar de control IR.

En una primera etapa, utilizamos un receptor infrarrojo modelo TL1838, conectado a un Arduino Uno. Para garantizar un funcionamiento correcto del sensor, se siguieron las recomendaciones especificadas en el datasheet del TL1838, implementando el circuito acondicionador correspondiente, incluyendo la correcta alimentación, desacople y conexión de salida del receptor.

Posteriormente, mediante el Arduino IDE, se desarrolló un programa que permitió capturar los pulsos infrarrojos emitidos por el control remoto original del aire acondicionado. El programa fue configurado para medir los tiempos de encendido y apagado de la señal (pulsos raw) y mostrarlos por el monitor serial, expresados en microsegundos. De esta forma, fue posible observar y registrar la secuencia completa de la señal infrarroja asociada al comando de encendido/apagado del equipo.

Una vez obtenidos los datos crudos de la señal, estos fueron copiados y analizados, realizando los ajustes necesarios para asegurar la correcta alternancia de pulsos positivos y negativos, así como la coherencia temporal de los valores. Luego, la señal procesada fue incorporada al archivo de configuración .yaml utilizado en ESPHome.

En la etapa final, se empleó un ESP32 junto con un módulo transmisor infrarrojo, configurado mediante ESPHome para reproducir la señal infrarroja capturada previamente. La transmisión se realizó utilizando el método transmit_raw, permitiendo emitir exactamente la misma secuencia de pulsos registrada desde el control original. De esta manera, se logró el encendido y apagado del aire acondicionado Tokyo de forma confiable.

La decisión de trabajar con señales raw fue fundamental en este caso, ya que el protocolo utilizado por el aire acondicionado no corresponde a ninguno de los protocolos estándar soportados por las librerías IR convencionales. El uso de señales crudas permitió evitar incompatibilidades y garantizar el correcto funcionamiento del sistema de control.







esphome:
  name: aula_sensors

esp32:
  board: esp32dev
  framework:
    type: arduino

logger:
ota:
  platform: esphome

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

api:
  reboot_timeout: 0s

remote_receiver:
  pin:
    number: GPIO14   # receptor IR TL1838
    inverted: true
  dump: raw
  tolerance: 30%

remote_transmitter:
  pin: GPIO4        # transmisor IR
  carrier_duty_percent: 50%

switch:
  - platform: template
    name: "AC ON"
    turn_on_action:
      - remote_transmitter.transmit_raw:
          carrier_frequency: 38000
          code: [
            9059, -4465, 586, -1661, 586, -1661, 586, -523, 587, -524,
            586, -524, 586, -524, 586, -1661, 587, -1660, 587, -1660,
            586, -1660, 587, -1660, 587, -523, 587, -523, 587, -523,
            586, -524, 586, -1660, 587, -525, 585, -524, 587, -523,
            587, -523, 586, -523, 587, -1660, 587, -1659, 586, -1661,
            587, -524, 585, -525, 585, -525, 586, -524, 586, -524,
            585, -524, 586, -524, 587, -524, 586, -523, 587, -523,
            586, -524, 587, -522, 587, -525, 585, -1660, 586, -525,
            585, -1661, 586, -523, 587, -524, 587, -523, 585, -525,
            586, -523, 587, -522, 587, -524, 586, -524, 586, -524,
            586, -523, 587, -523, 586, -523, 587, -523, 587, -524,
            585, -1661, 586, -524, 586, -524, 586, -524, 586, -524,
            586, -524, 586, -523, 587, -523, 586, -524, 586, -524,
            586, -523, 588, -523, 586, -523, 586, -524, 586, -524,
            587, -522, 586, -524, 587, -523, 586, -524, 586, -524,
            586, -524, 587, -522, 587, -523, 586, -1661, 586, -524,
            586, -524, 586, -524, 586, -523, 587, -523, 586, -524,
            586, -524, 586, -524, 586, -524, 585, -524, 586, -1661,
            587, -523, 586, -1661, 585, -524, 587, -523, 586, -524,
            586, -524, 586, -523, 587, -1660, 586, -1661, 586, -1661,
            586, -1660, 586, -525, 585, -1661, 586, -524, 585, -525, 586
          ]

  - platform: template
    name: "AC OFF"
    turn_on_action:
      - remote_transmitter.transmit_raw:
          carrier_frequency: 38000
          code: [
            9083, -4441, 559, -1688, 557, -1690, 558, -552, 557, -553,
            558, -552, 558, -553, 556, -1689, 558, -1690, 558, -1689,
            557, -1689, 559, -1688, 559, -551, 558, -552, 558, -553,
            558, -551, 558, -1689, 558, -552, 557, -553, 558, -552,
            558, -553, 557, -552, 558, -1688, 558, -1689, 558, -1689,
            558, -552, 558, -552, 558, -552, 558, -552, 557, -553,
            556, -554, 557, -553, 557, -552, 557, -554, 557, -553,
            556, -554, 555, -555, 556, -553, 556, -1691, 557, -552,
            556, -1692, 554, -555, 555, -555, 555, -555, 554, -556,
            555, -555, 554, -556, 554, -556, 555, -555, 554, -556,
            554, -556, 554, -556, 553, -557, 554, -556, 553, -556,
            554, -1693, 554, -556, 554, -555, 555, -555, 555, -555,
            555, -555, 554, -556, 554, -556, 554, -555, 555, -556,
            554, -556, 553, -556, 555, -555, 554, -556, 554, -556,
            554, -556, 553, -557, 553, -556, 555, -555, 554, -555,
            555, -555, 554, -557, 554, -555, 554, -556, 554, -556,
            553, -556, 554, -556, 554, -556, 554, -556, 554, -556,
            554, -556, 553, -557, 553, -557, 553, -556, 554, -1693,
            553, -557, 553, -1693, 554, -556, 553, -557, 553, -556,
            554, -556, 554, -556, 554, -1692, 554, -1693, 553, -1693,
            553, -1693, 554, -556, 554, -556, 554, -556, 554, -555,
            554
          ]







Nodo Virtual valores aleatorios:


esphome:
  name: nodo-virtual
  friendly_name: nodo-virtual

esp8266:
  board: esp01_1m

# Enable logging
logger:

ota:
  - platform: esphome
    password: "0269ef18412156e294badeaea850e58d"

wifi:
  ssid: "EstudiantesCYT"
  password: ""  # Red abierta, sin contraseña
  ap:
    ssid: "Nodo-Virtual Fallback Hotspot"
    password: "8J67NBXe3fQl"

captive_portal:

# CONFIGURACIÓN MQTT
mqtt:
  broker: 10.20.2.60   # IP del Mosquitto
  port: 1883
  topic_prefix: "MF/nodo_virtual"

# SENSOR VIRTUAL (Simulación)
sensor:
  - platform: template
    name: "Temperatura"
    unit_of_measurement: "°C"
    device_class: temperature
    state_class: measurement
    lambda: return 22.0 + random_float() * 23.0;  # Caso de verano - hasta 45 °C
    update_interval: 10s

  - platform: uptime
    name: "Tiempo Activo"
    unit_of_measurement: "min"
    update_interval: 60s
    filters:
      - lambda: return x / 60.0;

# VARIABLES GLOBALES
globals:
  - id: random_id
    type: float
    restore_value: no

# INTERVALO DE ACTUALIZACIÓN
interval:
  - interval: 10s
    then:
      - lambda: id(random_id) = random();








Nodo Virtual rampa - prueba para control de AC:


esphome:
  name: nodo-virtual
  friendly_name: nodo-virtual

esp8266:
  board: esp01_1m

# Enable logging
logger:

ota:
  - platform: esphome
    password: "0269ef18412156e294badeaea850e58d"

wifi:
  ssid: "EstudiantesCYT"
  password: ""  # Red abierta, sin contraseña
  ap:
    ssid: "Nodo-Virtual Fallback Hotspot"
    password: "8J67NBXe3fQl"

captive_portal:

# CONFIGURACIÓN MQTT
mqtt:
  broker: 10.20.2.60   # IP del Mosquitto
  port: 1883
  topic_prefix: "MF/nodo_virtual"

# SENSOR VIRTUAL (Simulación)
sensor:
  - platform: template
    name: "Temperatura"
    id: temp_virtual
    unit_of_measurement: "°C"
    device_class: temperature
    state_class: measurement
    lambda: |-
      static float temp = 22.0;
      static bool subiendo = true;
      if (subiendo) {
        temp += 0.5;
        if (temp >= 29.0) subiendo = false;
      } else {
        temp -= 0.5;
        if (temp <= 21.0) subiendo = true;
      }
      return temp;
    update_interval: 10s

  - platform: uptime
    name: "Tiempo Activo"
    unit_of_measurement: "min"
    update_interval: 60s
    filters:
      - lambda: return x / 60.0;

# VARIABLES GLOBALES
globals:
  - id: random_id
    type: float
    restore_value: no

# INTERVALO DE ACTUALIZACIÓN
interval:
  - interval: 10s
    then:
      - lambda: id(random_id) = random();







MQTT Discovery:

El MQTT Discovery es el mecanismo que permite que Home Assistant identifique, configure y habilite dispositivos automáticamente sin que el usuario deba escribir una sola línea de código en el servidor. Su funcionamiento se basa en un "mensaje de presentación" que el microcontrolador envía a un tópico de configuración específico apenas se conecta al Broker. Este mensaje contiene un archivo JSON con todos los metadatos necesarios: el nombre del sensor, la unidad de medida (como Celsius o centímetros), el tipo de dato y el tópico exacto donde se publicarán las lecturas reales.

Cuando Home Assistant detecta este mensaje, crea la entidad de forma instantánea en su base de datos y la pone a disposición del usuario en la interfaz gráfica. El mayor beneficio de este sistema es la escalabilidad y la facilidad de uso ("Plug & Play"), ya que elimina la necesidad de editar archivos manuales (configuration.yaml) cada vez que se agrega un sensor. En plataformas como ESPHome, este proceso es nativo y transparente, mientras que en desarrollos manuales con Arduino IDE, el programador debe asegurar que el mensaje JSON esté perfectamente estructurado y que el buffer de comunicación sea suficiente para enviarlo sin errores.





Flasheo OTA y Gestión de ESPHome:

El proceso de grabar un nuevo archivo .yaml desde el dashboard del navegador se basa en la tecnología OTA (Over-The-Air), la cual permite actualizar el firmware del ESP8266 a través de la red Wi-Fi sin necesidad de una conexión física por USB. Cuando se presiona el botón "Install" y se selecciona la opción inalámbrica, el dashboard compila el código en un archivo binario y lo envía al dispositivo mediante un puerto de red específico (por defecto el 8266). El firmware actual del dispositivo recibe estos datos, los almacena temporalmente en una sección libre de su memoria flash y, tras verificar que el archivo es correcto, reinicia el sistema para ejecutar la nueva versión del código de forma inmediata.

Esta funcionalidad requiere que el archivo .yaml original incluya el componente ota:, que actúa como el receptor de estas actualizaciones. Sin este componente, el dispositivo no sabría cómo escuchar las peticiones de flasheo inalámbrico y quedaría bloqueado para futuras actualizaciones por aire, obligando al usuario a volver al cable USB. Es una herramienta fundamental en entornos de desarrollo porque permite iterar cambios en el código, ajustar sensores o modificar calibraciones de forma remota, siempre que el dispositivo mantenga una conexión estable a la misma red local que el servidor donde corre el dashboard.

En el caso de instalaciones tipo Home Assistant Container, la ausencia de la pestaña de Add-ons y la imposibilidad de flashear directamente desde la interfaz de HA se debe a la arquitectura de aislamiento de Docker. Al ejecutarse como un contenedor independiente, Home Assistant no tiene permisos ni acceso al sistema de archivos o a las herramientas de compilación necesarias para gestionar el ecosistema de ESPHome por sí mismo. En este modelo, cada programa funciona en su propia "caja" cerrada, lo que impide que HA instale complementos internos; por lo tanto, el usuario debe gestionar el dashboard de ESPHome como una entidad separada en el navegador, manteniendo una independencia total entre la visualización de datos y la gestión del hardware.










