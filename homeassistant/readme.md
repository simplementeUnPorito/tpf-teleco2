# **Home Assistant**

Home Assistant se ejecuta en la PC servidor dentro de un **contenedor Docker** llamado `homeassistant`. La configuración persistente se guarda en:

```
/home/user/homeassistant
```

Para administrar este contenedor es necesario:

1. Estar físicamente en la PC servidor **o**
2. Conectarse por SSH, ya sea desde la red local o a través de la VPN Tailscale.


## 🐳 Creación del contenedor

El contenedor se creó con el siguiente comando:

```bash
docker run -d \
  --name homeassistant \
  --restart unless-stopped \
  -p 8123:8123 \
  -v /home/user/homeassistant:/config \
  --privileged \
  ghcr.io/home-assistant/home-assistant:stable

```

### Comandos útiles

Verificar que el contenedor está corriendo:

```bash
docker ps
```

Detener Home Assistant:

```bash
docker stop homeassistant
```

Volver a iniciarlo:

```bash
docker start homeassistant
```

---

# **Acceso a Home Assistant – Guía Rápida**

Este documento explica cómo acceder a Home Assistant una vez que la VPN está activa o cuando se está dentro de la red local de la facultad.

---

## **🌐 1. Acceso a Home Assistant**

Una vez levantada la VPN Tailscale **o si se está físicamente dentro de la red local**, se puede acceder al servidor de Home Assistant usando un navegador web.

**Formato general:**

```
http://[IP_DEL_SERVIDOR]:8123/
```

**Ejemplo real:**

```
http://100.109.64.19:8123/
```

---

## **👤 2. Usuarios del sistema**

Cada usuario tiene un acceso individual a Home Assistant.

El formato del nombre de usuario es:

```
[NOMBRE].[APELLIDO]
```

### **Ejemplo:**

```
fernando.brunetti
```

---

## **🔑 3. Contraseña temporal**

Todas las contraseñas iniciales siguen el formato:

```
[nombre][DDMM]
```

Es decir:

* El **nombre del usuario** en minúsculas
* Seguido por su **día y mes de nacimiento** (4 dígitos)

### **Ejemplo:**

Usuario:

```
fernando.brunetti
```

Si su cumpleaños es el **08 de julio**, entonces su contraseña sería:

```
fernando1111
```

Estas credenciales son **provisionales**. Cada usuario debe cambiarlas en su primer ingreso por razones de seguridad.

---

## **✔ Listo**

Con estos datos, cualquier integrante del equipo puede entrar a Home Assistant, revisar dashboards, crear automatizaciones y trabajar con los dispositivos ESPHome que se integren.

---

**Versión del documento:** 1.0
