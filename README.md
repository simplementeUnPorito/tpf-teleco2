# **TPF – Telecomunicaciones II (2025)**

## **Infraestructura IoT Multiplataforma – DEI/LED**

### Universidad Católica “Nuestra Señora de la Asunción”

Facultad de Ciencias y Tecnologías
Departamento de Ingeniería Electrónica e Informática

---

## **🎯 Objetivo del Proyecto**

Este proyecto tiene como finalidad **diseñar, implementar y documentar** una infraestructura IoT completa utilizando:

* **Home Assistant** como servidor central
* **ESPHome** para sensores y actuadores WiFi
* **Redes WSN** basadas en CTP *(postergado según indicación del profesor)*
* **LoRa / LoRaWAN** *(parcialmente postergado)*
* **MQTT, Grafana, InfluxDB y Telegraf** para telemetría *(según tareas asignadas)*
* Acceso remoto seguro mediante **VPN Tailscale**

El servidor principal se encuentra en una PC de la facultad y los estudiantes pueden acceder de forma remota mediante Tailscale para desarrollar, integrar y probar cada componente del sistema.

Este repositorio centraliza **todo el código, documentación y configuraciones**.

---

# **👥 Integrantes y distribución de tareas**

A continuación se presenta la tabla oficial de distribución de tareas del proyecto, basada en el documento entregado por el profesor. Se incluyen todos los módulos, tecnologías, cantidades, cargas de trabajo y responsables.

| Layer/Scope | Technology                    | Tarea                                          | Qty | Workload | Elías | Joel | Matías | Tania | Erick | Available |
| ----------- | ----------------------------- | ---------------------------------------------- | --- | -------- | ----- | ---- | ------ | ----- | ----- | --------- |
| APP         | WSN                           | Sensor de Temperatura/Humedad en aula          | 10  | 15       | 3     | 3    | 3      |       | 1     | 1         |
| APP         | Lora                          | Sensor de presencia/cruce/apertura             | 5   | 10       |       | 1    |        |       | 4     |           |
| APP         | Lora                          | Sensor de luz                                  | 2   | 10       |       |      |        | 1     | 1     |           |
| APP         | Lora                          | Sensor de humedad del suelo                    | 2   | 10       |       |      |        | 1     | 1     |           |
| APP         | WIFI Telegraf-Influx          | Sensor de humedad del suelo                    | 2   | 15       |       |      | 1      |       | 1     | 0         |
| APP         | WIFI ESP Home                 | Sensor de luz                                  | 4   | 15       | 1     | 1    | 1      |       | 1     | 0         |
| APP         | WIFI-HomeAssistant            | Actuador Emisor infrarrojo para controlar AACC | 2   | 30       | 1     |      | 1      |       |       | 0         |
| APP         | WIFI-HomeAssistant            | Actuador…                                      | 2   | 15       |       |      | 2      |       |       | 0         |
| APP         | WIFI-HomeAssistant            | Actuador Dispensador Spray                     | 2   | 15       |       |      |        | 1     | 1     |           |
| APP         | WIFI-MQTT                     | Actuador Emisor infrarrojo para controlar AACC | 2   | 30       |       |      |        |       | 2     |           |
| APP         | WIFI-MQTT                     | Actuador Dispensador Spray                     | 2   | 15       |       |      |        |       | 2     |           |
| APP         | WIFI-MQTT                     | Actuador…                                      | 2   | 15       |       |      |        |       | 2     |           |
| MAC         | WSN                           | CTP para recolección de datos                  | 1   | 20       | 1     |      |        |       |       | 0         |
| MAC         | WSN                           | Flooding Protocol por downlink comm            | 1   | 20       |       | 1    |        |       |       | 0         |
| FIS         | WSN-WIFI                      | WSN2MQTT Gateway                               | 2   | 30       |       |      | 1      |       | 1     | 0         |
| FIS         | WSN-LORA                      | WSN2LORA Gateway                               | 2   | 30       |       | 1    | 1      |       |       | 0         |
| FIS         | LORAWAN-TTN                   | LORAWAN Gateway                                | 0   | 20       |       |      |        |       |       | 0         |
| SERVERS     | Telegraf + Grafana + InfluxDB | —                                              | 1   | 30       |       |      |        |       | 1     | 0         |
| SERVERS     | HomeAssistant                 | —                                              | 2   | 30       | 1     |      |        |       | 1     | 0         |
| SERVERS     | Mosquitto                     | MQTT Broker                                    | 1   | 25       |       |      |        |       | 1     |           |
---

# **🖥️ Servidor IoT de la Facultad**

El servidor corre:

* **Ubuntu 24.04**
* **Home Assistant en Docker**
* **ESPHome en Docker**
* **OpenSSH Server**
* **VPN Tailscale**

Esto permite:

✔ Acceso remoto sin abrir puertos
✔ Desarrollo colaborativo entre todos los estudiantes
✔ Integración centralizada de sensores y actuadores
✔ Estandarización para pruebas de laboratorio

---

# **🔐 Acceso remoto mediante Tailscale**

Tailscale crea una VPN mesh segura basada en WireGuard. Cada dispositivo del equipo se conecta a la misma red virtual.

## **1️⃣ Crear una cuenta**

Ingresar a: [https://tailscale.com](https://tailscale.com)
Registrarse con Google / GitHub / Microsoft.

## **2️⃣ Aceptar invitación del administrador**

Uno de los integrantes enviará el "invite link" del proyecto.
Al aceptarlo, tu dispositivo queda autorizado.

## **3️⃣ Instalar Tailscale**

En Linux:

```
curl -fsSL https://tailscale.com/install.sh | sh
```

En Windows / Android / iOS:
Descargar desde [https://tailscale.com/download](https://tailscale.com/download).

## **4️⃣ Levantar la VPN**

```
sudo tailscale up
```

## **5️⃣ Ver la IP del servidor y los compañeros**

En el panel web de Tailscale:
[https://login.tailscale.com/admin/machines](https://login.tailscale.com/admin/machines)

Ahí se puede ver:

* IP Tailscale del servidor
* IPs de cada compañero
* Estado (online/offline)

---

# **🌐 Acceso al Home Assistant**

En un navegador web:

```
http://[IP_TAILSCALE_DEL_SERVIDOR]:8123
```

Ejemplo:

```
http://100.109.64.19:8123
```

---

# **💻 Acceso por SSH al servidor**

En terminal:

```
ssh [IP_DEL_SERVIDOR] -l user
```

Contraseña:

```
user
```

Ejemplo:

```
ssh 100.109.64.19 -l user
```

---
**Versión del documento:** 1.0
