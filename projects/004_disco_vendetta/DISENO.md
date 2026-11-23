# DISCO VENDETTA — diseño

**Frase**: 1975, un local de la mafia por planta; entras solo, coges lo que encuentras y limpias la planta. Un golpe
te mata, un golpe les mata, y reintentas al instante.
**Verbo central**: *golpear/disparar apuntando* (A), con *coger/lanzar* (B) como segundo verbo. Todo el texto del
juego va en inglés; la estética (clubes, casinos, almacenes; ocres, naranjas, granates, madera) y la música son
propias. No se usa nada de Hotline Miami: ni máscaras, ni personajes, ni nombres, ni plantas, ni paleta neón.

## 1. Bucle, meta y controles
- **Bucle** (de 30 a 90 s): entrar → leer la sala → romper el plan de los enemigos (puerta, lanzamiento, disparo) →
  morir o limpiar → reintentar o pasar a la siguiente planta. **Meta de la planta**: matar a todos y llegar a la
  salida `X`, que se abre cuando no queda ninguno. **Meta del juego**: 5 plantas y un jefe, con nota S–D en cada una.

| Botón | Acción | Nota |
|---|---|---|
| Cruceta | moverse en 8 direcciones (decisión del usuario) | se puede ametrallar en movimiento: la mira no sigue a la cruceta |
| L / R | girar la mira (continua, rotación por hardware) | un toque gira **5,625°** al instante; manteniendo, **6 f de espera y luego 6°/f** (180° en ~36 f) |
| A | golpear o disparar con lo que lleves | sobre un enemigo noqueado: rematar |
| B | coger el arma del suelo (si vas con las manos vacías) o lanzar la que llevas | prioridad fija: si llevas un arma, B la lanza (regla 14) |
| START | pausa: RESUME / RETRY / QUIT | |

Giro: 90° pulsando 4 veces o con 10 + 3·3 = 19 f manteniendo; 180° en 31 f (0,5 s); la vuelta entera en 55 f.
Así se gira 90° antes de que un pistolero dispare (30 f, ver §2) (regla 2).

**Cómo se lee hacia dónde apuntas (regla 5)**: (1) el sprite del jugador gira en las 16 direcciones; (2) delante de
él, 3 puntos de 2×2 a 16, 28 y 40 px (sólo 1 a 14 px con el puño) del color del arma; (3) los puntos se ponen
**rojos** cuando la ayuda a apuntar ha fijado un enemigo. La cámara adelanta 40 px en horizontal y 24 en vertical
hacia la mira, de modo que ves más hacia donde apuntas.

**Cómo se enseña en el primer minuto (regla 6)**: con el espacio y, como mucho, una línea abajo en el momento de
usarla, que desaparece al hacerlo. El orden es: `D-PAD MOVE  L/R AIM` (sala A, segura) → `A: PUNCH` (al tener a menos
de 32 px a un enemigo de espaldas) → `A: FINISH` (tras noquearlo) → `B: PICK UP` (sobre la pistola) → `A: SHOOT` →
`B: THROW` (al quedarte sin balas). Cada rótulo sale una sola vez por partida, también tras reintentar.

## 2. Números (60 fps, 1 tile lógico = 16 px)
| Cosa | Valor | Principio |
|---|---|---|
| Jugador | 1,5 px/f; en diagonal se normaliza a 1,06 por eje | flujo: cruzar la pantalla en 160 f (2,7 s); más rápido que cualquier enemigo que persigue |
| Matón patrullando / persiguiendo | 0,75 / 1,25 px/f | perdón: siempre puedes alejarte de un matón |
| Pistolero | quieto o 0,5 px/f; si te pierde, va a 1,0 al último punto donde te vio | tensión: no te persigue |
| Mira | toque = 5,625°; mantener = 6 f de espera y 6°/f | inmediatez: el primer toque no espera (regla 14) |
| Ayuda a apuntar | si hay un enemigo visible a ±22,5° y a menos de 120 px, el disparo o golpe va a él | perdón sin que se note (regla 2): 16 direcciones son pocas a esta distancia |
| Caja de daño del jugador | 8×8 dentro del sprite de 16×16 | perdón: las balas te rozan sin matarte |
| Caja de muro del jugador | 10×10; se desliza por los muros y corrige 3 px en las esquinas | perdón: no te atascas en los marcos de las puertas |
| Caja de daño del enemigo | 12×12 | generosa a favor del jugador |
| Bala del jugador / enemiga | 6 / 4 px/f, sprite de 4×4 con estela de 1 frame; colisión en 2 subpasos | se ve: la enemiga cruza 112 px en 28 f y da tiempo a apartarse |
| Visión del enemigo | cono de ±45°, 112 px, con línea de visión por tiles (las ventanas `w` y la barra `=` no la tapan) y 360° a **16 px** (menor que el alcance del puño, 20) | legibilidad: 112 px caben en la media pantalla; regla 2: el golpe por la espalda funciona |
| Reacción del pistolero | `!` y apunta durante 30 f (fogonazo en los 10 últimos) → dispara → 50 f entre disparos | anticipación (regla 5) |
| Reacción del matón | `!` y 20 f quieto → persigue → a 20 px levanta el brazo 18 f (parpadeo blanco) → golpe en arco de 90° a 16 px | anticipación; el golpe se cancela si te alejas |
| Disparar fuera de pantalla | prohibido: sólo apuntan y disparan los enemigos con el centro a **24 px** o más de cada borde de la cámara | nada mata sin aviso (regla 5): el enemigo entero y su `!` se ven |
| Oído | **por camino andando** (BFS por tiles, 8 conexiones; las puertas dejan pasar el ruido, los muros no): pistola **14 tiles**, escopeta 18, patada a una puerta **4 tiles**; los golpes no se oyen | riesgo y recompensa legible: el ruido atrae a quien está cerca por el camino; quien oye va a mirar (`?`, 1,0 px/f) y no dispara hasta verte |
| Puerta | se abre al empujarla andando (patada), al instante; quien esté en el barrido de 16×16 al otro lado queda noqueado 120 f | estilo: la puerta es un arma gratis |
| Noqueo | puño 180 f, lanzamiento 90 f, puerta 120 f; antes de levantarse tiembla durante 30 f; remate con A a 16 px (16 f, quieto) | riesgo y recompensa: rematar te deja quieto 16 f |

| Arma | Alcance | Cadencia | Munición | Ruido | Efecto | Al lanzarla |
|---|---|---|---|---|---|---|
| Puño | 20 px entre centros, arco de ±45° | 20 f | ∞ | no | **noquea** | no se lanza |
| Bate (bloque 2) | 20 px, arco de 120° | 24 f | ∞ | no | mata | noquea 90 f y cae donde golpea |
| Pistola | bala hasta el muro | 12 f | 8 (las del suelo, según el sitio) | 192 px | mata | noquea 90 f; conserva sus balas |
| Escopeta (bloque 2) | 5 perdigones en ±15°, vida de 18 f (unos 108 px) | 45 f | 4 | 256 px | mata y empuja a menos de 32 px | noquea 90 f |

**Decisión sobre el daño**: el puño noquea y todo lo demás mata. Así el puño tiene un papel (silencioso, pero obliga a
rematar) y el jugador empieza con algo útil. Lanzar: 5 px/f, hasta 120 px; un arma de fuego sin balas sale en gris
(estado visible, regla 9).

## 3. Enemigos
| Enemigo | Comportamiento | Aviso | Puntos | Principio |
|---|---|---|---|---|
| **Goon** (matón, bloque 1) | patrulla una ruta fija o está quieto; si te ve, persigue y golpea; mata de un golpe | `!` 20 f + brazo levantado 18 f | 300 | presentado de espaldas y quieto (regla 3) |
| **Gunman** (bloque 1) | quieto o en una ruta corta; si te ve, se gira y dispara; si oye algo, se gira hacia el ruido; suelta su pistola con 4 balas | `!` + 30 f apuntando con fogonazo | 500 | anticipación; recompensa: su arma |
| **Dog** (bloque 3) | 2,0 px/f, corre hacia el ruido; no se puede noquear (muere de cualquier golpe) | gruñe y se agacha 15 f | 200 | giro tardío: obliga a apuntar rápido |

## 4. Puntuación
- **Muerte**: los puntos del enemigo × el combo. **Combo**: cada muerte a menos de 150 f (2,5 s) de la anterior lo sube
  en 1, hasta ×8; una barra en el HUD muestra lo que queda de ventana.
- **Estilo** (se suma y también se multiplica por el combo): noqueo con puerta +200, noqueo con lanzamiento +200,
  remate +100; y al final, planta sin disparar +2000.
- **Tiempo**: (par − t) × 50 por segundo, si sale positivo. Par de la planta 1: 60 s.
- **Nota**: puntos / R, donde R es la puntuación de referencia medida con el recorrido grabado del diseñador:
  ≥100 % S, ≥80 % A, ≥60 % B, ≥40 % C y, por debajo, D. Cada intento empieza de 0; se guarda la mejor nota.

## 5. Planta 1 — "LOBBY" (tutorial). 36×22 tiles = 576×352 px (unas 2,4 × 2,2 pantallas)
```
          0         1         2         3
          012345678901234567890123456789012345
       0  ####################################
       1  #......#.........#................X#
       2  #......#.........#............7....#
       3  #..P...D...1>....#..........=====..#
       4  #......#.........#.................#
       5  #......#......p..#..............6..#
       6  ########.........#.................#
       7  ############D#####.................#
       8  ###########...####.................#
       9  ###########...####.................#
      10  ###########...####.5...............#
      11  ###########...####.................#
      12  ###########...####.................#
      13  ###########wDw##########D###########
      14  #.......2..........#.........#######
      15  #.....=............#.4.......#######
      16  #.....=............#.........#######
      17  #...3.=............D.........#######
      18  #.....=............#.........#######
      19  #.....=............#.........#######
      20  #..................#.........#######
      21  ####################################
```
Leyenda: `P` inicio · `D` puerta · `w` ventana (deja ver, no deja pasar ni a las balas) · `=` barra o mesa (no deja
pasar a nadie, pero sí a la vista y a las balas) · `p` pistola con 4 balas · `X` salida · números = enemigos.

| Tramo | Sala | Contenido | Regla |
|---|---|---|---|
| Presentar | A: entrada (6×5) | segura y sin enemigos; `D-PAD MOVE  L/R AIM` | 3, 6, 10 |
| Presentar | B: guardarropa (9×6) | **1** Goon quieto en (11,3) mirando al este, de espaldas a la puerta; se le noquea con el puño y se le remata; pistola en (14,5) | 3, 5 |
| Desarrollar | C: pasillo (3×5) | por las ventanas de (11,13) y (13,13) se ve a **2** Goon patrullar la fila 14 entre x=8 y x=16, cruzando por delante de la puerta; si empujas la puerta cuando pasa, queda noqueado | 6: se enseña con el espacio |
| Girar | D: pista de baile (18×7) | **3** Gunman en (4,17), detrás de la barra, mirando al este; desde la puerta está a 137 px (fuera de su vista de 112, pero en tu pantalla). Al disparar le llega el ruido a **4** Goon (21,15), en la cocina E, que entra por (19,17); el jugador aprende que el ruido atrae. Con las 4 balas puede quedarse sin munición → `B: THROW` | 5, riesgo y recompensa |
| Cerrar | F: despacho (17×12) | **5** Goon patrulla la fila 10 entre x=19 y x=27; **6** Goon patrulla la columna 32 entre y=4 e y=11; **7** Gunman en (30,2), detrás de la mesa, mirando al sur. Se entra por (24,13), a 200 px de 7. Junta puerta, lanzamiento y disparo. Salida en (34,1) | 3: examen de todo |

7 enemigos (el máximo es 12). Tiempo esperado: 60–90 s la primera vez (con 2–4 muertes, sobre todo en D y F), 30–40
s cuando ya te la sabes; par de 60 s. Tras 10 s sin matar, una flecha en el borde señala al enemigo vivo más cercano
(regla 9). Pausas seguras (regla 4): A, C y la cocina E una vez vacía.

## 6. Primer minuto y muerte (regla 10)
- 0–5 s: tarjeta `FLOOR 1 - LOBBY` (45 f, se salta con A) → empiezas en A, con el puño y nada cerca. 5–20 s: noquear y
  rematar a 1, coger la pistola. 20–40 s: la puerta y 2. 40–60 s: la barra, 3 y 4.
- **Muerte**: 30 f a cámara lenta con la pantalla en rojo y `YOU'RE DEAD  A: RETRY`. A responde a partir del frame 20
  (para no reintentar sin querer mientras disparas) y la planta se reinicia con un fundido de 10 f. **Coste total: menos
  de 1,5 s.** En juego, el reinicio va en START → RETRY. Se descarta R+A, porque R+A es girar y disparar a la vez.
- **Se conserva**: el número de muertes de la planta, la mejor nota y los rótulos ya vistos. **No se conserva**:
  puntos, armas ni el estado de los enemigos (la planta vuelve a empezar desde 0). El punto de control es el inicio de
  la planta, y por eso las plantas duran 90 s como mucho.

## 7. Bloques (alcance pequeño)
| Bloque | Contenido | Aceptación |
|---|---|---|
| **1** | planta 1 en greybox (cuadros de colores, texto con `bn::sprite_text_generator` y una fuente de `common`), puño y pistola, lanzar, Goon y Gunman, puertas, noqueo y remate, cámara con adelanto, reintento, puntos y combo (sin nota) | `gbarun.py` con entrada grabada que limpia la planta en ≤ 90 s; la captura muestra el `!` antes de cada disparo enemigo; reintento < 1,5 s |
| 2 | bate y escopeta, estilo, tiempo y nota; plantas 2–3 (sótano del club, casino) | nota S–D en la tarjeta del final |
| 3 | arte (sprites de 16×16 dibujados en 16 direcciones, paleta cálida), música y efectos propios, Dog | revisión del primer minuto con el arte final |
| 4 | plantas 4–5 (almacén del puerto) y el jefe (el dueño del casino, que examina la puerta, el lanzamiento y el ruido; regla 11), título, guardado de notas, publicación | kit de `gba-release` |

**No entra**: máscaras o mejoras, historia con escenas, más de 4 armas, vehículos, varios personajes. La opción "la
mira sigue a la cruceta" sería una tarea P4, sólo como opción (regla 14).

## Decisiones de diseño
| Decisión | Principio | Métrica |
|---|---|---|
| El puño noquea y lo demás mata | andamiaje, riesgo y recompensa | el 70 % de los jugadores de prueba rematan sin que se lo expliquen más que con el rótulo |
| Mira continua (5,625° por toque, 6°/f manteniendo) con ayuda de ±22,5° | regla 2 | menos de 1 de cada 5 disparos del jugador fallan a 64 px o menos en la entrada grabada |
| Los enemigos sólo disparan en pantalla y tras 30 f | regla 5 | 0 muertes en las que el que dispara no aparezca en la captura de los 30 f anteriores |
| Reiniciar con A tras morir; nunca con R+A | reglas 10 y 14 | reintento < 90 f medido con `gbarun.py` |
| (Usuario, 2026-11-23) Mira continua: L/R giran 6°/f manteniendo y 5,625° por toque; jugador y enemigos rotan por hardware (sprites afines), un solo dibujo por personaje | flujo e inmediatez: el giro a saltos de 22,5° no se sentía fluido | ≤ 13 matrices afines en uso (jugador + vivos); 180° en ~30 f |
| (Usuario) Gore: partículas de sangre en la dirección del golpe, manchas persistentes en una capa de fondo, charcos bajo cadáveres, hitstop 3 f y temblor de cámara | cada acción tiene respuesta (checklist del primer minuto); recompensa | 60 fps con 7 enemigos + 24 partículas |
| (Usuario) Vista cenital total, sin perspectiva: muros, puertas, muebles, personajes y manchas vistos desde arriba, sin caras laterales ni sombras proyectadas | lenguaje visual coherente; legibilidad de colisiones (lo que se ve es lo que choca) | ningún tile o sprite con cara frontal en la revisión de arte |
| (Revisión 1.er minuto) Ruido por camino: pistola 14 tiles, puerta 4 | regla 3 (D presenta "el ruido atrae" con un solo enemigo); regla 4 (la cocina E sigue siendo pausa) | con la ruta, al disparar a 3, el 4 investiga y el 5 no cambia |
| (Revisión) Visión cercana 16 px < alcance del puño 20 px; `A: PUNCH` sin exigir enemigo desprevenido y ≥ 60 f en pantalla | reglas 2 y 6 | en la ruta no hay `!` sobre el matón 1 antes del golpe y `A: PUNCH` se ve el frame anterior |
| (Revisión) Margen de disparo de 24 px | regla 5 | el que dispara está a ≥ 24 px de cada borde en el primer frame de apuntar |
| Otras cifras reales: golpe del matón 20 px; el pistolero no suelta el blanco hasta 160 px | medido en la ROM (regla 13) | — |

## Riesgos
- **Rotación**: Butano tiene 32 matrices afines, que bastan para 13 sprites en el greybox, pero el pixel art rotado por
  afín se ve mal. Para el arte final: 5 frames dibujados (0°, 22,5°, 45°, 67,5° y 90°) más volteos dan las 16
  direcciones.
- **CPU**: 12 rayos de visión por frame; se reparten a 3 por frame. Tope de 16 balas por bando.
- **Dificultad de la planta F**: 3 enemigos a la vez; si la entrada grabada muere ahí más de 3 veces de cada 5, se
  quita el 6.
- **Alcance**: el bloque 1 no lleva arte ni audio; si se alarga más de lo previsto, se recorta la puntuación antes que
  el verbo (regla 1).
