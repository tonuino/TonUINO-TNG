#!/usr/bin/env python3
"""
Renombra archivos de audio en subcarpetas numericas y genera un indice (TXT, CSV o ambos).
"""

import argparse
import csv
import re
import unicodedata
from pathlib import Path


def normalize_filename(base_name: str) -> str:
    """Normaliza un nombre: elimina acentos, reemplaza espacios/simbolos por '_'."""
    # Descompone caracteres combinados y elimina marcas diacriticas
    nfkd = unicodedata.normalize('NFKD', base_name)
    no_accents = ''.join(c for c in nfkd if not unicodedata.combining(c))
    # Reemplaza cualquier caracter no alfanumerico por guion bajo
    cleaned = re.sub(r'[^a-zA-Z0-9]', '_', no_accents)
    # Colapsa guiones bajos multiples y limpia extremos
    return re.sub(r'_+', '_', cleaned).strip('_')


def main():
    parser = argparse.ArgumentParser(
        description="Organiza archivos de audio y genera un inventario.",
        epilog=(
            "Ejemplos de uso:\n"
            "  python organiza.py /ruta/a/tarjetaSD\n"
            "  python organiza.py /ruta/a/tarjetaSD --dry-run\n"
            "  python organiza.py /ruta/a/tarjetaSD --no-normalize\n"
            "  python organiza.py /ruta/a/tarjetaSD --index-format csv\n"
            "\n"
            "Notas:\n"
            "  - Solo se procesan subcarpetas con nombres puramente numericos (ej. 01, 02, 15).\n"
            "  - Los archivos se ordenan alfabeticamente antes de renombrar.\n"
            "  - Usa python organiza.py --help para ver mas opciones."
        ),
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        "directorio",
        help="Ruta al directorio raiz donde se encuentran las subcarpetas numericas."
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Simula el proceso sin modificar archivos ni escribir indices en disco."
    )
    parser.add_argument(
        "--no-normalize",
        action="store_true",
        help="Mantiene los nombres de archivo originales (sin quitar acentos ni cambiar espacios)."
    )
    parser.add_argument(
        "--index-format",
        choices=["txt", "csv", "both"],
        default="both",
        help="Formato del indice a generar: 'txt', 'csv' o 'both' (por defecto: both)."
    )
    args = parser.parse_args()

    root = Path(args.directorio)
    if not root.is_dir():
        print(f"Error: '{root}' no es un directorio valido.")
        return

    print(f"Procesando directorio: {root.resolve()}")
    if args.dry_run:
        print("Modo simulacion (--dry-run) activado. No se escribira ningun cambio.\n")

    # Filtrar solo subcarpetas con nombres puramente numericos
    subdirs = sorted(
        [d for d in root.iterdir() if d.is_dir() and d.name.strip().isdigit()],
        key=lambda p: int(p.name)  # Orden numerico correcto (1, 2, ..., 10, 11)
    )

    if not subdirs:
        print("Advertencia: No se encontraron subcarpetas con nombres puramente numericos.")
        return

    index_data = []

    for subdir in subdirs:
        # Filtrar archivos visibles, ignorando archivos ocultos (.DS_Store, Thumbs.db, etc.)
        files = sorted(
            [f for f in subdir.iterdir() if f.is_file() and not f.name.startswith('.')],
            key=lambda p: p.name.lower()
        )
        if not files:
            print(f"  [INFO] Carpeta '{subdir.name}' esta vacia. Se omite.")
            continue

        print(f"\nCarpeta: {subdir.name}")
        file_records = []
        counter = 1

        for file in files:
            ext = file.suffix
            base = file.stem
            norm_base = base if args.no_normalize else normalize_filename(base)
            new_name = f"{counter:03d}_{norm_base}{ext}"
            new_path = subdir / new_name

            # Evitar sobrescribir archivos existentes (comparando rutas resueltas)
            if new_path.exists() and new_path.resolve() != file.resolve():
                print(f"  [SKIP] Colisión de nombres: {file.name} -> {new_name}")
                file_records.append({"original": file.name, "nuevo": new_name, "num": f"{counter:03d}", "renamed": False})
                counter += 1
                continue

            if args.dry_run:
                print(f"  [DRY] {file.name} -> {new_name}")
            else:
                try:
                    file.rename(new_path)
                    print(f"  [OK] {file.name} -> {new_name}")
                except PermissionError:
                    print(f"  [ERROR] Permiso denegado al renombrar: {file.name}")
                    file_records.append({"original": file.name, "nuevo": new_name, "num": f"{counter:03d}", "renamed": False})
                    counter += 1
                    continue
                except Exception as e:
                    print(f"  [ERROR] Fallo al renombrar {file.name}: {e}")
                    file_records.append({"original": file.name, "nuevo": new_name, "num": f"{counter:03d}", "renamed": False})
                    counter += 1
                    continue

            file_records.append({"original": file.name, "nuevo": new_name, "num": f"{counter:03d}", "renamed": True})
            counter += 1

        index_data.append({"folder_name": subdir.name, "files": file_records})

    # Generacion de indices
    if not args.dry_run and index_data:
        print(f"\nIndice(s) generado(s):")
        
        if args.index_format in ("txt", "both"):
            txt_path = root / "index.txt"
            with open(txt_path, "w", encoding="utf-8") as f:
                for i, entry in enumerate(index_data):
                    f.write(f"{entry['folder_name']}\n")
                    for rec in entry["files"]:
                        f.write(f"  {rec['num']} {rec['nuevo']}\n")
                    if i < len(index_data) - 1:
                        f.write("\n")
            print(f"  Texto: {txt_path}")

        if args.index_format in ("csv", "both"):
            csv_path = root / "index.csv"
            with open(csv_path, "w", encoding="utf-8", newline="") as f:
                writer = csv.writer(f)
                writer.writerow(["Carpeta", "Numero", "Archivo Original", "Archivo Nuevo", "Renombrado"])
                for entry in index_data:
                    for rec in entry["files"]:
                        writer.writerow([
                            entry["folder_name"],
                            rec["num"],
                            rec["original"],
                            rec["nuevo"],
                            "Si" if rec["renamed"] else "No"
                        ])
            print(f"  CSV:   {csv_path}")
            
    elif args.dry_run:
        print("\n[DRY-RUN] Los indices no se han escrito en disco.")
    else:
        print("\nNo se genero ningun indice (no se encontraron archivos para procesar).")

    print("Proceso finalizado.")


if __name__ == "__main__":
    main()