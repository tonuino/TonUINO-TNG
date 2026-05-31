# TrovaLibre — Hardware Connections

> This file is generated from `src/constants.hpp` and reflects exactly the pin
> assignments defined in the firmware. Verify against your physical board before
> wiring.

---

## Supported Board Variants

| Macro | Board | MCU |
|-------|-------|-----|
| `TROVALIBRE_CLASSIC` | Arduino Nano (ATmega328P) | Classic |
| `TROVALIBRE_EVERY` | Arduino Nano Every (ATmega4809) | Every |
| `TROVALIBRE_EVERY_4808` | Arduino Nano Every (ATmega4808) | Every 4808 |
| `ALLinONE` | ALLinONE board (LGT8F328P) | AiO |
| `ALLinONE_Plus` | ALLinONE Plus board (ATmega4809) | AiO+ |
| `TROVALIBRE_ESP32 100` | Arduino Nano ESP32 | ESP32 Nano |
| `TROVALIBRE_ESP32 200` | ESP32 Wroom 32 (Denky32) | ESP32 Wroom |

Set **exactly one** of these in `src/constants.hpp` (or via PlatformIO
`-D` flag in `platformio.ini`).

---

## Core Components — All Variants

These components are present in every build.

### MFRC522 RFID Reader (SPI)

The MFRC522 communicates over SPI. MOSI/MISO/SCK use the board's hardware SPI
pins (not redefinable here). Only RST and SS/SDA are software-defined.

| Signal | Classic / Every / AiO / ESP32 Nano | AiO+ | ESP32 Wroom |
|--------|--------------------------------------|------|-------------|
| RST | D9 | D11 | D22 |
| SS (SDA) | D10 | D7 | D5 |
| MOSI | D11 (hardware SPI) | hardware SPI | hardware SPI |
| MISO | D12 (hardware SPI) | hardware SPI | hardware SPI |
| SCK | D13 (hardware SPI) | hardware SPI | hardware SPI |
| 3.3 V | 3.3 V rail | 3.3 V rail | 3.3 V rail |
| GND | GND | GND | GND |

> Card cookie (EEPROM identifier): `0x1337b347`
> Card format version: `0x02`
> Card remove delay cycles: `3`
> **Warning:** Do not change `cardCookie` or `cardVersion`; existing programmed
> RFID cards depend on these values.

---

### DFPlayer Mini (MP3 module)

| Signal | Classic / Every (SW Serial, default) | Every / ESP32 Nano (HW Serial) | AiO (SW Serial) | AiO+ (HW Serial) | ESP32 Wroom |
|--------|---------------------------------------|--------------------------------|-----------------|-------------------|-------------|
| RX (board → player) | D3 (TX pin) | D1 (Serial1 TX) | D3 | Serial3 TX | D17 / Serial2 TX |
| TX (player → board) | D2 (RX pin) | D0 (Serial1 RX) | D2 | Serial3 RX | D16 / Serial2 RX |
| BUSY | D4 (active LOW) | D4 | D4 | D13 | D4 |
| VCC | 5 V | 5 V | 5 V | 5 V | 5 V |
| GND | GND | GND | GND | GND | GND |
| SPK+ / SPK− | Speaker | Speaker | Speaker | Speaker | Speaker |

Hardware serial mode is enabled with `#define DFPlayerUsesHardwareSerial`
(required for AiO+, optional for Every and ESP32 Nano).

Startup wait time: `1 200 ms` (default) / `2 500 ms` for slow chips
(MH2024K24SS, GD3200B).

---

## Buttons

Button logic level: **active LOW** (internal pull-up; open = HIGH, pressed = LOW).
Long-press threshold: `1 000 ms`.
Debounce time: `25 ms`.

### 3-Button Layout (default for Classic / Every; enable with `#define THREEBUTTONS`)

| Button | Classic / Every / ESP32 Nano | AiO | AiO+ | ESP32 Wroom |
|--------|-------------------------------|-----|------|-------------|
| Pause/Play | A0 | A0 | A0 | D33 |
| Volume Up / Next | A1 | A2 | A2 | D25 |
| Volume Down / Prev | A2 | A1 | A1 | D26 |

### 5-Button Layout (default for AiO / AiO+; enable with `#define FIVEBUTTONS`)

| Button | Classic / Every / ESP32 Nano | AiO | AiO+ | ESP32 Wroom |
|--------|-------------------------------|-----|------|-------------|
| Pause/Play | A0 | A0 | A0 | D33 |
| Volume+ | A1 | A2 | A2 | D25 |
| Volume− | A2 | A1 | A1 | D26 |
| Next (button 4) | A3 | A4 | A4 | D27 |
| Previous (button 5) | A4 | A3 | A3 | D14 |

### 3×3 Button Board (enable with `#define BUTTONS3X3`)

| Signal | Classic / Every / ESP32 Nano | AiO | AiO+ | ESP32 Wroom |
|--------|-------------------------------|-----|------|-------------|
| Pause/Play | A0 | A0 | A0 | D33 |
| Up | A1 | A2 | A4 | D25 |
| Down | A2 | A1 | A3 | D26 |
| Analog matrix | A3 | A2 | A2 | D27 |
| Debounce time | 50 ms | 50 ms | 50 ms | 50 ms |

> **Note:** 3×3 and 5-button modes are mutually exclusive.

---

## Optional Components

Enable each feature by uncommenting the corresponding `#define` in
`src/constants.hpp`.

### Shutdown Pin

Controls a power hold or Pololu switch to cut power.

| Board | Pin | Active level |
|-------|-----|-------------|
| Classic / Every (default) | D7 | HIGH |
| Classic / Every (`USE_TRAEGER_PLATINE_SHUTDOWN`) | D7 | LOW |
| AiO | D7 | LOW |
| AiO+ | D27 | LOW |
| ESP32 Nano (default) | D7 | HIGH |
| ESP32 Nano (`USE_TRAEGER_PLATINE_SHUTDOWN`) | D7 | LOW |
| ESP32 Wroom (default) | D13 | LOW |
| ESP32 Wroom (`USE_POLOLU_SHUTDOWN`) | D13 | HIGH |

Options:
- `#define USE_POLOLU_SHUTDOWN` — for Pololu power switch
- `#define USE_TRAEGER_PLATINE_SHUTDOWN` — for carrier board

---

### Speaker On/Off (Amplifier Enable) — `#define SPKONOFF`

Mutes/enables the speaker amplifier to suppress power-on/off noise.
Automatically enabled on AiO and AiO+.

| Board | Pin | Active level |
|-------|-----|-------------|
| Classic / Every / ESP32 Nano (`SPKONOFF`) | D6 | HIGH |
| AiO | D8 | LOW |
| AiO+ | D19 (PC5) | LOW |
| ESP32 Wroom | D15 | LOW |

---

### Headphone Jack Detection — `#define HPJACKDETECT`

Detects insertion of headphones; switches audio output accordingly.
Automatically enabled on AiO+.

| Board | Pin | Active level |
|-------|-----|-------------|
| Classic / Every / ESP32 Nano | D8 | LOW |
| AiO+ | D21 (PC7) | LOW |
| ESP32 Wroom | D34 | LOW |

---

### NeoPixel LED Ring — `#define NEO_RING`

One data wire to the ring's DIN pad. Use a 300–470 Ω series resistor between
the Arduino pin and DIN. Ring pixel count default: `24`.

| Board | Pin |
|-------|-----|
| Classic / Every / AiO / ESP32 Nano | D5 |
| AiO+ | D10 (PB2) |
| ESP32 Wroom | D2 (GPIO2) |

Optional second ring (`#define NEO_RING_2`):

| Board | Pin |
|-------|-----|
| Classic / Every (Every only) | D2 |
| AiO+ | D14 (PC0) |

Enable extended mode (displays volume level on ring): `#define NEO_RING_EXT`.

---

### Rotary Encoder — `#define ROTARY_ENCODER`

Controls volume. Optionally also controls next/previous with
`#define ROTARY_ENCODER_LONGPRESS`.

| Board | CLK pin | DT pin | Pull-up | Debounce |
|-------|---------|--------|---------|---------|
| Classic / Every / AiO / ESP32 Nano | A3 | A4 | none (external) | 75 ms |
| AiO+ (Opt header / Male) | D36 (PF2) | D37 (PF3) | none | 75 ms |
| AiO+ (Rotary Encoder socket) | D31 (PE1) | D32 (PE2) | none | 75 ms |
| ESP32 Wroom | D27 | D14 | none | 75 ms |

> **Note:** Rotary encoder and potentiometer cannot be used at the same time
> on Classic / Every (same pin A3).

---

### Potentiometer (Volume) — `#define POTI`

Analog potentiometer for volume control.

| Board | Pin |
|-------|-----|
| Classic / Every / AiO | A3 |
| AiO+ | A14 (PF4) |
| ESP32 Wroom | D27 |

---

### Battery Voltage Measurement — `#define BAT_VOLTAGE_MEASUREMENT`

Reads battery voltage through a voltage divider.
Not recommended for ALLinONE. Not available for ALLinONE.

| Board | Pin | Divider correction factor |
|-------|-----|--------------------------|
| Classic | A5 | 1.960 |
| Every | A5 | 2.007 |
| Every 4808 | A5 | 2.007 |
| AiO+ | A6 | 2.075 |
| ESP32 Wroom | D35 | 0.710 |

Voltage divider: 100 kΩ / 100 kΩ to GND.

Thresholds (Li-Ion default):
- Low: 2.95 V
- Empty: 2.90 V

Li-Po alternative values are commented out in `constants.hpp`.

---

### Special Start Shortcut via GPIO — `#define SPECIAL_START_SHORTCUT`

Reads a GPIO at startup; if active, plays folder 1 / track 1.

| Board | Pin | Active level |
|-------|-----|-------------|
| Classic / Every / AiO | A6 | HIGH |
| AiO+ | D33 (PE3) | HIGH |
| ESP32 Nano | A6 | HIGH |
| ESP32 Wroom | D12 | HIGH |

---

### Open Analog Pin (Random Seed)

Floated analog pin used to seed the random number generator.

| Board | Pin |
|-------|-----|
| All variants | A7 |
| ESP32 Wroom | D39 (VN) |

---

### Bluetooth Module — `#define BT_MODULE`

Controls an external BT audio module.

| Signal | Without `DFPlayerUsesHardwareSerial` | With `DFPlayerUsesHardwareSerial` |
|--------|--------------------------------------|-----------------------------------|
| BT ON/OFF | D6 | D2 |
| BT Pairing | D8 | D3 |
| Active level | HIGH | HIGH |
| Pairing pulse | 500 ms | 500 ms |

---

### LED Buttons — `#define USE_LED_BUTTONS`

Illuminates the physical buttons via the LedManager.
Use a 1 kΩ series resistor per LED to limit current.

| LED | Pin (Classic / Every / AiO / ESP32 Nano) |
|-----|------------------------------------------|
| Play/Pause | A5 |
| Up | D6 |
| Down | D5 |

Timing:
- Update interval: 800 ms
- Short blink: 400 ms

---

### Power Hold Ready Signal — `#define POWER_HOLD_READY`

Signals to external circuitry that the MCU is ready to hold its own power.

| Board | Pin | Active level |
|-------|-----|-------------|
| All | D2 | HIGH |

---

### USB Access Pin (AiO / AiO+ only)

Controls USB host access on integrated boards.

| Board | Pin | Active level |
|-------|-----|-------------|
| AiO | A5 | HIGH |
| AiO+ | D20 | HIGH |

---

## Pin Summary Tables

### Classic / Every / Every4808 — Most Common Layout

| Function | Pin | Note |
|----------|-----|------|
| Button Pause | A0 | Active LOW |
| Button Up / Vol+ | A1 | Active LOW |
| Button Down / Vol− | A2 | Active LOW |
| Button 4 / Next (5-btn) | A3 | Active LOW |
| Button 5 / Prev (5-btn) | A4 | Active LOW |
| Battery voltage | A5 | Optional |
| Special start shortcut | A6 | Optional, active HIGH |
| Random seed (open) | A7 | Floating |
| DFPlayer RX (SW Serial) | D2 | → DFPlayer TX |
| DFPlayer TX (SW Serial) | D3 | ← DFPlayer RX |
| DFPlayer BUSY | D4 | Active LOW |
| NeoPixel ring | D5 | Optional |
| Speaker enable / LED Up | D6 | Optional |
| Shutdown | D7 | Active HIGH (default) |
| Headphone detect | D8 | Optional, active LOW |
| MFRC522 RST | D9 | |
| MFRC522 SS | D10 | |
| MFRC522 MOSI | D11 | Hardware SPI |
| MFRC522 MISO | D12 | Hardware SPI |
| MFRC522 SCK | D13 | Hardware SPI |

### ESP32 Wroom 32 — Pin Summary

| Function | GPIO | Board label |
|----------|------|-------------|
| Button Pause | 33 | D33 |
| Button Up / Vol+ (3-btn / 5-btn) | 25 | D25 |
| Button Down / Vol− | 26 | D26 |
| Button 4 / Next (5-btn) | 27 | D27 |
| Button 5 / Prev (5-btn) | 14 | D14 |
| DFPlayer RX (SW/HW Serial2) | 16 | RX2 |
| DFPlayer TX (SW/HW Serial2) | 17 | TX2 |
| NeoPixel ring | 2 | D2 |
| Speaker enable | 15 | D15 |
| Shutdown | 13 | D13 |
| Headphone detect | 34 | D34 |
| Battery voltage | 35 | D35 |
| Special start shortcut | 12 | D12 |
| Random seed (open) | 39 | VN |
| DFPlayer BUSY | 4 | D4 |
| MFRC522 RST | 22 | D22 |
| MFRC522 SS | 5 | D5 |
| MFRC522 MOSI | 23 | Hardware SPI |
| MFRC522 MISO | 19 | Hardware SPI |
| MFRC522 SCK | 18 | Hardware SPI |
| Rotary CLK | 27 | D27 |
| Rotary DT | 14 | D14 |

---

## SPI Bus (MFRC522)

The MFRC522 shares the hardware SPI bus with no other device by default.
SS is pulled LOW to select the chip.

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

## DFPlayer Mini Wiring

```
Arduino/Board         DFPlayer Mini
  D3  (TX) ──[1kΩ]── RX
  D2  (RX) ────────── TX
  D4  ─────────────── BUSY  (active LOW → playing)
  5V  ─────────────── VCC
  GND ─────────────── GND
              ┌─────  SPK+
  Speaker     │
              └─────  SPK−
```

For hardware serial variants replace D2/D3 with the appropriate UART pair shown
in the table above.

---

## EEPROM Memory Map

Defined in `src/settings.cpp`. **Do not change offsets;** existing devices depend on them.

| Address range | Content |
|---------------|---------|
| 0 – 99 | Folder settings (audiobook progress per folder) |
| 100 – 140 | Admin settings (41 bytes) |
| 141 – 155 | Reserved (15 bytes) |
| 156 – 255 | Extra shortcuts (up to 25 shortcuts × 4 bytes) |

EEPROM sizes by board:

| Board | EEPROM |
|-------|--------|
| Arduino Nano (Classic) | 2 048 bytes |
| Nano Every | 256 bytes |
| AiO (LGT8F328P) | 512 bytes (emulated) |
| AiO+ | 256 bytes |
| ESP32 Nano / Wroom | 256 bytes (configured in `EEPROM.begin()`) |

---

## RFID Card Data Format

Cards store a `folderSettings` struct. The cookie and version fields are checked
on every read to detect compatible cards.

| Field | Size | Value / Notes |
|-------|------|---------------|
| Cookie | 4 bytes | `0x1337b347` — must match |
| Version | 1 byte | `0x02` — must match |
| Folder | 1 byte | 1–99 (SD card folder number) |
| Mode | 1 byte | `pmode_t` enum value (see below) |
| Special | 1 byte | First track / single track number |
| Special2 | 1 byte | Last track (for range modes) |

### Play Mode Enum Values (pmode_t) — must not be changed

| Value | Enum name | Description |
|-------|-----------|-------------|
| 0 | `none` | No mode |
| 1 | `audio_play` | Random track from folder |
| 2 | `album` | All tracks in order |
| 3 | `party` | All tracks in random order, looping |
| 4 | `single_track` | One specific track |
| 5 | `audiobook` | Full folder, saves progress |
| 6 | `admin` | Admin menu card |
| 7 | `audio_play_range` | Random track in [special … special2] |
| 8 | `album_vb` | All tracks in [special … special2] |
| 9 | `party_vb` | Random tracks in [special … special2], looping |
| 10 | `audiobook_single` | N tracks, saves progress |
| 11 | `repeat_last` | Repeat last card/shortcut |
| 12 | `quiz_game` | Quiz game |
| 13 | `memory_game` | Memory game |
| 14 | `switch_bt` | Toggle Bluetooth on/off |
| 15 | `teapot_game` | Teapot (Teekesselchen) game |
| 16 | `audiobook_range` | Audiobook in [special … special2] |
| 255 | `admin_card` | Admin card |

### Modifier Mode Enum Values (same pmode_t, different scope)

| Value | Enum name | Description |
|-------|-----------|-------------|
| 1 | `sleep_timer` | Auto-sleep after N minutes |
| 2 | `freeze_dance` | Freeze dance game |
| 3 | `fi_wa_ai` | Fire-Water-Air game |
| 4 | `toddler` | Toddler mode (buttons locked) |
| 5 | `kindergarden` | Daycare mode |
| 6 | `repeat_single` | Repeat single track |
| 7 | `bt_module` | Bluetooth module control |
| 8 | `jukebox` | Jukebox modifier |
| 9 | `pause_aft_tr` | Pause after each track |
| 10 | `stdb_timer_sw` | Toggle standby timer |

> **Critical:** The numeric values of `pmode_t` are persisted on RFID cards and
> in EEPROM. Renaming enum members in code is safe; **changing their numeric
> values would corrupt existing cards and settings.**

---

## SD Card Folder Structure

```
SD card root/
├── mp3/       ← System voice prompts (numbered 0300 – 0999)
├── advert/    ← Advertised (overlay) audio clips
├── 01/        ← User folder 1 (tracks 001.mp3 – 255.mp3)
├── 02/        ← User folder 2
│   ...
└── 99/        ← User folder 99 (max)
```

Track file naming: `NNN_optional-title.mp3` (e.g. `001_hello.mp3`).

---

## Component Architecture Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                       TrovaLibre firmware                    │
│                      (Arduino / ESP32)                       │
│                                                              │
│  ┌──────────┐   SPI    ┌────────────┐                        │
│  │  TrovaLibre │◄────────► MFRC522    │◄── RFID card          │
│  │  class   │          │ RFID reader│                        │
│  │          │                                                │
│  │          │  UART    ┌────────────┐    ┌──────────┐        │
│  │          │◄────────►│ DFPlayer   │───►│ Speaker  │        │
│  │          │          │ Mini       │    └──────────┘        │
│  │          │          └────────────┘                        │
│  │          │   GPIO                                         │
│  │          │◄──────── Buttons (3 / 5 / 3×3)                │
│  │          │                                                │
│  │ State    │   PWM    ┌────────────┐                        │
│  │ Machine  │─────────►│ NeoPixel   │  (optional)            │
│  │          │          │ Ring       │                        │
│  │          │   GPIO   └────────────┘                        │
│  │          │◄──────── Rotary encoder  (optional)            │
│  │          │◄──────── Potentiometer   (optional)            │
│  │          │◄──────── Headphone jack  (optional)            │
│  │          │          detect                                │
│  │          │─────────► Amp enable     (optional)            │
│  │          │─────────► Shutdown pin   (power control)       │
│  │          │◄──────── Battery ADC     (optional)            │
│  │          │─────────► BT module ON   (optional)            │
│  │          │─────────► BT pairing     (optional)            │
│  │          │                                                │
│  │ Settings │  EEPROM  ┌────────────┐                        │
│  │          │◄────────►│ EEPROM     │ (on-chip)              │
│  └──────────┘          └────────────┘                        │
│                                                              │
│  [ESP32 only]  WiFi    ┌────────────┐                        │
│                ◄──────►│ Web UI     │ admin / OTA            │
│                        └────────────┘                        │
└─────────────────────────────────────────────────────────────┘
```

---

*Source: `src/constants.hpp`, `src/settings.cpp`, `src/chip_card.hpp`*
*Project: TrovaLibre — based on TonUINO / TonUINO-TNG*
