---
name: gba-gfx
description: Gráficos GBA con Butano desde Aseprite - formatos BMP+JSON de graphics/, tipos (sprite, regular_bg, affine_bg, bg_palette...), tamaños, paletas 4bpp/8bpp, exportación desde .aseprite y límites de VRAM/OAM. Úsala al crear, exportar o depurar arte.
---

# Gráficos: Aseprite → Butano

## Flujo (arte editable por el usuario)
- Fuente: `projects/<juego>/art/<nombre>.aseprite`, **indexado**, paleta del juego, índice 0 = transparente, una
  etiqueta por animación con sus tiempos. `art/LÉEME.md` en castellano (colores, índices, pies, qué no tocar).
- Exportación a `projects/<juego>/graphics/<nombre>.bmp` (+ `<nombre>.json`). Aseprite está en
  `C:\Program Files (x86)\Steam\steamapps\common\Aseprite\Aseprite.exe` y tiene modo por lotes
  (`-b <in> --sheet <out>.bmp --sheet-type vertical`; las etiquetas y duraciones, con `--data`). Mientras no exista el
  conversor del estudio (tarea G-1, `ase2bn.py`), exporta así y revisa el BMP. Los generadores sólo
  siembran el `.aseprite`; nunca lo sobrescriben.
- Lo generado (`graphics/*.bmp`) se reconstruye desde `art/`; si el usuario edita el `.aseprite`, manda su versión.

## Formato de Butano (`graphics/`)
- BMP **indexado sin compresión** (4 u 8 bpp) + JSON hermano. Nombre `[a-z0-9_]`: el código ve
  `bn::sprite_items::<nombre>`, `bn::regular_bg_items::<nombre>`...
- Tipos: `sprite` (`"height"`: alto de un frame si el BMP es una tira vertical), `sprite_tiles`, `sprite_palette`,
  `regular_bg`, `regular_bg_tiles`, `affine_bg`, `affine_bg_tiles`, `bg_palette`, `palette_bitmap`, `direct_bitmap`.
- Opciones útiles: `bpp_mode` (`bpp_4`/`bpp_8`), `colors_count`, `compression` (`none`, `lz77`, `run_length`,
  `huffman`, `auto`), `palette_item` (compartir paleta), `repeated_tiles_reduction`, `flipped_tiles_reduction`, `big`.
  Referencia: `engine/butano/butano/include/documentation/` (sección de import) y `engine/butano/examples/sprites`,
  `regular_bgs`.
- El color 0 de la paleta es el transparente (y el de fondo en BG); en 4bpp cada sprite/tile usa 16 colores.

## Límites GBA que importan
- Pantalla 240×160. 128 sprites (OAM), formas 8×8 a 64×64 (cuadradas, 2:1, 1:2, 4:1 y 1:4: 8×16, 16×32, 32×64,
  8×32, 16×8...). Escala/rotación: 32 matrices afines.
- VRAM 96 KB: 64 KB para fondos, 32 KB para tiles de sprites (16 KB en modos bitmap). 256 colores de BG y 256 de
  sprites (16 paletas de 16 en 4bpp). Butano reparte VRAM y paletas solo; si se agota, `BN_ASSERT` lo dice: es una
  tarea de ingeniería (compartir paletas, tiles bajo demanda con `sprite_tiles_ptr`), no una excusa para recortar arte.
- Fondos regulares hasta 4 (modo 0), mapas grandes con `bn::regular_bg_map_ptr`/fondos dinámicos
  (`examples/dynamic_regular_bg`, `world_map`).

## Revisión
- Mira la captura de la ROM (skill `gba-run`), no sólo el PNG: legibilidad a 1x, contraste con el fondo, pies.
- PixelLab sólo dentro de la suscripción; concepto/base y limpieza a mano a tamaño final.
