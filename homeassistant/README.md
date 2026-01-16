# Home Assistant

Home Assistant se utiliza como **una de las capas centrales de automatización y UI** del proyecto IoT. Actúa como punto de orquestación para:

* Controlar dispositivos desde un **panel web** (dashboards)
* Construir **automatizaciones** (lógica de alto nivel) a partir de datos de sensores
* Generar **gráficas e historial** de mediciones
* Emitir **alertas/notificaciones** ante eventos
* Administrar **usuarios** del sistema

En este proyecto Home Assistant corre en el servidor de la facultad dentro de un **contenedor Docker**.

---

## Consideración sobre instalación nativa (Home Assistant OS)

Durante la etapa inicial se evaluó instalar **Home Assistant OS (HAOS)** de forma nativa. Sin embargo, en la PC disponible **no fue viable** porque el equipo no cuenta con **firmware UEFI** (solo modo **BIOS/Legacy**) y la instalación nativa recomendada para HAOS está pensada para arranque UEFI.

Por este motivo se adoptó una solución basada en **Docker sobre Ubuntu**, que resultó compatible con el hardware disponible y suficiente para un entorno académico y colaborativo.

---

## Instalación y ejecución (Docker)

La configuración persistente se almacena en el host en la ruta:

```txt
/home/user/homeassistant
```

Ejecución del contenedor (con reinicio automático):

```bash
docker run -d \
  --name homeassistant \
  --restart unless-stopped \
  -p 8123:8123 \
  -v /home/user/homeassistant:/config \
  --privileged \
  ghcr.io/home-assistant/home-assistant:stable
```

### Notas

* Se usa un **volumen bind-mounted** para preservar la configuración.
* El modo `--restart unless-stopped` permite que el servicio vuelva a levantarse ante reinicios.

---

## Comandos útiles de administración

Verificar contenedores en ejecución:

```bash
docker ps
```

Detener Home Assistant:

```bash
docker stop homeassistant
```

Iniciar Home Assistant:

```bash
docker start homeassistant
```

Ver logs:

```bash
docker logs homeassistant
```

---

## Acceso web

Formato general:

```txt
http://[IP_DEL_SERVIDOR]:8123/
```

Ejemplo:

```txt
http://100.109.64.19:8123/
```

---

## Usuarios del sistema

Cada integrante del proyecto dispone de un **usuario individual** en Home Assistant.

Formato del nombre de usuario:

```txt
nombre.apellido
```

Ejemplo:

```txt
fernando.brunetti
```

---

## Contraseña inicial

Las contraseñas iniciales se asignan con un formato estándar para facilitar el primer acceso:

```txt
[nombre][DDMM]
```

Donde:

* `nombre` está en minúsculas
* `DDMM` corresponde al día y mes de nacimiento

Ejemplo:

Usuario:

```txt
fernando.brunetti
```

Cumpleaños: 11 de noviembre →

```txt
fernando1111
```

> ⚠️ **Advertencia:** estas credenciales son provisorias. Cada usuario debe **cambiar su contraseña** en el primer inicio de sesión.

---
## Funcionalidades principales (visión general)

Home Assistant aporta una capa de **supervisión, control y automatización** para la red IoT del proyecto. A nivel general, permite:

* **Panel/UI web (Dashboards):** control manual de dispositivos y visualización en tiempo real.
* **Historial y gráficas:** registro y visualización de estados/mediciones disponibles en las entidades.
* **Automatizaciones y escenas:** creación de lógica de alto nivel basada en eventos/condiciones (por ejemplo: si un sensor supera un umbral, ejecutar una acción).
* **Alertas y notificaciones:** avisos ante eventos relevantes (por ejemplo: estados anómalos o condiciones críticas).
* **Gestión de usuarios y permisos:** cada integrante opera con su cuenta; el acceso es controlado.

> Nota: los detalles concretos (sensores, actuadores, reglas y dashboards específicos) se documentan en cada implementación/entrega correspondiente.

---

## Alta e incorporación de dispositivos (general)

Home Assistant integra dispositivos mediante **integraciones**. En términos generales, el flujo para añadir un dispositivo es:

1. Ingresar a la interfaz web de Home Assistant.
2. Ir a **Settings → Devices & Services**.
3. Presionar **Add Integration**.
4. Seleccionar la integración correspondiente (según el tipo de dispositivo/servicio).
5. Completar el emparejamiento o configuración mínima (según lo que solicite la integración).

Al finalizar, Home Assistant creará **entidades** asociadas al dispositivo (por ejemplo: sensores, switches, actuadores, etc.).

---

## Entidades: concepto general

Dentro de Home Assistant, cada dispositivo se representa mediante **entidades**, que son los “puntos de control/lectura” visibles en la UI.

Ejemplos típicos:

* Un sensor de temperatura → `sensor.temperatura_*`
* Un actuador tipo relay/switch → `switch.*`
* Una luz → `light.*`

Estas entidades se pueden:

* Mostrar en dashboards
* Usar en automatizaciones
* Consultar su historial/gráficas

---

## Automatizaciones (visión general)

Las **automatizaciones** permiten definir lógica de alto nivel del tipo:

* **Disparador (Trigger):** qué evento inicia la automatización.
* **Condiciones (Conditions):** filtros opcionales para decidir si se ejecuta.
* **Acciones (Actions):** qué se ejecuta (por ejemplo, activar un switch, mandar una alerta, etc.).

> Las automatizaciones específicas del proyecto se documentan en cada implementación que las utilice.
---
**Versión del documento:** 1.2
