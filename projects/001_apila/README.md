# 001_apila

Juego GBA hecho con [Butano](https://github.com/GValiente/butano) (motor C++), creado con
`python -I tools/scripts/new_game.py 001_apila`.

## Compilar y jugar

- Doble clic en `compilar.bat` (solo compila) o `compilar_y_jugar.bat` (compila y abre mGBA).
- Desde Git Bash, en la raíz del repo:

      source tools/env.sh
      make -C projects/001_apila -j8

## Probar sin ventana (Mesen 2 headless)

Autoprueba de reglas al arrancar (bolsa de 7, kick SRS, cuádruple + todo limpio, T-giro doble):

    python -I tools/scripts/gbarun.py projects/001_apila/001_apila.gba --frames 60         --check "test_selftest.32==15" --out <scratchpad>/run_apila

Partida corta (START en el frame 40, soltar piezas hasta perder, START para otra):
`--input "40:start;60:up;68:up;..."` y `--check "test_state.32==4"` (0 título, 1 jugando, 2 borrando, 3 pausa, 4 fin).

## Carpetas

| Carpeta | Contenido |
|---|---|
| `src/`, `include/` | código C++ (Butano) |
| `graphics/` | `.bmp` indexados + `.json` que lee el build |
| `art/` | fuentes `.aseprite` editables (ver `art/LÉEME.md`) |
| `audio/` | música (`.mod/.xm/.s3m/.it`) y efectos (`.wav`) para Maxmod |
| `dmg_audio/` | música del chip de sonido de Game Boy (opcional) |
| `tests/golden/` | capturas de referencia de `gbarun.py` |

## Butano check

| Necesidad | API de Butano revisada | ¿Basta? |
|---|---|---|
| Tablero, reserva y cola | `bn::regular_bg_item` con mapa propio + `bn::regular_bg_map_ptr::reload_cells_ref` (ejemplo `dynamic_regular_bg`) | sí |
| Colores por pieza | `bn::regular_bg_map_cell_info::set_palette_id`, 8 paletas de 16 (`bg_palette`) | sí |
| Texto | `bn::sprite_text_generator` + `common::variable_8x8/8x16_sprite_font` | sí |
| Azar con semilla | `bn::seed_random` (`set_seed`, `get_unbiased_int`) | sí |
| Entrada y auto-repetición | `bn::keypad` (*_pressed/*_held); DAS propio, 20 líneas | sí |
| Efecto de sonido | `bn::sound_items` (Maxmod) | sí |
