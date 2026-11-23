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
- **Paredes en los lados de las casillas** (modelo v2): cada pieza es la cara de una arista vista desde la casilla
  del jugador. Se ven **5 filas** (d = 0, la casilla del jugador, a d = 4) y **3 casillas a cada lado** (x = −3..+3).
- Cámara (motor, 2026-11-23, tras la prueba del usuario): ojo a media altura de pared, 0,7 casillas por detrás del
  centro de su casilla; **horizonte en y = 48** (40 % de la vista: mira un poco hacia abajo, más suelo que techo);
  focal 96 px por casilla (campo de visión más ancho). Cara frontal de la fila d en z = d + 1,2 casillas: anchos
  80 / 44 / 30 / 22 / 18 px (d = 0..4) redondeados a par; las caras laterales son trapecios a columnas de 2 px.
- Atenuación hacia **negro** con la distancia (exp(−0,42·(z − 1))): el fondo se pierde en oscuridad; más allá de
  z = 5,2 todo es negro. Los números exactos y las funciones están en `include/dl_view_geometry.h` (fuente de verdad);
  `tools/build_view.py` los lee de ahí.
- Objetos y monstruos de pie a 0,3 casillas del borde cercano de su casilla: escala 1,0 / 0,565 / 0,394 / 0,302
  (d = 1..4).
- Pendiente (relevo G-25): grosor de pared (~1/8 de casilla, cantos y jambas) y decoraciones de pared como sprites
  (`deco_*_front/left/right`) con parpadeo de luz; mientras, antorcha/estandarte/cadenas/palanca siguen dentro de la
  textura del muro.

## Flujo de arte (editable por el usuario)
1. El artista dibuja **texturas planas** en `art/*.aseprite` (indexado, paleta común del entorno de hasta 64 colores):
   - `tex_wall_stone` 64×64, `tex_door` 64×64 (cerrada y abierta), `tex_floor` 64×64, `tex_ceiling` 64×64,
     `tex_stairs_down` 64×64 (cara frontal con escalera).
2. La herramienta `tools/build_view.py` (la hace el motor) proyecta cada textura a todas las piezas de la geometría,
   aplica la atenuación por distancia, cuantiza el conjunto a la paleta de 256 colores (0 = negro, 0–191 entorno,
   192–255 reservados para el panel y la franja) y escribe los datos que compila el juego. Nunca toca `art/`.
3. Monstruos (decisión del usuario): **un solo dibujo por monstruo**, sprite base **64×64** mirando de frente
   (16 colores, índice 0 transparente), con frames de reposo (2), ataque (2, con anticipación) y golpeado (1).
   **El tamaño según la distancia lo pone el hardware** (sprites afines de Butano con `set_scale`), no versiones
   reducidas a mano: escala ≈ 1,0 a d = 1, ≈ 0,55 a d = 2, ≈ 0,35 a d = 3 (valores exactos en
   `dl_view_geometry.h`). Cuando un monstruo avanza o retrocede una casilla, la escala y la posición se interpolan
   durante el paso (≈ 12 f), así se acerca de forma continua. El dibujo base debe leerse bien reducido: siluetas
   claras, contorno oscuro de 1–2 px y pocos detalles de 1 px. Un sprite afín usa una de las 32 matrices: como mucho
   unos 8 monstruos visibles, de sobra.
4. Panel y franja: `ui_frame` 240×160 indexado con los mismos 64 índices reservados (192–255), con el hueco de la vista
   transparente (índice 0).

## Añadido tras DISENO.md (2026-11-23)
- **Objetos en el suelo y proyectiles** (bola de fuego, flechas): sprite base **32×32**, un solo dibujo, escalado por
  hardware igual que los monstruos (escala por profundidad de `dl_view_geometry.h`), apoyado en el suelo de su casilla.
- **Flechas de ataque lateral o por la espalda**: se dibujan como sprites en un hueco de 8×16 px pegado a cada borde
  de la vista (izquierda, derecha, abajo); el arte de pared no pone nada importante en esas franjas.
- **Franja inferior**: debe caber una barra de recarga de ≈ 48×4 px y una línea de mensaje de 28 caracteres de la fuente
  8×8 de Butano.
