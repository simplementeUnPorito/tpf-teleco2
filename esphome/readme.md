# **ESPHome – Implementación en Docker**

ESPHome se utiliza para **crear, compilar y administrar** los dispositivos WiFi del proyecto (ESP8266 y ESP32). En la PC del servidor de la facultad se creó un contenedor dedicado para que todos los estudiantes puedan usar el panel desde Home Assistant o desde la VPN.

A continuación se detalla cómo fue creado el contenedor y cómo puede replicarse localmente.

---

## **🧱 1. Contenedor ESPHome en el servidor de la facultad**

El contenedor oficial se creó con este comando:

```bash
docker run -d \
  --name esphome \
  --restart unless-stopped \
  -v /home/user/esphome:/config \
  --net=host \
  --privileged \
  --device=/dev/ttyUSB0 \
  --device=/dev/ttyACM0 \
  esphome/esphome
```

### 📌 **Descripción de parámetros**

* `-v /home/user/esphome:/config` → Guarda todos los proyectos ESPHome en el servidor.
* `--net=host` → Permite acceder al panel directamente vía:

  ```
  http://IP_DEL_SERVIDOR:6052/
  ```
* `--privileged` y `--device=/dev/...` → Permiten flashear ESPs conectados por USB al servidor.
* `--restart unless-stopped` → El contenedor se reinicia automáticamente.

---

## **🌐 2. Acceso al ESPHome del servidor**

Con el contenedor en ejecución, se accede al panel ESPHome desde cualquier dispositivo conectado por VPN o red local:

```
http://[IP_DEL_SERVIDOR]:6052/
```

Ejemplo:

```
http://100.109.64.19:6052/
```

---

**📝 Obs:** Para flashear un ESP por USB, se deben utilizar los puertos USB de la PC donde está corriendo el contenedor (servidor o tu PC local). No es posible flashear un dispositivo conectado físicamente a otra máquina.

## **💻 3. Crear tu propio ESPHome local (opcional)**

Si querés trabajar con tu ESP en tu casa sin depender del servidor, podés crear tu propio contenedor ESPHome local.

```bash
mkdir ~/esphome

docker run -d \
  --name esphome \
  --restart unless-stopped \
  -v ~/esphome:/config \
  -p 6052:6052 \
  --privileged \
  --device=/dev/ttyUSB0 \
  --device=/dev/ttyACM0 \
  esphome/esphome
```

### ✔ Esto te permite:

* Compilar tus YAMLs localmente.
* Flashear tu ESP por USB en su PC personal.
* Hacer pruebas sin depender del servidor de la facultad.
* Mantener tus configuraciones sincronizadas o separadas del servidor, según prefieras.

---

**Versión del documento:** 1.0
