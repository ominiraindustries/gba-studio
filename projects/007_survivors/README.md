# 007_survivors

Juego GBA hecho con [Butano](https://github.com/GValiente/butano) (motor C++), creado con
`python -I tools/scripts/new_game.py 007_survivors`.

## Compilar y jugar

- Doble clic en `compilar.bat` (solo compila) o `compilar_y_jugar.bat` (compila y abre mGBA).
- Desde Git Bash, en la raíz del repo:

      source tools/env.sh
      make -C projects/007_survivors -j8

## Controles (bloque 1)

Cruceta: mover (el mago mira hacia donde dispara). A: Hex Nova cuando el medidor de abajo a la izquierda está lleno.
B: Blink (salto corto, 3 s de recarga). START: pausa (armas y pasivas, Resume / Quit). SELECT: menú de efectos.

## Depuración (menú SELECT, `hb::debug_menu` en `include/hb_config.h`)

Filas `DBG`: **God mode** (sin daño), **Max power** (todo a Nv5), **Skip to 4:30** (salta el reloj; el jefe sale a
5:00), **Stress 56** (mantiene 56 enemigos para medir). Poner `debug_menu = false` para la versión publicada.

## Probar sin ventana (Mesen 2 headless)

Globales `test_*` de `src/hb_game.cpp` (modo, reloj, nivel, enemigos, CPU del peor frame en 1/10000 de frame, Nova…).
Ejemplo: arranque + 20 s moviéndose en círculo:

    python -I tools/scripts/gbarun.py projects/007_survivors/007_survivors.gba --frames 1300 --shot 30,1200         --input "40:start;100-400:right;400-700:down;700-1000:left" --check "test_time_s.32>=15" --nonblank         --out <scratchpad>/run_007

Para el jefe: SELECT (frame 60), 11 × abajo, A (God), abajo, abajo, A (Skip to 4:30), SELECT; luego moverse y pulsar A
de vez en cuando (cartas). Medidas de G-45: 56 enemigos con armas Nv1 = 82 % de frame en el peor caso; con Max power
el peor frame llega al 118 % (54 enemigos) y la Nova al 163 % (un frame): pendiente de perfilar.

## Provisionales

- Sonidos `audio/tmp_*.wav` (`tests/gen_tmp_sfx.py`) y gráficos `graphics/tmp_fx` (chispa de estela), `tmp_wave`
  (onda del rey) y `tmp_bossbar` (`tests/gen_tmp_gfx.py`). Todo el arte se elige en `include/hb_art.h`.
- Medidor de Nova = 2 piezas de `hud_xp` + "A" dorada; estela de Blink = copias translúcidas del mago (faltan
  `fx_nova` / `fx_blink`).
- Título en pantalla: `hb::game_title` (`include/hb_config.h`) y el logo pintado (`art/logo.aseprite`); ROM: `ROMTITLE`.
- Ranuras de armas (`hud_slots`) sólo en la pausa, para no gastar sprites en combate.

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
| Mapa infinito por trozos | `bn::regular_bg_map_item` + `reload_cells_ref` (ejemplo `dynamic_regular_bg`) | sí |
| Cámara | `bn::camera_ptr` | sí |
| Contenedores sin heap | `bn::vector`, `bn::optional` (partida en heap una vez por intento) | sí |
| Destellos, transparencia, mosaico | `set_palette`, `bg_palettes::set_fade`, `bn::blending`, `bn::sprites_mosaic` | sí |
| Medir CPU | `bn::core::last_cpu_usage()` → `test_cpu_max` | sí |
| Texto | `bn::sprite_text_generator` + `common::variable_8x8_sprite_font` | sí |
| Efecto de sonido | `bn::sound_items` (Maxmod) | sí |
| Entrada | `bn::keypad` | sí |
