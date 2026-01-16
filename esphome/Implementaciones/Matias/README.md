# ESPHome

## Instalación de Python en Windows

Instalar Python en Windows. Visitar:  
https://www.python.org/downloads/

Instalamos la versión más reciente y estable con instalador binario (por ejemplo, Python 3.12.10), asegurando que durante la instalación esté marcada la casilla **“Add python.exe to PATH”**.

Luego, en PowerShell, ejecutar:

```bash
python --version
pip install --upgrade pip setuptools
```

Es posible que aparezca un mensaje como el siguiente:

```text
ERROR: To modify pip, please run the following command:
C:\Users\FRANCO\AppData\Local\Programs\Python\Python312\python.exe -m pip install --upgrade pip setuptools
```

En ese caso, copiar y pegar exactamente el comando que indica el mensaje. Para este ejemplo:

```bash
C:\Users\FRANCO\AppData\Local\Programs\Python\Python312\python.exe -m pip install --upgrade pip setuptools
```

Luego de que termine de instalar, ejecutar en PowerShell:

```bash
pip install esphome
```

Esperar a que se complete la instalación.

---

## Creación del Dashboard de ESPHome

Crear una carpeta para los proyectos con ESPHome y luego ejecutar en PowerShell:

```bash
esphome dashboard "<aquí va la dirección de la carpeta creada>"
```

Asegurarse de que el servidor esté activo. La ventana de PowerShell (o CMD) que ejecuta el comando `esphome dashboard` debe permanecer abierta y mostrar una línea similar a:

```text
INFO Starting dashboard web server on http://0.0.0.0:6052
```

### Identificar el puerto

Observar la línea:

```text
INFO Starting dashboard web server on http://0.0.0.0:XXXX
```

Donde **XXXX** es el número de puerto que está usando el servidor (generalmente 6052).

<img width="1099" height="365" alt="ESPHome_parte1" src="https://github.com/user-attachments/assets/fb834b0a-fccb-4fb0-8914-2321eaf66ee5" />

### Abrir el Dashboard en el navegador

Abrir el navegador web y acceder a la dirección de loopback (127.0.0.1) utilizando el puerto identificado:

```
http://127.0.0.1:XXXX
```

<img width="1161" height="452" alt="ESPHome_parte2" src="https://github.com/user-attachments/assets/a69cd185-ed11-4c6f-a58c-9c84d967f256" />


**Nota:** El servidor escucha en la IP general `0.0.0.0`, pero se accede localmente utilizando la dirección reservada `127.0.0.1` (localhost) junto con el número de puerto indicado por la consola.

---

## Crear el dispositivo en el Dashboard de ESPHome

Ir al navegador y acceder a:

```
http://127.0.0.1:XXXX
```

(usando el puerto XXXX identificado anteriormente, generalmente 6052).

1. Hacer click en el botón **+ NEW DEVICE**
2. Click en **New Device Setup**

<img width="1918" height="878" alt="ESPHome_parte3" src="https://github.com/user-attachments/assets/5b2c4dc4-5bab-49e7-8d74-2ba85a325aa3" />


<img width="517" height="498" alt="ESPHome_parte4" src="https://github.com/user-attachments/assets/39944593-9b37-4759-80dd-3b6a0812f498" />


Ingresar los detalles solicitados:

- **Name:** nodo-virtual  
- **Wi-Fi:** ingresar el SSID (nombre) y la contraseña de tu red Wi-Fi  
- En **Installation**, seleccionar **Skip** por el momento  
- Seleccionar el tipo de dispositivo (en este caso, **ESP8266**)  
- Finalmente, hacer click en **Install**

---

Luego aparece una ventana con el mensaje:  
**“How do you want to install nodo-virtual.yaml on your device?”**

<img width="712" height="572" alt="esphome_part7" src="https://github.com/user-attachments/assets/e15dfce5-fb36-425e-bb08-fc7398f35e98" />


Por el momento, seleccionar **CANCEL**.

En el Dashboard de ESPHome, hacer click en **EDIT**, lo que abre el archivo de código YAML.


<img width="1261" height="347" alt="ESPHome_parte5" src="https://github.com/user-attachments/assets/9493c8ea-9dfb-4246-a979-bcf038bc7e1c" />


En la sección de Wi-Fi, asegurarse de que esté configurada de la siguiente manera:

```yaml
wifi:
  ssid: "AQUI_VA_EL_NOMBRE_EXACTO_DE_TU_WIFI"
  password: "AQUI_VA_LA_CONTRASEÑA_EXACTA_DE_TU_WIFI"
  min_auth_mode: WPA2  # Añadido para estabilidad
```

En este caso, también se debe incluir la información de MQTT, por ejemplo:

```yaml
# 1. CONFIGURACIÓN MQTT
# Esta sección le dice al nodo dónde está el servidor (Broker)
mqtt:
  broker: <IP del Broker Mosquitto>
  port: 1883        # Puerto estándar para MQTT no cifrado
```

---

## Ejemplo de nodo virtual

Para un ejemplo de un “nodo” virtual, utilizar la siguiente configuración:

```yaml
# 2. EL SENSOR VIRTUAL (Simulación de Telemetría)
sensor:
  - platform: template
    name: "Nodo Virtual - Valor Aleatorio"
    unit_of_measurement: "unidades"
    lambda: return id(random_id) * 100.0;
    update_interval: 10s

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
```

---

Luego, en la esquina superior derecha, hacer click en **SAVE** y luego en **INSTALL**.


<img width="1918" height="470" alt="ESPHome_parte6" src="https://github.com/user-attachments/assets/735435bb-d384-419c-8b63-f5471b3d49ab" />


Vuelve a aparecer la ventana:  
**“How do you want to install nodo-virtual.yaml on your device?”**

Seleccionar **Manual Download**.


<img width="713" height="577" alt="ESPHome_parte7" src="https://github.com/user-attachments/assets/ccd6ffd5-c16e-47cb-a47f-e88c912dba4c" />


---

## IMPORTANTE

Si en esta parte el **INSTALL** (o el **INSTALL** de la parte anterior) presenta problemas:

1. Ir a PowerShell  
2. Presionar **Ctrl + C** dos veces para detener el servidor  
3. Cerrar PowerShell y volver a abrirlo  
4. Ejecutar los siguientes comandos:

```bash
pio platform --global platformio/espressif8266@4.2.1 --remove
pio platform --global platformio/espressif8266@4.2.1
pip install esphome --upgrade
```

Luego reiniciar el Dashboard:

```bash
esphome dashboard "<aquí va la dirección de la carpeta creada>"
```

---

## Instalación final en el ESP

Volviendo al segundo **INSTALL**, esperar a que termine el proceso.  
Si no hay ningún problema, se descargará un archivo `.bin` (en este caso, `nodo-virtual.bin`).


<img width="1748" height="762" alt="ESPHome_parte8" src="https://github.com/user-attachments/assets/f33c8bda-12c4-479b-a4eb-b4c1b9f552ea" />


En una nueva pestaña del navegador, ir a:  
https://web.esphome.io/

Conectar el ESP a la computadora.  
Hacer click en **CONNECT** y seleccionar el puerto al que está conectado el ESP.


<img width="1507" height="577" alt="ESPHome_parte9" src="https://github.com/user-attachments/assets/2ad88cd0-e2ea-4b72-95a7-6998c64490b0" />


En la sección **ESP Device**, hacer click en **INSTALL**, seleccionar el archivo `.bin` (`nodo-virtual.bin`) y luego click en **INSTALL**.


<img width="736" height="257" alt="ESPHome_parte10" src="https://github.com/user-attachments/assets/b005f58c-fd03-4b36-9cd4-8f5fa72c7671" />

<img width="612" height="613" alt="ESPHome_parte11" src="https://github.com/user-attachments/assets/1faf694b-577e-424c-bb47-6556a4c55099" />


Esperar a que finalice la instalación.

Si todo salió correctamente, el Dashboard de ESPHome mostrará el nodo como **ONLINE**.
