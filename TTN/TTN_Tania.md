
---

## TTN – Payload Formatter (Uplink) y Downlink desde Node-RED

En TTN se configuró un **Payload Formatter de uplink** para interpretar los bytes recibidos desde el nodo LoRaWAN y exponerlos como variables legibles (por ejemplo, `humidity` y `light_level`).
El **downlink**, en cambio, no se genera desde TTN manualmente: se envía desde **Node-RED**, que construye el mensaje en el formato estricto requerido por TTN y lo publica por MQTT.

### Uplink: estructura de datos enviada por el nodo

El nodo envía un payload de **3 bytes** con el siguiente formato:

* `bytes[0]` y `bytes[1]`: humedad como entero sin signo de 16 bits (MSB primero), escalada por 100

  * Humedad real = `((bytes[0] << 8) | bytes[1]) / 100`
* `bytes[2]`: nivel de luz (0 a 3), que representa un estado discreto

De esta forma:

* la humedad se transmite con dos decimales (por ejemplo, `5634` → `56.34%`)
* la luz se transmite como un índice de estado (por ejemplo, `2` → “normal”)

### Uplink: decoder configurado en TTN

Se utiliza un **Custom JavaScript decoder** en TTN para transformar esos bytes en un objeto JSON con variables interpretables por los servicios consumidores (MQTT, Home Assistant, InfluxDB, etc.).

```js
function decodeUplink(input) {
  const hum = (input.bytes[0] << 8 | input.bytes[1]) / 100;
  const lvl = input.bytes[2];

  const levels = ["oscuro", "tenue", "normal", "muy iluminado"];

  return {
    data: {
      humidity: hum,
      light_level: lvl,
      light_label: levels[lvl]
    }
  };
}
```

Con este decoder, TTN publica los uplinks por MQTT incluyendo el campo `decoded_payload`, lo que simplifica el procesamiento aguas abajo (no es necesario decodificar bytes fuera de TTN).

---

### Downlink: generado por Node-RED

El downlink no se genera desde Home Assistant hacia TTN de forma directa, ya que TTN exige un topic y un formato de mensaje específico (JSON validado, payload en Base64, puerto lógico, autenticación por API Key).
Por este motivo se utiliza Node-RED como middleware: recibe comandos simples desde el entorno local y los traduce a un downlink válido para TTN.

#### Mapeo de puertos (control)

* `f_port = 1`: control de **LED**

  * `on` → encender
  * `off` → apagar

* `f_port = 2`: control de **servo cortina**

  * `open` → abrir
  * `close` → cerrar

#### Payload de downlink (1 byte)

Para simplificar, cada comando se representa con **un solo byte**:

* `0x01`: activar / abrir
* `0x00`: desactivar / cerrar

TTN requiere que el payload se envíe como `frm_payload` en **Base64**, por lo que Node-RED convierte ese byte a Base64 antes de publicar el JSON del downlink.

Ejemplos de equivalencia:

* `0x01` → Base64: `AQ==`
* `0x00` → Base64: `AA==`

#### Concepto importante: entrega del downlink

Aunque Node-RED publique correctamente el downlink, **TTN no lo entrega instantáneamente**. El mensaje queda encolado y se transmite al nodo únicamente cuando este abre una ventana de recepción (RX1/RX2), lo cual depende del protocolo LoRaWAN y del comportamiento de uplink del dispositivo.

---

