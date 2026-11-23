# DELVE — diseño

**Frase**: bajas por una mazmorra oscura generada con semilla, casilla a casilla, y luchas en tiempo real con
monstruos que avisan antes de golpear. **Verbo central**: *esquivar y castigar* (apartarse del golpe que se ve
venir y contraatacar). Referencia de género: Dungeon Hack / Eye of the Beholder (vista por casillas, tiempo real).
Las reglas y los números son propios, sin nada de D&D. Todo el texto del juego va en **inglés**. La pantalla, la vista
y los formatos están en `ESPEC_VISTA.md` (no se cambian aquí). Reglas citadas: R1–R14 de `docs/guias/diseno-juegos.md`.

> Magia, tesoros, equipo, consumibles e inventario: `MAGIA_Y_TESOROS.md` (G-39; cambia SELECT/B de §1 y amplía §5).

## 1. Controles (R14: inmediatos, sin combinaciones)
| Botón | En la mazmorra | En inventario / mapa |
|---|---|---|
| Arriba / Abajo | avanzar / retroceder una casilla | mover el cursor |
| Izquierda / Derecha | girar 90° | mover el cursor |
| L / R | paso lateral izq. / der. (opcional, muy útil contra los conjuros) | L = tirar objeto, R = asignar a B |
| A | atacar con el arma (si la barra está llena); al estar frente a una puerta, la abre o la cierra | usar / equipar |
| B | usar el objeto rápido (por defecto, la poción) | cerrar |
| START | inventario (pausa) | cerrar |
| SELECT | mapa completo de la planta (pausa) | cerrar |

- Paso: la vista se redibuja en el frame siguiente a la pulsación (sin animación de paredes). Recarga del paso **12 f**
  (5 casillas/s), del giro **8 f**. Mantener pulsado repite. Búfer de **1 pulsación** durante la recarga (R2).
- Andar contra una puerta cerrada la abre (R14): A sólo hace falta para cerrarla. Andar contra una puerta con
  llave usa la llave sola. Los objetos se recogen al pisar la casilla, sin botón.
- Sin la cruceta diagonal: la diagonal se ignora (manda el primer eje pulsado) para no girar sin querer.
- **Cómo se enseñan** (R6: una línea en la franja inferior, cuando hace falta, sólo la primera vez de cada partida):

| Disparador | Mensaje |
|---|---|
| inicio de la partida | `UP: WALK   LEFT/RIGHT: TURN` |
| primer monstruo visible | `A: ATTACK WHEN BAR IS FULL` (26 car.; la versión anterior tenía 30 y no cabía en los 28 de la franja) |
| primer aviso de ataque enemigo | `STEP AWAY TO DODGE!` |
| primer objeto recogido | `B: QUICK ITEM   START: BAG` |
| primer conjuro enemigo visto (planta 2) | `L/R: SIDESTEP` |
| primera puerta con llave | `LOCKED. FIND A KEY.` |

## 2. Personaje
Un solo personaje (sin creación ni clases: R14). **3 estadísticas**:

| Estadística | Inicial | Efecto |
|---|---|---|
| HP (vida) | 30 | 0 = muerte |
| STR (fuerza) | 2 | se suma al daño del arma |
| DEF (defensa) | 0 | se resta a cada golpe recibido (más la armadura) |

- Experiencia (XP) para subir: nivel 2 = 10, 3 = 25, 4 = 45, 5 = 70, 6 = 100, luego +35 por nivel. Tope: nivel 10.
- Cada nivel: **+6 HP máx., +1 STR**, y **+1 DEF en niveles pares**; cura la vida entera (alivio tras la tensión).
  Respuesta: destello dorado y `LEVEL UP!` en la franja.
- Curva esperada: nivel 3 al acabar la planta 1, 5 en la 2 y 6–7 ante el jefe (se medirá; ver métricas).

## 3. Combate en tiempo real
Sólo se golpea al monstruo de la casilla de enfrente (d = 1). Un monstruo por casilla.

| Regla | Valor | Principio | Métrica |
|---|---|---|---|
| Daño del jugador | arma + STR − DEF del monstruo, mínimo 1. Sin tiradas de fallo: todo golpe a d = 1 acierta | respuesta clara a cada acción | 0 golpes "sin efecto" en las pruebas |
| Recarga | barra en la franja inferior; se llena en la recarga del arma (30–72 f). A con barra vacía: se guarda si faltan ≤ 8 f (R2) | ritmo legible, riesgo/recompensa (armas lentas pegan más) | tiempo medio por combate 4–10 s |
| Esquiva | el monstruo golpea **la casilla** donde estabas al empezar su aviso; si al acabar el aviso ya no estás, falla (`MISS` sobre él) | anticipación (R5), verbo central | ≥ 60 % de esquivas tras la 1.ª planta |
| Crítico | 10 % al azar, ×2. **Contraataque**: golpe en los 30 f siguientes a una esquiva = crítico seguro | riesgo/recompensa: premia esquivar y volver | % de críticos por contraataque |
| Daño recibido | daño del monstruo − DEF total, mínimo 1; borde de la vista en rojo 6 f y número sobre el HP | respuesta | — |
| Invulnerabilidad | 20 f tras recibir un golpe | perdón (R2) | sin muertes por golpes encadenados |

- **Aviso de ataque (R5)**: el sprite pasa al frame de anticipación (2 frames de "ataque" de `ESPEC_VISTA.md`),
  parpadea en blanco cada 4 f y se acerca un 10 % de escala; después golpea. Sonido de aviso propio por monstruo en el
  bloque 3; en los bloques 1–2 basta un pitido corto de Butano (ver riesgos).
- **Ataques por el lado o la espalda**: un monstruo adyacente que no está enfrente puede atacar, pero su aviso dura
  **×1,5** y aparece una flecha roja parpadeante en el borde de la vista por ese lado, más el punto en el minimapa.
  Nada pega desde donde no se ve sin avisar (R5).
- **Movimiento**: cada monstruo avanza una casilla cada *N* frames (tabla), con la escala y la posición
  interpoladas en 12 f. Persigue en línea recta por la rejilla (el paso que más acorta la distancia, sin atravesar
  puertas cerradas). Se activa al verte (línea de vista ≤ 3 casillas) o al oírte pelear a ≤ 4 casillas; si no, se
  queda quieto o da vueltas por su sala. Nunca aparece a menos de 6 casillas del jugador (R5).

## 4. Monstruos (plantas 1–3)
Recarga = frames entre el final de un ataque y el comienzo del aviso siguiente.

| Monstruo | Planta | HP | DEF | Daño | Mueve cada | Aviso | Recarga | Comportamiento | XP | Botín | Principio |
|---|---|---|---|---|---|---|---|---|---|---|---|
| Rat | 1 | 6 | 0 | 3 | 30 f | 24 f | 70 f | persigue y muerde | 3 | 20 % pan | se presenta solo, andamiaje |
| Bat | 1–2 | 5 | 0 | 2 | 18 f | 18 f | 60 f | golpea y se aleja 2 casillas; vuelve | 3 | — | ritmo nuevo: espera a que vuelva |
| Slime | 1–2 | 16 | 0 | 4 | 60 f | 36 f | 80 f | lento, tapa pasillos; se puede rodear | 5 | 30 % poción | aguante, no reflejos |
| Skeleton | 2–3 | 20 | 1 | 6 | 30 f | 30 f | 90 f | persigue; enseña el contraataque | 10 | 25 % arma/armadura | desarrollar la esquiva |
| Cultist | 2–3 | 12 | 0 | 5 (conjuro) | 36 f | 40 f (manos que brillan) | 120 f | se queda a 2–3 casillas; lanza una bola de fuego en línea recta que avanza 1 casilla cada 8 f; si llegas a d = 1, huye | 12 | 40 % pergamino | giro: esquivar de lado (L/R) |
| **Grave Warden** (jefe) | 3 | 120 | 2 | 10 | 40 f | 36 f | 80 f | fase 1: golpe frontal. Fase 2 (≤ 50 % HP): barrido que cubre la casilla de enfrente y las dos laterales (sólo esquiva retroceder), aviso 30 f, brazos abiertos (frame distinto) | 60 | llave del final | examen de lo aprendido (R11) |

- Cada planta presenta **un tipo nuevo** primero a solas, en un pasillo, antes de mezclarlo (R3).
- Botín y monstruo derrotado: el monstruo se desvanece en 16 f y el objeto cae en su casilla.

## 5. Objetos
**Hambre: no.** Obligaría a una barra y a una gestión más sin aportar al verbo central (R14). La comida es
curación barata. La antorcha va en la mano izquierda (decorado de la franja) y no se gasta, por lo mismo.

| Arma | Daño | Recarga | Dónde | Principio |
|---|---|---|---|---|
| Dagger | 3 | 30 f | inicial | rápida, perdona |
| Short Sword | 5 | 40 f | planta 1–2 | primera mejora pronto (gradiente de meta) |
| Mace | 8 | 56 f, ignora 1 DEF | planta 2–3 | contra esqueletos |
| Battle Axe | 11 | 72 f | planta 3, sala cerrada | riesgo/recompensa: pega fuerte y deja expuesto |

| Armadura | DEF | Dónde |
|---|---|---|
| Leather | +1 | planta 1 |
| Chain Mail | +3 | planta 2 |
| Plate | +5 | planta 3, sala cerrada |

| Consumible | Efecto | Pila máx. |
|---|---|---|
| Bread | +10 HP | 9 |
| Healing Potion | +50 % del HP máx. | 9 |
| Iron Key | abre una puerta con llave de la planta (cualquiera, R7); sobra al bajar → se pierde | 9 |
| Scroll of Fire | 15 de daño al primer monstruo en línea recta (hasta d = 3) | 9 |
| Scroll of Sight | revela el mapa de la planta y las escaleras | 9 |

- Usar el objeto rápido (B) tarda 20 f sin poder atacar (riesgo: no se bebe gratis en mitad de un golpe).
- Armas y armaduras mejores se equipan solas al recogerlas si mejoran el daño o la DEF (R14); la otra va a la bolsa.
- **Inventario** (START, juego en pausa) sobre la vista 176×120: rejilla **4×3 = 12 huecos** de 32×32 (iconos de
  16×16), dos huecos de equipo (arma, armadura) en el panel derecho y una línea en la franja con nombre y efecto del
  objeto. Cruceta mueve el cursor, A usa/equipa, R lo pone en B, L lo tira, B o START cierra. Los objetos iguales
  se apilan. Bolsa llena: `BAG FULL` y el objeto se queda en el suelo.
- **Mapa** (SELECT): planta entera 32×32 a 3 px = 96×96, centrada en la vista; casillas vistas, puertas, escaleras
  y la flecha del jugador. El minimapa del panel muestra 15×15 casillas alrededor a 4 px (60×60).

## 6. Generador (semilla)
**Forma** (la porta el motor desde el generador v2 del usuario con las recomendaciones de su estudio; aquí sólo se
fija lo que afecta al juego). **Corregido tras leer la v2**: la rejilla es de **32×32 casillas con las paredes en los
lados** (cada arista guarda tipo + variable de 4 bits: WALL/OPEN/DOOR/LOCKED/SECRET/ARCH/BARS/STAIRS), no bloques 1:1
de roca y suelo. Lo no tallado es roca maciza (4 lados WALL, sin decoración). Banda 0 de la v2 (la de las plantas
1–3): objetivo **28 %** tallado (validación 18–42 % → **~290 casillas de suelo** de 1024), **2–9 callejones**,
disposición ROOMS con 4–7 salas de lado 3–5, rectitud de pasillos 88 %, conectividad garantizada sin teleportadores
y la llave nunca más honda que su cerradura. La entrada la elige la v2 en un callejón al azar (no hereda las
coordenadas de la escalera anterior: la bajada es de un solo sentido). Zona por planta en el bloque 1–2: **salas +
pasillos** en las plantas 1–3 (`maze_pct = 0` en sus tablas); el laberinto denso (78 %) y el cruce quedan para
plantas 4+ (bloque 3), como "giro" de una planta suelta (R3).

- Semilla de partida de 32 bits (visible en pausa y en la pantalla de muerte); semilla de planta = hash(partida,
  planta). Misma semilla = misma planta (R13: entrada reproducible).
- **Zona segura** (R4; corregido: la v2 no talla una sala 3×3 a medida y forzarla rompería su formato): la llegada es
  el callejón de entrada de la v2, mirando a su única abertura; sin monstruos a menos de **8 casillas por recorrido**
  (la v2 usa `SPAWN_CLEAR = 6`; la tabla del juego lo sube a 8) ni con vista directa (≤ 3 casillas en línea) desde
  las 3 primeras casillas, y sin trampas a menos de 6. Escalera de bajada: la regla de la v2 (callejón al azar del
  cuarto más lejano por recorrido, no siempre el más lejano, para que no sea predecible).
- **Llaves (encima de la regla de profundidad del generador)**: puertas con llave sólo en salas o callejones
  **opcionales** (tesoro); el camino entrada → escalera (y → jefe) nunca cruza una puerta con llave (R9, R12).
  Llaves genéricas de planta (R7) y llaves ≥ cerraduras. Cada cerradura se ve antes que su llave cuando el orden lo
  permite (R7).
- Validación al generar (la de forma la trae el generador; ésta es la de juego): escalera alcanzable sin llaves,
  sala segura limpia, ningún objeto ni monstruo dentro de la roca. Si falla, semilla + 1 (máx. 8 intentos).

**Reparto por zona dentro de la planta** (distancia por recorrido desde la entrada, en tercios del máximo):

| Zona | Monstruos | Objetos | Principio |
|---|---|---|---|
| Cerca (1.er tercio) | 25 %, sólo el tipo más débil de la planta; el tipo nuevo, solo y en pasillo | 40 % (curación sobre todo) | andamiaje, flujo |
| Media | 40 %, mezcla de 2 tipos | 30 % | desarrollar |
| Lejos (incluye escalera y salas con llave) | 35 %, los más fuertes; grupos de 2 como máximo | 30 % (equipo, tras llave) | riesgo/recompensa (R12) |

- Monstruos colocados en salas (60 %) y en tramos rectos largos (40 %): los cultists, siempre en tramos rectos de
  ≥ 5 casillas, para que la bola de fuego se vea venir (R5). Ningún monstruo en un callejón de 1 casilla (sin salida
  para esquivar).

| Planta | Monstruos | Tipos | Objetos | Cerraduras | Nuevo |
|---|---|---|---|---|---|
| 1 | 9 | rat ×4, bat ×3, slime ×2 | 7 (3 pan, 2 pociones, short sword, leather) | 0 | rat solo en el 1.er pasillo |
| 2 | 12 | bat ×2, slime ×2, skeleton ×5, cultist ×3 | 8 (+ mace o chain, 1–2 pergaminos, 1 llave por cerradura) | 1 | skeleton solo, luego cultist |
| 3 | 13 + jefe | skeleton ×6, cultist ×4, slime ×2, bat | 8 (+ axe/plate tras llave) | 1 | jefe en sala 5×5 antes de la escalera |

- Densidad ≈ 1 monstruo cada **25–35** casillas de suelo (corregido: ~290 casillas, no 480): con paso de 12 f, ~1
  encuentro cada 12–18 s de exploración, con silencios entre medias (R4). Las cifras de la tabla son por planta (una
  banda por planta en la demo) y sustituyen a las tiradas de la banda 0 de la v2. Cerraduras: la v2 no pone ninguna
  en sus 2 primeras plantas (`lock_free = 2`); el juego usa `lock_free = 1` → planta 1: 0, plantas 2 y 3: 1.
- **Dientes de sierra**: dentro de la planta la dificultad sube con la zona y baja al empezar la siguiente (sala
  segura + subida de nivel probable). Antes de la sala del jefe, una sala vacía con 1 poción garantizada (R4, R10).
- Riesgo de forma: con ~290 casillas y pasillos rectos se esperan plantas de ~4–6 min. Si el bot mide > 8 min por
  planta, bajar `open_pct`, no el tamaño; si < 3 min, subirlo hasta 32.
- Mapa (SELECT) y minimapa: las paredes se dibujan como líneas de 1 px en el borde de la casilla (3 px = 2 de suelo +
  1 de pared compartida), rejas en gris claro discontinuo, palancas como punto en el lado.

## 6b. Interruptores, trampas y atrezzo
**Cómo encaja en la v2** (no se toca su formato ni su orden de azar): la v2 ya define BARS (bloquea el paso, no la
vista), suelos de trampa en `dun_c` bits 0–2 (1 spike, 2 spin, 3 alarm, 4 tele; 5–7 libres), entidades TRAP y FURN
(`FURN: a = rnd(4), b = rnd(8)`) y la variable de 4 bits de cada WALL/DOOR/ARCH/SECRET. **No coloca rejas ni
interruptores**. El juego añade una fase **posterior** ("ganchos de juego") con el segundo flujo de azar que la v2 ya
reserva (`dunRngGame`), así la salida de la v2 sigue siendo idéntica bit a bit y sus pruebas de CRC valen. Los
ganchos: (1) reinterpretan por planta el tipo de trampa, el tipo de mueble y la variable de pared con tablas de 8/16
entradas; (2) añaden rejas e interruptores con las plantillas de abajo; (3) quitan o cambian lo que incumpla las reglas
duras (no reintentan la planta). Nuevas entidades de juego: `LEVER` (casilla, lado, arista objetivo), `PLATE`
(casilla, arista objetivo) y, en el bloque 3, `CRATE`.

### Interruptores
Un solo símbolo: **los interruptores sólo mueven rejas (BARS)**; nunca puertas ni cerraduras (R7: un símbolo por
mecanismo; R14). La reja deja ver lo que hay detrás (cerradura antes que llave, R7). Uso: **andar contra la palanca la
acciona** (como las puertas, R14); A también vale. La placa se activa al pisarla.

| Interruptor | Se ve | Efecto | Se queda | Temporizador | Principio |
|---|---|---|---|---|---|
| Palanca de pared (`LEVER`) | palanca en la cara de una pared, arriba = cerrada, abajo = abierta; brillo dorado suave hasta usarla | abre su reja | **fija**: no se vuelve a cerrar (no puede cerrarte un paso, R9) | no (bloque 3: palanca con tiempo, sólo en salas opcionales) | R7, R9, R14 |
| Placa de presión (`PLATE`, suelo 6) | losa elevada con borde claro, distinta de la trampa (sin agujeros) | abre su reja **mientras hay peso**: jugador, monstruo, caja u objeto tirado (L) | vuelve: se cierra **120 f** después de quedar libre (aviso: la reja tiembla y suena 2 veces en los últimos 40 f) | sí, 120 f | anticipación (R5), perdón (R2) |

**Lectura del enlace** (R5, R6, R7):
- **Regla dura del gancho**: desde la casilla del interruptor, la reja está a **≤ 3 casillas en línea recta sin
  arista que bloquee la vista** (basta girar para verla); desde la reja se ve el interruptor igual. Si no, no se pone.
- Al accionar: la reja sube en 16 f, sonido metálico propio y destello de la reja si está en la vista. La primera vez
  de la partida, una línea: `A GATE OPENED` (palanca) o `GATE OPEN WHILE PRESSED` (placa). Primera reja vista con su
  interruptor sin usar: `BARS: FIND THE LEVER`. Todas ≤ 28 caracteres.
- Mapa y minimapa muestran reja abierta/cerrada y la palanca (R9: el estado se ve).

**Plantillas generables** (todas resolubles por construcción; el gancho las valida con el comprobador de la v2 tratando
BARS como pared):

| Plantilla | Construcción | Por qué es resoluble | Planta / bloque | Principio |
|---|---|---|---|---|
| P1 Atajo | Una arista **WALL** entre dos casillas talladas *a* y *b* a **≥ 12 pasos** por recorrido (como los muros secretos de la v2) pasa a BARS. La palanca va en una pared de *b* (la más lejana de la entrada) perpendicular a la reja: mirando la palanca, la reja es la pared lateral de tu casilla, a la vista. Desde *a* se ve la palanca a través de la reja | antes era una pared: quitarla nunca cambia la conectividad | 1 (bloque 1) | R8 (atajo), R7 |
| P2 Sala con reja | La única abertura (DOOR/ARCH/OPEN) de una sala o callejón **opcional** (sin escalera, entrada, llave ni cerradura dentro; 4–20 casillas) pasa a BARS; palanca en la parte de fuera, a ≤ 3 casillas en línea de la reja. Dentro, 1 objeto garantizado (equipo o poción) | la palanca está en el lado alcanzable; dentro no hay nada necesario | 2 (bloque 2) | R12, R7 |
| P3 Placa y carrera | P2, pero con placa en vez de palanca, a **≤ 4 pasos** de la reja: 4 × 12 f = 48 f ≪ 120 f (margen ×2,5) | se puede pasar sólo con andar; objeto o caja son atajos, no requisito | 3 (bloque 3) | R2, R5 |
| P4 Placa y caja | P3 con la placa a > 4 pasos y una `CRATE` empujable (andar contra ella la mueve 1 casilla si la de detrás está libre) en la misma sala, en línea recta con la placa. Si la caja queda donde ya no puede llegar a la placa, vuelve a su sitio al alejarte ≥ 6 casillas | la caja se repone (R9); además sigue valiendo tirar un objeto (L) sobre la placa | 3+ (bloque 3) | giro (R3), R9 |
| P5 Dos palancas | Reja de P2 que pide **dos** palancas en ramas distintas, ambas fuera; tras la primera: `1 OF 2` | igual que P2 | 3+ (bloque 3) | desarrollar (R3) |

| Planta | Interruptores | Zona (tercios de distancia, §6) |
|---|---|---|
| 1 | 1 × P1 | palanca (*b*) en media o lejos; *a* más cerca: el atajo acorta la vuelta |
| 2 | 1 × P1 + 1 × P2 | P2 en media |
| 3 | 1 × P1 + 1 × P3 (bloque 3: + P4 o P5) | P3 en media, nunca en la sala previa al jefe |
| Máx. | 3 por planta; ninguno a < 6 casillas de la entrada ni en la casilla de la escalera | — |

- **Regla dura (R9)**: ninguna reja de interruptor está en el camino entrada → escalera (→ jefe), ni guarda una llave
  o una cerradura. Si el gancho no encuentra sitio válido, la planta va sin esa plantilla (nunca se reintenta).
- Al morir y reintentar la planta, los interruptores vuelven a su estado inicial (toda la planta se regenera igual).

### Trampas
La v2 pone las trampas sólo en **pasillos de paso** (casillas con 2 aberturas, fuera de salas, a ≥ 4 de la entrada) y
escribe `1 + t` (t = 0..2) en el suelo. El juego reinterpreta *t* por planta (tabla) y añade el foso con un gancho.
"Spin" y "alarm" de la v2 **no se usan** (girar al jugador sin verlo rompe R5).

| Trampa | Aviso (R5: se ve antes) | Daño (ignora DEF) | Esquivar / desactivar | Primera vez | Puede ir en paso obligado | Principio |
|---|---|---|---|---|---|---|
| Placa de pinchos (suelo 1) | losa con agujeros visible a d ≤ 3; ciclo **bajados 90 f → asoman 24 f (clic) → arriba 36 f** | 4 / 6 / 8 (planta 1/2/3) | pasar con los pinchos bajados (2 pasos de 12 f caben de sobra en 90 f); no se desactiva | planta 1, zona cerca: tramo recto con ≥ 2 casillas antes, sin monstruos a < 4; rótulo `WAIT FOR THE SPIKES TO DROP` | **sí** (se pasa con el ritmo) | anticipación, ritmo legible |
| Dardo de pared (suelo 2) | losa hundida con borde oscuro + rendija en la pared lateral de esa casilla (decoración fija); al pisar, clic y la rendija brilla **20 f** | 6 | no detenerse: el siguiente paso (12 f) sale antes de los 20 f; el búfer de 1 pulsación ayuda (R2). Se gasta tras **3** disparos | planta 2, zona cerca o media, sin monstruos a < 4; rótulo `KEEP MOVING ON PLATES` | **sí** (basta seguir andando) | anticipación, perdón |
| Runa explosiva (suelo 3) | runa roja que late (lenguaje de color: peligro) a d ≤ 3; al pisarla o golpearla a d = 1, mecha **30 f** con brillo creciente | 10 en su casilla y las 4 vecinas; también a monstruos (15) | alejarse 2 casillas (24 f < 30 f); se desactiva haciéndola estallar a propósito (A a d = 1 y retroceder) o con Scroll of Fire; un solo uso | planta 3, zona media; rótulo `RUNE! BACK OFF` | no (sólo con rodeo) | riesgo/recompensa: atraer monstruos a la runa |
| Foso (suelo 5, gancho) | losa agrietada visible; al pisarla cruje y tiembla **30 f** y se abre | 8, y vuelves a la casilla de la que venías | salir antes de 30 f; abierto queda como agujero **que bloquea el paso para siempre** | bloque 3 (planta 4+) | **nunca**: sólo en casillas que no sean de articulación (quitarla no desconecta escalera, llaves ni cerraduras) | giro (R3), R9 |

| Planta | t = 0 | t = 1 | t = 2 | Trampas por planta | Reparto por zona (cerca / media / lejos) |
|---|---|---|---|---|---|
| 1 | pinchos | pinchos | pinchos | **2** (bloque 1: 1) | 1 (la de presentación) / 1 / 0 |
| 2 | pinchos | dardo | dardo | 3 | 1 dardo de presentación / 1 / 1 |
| 3 | pinchos | dardo | runa | 4 | 0 / 2 (runa de presentación) / 2 |

**Reglas duras del gancho** (si no se cumplen, la trampa se quita; la planta no se reintenta):
- Nunca en la casilla de llegada ni a < 6 casillas de ella; nunca en la casilla de la escalera, de un interruptor, de
  un objeto ni delante (la casilla de uso) de puerta, reja, palanca o escalera.
- Nunca en un **recodo**: las dos aberturas de la casilla en línea recta, para que se vea a d ≥ 1 antes de pisarla.
- En una casilla de paso obligado (articulación) sólo pinchos y dardo, los que se pasan con ritmo o andando.
- Planta 1–2: ningún monstruo colocado a < 4 casillas de una trampa (monstruo + trampa es el giro de la planta 3).
- Ninguna dos trampas seguidas: ≥ 4 casillas entre trampas (R4, silencios).

### Atrezzo
**De pared**: lo elige la variable de 4 bits que la v2 ya pone en cada WALL/DOOR/ARCH, leída con una **tabla de 16
entradas por planta** (identidad, R8). Se dibuja como variante de textura proyectada por `build_view.py`; no bloquea
nada. Fijas (no salen de la tabla): grieta = SECRET, rendija = dardo, palanca = LEVER.

| Pared | Dónde | Lectura | Principio |
|---|---|---|---|
| Antorcha | todas; más en la 3 | luz cálida, hito para orientarse | hitos por zona |
| Estandarte | planta 3 | sala del guardián cerca | R8 identidad |
| Cadenas | plantas 2–3 | ambiente | R8 |
| Calaveras (muro con cráneos) | planta 2 | osario | R8 |
| Nicho | planta 2 | hueco con huesos; decorado | R8 |
| **Grieta** | **toda arista SECRET y sólo ellas** (las dos caras) | pista fiable de muro secreto: andar contra ella lo abre | R12 (premia mirar), R7 (símbolo consistente) |

Tabla de pared por planta (16 entradas de la variable; el resto, piedra lisa en 2 variantes):

| Planta | Lisa | Antorcha | Cadenas | Calaveras | Nicho | Estandarte |
|---|---|---|---|---|---|---|
| 1 "The Cellars" | 15 | 1 | 0 | 0 | 0 | 0 |
| 2 "The Ossuary" | 11 | 1 | 1 | 2 | 1 | 0 |
| 3 "Warden's Halls" | 11 | 2 | 1 | 0 | 0 | 2 |

Con ~290 casillas y ~2,2 paredes talladas por casilla, 1/16 da **~40 antorchas por planta** (una cada ~7 casillas de
recorrido): sirve de hito sin empapelar. Sobre la arista de una palanca, reja o escalera la variable se ignora.

**De suelo**: entidades FURN de la v2 (tipo `b = rnd(8)` leído con una tabla de 8 entradas por planta; `a = rnd(4)`
= orientación del dibujo). Sprites de 32×32 escalados como los objetos (`ESPEC_VISTA.md`).

| Mueble | Bloquea paso | Bloquea vista de monstruos | Romper / abrir | Botín | Principio |
|---|---|---|---|---|---|
| Barril | sí | no | 1 golpe (A a d = 1) | 30 %: pan 70 % / poción 30 % | recompensa variable |
| Caja | sí | no | 2 golpes | 40 %: pan 50 % / poción 40 % / pergamino 10 % (planta 2+) | recompensa variable |
| Huesos | no | no | — | — | ambiente, R8 |
| Charco | no | no | — | — | ambiente |
| Estatua | sí | sí | no | — | hito, cobertura contra cultists |
| Mesa | sí | no | no | — | ambiente |
| Cofre | sí | no | abrir andando contra él (R14) | **siempre 1 objeto** de la tabla de la planta (cuenta dentro de los objetos de §6, no los suma) | R12 |

| Planta | Tabla de 8 (b = 0..7) | Por sala | Pasillos | Callejones |
|---|---|---|---|---|
| 1 | barril ×3, caja, huesos ×2, charco ×2 | 1 + área/6 (máx. 4) | 1 no bloqueante cada ~12 casillas (huesos, charco) | 40 %: barril |
| 2 | huesos ×3, caja, barril, estatua, cofre, charco | 1 + área/5 (máx. 5) | ídem (huesos) | 40 %: barril o cofre (1 cofre máx.) |
| 3 | estatua ×2, mesa, cofre, huesos ×2, barril, charco | 1 + área/5 (máx. 5) | ídem | 40 %: barril o cofre (1 cofre máx.) |

La v2 tira pocos muebles (banda 0: 60 % de 1–3); la tabla del juego fija la cantidad por sala de arriba.

**Reglas duras del atrezzo** (el gancho cambia el mueble a uno no bloqueante o lo quita):
- **Bloqueante sólo en salas** y nunca en una casilla de articulación; dentro de la sala, siempre queda un camino
  entre todas sus aberturas (BFS). Barriles en callejones sí (se rompen), estatua/mesa/cofre en pasillo nunca.
- Nada en la casilla de uso de puerta, reja, palanca, escalera ni en la casilla vecina a una abertura de sala; nada
  sobre placa ni trampa (taparía el aviso, R5); nada en la casilla de llegada.
- Zona segura: 1 barril con **pan garantizado** en una casilla a ≤ 3 de la llegada que no sea de paso (rincón de sala o callejón lateral; si no hay ninguna, no se pone) (enseña a romper con el espacio,
  R6; opcional).

### Reparto por bloques
| Bloque | Entra | Prueba |
|---|---|---|
| 1 | antorcha (pared, 1/16), huesos y barriles (suelo; barril rompible con botín), **1 × P1** (palanca + reja de atajo) y **1 placa de pinchos** con su ciclo y rótulo. Tabla de planta 1 para todas las plantas | 100 semillas: P1 colocada ≥ 90 %, 0 trampas en recodo o a < 6 de la entrada, 0 muebles bloqueantes en articulación; captura de la reja vista desde la palanca y de los pinchos en sus 3 estados |
| 2 | P2, dardo, cadenas/calaveras/nicho/estandarte, grieta en SECRET, caja, estatua, mesa, cofre, charco; tablas por planta 1–3 | bot de 50 semillas sin softlock con rejas como pared |
| 3 | P3, P4 (caja empujable), P5, runa, foso, palanca con tiempo | según tareas nuevas |

## 7. Muerte y progreso
- **Reintento de la planta, no permadeath** (R10: fallar cuesta poco). Al morir: `YOU DIED` y en < 5 s vuelves al
  inicio de la misma planta (misma semilla) con el estado guardado al bajar a ella. Opción `NEW RUN` en la misma
  pantalla para semilla nueva. El modo "una vida" queda como opción futura (P4).
- **Guardado en SRAM** al bajar cada escalera (y al empezar la partida): semilla, planta, nivel, XP, HP, inventario,
  equipo y mensajes ya enseñados. Un solo hueco; `CONTINUE` / `NEW GAME` en el título.
- **Meta del bloque 1 del juego completo**: 3 plantas y el jefe al final de la 3 → pantalla `THE DEPTHS AWAIT`
  (fin de la demo) con tiempo total y muertes.

## 8. Primer minuto (checklist de la guía)
1. Título → `NEW GAME`. Apareces en el callejón de entrada (zona segura, §6) mirando a la única salida, con un barril con pan cerca si cabe (§6b), con la daga equipada, la
   antorcha en la mano izquierda, 30 HP y 1 pan. Rótulo `UP: WALK   LEFT/RIGHT: TURN`.
2. A 4–6 casillas por el pasillo, una **rata sola**, quieta hasta que te ve a d = 3: se ve antes de que se mueva.
   Rótulo `A: ATTACK WHEN THE BAR IS FULL`. Muere de 2 golpes; hace 3 de 30 (10 golpes para matarte): imposible
   morir en el primer encuentro sin querer.
3. Su primer aviso enseña la esquiva (`STEP AWAY TO DODGE!`). Suelta pan o hay un objeto en la sala siguiente:
   `B: QUICK ITEM   START: BAG`.
4. Objetivo en 10 s: el pasillo apunta fuera, la brújula y el minimapa muestran lo explorado; el rótulo de la
   escalera (`STAIRS DOWN: WALK IN`) aparece al verla.
- Comprobación: se entiende qué controlo en 10 s; cada acción responde (paso, golpe con número, `MISS`, recogida);
  nada mata sin aviso; reintento < 5 s.

## 9. Bloques
| Bloque | Contenido | Prueba |
|---|---|---|
| 1 | Una planta generada jugable con arte (texturas + 1 monstruo dibujado), movimiento y giros, puertas sin llave, rat y skeleton, combate completo (barra, esquiva, crítico), recoger pan, poción y short sword (sin pantalla de inventario: contador de pociones en el panel, B la usa), bajar escaleras → planta nueva con otra semilla; del §6b: antorchas, huesos, barriles, 1 palanca con reja (P1) y 1 placa de pinchos | `gbarun.py` con entrada grabada: sala inicial, matar la rata, bajar; capturas revisadas |
| 2 | Plantas 1–3 con su tabla, bat, slime, cultist, jefe con 2 fases, inventario 4×3 y mapa, llaves y salas cerradas, muerte con reintento, guardado SRAM, título | bot: 50 semillas sin softlock; partida completa con tiempos |
| 3 | Pergaminos → magia (maná en el panel), más plantas y monstruos, música y efectos por monstruo | según tareas nuevas |

**No entra** (a tareas P4): clases, creación de personaje, hambre, combustible de antorcha, tiendas, PNJ, misiones,
diagonales, varios personajes.

## 10. Decisiones de diseño
| Decisión | Principio / regla | Métrica que lo comprobará |
|---|---|---|
| L/R paso lateral, opcional | R14 (esquema básico basta), R5 | % de jugadores que lo usan contra cultists; se puede ganar sin él |
| Abrir puertas y recoger objetos andando | R14 | ninguna pulsación "muerta" en la grabación |
| Esquiva = salir de la casilla durante el aviso | R5, R1 (verbo central) | tasa de esquivas ≥ 60 % desde la planta 2 |
| Contraataque crítico tras esquivar | riesgo/recompensa | % de críticos por contraataque > 30 % |
| Sin hambre ni combustible | R14 | — (alcance) |
| Reintento de la planta en vez de permadeath | R10 | reintento < 5 s; muertes por planta ≤ 3 de media |
| Llaves sólo para salas opcionales | R9, R12, R7 | 0 softlocks de juego en 1000 semillas (sobre la prueba de forma del generador) |
| Sala segura e inicio sin monstruos a < 8 casillas | R4, R5 | 0 daños en los primeros 10 s en 100 semillas |
| Un tipo nuevo por planta, presentado solo | R3 | muertes ante el tipo nuevo en su primer encuentro |
| (2026-11-23) §6 corregido a la v2 real: paredes en los lados, ~290 casillas (28 %), 2–9 callejones, entrada en callejón al azar, zona segura en vez de sala 3×3 forzada, escalera en el cuarto más lejano, `lock_free = 1`, `maze_pct = 0` en plantas 1–3, monstruos a ≥ 8 | R4, R13 (números del generador real, no del documento) | tiempo por planta 4–6 min con bot; 0 daños en los primeros 10 s |
| Rejas, interruptores y reinterpretación de trampas/muebles/pared en una fase de ganchos con el 2.º flujo de azar, sin tocar la v2 | R13 (entrada reproducible; CRC de la v2 intacto) | CRC de la v2 igual antes y después de los ganchos en 1000 semillas |
| Interruptores sólo mueven rejas; palanca fija, placa con 120 f de cierre | R7, R9, R14 | 0 rejas cerradas en el camino crítico en 1000 semillas |
| Reja a ≤ 3 casillas en línea del interruptor + sonido + rótulo la 1.ª vez | R5, R6, R7 | tiempo de la palanca a la reja < 10 s en las pruebas |
| Rejas de interruptor sólo en atajos (antes pared) o zonas opcionales | R9, R12, R8 | comprobador de la v2 con BARS = pared llega a la escalera en 100 % |
| Trampas: pinchos con ciclo, dardo, runa, foso; spin y alarm de la v2 fuera | R5 | 0 daños de trampa sin que estuviera en la vista el frame anterior (registro del bot) |
| Trampas nunca en recodo, a < 6 de la entrada ni en paso obligado si no se pasan con ritmo | R5, R9, R4 | 0 casos en 1000 semillas |
| Grieta en todas las SECRET y sólo en ellas | R7, R12 | % de secretos encontrados (objetivo > 30 %) |
| Atrezzo por planta con tablas de 16/8 entradas (Cellars, Ossuary, Warden's Halls) | R8 | captura a 1x: cada planta se reconoce sin texto |
| Muebles bloqueantes sólo en salas, nunca en articulación ni tapando usos | R9 | 0 softlocks; BFS entre aberturas de sala en 1000 semillas |
| Mensaje de ataque acortado a `A: ATTACK WHEN BAR IS FULL` | R6 (cabe en una línea de 28) | — |

## 11. Riesgos y propuestas para `ESPEC_VISTA.md`
- **Redibujo del modo 4 al dar un paso**: si pasa de 2 frames, la recarga de 12 f lo oculta; si pasa de 6 f se nota.
  Medir con `gba-performance` en el bloque 1.
- **Tiempo real con giros de 90°**: un monstruo a la espalda es injusto si no se ve; por eso el aviso ×1,5 y la flecha.
  Validar con capturas.
- **Sonido de aviso** antes del bloque 3: la guía pide respuesta sonora; propongo 3 efectos mínimos ya en el bloque 1
  (golpe, aviso, recogida).
- **Generador**: el mayor riesgo de bloqueo; la prueba de 1000 semillas va con el generador, no después.
- **Rejas en la vista**: BARS deja ver detrás; el redibujo tiene que pintar lo de detrás y la reja encima (textura con
  transparente). Medir el coste en el bloque 1 con P1.
- **Variantes de pared**: cada decoración es una textura más proyectada a todas las piezas (ROM y tiempo de
  `build_view.py`). Bloque 1: 1 (antorcha) + palanca (2 estados) + reja; bloque 2: hasta 7 más. Si la ROM crece
  demasiado, dibujar la decoración sólo en caras frontales y laterales de d ≤ 2.
- **Atajos con poca vuelta**: la bajada es de un solo sentido y el reintento regenera la planta, así que P1 vale menos
  que en un mundo conectado (R8); si el bot apenas los usa, cambiar P1 por P2 en la planta 1.
- Propuestas (no cambio la espec, decide el motor/artista): (a) falta el formato de los **objetos en el suelo** y de
  la **bola de fuego**: propongo sprite base 32×32, 16 colores, escalado igual que los monstruos y apoyado en el
  suelo; (b) reservar en la vista un hueco para las **flechas de ataque lateral** (borde izq./der., 8×16);
  (c) en la franja inferior, la **barra de recarga** (≈ 48×4 px junto al arma) y la línea de mensajes (≤ 28
  caracteres con la fuente de 8 px) deben caber en los 40 px de alto: comprobarlo en `ui_frame`.
