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


