# Arte editable de DELVE (006_delve)

Los `.aseprite` de esta carpeta son la **fuente de verdad** (modo indexado). El build no los lee: se exportan a
`graphics/` (sprites y `ui_frame`, que compila Butano) o a `art/export/` (texturas, que lee la herramienta de vista
del motor, `tools/build_view.py`). Formatos y tamaños comunes: `../ESPEC_VISTA.md`.

## Exportar tras editar

    python -I projects/006_delve/art/_seed/seed_all.py export-only

Sin `export-only` también regenera las semillas (`_seed/out/*.png`) y crea los `.aseprite` **que falten**; nunca
sobrescribe uno existente. Para rehacer uno desde la semilla, bórralo antes (sólo si no tiene cambios tuyos).
Revisión: ROM `projects/lab_delve_art` (A/B cambia de escena; `tools/make_scenes.py` la regenera desde aquí).

## Paleta del entorno (texturas, índices 0–63)

Común a todas las texturas `tex_*` (64 entradas; usadas 0–39, el resto negro). **0 = negro** (fondo del bitmap; en
`tex_bars` y `tex_arch` es el hueco por el que se ve lo de detrás). En los `.aseprite` de fondo el índice
transparente es el 63 (sin usar) para que el 0 siga siendo negro.

| Índices | Rampa | Uso |
|---|---|---|
| 0–1 | negro, vacío | huecos, sombras profundas |
| 2–9 | piedra cálida (oscura → clara) | muros, suelo, techo, arcos |
| 10–14 | musgo | juntas |
| 15–21 | madera | puerta, vigas, mangos |
| 22–27 | hierro | herrajes, reja, palanca, cadenas |
| 28–32 | llama | antorcha |
| 33–36 | hueso | calaveras, estandarte |
| 37–39 | tela roja | estandarte |

Luz: antorcha del jugador, de frente y desde arriba a la izquierda (bordes superiores claros, inferiores oscuros).
Sin ruido de 1 px: manchas de 3×2 o más y líneas limpias, para que no "hierva" al reducir.

## Paleta de la interfaz (`ui_frame`, índices 192–255)

HUD opción A de `../HUD.md` (G-32), dibujado por `_seed/delve_ui.py`. Usados 192–223: piedra oscura 192–197, hierro
198–203, latón 204–207, placas hundidas 208–209, vida (rojo) 210–214, recarga 215–216 (el relleno es el latón),
hoja de acero 217–218, cuero 219–221, vidrio 222–223. **240–247 son del minimapa del motor**: no se usan aquí. El
índice 0 es el hueco transparente de la vista (0,0,176,120). El juego copia los colores 192–239 y 248–255 de este
fichero a la paleta al arrancar (`src/dl_hud.cpp`): editar un color aquí y exportar basta.

Cajas del `ui_frame` (x0, y0, x1, y1 inclusive; el código las usa, si se mueven hay que tocar `src/dl_hud.cpp`):
- Panel: brújula y planta (178, 2, 237, 27), con los puntos de la cinta en y = 9; pozo del minimapa (178, 30, 237, 91),
  el motor dibuja 60×60 en (178, 31); llaves (178, 94, 237, 117), icono de llave 16×16 en (181, 97) (el juego lo tapa
  sin llaves).
- Franja: arma (2, 122, 39, 157) con la espada 32×28 en (4, 124) y la ranura de recarga (4, 152, 37, 156); vida y
  objeto (42, 122, 237, 145) con la ranura de vida (60, 127, 155, 134) y la poción en (200, 126); mensajes
  (42, 147, 237, 158), 28 caracteres de la fuente 8×8 desde (46, 149).
- Todo el texto (`E · S · W`, `FLOOR 1`, `HP`, `24/30`, `x3`, `B`, `x1`, mensajes) lo pone el juego con sprites de
  la fuente de Butano: no se dibuja texto en el `.aseprite`.

`ui_arrow` (sprite 8×16, 16 colores, 0 transparente): flecha de daño que apunta a la izquierda; el juego la voltea
para el borde derecho.

## Texturas 64×64 (`art/export/*.bmp`, 8 bpp)

`tex_wall_stone` (repite en horizontal y vertical), `tex_door`, `tex_door_open`, `tex_floor`, `tex_ceiling`,
`tex_stairs_down`, y decoración sobre el mismo muro: `tex_wall_torch` (2 frames, etiqueta `flame`, 150 ms; el
export es una tira 64×128), `tex_wall_lever_up`, `tex_wall_lever_down`, `tex_wall_chains`, `tex_wall_crack`
(pista de secreto), `tex_wall_dart`, `tex_wall_banner`, `tex_wall_skulls`, `tex_bars` (reja, 0 = se ve a través),
`tex_arch` (arco, 0 = se ve a través).

## Monstruos 64×64 (`graphics/mon_*.bmp`, 16 colores, 0 transparente)

`mon_rat`, `mon_skeleton`, `mon_slime`. Un solo dibujo; el tamaño por distancia lo pone el hardware (1,0 / 0,55 /
0,35). Tira vertical de 5 frames, pies en la última fila:

| Etiqueta | Frames | ms | Qué es |
|---|---|---|---|
| `idle` | 1–2 | 400 | reposo y respiración |
| `attack` | 3–4 | 150 | anticipación (se encoge y crece hacia arriba) y golpe (se acerca, 1,12×) |
| `hit` | 5 | 200 | retroceso y destello claro |

Índice 1 = contorno casi negro; 2–15 de oscuro a claro. Contorno de 2 px en casi todo el borde.

## Atrezzo 32×32 (`graphics/prop_*.bmp`, 16 colores, 0 transparente)

`prop_barrel`, `prop_bones`, `prop_crate`, `prop_statue`, `prop_table`, `prop_chest` (frames `closed`, `open`),
`prop_spikes` (`down`, `up`), `prop_plate` (`up`, `pressed`), `prop_pit`. Apoyados en la fila inferior; planos
(`spikes`, `plate`, `pit`) dibujados ya en perspectiva de suelo.

## Origen

Conceptos de PixelLab (dentro de la suscripción) en `_seed/concepts/`: monstruos y atrezzo con volumen. Texturas,
`ui_frame`, pinchos, placa y foso, dibujados por `_seed/delve_art.py`, `delve_ui.py` y `delve_sprites.py`.
Los monstruos se reducen a 16 colores, se les engorda el contorno y se animan con transformaciones a mano.

## Adornos de pared en relieve (`deco_*`, sprites 32×64, 16 colores, 0 transparente)

Sustituyen a los adornos pintados dentro del muro (`tex_wall_torch`, etc.; esas texturas siguen ahí sin tocar).
Fuente `art/deco_*.aseprite` con dos capas: **`sombra`** (sombra arrojada sobre el muro) y **`objeto`**. Exportar:

    python -I projects/006_delve/art/_seed/deco_seed.py export-only

(sin `export-only` regenera las semillas con `_seed/deco_art.py` y crea sólo los `.aseprite` que falten).

**Escala**: el sprite mide lo que el muro, en texels: la fila `v` del sprite es la fila `v` de la textura de 64 de alto
(ojo a la altura 32). El motor lo escala igual que la cara de pared: `escala = alto de la cara en px / 64` (frente d=0
1,25 … d=4 0,31). Vistas: `_front` (pared de frente), `_left` (pared a la izquierda: el adorno sale hacia la derecha,
hacia el centro de la vista; lo paralelo al muro va comprimido a 0,32 y lo que sobresale va 1:1), `_right` (espejo).

| Sprite | Frames y etiquetas | Anclaje (píxel del sprite que toca el muro) |
|---|---|---|
| `deco_torch_front` | `flame` 1–4: 110/70/140/90 ms | (16, 43): centro de la placa |
| `deco_torch_left` / `_right` | ídem | (6, 43) / (26, 43) |
| `deco_banner_front` | `wave` 1–2: 500 ms | (16, 5): entre los dos soportes de la barra |
| `deco_banner_left` / `_right` | ídem | (9, 5) / (23, 5) |
| `deco_chains_front` | 1 frame | (16, 10): entre las dos argollas |
| `deco_chains_left` / `_right` | 1 frame | (9, 10) / (23, 10) |
| `deco_lever_front` | `up` 1, `down` 2 | (16, 32): eje de la palanca |
| `deco_lever_left` / `_right` | ídem | (9, 32) / (23, 32) |
| `deco_torch_glow` | 64×64, 1 frame | su centro va en el centro de la llama: píxel (16, 12) de `torch_front`/`_left`, (15, 12) de `_right` |

El anclaje lateral corresponde a la columna central de la cara (u = 32 de la textura) a la fila indicada. Para colocar
el sprite: `centro = punto_del_muro − (anclaje − (16, 32)) · escala`.

Halo: el motor lo pinta **detrás** de su antorcha (más `z_order`) con `set_blending_enabled(true)` y
`bn::blending::set_transparency_alpha` ≈ 0,4, a la misma escala que la antorcha (en GBA no hay mezcla entre sprites:
el halo encima taparía la antorcha). Rampa propia de 7 tonos cálidos claros, para que nunca oscurezca el muro.

Paletas (todas con 0 = transparente, 1 = contorno, 2–3 = sombra sobre piedra):
- **A** (antorcha, cadenas, palanca; idéntica en los tres, Butano la comparte): 4–8 hierro (8 = brillo cálido de la
  llama), 9–11 madera, 12–15 llama.
- **B** (estandarte): 4–8 hierro, 9–12 tela roja, 13–15 oro.
- **Halo**: 1–7 de borde a centro.

Luz: la del jugador (frente, arriba a la izquierda; en las laterales, desde el lado cercano) para las caras y las
sombras arrojadas (abajo a la derecha en el frente); la llama añade su brillo cálido al soporte. Sin ruido de 1 px.
Pendiente del motor: oscurecer los adornos con la distancia como el muro (p. ej. `set_fade` en copias de la paleta
por profundidad) y compartir los frames de la llama entre antorchas (cada frame 32×64 son 32 tiles; en modo 4 hay 512).
