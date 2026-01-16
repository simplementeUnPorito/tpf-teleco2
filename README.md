# TPF – Telecomunicaciones II (2025)

## Red IoT Híbrida Multi-Arquitectura

**Universidad Católica “Nuestra Señora de la Asunción”**
Facultad de Ciencias y Tecnologías
Departamento de Ingeniería Electrónica e Informática

Materia: **Telecomunicaciones II**
Profesor: **Fernando Brunetti**

---

## Descripción general del proyecto

Este repositorio corresponde al **Trabajo Práctico Final (TPF)** desarrollado por el **grupo de estudiantes de Telecomunicaciones II – 2025**.

El proyecto consiste en el **diseño e implementación de una red IoT híbrida y multi-arquitectura**, orientada a la **integración de sensado y actuación** mediante distintos tipos de dispositivos, protocolos y tecnologías de comunicación.

La infraestructura desarrollada funciona como una **base común y reutilizable**, pensada no solo para este trabajo práctico, sino también como **plataforma de referencia para futuros trabajos y experiencias de la materia**.

---

## Objetivo del trabajo

El objetivo principal es **crear una red IoT flexible y extensible** que permita:

* Integrar dispositivos de **sensado y actuación**
* Utilizar **múltiples tecnologías de comunicación** (WiFi y LoRa)
* Centralizar datos en un backend común
* Visualizar información y controlar dispositivos de forma remota
* Facilitar el trabajo colaborativo entre estudiantes

---

## Arquitectura general del sistema
<img width="960" height="720" alt="IoT_NEG_DEI_LED" src="https://github.com/user-attachments/assets/e3828d64-0867-454a-92f0-b70719c93680" />

---
La red implementada se basa en una **arquitectura IoT híbrida**, donde conviven:

* **Dispositivos WiFi** (ESP8266, ESP32)
* **Dispositivos LoRa** (LoRa32u4ii)
* Un **servidor IoT central** que actúa como punto de integración

Los detalles específicos de sensores, actuadores y casos de uso se documentan en las implementaciones individuales de cada alumno.

---

## ¿Qué es cada servicio/carpeta? (definiciones a alto nivel)

> Esta sección describe **qué es** cada componente del stack.
> El **cómo se instala/usa** está en los `README.md` de cada carpeta.

* **`MQTT/`**
  Broker y configuración asociada a **MQTT**, un sistema de mensajería **pub/sub** común en IoT para intercambiar telemetría y comandos.

* **`InfluxDB/`**
  **Base de datos de series temporales**: diseñada para almacenar mediciones indexadas por tiempo (sensores), consultas por rango y agregaciones.

* **`Grafana/`**
  Plataforma de **visualización** y creación de dashboards: consume datos (por ejemplo desde InfluxDB) y los presenta en paneles.

* **`NodeRed/`**
  Herramienta de **programación por flujos**: permite integrar fuentes/servicios, transformar datos y construir automatizaciones sin mucho código.

* **`homeassistant/`**
  Plataforma de **automatización del hogar/IoT**: integra dispositivos y servicios, permite crear reglas, escenas y control centralizado.

* **`esphome/`**
  Framework para **firmware declarativo** en ESP8266/ESP32: simplifica la creación de nodos IoT con sensores/actuadores y su integración.

* **`LoRa/`**
  Contenido relacionado a **comunicación LoRa**: pruebas, nodos y documentación vinculada a enlaces de largo alcance y bajo consumo.

* **`TTN/`**
  Integración con **The Things Network (TTN)** / ecosistema LoRaWAN: incluye configuraciones y elementos necesarios para operar dispositivos LoRaWAN.

* **`Scripts/`**
  Scripts auxiliares: utilidades para soporte, pruebas, administración o tareas repetitivas del proyecto.

---

## Infraestructura del servidor (soporte)

El sistema se ejecuta sobre una **PC de hardware antiguo** perteneciente a la facultad, reutilizada como **servidor IoT dedicado**, corriendo **Linux**.

### Nota importante

El uso de **SSH, Docker y Tailscale** responde a decisiones prácticas de administración y **no constituye un objetivo evaluado del trabajo**, sino herramientas de soporte para el desarrollo del proyecto.

### Acceso por SSH (uso básico)

SSH permite administrar el servidor de forma remota mediante terminal.

* Verificar servicio en el servidor:

```bash
systemctl status ssh
```

* Conectarse desde un cliente:

```bash
ssh user@IP_DEL_SERVIDOR
```

### Acceso remoto con Tailscale (VPN)

Tailscale crea una VPN tipo *mesh* (basada en WireGuard), permitiendo acceso remoto seguro sin exponer puertos.

* Activar en Linux:

```bash
sudo tailscale up
```

* Luego se usa la IP de Tailscale para SSH:

```bash
ssh user@IP_TAILSCALE
```

---


## Alcance y proyección

La infraestructura desarrollada **no está pensada como un sistema cerrado**, sino como una **base extensible**, sobre la cual cada estudiante puede:

* Incorporar nuevos nodos
* Probar distintos sensores y actuadores
* Evaluar arquitecturas de comunicación
* Experimentar con distintos flujos de datos

Este enfoque permite que el proyecto pueda **evolucionar en futuros semestres** y adaptarse a nuevos objetivos académicos.

---

**Trabajo Práctico Final – Telecomunicaciones II (2025)**
Universidad Católica “Nuestra Señora de la Asunción”
