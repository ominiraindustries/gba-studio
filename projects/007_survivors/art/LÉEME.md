# Arte de 007_survivors (HEXBOUND) — estilo C "paleta limitada con brillo"

Desde G-44 (seguimiento) todo el set está en el **estilo C** elegido por el usuario (`style_samples/style_C.png`):
suelo casi negro con textura sutil, enemigos oscuros con **borde frío de 1 px** (`#3d5590`) y ojos de color,
Wendel en lila luminoso con **contorno claro** (`#b4a4f4`), y magia, gemas y FX en **neón** (magenta, cian, lima,
lila). Mismos nombres, tamaños, etiquetas y orden de frames que antes: el código no cambia.
El set anterior (pradera, colores vivos) está guardado en `old_style/` (no se exporta).

Fuente de verdad: los `.aseprite` de esta carpeta (modo **indexado**, 16 colores, **índice 0 = transparente**, una
etiqueta por animación con sus tiempos). El build de Butano lee `graphics/*.bmp` + `.json`, que se exportan desde aquí.

## Flujo

```
python -I art/tools/build_art.py export    # .aseprite -> graphics/*.bmp (4 bpp) + .json  (usa Aseprite en modo lotes)
python -I art/tools/build_art.py contact D # mockup 240x160 (1x y 3x) en la carpeta D, hecho sólo con graphics/
python -I art/tools/build_art.py seed      # SÓLO siembra: crea los .aseprite que falten desde art/raw/. Nunca pisa uno existente.
python -I art/tools/fetch_raw.py           # vuelve a bajar las fuentes de PixelLab a art/raw/ (no hace falta normalmente)
python -I art/tools/restyle_c.py           # REHACE el set en estilo C desde old_style/ (PISA art/*.aseprite: sólo si
                                           # nadie los ha editado a mano; luego build_art.py export)
```

Si editas un `.aseprite`, manda tu versión: `export` lo respeta. Para regenerar uno desde cero, bórralo y `seed <nombre>`.
`art/raw/` son las imágenes originales de PixelLab (semilla); `art/raw/_export/` es temporal.

## Recursos (nombre en código = nombre del fichero)

| Fichero | Tipo Butano | Frame | Etiquetas (frames) | Paleta |
|---|---|---|---|---|
| `wizard` (Wendel) | sprite | 32×32 | idle_s 0–3, idle_e 4–7, walk_s 8–13, walk_e 14–19, walk_n 20–25, hurt 26–27, death 28–34, idle_se 35–38, idle_ne 39–42, idle_n 43–46, walk_se 47–52, walk_ne 53–58 (O/SO/NO = volteo horizontal; 0–34 igual que antes) | mago (10 colores del maestro) |
| `slime` | sprite | 16×16 | hop 0–3, death 4–7 | propia (7 colores del maestro) |
| `slime_king` | sprite | 64×64 | idle 0–3, crouch 4, air 5–6, land 7–8 | slime+rey |
| `bat` | sprite | **32×16** | fly 0–3, death 4–6 | no-muertos C (= skeleton) |
| `skeleton` | sprite | **16×32** | walk_e 0–3, death 4–7, walk_s 8–11, walk_se 12–15, walk_ne 16–19, walk_n 20–23 (O/SO/NO = volteo) | no-muertos C (= bat) |
| `skeleton_big` | sprite | 32×32 | walk 0–5, death 6–12 | no-muertos (bloque 2 / élite) |
| `goblin` | sprite | **32×32** | walk_e 0–3, death 4–7, walk_s 8–11, walk_se 12–15, walk_ne 16–19, walk_n 20–23 (O/SO/NO = volteo; figura ~20 px, pies en la fila 30) | pieles verdes C (= orc) |
| `orc` | sprite | 32×32 | igual que `goblin` (pies en la fila 31) | pieles verdes C (= goblin) |
| `orc_warchief` | sprite | 64×64 | walk 0–5, attack 6–14 | propia (jefe bloque 2, provisional) |
| `gems` | sprite | 8×8 | cian pequeña 0–1 (1 XP), magenta con halo 2–3 (5 XP), oro grande 4–5 (25 XP); el 2.º frame de cada una es el destello | recogibles (fija) |
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
| `fx_nova` | sprite | 64×64 | burst 0–7 (50 ms) | FX: anillo de Hex Nova que crece (0–1 destello central, 4–7 se deshace en tramado) |
| `fx_nova_spark` | sprite | 8×8 | twinkle 0–3 (60 ms) | FX: chispa suelta de la Nova |
| `fx_blink` | sprite | 32×32 | blink 0–5 (50 ms) | FX: 0–2 implosión (salida), 3–5 destello (llegada) |
| `wizard_ghost` | sprite | 32×32 | mismas que `wizard` (59 frames) | silueta lila plana con borde claro para la estela del Blink (usar con transparencia) |
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

Todos los `hud_*`, `card`, `fx_levelup` y `logo` comparten la **paleta UI** (un solo `sprite_palette_ptr`). Mismos
índices que antes, colores de estilo C: 1 contorno `#05050a`; 2 panel `#14162c`; 3 lila neón `#b070ff`; 4–6 marcos de
acero frío `#1c2440 #34466e #5a74a8` (antes maderas); 7–8 cian neón `#1aa8d0 #80f4ff` (antes oros: logo, flechas
"gold", "selected"); 9–10 panel de carta `#181c36 #22284a` (antes pergamino); 11–13 magenta de XP `#7a1478 #e020c0
#ff9cf0`; 14 rojo `#ff3050`; 15 blanco `#f4f0ff`. `hud_slots` usa la paleta **recogibles** (la de `icons`).
Texto: `text_ink` = blanco frío sobre el panel oscuro de la carta, `text_gold` = cian neón, `text_red` = rojo neón
(los nombres se mantienen por el código).

| Fichero (código) | Tipo | Frame | Etiquetas / frames | Uso |
|---|---|---|---|---|
| `hud_xp` | sprite | 32×8 | fill 0–16 (frame k = 2·k px llenos) | barra de XP: 7 piezas desde x=4 (224 px); pieza i usa `clamp(xp_px − 32·i, 0, 32) / 2` |
| `hud_xp_cap` | sprite | 8×8 | left 0, right 1 | remates: izq. en x=1, der. en x=228 (encima de la barra) |
| `hud_hp` | sprite | 32×8 | fill 0–9 (2 px por frame, 18 px) | barra de vida bajo el mago (barra de 20 px centrada) |
| `hud_heart` | sprite | 8×8 | full 0, half 1, empty 2 | alternativa en corazones |
| `hud_plaque` | sprite | 32×16 | left 0, right 1 | panel de 64×16 para el reloj / nivel; texto centrado en su fila 4 |
| `hud_slots` | sprite | 16×16 | weapon 0–8, passive 9–17 | ranura con icono: 0/9 vacía, 1–8 / 10–17 = `icons` 0–7 (oro = arma, azul = pasiva) |
| `hud_arrow` | sprite | 16×16 | gold 0–2, red 3–5 (este, noreste, norte) | flecha de borde; resto de direcciones con `set_horizontal_flip`/`set_vertical_flip` |
| `hud_nova` | sprite | 16×16 | charge 0–4, full 5–6 (150 ms) | medidor de Hex Nova: hexágono que se llena de magenta (0 vacío … 4 lleno); 5↔6 brillo de "lista" |
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

Título: escena nocturna (cielo con estrellas, arco, lápidas, obelisco, árbol muerto, Wendel con brillo magenta y gemas
en el suelo). "PRESS START" lo pone el código (hacia y=148 sobre el suelo oscuro; con la sombra de la fuente se lee bien).
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

En estilo C los nombres se mantienen pero el dibujo cambia (fuentes en `raw/style_samples/C_deco|C_chars`):
tree y pine = árboles muertos · arch = arco rúnico · pillar = obelisco rúnico · ruin_wall = arco en ruinas ·
rock = roca · bush / bush_flowers = matorrales oscuros · stump = calavera · mushrooms = setas con sombrero cian que
brilla · pillar_stump = columna caída · rune_stone = piedra rúnica pequeña · tall_grass / flowers = hierba muerta ·
pond = charca oscura con reflejo cian · crystal = cristal frío · log = lápidas · rocks_small = piedras.

## Paletas (4 bpp, 16 colores; índice 0 transparente)

Colores exactos en `tools/restyle_c.py` (tablas `WIZ`, `UI`, `PICK`, `FX`, `CLASS_RECIPES`).

- **mago** (fija): 1 **contorno claro** `#b4a4f4` (borde exterior de 1 px); 2–6 lilas `#2e1c5c #5a3aa8 #8c64e0 #b898ff
  #e8dcff` (más luminosos que cualquier enemigo); 7–8 piel; 9–10 barba; 11–12 bastón; 13 cian neón; 14–15 cristal
  magenta `#ff5ce8 #a02090`.
- **recogibles** (fija): 1 contorno; 2–4 lima neón (gema `green`, 5 XP); 5–7 cian neón (gema `blue`, 1 XP); 8–10
  magenta/rosa (gema `red`, 25 XP; corazón); 11–13 oros (moneda, cofre); 14 blanco; 15 morado oscuro.
- **FX** (fija): 1 contorno; 2 blanco; 3–5 rosas/magenta neón (chispa, Nova); 6 rojo (aura de élite); 7–10 cian
  (hielo, Blink); 11–14 lilas (runa, Nova); 15 sombra casi negra. `fx_nova`, `fx_nova_spark`, `fx_blink` la usan.
- **enemigos**: cuerpo oscuro, ojos de color y borde frío `#3d5590` fuera de la silueta. Grupos con paleta común:
  **slime** (verde azulado oscuro, ojos lima) · **no-muertos** (huesos fríos casi blancos, alas moradas, ojos rojos)
  · **pieles verdes** (piel azul noche, ojos amarillo neón) · **orc_warchief** (armadura oscura, rojo neón) ·
  **slime_king** (paleta propia) · **shrine** (piedra fría, runa magenta).
- **suelo** `ground`: `#07060c` con manchas suaves `#0b0a15`/`#100f1e`, motas frías y grietas. Índice 0 = `#07060c`
  (color de fondo de la pantalla), no lo usa ningún píxel. Pocos tiles únicos (~130).
- **atrezzo**: 15 colores fríos fijos (`PROPS_PAL`) con cian y magenta para los brillos.

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
- Estilo C: los enemigos y el mago conservan las animaciones del set anterior recoloreadas (mismas siluetas); si se
  quiere más detalle "oscuro", rehacer animaciones con PixelLab sobre la misma rejilla de frames.

## Paleta maestra y personajes desde las muestras C (G-44, revisión "NES oscuro mágico")

- **Paleta maestra**: `palette_c.json` (38 colores sacados de `style_samples/style_C.png`). Todo `.aseprite` usa sólo
  esos colores: `python -I art/tools/snap_master.py [nombres]` ajusta la paleta de cada fichero al color maestro más
  cercano (no toca índices ni frames). Si pintas a mano, elige de esa lista; nada de tonos nuevos.
- **Personajes**: `python -I art/tools/build_chars_c.py` rehace `wizard`, `wizard_ghost`, `slime`, `bat`, `skeleton`
  desde `raw/style_c2/` (PISA esos cinco: no lo uses si los has retocado). Origen: las muestras `C_chars` 00 (Wendel),
  04 (Wendel de lado), 06 (slime), 09 (murciélago), 12 (esqueleto), recortadas al tamaño del juego en
  `raw/style_c2/first/` y animadas con PixelLab (personajes con plantillas walking/breathing-idle/falling-back-death;
  slime y murciélago con animación v3; muertes de slime/murciélago y golpe de Wendel hechos por código con píxel duro).
  Wendel: un solo personaje PixelLab (`raw/style_c2/wendel_v3/`) en las 5 direcciones, misma altura (30–31 px) y el
  mismo **borde claro `#8ca4b4` de 1 px** en todas (el frío `#34466e` no se veía sobre el suelo casi negro). S y N =
  plantillas walking/breathing-idle; SE y NE = animación v3 desde sus rotaciones; E = v3 desde la rotación este con el
  bastón dibujado a mano (`wendel_v3/Idle/east_staff.png`). Esqueleto `walk_e`: huesos engrosados 1 px (`thicken`) y
  tela de la plantilla pasada al verde azulado de las costillas, para que pese como la vista frontal.
- **Neones** (`python -I art/tools/neon_c.py`, PISA `gems.aseprite`): añade a `palette_c.json` cian `#1a7f9c #40e8f8
  #c0fcff` y oro `#a06818 #ffc838 #fff4b0` (lista `neons`, con los magentas de la muestra). Recogibles: 2–3 cian, 4 oro
  oscuro, 10 magenta `#fd3ffd`, 12–13 oro; FX: 7–9 hielo cian (Frost Ring, Blink).
- **goblin / orco**: personajes PixelLab v3 desde las muestras C 16 y C 20 (`raw/style_c2/goblin_c`, `orc_c`), walk
  plantilla de 4 frames en 5 direcciones + muerte de lado; paleta común de las muestras 16–21 con borde frío `#34466e`.
  Ojo VRAM: 32×32 en 5 direcciones = 64 tiles por dirección; con goblin y orco a la vez en caché llenó la VRAM de
  sprites en una prueba (ver tarea G-44).
- Direcciones: dibujadas S, SE, E, NE, N; las de la izquierda se hacen con `set_horizontal_flip(true)`.
- **Título**: el mago es `raw/style_c2/title/title_wizard.png` (PixelLab, estilo de la muestra 00, paleta maestra);
  `title_bg` se rehízo con `restyle_c.build_title` apuntando a esa carpeta y luego `snap_master.py`.
- Versión anterior (recoloreado) en `old_style/recolor_c/`.
