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

## Bloque 2 de arte: HUD, carta, título (`tools/build_ui.py`)

```
python -I art/tools/build_ui.py seed       # SÓLO siembra los .aseprite que falten (nunca pisa)
python -I art/tools/build_ui.py palettes   # graphics/text_*.bmp (paletas para el texto)
python -I art/tools/build_ui.py mock D     # mockups 240x160 (partida+HUD, subida de nivel, título) en D
```
La exportación es la misma (`build_art.py export`). Fuentes de PixelLab en `art/raw/ui/` (`card_0.png` = marco de la
carta, `title_a.png` = ilustración del título); el resto está dibujado en `build_ui.py`.

Todos los `hud_*`, `card`, `fx_levelup` y `logo` comparten la **paleta UI** (un solo `sprite_palette_ptr`):
1 contorno `#140c1c`; 2 morado oscuro (huecos de barra, paneles); 3 morado runa; 4–6 maderas; 7–8 oros; 9–10 pergamino;
11–13 azules de XP; 14 rojo; 15 blanco. `hud_slots` usa la paleta **recogibles** (la de `icons`).

| Fichero (código) | Tipo | Frame | Etiquetas / frames | Uso |
|---|---|---|---|---|
| `hud_xp` | sprite | 32×8 | fill 0–16 (frame k = 2·k px llenos) | barra de XP: 7 piezas desde x=4 (224 px); pieza i usa `clamp(xp_px − 32·i, 0, 32) / 2` |
| `hud_xp_cap` | sprite | 8×8 | left 0, right 1 | remates: izq. en x=1, der. en x=228 (encima de la barra) |
| `hud_hp` | sprite | 32×8 | fill 0–9 (2 px por frame, 18 px) | barra de vida bajo el mago (barra de 20 px centrada) |
| `hud_heart` | sprite | 8×8 | full 0, half 1, empty 2 | alternativa en corazones |
| `hud_plaque` | sprite | 32×16 | left 0, right 1 | panel de 64×16 para el reloj / nivel; texto centrado en su fila 4 |
| `hud_slots` | sprite | 16×16 | weapon 0–8, passive 9–17 | ranura con icono: 0/9 vacía, 1–8 / 10–17 = `icons` 0–7 (oro = arma, azul = pasiva) |
| `hud_arrow` | sprite | 16×16 | gold 0–2, red 3–5 (este, noreste, norte) | flecha de borde; resto de direcciones con `set_horizontal_flip`/`set_vertical_flip` |
| `hud_alert` | sprite | 16×16 | blink 0–1 (150 ms) | "!" de aviso de oleada (rombo rojo; frame 1 = destello) |
| `fx_levelup` | sprite | 64×64 | burst 0–3 (60 ms; 0–2 crece, 3 girado) | rayos detrás del mago al subir de nivel; alternar 2↔3 en bucle |
| `card` + `card_base` | sprite | 64×64 + 64×32 | normal 0, selected 1 | carta 64×80 = `card` en (x,y) + `card_base` en (x,y+48) (se solapan 16 filas idénticas) |
| `logo` | sprite | 64×32 | 6 celdas | logo HEXBOUND 192×64: celda k en (24 + (k%3)·64, 4 + (k/3)·32) |
| `title_bg` | regular_bg **8 bpp** 256×256 | — | — | título: la imagen ocupa (0,0)–(239,159); `create_bg(8, 48)` la alinea a pantalla |
| `text_ink` / `text_gold` / `text_red` | sprite_palette | — | — | `text_generator.set_palette_item(...)`: tinta marrón para la carta, oro para "LEVEL UP!", rojo de aviso |

Fuente del editable: `card.aseprite` es la carta entera 64×80 con 2 frames; la exportación la parte en `card` +
`card_base`. `logo.aseprite` es el logo entero 192×64; la exportación lo corta en celdas de 64×32. Si cambia el nombre
del juego, sólo hay que repintar `logo.aseprite` (o `seed logo` tras borrarlo y cambiar la palabra en `build_ui.py`).

Carta: hueco de icono 16×16 en (x+24, y+10) dentro de un círculo de 20 px; texto de pergamino de unos 48 px de ancho:
nombre en dos líneas (y+28, y+37), filete en y+46, nivel/"NEW!" en y+50, efecto corto en y+59. "Swift Boots" no cabe
en una línea con la fuente variable 8×8. Seleccionada: borde dorado brillante, madera clara y gemas cian; el código
puede además subirla 4 px.

Título: "PRESS START" lo pone el código (hacia y=148 sobre la hierba; con la sombra de la fuente se lee bien).
`title_bg` usa los 256 colores de fondo (8 bpp, 64 usados): mientras esté en pantalla no caben otros fondos 4 bpp con
paleta propia; el logo es de sprites para poder quitarlo o cambiarlo.

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
- Faltan: wolf, Big Slime, charca con bordes. (HUD, carta, flecha, "!" y título: bloque 2, arriba.)
- Élite: `fx_aura` + paleta tintada en código (`set_fade` a rojo/oro) en vez de contorno de color dibujado.
