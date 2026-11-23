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

Usados 192–209: piedra oscura 192–197, hierro 198–203, latón 204–207, placas hundidas 208–209. El índice 0 es el
hueco transparente de la vista (0,0,176,120).

Huecos del `ui_frame` (para el motor):
- Panel: placa de estado (179..236, 3..54); pozo del minimapa 60×60 exacto (178..237, 58..117).
- Franja: pozo del arma (3..56, 125..140); ranura de recarga 52×8 (60..111, 128..135) para la barra de 48×4 en
  (62,130); placa de mensajes 230×12 (5..234, 144..155) para 28 caracteres de la fuente 8×8.

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
