# Plantillas

## `butano_game/`

Armazón de un juego Butano que ya compila y funciona: un sprite que se mueve con la cruceta sobre un fondo,
texto con `bn::sprite_text_generator` (fuente `common::variable_8x8_sprite_font` de `engine/butano/common/`)
y un efecto de sonido con A. Expone `test_player_x`, `test_player_y` y `test_sounds_played` para
`tools/scripts/gbarun.py --check`.

Crear un proyecto (no copies a mano: el script sustituye los marcadores `@NAME@`, `@TITLE@`, `@ROMTITLE@`,
`@ROMCODE@`):

    python -I tools/scripts/new_game.py 001_mi_juego --title "Mi juego" --romtitle MIJUEGO --romcode 2MJE

Contenido:

| Ruta | Qué es |
|---|---|
| `Makefile` | el de `engine/butano/template/Makefile`, con `LIBBUTANO := ../../engine/butano/butano` y `common/` de Butano |
| `src/main.cpp`, `include/` | código C++ |
| `graphics/*.bmp` + `*.json` | BMP indexados (16 colores, índice 0 transparente) que lee el build |
| `art/LÉEME.md` | reglas del arte y cómo exportar `.aseprite` → `graphics/*.bmp` |
| `audio/blip.wav` | efecto de ejemplo (Maxmod: `.wav` = efectos, `.mod/.xm/.s3m/.it` = música) |
| `dmg_audio/` | música del chip de Game Boy (vacío) |
| `compilar.bat`, `compilar_y_jugar.bat` | doble clic para el usuario (el segundo abre mGBA con ventana) |
| `tests/golden/` | capturas de referencia de `gbarun.py` (las crea `--update-golden`) |

Los assets de ejemplo se generaron con un script de Python (paleta de 16 colores); sustitúyelos libremente.
Ejemplo vivo: `projects/000_hello` (creado con esta plantilla, con golden).
