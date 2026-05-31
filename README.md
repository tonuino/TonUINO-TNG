# TrovaLibre

> 🇬🇧 English version: [docs/README.en.md](docs/README.en.md)

TrovaLibre es un reproductor de audio libre basado en Arduino, basado en TonUINO, pensado para que ninas y ninos puedan escuchar cuentos, musica y audio educativo mediante tarjetas RFID.

TrovaLibre es una adaptacion y localizacion del proyecto de software libre TonUINO / TonUINO-TNG para usuarios castellanohablantes.

## Que Es TrovaLibre

TrovaLibre mantiene la funcionalidad original de TonUINO-TNG y adapta su identidad y documentacion para la comunidad de habla hispana.

## Origen Y Creditos

- Proyecto original: TonUINO / TonUINO-TNG
- Se mantienen autores y creditos originales
- Se conserva la atribucion a Thorsten Voss y Boerge1
- Se conserva la licencia GPL y los avisos legales

## Requisitos De Hardware

- Placa compatible con Arduino (Classic, Every, Every4808, ALLinONE, ALLinONE_Plus, variantes ESP32)
- Modulo DFPlayer Mini compatible
- Lector RFID MFRC522
- Botones (3, 5 o matriz 3x3 segun configuracion)
- Altavoz y, opcionalmente, deteccion de auriculares
- Opcional: NeoPixel ring, encoder rotatorio, potenciometro, modulo BT

## Instalacion

### Con PlatformIO

1. Instala PlatformIO.
2. Abre este repositorio.
3. Selecciona un entorno `TrovaLibre_*` en `platformio.ini`.
4. Compila y sube el firmware.

### Con Arduino IDE

1. Abre `TrovaLibre.ino`.
2. Selecciona placa y puerto.
3. Configura opciones de placa y funciones en `src/constants.hpp`.
4. Compila y sube.

## Configuracion

- La configuracion de placa y funciones esta en `src/constants.hpp`.
- Los entornos de compilacion estan en `platformio.ini`.
- Los ajustes persistentes se guardan en EEPROM como en el proyecto original.

## Uso Basico

1. Enciende TrovaLibre.
2. Acerca una tarjeta RFID configurada.
3. Usa los botones para reproducir, pausar, cambiar pista y volumen.
4. Usa el menu admin para ajustes avanzados.

## Como Grabar Tarjetas RFID

1. Acerca una tarjeta nueva.
2. Sigue las locuciones.
3. Selecciona carpeta y modo.
4. Guarda la configuracion de la tarjeta.

## Licencia

Este proyecto se distribuye bajo GNU GPL v3. Consulta `LICENSE`.

TrovaLibre esta basado en el proyecto de software libre TonUINO / TonUINO-TNG.

## Contribuir

Consulta [`.github/CONTRIBUTING.md`](.github/CONTRIBUTING.md) para normas de contribucion.
