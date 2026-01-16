
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



