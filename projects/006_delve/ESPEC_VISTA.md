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
  focal 96 px por casilla (campo de visión más ancho).
- **Grosor de pared: modelo de bandas** (G-31, corrección del usuario a G-29; aspecto de mapa de Dungeon Hack). Cada
  casilla conserva su suelo de **ancho 1** con las caras de sus paredes a ±0,5 del centro (pasillos tan anchos como
  antes de G-29). El grosor **t = 0,375 casillas** (`wall_thick_64` = 24 texels; elegido en el rango 0,375–0,5 para que
  quepan 5 filas sin oscurecer más el fondo) va **entre** casillas: paso de rejilla 1 + t = 1,375. En una arista con
  pared la banda es piedra maciza (se ve igual desde los dos lados; sus extremos son caras de bloque); en una arista
  abierta es suelo y techo (un conector de largo t a ancho completo). Puertas, arcos, rejas y escalera llevan jambas
  de hondo t con la hoja / reja en el centro de la banda (escalera al fondo). El cuadrado t × t donde se cruzan cuatro
  bandas es un **bloque macizo si alguna de sus cuatro aristas es pared** y suelo si todas son abiertas (las salas no
  tienen pilares). Cara frontal de la fila d en z = 1,375·d + 1,2 (anchos 80 / 37 / 24 / 18 / 14 px aprox.).
- Atenuación hacia **negro** con la distancia: exp(−0,30·(z − 1)) (`fade_x1000`), así la cara frontal de la fila 4
  (z = 6,7) conserva el 17 % de antes; más allá de z = 6,7 (`fog_z10`) todo es negro. Números y funciones en
  `include/dl_view_geometry.h` (fuente de verdad); `tools/build_view.py` los lee de ahí.
- Objetos y monstruos de pie 0,2 casillas por delante del centro de su casilla (hacia la cámara): escala = 1,5 / z
  (1 a z = 1,5; ≈ 0,8 / 0,5 / 0,36 / 0,28 en d = 1..4), pies en la línea del suelo a esa z.
- **Cómo se dibuja** (G-31): `tools/build_view.py` traza por píxel cada **pose de cámara**: la vista en reposo y 5
  poses intermedias por tipo de movimiento. Cada pose es una lista de *slots* en orden de pintor (cara de banda,
  bloque de vértice) con una pieza por conjunto de textura, más suelo/techo por orientación y paridad (baldosas fijas
  al **mundo**, una por paso de rejilla, en damero espejado). En el juego, `dl::view` elige el conjunto de cada slot,
  descarta lo tapado por columnas (una pared opaca cubre toda su columna hacia atrás) y copia los tramos; el suelo y
  el techo sólo se copian fuera de la altura de la pared más cercana de cada franja de 8 px. Datos de poses: blob
  binario `src/dl_view_motion.bin` (~14 MB de ROM, incluido con `.incbin`).
- **Movimiento suave tipo Grimrock** (G-31): paso adelante/atrás y lateral (L/R) con 5 posiciones reales de cámara
  intermedias (k/6 del paso de 1,375), giro con 5 ángulos reales (15° a 75°, el ojo en la órbita centro − 0,7·frente,
  así ambos extremos son vistas en reposo). Una imagen cada 2 fotogramas (30 fps): poses en los fotogramas 0, 2..8 y
  la vista nueva en el 10; la orden siguiente se acepta en el 12, así mantener ARRIBA camina sin pararse. Atrás,
  izquierda y giro a la izquierda reproducen las mismas poses al revés desde el destino. Las celdas visibles son la
  unión de las dos vistas en reposo. Interruptor `smooth_moves` en `include/dl_fx.h`; apagado vuelven los trucos
  anteriores (zoom de la vista vieja, deslizamiento al girar, balanceo).
- **Adornos de pared** (antorcha, estandarte, cadenas, palanca): sprites `deco_*` en el centro de la cara que los
  lleva, escala = alto de la cara / 64 (`deco_ref_z10`), máx. 2; vista frontal o lateral según el ángulo de la cara con
  la cámara (cambia a 45° durante el giro); en las laterales la escala horizontal sigue el escorzo real frente al 0,32
  del dibujo (0,6–1,5). Se oscurecen por bandas de profundidad (60 % / 36 %; la llama no). Sólo se muestran si su
  cara es lo que se ve en su columna. Siguen la cámara intermedia en cada pose, igual que monstruos y objetos.
- **Monstruos y objetos**: se ocultan si una cara opaca más cercana que ellos es lo que se ve en alguna columna de los
  3/4 centrales de su sprite (`dl::view::occluded`, profundidad exacta de la cara en esa columna). No se recortan con
  ventanas: hay 2 ventanas para todos los sprites y el arte deja margen transparente.
- **Rendimiento** (gbarun, recorrido de G-29 `plan2.txt`, 4389 ticks = 1 f; antes = G-29 con losas centradas):

  | Medida | Antes | G-31 |
  |---|---|---|
  | Vista en reposo, máx. | 4842 (1,10 f) | 3926 (0,89 f) |
  | Vista en reposo, media | ≈ 4300 | 3349 (0,76 f) |
  | Redibujado vista + minimapa, máx. | 4991 (1,14 f) | 4361 (0,99 f) |
  | Pose intermedia de movimiento, máx. | — (zoom 3331) | 3751 (0,85 f) |
  | Preparación (visibilidad + rejilla), máx. | — | 537 (0,12 f) |

  Las cifras incluyen la interrupción de VBlank si cae dentro. Claves: código caliente en IWRAM (`BN_CODE_IWRAM` en
  todas las funciones, no sólo las del fichero), descarte por columnas y suelo/techo recortado por franjas.
- **Halo de antorcha** (G-29): mezcla aditiva (pesos 1 / 1) con una paleta que baja a negro en el borde, así el disco
  no deja bordes rectos ni anillos sobre lo de detrás; intensidad por bandas 100 / 85 / 65 % (d = 0 / 1 / ≥ 2).
- Puertas abiertas dejan ver lo que hay detrás; la puerta con cerradura usa `tex_door_locked` (provisional generada
  desde `tex_door` con banda y candado de hierro hasta que el artista la dibuje).

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
