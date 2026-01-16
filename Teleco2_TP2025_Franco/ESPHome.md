
ESPHome:


Instalar Python en Windows. Visitar https://www.python.org/downloads/

Instalamos la versión más reciente y estable con instalador binario (ej. Python 3.12.10), asegurando que durante la instalación esté marcada la casilla "Add python.exe to PATH".

Luego en el PowerShell:

python --version
pip install --upgrade pip setuptools

Dirá algo como:
ERROR: To modify pip, please run the following command:
C:\Users\FRANCO\AppData\Local\Programs\Python\Python312\python.exe -m pip install --upgrade pip setuptools

Entonces, copiar y pegar el comando que indica. Para este caso:
C:\Users\FRANCO\AppData\Local\Programs\Python\Python312\python.exe -m pip install --upgrade pip setuptools

Luego de que termine de instalar, ejecutar en el PowerShell
pip install esphome
esperar a que se instale...

Crear una carpeta para los proyectos con ESPHome y luego ejecutar en PowerShell:
esphome dashboard "< aqui va la direccion de la carpeta creada >"

Asegurarse de que el servidor esté activo: La ventana de PowerShell (o CMD) que ejecuta el comando esphome dashboard... debe estar abierta y mostrando la línea INFO Starting dashboard web server on http://0.0.0.0:6052...

Identificar el Puerto: Observar la línea INFO Starting dashboard web server on http://0.0.0.0:XXXX.
XXXX es el número de puerto que está usando el servidor (generalmente 6052).

Abrir en el Navegador: Ir a tu navegador web y abrir la dirección de loopback (127.0.0.1), usando el puerto XXXX que encontraste en la consola:

http://127.0.0.1:XXXX

(Nota: El servidor escucha en la IP general 0.0.0.0, pero accedemos a él localmente usando la dirección reservada 127.0.0.1 [localhost] y el número de puerto que te indica la consola.)




Crear el Dispositivo en el Dashboard de ESPHome:
Ir al navegador y usar la dirección http://127.0.0.1:XXXX (usando el puerto XXXX que identificaste en la consola, generalmente 6052).
Hacer click en el botón + NEW DEVICE.
Click en New Device Setup

Ingresar los detalles solicitados:
Name: nodo-virtual
Wi-Fi: Ingresar el SSID (Nombre) y la Contraseña de tu red Wi-Fi.
En Installation le podemos dar Skip por el momento.
Luego seleccionar el tipo de dispositivo (en este caso fue el esp8266).
Luego le damos a Install.



Luego sale una ventana que dice: "How do you want to install nodo-virtual.yaml on your device?"
Le damos a CANCELL por ahora.
En el dashboard del ESPHome, damos click en EDIT. Esto nos abre el codigo yaml.
en la parte de wifi hay que asegurarse que esté así:
wifi:
  ssid: "AQUI_VA_EL_NOMBRE_EXACTO_DE_TU_WIFI" 
  password: "AQUI_VA_LA_CONTRASEÑA_EXACTA_DE_TU_WIFI"
  min_auth_mode: WPA2 # Añadido para estabilidad

y también incluir la información del mqtt, por ejemplo:

# 1. CONFIGURACIÓN MQTT
# Esta sección le dice al nodo dónde está el servidor (Broker)
mqtt:
  broker: < IP del Broker Mosquitto > 
  port: 1883        # <--- Puerto estándar para MQTT no cifrado


Para un ejemplo de un "nodo" virtual, use esto:

# 2. EL SENSOR VIRTUAL (Simulación de Telemetría)
sensor:
  # Genera un valor aleatorio entre 0 y 100
  - platform: template
    name: "Nodo Virtual - Valor Aleatorio"
    unit_of_measurement: "unidades"
    lambda: return id(random_id) * 100.0; 
    update_interval: 10s
    
  # Sensor Secundario: Muestra el tiempo de actividad del nodo
  - platform: uptime 
    name: "Tiempo Activo del Nodo"
    unit_of_measurement: "min"
    update_interval: 60s

# 3. VARIABLES Y LÓGICA DE LA SIMULACIÓN
globals:
  - id: random_id 
    type: float
    restore_value: no
    
interval:
  - interval: 10s 
    then:
      - lambda: id(random_id) = random();



Luego, en la esquina superior derecha le damos a SAVE, y luego a INSTALL.
Vuelve a salir la ventana que dice: "How do you want to install nodo-virtual.yaml on your device?"
Darle click a Manual Download


IMPORTANTE!
Si en esta parte el INSTALL o el INSTALL de la parte anterior dan problema:
Ir al PowerShell y en el terminal darle 2 veces crtl + C para detener el servidor.
Cerrar el PowerShell, y volver a abrirlo.
Ejecutar los siguientes en el PowerShell:

pio platform --global platformio/espressif8266@4.2.1 --remove 

pio platform --global platformio/espressif8266@4.2.1

pip install esphome --upgrade

Luego reiniciar el Dashboard:
esphome dashboard "< aqui va la direccion de la carpeta creada >"


Volviendo al segundo INSTALL...
esperar a que termine de instalarse... si no hay ningun problema, se va a descargar un archivo .bin (para este caso se llama nodo-virtual.bin)

En una nueva pestaña del navegador ir a: https://web.esphome.io/
Conectar el esp a la computadora.
Darle click en CONNECT, seleccionar el puerto al que está conectado el esp.

En la parte de ESP Device, darle click a INSTALL, seleccionar el archivo .bin (nodo-virtual.bin), y le damos click a INSTALL.
Esperamos a que termine de instalar...

Si está todo bien, debería aparecernos en el dashboard del ESPHome que nuestro nodo está "ONLINE".



















CODIGO .yaml utilizado:

Para este trabajo, se creó un nodo virtual usando el ESPHome con el siguiente codigo .yaml:


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



DESCRIPCIÓN DEL ARCHIVO YAML UTILIZADO EN ESPHOME

El siguiente archivo .yaml corresponde a la configuración cargada en un microcontrolador ESP8266 real, programado mediante ESPHome. El objetivo de esta configuración es que el ESP8266 funcione como un nodo de monitoreo que simula un sensor de temperatura, sin utilizar un sensor físico, y que publique los datos generados a través del protocolo MQTT.

El nodo recibe el nombre “nodo-virtual”, tanto a nivel interno como visible para otros sistemas. Este nombre permite identificar fácilmente el dispositivo dentro de ESPHome y en plataformas externas como Home Assistant o un broker MQTT.

Se especifica que el hardware utilizado es un ESP8266, configurado como una placa del tipo esp01_1m. Esta definición es necesaria para que ESPHome genere el firmware correcto y compatible con las limitaciones de memoria y arquitectura del microcontrolador.

El sistema de logging se encuentra habilitado, lo que permite observar mensajes de estado, errores y valores generados por los sensores. Esta funcionalidad es especialmente útil durante las etapas de prueba y depuración del sistema.

La configuración OTA (Over The Air) permite actualizar el firmware del ESP8266 de forma remota, sin necesidad de volver a conectar el dispositivo por cable USB. Para mayor seguridad, el acceso a esta función se encuentra protegido mediante una contraseña.

En cuanto a la conectividad, el ESP8266 se configura para conectarse a la red WiFi denominada “EstudiantesCYT”, la cual es una red abierta y no requiere contraseña. En caso de que el dispositivo no pueda conectarse a dicha red, se habilita automáticamente un punto de acceso propio con el nombre “Nodo-Virtual Fallback Hotspot”. Este punto de acceso permite acceder al dispositivo para tareas de mantenimiento o reconfiguración.

Además, se habilita el portal cautivo, el cual facilita la configuración del dispositivo mediante una interfaz web cuando el ESP8266 se encuentra en modo punto de acceso.

Para la comunicación de datos se utiliza el protocolo MQTT. El ESP8266 se conecta a un broker Mosquitto ubicado en la dirección IP 10.20.2.60, utilizando el puerto estándar 1883. Todos los mensajes publicados por el nodo utilizan el prefijo de tópico “MF/nodo_virtual”, lo que permite organizar y distinguir fácilmente los datos provenientes de este dispositivo dentro del broker MQTT.

El archivo define un sensor de tipo “template”, el cual permite crear sensores virtuales mediante código. En este caso, se implementa un sensor de temperatura que no depende de ningún sensor físico. El valor de la temperatura se genera mediante una función lambda, que produce valores aleatorios comprendidos aproximadamente entre 22 °C y 45 °C, simulando condiciones ambientales elevadas, como las que podrían darse en un día de verano. El valor del sensor se actualiza cada 10 segundos.

Adicionalmente, se configura un sensor de tiempo activo (uptime), el cual mide el tiempo que el ESP8266 lleva encendido. Este valor se actualiza cada 60 segundos y se convierte de segundos a minutos mediante un filtro, facilitando su interpretación.

Se definen también variables globales internas al firmware. En este caso, se utiliza una variable de tipo flotante que almacena un valor aleatorio y que no se conserva tras un reinicio del dispositivo. Esta variable sirve como ejemplo del uso de memoria interna y puede ser utilizada para futuras ampliaciones del sistema.

Finalmente, se configura una tarea periódica mediante la directiva “interval”. Cada 10 segundos, el ESP8266 ejecuta una función que genera un nuevo valor aleatorio y lo almacena en la variable global definida previamente. Esta estructura permite simular comportamientos dinámicos y extender fácilmente la lógica del nodo.

En conjunto, esta configuración permite que un ESP8266 real actúe como un nodo de prueba capaz de simular un sensor de temperatura, conectarse a una red WiFi, publicar datos mediante MQTT y ser integrado en sistemas de monitoreo o automatización, todo sin necesidad de sensores físicos adicionales.









Codigo utilizado para segunda prueba:

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
  #ssid: "Internet Personal " 
  #ssid: "TrickPhone"
  ssid: "InvestigadoresCYT"
  #password: "011141179595+"
  #password: "ffuiibo-yff-442hhs-rrg"
  password: "investigacion2018"
  min_auth_mode: WPA2 # Añadido para estabilidad

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "Nodo-Virtual Fallback Hotspot"
    password: "8J67NBXe3fQl"


captive_portal:

# 1. CONFIGURACIÓN MQTT
# Esta sección le dice al nodo dónde está el servidor (Broker)
mqtt:
  broker: 172.19.0.2   # IP de tu Mosquitto
  port: 1883
  topic_prefix: "MF/nodo_virtual"


# 2. EL SENSOR VIRTUAL (Simulación de Telemetría)
sensor:
  # Genera un valor aleatorio entre 0 y 100
  - platform: template
    name: "Nodo Virtual - Valor Aleatorio"
    unit_of_measurement: "unidades"
    lambda: return random_float() * 100.0; # Genera un valor entre 0 y 100
    update_interval: 10s
    
  # Sensor Secundario: Muestra el tiempo de actividad del nodo
  - platform: uptime 
    name: "Tiempo Activo del Nodo"
    unit_of_measurement: "min"
    update_interval: 60s

# 3. VARIABLES Y LÓGICA DE LA SIMULACIÓN
globals:
  - id: random_id 
    type: float
    restore_value: no
    
interval:
  - interval: 10s 
    then:
      - lambda: id(random_id) = random();


