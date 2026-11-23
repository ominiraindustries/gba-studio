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
| **B** contra el bot | EV contra el modelo de rival del modo carrera (rango y reglas conocidos), Monte Carlo en la GBA; sale `APPROX` y `vs <PROFILE>` | decisiones postflop del modo carrera (§6); no dice "GTO", dice "contra este jugador" |

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
| Mesa | 12–115 | óvalo verde; rival(es) arriba con pila (`10.0 BB`) y acción previa (`ALL-IN`); bote al centro; ficha de botón; en mesas de 6 y 8 asientos, layout de §6.7 |
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

Los antiguos 0-4 (outs) y 0-5 (pot odds) pasan a ser **MD-1** y **MD-2** del MATH DOJO (§7).

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
| 2-1 | push/fold en cualquier posición (8-max, ante; 9-max como variante) | primer jugador en entrar, 5–15 bb, UTG…SB | M: Nash push/fold multijugador por juego ficticio, chip EV |
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
| 3-6 | profundidad al azar 25/40/100/200 bb en cada mano (**giro**) | RFI y defensa 6-max (full ring 9 sólo en el futuro modo `LIVE`) |

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
| 2 | **Modo carrera preflop** (§6.8): mesa 6-max, rivales que entran y salen, NL2–NL10, puntuación preflop | postflop jugado, HUD, NL25+ |
| 2b | **MATH DOJO I** (§7): MD-1…MD-5 (outs, pot odds, equidad, SPR, implied odds); todo E, reutiliza el bucle de ejercicios del bloque 1 + selector numérico | MD-6+ |
| 3 | **Modo carrera postflop** (§6.8): IA postflop, Monte Carlo, puntuación postflop, tamaños; **MATH DOJO II** MD-6…MD-8 (EV, fold equity, MDF/alpha) | NL25+, HUD |
| 4 | **Escalera completa** (§6.8): NL25–NL100, reg, HUD, informes y repaso de manos; Mundo 3 como `DRILL` (mismas tablas) | — ; candidato a release itch.io |
| 5 | resto de Mundo 0 y Mundo 1 (todas las pilas 1–25, ante), mapa de errores, repaso Leitner, `REVIEW`, rachas | — |
| 6 | Mundo 2 (push/fold multijugador e ICM, modelo M) y mesa 8-max (layout de §6.7); **MATH DOJO III** MD-9…MD-11 (M de Harrington, ICM) | — |
| 7 | Mundo 4, Mundo 5 (5-1, 5-2 E; 5-3/5-4 si hay modelo), Mundo 6; arte final y sonido; release final | niveles P sin fuente |

El antiguo bloque 4 (Mundo 3) se absorbe en el 4: sus niveles son spots de las mismas tablas preflop del modo carrera.

Lo que no entra en un bloque va a tareas P4, no al juego (skill `gba-design`).

**Puertas matemáticas** (cada modo de juego exige su prerrequisito del MATH DOJO; una puerta sólo se activa cuando su
nivel ya existe en la ROM, nunca bloquea por algo no publicado, regla 9):

| Modo / mundo | Exige | Porqué |
|---|---|---|
| Carrera NL2 y NL5 | nada (abierto desde el mapa) | el usuario quiere jugar mesas pronto; preflop no necesita aritmética nueva |
| Subir a NL5 | MD-1, MD-2 | desde B3 se juega postflop: outs y pot odds son el lenguaje del feedback |
| Subir a NL10 | MD-3, MD-4, MD-5 | cash: equidad, SPR y implied odds explican calls, set-mining y compromiso |
| Subir a NL25 | MD-6, MD-7, MD-8 | rivales agresivos: EV, faroles y MDF |
| Mundo 3 (`DRILL` cash) | MD-4, MD-5 | — |
| Mundo 2 y 4 (torneo) | MD-9 (2-1, 4-x), MD-10 y MD-11 (2-4, 2-5) | M y ICM |
| Mundo 5 (postflop) | MD-2, MD-7, MD-8 | — |

## 6. Modo carrera: mesas cash 6-max

**Frase:** te sientas en mesas cash 6-max de NL2 a NL100, entran y salen rivales con perfiles distintos y pilas
distintas, y el maestro puntúa cada decisión tuya (`GOOD` / `OK` / `LEAK`) según posición, profundidad efectiva y
rival. Subes de mesa por **calidad de decisiones** y bajas por **bankroll**, como un jugador real. El verbo sigue siendo
*decidir una mano* (regla 1); la mesa es el contexto. En el mapa es la sala `CASH TABLES`.

### 6.1 Escalera de mesas

Buy-in por defecto 100 bb; el héroe elige al sentarse 40 / 60 / 100 bb (←/→). `AUTO TOP-UP` (ON por defecto): entre
manos rellena hasta el buy-in elegido. Si pierde la pila, recompra automática por el mismo importe (mensaje
`Rebuy 100bb ($10.00)`). Sin rake (el modelo de las tablas tampoco lo tiene; ver riesgos). Mesa máxima 100 bb para el
héroe; los rivales pueden traer más (pilas ganadas, §6.3).

| Nivel | Ciegas | Buy-in 100 bb | Idea nueva (regla 3) | Mezcla de perfiles % (fish/station/nit/maniac/TAG/LAG/reg) | Pilas rivales % corta/normal/profunda | Icono de perfil | Bloque |
|---|---|---|---|---|---|---|---|
| NL2 | $0.01/0.02 | $2 | abrir por posición y cobrar a los que pagan todo | 35/25/15/10/15/0/0 | 5/90/5 | visible siempre | B2 |
| NL5 | $0.02/0.05 | $5 | respetar a los nits, robar ciegas | 30/20/20/10/15/5/0 | 10/80/10 | visible siempre | B2 |
| NL10 | $0.05/0.10 | $10 | botes 3-bet contra TAG | 20/15/20/5/30/10/0 | 10/75/15 | tras 20 manos vistas | B2 |
| NL25 | $0.10/0.25 | $25 | LAG y pilas cortas (20–40 bb) | 15/10/15/5/35/15/5 | 25/60/15 | tras 40 manos | B4 |
| NL50 | $0.25/0.50 | $50 | regs y pilas profundas (150–250 bb) | 10/5/10/5/30/15/25 | 15/55/30 | nunca (sólo HUD) | B4 |
| NL100 | $0.50/1 | $100 | **examen** (giro, regla 11): todo mezclado, sin iconos | 8/4/8/0/25/15/40 | 15/50/35 | nunca | B4 |

**Bankroll** (en $ y en buy-ins, `BI`, siempre a la vista en la franja superior). El juego comprime el tiempo: subir
sólo con ganancias exigiría decenas de miles de manos, así que el maestro **banca** (`STAKE`):

| Regla | Cifra | Principio |
|---|---|---|
| Bankroll inicial | $50 = 25 BI de NL2 (bancado) | gestión real: 20–30 BI por nivel |
| Subir (`PROMOTION`) | ventana de las últimas 200 manos (NL2–NL10) o 300 (NL25+) en el nivel, ≥ 60 decisiones puntuadas, `LEAK` ≤ 10 / 8 / 7 / 6 / 5 % (NL2→NL5 … NL50→NL100), pérdida media ≤ 0,20 / 0,15 / 0,12 / 0,10 / 0,08 bb por decisión, bankroll ≥ 20 BI del nivel actual y puertas matemáticas (§5) | se juzga el proceso, no el resultado (varianza); gradiente de meta |
| Al subir | el maestro completa hasta 25 BI del nivel nuevo (`STAKED: $125 for NL5`); si ya tienes más, no añade | aversión a la pérdida baja; enseña la cifra de 25 BI |
| Bajar (`MOVE DOWN`) | bankroll < 15 BI del nivel → baja uno, con la línea `Below 15 BI: move down (bankroll rule)` | gestión de bankroll realista (stop-loss) |
| Volver a subir | nueva ventana completa en el nivel bajo; se vuelve a bancar hasta 25 BI | regla 10: fallar cuesta poco |
| Bajo NL2 | si < 15 BI en NL2: `RESTAKE` a 25 BI, contador visible | regla 9: nunca queda bloqueado |
| Elegir mesa | cualquier nivel desbloqueado con bankroll ≥ 15 BI | regla 12 (lo opcional regula la dificultad) |
| Marcador de carrera | `NET WON $` (sin contar stakes) y bb/100 de por vida | recompensa a largo plazo |

Ritmo de la carrera medido en manos: ≥ 200 manos por nivel × 5 ascensos ≈ 1.100 manos mínimo; a ≈ 7 s por mano media
(B3) son ≈ 2 h 10 min de juego. En B2 (sólo preflop, ≈ 4 s/mano) un nivel son ≈ 13 min.

### 6.2 Perfiles de rival

Cifras objetivo en 6-max (estadísticas medidas por la simulación de PC de 100.000 manos de bots; tolerancia ±3 puntos;
si no se alcanza, se ajustan los factores, no las cifras):

| Perfil | Icono 8×8 (color) | VPIP | PFR | 3-bet | Fold a 3-bet | C-bet flop | Fold a c-bet | AF | WTSD | Consejo de explotación (feedback) | Principio |
|---|---|---|---|---|---|---|---|---|---|---|---|
| NIT | piedra gris | 12 | 10 | 3 | 65 | 55 | 60 | 1.8 | 22 | `vs NIT: steal more, fold to his raises` | contraste: aprendes que no todo rival es igual |
| TAG | escudo azul | 22 | 18 | 7 | 55 | 65 | 45 | 2.8 | 27 | `vs TAG: play the chart` | referencia = la tabla base |
| LAG | llama naranja | 30 | 25 | 11 | 45 | 75 | 38 | 3.5 | 28 | `vs LAG: call down wider, 3-bet for value` | — |
| STATION | teléfono amarillo | 40 | 5 | 2 | 25 | 40 | 20 | 0.7 | 38 | `vs STATION: no bluffs, bet big for value` | lenguaje de color: amarillo = presa |
| MANIAC | bomba roja | 60 | 45 | 20 | 30 | 85 | 30 | 5.0 | 33 | `vs MANIAC: trap, call down, never bluff` | rojo = peligro |
| FISH | pez cian | 38 | 12 | 4 | 40 | 50 | 35 | 1.2 | 32 | `vs FISH: isolate and value bet` | recompensa variable (aciertos y regalos) |
| REG | corbata morada (sólo NL25+) | 24 | 20 | 9 | 50 | 60 | 48 | 2.6 | 26 | `vs REG: balanced; he adapts to you` | jefe del nivel alto (regla 11) |

- **Sin limps de la IA en B2–B4** (una familia de spots menos, regla 14): las manos que un fish "limpearía" las abre al
  tamaño estándar (fish que sube con basura, típico de micro); las de la station se retiran primero y su VPIP sale de
  pagar opens y defender ciegas. Limps + aislamiento: tarea P4.
- **REG adaptativo:** tras 50 manos contigo, si tu fold a 3-bet > 60 % su 3-bet sube ×1,5; si tu fold a c-bet > 55 %,
  su c-bet +15 puntos. El feedback lo dice (`REG adapts: you fold to 3-bets 68%`).
- **Reconocerlos:** nombre propio de 6 letras (`MIKA`, `ROLO`…) que **no** delata el perfil; 48 personajes por carrera,
  cada uno con perfil fijo y nivel habitual (vuelven a aparecer y el HUD los recuerda: regla 8). Icono según el nivel
  (tabla de §6.1: andamiaje que se retira). En B4, HUD con B (§6.7): `VPIP/PFR/3B/AF/FCB/WTSD` y `n` de manos; cada
  estadística sale en gris hasta su muestra mínima (VPIP/PFR 20 manos, AF 30, 3-bet y fold a c-bet 15 oportunidades).
  En B4 puedes poner tu lectura (`READ`) a cada rival; al levantarse se revela y cuenta (`READS 7/9`).
- **Cuánto se quedan** (manos objetivo al sentarse, uniforme): fish 20–60, station 30–90, maniac 15–45, nit 60–200,
  TAG 80–250, LAG 60–200, reg 100–300. Cumplido el objetivo, 15 % por mano de levantarse al final de la mano.
- **Cuándo se levantan antes:** pierde la pila → recompra fish 40 %, station 50 %, maniac 70 % (máx. 2), resto 80 %;
  si no, se va. Gana ≥ 100 bb sobre su entrada → fish y station 20 %/mano (`hit and run`); nit con +40 bb 30 %/mano.
  TAG/LAG/reg rellenan hasta 100 bb.
- **Cómo entran:** el asiento queda `EMPTY` 1–3 manos; nunca menos de 4 jugadores en la mesa (si quedan 3, se llena en
  la mano siguiente). El nuevo se elige por la mezcla del nivel, entra en la mano siguiente sin ciega muerta
  (simplificación) y sale el rótulo en la franja superior 2 s: `MIKA sits down (100bb)`. Su recuadro aparece con
  fundido de 8 frames (regla 5: nada aparece de golpe). Con 5 jugadores no hay UTG (las posiciones empiezan en HJ).

### 6.3 Pilas y profundidad efectiva

La profundidad **efectiva** (la menor entre el héroe y cada rival implicado) elige la tabla preflop y aparece en el
feedback siempre que cambia la respuesta: `Eff 25bb: 3-bet = all-in`, `SPR 1.8: top pair stacks off`.

| Efectiva | Tabla | Preflop | Postflop (SPR típico en bote subido) | Bloque | Principio |
|---|---|---|---|---|---|
| < 15 bb | push/fold | sin nota (`UNGRADED`) hasta que exista el modelo del Mundo 2 | — | B6 | no castigar sin fuente |
| 15–32 | 25 | open 2x, 3-bet = all-in, sin flat calls especulativos | SPR ≤ 3: top pair = all-in | B2 | una idea: corta = compromiso |
| 33–55 | 40 | open 2.2x (SB 2.5x), 3-bet 3x o all-in, 4-bet = all-in | SPR 3–6 | B2 | — |
| 56–125 | 100 | open 2.5x (SB 3x), 3-bet 3x IP / 4x OOP, 4-bet 2.2x, 5-bet all-in | SPR 6–13 | B2 | la tabla base |
| 126+ | 200 | open 2.5x, más pares pequeños y suited connectors (implied odds), menos 4-bet farol | SPR 13+: top pair = controlar bote | B4 (en B2–B3 se usa 100 con tolerancia doble) | — |

Pilas rivales al sentarse: corta 20–40, normal 60–120 (moda 100), profunda 150–250 (porcentajes por nivel en §6.1).

### 6.4 Puntuación de cada decisión

Escala única, color y sonido propios (lenguaje de color, regla 6):

| Nota | Preflop (tabla M, `APPROX`) | Postflop (B, `APPROX vs <PROFILE>`) | Color |
|---|---|---|---|
| `GOOD` | frecuencia de la acción ≥ 25 % en la tabla, o pérdida < 0,10 bb, o explotación correcta | pérdida < 0,25 bb o < 10 % del bote | verde |
| `OK` | pérdida 0,10–0,50 bb, o frecuencia 5–25 % (`Mixed: mostly FOLD`), o spot de baja confianza | pérdida bajo el umbral de `LEAK` o diferencia dentro del ruido | amarillo |
| `LEAK` | pérdida ≥ 0,50 bb **y** frecuencia < 5 % **y** confianza alta (umbral 1,0 bb si baja) | pérdida ≥ 1,0 bb en flop/turn o ≥ 0,5 bb en river **y** diferencia > 2 errores estándar del Monte Carlo | rojo |

**Preflop.** Tablas por profundidad, posición y acción previa (RFI, contra open, contra 3-bet, contra 4-bet, contra
all-in): frecuencia y EV de cada acción por mano (169). Tamaños: cada spot tiene uno estándar; otro tamaño de la lista se
puntúa por familia (raise/call/fold) con la nota `Standard: 2.5x` sin bajar la nota. **Explotación:** la nota final es
la **mejor** de dos: contra la tabla base y contra el rango real del bot (se conoce: §6.5). Si una de las dos da
`GOOD`, es `GOOD (exploit)`. Las decisiones all-in se valoran con la equidad exacta 169×169 contra el rango real del bot.
**Baja confianza** (umbral `LEAK` a 1,0 bb): multiway (open + caller), mesa de 5, efectiva fuera de su tabla, tamaño
del rival no estándar.

**Postflop (B3).** El rango del rival es su rango preflop (perfil × tabla) **filtrado** por sus propias reglas
postflop en cada calle (las reglas son nuestras, así que el filtro es exacto dentro del modelo). Luego:
- *Pagar o retirarse:* equidad necesaria = `call / (pot + call)` contra equidad Monte Carlo contra el rango filtrado.
  EV(call) ≈ eq × (pot + call) − call; en flop y turn se ignoran las calles siguientes (por eso el umbral de `LEAK` es
  mayor). Proyecto con pila profunda: `+ implied` suma lo que el perfil paga cuando ligas (station alto, nit bajo).
- *Apostar o pasar, y tamaño (33/50/75/POT/ALL-IN):* fold equity FE = fracción del rango que se retira ante ese tamaño
  (filtro de reglas), eq_c = equidad contra lo que paga. EV(bet) ≈ FE × pot + (1 − FE)(eq_c × (pot + 2b) − b),
  EV(check) ≈ eq × pot × realización (0,9 IP, 0,75 OOP). Se puntúa el mejor tamaño; otro a < 0,3 bb es `GOOD`.
- *Reglas de textura* para la línea del porqué: seco (A72r) → apuesta pequeña frecuente; mojado (JT9ss) → menos y más
  grande; farol sólo con equidad de reserva o bloqueadores; SPR < 2 → no hay farol, sólo compromiso.
- *Decisiones mixtas o ambiguas:* si la diferencia cae dentro de 2 errores estándar o de la tolerancia, es `OK`
  (`Close spot`), nunca `LEAK` (regla 10 y fiabilidad del trainer: el jugador debe poder fiarse de cada `LEAK`).

**Monte Carlo en la GBA:** 512 tiradas por decisión (error estándar ≈ 2,2 puntos a 50 %), hasta 40 tiradas por frame
(≤ 50 % del frame) mientras el jugador piensa; listo en ≈ 13–25 frames. Coste estimado 7.000 ciclos/tirada (2
evaluaciones de 7 cartas + sorteo); **se mide** con `gbarun.py` (regla 13) antes de cerrar B3. Semilla por mano: la
nota es reproducible.

**Feedback en mesa:** no puede parar cada mano (regla 4, ritmo). `GOOD`/`OK`: chapa de 1 línea 60 frames en el panel
sin detener la mano. `LEAK`: la mano se detiene, panel de 4 líneas (§6.7), A sigue. Opción `COACH: ALL / LEAKS / OFF`
(OFF = examen; las notas sólo salen en el resumen). Por defecto ALL en NL2–NL10, LEAKS desde NL25.

### 6.5 IA de los rivales

Barata en CPU (sin Monte Carlo para la IA), con azar reproducible (`bn::random` sembrado con semilla de sesión + nº de
mano + asiento).

- **Preflop:** cada spot tiene la ordenación de las 169 manos de la tabla base (por EV de subir y de pagar). El bot
  sube si su mano está en el `top X %` con X = X_base × factor del perfil, paga si está en la franja siguiente, si no se
  retira. Ruido: umbral ± 10 % por mano; fish y maniac hacen además una acción al azar el 8 % de las manos.

| Factor sobre la base | Nit | TAG | LAG | Station | Maniac | Fish | Reg |
|---|---|---|---|---|---|---|---|
| Abrir (RFI) | 0,6 | 1,0 | 1,35 | 0,4 | 2,0 | 1,3 | 1,05 |
| Pagar un open | 0,5 | 1,0 | 1,2 | 3,0 | 1,5 | 2,5 | 1,0 |
| 3-bet | 0,5 | 1,0 | 1,6 | 0,3 | 2,8 | 0,6 | 1,3 |
| Pagar/4-bet contra 3-bet | 0,6 | 1,0 | 1,3 | 2,0 | 1,8 | 1,5 | 1,0 |

- **Postflop:** clase de mano (1 evaluación + detector de proyectos): `MONSTER` (dobles+, trío), `STRONG` (overpair,
  top pair buen kicker), `MEDIUM` (top pair flojo, segunda pareja), `DRAW` (≥ 8 outs), `WEAK` (gutshot, carta alta),
  `AIR`. Textura `DRY`/`WET` (proyecto de color posible o 3 cartas en 5 rangos). SPR < 1,5: `MONSTER`/`STRONG` van
  all-in.

| % apostar si le pasan (MONSTER/STRONG/MEDIUM/DRAW/AIR) | % retirarse contra apuesta ≤ 50 % / > 50 % (MEDIUM, DRAW, WEAK, AIR) | % subir (MONSTER/STRONG/DRAW) | Tamaño propio |
|---|---|---|---|
| Nit 70/70/25/30/15 | 40-75, 30-60, 85-95, 100-100 | 50/10/0 | 50 % |
| TAG 75/80/40/50/35 | 20-50, 15-40, 70-90, 85-95 | 45/15/15 | 33 % seco, 66 % mojado |
| LAG 80/85/55/70/55 | 15-40, 10-30, 55-80, 70-85 | 50/20/30 (+8 % farol con AIR) | 50–75 %, river overbet 10 % |
| Station 40/35/20/15/10 | 0-10, 5-15, 30-50, 60-75 | 20/0/0 | 33 % |
| Maniac 95/95/85/90/80 | 10-25, 5-15, 40-60, 60-70 | 60/35/45 (+20 % con AIR) | 75 %–POT; all-in 20 % si SPR < 2 |
| Fish 60/60/40/40/30 | 10-35, 10-30, 45-70, 75-85 | 35/5/10 | al azar 33/50/POT |
| Reg = TAG, pero su fold total nunca pasa de alpha + 10 puntos (defiende MDF) | — | — | TAG + river 125 % polar 10 % |

Textura: en `DRY`, +10 a apostar con AIR para TAG/LAG/reg; en `WET`, −15. Coste: < 2.000 ciclos por acción.
Ritmo: cada acción de bot 18 frames (retirarse 8); tras retirarte, la mano corre ×4 y A la salta.

### 6.6 Sesión

- **Longitud:** 25 / 50 / 100 manos (50 por defecto ≈ 6 min en B3); por manos y no por tiempo (reproducible, regla 13).
- **Pausa** en cualquier momento (START: `RESUME` / `TABLE INFO` / `COACH` / `LEAVE TABLE`). La mano en curso se guarda
  como semilla + lista de acciones tras cada acción: al apagar y volver, `CONTINUE SESSION` la reconstruye exacta.
  Dejar la mesa a mitad de mano = retirarse (aviso `You fold this hand`).
- **Resumen** (3 páginas, ←/→): (1) manos, bb ganadas y **bb/100**, $ y BI, VPIP/PFR/3-bet propios; recuento
  `GOOD/OK/LEAK` y EV perdido; línea fija `Results vary. Decisions count.`; barra de ascenso
  `NL10: 142/200 hands, LEAK 6% (need <=7%)` (gradiente de meta). (2) **Las 3 fugas principales** por familia
  (posición × spot), p. ej. `BB vs BTN open: folds too much 4x -2.1bb`, con `A: DRILL` al nivel del Mundo 3 que lo
  entrena (B4). (3) repaso: hasta 10 manos con `LEAK`, reproducidas desde la semilla, decisión por decisión (B4).
- **SRAM** (se suma a la de §3; total < 14 KB de 32 KB):

| Bloque | Tamaño | Contenido |
|---|---|---|
| Carrera | 32 B | nivel actual y máximo, bankroll en centavos (u32), `NET WON`, restakes, opciones (buy-in, coach, longitud) |
| Ventana de ascenso | 6 × 80 B | por nivel: anillo de 300 notas (2 bits) + manos jugadas |
| Personajes | 48 × 24 B ≈ 1,2 KB | perfil, nivel, contadores del HUD (manos, VPIP, PFR, 3-bet/oport., c-bet/oport., fold a c-bet/oport., apuestas, pagos, showdowns), tu `READ` |
| Fugas | 40 familias × 6 posiciones × 4 B ≈ 1 KB | nº de `LEAK` y EV perdido (cbb) |
| Sesión en curso y repaso | ≈ 1 KB | semilla, pilas y asientos al empezar, mano actual (semilla + acciones), 10 manos de repaso × 64 B |
| De por vida | 32 B | manos, bb ganadas, VPIP/PFR |

### 6.7 Pantalla (240×160) y controles

Asientos en el sentido de las agujas: héroe abajo al centro, 1 abajo izquierda, 2 arriba izquierda, 3 arriba centro,
4 arriba derecha, 5 abajo derecha. Recuadro de rival 48×20: línea 1 icono 8×8 + nombre (6 letras); línea 2
posición + pila en bb (`CO 102`; decimales sólo < 10 bb). Botón de dealer: ficha 8×8 en la esquina del recuadro.
Retirado = recuadro en gris; el que actúa = borde amarillo; vacío = `EMPTY`. En showdown, 2 cartas mini 12×16 sobre
la línea 2.

| Zona | x | y | Contenido |
|---|---|---|---|
| Franja superior | 0–239 | 0–9 | `NL10 #23/50 $212.40 21BI` / rótulos de entrada y salida 2 s |
| Asientos 2, 3, 4 | 0–47, 96–143, 192–239 | 12–31 | recuadros |
| Apuestas 2, 3, 4 | bajo cada recuadro | 33–40 | ficha + importe, o la acción 30 frames (`3BET 9`, `FOLD`) |
| Tablero | 66–173 (5 × 20 px + 4 × 2) | 44–71 | cartas 20×28 |
| Bote | 90–149 | 74–81 | `POT 12.5` |
| Asientos 1, 5 | 0–47, 192–239 | 88–107 | recuadros |
| Apuestas 1, 5 | 50–89, 150–189 | 94–101 | hacia el centro |
| Cartas del héroe | 94–117, 120–143 | 84–115 | 24×32 |
| Línea del héroe | 0–239 | 118–125 | `BTN 97.5bb EFF 97  SPR 4.1` y su apuesta a la derecha |
| Contexto + tiempo | 0–239 | 127–134 | barra de 15 s (2 px) y, sólo NL2–NL5, `Call 2.5 to win 6.0 (29%)` (andamiaje) |
| Botones | 4 × 56×18 en x 2, 62, 122, 182 | 138–155 | línea 1 acción, línea 2 importe (`RAISE` / `7.5bb`) |
| Panel de feedback | 0–239 | 128–159 | tapa contexto y botones: `LEAK -1.8bb  Best: FOLD` / porqué ≤ 38 car. / consejo del perfil / `A: next  B: chart` |

Comprobación: ninguna zona se solapa (bordes de 2–4 px entre recuadros y apuestas; el recuadro 5 empieza en x 192,
la apuesta del héroe acaba en 189). Texto con `common::variable_8x8_sprite_font`: 8 px de alto, ≈ 5–6 px por carácter,
48 px caben 8 caracteres; 38 caracteres ≈ 210 px. Sprites estimados: textos ≈ 60, cartas 7 × 32×32, mini cartas 2,
fichas y dealer 7, iconos 5, botones/panel ≈ 16 → ≈ 97 de 128 (el panel sustituye a los botones). Si se pasa, la
franja superior y la línea del héroe van a texto en fondo. Captura a 1x revisada en cada bloque.

**Boceto 8-max / 9-max (torneos, B6; no cerrado):** mismo recuadro 48×20, mismo tablero, bote, héroe y botones.
Se añaden dos asientos laterales a la altura del tablero (x 0–47 y 192–239, y 46–65, apuesta debajo en y 67–74;
el tablero empieza en x 66: 18 px libres). 8-max = 3 arriba (x 0, 96, 192) + 2 laterales + 2 abajo = 7 rivales;
9-max = 4 arriba (x 0, 64, 128, 192, huecos de 16 px) + 2 laterales + 2 abajo = 8 rivales. Sprites: ≈ 109 (8-max) y
≈ 115 (9-max): en torneo la franja superior y la línea del héroe van **siempre** a texto en fondo para dejar margen.
Nombres a 5 letras si la captura a 1x no se lee.

**Controles (regla 14, los mismos del dojo):**

| Botón | En tu turno | Tras retirarte | En feedback / resumen |
|---|---|---|---|
| ←/→ | elegir entre los botones legales (máx. 4: `FOLD`, `CHECK`/`CALL`, `RAISE`/`BET`, `ALL-IN`) | — | página |
| ↑/↓ | con `RAISE`/`BET` elegido: tamaño de la lista del spot (preflop 3: p. ej. `2x/2.5x/3x`; postflop `33%/50%/75%/POT`); el cursor empieza en el estándar | — | — |
| A | confirmar (ignora A los 10 frames siguientes a aparecer los botones: búfer anti-pulsación, regla 2) | saltar al final de la mano | seguir / `DRILL` |
| B | `TABLE INFO`: iconos, pilas y efectivas; en B4, HUD y `READ` (no penaliza: un HUD es información legal) | igual | tabla del spot con tu mano (como en el dojo) |
| START | pausa | pausa | pausa |

El cursor empieza en `CHECK` si es legal, si no en `FOLD` (nunca en la acción correcta: no la delata). L, R y SELECT
siguen sin uso.

**Primer minuto de la carrera:** la primera vez, mesa NL2 con semilla fija; rótulo `Your turn: LEFT/RIGHT to choose,
A to confirm.` Mano 1: héroe en BTN con AKs y todos se retiran → `RAISE` → `GOOD` con sonido. Manos 2–3 también CLEAR
(fold de 72o en UTG, call en BB con buena mano); nada cuenta en la ventana de ascenso en esas 3 (regla 3, presentar en
sitio seguro; regla 10). En la mano 4 entra un rival con su rótulo (se ve antes de que importe, regla 5).

### 6.8 Bloques del modo carrera (cada uno jugable solo)

| Bloque | Entra | No entra | Prueba de aceptación |
|---|---|---|---|
| **B2** | mesa 6-max greybox con el layout de §6.7; ciegas, posiciones y botón que rotan; acciones preflop con tamaños; IA preflop por perfil (sin reg, sin LAG en NL2); entrada y salida de rivales con rótulo; pilas 20–250 con tablas 25/40/100; **si se llega al flop sin all-in, se reparte el resto y se resuelve en showdown** (check-down); all-in: se reparte y se resuelve; puntuación preflop GOOD/OK/LEAK con feedback; NL2–NL10, bankroll, stake, ascenso y descenso; sesión de 25/50/100 con resumen página 1 y fugas página 2 (sin `DRILL`); iconos visibles; SRAM de carrera | postflop jugado, HUD, `READ`, repaso de manos, NL25+ | 50 manos con `gbarun.py` sin cuelgues; capturas de mesa llena, entrada de rival, `LEAK` y resumen revisadas a 1x; simulación de PC de 100.000 manos: VPIP/PFR de cada perfil ±3 puntos |
| **B3** | tablero y calles; IA postflop por perfil; rango filtrado + Monte Carlo; puntuación postflop con tamaños; SPR en la línea del héroe; showdown con mini cartas; MD-6…MD-8 | NL25+, HUD | Monte Carlo ≤ 25 frames medido; ningún `LEAK` en spots con diferencia < 2 SE (prueba de PC con 1.000 spots) |
| **B4** | NL25–NL100, REG adaptativo, tabla de 200 bb, HUD con B, iconos que se retiran, `READ`, repaso de manos, `DRILL` al Mundo 3, `CHANGE TABLE` desde la pausa | limps (P4), rake (P4) | ascenso NL2→NL100 completo con bot de entrada en `gbarun.py`; HUD de un personaje igual tras apagar y encender |

La carrera de B2 se guarda con su versión; al llegar B3 (el resultado de las manos cambia al jugarse postflop) se avisa
`SAVE RESET: career v2` y empieza de nuevo en NL2 (regla del §3: nunca se borra en silencio). En B2 los bb/100 de una
station salen inflados por el check-down; el resumen de B2 lo dice (`Preflop-only table`).

## 7. MATH DOJO

**Frase:** niveles cortos (10 preguntas, ≈ 2 min) de la aritmética del póker, de contar outs a ICM. Todo **E**: se
calcula exacto en la GBA (enumeración o fórmula) o en el PC (plantillas de proyectos). Cinturón propio (blanco →
negro de matemáticas) en una sala `MATH DOJO` del mapa.

**Formato:** dos tipos de respuesta. *Opciones* (3–4 botones con ←/→, como el dojo). *Número* (selector: ↑/↓ ±1, ←/→
±10, A confirma; empieza en el centro del rango plausible, nunca en la respuesta). Andamiaje dentro del nivel:
preguntas 1–4 con opciones, 5–10 con número, y las del final cerca del umbral (CLEAR → EDGE, como en §2). Feedback con
la fórmula y sus números en una línea: `Need = 5 / (10+5+5) = 25%`. Aprobar: **8/10** (un número dentro de la
tolerancia cuenta como acierto); estrellas: ★ 8/10, ★★ 9/10, ★★★ 10/10 sin pasarse de 15 s por pregunta.

| Nv | Objetivo | Pregunta y respuesta en la GBA | Tolerancia | Generación | Fuente | Bloque |
|---|---|---|---|---|---|---|
| MD-1 | outs y regla del 2 y del 4 | mano + flop: `How many outs?` (opciones 4/8/9/15), luego `Equity to river?` (número %) | outs exactas; % ±4 puntos del exacto (la regla del 4 se acepta) | plantillas de PC (color, OESD, gutshot, overcards, combos) con palos al azar; outs y equidad contadas en la GBA (enumeración de 47 cartas) | E | 2b |
| MD-2 | pot odds | bote P, apuesta B: `Equity needed?` (opciones 20/25/33/40 %, luego número); al final `CALL/FOLD` con equidad dada | ±2 puntos | tamaños 25/33/50/66/75/100/150 % del bote; EDGE = equidad a < 4 puntos del umbral | E (B/(P+2B)) | 2b |
| MD-3 | equidad mano contra mano y contra un rango sencillo | `AKo vs QQ?` (opciones 30/45/55/70 %, luego número); flop: mano contra mano; preflop contra `QQ+, AK` | ±5 puntos | arquetipos (par contra overcards, dominada, coinflip, suited vs offsuit) y rangos de 2–4 grupos | E (tabla 169×169 y enumeración en la GBA) | 2b |
| MD-4 | pila efectiva y SPR | mesa de 3–4 pilas: `Effective stack vs SEAT 4?` (opciones); `SPR?` (número, 0,1); `Top pair, SPR 2: STACK OFF / POT CONTROL` | efectiva exacta; SPR ±0,3 | pilas 20–250 bb, botes 3–30 bb | E (aritmética) + zonas de compromiso M (SPR < 3 TP+, 3–6 overpair/TPTK, > 10 dobles+) | 2b |
| MD-5 | implied odds | `22 vs 3bb open, 100bb deep: CALL/FOLD`; `How much must you win when you hit?` (número bb); contraste 100 bb contra 20 bb | ±10 % | pares 22–99, opens 2–4 bb, efectivas 20–200; supuesto mostrado: ganas W si ligas trío (11,8 %) y pierdes el call si no | E: W ≥ (1−p)·C/p − P | 2b |
| MD-6 | EV de una decisión | `EV of CALL?` (opciones en bb, luego número) con equidad, bote y apuesta dados; luego elegir la de mayor EV entre 2 líneas | ±0,2 bb | botes 5–60 bb | E: eq·(P+C) − C | 3 |
| MD-7 | fold equity y farol | `Bet 75% pot: how often must he fold?` (número %); `He folds 40%: profitable?` `YES/NO`; semifarol con equidad de reserva | ±2 puntos | tamaños 33–200 %; FE 10–70 % | E: B/(P+B); EV = FE·P + (1−FE)(eq·(P+2B) − B) | 3 |
| MD-8 | MDF y alpha | `MDF vs 1/2 pot?` (opciones 50/60/67/75 %, luego número); `You defend 40% vs 75% pot: OVERFOLD / OK` | ±2 puntos | tamaños 25–200 % | E: alpha = B/(P+B), MDF = P/(P+B) | 3 |
| MD-9 | M de Harrington y zonas | pila, ciegas y ante: `M?` (número, 0,5); `Zone?` (4 de GREEN ≥20 / YELLOW 10–20 / ORANGE 6–10 / RED 1–5 / DEAD <1) | M ±0,5; zona exacta | pilas 2–60 bb, mesas de 6/8 con y sin ante | E (M = pila / (SB+BB+antes)) + zonas convencionales | 6 |
| MD-10 | ICM básico | 3 jugadores, premios 50/30/20, pilas dadas: `Your $ equity?` (número %); `Who loses most per chip?` | ±2 puntos | pilas que suman 100 % en pasos de 5 | E dentro del modelo Malmuth-Harville (cálculo exacto en la GBA) | 6 |
| MD-11 | burbuja | 4 jugadores, paga 3 (50/30/20): `Equity needed to call this shove?` chip EV contra ICM (opciones, luego número) | ±3 puntos | push del líder o del corto; pilas 10–60 % | E (Malmuth-Harville, enumeración de los 2 resultados del all-in) | 6 |

Coste: el bucle de ejercicios del bloque 1 + un selector numérico; sin tablas grandes (sólo plantillas de proyectos de
MD-1, < 4 KB). SRAM: 11 niveles × 4 B en la tabla de niveles existente (hay sitio: 64 entradas).

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
| (2026-11-23, decisión del usuario) POKET GTO es un trainer general **online**: cash = 6-max por defecto (full ring 9 sólo como futuro modo `LIVE`); MTT = 8-max por defecto, 9-max como variante, mesa final del mismo formato; spin/3-max y heads-up sólo en niveles puntuales (Mundo 1, MD). Cambios: 2-1 pasa a 8-max, 3-6 deja de ser 9-max (giro = profundidad al azar), bloque 6 con mesa 8-max, boceto 8/9 en §6.7 | formatos más jugados online; una idea por formato (regla 3) | layout de 6 cerrado en B2; el de 8 se valida con captura a 1x antes de B6 |
| (2026-11-23) Modo carrera cash 6-max pasa a los bloques 2–4; el resto del dojo se corre a 5–7 y el Mundo 3 se absorbe como `DRILL` (mismas tablas) | petición del usuario ("trainer real de mesas"); regla 1 (mismo verbo); alcance: cada bloque jugable | B2 jugable con 3 tablas de profundidad y 0 postflop |
| Ascenso por calidad de decisiones (ventana 200/300 manos, `LEAK` ≤ 10→5 %) y descenso por bankroll (< 15 BI); el maestro banca hasta 25 BI | gradiente de meta; juzgar proceso y no resultado; gestión de bankroll real; regla 9 (`RESTAKE`, sin bloqueo) | tiempo medio por nivel 15–40 min en pruebas; nº de descensos por sesión ≈ 0 en juego correcto |
| Nota final = la mejor entre tabla base y rango real del bot; `LEAK` sólo con confianza alta y fuera de 2 SE | regla 10; fiabilidad del trainer (nunca castigar lo razonable) | prueba de PC: 0 `LEAK` en jugadas de frecuencia ≥ 5 % o diferencia < 2 SE |
| Postflop puntuado contra el bot (marca **B**), no contra "GTO" | honestidad del feedback: los niveles P siguen sin publicarse | rótulo `vs <PROFILE>` en todo feedback postflop |
| `GOOD`/`OK` no detienen la mano; `LEAK` sí | regla 4 (ritmo con silencios); regla 6 | ≥ 8 manos/min en B2 con COACH ALL |
| Iconos de perfil visibles en NL2–NL5, después de n manos en NL10–NL25, nunca en NL50+ | andamiaje que se retira; regla 3 (giro en NL100) | % de `READ` correctos en B4 ≥ 70 % tras 40 manos |
| IA sin limps (B2–B4) | regla 14 y alcance (una familia de spots menos) | — ; limps como P4 |
| Sin rake en el modelo ni en el juego | coherencia tabla-juego; alcance | — ; riesgo anotado |
| B: `TABLE INFO` en la carrera (sin penalizar), tabla del spot sólo tras decidir | un HUD es información legal; estudio y examen separados (Bjork) | % de manos con B abierto ≤ 30 % tras NL5 |
| MATH DOJO como mundo propio; 0-4 y 0-5 pasan a MD-1/MD-2; puertas matemáticas para subir de mesa y entrar en cada mundo | petición del usuario; regla 7 (cerradura antes que llave: cada nivel de matemáticas abre varios modos); andamiaje | acierto del primer intento en MD-n ≥ 70 %; ningún jugador bloqueado por una puerta de nivel no publicado |
| MATH DOJO I (MD-1…MD-5) en un bloque 2b propio tras B2; II con B3; III con los torneos (B6) | coste bajo (todo E, reutiliza el bucle del bloque 1); el prerrequisito llega justo antes de usarlo | 2b cabe en < 1/3 del esfuerzo de B2 (estimar con `grep` al abrir la tarea) |

## Riesgos

| Riesgo | Mitigación |
|---|---|
| Rangos M equivocados enseñan errores | etiqueta `APPROX`, tolerancia 0,10 bb, pruebas de sanidad (monotonía por posición y pila); los P no se publican |
| Cartas y mesa ilegibles a 240×160 (6/8 asientos) | cartas del héroe 24×32 grandes; rivales sólo como etiqueta + pila; captura a 1x revisada en cada bloque |
| Monotonía (20 manos iguales) | giro en el último nivel de cada mundo, repaso intercalado, rachas |
| Alcance (7 mundos) | bloque 1 con 4 niveles y sólo datos E; cada bloque es publicable por sí solo |
| SRAM corrupta o de versión vieja | cabecera con magia y versión; migración o aviso explícito |
| Modo carrera demasiado grande (mesa, IA, churn, bankroll, puntuación) | B2 sólo preflop con check-down; HUD, repaso y NL25+ fuera hasta B4 |
| Tablas preflop M sin rake difieren de las mesas reales de micro (más fold en BB con rake) | aviso en la entrada de la carrera (`No rake model`); rake como P4 |
| Bots predecibles o perfiles indistinguibles | simulación de PC de 100.000 manos con estadísticas ±3 puntos; ruido por mano; personajes que vuelven |
| Monte Carlo lento en la GBA (ARM7 16,78 MHz) | medir en B3; si > 25 frames, bajar a 256 tiradas y subir el umbral de `LEAK` |
| 6 asientos + texto superan 128 sprites o no se leen | recuento en §6.7 (≈ 97); plan B: franjas de texto en fondo; captura a 1x |
| Feedback postflop "contra el bot" enseña a explotar bots, no personas | perfiles calibrados a cifras de población reales; rótulo `vs <PROFILE>`; consejos generales, no trucos del bot |
| La carrera es larga (≥ 1.100 manos) y el usuario abandona | sesiones de 50 manos ≈ 6 min, barra de ascenso visible, guardado tras cada acción |
