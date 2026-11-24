# Arte de 007_survivors (Mossy Meadow, bloque 1)

Fuente de verdad: los `.aseprite` de esta carpeta (modo **indexado**, 16 colores, **índice 0 = transparente**, una
etiqueta por animación con sus tiempos). El build de Butano lee `graphics/*.bmp` + `.json`, que se exportan desde aquí.

## Flujo

```
python -I art/tools/build_art.py export    # .aseprite -> graphics/*.bmp (4 bpp) + .json  (usa Aseprite en modo lotes)
python -I art/tools/build_art.py contact D # mockup 240x160 (1x y 3x) en la carpeta D, hecho sólo con graphics/
python -I art/tools/build_art.py seed      # SÓLO siembra: crea los .aseprite que falten desde art/raw/. Nunca pisa uno existente.
python -I art/tools/fetch_raw.py           # vuelve a bajar las fuentes de PixelLab a art/raw/ (no hace falta normalmente)
```

Si editas un `.aseprite`, manda tu versión: `export` lo respeta. Para regenerar uno desde cero, bórralo y `seed <nombre>`.
`art/raw/` son las imágenes originales de PixelLab (semilla); `art/raw/_export/` es temporal.

## Recursos (nombre en código = nombre del fichero)

| Fichero | Tipo Butano | Frame | Etiquetas (frames) | Paleta |
|---|---|---|---|---|
| `wizard` (Wendel) | sprite | 32×32 | idle 0–3, idle_side 4–7, walk_down 8–13, walk_side 14–19, walk_up 20–25, hurt 26–27, death 28–34 | mago (fija) |
| `slime` | sprite | 16×16 | hop 0–3, death 4–7 | slime+rey |
| `slime_king` | sprite | 64×64 | idle 0–3, crouch 4, air 5–6, land 7–8 | slime+rey |
| `bat` | sprite | **32×16** | fly 0–3, death 4–6 | no-muertos |
| `skeleton` | sprite | **16×32** | walk 0–3, death 4–7 | no-muertos |
| `skeleton_big` | sprite | 32×32 | walk 0–5, death 6–12 | no-muertos (bloque 2 / élite) |
| `goblin` | sprite | 16×16 | walk 0–5 | pieles verdes (bloque 2) |
| `orc` | sprite | 32×32 | walk 0–5 | pieles verdes (bloque 2, provisional) |
| `orc_warchief` | sprite | 64×64 | walk 0–5, attack 6–14 | propia (jefe bloque 2, provisional) |
| `gems` | sprite | 8×8 | blue 0–1 (1 XP), green 2–3 (5 XP), red 4–5 (25 XP) | recogibles (fija) |
| `coin` | sprite | 8×8 | spin 0–1 | recogibles |
| `chest` | sprite | 16×16 | closed 0, open 1 | recogibles |
| `heart` | sprite | 16×16 | idle 0 | recogibles |
| `icons` | sprite | 16×16 | spark_bolt 0, rune_orbit 1, frost_ring 2, swift_boots 3, heart_amulet 4, magnet 5, spell_tome 6, star 7 | recogibles |
| `shrine` | sprite | 16×32 | idle 0 | propia |
| `fx_spark` | sprite | 8×8 | loop 0–1 (Spark Bolt) | FX (fija) |
| `fx_rune` | sprite | 8×8 | loop 0–1 (Rune Orbit) | FX |
| `fx_frost` | sprite | 16×16 | grow 0–2 (segmento de Frost Ring) | FX |
| `fx_impact` | sprite | 8×8 | hit 0–2 | FX |
| `fx_poof` | sprite | 8×8 | poof 0–3 | FX |
| `fx_aura` | sprite | 16×16 | loop 0–1 (aura de élite) | FX |
| `shadow_small` / `shadow_big` | sprite | 16×8 / 32×16 | idle 0 (sombra; usar con transparencia; el rey la usa de aviso de caída) | FX |
| `ground` | regular_bg 512×512 | — | — | suelo (se repite sin costura: el borde derecho casa con el izquierdo y el de abajo con el de arriba) |
| `props` + `props_palette` | regular_bg_tiles + bg_palette | — | ver tabla de atrezzo | atrezzo |

Pies: los personajes de suelo tienen los pies en la **última fila** del frame (el mago, 1 px por encima). El
murciélago está centrado. Cada animación lleva un único desplazamiento (no tiembla dentro del bucle).
Lado: `walk_side`, `skeleton`, `goblin`, `orc` miran a la **derecha**; a la izquierda, `set_horizontal_flip(true)`.

## Atrezzo (`props.bmp`, 256×136, rectángulos alineados a 8 px; `art/props_layout.json`)

tree 0,0 56×64 · pine 56,0 48×64 · arch 104,0 64×64 · pillar 168,0 32×64 · ruin_wall 0,64 64×40 · rock 64,64 32×32 ·
bush 96,64 · bush_flowers 128,64 · stump 160,64 · mushrooms 192,64 · pillar_stump 224,64 (32×32) · rune_stone 0,104 24×32 ·
tall_grass 24,104 · flowers 56,104 · pond 88,104 · crystal 120,104 · log 152,104 (32×32) · rocks_small 184,104 32×24.
El índice 0 es transparente: se pinta en una segunda capa de fondo sobre `ground`.

## Paletas (4 bpp, 16 colores; índice 0 transparente)

- **mago** (fija, `build_art.py: PAL_WIZARD`): 1 contorno `#1b1028`; 2–6 morados/lilas `#3a2160 #5e3496 #8a55c8 #b78ae6
  #e2c6ff`; 7–8 piel; 9–10 barba/gris; 11–12 madera del bastón; 13 oro `#f2c84a`; 14–15 cristal `#5ad8f0 #2a7ab0`.
- **recogibles** (fija, `PAL_PICKUPS`): 1 contorno; 2–4 verdes; 5–7 azules; 8–10 rojos; 11–13 oros; 14 blanco; 15 marrón.
- **FX** (fija, `PAL_FX`): 1 contorno azul noche; 2 blanco; 3–6 amarillo→rojo (chispa); 7–10 hielo; 11–14 lilas/runa;
  15 sombra `#102010`.
- **slime+rey**, **no-muertos** (skeleton, bat, skeleton_big), **pieles verdes** (goblin, orc), **orc_warchief**,
  **shrine**, **atrezzo**: paleta común calculada por corte de mediana de todos sus frames (ordenada de oscuro a claro).
  Los sprites del mismo grupo comparten paleta: en código pueden usar un solo `sprite_palette_ptr`.
- **suelo**: verdes y tierras apagados (saturación ×0,62, brillo ×0,9) para que los sprites destaquen; 13–15 flores
  (rosa, amarillo, blanco roto). Índice 0 = color de fondo (verde medio), no lo usa ningún píxel.

## Qué no tocar

- No cambies el tamaño de frame ni el orden de etiquetas sin avisar al programador: el código usa los índices de
  arriba. Añadir etiquetas al final no rompe nada.
- No pases de 16 colores ni muevas el índice 0.
- El texto del juego no va aquí: lo pone `bn::sprite_text_generator`.

## Pendiente / ideas

- Mago: diseño pedía 16×32; es 32×32 (la figura con bastón mide 20 px de ancho).
- `skeleton` es 16×32 (la figura mide 11×22) y la muerte se recorta a 16 px de ancho.
- `orc` y `orc_warchief` (bloque 2) son provisionales: el orco parece encapuchado y el jefe se sale del lienzo al atacar.
- Faltan: HUD (marcos de barras, carta de mejora, flecha de borde, "!"), wolf, Big Slime, título, charca con bordes.
- Élite: `fx_aura` + paleta tintada en código (`set_fade` a rojo/oro) en vez de contorno de color dibujado.
