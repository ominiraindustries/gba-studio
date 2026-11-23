# Herramientas y toolchain GBA

Todo se instala dentro del repo (sin GUI, sin administrador) y queda fuera de git. Versiones fijadas
(instalado el 2026-11-23):

| Componente | Versión | Dónde | Tamaño |
|---|---|---|---|
| devkitARM (gcc, binutils, newlib) | r68: gcc 16.1.0, binutils 2.46.0, newlib 4.6.0.20260123 | `toolchain/devkitPro/devkitARM` | 375 MB (con tools) |
| devkitPro tools | gba-tools 1.2.0 (gbafix), general-tools 1.4.4 (bin2s), grit 0.10.0, mmutil 1.10.1 | `toolchain/devkitPro/tools/bin` | (incluido) |
| GNU make (MSYS2) | 4.4.1-3 | `toolchain/msys-make/bin/make.exe` | 0.2 MB |
| Butano | 21.10.1 (commit `67afc034`), clon superficial y disperso | `engine/butano` | 33 MB |
| mGBA (para jugar) | 0.10.5 win64 portable | `tools/mgba` | 51 MB |
| Mesen 2 (pruebas sin ventana) | 2.1.1 del repo SNES, **no se copia** | `../SNES/tools/mesen/Mesen.exe` o `$MESEN` | — |
| BIOS GBA libre (para Mesen) | Cult-of-GBA BIOS, commit `087f6e56` (MIT) | `tools/bios/` (versionado, 16 KB) | — |

Python 3.14 (miniforge) y Git for Windows ya estaban en la máquina.

## Reinstalar

```bash
python -I tools/scripts/install_toolchain.py           # instala lo que falte (verifica SHA-256 de cada descarga)
python -I tools/scripts/install_toolchain.py --check   # sólo informa
python -I tools/scripts/install_toolchain.py --only devkitpro,make,butano,mgba
```

El script descarga los paquetes de devkitPro directamente de `pkg.devkitpro.org` (los mismos que instalaría
`dkp-pacman`) y los extrae; no hace falta el instalador gráfico de devkitPro ni pacman. Para actualizar una versión:
cambia la URL y el SHA-256 en `install_toolchain.py` (el SHA está en el `desc` de `dkp-libs.db` /
`windows/x86_64/dkp-windows.db`), borra la carpeta del componente y vuelve a ejecutar.

Para mGBA el script necesita `7z`/`7zr` en el PATH; si no lo hay descarga `7zr.exe` de 7-zip.org a una carpeta
temporal.

## Compilar

Desde Git Bash:

```bash
source tools/env.sh          # DEVKITPRO, DEVKITARM, BUTANO y PATH (make de MSYS2 + devkitARM)
make -C projects/000_hello -j8
```

O doble clic en `projects/<juego>/compilar.bat` (`compilar_y_jugar.bat` además abre mGBA).
mGBA bloquea el `.gba` mientras lo tiene abierto: ciérralo antes de recompilar (si no, falla el paso
`gbafix`/`objcopy` del `.gba`).

Por qué `toolchain/msys-make`: las reglas de devkitARM esperan un `make` de MSYS (devkitPro trae el suyo). Con un
`make` nativo de Windows la regla `bin2o` pasa a gcc una ruta `/tmp/...` que gcc no entiende y falla el
`bn_audio_soundbank.bin.o`. El `make.exe` de MSYS2 usa el `msys-2.0.dll` de Git for Windows.

## Probar sin ventana

`tools/scripts/gbarun.py` (modelo: `snesrun.py` del repo SNES) ejecuta la ROM en Mesen 2 `--testrunner` con Lua:
capturas, entrada programada, comprobaciones sobre símbolos del `.elf`, `--nonblank`, goldens. Ver `--help`.

- Mesen 2.1 emula GBA pero **necesita una BIOS**: sin ella la CPU no arranca y todas las capturas son grises.
  `gbarun.py` copia la BIOS libre `tools/bios/cult_of_gba_bios.bin` a `Documents/Mesen2/Firmware/gba_bios.bin` si
  no hay ninguna (ya hecho en esta máquina). Si pones allí una BIOS oficial volcada de tu consola, se usará esa.
- Fuerza `gbaAdjustColors=false` y `blendFrames=false`: las capturas son el RGB555 puro, iguales con cualquier
  configuración de la ventana de Mesen (`--doNotSaveSettings`: no toca la configuración del usuario).
- Comparte cerrojo con `snesrun.py`: una sola ejecución de Mesen a la vez en la máquina.

## Scripts

| Script | Uso |
|---|---|
| `tools/scripts/install_toolchain.py` | instalar / comprobar el toolchain fijado |
| `tools/scripts/new_game.py NOMBRE` | crear `projects/NOMBRE` desde `templates/butano_game` |
| `tools/scripts/gbarun.py ROM ...` | prueba sin ventana en Mesen 2 |
| `tools/env.sh` | entorno de compilación para Git Bash |
