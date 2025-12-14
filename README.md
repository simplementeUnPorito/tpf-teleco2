# **TPF – Telecomunicaciones II (2025)**

## **Infraestructura IoT Base – DEI/LED**

**Universidad Católica “Nuestra Señora de la Asunción”**
Facultad de Ciencias y Tecnologías
Departamento de Ingeniería Electrónica e Informática

---

## 🎯 Objetivo del Proyecto

Este repositorio documenta y centraliza la **infraestructura base** utilizada en el Trabajo Práctico Final de **Telecomunicaciones II (2025)**.

El objetivo principal es **diseñar, desplegar y mantener un servidor IoT reproducible**, que permita a los estudiantes:

* Desarrollar y probar sensores y actuadores
* Integrar distintos protocolos de comunicación
* Trabajar de forma colaborativa y remota
* Utilizar una base común estandarizada para prácticas de laboratorio

> ⚠️ **Este repositorio NO es un backup del servidor.**
> Contiene únicamente configuraciones, scripts y documentación necesarios para **recrear el entorno**, no su estado en ejecución.

---

## 🖥️ Servidor IoT de la Facultad

El servidor físico se encuentra en una PC dedicada dentro de la facultad y corre:

* **Ubuntu 24.04 LTS**
* **Docker** como plataforma de contenedores
* **OpenSSH Server** para administración remota
* **VPN Tailscale** para acceso seguro desde redes externas

Este enfoque permite:

✔ Acceso remoto sin abrir puertos en el router
✔ Trabajo desde casa de manera segura
✔ Un único punto de integración para todos los módulos del TP
✔ Reproducibilidad del entorno en futuros semestres

---

## 🔐 Acceso por SSH al servidor

El acceso administrativo al servidor se realiza mediante **SSH**.

### Instalación del servidor SSH

En una instalación limpia de Ubuntu, basta con ejecutar:

```bash
sudo apt update
sudo apt install openssh-server
```

El servicio queda activo automáticamente. Para verificar:

```bash
systemctl status ssh
```

### Conexión al servidor

Desde cualquier cliente:

```bash
ssh user@[IP_DEL_SERVIDOR]
```

Ejemplo usando IP asignada por Tailscale:

```bash
ssh user@100.109.64.19
```

> ⚠️ **Buenas prácticas:**
> En este proyecto se evita exponer el puerto SSH a Internet.
> El acceso remoto se realiza exclusivamente a través de la VPN Tailscale.

---

## 🌐 Acceso remoto mediante Tailscale (VPN)

Para permitir el trabajo remoto desde redes domésticas sin abrir puertos, el proyecto utiliza **Tailscale**, una VPN mesh basada en **WireGuard**.

### ¿Por qué Tailscale?

* No requiere configuración de NAT o port forwarding
* Funciona detrás de routers domésticos y redes móviles
* Cada integrante se autentica con su propia identidad
* El servidor no queda expuesto públicamente

### Instalación

Crear una cuenta en:

👉 [https://tailscale.com](https://tailscale.com)

Instalar Tailscale en cada dispositivo:

* **Linux**:

```bash
curl -fsSL https://tailscale.com/install.sh | sh
```

* **Windows / macOS / Android / iOS**:
  [https://tailscale.com/download](https://tailscale.com/download)

### Activar la VPN

```bash
sudo tailscale up
```

### Visualización de dispositivos

Desde el panel web:

👉 [https://login.tailscale.com/admin/machines](https://login.tailscale.com/admin/machines)

Se puede ver:

* IP Tailscale del servidor
* IPs de cada integrante
* Estado de conexión (online/offline)

---

## 📂 Organización del repositorio

El repositorio se organiza por módulos, cada uno con su propia documentación:

* **`homeassistant/`**
  Configuración reproducible del servidor Home Assistant (Docker).

* **`esphome/`**
  Configuración de nodos ESPHome y entorno de compilación (Docker).

* **`Scripts/`**
  Scripts y herramientas auxiliares utilizadas durante el desarrollo y las pruebas.

Cada carpeta contiene su propio `README.md` con detalles específicos de uso.

---

## 🎓 Uso académico

Este repositorio está pensado como:

* Base común para el TP 2025
* Referencia técnica para futuros semestres
* Ejemplo de infraestructura IoT documentada y reproducible

El enfoque prioriza **criterio de diseño, seguridad y claridad**, por sobre soluciones ad-hoc o configuraciones dependientes del estado del sistema.

---

**Versión del documento:** 1.1

