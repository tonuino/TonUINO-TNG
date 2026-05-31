# Soporte para Arduino IDE

Este directorio contiene archivos auxiliares **solo necesarios si compilas TrovaLibre con el IDE clásico de Arduino** en lugar de PlatformIO. Si usas PlatformIO (recomendado), puedes ignorar todo lo que hay aquí.

## `platform.local.txt`

Activa el estándar C++17 y los *concepts* de GCC, que TrovaLibre necesita para compilar:

```
compiler.cpp.extra_flags=-std=gnu++17 -fconcepts
```

PlatformIO ya aplica estos flags vía `build_flags` en `platformio.ini`, así que este archivo **solo hace falta para Arduino IDE**.

### Cómo instalarlo

Arduino IDE no lee `platform.local.txt` desde la carpeta del sketch — hay que copiarlo a la carpeta del *hardware platform* correspondiente:

- **AVR (Nano clásico, Uno, etc.)**
  - Windows: `%LOCALAPPDATA%\Arduino15\packages\arduino\hardware\avr\<versión>\`
  - macOS:   `~/Library/Arduino15/packages/arduino/hardware/avr/<versión>/`
  - Linux:   `~/.arduino15/packages/arduino/hardware/avr/<versión>/`
- **megaAVR (Nano Every, ATmega4809)**
  - Sustituye `avr` por `megaavr` en las rutas anteriores.
- **ESP32**
  - Sustituye `arduino/hardware/avr` por `esp32/hardware/esp32` en las rutas anteriores.

Reinicia el IDE después de copiarlo.
