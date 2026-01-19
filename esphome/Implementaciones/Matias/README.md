# DESCRIPCIÓN DEL ARCHIVO YAML UTILIZADO EN ESPHOME

Los siguientes archivos .yaml corresponde a la configuraciones cargadas en un microcontrolador ESP8266 real, programado mediante ESPHome. El objetivo de estas configuraciones es que el ESP8266 funcione como un nodo de monitoreo que simula un sensor de temperatura, sin utilizar un sensor físico, y que publique los datos generados a través del protocolo MQTT.

El nodo recibe el nombre “nodo-virtual”, tanto a nivel interno como visible para otros sistemas. Este nombre permite identificar fácilmente el dispositivo dentro de ESPHome y en plataformas externas como Home Assistant o un broker MQTT.

Se especifica que el hardware utilizado es un ESP8266, configurado como una placa del tipo esp01_1m. Esta definición es necesaria para que ESPHome genere el firmware correcto y compatible con las limitaciones de memoria y arquitectura del microcontrolador.

El sistema de logging se encuentra habilitado, lo que permite observar mensajes de estado, errores y valores generados por los sensores. Esta funcionalidad es especialmente útil durante las etapas de prueba y depuración del sistema.

La configuración OTA (Over The Air) permite actualizar el firmware del ESP8266 de forma remota, sin necesidad de volver a conectar el dispositivo por cable USB. Para mayor seguridad, el acceso a esta función se encuentra protegido mediante una contraseña.

En cuanto a la conectividad, el ESP8266 se configura para conectarse a la red WiFi denominada “EstudiantesCYT”, la cual es una red abierta y no requiere contraseña. En caso de que el dispositivo no pueda conectarse a dicha red, se habilita automáticamente un punto de acceso propio con el nombre “Nodo-Virtual Fallback Hotspot”. Este punto de acceso permite acceder al dispositivo para tareas de mantenimiento o reconfiguración.

Además, se habilita el portal cautivo, el cual facilita la configuración del dispositivo mediante una interfaz web cuando el ESP8266 se encuentra en modo punto de acceso.

Para la comunicación de datos se utiliza el protocolo MQTT. El ESP8266 se conecta a un broker Mosquitto ubicado en la dirección IP 10.20.2.60, utilizando el puerto estándar 1883. Todos los mensajes publicados por el nodo utilizan el prefijo de tópico “MF/nodo_virtual”, lo que permite organizar y distinguir fácilmente los datos provenientes de este dispositivo dentro del broker MQTT.

El archivo define un sensor de tipo “template”, el cual permite crear sensores virtuales mediante código. En este caso, se implementa un sensor de temperatura que no depende de ningún sensor físico. El valor de la temperatura se genera mediante una función lambda, que produce valores aleatorios comprendidos aproximadamente entre 22 °C y 45 °C, simulando condiciones ambientales elevadas, como las que podrían darse en un día de verano. El valor del sensor se actualiza cada 10 segundos.

Adicionalmente, se configura un sensor de tiempo activo (uptime), el cual mide el tiempo que el ESP8266 lleva encendido. Este valor se actualiza cada 60 segundos y se convierte de segundos a minutos mediante un filtro, facilitando su interpretación.

Se definen también variables globales internas al firmware. En este caso, se utiliza una variable de tipo flotante que almacena un valor aleatorio y que no se conserva tras un reinicio del dispositivo. Esta variable sirve como ejemplo del uso de memoria interna y puede ser utilizada para futuras ampliaciones del sistema.

Finalmente, se configura una tarea periódica mediante la directiva “interval”. Cada 10 segundos, el ESP8266 ejecuta una función que genera un nuevo valor aleatorio y lo almacena en la variable global definida previamente. Esta estructura permite simular comportamientos dinámicos y extender fácilmente la lógica del nodo.

En conjunto, esta configuración permite que un ESP8266 real actúe como un nodo de prueba capaz de simular un sensor de temperatura, conectarse a una red WiFi, publicar datos mediante MQTT y ser integrado en sistemas de monitoreo o automatización, todo sin necesidad de sensores físicos adicionales.



