# 006_delve

Juego GBA hecho con [Butano](https://github.com/GValiente/butano) (motor C++), creado con
`python -I tools/scripts/new_game.py 006_delve`.

## Compilar y jugar

- Doble clic en `compilar.bat` (solo compila) o `compilar_y_jugar.bat` (compila y abre mGBA).
- Desde Git Bash, en la raíz del repo:

      source tools/env.sh
      make -C projects/006_delve -j8

## Probar sin ventana (Mesen 2 headless)

    python -I tools/scripts/gbarun.py projects/006_delve/006_delve.gba --frames 100 --shot 100 \
        --input "31-90:right;95:a" --check-at "30:test_player_x.32==0" \
        --check "test_player_x.32==60,test_sounds_played.32==1" --nonblank \
        --golden projects/006_delve/tests/golden --out <scratchpad>/run_006_delve

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

## Vista y generador (G-25)

- **Piezas de la vista**: tras exportar el arte (`art/export/tex_*.bmp`, ver `art/LÉEME.md`), regenera las piezas y
  recompila:

      python -I projects/006_delve/tools/build_view.py
      make -C projects/006_delve -j8

  Lee la geometría de `include/dl_view_geometry.h` y las texturas de `art/export/` (sólo lectura; si falta la carpeta
  usa `tools/test_textures/`), y escribe `include/dl_view_data.h` + `src/dl_view_data.cpp` (generados, no editar).
  Los colores 192–255 salen de la paleta de `graphics/ui_frame.bmp` (240–247: colores del minimapa).
- **Generador**: `src/dl_gen.cpp` es el port bit a bit de la v2 (`Desktop/SNES/tools/scripts/dungeon_model.py`);
  `src/dl_gen_hooks.cpp` añade los ganchos de juego (DISENO §6b). Comprobación (compila para PC con g++):

      python -I projects/006_delve/tools/gen_metrics.py --fidelity 50 3 --seeds 1000 --levels 3 --ascii 3

  `--fidelity`: CRC idéntico a la v2 de Python; `--seeds`: métricas, reglas duras y solucionador (0 sin solución).
- **Controles de prueba**: B coloca un monstruo de prueba 3 casillas delante (provisional, B será "objeto rápido").

## Vista (G-25, bloque 2)

- `python -I projects/006_delve/tools/build_view.py` regenera `include/dl_view_data.h` y `src/dl_view_data.cpp`
  (grosor de pared, jambas, variantes de muro, fondos por paridad, cerradura provisional en `tools/test_textures/`).
- Rendimiento medido con gbarun (4389 ticks = 1 f): vista máx. 0,78 f, minimapa máx. 0,22 f (incremental), redibujado
  total máx. 0,82 f; zoom de paso 0,76 f por imagen compuesta (se compone en fotogramas alternos); generación de planta
  0,37 s (pantalla `DESCENDING...` con el marco mientras tanto).
- Efectos con interruptor: `include/dl_fx.h` (llama, estandarte, halo, oscurecido de adornos, zoom de paso, giro,
  balanceo de cabeza).
