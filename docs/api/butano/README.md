# Referencia de Butano 21.10.1

Generada desde los comentarios Doxygen de `engine/butano/butano/include/bn_*.h` por `python -I .claude/scripts/gen_butano_api.py`. Regenérala al actualizar Butano. No se edita a mano.

Consulta rápida: `python -I .claude/scripts/kb.py symbol bn::sprite_ptr` o `kb.py query "..." --kind butano` (skill `gba-kb`).

- `index.jsonl`: 7729 símbolos (function 6811, class 776, macro 84, namespace 36, enum 14, variable 5, struct 3).
- 67 módulos Markdown (agrupados por `@ingroup` del fichero).
- `manual/`: páginas del manual de Butano (getting started, import de recursos, FAQ, ejemplos, changelog recortado).

## Módulos

| Módulo | Título | Símbolos |
|---|---|---|
| [action](action.md) | Actions | 2415 |
| [affine_bg](affine_bg.md) | Affine backgrounds | 221 |
| [affine_mat](affine_mat.md) | Affine transformation matrices | 125 |
| [allocator](allocator.md) | Allocator | 20 |
| [any](any.md) | Any | 40 |
| [array](array.md) | Array | 38 |
| [assert](assert.md) | Asserts | 10 |
| [audio](audio.md) | Audio | 30 |
| [backdrop](backdrop.md) | Backdrop | 7 |
| [bg](bg.md) | Backgrounds | 8 |
| [bg_map](bg_map.md) | Background maps | 210 |
| [bitmap_bg](bitmap_bg.md) | Bitmap backgrounds | 20 |
| [bitset](bitset.md) | Bitset | 69 |
| [blending](blending.md) | Blending | 39 |
| [camera](camera.md) | Cameras | 27 |
| [color](color.md) | Colors | 34 |
| [core](core.md) | Core | 36 |
| [date_time](date_time.md) | Date and time | 34 |
| [deque](deque.md) | Deque | 119 |
| [direct_bitmap_bg](direct_bitmap_bg.md) | Direct bitmap backgrounds | 16 |
| [display](display.md) | Display | 4 |
| [dmg_music](dmg_music.md) | DMG music | 31 |
| [dp_direct_bitmap_bg](dp_direct_bitmap_bg.md) | Dual page direct bitmap backgrounds | 206 |
| [forward_list](forward_list.md) | Forward list | 73 |
| [functional](functional.md) | Functional | 9 |
| [game_pak](game_pak.md) | Game Pak | 11 |
| [green_swap](green_swap.md) | Green swap | 4 |
| [hblank_effect](hblank_effect.md) | H-Blank effects | 698 |
| [hdma](hdma.md) | H-Blank direct memory access | 10 |
| [inside_window](inside_window.md) | Inside windows | 6 |
| [intrusive_forward_list](intrusive_forward_list.md) | Intrusive forward list | 57 |
| [intrusive_list](intrusive_list.md) | Intrusive list | 68 |
| [keypad](keypad.md) | Keypad | 39 |
| [link](link.md) | Link communication | 24 |
| [list](list.md) | List | 86 |
| [log](log.md) | Logging | 10 |
| [math](math.md) | Math | 392 |
| [memory](memory.md) | Memory | 66 |
| [mosaic](mosaic.md) | Mosaic | 34 |
| [music](music.md) | Music | 29 |
| [optional](optional.md) | Optional | 57 |
| [other](other.md) | Other | 14 |
| [palette](palette.md) | Color palettes | 126 |
| [palette_bitmap_bg](palette_bitmap_bg.md) | Palette bitmap backgrounds | 245 |
| [pool](pool.md) | Pool | 21 |
| [profiler](profiler.md) | Profiler | 6 |
| [random](random.md) | Random | 32 |
| [rect_window](rect_window.md) | Rectangle windows | 30 |
| [regular_bg](regular_bg.md) | Regular backgrounds | 161 |
| [rumble](rumble.md) | Rumble | 3 |
| [sound](sound.md) | Sound effects | 26 |
| [sp_direct_bitmap_bg](sp_direct_bitmap_bg.md) | Single page direct bitmap backgrounds | 199 |
| [span](span.md) | Span | 44 |
| [sprite](sprite.md) | Sprites | 279 |
| [sram](sram.md) | Game Pak SRAM | 19 |
| [std](std.md) | Standard library | 7 |
| [string](string.md) | Strings | 223 |
| [template_action](template_action.md) | Action templates | 116 |
| [text](text.md) | Text | 71 |
| [tile](tile.md) | Tiles | 103 |
| [timer](timer.md) | Timers | 11 |
| [tool](tool.md) | Tools | 278 |
| [unique_ptr](unique_ptr.md) | Unique pointer | 5 |
| [unordered_map](unordered_map.md) | Unordered map | 101 |
| [unordered_set](unordered_set.md) | Unordered set | 80 |
| [vector](vector.md) | Vector | 71 |
| [window](window.md) | Windows | 26 |

## Manual

| Página | Origen | Bytes |
|---|---|---|
| [manual/getting_started.md](manual/getting_started.md) | `bn_documentation_aa_getting_started.h` | 1556 |
| [manual/getting_started_dka.md](manual/getting_started_dka.md) | `bn_documentation_ab_getting_started_dka.h` | 4013 |
| [manual/getting_started_wt.md](manual/getting_started_wt.md) | `bn_documentation_ac_getting_started_wt.h` | 3512 |
| [manual/examples.md](manual/examples.md) | `bn_documentation_b_examples.h` | 4765 |
| [manual/import.md](manual/import.md) | `bn_documentation_c_import.h` | 32826 |
| [manual/qt_creator.md](manual/qt_creator.md) | `bn_documentation_d_qt_creator.h` | 2567 |
| [manual/nocashgba_exception.md](manual/nocashgba_exception.md) | `bn_documentation_e_nocashgba_exception.h` | 1597 |
| [manual/faq.md](manual/faq.md) | `bn_documentation_f_faq.h` | 29556 |
| [manual/changelog.md](manual/changelog.md) | `bn_documentation_g_changelog.h` | 18515 |
| [manual/groups.md](manual/groups.md) | `bn_documentation_groups.h` | 13437 |
| [manual/main_page.md](manual/main_page.md) | `bn_documentation_main_page.h` | 1499 |
