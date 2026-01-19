## ¿Qué es Node-RED?

**Node-RED** es una herramienta de desarrollo basada en **flujos (flows)** que permite integrar dispositivos, servicios y aplicaciones mediante programación visual. Su funcionamiento se basa en el uso de **nodos** conectados entre sí, donde cada nodo cumple una función específica (por ejemplo: recibir datos, transformar mensajes, publicar en MQTT o visualizar información).  
Node-RED se ejecuta sobre **Node.js** y es ampliamente utilizado en proyectos **IoT** por su facilidad para conectar múltiples tecnologías sin necesidad de desarrollar toda la lógica desde cero.

---

## ¿Cómo se utilizó Node-RED en este proyecto?

En este proyecto, Node-RED fue utilizado como **plataforma de integración y visualización** para procesar los datos enviados por el nodo LoRaWAN y mostrarlos en tiempo real en un dashboard.

El rol de Node-RED dentro del sistema fue:

- **Suscribirse** al broker MQTT de **TTN** para recibir los uplinks del dispositivo.
- **Decodificar y extraer** los valores ya formateados en `decoded_payload` (luz, temperatura, humedad e IR).
- **Distribuir** los datos hacia diferentes salidas (dashboard y/o otros servicios).
- **Visualizar** los datos mediante medidores y gráficos históricos usando Node-RED Dashboard.

---

## Flujo de integración implementado

El flujo general de datos implementado con Node-RED fue el siguiente:

TTN (MQTT) → Node-RED (procesamiento) → Dashboard (visualización)


### 1) Recepción de datos desde TTN vía MQTT
Se configuró un nodo **MQTT IN** en Node-RED conectado al broker de TTN (TLS, puerto 8883).  
El nodo se suscribió al topic del dispositivo: v3/<app>@<tenant>/devices/<device_id>/up


De esta forma, Node-RED recibe cada uplink que TTN publica cuando el nodo LoRa envía una lectura.

---

### 2) Conversión del mensaje a objeto (JSON)
El mensaje MQTT recibido contiene un JSON con información completa del uplink.  
Para poder acceder a los campos internos, se utilizó un nodo **JSON** (o salida parseada directamente), obteniendo una estructura accesible en `msg.payload`.

---

### 3) Extracción y preparación de variables del sensor
A partir de `msg.payload.uplink_message.decoded_payload`, se extrajeron las variables del proyecto:

- `temp_c` → Temperatura (°C)
- `hum_rh` → Humedad (%)
- `luz` → Nivel de luz (valor escalado)
- `ir` → Estado del sensor infrarrojo (0/1)

Se utilizó un nodo **Function** para generar salidas separadas por variable, asignando `msg.topic` y `msg.payload` adecuados para los widgets del dashboard.

---

### 4) Visualización en Node-RED Dashboard
Se creó un dashboard (`/ui`) utilizando los nodos de **node-red-dashboard**, incorporando:

- **Gauges** para temperatura, humedad y luz.
- **Indicador (LED o texto)** para el sensor IR.
- **Gráficos históricos (ui_chart)** para variables continuas.
- Un gráfico separado para **IR**, debido a que su naturaleza es binaria (0/1), evitando que se “pierda” en la escala de las otras variables.

---

## Ventajas de utilizar Node-RED en este proyecto

El uso de Node-RED resultó conveniente por las siguientes razones:

- Permite integrar TTN/MQTT de manera rápida y visual.
- Facilita el procesamiento de mensajes sin desarrollar una aplicación completa.
- Permite crear dashboards en tiempo real sin herramientas externas.
- Es modular y escalable: se pueden añadir más sensores, alarmas o integraciones (bases de datos, Home Assistant, webhooks, etc.) con pocos cambios.

---

## Conclusión

Node-RED permitió implementar una solución completa de integración para el proyecto, actuando como intermediario entre **TTN (MQTT)** y la **visualización final**, logrando un flujo funcional y verificable desde los sensores físicos hasta el dashboard en tiempo real, con posibilidad de expansión a futuras integraciones.

# Anexo- Guia de instalación

## 1. Instalación y configuración de Node-RED (Windows 11)

Esta sección describe el procedimiento de instalación y configuración del entorno necesario para utilizar **Node-RED** en el sistema operativo **Windows 11**, incluyendo la instalación de **Node.js**, **Node-RED** y los paquetes requeridos para la integración MQTT y la visualización mediante dashboard.

---

### 1.1 Instalación de Node.js (LTS)

Node-RED se ejecuta sobre **Node.js**, por lo que es un requisito previo contar con este entorno instalado.

Los pasos seguidos fueron los siguientes:

1. Descargar la versión **LTS (Long Term Support)** de Node.js desde el sitio oficial: https://nodejs.org/
2. Ejecutar el instalador y completar la instalación utilizando las **opciones por defecto**.
3. Una vez finalizada la instalación, abrir la consola de comandos (**CMD**) y verificar la correcta instalación ejecutando:

```bash
node -v
npm -v
```
Si ambos comandos devuelven un número de versión, la instalación se realizó correctamente.


---

### 1.2 Instalación de Node-RED

Con Node.js instalado, se procede a la instalación de Node-RED mediante el gestor de paquetes npm.

Abrir la consola de comandos (CMD) con permisos de usuario.

Ejecutar el siguiente comando para instalar Node-RED de forma global:
npm install -g --unsafe-perm node-red

Una vez finalizada la instalación, iniciar Node-RED ejecutando:
node-red

Si el inicio es correcto, la consola mostrará mensajes indicando que el servidor está activo y escuchando conexiones.


---

### 1.3 Acceso a la interfaz web de Node-RED
Con Node-RED en ejecución, se accede a la interfaz gráfica desde un navegador web ingresando la siguiente dirección:  http://localhost:1880/
Esta interfaz permite crear y editar flujos mediante programación visual, arrastrando y conectando nodos.


---

### 1.4 Instalación de paquetes adicionales en Node-RED

Para implementar la integración MQTT con TTN y la visualización de datos, es necesario instalar paquetes adicionales.

El procedimiento fue el siguiente:

En la interfaz web de Node-RED, hacer clic en el menú superior derecho (ícono de tres líneas).

Seleccionar la opción Manage palette.

Ingresar a la pestaña Install.

Buscar e instalar los siguientes paquetes:

node-red-node-mqtt

node-red-dashboard

Estos paquetes agregan nodos específicos para la conexión a brokers MQTT y la creación de dashboards interactivos.


---

### 1.5 Verificación de la instalación

Una vez instalados los paquetes:

Deben aparecer nodos mqtt in / mqtt out en la paleta.

Deben aparecer nodos ui_gauge, ui_chart, ui_text, entre otros, correspondientes al dashboard.

El dashboard estará disponible en: http://localhost:1880/ui


Con esto, el entorno Node-RED queda correctamente configurado para integrarse con TTN vía MQTT y visualizar los datos del proyecto.


---

### 1.6 Rol de Node-RED en el proyecto

En este proyecto, Node-RED actúa como una plataforma central de integración, permitiendo:

Recibir los datos enviados por el nodo LoRaWAN a través de MQTT.

Procesar y separar las variables de los sensores.

Visualizar los datos en tiempo real mediante un dashboard web.

Facilitar futuras ampliaciones del sistema sin modificar el firmware del nodo.
