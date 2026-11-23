# Arte editable de 002_pop_panic (POP PANIC)

Fuente de verdad: los `.aseprite` de esta carpeta (modo **indexado**, índice 0 = transparente).
El build de Butano lee `graphics/<nombre>.bmp` + `.json`; tras editar un `.aseprite`, exporta:

    sh projects/002_pop_panic/art/exportar.sh            (todos)
    sh projects/002_pop_panic/art/exportar.sh player     (sólo uno)

`exportar.sh` usa el Aseprite de Steam (tira vertical) y escribe el `.json`. Con `--seed` crea los
`.aseprite` que falten desde `_seed/` (nunca sobrescribe uno existente). Después, `compilar.bat`.

## Qué no tocar

- El **orden de los colores** de cada paleta (el juego cambia colores por índice).
- Los tamaños de lienzo y el orden de los fotogramas (el código los pide por número).
- `graphics/gb_*.bmp`: cuadros provisionales del programador, no son arte.
- `_seed/`: generador (`seed_art.py`), bases de PixelLab (`pixellab_*.png`) y `build_aseprite.lua`. Sólo
  sirve para volver a sembrar; tus ediciones en los `.aseprite` mandan.

## Sprites (4 bpp, 16 colores)

| Fichero | Lienzo por frame | Frames (orden) | Etiquetas Aseprite |
|---|---|---|---|
| `player` | 16×16, personaje 12×15, **pies en la fila 15** | 0-1 idle · 2-5 walk · 6 shoot (arpón hacia arriba) · 7 hurt · 8 death (tumbado) | idle, walk, shoot, hurt (7-8) |
| `ball_32` | 32×32, bola de 32 | 1 | — |
| `ball_24` | 32×32, bola de 24 centrada | 1 | — |
| `ball_16` | 16×16 | 1 | — |
| `ball_8` | 8×8 | 1 | — |
| `harpoon` | 8×16 | 0 punta · 1 tramo de cadena (se repite hacia abajo cada 16 px) | tip, chain |
| `pop_fx` | 32×32 | 0-3 estallido (destello, anillo, chispas, restos) | pop |
| `item_double` | 16×16 | 1 (insignia azul con dos arpones) | — |
| `item_shield` | 16×16 | 1 (insignia dorada con escudo) | — |
| `pigeon` | 16×16, mira a la derecha | 0 alas arriba · 1 medio · 2 abajo | fly |

El jugador y la paloma miran a la **derecha**; el juego los voltea.

### Paleta del jugador (`player`)
0 transparente · 1 contorno · 2 gorra · 3 gorra sombra · 4 piel · 5 piel sombra · 6 pelo · 7 chaqueta ·
8 chaqueta sombra · 9 pantalón · 10 botas · 11 suela · 12 arma claro · 13 arma oscuro · 14 blanco · 15 amarillo.

### Paleta de las bolas (las cuatro, mismo orden)
0 transparente · **1-4 color base de claro a oscuro** · **5 contorno** · **6 brillo blanco** · 7-15 sin uso.
El juego recolorea cada tamaño cambiando sólo 1-5. Si redibujas, respeta los índices.

### Paleta del arpón
0 transparente · 1 contorno · 2 metal claro · 3 metal oscuro · 4 blanco · 5 dorado · 6 dorado oscuro.

### Paleta de `pop_fx`
0 transparente · 1 blanco · 2 crema · 3 amarillo · 4 naranja · 5 rojo · 6 contorno (reserva).

### Paleta común de `item_double`, `item_shield` y `pigeon`
0 transparente · 1 contorno · 2 blanco · 3 amarillo · 4 dorado oscuro · 5 azul claro · 6 azul · 7 azul oscuro ·
8 gris claro · 9 gris · 10 gris oscuro · 11 verde cuello · 12 morado cuello · 13 naranja (ojo) · 14 rojo · 15 rosa.

## Fondos (`bg_madrid`, `bg_paris`, `bg_london`)

- 256×256; se ve el rectángulo 240×160 de arriba a la izquierda. Filas **0-151 escenario**, **152-159 suelo**
  (fila 152 borde de color, 153-159 oscuro y liso: encima va el texto blanco del HUD).
- Columnas 240-255 y filas 160-255 no se ven: van con el índice 0.
- **4 bpp "manual"**: la paleta de 256 está dividida en bancos de 16 (0-15, 16-31, ...). **Cada cuadro de 8×8
  sólo puede usar colores de un mismo banco**, y el primer color de cada banco (0, 16, 32...) no se usa en lo
  visible. Si pintas con colores de dos bancos en un mismo cuadro, la conversión falla. Bancos usados:
  Madrid 7, París 13, Londres 15 (de 16 posibles). `exportar.sh` pone `colors_count` solo.
- Bases de PixelLab (`_seed/pixellab_*.png`, 240×144), apagadas hacia el centro para que bolas y jugador
  destaquen; el escenario se alarga 8 filas repitiendo su franja inferior.
