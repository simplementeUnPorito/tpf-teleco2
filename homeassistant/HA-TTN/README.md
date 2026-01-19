
---

## Comunicación descendente (Downlink) y limitaciones con MQTT y Home Assistant

La comunicación descendente en una red LoRaWAN presenta restricciones que la diferencian claramente de un flujo MQTT tradicional. Un *downlink* no es un mensaje que se envía de forma inmediata al nodo, sino una solicitud que se encola en el backend de The Things Network (TTN) y que únicamente será transmitida cuando el dispositivo abra una ventana de recepción válida, de acuerdo con el protocolo LoRaWAN.

TTN expone una interfaz MQTT para la gestión de downlinks, pero dicha interfaz no funciona como un broker MQTT genérico. Para que un downlink sea aceptado, el mensaje publicado debe cumplir estrictamente con una estructura específica: utilizar un *topic* determinado asociado a la aplicación y al dispositivo, incluir un payload codificado en Base64, especificar el puerto lógico (*f_port*) y autenticarse mediante una API Key con permisos adecuados. Cualquier desviación de este formato provoca que el mensaje sea rechazado o descartado por el backend.

Durante el desarrollo del sistema se intentó enviar comandos directamente desde Home Assistant hacia TTN utilizando MQTT. Sin embargo, esta integración no fue posible debido a que la implementación MQTT de Home Assistant está pensada para interactuar con brokers MQTT clásicos, donde los mensajes pueden publicarse libremente en cualquier *topic* con payloads simples. Home Assistant no genera de forma nativa los mensajes con la semántica y el formato requeridos por TTN para la creación de downlinks LoRaWAN.

Asimismo, el uso de un bridge MQTT entre el broker local y TTN no resultó viable. TTN no acepta publicaciones arbitrarias ni replica mensajes como lo haría un broker convencional, ya que su MQTT actúa como una API del backend LoRaWAN y no como un sistema de mensajería bidireccional.

Como consecuencia de estas limitaciones, los comandos enviados desde Home Assistant mediante MQTT no llegaban a convertirse en downlinks válidos y, por lo tanto, no eran entregados al nodo LoRa. Para resolver esta situación fue necesario incorporar una capa intermedia capaz de adaptar los mensajes al formato exigido por TTN.

Esta experiencia evidencia que, si bien MQTT es utilizado como interfaz de acceso a TTN, la comunicación descendente en LoRaWAN no puede implementarse como un flujo MQTT directo entre Home Assistant y el backend, sino que requiere un mecanismo adicional que traduzca los comandos de control a la semántica propia del protocolo LoRaWAN.

---

