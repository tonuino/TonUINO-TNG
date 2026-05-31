#!/usr/bin/env python3
"""
📖 README / GUÍA DE USO
======================
Genera archivos MP3 usando Azure AI Speech con soporte SSML.

PRE-REQUISITOS:
  - Python 3.8+
  - ffmpeg en el PATH del sistema
  - pip install azure-cognitiveservices-speech

CONFIGURACIÓN AZURE:
  - Defnir como variables de entorno (AZURE_SPEECH_KEY y AZURE_SPEECH_REGION)
  - $env:AZURE_SPEECH_KEY="la_key"
  - $env:AZURE_SPEECH_REGION="la_region"
  
FORMATO DEL ARCHIVO .txt:
  ruta/archivo.mp3|Texto a sintetizar
  advert/0301_freeze_freeze1.mp3|¡Detente!<break time='300ms'/>Congelado
  mp3/001_bienvenida.mp3|Bienvenido al sistema
  # Las líneas con # son comentarios y se ignoran

SSML SOPORTADO:
Se pueden usar etiquetas SSML directamente en el texto:
  - <break time="500ms"/> (pausa)
  - <prosody rate="slow">lento</prosody> (velocidad)
  - <prosody pitch="high">agudo</prosody> (tono)
  - <emphasis level="strong">énfasis</emphasis>
  
VOCES

Masculinas
  - es-ES-AlvaroNeural - Neutra, clara. 
  - es-ES-ArnauNeural - Cálida, expresiva. 
  - es-ES-DarioNeural - Formal, corporativa.
  - es-ES-NilNeural - Joven, fresca. 
  - es-ES-TeoNeural - Amigable, cercana. Me gusta
  - es-ES-TristanMultilingualNeural - Parece recién salido del barrio
  - es-ES-SaulNeural. Aterciopelada. Me gusta

Femeninas
  - es-ES-ElviraNeural - Suave, muy natural. Excelente para narración de cuentos.
  - es-ES-AbrilNeural - Juvenil, dinámica. Perfecta para interfaces infantiles.
  - es-ES-EstrellaNeural - Energética, articulada. Ideal para anuncios.
  - es-ES-IreneNeural - Madura, pausada. Muy cómoda para audiolibros largos.
  - es-ES-LaiaNeural - Dulce, educativa. Frecuente en apps de aprendizaje.
  - es-ES-TrianaNeural - Expresiva, con matices. Soporta SSML avanzado.
  - es-ES-XimenaNeural - Clara, estable. Funciona bien con DFPlayer Mini.

EJECUCIÓN:
  python audio_gen.py                  # Generación completa
  python audio_gen.py --test           # Solo primeras 3 líneas
  python audio_gen.py --dry-run        # Simulación (0 tokens)
  python audio_gen.py -v es-ES-ElviraNeural --test #ejemplo
  
"""

import os, sys, argparse, subprocess, time
import azure.cognitiveservices.speech as speechsdk


def main():
    # 1. Parseo de argumentos CLI
    parser = argparse.ArgumentParser(description="Generador de audios TonUINO con Azure Speech")
    parser.add_argument("-i", "--input", default="audio_messages_es.txt", help="Archivo .txt de entrada")
    parser.add_argument("-o", "--output", default="sd_root", help="Carpeta raíz de salida (simula SD)")
    parser.add_argument("-v", "--voice", default="es-ES-TeoNeural", help="Voz Azure (es-ES-ArnauNeural, es-ES-AlvaroNeural...)")
    parser.add_argument("--test", action="store_true", help="Procesa solo las 3 primeras líneas")
    parser.add_argument("--dry-run", action="store_true", help="Simulación (sin consumir tokens)")
    parser.add_argument("--key", help="Clave Azure (opcional, sobrescribe AZURE_SPEECH_KEY)")
    parser.add_argument("--region", help="Región Azure (opcional, sobrescribe AZURE_SPEECH_REGION)")
    args = parser.parse_args()

    # 2. Validación de credenciales (solo si no es dry-run)
    # python audio_gen.py --key "key" --region "codigoregion" --test
    key = args.key or os.getenv("AZURE_SPEECH_KEY")
    region = args.region or os.getenv("AZURE_SPEECH_REGION")
    if not args.dry_run and (not key or not region):
        print("\n Error: Pasa --key y --region, o define AZURE_SPEECH_KEY/AZURE_SPEECH_REGION")
        sys.exit(1)

    # 3. Lectura y filtrado del archivo .txt
    valid_entries = []
    try:
        with open(args.input, "r", encoding="utf-8") as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith("#"): continue
                if "|" not in line: continue
                filepath, txt = line.split("|", 1)
                valid_entries.append((filepath.strip(), txt.strip()))
    except FileNotFoundError:
        print(f"No se encontró: {args.input}")
        sys.exit(1)

    # 4. Configuración de límites y resumen
    total = len(valid_entries)
    limit = 3 if args.test else total
    print(f"\n Entrada: {args.input} | Raíz Salida: /{args.output}/")
    print(f"Voz: {args.voice} | Test: {bool(args.test)} | Dry: {bool(args.dry_run)}")
    print(f"Total a procesar: {limit} líneas\n")

    # 5. Función de barra de progreso en terminal
    def update_progress(current, total, status=""):
        percent = (current / total) * 100 if total > 0 else 100
        filled = int(20 * current / total) if total > 0 else 20
        bar = "█" * filled + "░" * (20 - filled)
        sys.stdout.write(f"\r[{bar}] {percent:5.1f}% | {current:3d}/{total} {status}")
        sys.stdout.flush()

    # 6. Envoltorio SSML automático
    def wrap_ssml(text, voice_name):
        if text.strip().startswith("<speak"):
            return text.strip()
        return f'<speak version="1.0" xmlns="http://www.w3.org/2001/10/synthesis" xml:lang="es-ES"><voice name="{voice_name}">{text}</voice></speak>'

    # 7. Síntesis con Azure + conversión a MP3 DFPlayer
    def synthesize_azure(text, out_path):
        full_path = os.path.join(args.output, out_path)
        dir_name = os.path.dirname(full_path)
        if dir_name: os.makedirs(dir_name, exist_ok=True)

        wav_path = full_path.replace(".mp3", "_temp.wav")
        ssml = wrap_ssml(text, args.voice)
        
        speech_config = speechsdk.SpeechConfig(subscription=key, region=region)
        audio_config = speechsdk.audio.AudioOutputConfig(filename=wav_path)
        synthesizer = speechsdk.SpeechSynthesizer(speech_config=speech_config, audio_config=audio_config)

        result = synthesizer.speak_ssml_async(ssml).get()
        if result.reason == speechsdk.ResultReason.SynthesizingAudioCompleted:
            cmd = ["ffmpeg", "-y", "-i", wav_path, "-ar", "44100", "-ac", "1", "-b:a", "128k", full_path]
            subprocess.run(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=True)
            #os.remove(wav_path)
            return True
        print(f"\nError Azure en {out_path}: {result.reason}")
        return False

    # 8. Modo simulación (0 tokens)
    def dry_run_silent(out_path):
        full_path = os.path.join(args.output, out_path)
        dir_name = os.path.dirname(full_path)
        if dir_name: os.makedirs(dir_name, exist_ok=True)
        cmd = ["ffmpeg", "-y", "-f", "lavfi", "-i", "anullsrc=r=44100:cl=mono", "-t", "1", "-ar", "44100", "-ac", "1", "-b:a", "128k", full_path]
        subprocess.run(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=True)
        return True

    # 9. Bucle principal de procesamiento
    update_progress(0, limit, "Iniciando...")
    processed = 0
    for filepath, text in valid_entries[:limit]:
        update_progress(processed, limit, f"| {filepath}                         ")
        if args.dry_run:
            dry_run_silent(filepath)
        else:
            synthesize_azure(text, filepath)
        processed += 1
        time.sleep(0.15)

    # 10. Finalización
    update_progress(limit, limit, "")
    print(f"\n\nFinalizado. {processed} archivo(s) generados en /{args.output}/")
    #cleanup_temp_files(args.output)    
        
        
    def cleanup_temp_files(output_dir):
        """Elimina todos los archivos _temp.wav residuales"""
        import glob
        temp_files = glob.glob(os.path.join(output_dir, "**", "*_temp.wav"), recursive=True)
        for f in temp_files:
            try:
                os.remove(f)
            except: pass
        if temp_files:
            print(f"Limpiados {len(temp_files)} archivos temporales")
            
if __name__ == "__main__":
    main()