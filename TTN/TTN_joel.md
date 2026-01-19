## 6. Configuración en The Things Network (TTN)

Esta sección documenta el proceso de creación de la aplicación en **The Things Network (TTN)**, el registro del dispositivo utilizando **OTAA** y la configuración del **payload formatter (decoder)** para interpretar los datos recibidos desde el nodo LoRaWAN.

---

### 6.1 Crear aplicación

Accedemos a la página oficial de TTN: https://id.thethingsnetwork.org/


e iniciamos sesión con el usuario habilitado. Una vez dentro de la consola, seguimos los pasos detallados a continuación:

1. Crear una nueva **Application**.
2. Insertar el **Application ID** y el **nombre de la aplicación**.



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



