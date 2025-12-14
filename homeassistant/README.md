# **Home Assistant**

Home Assistant se utiliza como **servidor central de automatización e integración IoT** del proyecto. Corre dentro de un **contenedor Docker** en la PC servidor de la facultad.

---

## 🖥️ Consideración sobre instalación nativa (Home Assistant OS)

Durante la etapa inicial del proyecto se evaluó la posibilidad de instalar
**Home Assistant OS de forma nativa** en una PC dedicada, con el objetivo de
simplificar la administración y prescindir del uso de Docker.

Sin embargo, esta alternativa **no pudo implementarse** debido a limitaciones
de hardware de la PC adquirida:

- El equipo **no cuenta con firmware UEFI**
- Home Assistant OS requiere **UEFI** para su instalación y arranque nativo
- El sistema solo soporta modo **BIOS / Legacy**, incompatible con HAOS

Debido a esta restricción, se decidió **descartar la instalación nativa** y
optar por una solución basada en **Docker sobre Ubuntu**, la cual resultó:

- Totalmente compatible con el hardware disponible
- Más flexible para depuración y acceso por SSH
- Adecuada para un entorno académico y colaborativo

Esta decisión permitió continuar el proyecto sin depender de un cambio de
hardware y manteniendo control total sobre el sistema operativo base.

La configuración persistente se almacena en el host en la ruta:

```
/home/user/homeassistant
```

Para administrar el sistema es necesario:

1. Estar físicamente en la PC servidor **o**
2. Conectarse por **SSH**, ya sea desde la red local de la facultad o mediante la **VPN Tailscale**.

---

## 🐳 Ejecución del contenedor Docker

El contenedor fue creado manualmente utilizando Docker, con reinicio automático ante fallos o reinicios del sistema.

```bash
docker run -d \
  --name homeassistant \
  --restart unless-stopped \
  -p 8123:8123 \
  -v /home/user/homeassistant:/config \
  --privileged \
  ghcr.io/home-assistant/home-assistant:stable
```

### 📌 Notas de diseño

* Se utiliza un **volumen bind-mounted** para preservar la configuración
* No se versionan bases de datos, logs ni estados internos
* El contenedor se ejecuta en modo `--privileged` para facilitar la integración con hardware y red

---

## 🔧 Comandos útiles de administración

Verificar que el contenedor esté en ejecución:

```bash
docker ps
```

Detener Home Assistant:

```bash
docker stop homeassistant
```

Iniciar nuevamente el contenedor:

```bash
docker start homeassistant
```

Ver logs del contenedor:

```bash
docker logs homeassistant
```

---

## 🌐 Acceso a Home Assistant

Una vez activa la VPN Tailscale (o estando dentro de la red local de la facultad), se puede acceder mediante un navegador web.

### Formato general

```
http://[IP_DEL_SERVIDOR]:8123/
```

### Ejemplo real

```
http://100.109.64.19:8123/
```

---

## 👤 Usuarios del sistema

Cada integrante del proyecto dispone de un **usuario individual** en Home Assistant.

Formato del nombre de usuario:

```
NOMBRE.APELLIDO
```

### Ejemplo

```
fernando.brunetti
```

---

## 🔑 Contraseña inicial

Las contraseñas iniciales se asignan con un formato estándar para facilitar el primer acceso.

```
[nombre][DDMM]
```

Donde:

* `nombre` está en minúsculas
* `DDMM` corresponde al día y mes de nacimiento

### Ejemplo

Usuario:

```
fernando.brunetti
```

Cumpleaños: 11 de noviembre →

```
fernando1111
```

> ⚠️ Estas credenciales son **provisorias**.
> Cada usuario debe cambiar su contraseña en el primer inicio de sesión.

---

## 🔌 Integración de dispositivos ESPHome

Los dispositivos desarrollados con **ESPHome** se integran de forma nativa en Home Assistant.

### Requisitos previos

* El nodo ESPHome debe estar **conectado a la misma red** que el servidor (red local o VPN Tailscale)
* El dispositivo debe haber sido **compilado y flasheado** correctamente desde el entorno ESPHome

---

### ➕ Agregar un dispositivo ESPHome

1. Ingresar a Home Assistant desde el navegador
2. Ir a:

```
Overview → +
```

3. Presionar **“Add device”**
4. Buscar y seleccionar **ESPHome**

Home Assistant detectará automáticamente los nodos ESPHome disponibles en la red.

---

### 🔗 Vinculación del nodo

Una vez detectado el dispositivo:

* Seleccionar el nodo correspondiente
* Aceptar la integración
* El dispositivo quedará registrado como una nueva **entidad** dentro del sistema

Las entidades creadas pueden ser:

* Sensores
* Actuadores
* Switches
* Luces
* Emisores infrarrojos, etc.

---

## 📊 Uso dentro de Home Assistant

Una vez integrado, el dispositivo ESPHome puede:

* Aparecer en **Overview**
* Utilizarse en **Automatizaciones**
* Ser controlado manualmente desde la interfaz web
* Interactuar con otros dispositivos del sistema

No es necesario editar manualmente archivos de configuración para esta integración básica.

---



**Versión del documento:** 1.1

