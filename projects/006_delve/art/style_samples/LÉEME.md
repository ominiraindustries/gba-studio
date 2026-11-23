# Muestras de estilo "oscuro retro" (G-33)

Tres propuestas para sustituir el arte actual (demasiado "mono"). **Sólo muestras**: no tocan `art/*.aseprite`,
`graphics/` ni el motor. El usuario elige una y luego se pasa a los `.aseprite` (segunda fase de G-33).

| Carpeta | Idea |
|---|---|
| `a_cold_dungeon/` | Mazmorra fría: sillares gris azulado, juntas hundidas, regueros de óxido, ojos cian. |
| `b_wet_sewer/` | Cloaca húmeda: ladrillo pequeño verde negruzco, limo en la parte baja, charcos en el suelo, ojos verdes. |
| `c_ancient_crypt/` | Cripta antigua: bloques grandes irregulares pardo grisáceos, grietas, polvo, harapo rojo seco, ojos rojos. |

En cada carpeta: `mockup.png` (240×160) y `mockup_3x.png`; `sheet_3x.png` (muro, suelo, techo, puerta, antorcha,
esqueleto); texturas 64×64 indexadas `tex_wall/floor/ceiling/door.png` con la paleta de la variante (`palette.png`,
33 colores + negro en el índice 0, que las texturas no usan); `deco_torch_front.png` (32×64, índice 0 transparente,
ancla como `deco_torch_*`) y `mon_skeleton.png` (64×64, 16 colores, índice 0 transparente).

Cómo se hizo: pintura procedural (`_src/paint.py`: ruido fBm + tramado Bayer dentro de cada piedra, sin contorno
negro, luz de arriba-izquierda, juntas en sombra, manchas y grietas; esqueleto modelado con cápsulas e iluminado).
La vista está proyectada con una **copia** de `tools/build_view.py` (pose de reposo) y cuantizada a su paleta de 5 bits:
126–140 colores de 191, así que el fundido cabe. Los mockups usan `fade = 0.42` (el motor usa `fade_x1000 = 300`):
oscurecer más con la distancia es parte de la propuesta. El resplandor de la antorcha imita el sprite aditivo del juego.

Regenerar: copiar `tools/build_view.py` junto a `_src/*.py` cambiando `PROJ = ...` por `PROJ = os.environ["DL_PROJ"]`
y ejecutar `python -I mockup.py` (escribe aquí; crea `mirror_*` junto a los scripts).

Pendiente si se elige una: retocar a mano el esqueleto (pose rígida y simétrica), dibujar variantes de muro,
`tex_arch/bars/stairs/door_open`, vistas laterales de la antorcha y rehacer el marco del HUD en el mismo tono
(el actual, latón limpio, sigue viéndose "mono" al lado de estas paredes). Riesgo: el tramado de 1 px puede
parpadear al moverse (los pasos interpolados); comprobar en la ROM antes de cerrar.
