## The Things Network (TTN)

**The Things Network (TTN)** es una infraestructura de red **LoRaWAN** de alcance global que permite conectar dispositivos IoT de bajo consumo y largo alcance a Internet de forma segura y escalable. TTN actúa como intermediario entre los **nodos LoRaWAN** y las **aplicaciones finales**, gestionando la comunicación y el encaminamiento de los datos.

La arquitectura de TTN está compuesta por **gateways LoRaWAN**, **servidores de red** y **servicios de aplicación**, permitiendo que los mensajes transmitidos por los nodos sean recibidos por múltiples gateways y procesados de manera centralizada.

---

### Funciones principales

- **Gestión de red LoRaWAN**  
  Administra la activación de dispositivos mediante **OTAA o ABP**, el control de frecuencia, potencia de transmisión y data rate.

- **Recepción y procesamiento de datos (uplinks)**  
  Los mensajes enviados por los nodos LoRaWAN son recibidos por uno o varios gateways y encaminados al servidor de red de TTN.

- **Decodificación de payload**  
  TTN permite convertir el payload binario en datos legibles mediante **Payload Formatters**, generando estructuras en formato **JSON**.

- **Integraciones**  
  Proporciona integraciones nativas como **MQTT**, **HTTP Webhooks** y **Data Storage**, facilitando la conexión con plataformas externas como **Node-RED**, bases de datos y dashboards.

---

### Seguridad

TTN implementa mecanismos de seguridad basados en **cifrado AES-128**, garantizando la confidencialidad e integridad de los datos. En el modo **OTAA**, cada dispositivo utiliza claves únicas para autenticación y comunicación segura.

---

### Rol de TTN en este proyecto

En este proyecto, TTN cumple las siguientes funciones:

- Recepción de los datos enviados por el nodo **Heltec LoRa32u4 II** mediante **LoRaWAN**.
- Decodificación del payload binario en formato **JSON**.
- Publicación de los datos decodificados a través del protocolo **MQTT**.
- Provisión de los datos a **Node-RED** para su visualización y análisis en tiempo real.

---

### Flujo de datos

Sensores → Nodo LoRa → LoRaWAN → TTN → MQTT → Node-RED → Dashboard
