## Interacción entre servidor MQTT y The Things Network (TTN)

La interacción entre un servidor MQTT y The Things Network (TTN) se realiza a través de una interfaz MQTT que actúa como punto de acceso al backend LoRaWAN. Sin embargo, esta interfaz no debe interpretarse como un broker MQTT clásico, ya que su comportamiento y restricciones difieren significativamente según se trate de tráfico ascendente (uplink) o descendente (downlink).

### Comunicación ascendente (uplink)

En el flujo de datos ascendente, cuando un nodo LoRa transmite un paquete LoRaWAN, TTN recibe el mensaje y lo procesa dentro de su backend. Posteriormente, TTN publica automáticamente la información del uplink en su broker MQTT interno, utilizando una estructura de topics predefinida.

Los servicios externos o servidores MQTT pueden suscribirse a estos topics y recibir los mensajes en formato JSON. Desde el punto de vista del cliente, este mecanismo se comporta de manera similar a un esquema clásico de publicación–suscripción, donde TTN actúa como publicador y los servicios externos como suscriptores.

Debido a que el uplink constituye un flujo pasivo de información —los datos ya han sido generados por el nodo y solo deben ser consumidos—, la integración resulta estable y directa. Esto permite que plataformas como Home Assistant, InfluxDB o Node-RED consuman los datos sin necesidad de transformaciones complejas ni lógica adicional en el backend.

### Comunicación descendente (downlink)

El flujo descendente presenta un comportamiento sustancialmente diferente. Un downlink no es un mensaje que se envía de forma inmediata al nodo, sino una solicitud que se encola en el backend de TTN y que solo será transmitida cuando el dispositivo abra una ventana de recepción válida, de acuerdo con el protocolo LoRaWAN.

Para aceptar un downlink, TTN exige que el mensaje publicado vía MQTT cumpla estrictamente con una estructura determinada. Esto incluye el uso de topics específicos asociados a la aplicación y al dispositivo, un formato JSON definido, la codificación del payload en Base64 y la indicación explícita del puerto lógico (f_port). Además, el acceso requiere autenticación mediante claves de API con permisos adecuados.

Estas condiciones hacen que TTN no pueda ser tratado como un broker MQTT genérico para escritura. En este contexto, la interfaz MQTT funciona como una API del backend LoRaWAN y no como un canal de mensajería libre.

### Problemas en la integración directa del downlink

Durante el desarrollo del sistema se intentó implementar un flujo de control directo desde Home Assistant hacia TTN utilizando MQTT. Esta aproximación presentó múltiples inconvenientes. Por un lado, la estructura de autenticación, topics y payload requerida por TTN no es compatible con el modelo de publicación simple utilizado por las integraciones MQTT estándar. Por otro lado, el uso de bridges MQTT bidireccionales no resultó viable, ya que TTN no replica mensajes ni acepta publicaciones arbitrarias como lo haría un broker convencional.

Como consecuencia, los mensajes de control enviados de forma directa no eran aceptados por TTN o eran descartados, impidiendo el envío efectivo de downlinks hacia el nodo LoRa.

### Uso de middleware para la comunicación descendente

Para resolver esta limitación, se incorporó un middleware —en este caso Node-RED— entre el servidor MQTT local y TTN. Este middleware actúa como cliente MQTT de TTN y se encarga de adaptar los comandos de control a la estructura exacta requerida por el backend LoRaWAN. De este modo, Node-RED construye el mensaje JSON correspondiente, selecciona el puerto lógico adecuado y publica el downlink utilizando los mecanismos de autenticación correctos.

Esta solución permite mantener la simplicidad del control a nivel de Home Assistant o del servidor MQTT local, al mismo tiempo que se respetan las restricciones y la semántica del protocolo LoRaWAN impuestas por TTN.

### Síntesis

En síntesis, la interfaz MQTT de TTN funciona de manera eficiente para la recepción de uplinks, ya que estos se comportan como un flujo pasivo de datos. En cambio, el envío de downlinks requiere una interacción más compleja, debido a que MQTT es utilizado por TTN como una API del backend LoRaWAN y no como un broker genérico. Por este motivo, la incorporación de un middleware resulta necesaria para lograr una comunicación descendente confiable y compatible con el sistema.
