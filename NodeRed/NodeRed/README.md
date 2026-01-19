
---

## Node-RED (solo Downlink) – Control de LED y Servo por puertos LoRa

En este proyecto, Node-RED se utiliza únicamente para enviar **downlinks** hacia TTN. Se emplea como capa intermedia porque Home Assistant no pudo publicar directamente downlinks válidos para TTN mediante MQTT, debido a los requisitos estrictos de topic, autenticación y formato del mensaje.

### Mapeo de puertos y comandos

La lógica de control se definió usando puertos LoRaWAN como canales lógicos:

* **f_port = 1**: Control de **LED**

  * `on` → encender LED
  * `off` → apagar LED

* **f_port = 2**: Control de **servo (cortina)**

  * `open` → abrir cortina
  * `close` → cerrar cortina

Node-RED escucha comandos simples desde el broker MQTT local (publicados por Home Assistant) y los transforma al formato requerido por TTN.

---

## Topics MQTT locales (desde Home Assistant hacia Node-RED)

Node-RED se suscribe a los siguientes topics en el broker MQTT local:

* LED: `ha/lora/led/set`
* Cortina: `ha/lora/cortina/set`

Payloads esperados:

* LED: `on` / `off`
* Cortina: `open` / `close`

---

## Publicación del downlink hacia TTN

Node-RED publica el downlink en TTN usando el topic:

`v3/<APP_ID>@ttn/devices/<DEVICE_ID>/down/push`

El payload publicado debe ser un JSON con:

* `f_port` (1 o 2 según el actuador)
* `frm_payload` en Base64 (representando 1 byte de comando)
* prioridad (por ejemplo `NORMAL`)

---

## Implementación en Node-RED: Function Nodes (código)

A continuación se muestran los códigos que deben colocarse en los nodos **function** de Node-RED.
En ambos casos se debe reemplazar `APP_ID` y `DEVICE_ID` por los valores reales del proyecto.

### Function Node: LED (f_port = 1)

```javascript
// Entrada: msg.payload = 'on' o 'off' (string desde MQTT local)
// Salida: msg.topic y msg.payload con formato TTN downlink

const cmd = (msg.payload || '').toString().trim().toLowerCase();
let b;

if (cmd === 'on') b = 0x01;
else if (cmd === 'off') b = 0x00;
else {
  node.warn('CMD LED inválido: ' + cmd);
  return null;
}

const base64 = Buffer.from([b]).toString('base64');

msg.topic = 'v3/APP_ID@ttn/devices/DEVICE_ID/down/push';
msg.payload = {
  downlinks: [{
    f_port: 1,
    frm_payload: base64,
    priority: 'NORMAL'
  }]
};

return msg;
```

### Function Node: Servo cortina (f_port = 2)

```javascript
// Entrada: msg.payload = 'open' o 'close' (string desde MQTT local)
// Salida: msg.topic y msg.payload con formato TTN downlink

const cmd = (msg.payload || '').toString().trim().toLowerCase();
let b;

if (cmd === 'open') b = 0x01;
else if (cmd === 'close') b = 0x00;
else {
  node.warn('CMD cortina inválido: ' + cmd);
  return null;
}

const base64 = Buffer.from([b]).toString('base64');

msg.topic = 'v3/APP_ID@ttn/devices/DEVICE_ID/down/push';
msg.payload = {
  downlinks: [{
    f_port: 2,
    frm_payload: base64,
    priority: 'NORMAL'
  }]
};

return msg;
```

---
