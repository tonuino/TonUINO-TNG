# Scripts de utilidad / Utility scripts

Scripts en Python 3 para generar y procesar los archivos de audio que TrovaLibre reproduce desde la tarjeta SD.

## Scripts

### Generación de audio (TTS)

- **`text_to_speech.py`** — Librería compartida. Convierte texto a MP3 usando uno de varios motores TTS (Google Cloud, Amazon Polly, Coqui TTS, o `say` de macOS). No se ejecuta directamente; lo importan `create_audio_messages.py` y `add_lead_in_messages.py`.
- **`create_audio_messages.py`** — Genera los mensajes de voz del sistema (números `0001.mp3`–`0255.mp3` y avisos) a partir de un fichero `audio_messages_<lang>.txt`. Salida pensada para copiarse a la SD en `mp3/`.
- **`add_lead_in_messages.py`** — Toma una carpeta de MP3 de música y antepone a cada pista un mensaje hablado con su título (útil para que el TrovaLibre anuncie la canción antes de reproducirla).
- **`generate_audio_azure.py`** — Alternativa a `create_audio_messages.py` que usa **Azure AI Speech** (voces neuronales `es-ES-*`) con soporte completo de **SSML** (`<break>`, `<prosody>`, `<emphasis>`…). Lee un `.txt` con líneas `ruta/archivo.mp3|Texto a sintetizar` y produce los MP3 a 44.1 kHz mono 128 kbps (compatible DFPlayer Mini).
  - Requiere `pip install azure-cognitiveservices-speech` y `ffmpeg` en el PATH.
  - Credenciales por entorno: `AZURE_SPEECH_KEY`, `AZURE_SPEECH_REGION` (o flags `--key` / `--region`).
  - Voces recomendadas: `es-ES-TeoNeural`, `es-ES-SaulNeural`, `es-ES-ElviraNeural`, `es-ES-XimenaNeural` (ver cabecera del script para la lista completa con notas de uso).
  - Modos: `--test` (solo 3 primeras líneas), `--dry-run` (genera silencios, no consume tokens Azure), `-v <voz>` para cambiar de voz.

### Organización de la SD

- **`organize_sd_card.py`** — Renombra los archivos de audio dentro de subcarpetas numéricas (`01/`, `02/`, …, `15/`) al formato `NNN_nombre.ext` que espera el DFPlayer Mini, y genera un índice (`index.txt` y/o `index.csv`) con la correspondencia original → nuevo nombre.
  - Normaliza nombres por defecto: quita acentos y reemplaza espacios/símbolos por `_` (desactivable con `--no-normalize`).
  - Modo `--dry-run` para simular sin tocar archivos.
  - `--index-format {txt,csv,both}` controla qué índice generar.
  - Solo procesa subcarpetas con nombre puramente numérico; ignora archivos ocultos (`.DS_Store`, `Thumbs.db`).

## Requisitos

Python 3.7+ (solo librería estándar). Las dependencias son **binarios externos** según el motor TTS que elijas:

| Motor       | Necesitas                                      | Plataforma           |
|-------------|-----------------------------------------------|----------------------|
| `google`    | API key de Google Cloud Text-to-Speech        | cualquiera           |
| `amazon`    | AWS CLI configurado (`aws configure`) + Polly | cualquiera           |
| `coqui`     | [`coqui-tts`](https://github.com/coqui-ai/TTS) (`pip install TTS`) + `ffmpeg` | cualquiera |
| `say`       | macOS (`say` incluido) + `ffmpeg`             | solo macOS           |
| `azure`     | `pip install azure-cognitiveservices-speech` + `ffmpeg` + clave y región Azure | cualquiera |

`ffmpeg` también es necesario para `add_lead_in_messages.py` (mezcla la voz con la pista).

Instalar `ffmpeg`:
- **Windows**: `winget install ffmpeg` o `choco install ffmpeg`
- **macOS**: `brew install ffmpeg`
- **Linux**: `apt install ffmpeg` / equivalente

## Uso rápido

Generar mensajes del sistema en español usando `say` (macOS):

```sh
python3 create_audio_messages.py --lang es --tts say -o ../sd-card-files/sd-card-spanish
```

Añadir lead-in a una carpeta de música:

```sh
python3 add_lead_in_messages.py -i /ruta/musica -o /ruta/salida --lang es --tts google --add-numbering
```

Generar mensajes del sistema con Azure (mejor calidad, voces neuronales):

```sh
$env:AZURE_SPEECH_KEY="<tu_clave>"
$env:AZURE_SPEECH_REGION="<tu_region>"
python generate_audio_azure.py -i audio_messages_es.txt -o ../sd-card-files/sd-card-spanish -v es-ES-TeoNeural
```

Organizar una SD recién copiada (renombrar a `NNN_*` y crear índice):

```sh
python organize_sd_card.py D:\ --index-format both
```

Cada script acepta `-h` / `--help` para ver todas las opciones.
