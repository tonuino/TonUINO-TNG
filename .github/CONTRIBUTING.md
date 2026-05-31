# Contributing To TrovaLibre

## English

Thanks for contributing to TrovaLibre.

### Principles

- Keep functional behavior compatible unless the change is documented.
- Preserve EEPROM/RFID data compatibility when touching modes and settings.
- Keep legal attribution to TonUINO / TonUINO-TNG.
- Do not remove third-party license notices.

### Development Guidelines

- Use clear technical English for identifiers.
- Keep comments concise; for complex logic, prefer EN + ES comments.
- Avoid blind global replacements; review impact file by file.
- Do not rename external library APIs or required framework symbols.

### Validation

- Build at least one AVR target and one ESP32 target.
- Run CMake unit tests in `test/` when available.
- Search for unintended leftover legacy identifiers before submitting.

## Espanol

Gracias por contribuir a TrovaLibre.

### Principios

- Mantener compatibilidad funcional salvo cambios documentados.
- Preservar compatibilidad de EEPROM/RFID al tocar modos y ajustes.
- Mantener atribucion legal a TonUINO / TonUINO-TNG.
- No eliminar avisos de licencia de terceros.

### Guia de desarrollo

- Usar ingles tecnico claro para identificadores internos.
- Mantener comentarios concisos; en logica compleja, usar EN + ES.
- Evitar reemplazos masivos ciegos; revisar impacto por archivo.
- No renombrar APIs externas ni simbolos requeridos por frameworks.

### Validacion

- Compilar al menos un objetivo AVR y uno ESP32.
- Ejecutar los tests CMake de `test/` cuando esten disponibles.
- Buscar restos de nomenclatura legacy no deseados antes de enviar cambios.
