# 003_poket_gto

Juego GBA hecho con [Butano](https://github.com/GValiente/butano) (motor C++), creado con
`python -I tools/scripts/new_game.py 003_poket_gto`.

## Compilar y jugar

- Doble clic en `compilar.bat` (solo compila) o `compilar_y_jugar.bat` (compila y abre mGBA).
- Desde Git Bash, en la raíz del repo:

      source tools/env.sh
      make -C projects/003_poket_gto -j8

## Probar sin ventana (Mesen 2 headless)

    python -I tools/scripts/gbarun.py projects/003_poket_gto/003_poket_gto.gba --frames 100 --shot 100 \
        --input "31-90:right;95:a" --check-at "30:test_player_x.32==0" \
        --check "test_player_x.32==60,test_sounds_played.32==1" --nonblank \
        --golden projects/003_poket_gto/tests/golden --out <scratchpad>/run_003_poket_gto

`--update-golden` regenera las capturas de referencia (revísalas antes de commitear).

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
| Sprite movible | `bn::sprite_ptr`, `bn::sprite_items` | sí |
| Fondo | `bn::regular_bg_ptr`, `bn::regular_bg_items` | sí |
| Texto | `bn::sprite_text_generator` + `common::variable_8x8_sprite_font` | sí |
| Efecto de sonido | `bn::sound_items` (Maxmod) | sí |
| Entrada | `bn::keypad` | sí |
