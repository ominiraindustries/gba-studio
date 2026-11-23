# Arte editable de @NAME@

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
