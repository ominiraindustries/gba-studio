---
name: gba-build
description: Compilar un juego o laboratorio GBA con Butano (make + devkitARM) desde Git Bash, crear un proyecto desde la plantilla y montar compilar.bat. Úsala antes de compilar, al crear un armazón o ante un error de build.
---

# Compilar con Butano

- Siempre desde **Git Bash**: `source tools/env.sh` (fija `DEVKITPRO`, `DEVKITARM`, `BUTANO` y el `PATH`).
- Compilar: `make -C projects/<juego> -j8` → `projects/<juego>/<juego>.gba`. Limpiar: `make -C ... clean`
  (sólo si cambió la configuración o los recursos no se regeneran).
- El `Makefile` del proyecto (copiado de `templates/`) define `LIBBUTANO`, `SOURCES`, `INCLUDES`, `GRAPHICS`,
  `AUDIO`, `AUDIOBACKEND`, `DMGAUDIO`, `ROMTITLE` (12 caracteres), `ROMCODE` (4) y `USERFLAGS`. Ruta a Butano
  relativa o vía `$(BUTANO)`; no la cambies por proyecto sin motivo.
- Resume la salida (`2>&1 | tail -20` o `grep -E "error|warning"`); nunca vuelques el log entero.

## Proyecto nuevo (armazón)
1. Crea el proyecto con `python -I tools/scripts/new_game.py NNN_nombre --title "..." --romtitle X --romcode 2XXE`
   (copia `templates/butano_game` y sustituye sus marcadores; nunca copies a mano). Ver `templates/README.md`.
2. Desde el primer día: `art/` con `LÉEME.md`, `DISENO.md` (lo escribe `gba-designer`), `compilar.bat` y
   `compilar_y_jugar.bat` (doble clic para el usuario; el segundo abre el emulador con ventana: es **para el usuario**,
   los agentes no lo ejecutan), texto con `bn::sprite_text_generator` y una fuente de `engine/butano/common/`.
3. Tabla "Butano check" en el README del proyecto: necesidad → API de Butano → por qué basta o no.

## Errores frecuentes
- `bn::regular_bg_items::x` no existe → el `.bmp` no tiene `.json` hermano o el nombre no es `[a-z0-9_]`.
- Paleta/colores mal → el BMP no está indexado o el color 0 no es el transparente (skill `gba-gfx`).
- `region IWRAM overflowed` → demasiado `BN_CODE_IWRAM`/`.bn_iwram.cpp`: sólo lo caliente (skill
  `gba-performance`).
- Warnings nuevos no se entregan.
- Detalle de opciones: `engine/butano/butano/include/documentation/` (FAQ y "getting started") y
  `engine/butano/template/Makefile`.
