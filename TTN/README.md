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

## 6. Configuración en The Things Network (TTN)

Esta sección documenta el proceso de creación de la aplicación en **The Things Network (TTN)**, el registro del dispositivo utilizando **OTAA** y la configuración del **payload formatter (decoder)** para interpretar los datos recibidos desde el nodo LoRaWAN.

---

### 6.1 Crear aplicación

Accedemos a la página oficial de TTN: https://id.thethingsnetwork.org/


e iniciamos sesión con el usuario habilitado. Una vez dentro de la consola, seguimos los pasos detallados a continuación:

1. Crear una nueva **Application**.
2. Insertar el **Application ID** y el **nombre de la aplicación**.

> **Figura 6:** Creación de la aplicación en TTN.  
*(Insertar captura de pantalla correspondiente)*

---

### 6.2 Registrar dispositivo (OTAA)

Una vez creada la aplicación, procedemos a registrar el dispositivo final (End Device):

1. Seleccionar la opción **Add end device**.
2. Elegir como **Activation mode: OTAA (Over-The-Air Activation)**.
3. Copiar y guardar los siguientes identificadores y claves:
   - **DevEUI**
   - **JoinEUI**
   - **AppKey**

Estos valores serán utilizados posteriormente en el código del nodo LoRa.

> **Figura 7:** Registro del dispositivo final en TTN.  
*(Insertar captura de pantalla correspondiente)*

---

### 6.3 Payload Formatter (Uplink Decoder)

Luego de registrar el dispositivo, ingresamos a la pestaña **Payload Formatters** dentro de la aplicación y seleccionamos la opción **Custom JavaScript Formatter**.

En el campo correspondiente, se pega el siguiente código para decodificar el payload binario recibido desde el nodo:

```js
function decodeUplink(input) {
  return {
    data: {
      luz: input.bytes[0],
      ir: input.bytes[1],
      temp_c: ((input.bytes[2] << 8) | input.bytes[3]) / 100,
      hum_rh: ((input.bytes[4] << 8) | input.bytes[5]) / 100
    }
  };
}

Este decoder interpreta el payload enviado por el nodo y lo convierte en datos legibles en formato JSON.
---

### 6.4 Recepción de paquetes en TTN

Una vez finalizada la configuración del payload formatter, accedemos a la pestaña Live Data dentro de la aplicación.

En esta sección se deben visualizar los paquetes uplink recibidos por el gateway TTN, mostrando tanto el payload crudo como los datos ya decodificados.

---

### 7. Integración MQTT

Para el consumo y procesamiento de los datos enviados por el nodo, se utiliza el protocolo MQTT, permitiendo recibir los uplinks publicados por TTN y reenviarlos o procesarlos en sistemas externos.


---

### 7.1 MQTT en TTN

Dentro de la TTN Console, se accede a: Integrations → MQTT

En esta sección se identifican los siguientes parámetros necesarios para la conexión:

Host

Puerto: 8883 (TLS)

Username


Importante:
Es necesario hacer clic en Generate new API key y guardar dicha clave, ya que será utilizada posteriormente para la conexión desde Node-RED.

---

### 7.2 Topics recomendados

Los siguientes topics MQTT son utilizados para la recepción de los mensajes uplink:

Uplink por dispositivo: v3/<app>@<tenant>/devices/<device_id>/up

Uplink para todos los dispositivos de la aplicación: v3/<app>@<tenant>/devices/+/up

Estos topics permiten suscribirse a los mensajes enviados por uno o varios dispositivos registrados en la aplicación TTN.

