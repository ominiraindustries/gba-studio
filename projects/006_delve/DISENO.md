# DELVE — diseño

**Frase**: bajas por una mazmorra oscura generada con semilla, casilla a casilla, y luchas en tiempo real con
monstruos que avisan antes de golpear. **Verbo central**: *esquivar y castigar* (apartarse del golpe que se ve
venir y contraatacar). Referencia de género: Dungeon Hack / Eye of the Beholder (vista por casillas, tiempo real).
Las reglas y los números son propios, sin nada de D&D. Todo el texto del juego va en **inglés**. La pantalla, la vista
y los formatos están en `ESPEC_VISTA.md` (no se cambian aquí). Reglas citadas: R1–R14 de `docs/guias/diseno-juegos.md`.

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
| primer monstruo visible | `A: ATTACK WHEN THE BAR IS FULL` |
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
fija lo que afecta al juego): planta de **32×32** casillas, ~**47 %** abierta (~480 casillas de suelo), **15–20
callejones**, conectividad garantizada sin teleportadores, entrada de la planta N en las coordenadas de la escalera de
N−1, y la llave nunca más honda que su cerradura. Zona por planta en el bloque 1–2: **salas + laberinto** en las
plantas 1–3 (la más legible para aprender); el laberinto puro y el cruce sin salas quedan para plantas 4+ (bloque 3),
como "giro" de una planta suelta (R3).

- Semilla de partida de 32 bits (visible en pausa y en la pantalla de muerte); semilla de planta = hash(partida,
  planta). Misma semilla = misma planta (R13: entrada reproducible).
- **Sala segura** (R4): el motor fuerza una sala de **3×3** alrededor de la entrada (la llegada mira hacia la salida
  de la sala), sin monstruos a menos de **8 casillas por recorrido** ni con vista directa (≤ 3 casillas en línea) desde
  la sala. Escalera de bajada en el callejón más lejano por recorrido desde la entrada.
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
| 1 | 9 | rat ×4, bat ×3, slime ×2 | 7 (3 pan, 2 pociones, short sword, leather) | 0–1 | rat solo en el 1.er pasillo |
| 2 | 12 | bat ×2, slime ×2, skeleton ×5, cultist ×3 | 8 (+ mace o chain, 1–2 pergaminos, 1 llave por cerradura) | 1–2 | skeleton solo, luego cultist |
| 3 | 13 + jefe | skeleton ×6, cultist ×4, slime ×2, bat | 8 (+ axe/plate tras llave) | 1–2 | jefe en sala 5×5 antes de la escalera |

- Densidad ≈ 1 monstruo cada 35–50 casillas de suelo: con paso de 12 f, ~1 encuentro cada 15–20 s de exploración,
  con silencios entre medias (R4).
- **Dientes de sierra**: dentro de la planta la dificultad sube con la zona y baja al empezar la siguiente (sala
  segura + subida de nivel probable). Antes de la sala del jefe, una sala vacía con 1 poción garantizada (R4, R10).
- Riesgo de forma: 47 % de abierto y 15–20 callejones en 32×32 da plantas largas (~6–8 min). Si el bot mide > 10 min
  por planta, bajar el abierto, no el tamaño.

## 7. Muerte y progreso
- **Reintento de la planta, no permadeath** (R10: fallar cuesta poco). Al morir: `YOU DIED` y en < 5 s vuelves al
  inicio de la misma planta (misma semilla) con el estado guardado al bajar a ella. Opción `NEW RUN` en la misma
  pantalla para semilla nueva. El modo "una vida" queda como opción futura (P4).
- **Guardado en SRAM** al bajar cada escalera (y al empezar la partida): semilla, planta, nivel, XP, HP, inventario,
  equipo y mensajes ya enseñados. Un solo hueco; `CONTINUE` / `NEW GAME` en el título.
- **Meta del bloque 1 del juego completo**: 3 plantas y el jefe al final de la 3 → pantalla `THE DEPTHS AWAIT`
  (fin de la demo) con tiempo total y muertes.

## 8. Primer minuto (checklist de la guía)
1. Título → `NEW GAME`. Apareces en la sala segura de 3×3 mirando a la única salida, con la daga equipada, la
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
| 1 | Una planta generada jugable con arte (texturas + 1 monstruo dibujado), movimiento y giros, puertas sin llave, rat y skeleton, combate completo (barra, esquiva, crítico), recoger pan, poción y short sword (sin pantalla de inventario: contador de pociones en el panel, B la usa), bajar escaleras → planta nueva con otra semilla | `gbarun.py` con entrada grabada: sala inicial, matar la rata, bajar; capturas revisadas |
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

## 11. Riesgos y propuestas para `ESPEC_VISTA.md`
- **Redibujo del modo 4 al dar un paso**: si pasa de 2 frames, la recarga de 12 f lo oculta; si pasa de 6 f se nota.
  Medir con `gba-performance` en el bloque 1.
- **Tiempo real con giros de 90°**: un monstruo a la espalda es injusto si no se ve; por eso el aviso ×1,5 y la flecha.
  Validar con capturas.
- **Sonido de aviso** antes del bloque 3: la guía pide respuesta sonora; propongo 3 efectos mínimos ya en el bloque 1
  (golpe, aviso, recogida).
- **Generador**: el mayor riesgo de bloqueo; la prueba de 1000 semillas va con el generador, no después.
- Propuestas (no cambio la espec, decide el motor/artista): (a) falta el formato de los **objetos en el suelo** y de
  la **bola de fuego**: propongo sprite base 32×32, 16 colores, escalado igual que los monstruos y apoyado en el
  suelo; (b) reservar en la vista un hueco para las **flechas de ataque lateral** (borde izq./der., 8×16);
  (c) en la franja inferior, la **barra de recarga** (≈ 48×4 px junto al arma) y la línea de mensajes (≤ 28
  caracteres con la fuente de 8 px) deben caber en los 40 px de alto: comprobarlo en `ui_frame`.
