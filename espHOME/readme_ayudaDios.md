# Explicación técnica del bloqueo de OTA por WiFi en la red de la facultad

A continuación dejo un texto claro, formal y breve para presentar al profesor y justificar por qué ESPHome no permite realizar actualizaciones OTA dentro de la red WiFi de la facultad.

---

## **1. Contexto del problema**

Se intentó utilizar la función **OTA (Over‑The‑Air)** de ESPHome para actualizar el firmware del ESP32 a través de la red WiFi.

El dispositivo se conecta correctamente a la red *InvestigadoresCYT* y obtiene una dirección IP válida. También responde a ping y permite acceder a su interfaz web. Sin embargo, cuando ESPHome intenta iniciar la actualización OTA, la conexión falla inmediatamente.

---

## **2. Cómo funciona OTA en ESPHome**

ESPHome utiliza un pequeño servidor TCP dentro del ESP32 para recibir el nuevo firmware. Este servidor **siempre opera en el puerto 3232/TCP**.

La secuencia de OTA es:

1. ESPHome compila el firmware.
2. ESPHome intenta conectarse al ESP32 mediante `tcp://<IP>:3232`.
3. Si ese puerto está accesible, la actualización se transfiere y se flashea.
4. Si el puerto está bloqueado, la conexión falla.

---

## **3. Qué está ocurriendo en la red de la facultad**

Al ejecutar:

```
nc -vz <IP_DEL_ESP> 3232
```

el resultado es:

```
Connection timed out
```

Esto indica que **el puerto 3232 está filtrado por la red**. Es decir, el firewall o la política interna de la facultad **bloquea cualquier tráfico entrante hacia puertos no estándar**, lo cual es habitual en redes académicas para restringir servicios inseguros.

Este tipo de filtrado afecta puntualmente a:

* Servidor OTA (puerto TCP 3232)
* mDNS (multicast 224.0.0.251 para descubrir `<nombre>.local`)
* Broadcast entre clientes dentro de la misma WiFi

Por eso:

* El ESP se ve en Home Assistant.
* Se puede acceder por IP.
* **Pero OTA no funciona**, porque la red **no permite conexiones entrantes al puerto 3232**.

---

## **4. Conclusión técnica**

La red de la facultad **no permite OTA** porque:

* Filtra puertos no estándar (incluido 3232/TCP).
* Restringe multicast/broadcast requeridos para mDNS.

El ESP32 está configurado correctamente; el problema es puramente de red.

---

## **5. Soluciones posibles**

### **A. Actualizar vía USB (solución garantizada)**

Conectar el ESP32 por cable y flashear desde la PC.

### **B. Usar un hotspot del celular**

Conectar la PC y el ESP al hotspot. OTA funciona porque no hay filtrado.

### **C. Utilizar un router propio conectado por cable**

El router crea una red interna sin restricciones. El ESP y la PC quedan libres para usar OTA.

---

