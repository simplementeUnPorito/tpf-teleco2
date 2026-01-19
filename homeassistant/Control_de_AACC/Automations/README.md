# Automatización de Aire Acondicionado

Este apartado detalla la configuración de la lógica de control para el aire acondicionado. Para validar el funcionamiento, se utilizaron dos automatizaciones (encendido y apagado) basadas en un entorno de simulación.

### Objetivo de la Configuración
La intención principal fue crear un sistema de pruebas utilizando un **sensor de temperatura ficticio** generado en un nodo virtual con ESPHome. Esto permitió simular variaciones de temperatura (subidas y bajadas graduales) para verificar que las automatizaciones se activen exactamente cuando se alcanzan los umbrales definidos, sin depender de las condiciones ambientales reales del momento.

## 1. Acceso y Creación
El primer paso consiste en navegar por el menú lateral para iniciar la configuración:
* Ir a **Settings** (Ajustes).

  <img width="362" height="816" alt="HA_auto_part1" src="https://github.com/user-attachments/assets/74b2e084-afd3-4632-b076-623a63cc8d80" />

* Seleccionar **Automations & scenes** (Automatizaciones y escenas).
  <img width="1159" height="486" alt="HA_auto_part2" src="https://github.com/user-attachments/assets/17159cbf-082a-4271-87af-51c3165a4af0" />

* Pulsar el botón **+ Create automation** (+ Crear automatización).
  <img width="1146" height="903" alt="HA_auto_part3" src="https://github.com/user-attachments/assets/a0491327-1d86-46ae-b700-00d7823f5119" />

* Elegir la opción **Create new automation** (Crear nueva automatización).
  <img width="665" height="438" alt="HA_auto_part5" src="https://github.com/user-attachments/assets/a1fdf9b3-4e72-4100-ab18-78931f375b13" />


## 2. Interfaz de Nueva Automatización
Una vez seleccionada la opción anterior, se presenta la ventana de configuración donde definiremos disparadores, condiciones y acciones:

<img width="1577" height="882" alt="HA_auto_part4" src="https://github.com/user-attachments/assets/145fd05a-3e0a-4d1e-adc3-03b16737d669" />



## 3. Definición del Disparador (Trigger)
En esta ventana, procedemos a definir qué evento iniciará la acción:
* Pulsar en **Add Trigger** (Añadir disparador).
* En el buscador, escribir y seleccionar **Numeric state** (Estado numérico).

<img width="952" height="353" alt="HA_auto_part6" src="https://github.com/user-attachments/assets/4584fcea-6a5f-4776-b160-b1975bb955ed" />

### Configuración del Trigger en ambas automatizaciones
Para lograr el control del clima, se configuraron dos automatizaciones en este trabajo con parámetros específicos basados en el sensor virtual:

**Automatización de Encendido (ON):**
* **Entity**: Sensor de temperatura virtual.
* **Above**: 26 (Se activa cuando la temperatura supera los 26°C).
* **Below**: (Vacío).
* **Attributes**: (Vacío).
* **For**: 30 segundos. 
  > *Nota: Este tiempo es ajustable; se define para asegurar que la condición sea estable antes de ejecutar la acción.*

**Automatización de Apagado (OFF):**
* **Entity**: Sensor de temperatura virtual.
* **Above**: (Vacío).
* **Below**: 24.2 (Se activa cuando la temperatura baja de 24.2°C).
* **Attributes**: (Vacío).
* **For**: 30 segundos.

### ### Observación: Lógica de los disparadores
Es fundamental entender que Home Assistant permite añadir más de un disparador (*trigger*) en una misma automatización. Cuando se configuran múltiples triggers, el sistema se comporta internamente como un **OR lógico** (operador `||` en programación). 

Esto significa que la automatización se ejecutará si **cualquiera** de los eventos definidos ocurre. No es necesario que todos se cumplan simultáneamente; basta con que uno solo pase de "falso" a "verdadero" para disparar la acción.


### Creación del Helper (Control Automático)
Antes de configurar las condiciones, es necesario crear un interruptor virtual que nos permita habilitar o deshabilitar todo el sistema de automatización de forma manual.

A. Ir a **Settings** (Ajustes) > **Devices & Services** (Dispositivos y Servicios).
<img width="1201" height="387" alt="HA_boton1" src="https://github.com/user-attachments/assets/1392aeb9-81d9-474b-9b55-816f6efae226" />

   
B. Seleccionar la pestaña **Helpers** (Ayudantes) en la parte superior.
<img width="1575" height="888" alt="HA_boton2" src="https://github.com/user-attachments/assets/33275fc6-905f-45ad-98b3-e08525708c83" />


C. Pulsar en **+ Create Helper** y elegir la opción **Toggle** (Interruptor).

D. Nombre en este caso: `Control Automático Aire`.


## 4. Configuración de la Condición (And if)
Para evitar que el aire se encienda o apague solo cuando no lo deseamos, añadimos una condición que verifica si el interruptor de "Control Automático" está encendido.

En el buscador de condiciones, escribimos **State** (Estado) y configuramos lo siguiente para ambas automatizaciones (ON y OFF):

<img width="967" height="442" alt="HA_auto_part7" src="https://github.com/user-attachments/assets/d5692953-0841-44e8-852a-abd688a40691" />


* **Entity**: `input_boolean.control_automatico_aire` (el helper creado anteriormente).
* **State**: `on`.
* **Attribute**: (Vacío).
* **For**: (Vacío).

De esta manera, la automatización solo se ejecutará si se cumplen dos requisitos: que se alcance el umbral de temperatura (**Trigger**) y que el botón de control automático esté activado (**Condition**).


## 5. Configuración de la Acción (Add action)
Finalmente, definimos qué debe hacer el sistema cuando se cumplen el disparador y la condición. En este paso es donde vinculamos la lógica con el hardware físico (el ESP32 con el transmisor IR).

Para ambas automatizaciones, el procedimiento fue el siguiente:

* Pulsar en **Add Action** (Añadir acción).
* Seleccionar la opción **Device** (Dispositivo).

<img width="1300" height="487" alt="HA_auto_part8" src="https://github.com/user-attachments/assets/c3851c73-568f-4f62-8e06-030d46990bdc" />


* **Device**: Seleccionar el dispositivo **ESP32** (el cual tiene conectado el módulo transmisor IR).

Dependiendo de la automatización, se configuró de la siguiente manera:

* **Para la automatización de Encendido (ON):**
    * **Action**: Seleccionar **Turn on**.
  
* **Para la automatización de Apagado (OFF):**
    * **Action**: Seleccionar **Turn off**.

---

---

### Verificación Final
Para que la lógica entre en funcionamiento, es indispensable verificar que las automatizaciones creadas estén habilitadas en el panel general.

* Ir a **Settings** > **Automations & scenes**.
* En la lista de automatizaciones, el **State** (Estado) de ambas (`AC-ON Automation` y `AC-OFF Automation`) debe estar en **ON** (activado). 

<img width="1577" height="478" alt="HA_auto_part12" src="https://github.com/user-attachments/assets/451aa9a0-2cee-478d-aea6-1561bef4da70" />


> **Importante**: Si el interruptor en esta sección está apagado, la automatización no escuchará al sensor de temperatura ni enviará señales al ESP32, independientemente de cómo esté configurado el botón de "Control Automático".
