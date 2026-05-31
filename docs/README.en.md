# TrovaLibre

TrovaLibre is an open source Arduino-based audio player for children, based on the TonUINO / TonUINO-TNG project, designed to play stories, music, and educational audio from RFID cards.

TrovaLibre is an adaptation and localization of the TonUINO / TonUINO-TNG open source project for Spanish-speaking users.

TrovaLibre is based on the open source TonUINO / TonUINO-TNG project.

TrovaLibre esta basado en el proyecto de software libre TonUINO / TonUINO-TNG.

## What Is TrovaLibre

TrovaLibre keeps the proven TonUINO architecture and behavior while rebranding and localizing the project for Spanish-speaking families, makers, and educators.

## Origin And Credits

- Original project: TonUINO / TonUINO-TNG
- Original creators and contributors are preserved
- Thorsten Voss and Boerge1 attribution is kept in source and startup logs
- Original license terms and notices are preserved

## Hardware Requirements

- Arduino-compatible board (Classic, Every, Every4808, ALLinONE, ALLinONE_Plus, ESP32 variants)
- DFPlayer Mini compatible module
- MFRC522 RFID reader
- Buttons (3, 5, or 3x3 matrix depending on build)
- Speaker and optional headphone jack detection hardware
- Optional: NeoPixel ring, rotary encoder, potentiometer, BT module

## Installation

### PlatformIO

1. Install PlatformIO.
2. Open this repository.
3. Select one of the `TrovaLibre_*` environments in `platformio.ini`.
4. Build and upload.

### Arduino IDE

1. Open `TrovaLibre.ino`.
2. Select your board and port.
3. Configure board/profile options in `src/constants.hpp`.
4. Compile and upload.

## Configuration

- Board and feature toggles live in `src/constants.hpp`.
- Build environments are defined in `platformio.ini`.
- Runtime/admin settings are stored in EEPROM as before.

## Basic Usage

1. Power on TrovaLibre.
2. Place a configured RFID card.
3. Use buttons to play, pause, navigate tracks, and adjust volume.
4. Enter admin menu to configure advanced behavior.

## How To Program RFID Cards

1. Present a new RFID card.
2. Follow spoken prompts.
3. Select folder and mode.
4. Save card configuration.

Supported modes include album, random play, audiobook, single track, quiz/memory, and modifier cards.

## Build And Test

- CMake unit tests are under `test/`.
- PlatformIO firmware builds are under `platformio.ini`.
- CI workflow is defined in `.github/workflows/build_and_run_tests.yml`.

## License

This project is distributed under GNU GPL v3. See `LICENSE`.

Attribution note:

- TrovaLibre is based on the open source TonUINO / TonUINO-TNG project.
- TrovaLibre is an adaptation and localization of the TonUINO / TonUINO-TNG open source project for Spanish-speaking users.

## Contributing

Please read [`.github/CONTRIBUTING.md`](../.github/CONTRIBUTING.md) before opening pull requests.

For Spanish documentation, see [`README.md`](../README.md).
