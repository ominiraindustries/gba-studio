# Arte editable de 001_apila

Aquí van los ficheros fuente de **Aseprite** (`.aseprite`), en modo de color **indexado**.
El build de Butano no lee `.aseprite`: lee las imágenes `.bmp` indexadas de `graphics/`
(cada una con su `.json` al lado). Tras editar un `.aseprite`, expórtalo a `graphics/`.

## Reglas

- Modo indexado, **16 colores por paleta** (4 bpp). El **índice 0 es transparente** en sprites y es el
  color de fondo (backdrop) en fondos.
- Sprites: ancho y alto de 8, 16, 32 o 64 px. Si el sprite tiene varios fotogramas, se apilan en
  **vertical** en un solo `.bmp` y el `.json` dice el alto de cada uno (`"height": 16`).
- Fondos regulares: 256×256, 256×512, 512×256 o 512×512 px.
- El nombre del fichero es el nombre en el código: `graphics/hero.bmp` → `bn::sprite_items::hero`.
  Solo minúsculas, números y `_`.

## Exportar desde Aseprite

Con la interfaz: *Archivo → Exportar como…* → `graphics/<nombre>.bmp` (para una tira de fotogramas:
*Exportar hoja de sprites*, disposición **vertical**, sin bordes).

Por línea de comandos (si `aseprite` está en el PATH):

    aseprite -b art/hero.aseprite --sheet-type vertical --sheet graphics/hero.bmp

Después, `compilar.bat`.

## Arte de APILA (provisional, greybox)

- `cells.aseprite` → `graphics/cells.bmp`: tira de 6 tiles de 8×8 que forman el tablero, en este orden
  (no cambies el orden: el código usa la posición):
  0 vacío (color de fondo), 1 celda del pozo, 2 bloque, 3 sombra (fantasma), 4 pared, 5 destello al borrar.
- Los colores salen de `graphics/cells_palette.bmp`: 8 paletas de 16 colores. Paleta 0 = gris (fin de partida y
  reserva usada), paletas 1–7 = piezas I, O, T, S, Z, J, L. Índices dentro de cada paleta:
  1 brillo, 2 color de la pieza, 3 sombra, 5 pozo, 6 punto de rejilla, 7 pared, 8 luz de pared, 9 blanco.
- Un tile se pinta con esos índices y el juego le pone la paleta de cada pieza; así un solo dibujo de bloque sirve
  para los 7 colores.
- Exportar: `aseprite -b art/cells.aseprite --save-as graphics/cells.bmp`.
- `tools/gen_greybox.py` sólo siembra el BMP provisional y nunca toca `art/`.
