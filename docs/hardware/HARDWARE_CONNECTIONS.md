# TrovaLibre — Conexiones de Hardware

> Este archivo se genera a partir de `src/constants.hpp` y refleja exactamente las
> asignaciones de pines definidas en el firmware. Verifica los datos con tu placa
> física antes de realizar cualquier conexión.

---

## Variantes de Placa Soportadas

| Macro | Placa | MCU |
|-------|-------|-----|
| `TROVALIBRE_CLASSIC` | Arduino Nano (ATmega328P) | Classic |
| `TROVALIBRE_EVERY` | Arduino Nano Every (ATmega4809) | Every |
| `TROVALIBRE_EVERY_4808` | Arduino Nano Every (ATmega4808) | Every 4808 |
| `ALLinONE` | Placa ALLinONE (LGT8F328P) | AiO |
| `ALLinONE_Plus` | Placa ALLinONE Plus (ATmega4809) | AiO+ |
| `TROVALIBRE_ESP32 100` | Arduino Nano ESP32 | ESP32 Nano |
| `TROVALIBRE_ESP32 200` | ESP32 Wroom 32 (Denky32) | ESP32 Wroom |

Define **exactamente una** de estas macros en `src/constants.hpp` (o mediante el
flag `-D` en `platformio.ini`).

---

## Componentes Principales — Todas las Variantes

Estos componentes están presentes en todas las compilaciones.

### Lector RFID MFRC522 (SPI)

El MFRC522 se comunica por SPI. Los pines MOSI/MISO/SCK utilizan el SPI hardware
de la placa (no son redefinibles aquí). Solo RST y SS/SDA se definen por software.

| Señal | Classic / Every / AiO / ESP32 Nano | AiO+ | ESP32 Wroom |
|-------|-------------------------------------|------|-------------|
| RST | D9 | D11 | D22 |
| SS (SDA) | D10 | D7 | D5 |
| MOSI | D11 (SPI hardware) | SPI hardware | SPI hardware |
| MISO | D12 (SPI hardware) | SPI hardware | SPI hardware |
| SCK | D13 (SPI hardware) | SPI hardware | SPI hardware |
| 3,3 V | Línea 3,3 V | Línea 3,3 V | Línea 3,3 V |
| GND | GND | GND | GND |

> Cookie de tarjeta (identificador EEPROM): `0x1337b347`
> Versión de formato de tarjeta: `0x02`
> Ciclos de retardo al retirar tarjeta: `3`
> **Advertencia:** No modifiques `cardCookie` ni `cardVersion`; las tarjetas RFID
> ya programadas dependen de estos valores.

---

### DFPlayer Mini (módulo MP3)

| Señal | Classic / Every (SW Serial, por defecto) | Every / ESP32 Nano (HW Serial) | AiO (SW Serial) | AiO+ (HW Serial) | ESP32 Wroom |
|-------|------------------------------------------|-------------------------------|-----------------|-------------------|-------------|
| RX (placa → reproductor) | D3 (pin TX) | D1 (Serial1 TX) | D3 | Serial3 TX | D17 / Serial2 TX |
| TX (reproductor → placa) | D2 (pin RX) | D0 (Serial1 RX) | D2 | Serial3 RX | D16 / Serial2 RX |
| BUSY | D4 (activo en LOW) | D4 | D4 | D13 | D4 |
| VCC | 5 V | 5 V | 5 V | 5 V | 5 V |
| GND | GND | GND | GND | GND | GND |
| SPK+ / SPK− | Altavoz | Altavoz | Altavoz | Altavoz | Altavoz |

El modo serie hardware se activa con `#define DFPlayerUsesHardwareSerial`
(obligatorio para AiO+, opcional para Every y ESP32 Nano).

Tiempo de espera al arrancar: `1 200 ms` (por defecto) / `2 500 ms` para chips lentos
(MH2024K24SS, GD3200B).

---

## Botones

Nivel lógico de botones: **activo en LOW** (pull-up interno; reposo = HIGH, pulsado = LOW).
Umbral de pulsación larga: `1 000 ms`.
Tiempo de antirrebote: `25 ms`.

### Distribución de 3 Botones (por defecto en Classic / Every; activar con `#define THREEBUTTONS`)

| Botón | Classic / Every / ESP32 Nano | AiO | AiO+ | ESP32 Wroom |
|-------|------------------------------|-----|------|-------------|
| Pausa/Reproducción | A0 | A0 | A0 | D33 |
| Subir volumen / Siguiente | A1 | A2 | A2 | D25 |
| Bajar volumen / Anterior | A2 | A1 | A1 | D26 |

### Distribución de 5 Botones (por defecto en AiO / AiO+; activar con `#define FIVEBUTTONS`)

| Botón | Classic / Every / ESP32 Nano | AiO | AiO+ | ESP32 Wroom |
|-------|------------------------------|-----|------|-------------|
| Pausa/Reproducción | A0 | A0 | A0 | D33 |
| Subir volumen | A1 | A2 | A2 | D25 |
| Bajar volumen | A2 | A1 | A1 | D26 |
| Siguiente (botón 4) | A3 | A4 | A4 | D27 |
| Anterior (botón 5) | A4 | A3 | A3 | D14 |

### Panel de 3×3 Botones (activar con `#define BUTTONS3X3`)

| Señal | Classic / Every / ESP32 Nano | AiO | AiO+ | ESP32 Wroom |
|-------|------------------------------|-----|------|-------------|
| Pausa/Reproducción | A0 | A0 | A0 | D33 |
| Subir | A1 | A2 | A4 | D25 |
| Bajar | A2 | A1 | A3 | D26 |
| Matriz analógica | A3 | A2 | A2 | D27 |
| Tiempo antirrebote | 50 ms | 50 ms | 50 ms | 50 ms |

> **Nota:** Los modos de 3×3 y 5 botones son mutuamente excluyentes.

---

## Componentes Opcionales

Activa cada función descomentando el `#define` correspondiente en
`src/constants.hpp`.

### Pin de Apagado

Controla un interruptor de alimentación (Pololu u otro) para cortar la corriente.

| Placa | Pin | Nivel activo |
|-------|-----|-------------|
| Classic / Every (por defecto) | D7 | HIGH |
| Classic / Every (`USE_TRAEGER_PLATINE_SHUTDOWN`) | D7 | LOW |
| AiO | D7 | LOW |
| AiO+ | D27 | LOW |
| ESP32 Nano (por defecto) | D7 | HIGH |
| ESP32 Nano (`USE_TRAEGER_PLATINE_SHUTDOWN`) | D7 | LOW |
| ESP32 Wroom (por defecto) | D13 | LOW |
| ESP32 Wroom (`USE_POLOLU_SHUTDOWN`) | D13 | HIGH |

Opciones:
- `#define USE_POLOLU_SHUTDOWN` — para interruptor de alimentación Pololu
- `#define USE_TRAEGER_PLATINE_SHUTDOWN` — para placa portadora

---

### Activación del Altavoz (Habilitación del Amplificador) — `#define SPKONOFF`

Silencia / activa el amplificador del altavoz para suprimir el ruido al encender/apagar.
Se activa automáticamente en AiO y AiO+.

| Placa | Pin | Nivel activo |
|-------|-----|-------------|
| Classic / Every / ESP32 Nano (`SPKONOFF`) | D6 | HIGH |
| AiO | D8 | LOW |
| AiO+ | D19 (PC5) | LOW |
| ESP32 Wroom | D15 | LOW |

---

### Detección de Auriculares — `#define HPJACKDETECT`

Detecta la inserción de auriculares y redirige la salida de audio.
Se activa automáticamente en AiO+.

| Placa | Pin | Nivel activo |
|-------|-----|-------------|
| Classic / Every / ESP32 Nano | D8 | LOW |
| AiO+ | D21 (PC7) | LOW |
| ESP32 Wroom | D34 | LOW |

---

### Anillo de LED NeoPixel — `#define NEO_RING`

Un cable de datos al pad DIN del anillo. Usa una resistencia serie de 300–470 Ω
entre el pin del Arduino y DIN. Número de píxeles por defecto: `24`.

| Placa | Pin |
|-------|-----|
| Classic / Every / AiO / ESP32 Nano | D5 |
| AiO+ | D10 (PB2) |
| ESP32 Wroom | D2 (GPIO2) |

Segundo anillo opcional (`#define NEO_RING_2`):

| Placa | Pin |
|-------|-----|
| Classic / Every (solo Every) | D2 |
| AiO+ | D14 (PC0) |

Modo extendido (muestra el nivel de volumen en el anillo): `#define NEO_RING_EXT`.

---

### Encoder Rotativo — `#define ROTARY_ENCODER`

Controla el volumen. Opcionalmente también controla siguiente/anterior con
`#define ROTARY_ENCODER_LONGPRESS`.

| Placa | Pin CLK | Pin DT | Pull-up | Antirrebote |
|-------|---------|--------|---------|-------------|
| Classic / Every / AiO / ESP32 Nano | A3 | A4 | ninguno (externo) | 75 ms |
| AiO+ (conector Opt / Macho) | D36 (PF2) | D37 (PF3) | ninguno | 75 ms |
| AiO+ (zócalo Encoder Rotativo) | D31 (PE1) | D32 (PE2) | ninguno | 75 ms |
| ESP32 Wroom | D27 | D14 | ninguno | 75 ms |

> **Nota:** El encoder rotativo y el potenciómetro no pueden usarse al mismo tiempo
> en Classic / Every (comparten el pin A3).

---

### Potenciómetro (Volumen) — `#define POTI`

Potenciómetro analógico para control de volumen.

| Placa | Pin |
|-------|-----|
| Classic / Every / AiO | A3 |
| AiO+ | A14 (PF4) |
| ESP32 Wroom | D27 |

---

### Medición de Tensión de Batería — `#define BAT_VOLTAGE_MEASUREMENT`

Lee la tensión de la batería a través de un divisor de tensión.
No recomendado para ALLinONE. No disponible para ALLinONE.

| Placa | Pin | Factor de corrección del divisor |
|-------|-----|----------------------------------|
| Classic | A5 | 1,960 |
| Every | A5 | 2,007 |
| Every 4808 | A5 | 2,007 |
| AiO+ | A6 | 2,075 |
| ESP32 Wroom | D35 | 0,710 |

Divisor de tensión: 100 kΩ / 100 kΩ a GND.

Umbrales (Li-Ion por defecto):
- Bajo: 2,95 V
- Vacío: 2,90 V

Los valores alternativos para Li-Po están comentados en `constants.hpp`.

---

### Acceso Directo Especial al Inicio por GPIO — `#define SPECIAL_START_SHORTCUT`

Lee un GPIO al arrancar; si está activo, reproduce la carpeta 1 / pista 1.

| Placa | Pin | Nivel activo |
|-------|-----|-------------|
| Classic / Every / AiO | A6 | HIGH |
| AiO+ | D33 (PE3) | HIGH |
| ESP32 Nano | A6 | HIGH |
| ESP32 Wroom | D12 | HIGH |

---

### Pin Analógico Flotante (Semilla Aleatoria)

Pin analógico sin conectar, utilizado para inicializar el generador de números aleatorios.

| Placa | Pin |
|-------|-----|
| Todas las variantes | A7 |
| ESP32 Wroom | D39 (VN) |

---

### Módulo Bluetooth — `#define BT_MODULE`

Controla un módulo de audio BT externo.

| Señal | Sin `DFPlayerUsesHardwareSerial` | Con `DFPlayerUsesHardwareSerial` |
|-------|-----------------------------------|-----------------------------------|
| BT ON/OFF | D6 | D2 |
| BT Emparejamiento | D8 | D3 |
| Nivel activo | HIGH | HIGH |
| Pulso de emparejamiento | 500 ms | 500 ms |

---

### Botones con LED — `#define USE_LED_BUTTONS`

Ilumina los botones físicos mediante el LedManager.
Usa una resistencia serie de 1 kΩ por LED para limitar la corriente.

| LED | Pin (Classic / Every / AiO / ESP32 Nano) |
|-----|------------------------------------------|
| Reproducción/Pausa | A5 |
| Subir | D6 |
| Bajar | D5 |

Temporización:
- Intervalo de actualización: 800 ms
- Parpadeo corto: 400 ms

---

### Señal de Retención de Alimentación Lista — `#define POWER_HOLD_READY`

Señaliza a un circuito externo que el MCU está listo para mantener su propia alimentación.

| Placa | Pin | Nivel activo |
|-------|-----|-------------|
| Todas | D2 | HIGH |

---

### Pin de Acceso USB (solo AiO / AiO+)

Controla el acceso del host USB en placas integradas.

| Placa | Pin | Nivel activo |
|-------|-----|-------------|
| AiO | A5 | HIGH |
| AiO+ | D20 | HIGH |

---

## Tablas Resumen de Pines

### Classic / Every / Every4808 — Distribución más común

| Función | Pin | Nota |
|---------|-----|------|
| Botón Pausa | A0 | Activo en LOW |
| Botón Subir / Vol+ | A1 | Activo en LOW |
| Botón Bajar / Vol− | A2 | Activo en LOW |
| Botón 4 / Siguiente (5 btn) | A3 | Activo en LOW |
| Botón 5 / Anterior (5 btn) | A4 | Activo en LOW |
| Tensión batería | A5 | Opcional |
| Acceso directo especial | A6 | Opcional, activo en HIGH |
| Semilla aleatoria (flotante) | A7 | Sin conectar |
| DFPlayer RX (SW Serial) | D2 | → TX del DFPlayer |
| DFPlayer TX (SW Serial) | D3 | ← RX del DFPlayer |
| DFPlayer BUSY | D4 | Activo en LOW |
| Anillo NeoPixel | D5 | Opcional |
| Activar altavoz / LED Subir | D6 | Opcional |
| Apagado | D7 | Activo en HIGH (por defecto) |
| Detección auriculares | D8 | Opcional, activo en LOW |
| MFRC522 RST | D9 | |
| MFRC522 SS | D10 | |
| MFRC522 MOSI | D11 | SPI hardware |
| MFRC522 MISO | D12 | SPI hardware |
| MFRC522 SCK | D13 | SPI hardware |

### ESP32 Wroom 32 — Resumen de Pines

| Función | GPIO | Etiqueta |
|---------|------|----------|
| Botón Pausa | 33 | D33 |
| Botón Subir / Vol+ (3 / 5 btn) | 25 | D25 |
| Botón Bajar / Vol− | 26 | D26 |
| Botón 4 / Siguiente (5 btn) | 27 | D27 |
| Botón 5 / Anterior (5 btn) | 14 | D14 |
| DFPlayer RX (SW/HW Serial2) | 16 | RX2 |
| DFPlayer TX (SW/HW Serial2) | 17 | TX2 |
| Anillo NeoPixel | 2 | D2 |
| Activar altavoz | 15 | D15 |
| Apagado | 13 | D13 |
| Detección auriculares | 34 | D34 |
| Tensión batería | 35 | D35 |
| Acceso directo especial | 12 | D12 |
| Semilla aleatoria (flotante) | 39 | VN |
| DFPlayer BUSY | 4 | D4 |
| MFRC522 RST | 22 | D22 |
| MFRC522 SS | 5 | D5 |
| MFRC522 MOSI | 23 | SPI hardware |
| MFRC522 MISO | 19 | SPI hardware |
| MFRC522 SCK | 18 | SPI hardware |
| Encoder Rotativo CLK | 27 | D27 |
| Encoder Rotativo DT | 14 | D14 |

---

## Bus SPI (MFRC522)

El MFRC522 comparte el bus SPI hardware sin ningún otro dispositivo por defecto.
SS se lleva a LOW para seleccionar el chip.

```
Arduino          MFRC522
  D9  ────────── RST
  D10 ────────── SDA/SS
  D11 ────────── MOSI
  D12 ────────── MISO
  D13 ────────── SCK
  3.3V ──────── 3.3V
  GND ─────────  GND
```

---

## Cableado del DFPlayer Mini

```
Arduino/Placa         DFPlayer Mini
  D3  (TX) ──[1kΩ]── RX
  D2  (RX) ────────── TX
  D4  ─────────────── BUSY  (activo en LOW → reproduciendo)
  5V  ─────────────── VCC
  GND ─────────────── GND
              ┌─────  SPK+
  Altavoz     │
              └─────  SPK−
```

En las variantes con serie hardware, sustituye D2/D3 por el par UART correspondiente
indicado en la tabla anterior.

---

## Mapa de Memoria EEPROM

Definido en `src/settings.cpp`. **No modifiques los desplazamientos;** los
dispositivos existentes dependen de ellos.

| Rango de direcciones | Contenido |
|----------------------|-----------|
| 0 – 99 | Ajustes de carpeta (progreso del audiolibro por carpeta) |
| 100 – 140 | Ajustes de administrador (41 bytes) |
| 141 – 155 | Reservado (15 bytes) |
| 156 – 255 | Accesos directos extra (hasta 25 accesos × 4 bytes) |

Tamaños de EEPROM por placa:

| Placa | EEPROM |
|-------|--------|
| Arduino Nano (Classic) | 2 048 bytes |
| Nano Every | 256 bytes |
| AiO (LGT8F328P) | 512 bytes (emulada) |
| AiO+ | 256 bytes |
| ESP32 Nano / Wroom | 256 bytes (configurados en `EEPROM.begin()`) |

---

## Formato de Datos de Tarjeta RFID

Las tarjetas almacenan una estructura `folderSettings`. Los campos cookie y versión
se comprueban en cada lectura para detectar tarjetas compatibles.

| Campo | Tamaño | Valor / Notas |
|-------|--------|---------------|
| Cookie | 4 bytes | `0x1337b347` — debe coincidir |
| Versión | 1 byte | `0x02` — debe coincidir |
| Carpeta | 1 byte | 1–99 (número de carpeta en la tarjeta SD) |
| Modo | 1 byte | Valor del enum `pmode_t` (ver abajo) |
| Especial | 1 byte | Primera pista / número de pista individual |
| Especial2 | 1 byte | Última pista (para modos de rango) |

### Valores del Enum de Modo de Reproducción (pmode_t) — no deben modificarse

| Valor | Nombre del enum | Descripción |
|-------|----------------|-------------|
| 0 | `none` | Sin modo |
| 1 | `audio_play` | Pista aleatoria de la carpeta |
| 2 | `album` | Todas las pistas en orden |
| 3 | `party` | Todas las pistas en orden aleatorio, en bucle |
| 4 | `single_track` | Una pista específica |
| 5 | `audiobook` | Carpeta completa, guarda el progreso |
| 6 | `admin` | Tarjeta de menú de administrador |
| 7 | `audio_play_range` | Pista aleatoria en [especial … especial2] |
| 8 | `album_vb` | Todas las pistas en [especial … especial2] |
| 9 | `party_vb` | Pistas aleatorias en [especial … especial2], en bucle |
| 10 | `audiobook_single` | N pistas, guarda el progreso |
| 11 | `repeat_last` | Repetir última tarjeta / acceso directo |
| 12 | `quiz_game` | Juego de preguntas |
| 13 | `memory_game` | Juego de memoria |
| 14 | `switch_bt` | Activar / desactivar Bluetooth |
| 15 | `teapot_game` | Juego de la Tetera (Teekesselchen) |
| 16 | `audiobook_range` | Audiolibro en [especial … especial2] |
| 255 | `admin_card` | Tarjeta de administrador |

### Valores del Enum de Modo Modificador (mismo pmode_t, distinto alcance)

| Valor | Nombre del enum | Descripción |
|-------|----------------|-------------|
| 1 | `sleep_timer` | Suspensión automática tras N minutos |
| 2 | `freeze_dance` | Juego del baile congelado |
| 3 | `fi_wa_ai` | Juego Fuego-Agua-Aire |
| 4 | `toddler` | Modo infantil (botones bloqueados) |
| 5 | `kindergarden` | Modo guardería |
| 6 | `repeat_single` | Repetir pista individual |
| 7 | `bt_module` | Control de módulo Bluetooth |
| 8 | `jukebox` | Modificador gramola |
| 9 | `pause_aft_tr` | Pausa después de cada pista |
| 10 | `stdb_timer_sw` | Activar / desactivar temporizador en espera |

> **Crítico:** Los valores numéricos de `pmode_t` se almacenan de forma persistente
> en las tarjetas RFID y en la EEPROM. Renombrar los miembros del enum en el código
> es seguro; **cambiar sus valores numéricos corrompería las tarjetas y los ajustes existentes.**

---

## Estructura de Carpetas de la Tarjeta SD

```
Raíz de la tarjeta SD/
├── mp3/       ← Mensajes de voz del sistema (numerados 0300 – 0999)
├── advert/    ← Clips de audio superpuestos (anuncios)
├── 01/        ← Carpeta de usuario 1 (pistas 001.mp3 – 255.mp3)
├── 02/        ← Carpeta de usuario 2
│   ...
└── 99/        ← Carpeta de usuario 99 (máximo)
```

Nomenclatura de archivos de pista: `NNN_titulo-opcional.mp3` (p. ej. `001_hola.mp3`).

---

## Diagrama de Arquitectura de Componentes

```
┌─────────────────────────────────────────────────────────────┐
│                    Firmware TrovaLibre                       │
│                    (Arduino / ESP32)                         │
│                                                              │
│  ┌──────────┐   SPI    ┌────────────┐                        │
│  │TrovaLibre│◄────────►│  MFRC522   │◄── Tarjeta RFID        │
│  │  clase   │          │ Lector RFID│                        │
│  │          │                                                │
│  │          │  UART    ┌────────────┐    ┌──────────┐        │
│  │          │◄────────►│ DFPlayer   │───►│ Altavoz  │        │
│  │          │          │   Mini     │    └──────────┘        │
│  │          │          └────────────┘                        │
│  │          │   GPIO                                         │
│  │          │◄──────── Botones (3 / 5 / 3×3)                │
│  │          │                                                │
│  │ Máquina  │   PWM    ┌────────────┐                        │
│  │  de      │─────────►│  NeoPixel  │  (opcional)            │
│  │ Estados  │          │   Anillo   │                        │
│  │          │   GPIO   └────────────┘                        │
│  │          │◄──────── Encoder rotativo  (opcional)          │
│  │          │◄──────── Potenciómetro     (opcional)          │
│  │          │◄──────── Detección auriculares (opcional)      │
│  │          │─────────► Activación ampli.   (opcional)       │
│  │          │─────────► Pin de apagado      (control alim.)  │
│  │          │◄──────── ADC batería          (opcional)       │
│  │          │─────────► BT módulo ON        (opcional)       │
│  │          │─────────► BT emparejamiento   (opcional)       │
│  │          │                                                │
│  │ Ajustes  │  EEPROM  ┌────────────┐                        │
│  │          │◄────────►│   EEPROM   │ (en chip)              │
│  └──────────┘          └────────────┘                        │
│                                                              │
│  [Solo ESP32]  WiFi    ┌────────────┐                        │
│                ◄──────►│  Panel Web │ admin / OTA            │
│                        └────────────┘                        │
└─────────────────────────────────────────────────────────────┘
```

---

*Fuente: `src/constants.hpp`, `src/settings.cpp`, `src/chip_card.hpp`*
*Proyecto: TrovaLibre — basado en TonUINO / TonUINO-TNG*
