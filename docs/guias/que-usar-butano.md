# Qué usar: necesidad → Butano

Consulta esto **antes** de escribir un sistema. Si una fila dice "Butano", no se reescribe. Si algo no está, mira
`engine/butano/butano/include/bn_*.h` y `engine/butano/examples/`; si falta de verdad, tarea `area:lib` para
`gba-engine-dev` (complemento `lib/`, `namespace gx`) y fila nueva aquí. Se completa a medida que aprendemos:
la columna "Notas" recoge lo medido o lo que sorprendió, con fecha.

Butano 21.10.1. Ejemplo = carpeta en `engine/butano/examples/`.

| Necesidad | API | Ejemplo | Notas |
|---|---|---|---|
| Bucle principal | `bn::core::init()`, `bn::core::update()` | todos | una llamada a `update()` por frame |
| Sprites, animación | `bn::sprite_ptr`, `bn::sprite_items::x.create_sprite()`, `bn::create_sprite_animate_action_forever` | `sprites` | |
| Sprites que cambian de tiles (muchos frames) | `bn::sprite_tiles_ptr`, `set_tiles` | `sprites` | evita tener todo en VRAM |
| Fondos de tiles | `bn::regular_bg_ptr`, `bn::regular_bg_items::x.create_bg()` | `regular_bgs` | |
| Mapas grandes o generados | `bn::regular_bg_map_ptr`, `bn::regular_bg_map_cell` | `dynamic_regular_bg`, `world_map` | |
| Colisión con el mapa | celdas del mapa + tabla propia de tipos | `map_collision` | |
| Rotación / escala / modo 7 | `bn::affine_bg_ptr`, `bn::sprite_affine_mat_ptr` | `affine_bgs`, `mode_7` | |
| Cámara | `bn::camera_ptr` (`set_camera` en sprites y fondos) | `cameras` | |
| Texto (menús, HUD, diálogos) | `bn::sprite_text_generator` + fuentes `common_*_font` | `text` | **única vía permitida**; UTF-8 con `bn::utf8_character` |
| Números en texto | `bn::to_string<N>()`, `bn::format` | `text` | sin `printf` |
| Entrada | `bn::keypad::pressed/held/released` | `keypad` | |
| Grabar y reproducir entrada | keypad logger (`BN_CFG_KEYPAD_LOG_ENABLED`), `bn::core::init(keypad_commands)` | `keypad_commands` | pruebas deterministas y fallos del usuario |
| Azar | `bn::random` (rápido), `bn::seed_random` | `random` | semilla fija en pruebas |
| Matemática | `bn::fixed`, `bn::sin`/`bn::cos`, `bn::fixed_point` | — | sin `float` |
| Contenedores sin heap | `bn::vector`, `bn::deque`, `bn::pool`, `bn::unordered_map`, `bn::string` | — | capacidad fija en tiempo de compilación |
| Fundidos de pantalla | `bn::bg_palettes::set_fade`, `bn::sprite_palettes::set_fade`, `*_fade_to_action` | `palettes` | |
| Transparencia, brillo | `bn::blending` | `blending` | |
| Ventanas (recortes, focos) | `bn::rect_window`, `bn::window` | `windows` | |
| Mosaico | `bn::sprites_mosaic`, `bn::bgs_mosaic` | `mosaic` | |
| Efectos por línea (ondas, degradados) | `*_hbe_ptr` (H-Blank effects), `bn::hdma` | `hdma_palette`, `regular_bgs` | |
| Polígonos / 3D simple | sprites + HBE | `polygons`, `texture_polygons` | |
| Fondos bitmap | `bn::palette_bitmap_bg_ptr`, `*_direct_bitmap_bg_ptr` | `palette_bitmap_bgs`, `dp_direct_bitmap_bgs` | caros en CPU |
| Música y efectos | `bn::music_items`, `bn::sound_items` (Maxmod) | `audio`, `audio_player` | skill `gba-audio` |
| Sonido Game Boy | `bn::dmg_music` | `dmg_audio` | casi sin CPU |
| Guardado | `bn::sram::write/read` + firma y versión propias | `sram` | validar firma al leer |
| Temporizadores | `bn::timer` | `timer` | |
| Fecha y hora (cartucho con RTC) | `bn::date`, `bn::time` | `date_time` | |
| Multijugador por cable | `bn::link` | `link` | |
| Vibración | `bn::rumble` | `rumble` | |
| Medir CPU | `bn::core::last_cpu_usage()`, `BN_PROFILER_START/STOP` | `profiler` | skill `gba-performance` |
| Log y asserts | `BN_LOG`, `BN_ASSERT` | `log` | log por mGBA |

## Complemento propio (`lib/`, namespace `gx`)
Vacío por ahora. Candidatos que en SNES acabaron en la biblioteca y quizá falten aquí (comprobar antes en Butano):
máquina de escenas/pantallas, menú con cursor y opciones, tabla de récords con iniciales en SRAM, pantallas de
título/"cómo se juega", entidades con lista compacta de activos y colisión por rejilla.

| Necesidad | API `gx` | Tarea | Notas |
|---|---|---|---|
