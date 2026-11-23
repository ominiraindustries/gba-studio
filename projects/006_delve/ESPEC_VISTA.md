# DELVE — especificación común de vista y formatos (diseño, arte y motor)

Documento corto que comparten el diseñador, el artista y el programador del motor. Si algo cambia aquí, se avisa a los
tres. El diseño del juego va en `DISENO.md`.

## Pantalla (240×160)
| Zona | Rectángulo (x, y, ancho, alto) | Contenido |
|---|---|---|
| Vista 3D | (0, 0, 176, 120) | mazmorra en primera persona |
| Panel derecho | (176, 0, 64, 120) | vida/maná, brújula, minimapa pequeño, estado |
| Franja inferior | (0, 120, 240, 40) | manos/arma con su recarga, mensajes de una línea |

- Modo de vídeo: **bitmap de paleta 8 bpp (modo 4 de GBA, Butano `palette_bitmap_bg`)** a pantalla completa, con doble
  búfer. La vista sólo se redibuja al dar un paso, girar o abrir una puerta; los monstruos, objetos, efectos y el texto
  del HUD van en **sprites** encima.
- Texto: sólo `bn::sprite_text_generator` con fuentes de `engine/butano/common/`, en inglés.

## Geometría de la vista (clásica, por casillas)
- Rejilla de casillas cuadradas; el jugador está en el centro de su casilla mirando a N, E, S u O; giros de 90°.
- Se ven **4 profundidades**: d = 0 (la casilla del jugador: sólo paredes laterales), 1, 2 y 3, y hasta 2 casillas
  a cada lado en cada profundidad.
- Perspectiva: punto de fuga en el centro de la vista (88, 60). El ancho de una cara frontal a profundidad d es
  `176 / (d + 0,5) · 0,5` redondeado a par (d=1 → 58·… lo fija el motor en `include/dl_view_geometry.h`, que es la
  **fuente de verdad** de los rectángulos de cada pieza).
- Piezas: cara frontal (d, x), cara lateral izquierda y derecha (d, x), suelo y techo por franjas. Más oscuro a más
  profundidad (atenuación de distancia aplicada por la herramienta).

## Flujo de arte (editable por el usuario)
1. El artista dibuja **texturas planas** en `art/*.aseprite` (indexado, paleta común del entorno de hasta 64 colores):
   - `tex_wall_stone` 64×64, `tex_door` 64×64 (cerrada y abierta), `tex_floor` 64×64, `tex_ceiling` 64×64,
     `tex_stairs_down` 64×64 (cara frontal con escalera).
2. La herramienta `tools/build_view.py` (la hace el motor) proyecta cada textura a todas las piezas de la geometría,
   aplica la atenuación por distancia, cuantiza el conjunto a la paleta de 256 colores (0 = negro, 0–191 entorno,
   192–255 reservados para el panel y la franja) y escribe los datos que compila el juego. Nunca toca `art/`.
3. Monstruos: sprite base **64×64** mirando de frente (16 colores, índice 0 transparente), con frames de reposo (2),
   ataque (2, con anticipación) y golpeado (1). La herramienta genera las versiones de 48, 32 y 16 px para
   d = 1, 2 y 3 (vecino más cercano + limpieza; si una reducción no se lee, el artista la retoca a mano en su propio
   `.aseprite`, que entonces manda).
4. Panel y franja: `ui_frame` 240×160 indexado con los mismos 64 índices reservados (192–255), con el hueco de la vista
   transparente (índice 0).
