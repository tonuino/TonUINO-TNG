# TrovaLibre RFID Card Format Specification

**Document version:** 1.0 (2026-05-23)  
**Firmware version:** TrovaLibre-TNG (any)  
**Card format version:** 0x02 (cardVersion)

---

## Overview

This document defines the binary format of RFID cards written by TrovaLibre firmware. It is the **authoritative contract** for third-party tools, integrations, and future implementations.

Every card written by TrovaLibre firmware stores exactly **9 bytes** at **sector 1, block 0** of a MIFARE Classic 1K chip.

---

## Byte Layout

| Byte Range | Field | Size | Value(s) | Description |
|---|---|---|---|---|
| 0–3 | `cardCookie` | 4 B | `0x13 0x37 0xB3 0x47` | **Validation cookie**. If these 4 bytes do not match exactly, firmware treats the card as uninitialized/unrecognized. **Hex mnemonic:** 0x1337b347 = "1337" (leet) + "b3 47" (B347, a nod to early TonUINO days). |
| 4 | `cardVersion` | 1 B | `0x02` | **Format version**. Current version is 2. If firmware encounters a card with `cardVersion > 0x02`, it may refuse to read it (to avoid compatibility issues). Allows future evolution without breaking existing cards. |
| 5 | `folder` | 1 B | 1–99 | **SD card folder number**. Folder 00 is reserved (ignored). Folders 1–99 map to SD root directories `/01/`, `/02/`, ..., `/99/`. Folder 100+ treated as invalid. |
| 6 | `mode` | 1 B | See table below | **Playback mode** or **modifier type** (`pmode_t` enum). Determines card behavior: which files to play, in what order, which buttons do what, etc. |
| 7 | `special` | 1 B | 0–255 | **Parameter 1**. Meaning depends on `mode`: track number (Individual), range start (audio_play_range, etc.), quiz option count (quiz_game), modifier level (freeze_dance), timer minutes (sleep_timer), etc. If mode doesn't use it, value is ignored (set to 0x00). |
| 8 | `special2` | 1 B | 0–255 | **Parameter 2**. Meaning depends on `mode`: range end (_range modes), include solution (quiz_game), etc. If mode doesn't use it, value is ignored (set to 0x00). |

---

## Mode Enumeration (`pmode_t`)

### Folder Content Modes (playback modes)

These are card modes that play audio from an SD card folder. Firmware selects files from folder `folder` according to the mode's playback logic.

| Value | Identifier | Name (ES) | Parameters | Behavior |
|-------|---|---|---|---|
| 0x00 | `none` | — | — | Invalid/uninitialized. |
| 0x01 | `audio_play` | Cuento | — | Play **one random track** from folder. No sequential navigation. Each card placement selects a different random track. |
| 0x02 | `album` | Álbum | — | Play **all tracks sequentially** from start to end. Next/Previous buttons navigate. Stops at end. |
| 0x03 | `party` | Fiesta | — | Play **all tracks shuffled, infinite loop**. Reshuffles each cycle. Next/Previous buttons navigate within current shuffle. |
| 0x04 | `single_track` | Individual | `special` = track # | Play **one specific track** (1–255, field `special`). Ignores other files in folder. |
| 0x05 | `audiobook` | Audiolibro | — | Play **all tracks sequentially** (like Album) **with persistent progress**. Firmware saves which track was reached in EEPROM. Next card placement resumes from saved position. Useful for long stories. |
| 0x06 | `admin` | — | — | Internal mode. Do not use on user cards. |
| 0x07 | `audio_play_range` | Cuento con rango | `special` = start, `special2` = end | Play **one random track** from a sub-range of the folder (tracks `special` to `special2` inclusive). |
| 0x08 | `album_vb` | Álbum con rango | `special` = start, `special2` = end | Play **all tracks sequentially** from sub-range (tracks `special` to `special2` inclusive). |
| 0x09 | `party_vb` | Fiesta con rango | `special` = start, `special2` = end | Play **all tracks shuffled, infinite loop** from sub-range. |
| 0x0A | `audiobook_single` | Audiolibro individual | `special` = track # | Play **one track** with **persistent progress** (like Audiobook but single track). Rarely used; not commonly documented. |
| 0x0B | `repeat_last` | Repetir última | — | **Meta mode**: re-activate the last card that was played (before this card was placed). Does not point to a folder. Useful as a "replay" shortcut. |
| 0x0C | `quiz_game` | Quiz | `special` = option count (0/2/4), `special2` = include solution (0/1) | **Interactive quiz mode**. Firmware enters Quiz state machine. Folder must contain structured audio: questions + answers + optional solutions. Requires `QUIZ_GAME` compile flag. |
| 0x0D | `memory_game` | Memoria | — | **Memory matching game**. Firmware enters Memory state machine. Folder contains paired sound files (odd–even pairs are matched). Requires memory game state. |
| 0x0E | `switch_bt` | — | — | **Bluetooth control mode**. Toggles external Bluetooth module (if installed). Undocumented; rarely used. |
| 0x0F | `teapot_game` | Palabra Secreta | — | **Secret word guessing game** (internal name: `teapot_game`, from German *Teekesselchen*). Firmware enters Teapot state machine. Folder contains 6-track blocks: intro + 4 clues + solution. Requires `TEAPOT_GAME` compile flag. Public name: "Palabra Secreta" (Spanish for "Secret Word"). |
| 0x10 | `audiobook_range` | Audiolibro con rango | `special` = start, `special2` = end | Play **all tracks sequentially with persistent progress** from sub-range (like Audiobook + range). |

### Modifier Modes (behavior modifiers)

These are card modes that **overlay** onto any active playback. They do **not** point to a folder; instead, they modify how the current content behaves. Place a modifier card while content is playing to toggle the modifier on/off.

| Value | Identifier | Name (ES) | Parameters | Behavior |
|-------|---|---|---|---|
| 0x01 | `sleep_timer` | Temporizador de apagado | `special` = minutes (5/15/30/60) | Device powers off after `special` minutes. Firmware announces countdown. Place same card again to cancel. |
| 0x02 | `freeze_dance` | Baile congelado | `special` = difficulty (1/2/3) | Music pauses randomly. Players must "freeze" until button pressed to resume. Difficulty controls pause frequency. |
| 0x03 | `fi_wa_ai` | Fuego, Agua y Viento | `special` = difficulty (1/2/3) | Like freeze_dance but announces "Fuego"/"Agua"/"Viento" (Fire/Water/Wind) — players must perform corresponding actions. German origin (*Feuer, Wasser, Luft*). |
| 0x04 | `toddler` | Modo Peques | — | **All physical buttons locked**. Only RFID cards can change content. Prevents accidental button presses by very young children. |
| 0x05 | `kindergarden` | Modo Guardería | — | **Queuing mode**. New cards enqueue instead of interrupting. Button navigation restricted (no track skips). Safer for group use. Note: spelling "kindergarden" (not "kindergarten") is intentional for firmware compatibility. |
| 0x06 | `repeat_single` | Repetir pista | — | **Current track loops infinitely**. When track ends, restart from beginning instead of advancing. Place same card again to toggle off. |
| 0x07 | `bt_module` | Interruptor Bluetooth | — | Toggle external Bluetooth module. Requires `TROVALIBRE_ESP32` and hardware BT module. |
| 0x08 | `jukebox` | Jukebox | — | **Queue mode**. Up to 10 cards can queue. New cards don't interrupt; they append to queue. Requires `MODIFICATION_CARD_JUKEBOX` compile flag. |
| 0x09 | `pause_aft_tr` | Pausa tras pista | — | Firmware pauses after each track ends (instead of auto-advancing). Press Play to continue. Useful for narration control. Requires `MODIFICATION_CARD_PAUSE_AFTER_TRACK` compile flag. |
| 0x0A | `stdb_timer_sw` | Interruptor standby | — | Toggle the automatic standby timer on/off. If device normally powers down after 30 min inactivity, this card disables that behavior temporarily. |

### Special Values

| Value | Meaning |
|---|---|
| 0xFF | **Admin card**. Bypasses security and opens the admin menu immediately when placed on the reader (useful if buttons are disabled). Not a playback mode. |

---

## Practical Examples

### Example 1: Content Card — "Cuento" (Random Story)

```
Byte 0–3: 0x13 0x37 0xB3 0x47  (cookie)
Byte 4:   0x02                  (version)
Byte 5:   0x0A                  (folder 10)
Byte 6:   0x01                  (mode: audio_play)
Byte 7:   0x00                  (special: unused)
Byte 8:   0x00                  (special2: unused)
```

**Meaning:** When placed, device picks a random MP3 file from folder `/10/` and plays it.

---

### Example 2: Content Card — "Individual" (One Specific Track)

```
Byte 0–3: 0x13 0x37 0xB3 0x47  (cookie)
Byte 4:   0x02                  (version)
Byte 5:   0x14                  (folder 20)
Byte 6:   0x04                  (mode: single_track)
Byte 7:   0x03                  (special: track 3)
Byte 8:   0x00                  (special2: unused)
```

**Meaning:** Play track 003.mp3 from folder `/20/`.

---

### Example 3: Game Card — "Quiz" (2 Options, with Solution)

```
Byte 0–3: 0x13 0x37 0xB3 0x47  (cookie)
Byte 4:   0x02                  (version)
Byte 5:   0x32                  (folder 50)
Byte 6:   0x0C                  (mode: quiz_game)
Byte 7:   0x02                  (special: 2 options)
Byte 8:   0x01                  (special2: include solution)
```

**Meaning:** Enter quiz mode with folder `/50/`. Each question has 2 answer options, and the solution is included (question + opt1 + opt2 + solution = 4 tracks per question).

---

### Example 4: Modifier Card — "Sleep Timer" (30 minutes)

```
Byte 0–3: 0x13 0x37 0xB3 0x47  (cookie)
Byte 4:   0x02                  (version)
Byte 5:   0x00                  (folder: unused)
Byte 6:   0x01                  (mode: sleep_timer)
Byte 7:   0x1E                  (special: 30 decimal)
Byte 8:   0x00                  (special2: unused)
```

**Meaning:** When placed, device announces "30 minutes" and sets a 30-minute auto-shutdown timer.

---

## Implementation Notes

### Byte Order
- All multi-byte values are stored **little-endian** (least significant byte first). Exception: `cardCookie` is a fixed literal sequence, no endianness.

### EEPROM Persistence
- The 9 bytes above are written to **MIFARE sector 1, block 0** only.
- Firmware also maintains per-card **progress state** in microcontroller EEPROM (not on the card):
  - For `audiobook` mode: current track number (linked by folder ID).
  - This state is **not** read from or written to the card; it's stored in device EEPROM and keyed by `folder` value.

### Compatibility & Future Versions
- If `cardVersion != 0x02`, firmware behavior is undefined. Future versions might:
  - Ignore the card entirely (safe default).
  - Try to interpret with version-specific logic.
  - This field exists to enable graceful degradation.

### Validation
- Firmware **always** checks `cardCookie` first. If it doesn't match `0x1337b347`, the card is treated as blank/uninitialized, regardless of other bytes.
- After checking cookie and version, firmware validates `mode` is in the legal enumeration range.
- If `folder` is 0 or >99, behavior is undefined (usually treated as invalid).

---

## References

- **Firmware source (authoritative):**
  - [`src/constants.hpp`](https://github.com/eTrova/TrovaLibre-TNG/blob/main/src/constants.hpp) — definitions of `cardCookie` and `cardVersion`
  - [`src/chip_card.hpp`](https://github.com/eTrova/TrovaLibre-TNG/blob/main/src/chip_card.hpp) — `struct folderSettings` and `enum pmode_t`
  - [`src/chip_card.cpp`](https://github.com/eTrova/TrovaLibre-TNG/blob/main/src/chip_card.cpp) — read/write implementation

- **Documentation:**
  - [Configurar las tarjetas](https://trovalibre.es/configura/configurar-tarjeta.html) — user-friendly setup guide
  - [Modos de reproducción](https://trovalibre.es/juega/reproduccion.html) — description of playback modes
  - [Juegos](https://trovalibre.es/juega/juegos-contenido.html) — game mode details

---

## Change History

| Version | Date | Changes |
|---|---|---|
| 1.0 | 2026-05-23 | Initial specification. Firmware v0x02 encoding complete. |

---

*This specification is part of the TrovaLibre open-source project (GitHub: eTrova/TrovaLibre-TNG, License: GPL-3.0). For questions or contributions, see the repository.*
