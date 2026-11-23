# POKET GTO — diseño

**Frase:** POKET GTO es un dojo de póker Texas Hold'em No-Limit en el que cada ejercicio es una mano: decides en menos de 10 s y
el maestro te dice al momento cuánto EV has perdido y por qué. Avanzas de cinturón blanco a negro (cerca de GTO).

Nombre del juego (en pantalla y en textos): **POKET GTO**. La carpeta sigue siendo `projects/003_poker_dojo`.

**Verbo central:** *decidir una mano* (fold / call / raise / all-in). Todo lo demás (mapa, estrellas, repaso) sirve
a ese verbo (regla 1). Todo el texto del juego va en **inglés**; la fuente es `common::variable_8x8_sprite_font` con
`bn::sprite_text_generator` (nada de fuentes propias).

**Fuentes de verdad** (cada nivel lleva una; se muestra en la pantalla de entrada del nivel):

| Marca | Significado | Qué cubre |
|---|---|---|
| **E** exacta | calculada por nosotros en el PC con garantía | reglas, ranking, outs, pot odds, equidades all-in 169×169, Nash push/fold HU 1–25 bb con y sin ante |
| **M** modelo propio | modelo simplificado documentado y reproducible; en la UI sale `APPROX` y la tolerancia es mayor | push/fold multijugador (juego ficticio), ICM Malmuth-Harville, rangos de apertura/defensa preflop |
| **P** pendiente | necesita un solver serio; el nivel no se publica hasta tenerlo | c-bets, juego postflop con rangos completos, mesa final profunda |

La GBA no calcula nada: un script de `tools/` genera tablas (`.h` o binario en ROM) y una prueba de PC verifica sumas
y simetrías. Ninguna tabla se copia de sitios comerciales.

## 1. Bucle, controles y pantallas

**Bucle (20 manos ≈ 4 min por nivel):** entrada del nivel (1 línea + `B: chart`) → mano → decisión (≤ 10 s) →
feedback (≈ 3 s, A para seguir) → … → resultado (estrellas) → `A: retry` o mapa. Reintentar cuesta < 5 s (regla 10).

| Botón | En la mano | En el feedback | En menús |
|---|---|---|---|
| Cruceta ←/→ | elegir acción entre los botones visibles | — | moverse |
| Cruceta ↑/↓ | cambiar tamaño si está elegido RAISE (`2x`, `2.5x`, `3x`; postflop `33%`, `50%`, `75%`, `POT`) | — | moverse |
| A | confirmar | siguiente mano | entrar |
| B | ver la tabla/rango del spot (la mano cuenta como fallo: `PEEKED`) | ver la tabla con tu mano marcada | atrás |
| START | pausa (`RESUME` / `CHART` / `QUIT`) | pausa | — |

L, R y SELECT no se usan (regla 14). Sólo se muestran los botones legales del spot: en el Mundo 1 hay **dos**
(`FOLD` / `ALL-IN`), y en el nivel 0-1 tres (`LEFT` / `TIE` / `RIGHT`). La complejidad llega con los mundos, no antes.

**Pantalla de mano (240×160):**

| Zona | px | Contenido |
|---|---|---|
| Franja superior | 0–11 | `1-1 SB PUSH 10BB   7/20` y barra de tiempo (10 s, 60 px) |
| Mesa | 12–115 | óvalo verde; rival(es) arriba con pila (`10.0 BB`) y acción previa (`ALL-IN`); bote al centro; ficha de botón; en 6-max, 5 asientos de 32×16 |
| Héroe | 92–131 | 2 cartas 24×32 (rango grande, palo con color: ♥♦ rojo, ♣♠ negro; 4 colores opcionales), posición y pila |
| Barra de acción | 136–159 | botones de 56×18; el elegido se invierte |

Postflop: el tablero (hasta 5 cartas 20×28) va en el centro de la mesa. Feedback: panel de 2 líneas sobre la barra:
`CORRECT +0.00 BB` (verde, sonido corto) o `MISTAKE -0.42 BB` (rojo), `Best: ALL-IN` y el porqué de una línea
(≤ 38 caracteres) sacado de plantillas con números de la tabla, p. ej. `They fold 63%. Called: 41% equity.` (regla 6:
una línea en el momento en que se usa).

**Tabla 169 (B):** rejilla 13×13 de 10 px (130×130) a la izquierda: pares en la diagonal, suited arriba, offsuit
abajo; color = acción (rojo all-in/raise, verde call, gris fold; mezcla = mitad y mitad). Panel derecho (110 px):
mano bajo el cursor, acción, frecuencia y ΔEV. Tu mano parpadea.

**Pantallas:** título → (primera vez: directo al nivel 0-1) → mapa del dojo (mundos como salas, niveles con
estrellas) → entrada de nivel → mano/feedback → resultado. Extra desde el mapa: `MISTAKES` (mapa de errores) y
`REVIEW` (repaso). Pausa en todo momento.

**Primer minuto:** `POKET GTO / PRESS START` → nivel 0-1 sin menú. Rótulo: `Which hand wins? LEFT / TIE / RIGHT,
A to confirm.` Primera mano obvia (color contra pareja), respuesta en 5 s, `CORRECT` con sonido. Nada cuenta en
contra en las 3 primeras manos (regla 3: presentar en sitio seguro; regla 10).

## 2. Temario

Orden propio (cambia el propuesto) y porqué:
- **Push/fold HU va justo después de lo básico:** es la única estrategia **exacta** (el feedback merece confianza),
  la decisión es binaria (regla 14) y enseña rangos, equidad y fold equity, que todo lo demás reutiliza.
- **Pot odds pasan al Mundo 0:** el "BB call" del Mundo 1 es exactamente pot odds contra un rango.
- **Torneo se parte por fases:** la fase corta (push/fold multijugador, reshove, burbuja ICM) sigue al Mundo 1 con
  modelo propio fiable; las fases de pila media/profunda reutilizan el preflop de cash y van después de él.
- **Postflop al final:** lo más difícil y lo menos exacto.

**Generación de ejercicios (todos los niveles de decisión):** para cada spot la tabla da ΔEV = EV(mejor) − EV(otra)
por mano. Las manos se clasifican en **CLEAR** (|ΔEV| ≥ 1 bb), **MEDIUM** (0,25–1 bb) y **EDGE** (< 0,25 bb). En una
tanda de 20: manos 1–5 CLEAR, 6–14 MEDIUM, 15–20 EDGE (andamiaje dentro del nivel), más hasta 4 de repaso (§4). Si
|ΔEV| < 0,02 bb (E) o < 0,10 bb (M), o la frecuencia Nash está entre 10 % y 90 %, ambas acciones valen (`CLOSE: both
OK`). Las cartas concretas (palos) se sortean dentro de la clase de mano.

**Criterio estándar de aprobado:** 20 manos, ≥ 17 aciertos (85 %) y pérdida media < 0,10 bb (en M: < 0,15 bb). En
niveles sin EV (reglas, ranking) sólo cuenta el acierto. Aprobar desbloquea el nivel siguiente; el último nivel de un
mundo desbloquea el mundo siguiente.

### Mundo 0 — Basics (cinturón blanco)

| Nv | Objetivo | Ejercicio | Aprobar | Fuente |
|---|---|---|---|---|
| 0-1 | ordenar categorías de manos | 2 manos + tablero; `LEFT/TIE/RIGHT`. Primero categorías distintas y lejanas (color vs pareja), luego vecinas (escalera vs trío) | 17/20 | E (400 spots precalculados) |
| 0-2 | kickers, mejor 5 de 7, botes divididos | misma categoría; el tablero juega; kicker decide | 17/20 | E |
| 0-3 | posiciones y orden de acción | mesa 6-max: `Who acts first preflop/postflop?`, `Which seat is CO?` (elegir asiento con cruceta) | 17/20 | E |
| 0-4 | contar outs | flop + mano: 2/4/8/9/15 outs (elegir valor) | 17/20 | E |
| 0-5 | pot odds | bote, apuesta y tu equidad (dada); `CALL/FOLD`; regla del 2 y del 4 en el porqué | 17/20, pérdida < 0,10 bb | E |

### Mundo 1 — Heads-up Push/Fold (cinturón amarillo, todo E)

Juego: SB y BB con pilas efectivas S; la SB sólo empuja o se retira (sin limp); la BB paga o se retira. Ante = BB
ante de 1 bb (formato de torneo actual). EV en bb contra la estrategia Nash del rival.

| Nv | Objetivo | Spot | Desbloquea |
|---|---|---|---|
| 1-1 | rango de push SB a 10 bb (≈ 55–60 % de manos; la cifra exacta la da el generador) | SB, 10 bb, sin ante | 1-2 |
| 1-2 | rango de call BB a 10 bb (≈ 35–40 %) | BB contra push, 10 bb | 1-3 |
| 1-3 | cómo se ensancha el push al bajar la pila | SB a 8 y 6 bb | 1-4 |
| 1-4 | call de BB a 8 y 6 bb | BB a 8 y 6 bb | 1-5 |
| 1-5 | pila crítica: casi todo es push | ambos papeles, 1–4 bb | 1-6 |
| 1-6 | efecto del ante (bote mayor → más push y más call) | ambos papeles, 4–12 bb con ante | 1-7 |
| 1-7 | examen del mundo (**giro**: pila al azar) | ambos papeles, 1–25 bb, con/sin ante | Mundo 2 |

### Mundo 2 — Tournament: short stack (cinturón naranja, M)

| Nv | Objetivo | Spot | Fuente |
|---|---|---|---|
| 2-1 | push/fold en cualquier posición (9-max, ante) | primer jugador en entrar, 5–15 bb, UTG…SB | M: Nash push/fold multijugador por juego ficticio, chip EV |
| 2-2 | call contra push según quién empuja y cuántos quedan detrás | 5–15 bb | M (mismo modelo) |
| 2-3 | reshove sobre una apertura | 12–25 bb contra open 2x | M |
| 2-4 | burbuja: el ICM aprieta los calls | 4 jugadores, paga 3, pilas variadas, push/call | M: ICM Malmuth-Harville + Nash push/fold (exacto dentro del modelo) |
| 2-5 | mesa final corta: escalones de premio, pila grande contra corta | 3–5 jugadores | M |

### Mundo 3 — Cash 6-max preflop, 100 bb (cinturón verde, M)

| Nv | Objetivo | Spot |
|---|---|---|
| 3-1 | RFI por posición (UTG, HJ, CO, BTN), open 2.5x | todos se retiran hasta ti; `FOLD/RAISE` |
| 3-2 | SB contra BB (raise o fold; sin limp por defecto) | SB 3x |
| 3-3 | defensa de BB contra BTN/CO/SB | `FOLD/CALL/RAISE` (3-bet) |
| 3-4 | 3-bet o fold en posición | BTN/CO contra open anterior |
| 3-5 | contra 3-bet: fold / call / 4-bet | open propio que recibe 3-bet |
| 3-6 | 9-max: posiciones tempranas más cerradas (**giro**) | RFI y defensa 9-max |

Fuente M: rangos producidos por un modelo propio documentado (equidad contra el rango de respuesta × factor de
realización por posición, iterado hasta estabilidad), revisados a ojo contra teoría pública general. Pasan a E-solver
si más adelante tenemos un solver preflop CFR propio.

### Mundo 4 — Tournament: deep and mid stacks (cinturón azul, M)

| Nv | Objetivo | Spot |
|---|---|---|
| 4-1 | fase temprana (60–100 bb): como cash pero chip EV | RFI y defensa, con ante |
| 4-2 | pila media 20–40 bb: open 2–2.2x, menos call, más 3-bet all-in | RFI / defensa / reshove |
| 4-3 | cambio de fase en la misma sesión (**giro**: la pila cambia cada mano) | mezcla 10–100 bb |
| 4-4 | mesa final con premios (ICM con pilas profundas) | P hasta tener solver; se publica como 2-5 ampliado |

### Mundo 5 — Postflop basics (cinturón rojo)

| Nv | Objetivo | Spot | Fuente |
|---|---|---|---|
| 5-1 | pagar con proyecto según pot odds contra una mano/rango dado | flop/turn, apuesta 33–100 % | E (equidad exacta contra el rango indicado) |
| 5-2 | cuánto debe funcionar un farol / MDF | apuesta X en bote Y: `CALL/FOLD` con bluff-catcher | E (matemática de indiferencia) |
| 5-3 | c-bet en flop seco (A72r, K83r): pequeño y frecuente | `CHECK/BET 33%` | M (ventaja de rango por equidad exacta + regla) → P |
| 5-4 | c-bet en flop mojado (JT9ss, 876): menos y más grande | `CHECK/BET 33%/75%` | M → P |

### Mundo 6 — Black belt exam

50 manos al azar de todos los niveles aprobados (pesadas hacia EDGE y hacia tus errores); aprobar con ≥ 90 % y
pérdida < 0,08 bb. Es el "jefe" que examina lo aprendido (regla 11).

## 3. Puntuación y progresión

| Estrellas | Niveles con EV | Niveles sin EV |
|---|---|---|
| ★ | ≥ 17/20 y pérdida < 0,10 bb | ≥ 17/20 |
| ★★ | ≥ 18/20 y < 0,05 bb | ≥ 19/20 |
| ★★★ | ≥ 19/20, < 0,03 bb y todas en ≤ 10 s | 20/20 en ≤ 10 s |

- **Tiempo:** el límite de 10 s es **blando** en los bloques 1–2: al agotarse la barra sale `TIME` pero puedes
  seguir; sólo quita la 3.ª estrella. Se endurece (cuenta como fold) en el Mundo 6.
- **Racha:** `STREAK n` en la franja superior; se guarda la mejor por mundo. Sin multiplicadores (no premia jugar
  rápido a ciegas).
- **Cinturón** por mundo completado (blanco, amarillo, naranja, verde, azul, rojo, negro), visible en el mapa
  (gradiente de meta).
- **Mapa de errores (`MISTAKES`):** eliges familia de spot (p. ej. `SB PUSH 10BB`) y ves la rejilla 169: gris = no
  vista, verde = acertada, rojo (más oscuro = más fallos) = fallada, amarillo = pendiente de repaso. A sobre una celda:
  tu acción, la correcta y el ΔEV.
- **SRAM** (`bn::sram`, 32 KB; se escribe al final de cada mano, nada se pierde al apagar):

| Bloque | Tamaño | Contenido |
|---|---|---|
| Cabecera | 16 B | magia `PGTO`, versión, nº de sesiones, opciones (4 colores) |
| Niveles | 64 × 4 B | estrellas, mejor acierto, mejor pérdida (cbb), intentos |
| Rachas | 8 × 2 B | mejor racha por mundo |
| Tarjetas de repaso | 24 familias × 169 × 2 B ≈ 8,1 KB | caja Leitner (0–5), sesión de vencimiento, nº de fallos (saturado a 15) |

Al cambiar la versión se migra o se avisa (`SAVE RESET`); nunca se borra en silencio.

## 4. Psicología del aprendizaje

| Decisión | Principio | Métrica que lo comprobará |
|---|---|---|
| CLEAR → MEDIUM → EDGE dentro de cada nivel; mundos de lo exacto/binario a lo aproximado/múltiple | andamiaje (Vygotsky, zona de desarrollo próximo); flujo (Chen): reto ≈ habilidad; regla 3 | acierto por tramo: CLEAR ≥ 95 %, EDGE 60–80 % en pruebas; si EDGE < 50 %, más manos MEDIUM |
| Feedback tras cada mano con ΔEV, acción correcta y una línea | feedback inmediato y específico (Hattie y Timperley); regla 6 | tiempo medio en feedback 2–5 s (si < 1 s nadie lo lee: acortar) |
| Errores a caja 1 de Leitner; vencen en 1, 2, 4, 8 sesiones; caja 5 = aprendida | repetición espaciada (Ebbinghaus, Leitner); práctica de recuperación | % de aciertos en repaso sube por caja; fallos repetidos de la misma mano bajan entre sesiones |
| Hasta 4 manos de repaso mezcladas en cada tanda + modo `REVIEW` de 10 manos | práctica intercalada (Rohrer); regla 8: volver da algo nuevo | sesiones con repaso vencido > 0 al abrir el mapa |
| Fallar un nivel: `A: retry` inmediato; no hay vidas | regla 10 (fallar cuesta poco); aversión a la pérdida baja | reintento < 5 s medido con `gbarun.py`; % de abandonos tras suspender |
| Sólo los botones legales; 2 en el Mundo 1 | carga cognitiva (Sweller); regla 14 | decisiones de Mundo 1 en < 6 s de media |
| `CLOSE: both OK` en manos mixtas o casi indiferentes | no castigar lo que GTO no distingue (evita aprender ruido) | ningún fallo con |ΔEV| < umbral |
| B durante la mano = `PEEKED` (cuenta como fallo y va a repaso); la tabla es libre antes y después | estudio y examen separados; dificultad deseable (Bjork) | % de manos `PEEKED` < 10 % tras el primer intento de cada nivel |

## 5. Reparto en bloques

| Bloque | Contenido | Qué NO entra |
|---|---|---|
| **1 (demo jugable)** | título, mapa mínimo (2 salas), niveles **0-1, 0-2, 1-1, 1-2**; mesa HU sencilla en greybox (rectángulos + cartas 24×32 con rango y palo en texto/colores); feedback completo; tabla 169 con B; estrellas y desbloqueo en SRAM; tablas E de equidades 169×169 y Nash HU 10 bb sin ante | repaso Leitner, mapa de errores, ante, otras pilas, 6-max, arte final, música |
| 2 | resto de Mundo 0 y Mundo 1 (todas las pilas 1–25, ante), mapa de errores, repaso Leitner, `REVIEW`, rachas | multijugador |
| 3 | Mundo 2 (push/fold multijugador e ICM, modelo M) y mesa 9-max | cash 100 bb |
| 4 | Mundo 3 (cash 6-max preflop, M), tamaños de raise | postflop |
| 5 | Mundo 4, Mundo 5 (5-1, 5-2 E; 5-3/5-4 si hay modelo), Mundo 6; arte final y sonido; release itch.io | niveles P sin fuente |

Lo que no entra en un bloque va a tareas P4, no al juego (skill `gba-design`).

**Datos del bloque 1:** equidades 169×169 en u16 (≈ 56 KB) y Nash HU como ΔEV en centi-bb i16 por mano, pila y papel
(169 × 25 × 2 × 2 ≈ 34 KB para todo el Mundo 1; el bloque 1 sólo usa 10 bb). Prueba de PC: push 10 bb de la SB y
call de la BB coinciden con la indiferencia en la mano frontera (±0,01 bb).

## Decisiones de diseño

| Decisión | Principio / regla | Métrica |
|---|---|---|
| Push/fold HU antes que cash | regla 1 (verbo cerrado con datos exactos), regla 14 | bloque 1 jugable con 0 tablas M |
| Torneo partido en fase corta (M2) y profunda (M4) | regla 3 (una idea por mundo); continuidad con lo aprendido | acierto del primer intento en 2-1 ≥ 70 % tras aprobar M1 |
| Tiempo blando hasta el Mundo 6 | flujo; regla 10 | < 10 % de manos con `TIME` en pruebas; si se supera, revisar legibilidad |
| Etiqueta `APPROX` y tolerancia mayor en niveles M | honestidad del feedback; no enseñar ruido | ningún nivel P publicado |
| Sin vidas ni castigo al suspender | regla 10 | reintento < 5 s |

## Riesgos

| Riesgo | Mitigación |
|---|---|
| Rangos M equivocados enseñan errores | etiqueta `APPROX`, tolerancia 0,10 bb, pruebas de sanidad (monotonía por posición y pila); los P no se publican |
| Cartas y mesa ilegibles a 240×160 (6/9 asientos) | cartas del héroe 24×32 grandes; rivales sólo como etiqueta + pila; captura a 1x revisada en cada bloque |
| Monotonía (20 manos iguales) | giro en el último nivel de cada mundo, repaso intercalado, rachas |
| Alcance (7 mundos) | bloque 1 con 4 niveles y sólo datos E; cada bloque es publicable por sí solo |
| SRAM corrupta o de versión vieja | cabecera con magia y versión; migración o aviso explícito |
