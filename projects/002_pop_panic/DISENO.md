# POP PANIC — documento de diseño

**Frase**: un chico con un arpón da la vuelta al mundo limpiando de bolas que rebotan y se parten diez capitales.
Género "disparar hacia arriba a bolas que se parten" (referencia: Super Pang); nombre, personaje, ruta, escenarios,
enemigos y niveles son propios. Estilo retro colorido. **Todo el texto del juego en inglés.**
Estructura: 10 capitales × 2 niveles = **20 niveles** de una pantalla; el 2.º de cada ciudad usa el mismo fondo y es
más difícil (pico); el 1.º de la ciudad siguiente baja (descanso). Jefe en el nivel 20.

## 0. Tamaños (decisión)
Fijado por el usuario: jugador en sprite 16×16 (personaje ≈ 12×15), suelo fino de 8 px (filas 152–159) con el HUD
encima, zona de juego 240×152 (`floor_y = 152`).
**Decisión: bolas de 32 / 24 / 16 / 8 px de diámetro** (sprites 32×32, 32×32, 16×16, 8×8). Sustituye a 48/32/16/8:
con un jugador de 15 px, una bola de 48 mediría 3.2 alturas de jugador (el género ronda 1.7) y taparía el 20 % del
ancho. El artista debe dibujar con estos tamaños.

Comparación con el género (arcade 384×224 escalado ×0.625; cifras de capturas, ±15 %):

| Medida | Referencia escalada | 48/32/16/8 + jugador 15 | **32/24/16/8 + jugador 15** | Principio |
|---|---|---|---|---|
| Bolas (px) | ≈ 30 / 20 / 10 / 5 | 48 / 32 / 16 / 8 | 32 / 24 / 16 / 8 | proporción del género (regla 1) |
| Bola grande / alto jugador | ≈ 1.7 | 3.2 | 2.1 | la grande se lee "enorme" sin agobiar |
| Bola grande / ancho pantalla | 12.5 % | 20 % | 13 % | espacio para esquivar |
| Bola mínima a 1x | 5 px (ilegible en GBA) | 8 | 8 | legibilidad: no bajar de 8 |
| Pantalla en anchos de jugador | ≈ 15–16 | 20 | 20 | más sitio que el arcade: margen a favor |
| Zona en altos de jugador | ≈ 7 | 10 | 10 | rebotes con aire por encima |

Se descarta 30/20/10/5 (copia exacta de la escala): la de 5 px no se ve. El salto 32→24 es menor que en el género,
pero se distingue a 1x (8 px) y cada tamaño lleva su color.

**Medido en la ROM greybox anterior** (gbarun, START en f 30, GO en f ≈ 131, sin tocar nada):
- El jugador quieto muere ≈ 40 f después de GO: la bola del nivel 1 nace en x 60, y 50, **moviéndose hacia él** a
  1 px/f. Rompe la regla 5 y el antipatrón "muere al aparecer". Disparando en f 140 la bola llega por el lado antes
  de que se pueda reaccionar.
- `ball_gravity 0.1` + `ball_bounce_speed[0] 4.47` = rebote de 100 px: la bola grande toca el techo en cada bote.
- `ball_speed_x = 1` igual para todos: la grande pasa sobre el jugador demasiado rápido para leerla (flujo).

Cifras con los valores de §2 (zona 152, caja del jugador 8×12):
- **Pasar por debajo**: la bola pasa si su borde inferior > 14 px. Fracción de cada bote por debajo de 14 px:
  32 ≈ 9 f de 108, 24 ≈ 10 de 93, 16 ≈ 13 de 75, 8 ≈ 20 de 55 (36 %). Bajo la 32 y la 24 se pasa casi siempre;
  bajo la 8 (ápice 24 px, cruza la caja en 16 f) sólo con buen momento → hay que dispararla (riesgo/recompensa).
- **Hueco para esquivar**: la 32 contra una pared deja 240 − 32 − 8 = 200 px; dos 32 en el centro dejan dos pasillos
  de ≥ 70 px (≈ 6 anchos de jugador).
- **Jugador 1.5 px/f contra bola máx. 1.3 px/f** (8 × factor 1.30): siempre se huye; cruzar la pantalla 152 f.
- **Arpón**: hoy 3 px/f; desde la cabeza (y 137) tarda 46 f al techo. Decisión **4 px/f (34 f)**: la punta llega al
  borde inferior de la 32 en su ápice (y 60) en 19 f; la 8 cruza la columna del arpón en 14 f. Toda la cuerda golpea.
- **Bolas pequeñas a la vez**: una 32 da como mucho 8 de 8 px (64 de 240 px ocupados). Tope de diseño **≤ 16 vivas**
  (2 cascadas completas); el nivel 18 (3 × 32) llega a 24 sólo en teoría. Si el bot ve > 16, pasa a 32 + 32 + 24.

**Cambios pedidos en `include/pop_tuning.h`** (no editado; para el implementador):
`ball_diameter {48,32,16,8} → {32,24,16,8}` · `ball_gravity 0.1 → 0.0625` · `ball_speed_x 1 →` por tamaño
`{0.625, 0.75, 0.875, 1.0}` × factor de ciudad · `ball_bounce_speed → {3.39, 2.92, 2.35, 1.73}` (h 92/68/44/24) ·
`ball_split_hop 2.5 → 1.75` · `harpoon_speed 3 → 4` · `player_hit_width/height → 8 / 12` ·
`ball_points → {20, 40, 80, 160}` · bolas iniciales en el ápice (centro y = 44 / 72 / 100 / 124 para 32/24/16/8),
`vy = 0`, dirección **hacia fuera** del jugador y centro a ≥ 64 px de él (tabla §3). `ready_frames 100` y
`dying_frames 70` valen (golpe → control ≈ 2.8 s < 5 s, regla 10).

## 1. Verbo, bucle, meta y controles
- **Verbo**: disparar hacia arriba (colocarse debajo + A). Moverse sólo sirve para apuntar y esquivar (regla 1).
- **Bucle** (5–10 s): ver el bote → colocarse → disparar → la bola se parte → esquivar las hijas → repetir.
- **Meta del nivel**: no dejar ninguna bola. **Meta de la partida**: llegar a Washington D.C. y vencer al jefe.
- **Controles** (regla 14): cruceta ←/→ mover, **A** disparar, **START** pausa. Nada más en todo el juego (sin
  escaleras: añadirían ↑/↓ y otro verbo). Búfer de A de 6 frames: si se pulsa con el arpón aún en pantalla, sale en
  cuanto desaparece (regla 2).

## 2. Números base (60 fps, `bn::fixed`, 12 bits fraccionarios)
Zona de juego y = 0..152 (suelo en y = 152), x = 0..240; filas 152..159 = suelo fino con el HUD encima.
Altura de rebote = altura del **borde inferior** de la bola sobre el suelo en el ápice. Al tocar suelo la bola recibe
siempre `vy = -v_rebote` de su tamaño (`v = sqrt(2·g·h)`), así cada tamaño rebota siempre igual.

| Parámetro | Valor | Principio |
|---|---|---|
| Gravedad de las bolas | 0.0625 px/f² (raw 256), igual en todo el juego | anticipación: arco aprendible (flujo) |
| Bola 32 (sprite 32×32) | vx 0.625 · rebote 92 px (v 3.39) · vuelo 108 f · ápice: borde sup. y 28 | lenta y alta: se pasa por debajo |
| Bola 24 (sprite 32×32) | vx 0.75 · rebote 68 px (v 2.92) · vuelo 93 f · borde sup. y 60 | paso intermedio |
| Bola 16 (sprite 16×16) | vx 0.875 · rebote 44 px (v 2.35) · vuelo 75 f · borde sup. y 92 | ya no siempre se pasa por debajo: tensión |
| Bola 8 (sprite 8×8) | vx 1.0 · rebote 24 px (v 1.73) · vuelo 55 f · borde sup. y 120 | rápida y baja: riesgo alto, más puntos |
| Factor de ciudad | multiplica sólo vx (1.00 → 1.30, ver §3); alturas y gravedad no cambian | la dificultad sube sin romper lo aprendido |
| Paredes / techo | rebote elástico en x; techo: si el borde superior < 0, `vy = 0` | nunca sale de pantalla (regla 9) |
| Impulso al partirse | 2 hijas en el centro de la madre, `vx = ±vx_hija`, `vy = -1.75` (sube ~24 px) | se ve la partición antes de que caigan (regla 5) |
| Jugador | 1.5 px/f, sin aceleración ni inercia | > bola más rápida (1.0 × 1.30 = 1.3): siempre se puede huir |
| Arpón | punta sube 4 px/f desde la cabeza (y 137; 34 f al techo); se queda en la x del disparo; 1 a la vez | toda la cuerda (suelo→punta) golpea |
| Caja del arpón | 6 px de ancho (dibujo 2–3 px) contra el círculo de la bola + 1 px | caja ofensiva generosa (regla 2) |
| Caja de daño del jugador | 8×12 px, pegada a los pies y centrada (personaje 12×15 en sprite 16×16) | perdona roces de cabeza y bordes (regla 2) |
| Caja de la bola contra jugador | círculo de radio r−2 (32, 24) y r−1 (16, 8) | nunca "me mató sin tocarme" |
| Puntos por bola | 32: 20 · 24: 40 · 16: 80 · 8: 160 | riesgo/recompensa: lo pequeño es más peligroso |
| Bonus de nivel | 500 × nº de nivel (+ 20 × s sobrantes desde el bloque 2) | gradiente de meta |
| Vidas | 3 al empezar; máximo 5; vida extra cada 20 000 puntos (bloque 2) | aversión a la pérdida sin castigo largo |
| Invulnerabilidad al reaparecer | 60 f tras GO, parpadeo cada 4 f | red de seguridad (regla 10) |
| Hitstop | romper bola: 2 f (32: 3 f); morir: 30 f | cada acción tiene respuesta |

**Roturas** de una bola: 32 = 15, 24 = 7, 16 = 3, 8 = 1 (las tablas de §3 usan estos tamaños).

## 3. Ruta y curva de dificultad (dientes de sierra)
Ruta propia, de oeste a este y vuelta por el Pacífico; empieza en casa (Madrid) y termina en Washington D.C., donde
cabe un jefe "de capital". Regla de seguridad en **todos** los niveles (regla 5, antipatrón "muere al aparecer"):
jugador en x = 120; cada bola empieza **en su ápice**, con su centro a ≥ 64 px del jugador, moviéndose **hacia
fuera** y congelada durante READY/GO. Tiempo límite sólo desde el bloque 2 (≈ 2× el esperado); a 10 s el contador
parpadea en rojo; agotarlo cuesta una vida.

| Nº | Ciudad | Bolas iniciales (tamaño @ x, dir.) | Roturas | Factor vx | Novedad / idea (regla 3) | Esperado | Límite |
|---|---|---|---|---|---|---|---|
| 1 | Madrid | 24 @ 40 ← | 7 | 1.00 | presentar el arpón | 20 s | — |
| 2 | Madrid ▲ | 32 @ 192 → | 15 | 1.00 | cascada completa; presenta DOUBLE | 35 s | — |
| 3 | Paris ▽ | 24 @ 40 ←, 16 @ 216 → | 10 | 1.05 | dos focos pequeños; presenta SHIELD | 25 s | — |
| 4 | Paris ▲ | 32 @ 40 ←, 24 @ 200 → | 22 | 1.05 | cascada + segundo foco | 50 s | — |
| 5 | London ▽ | 16 @ 24 ←, 16 @ 56 ←, 16 @ 216 → | 9 | 1.10 | presenta PIGEON; lluvia baja | 30 s | — |
| 6 | London ▲ | 32 @ 192 →, 24 @ 40 ← | 22 | 1.10 | presenta ladrillos rompibles | 55 s | 110 |
| 7 | Rome ▽ | 24 @ 40 ←, 24 @ 200 → | 14 | 1.15 | presenta columnas irrompibles; STICKY | 35 s | 80 |
| 8 | Rome ▲ | 32 @ 32 ←, 32 @ 208 → | 30 | 1.15 | dos cascadas entre columnas | 65 s | 130 |
| 9 | Cairo ▽ | 32 @ 200 → | 15 | 1.20 | presenta SCARAB; FREEZE | 40 s | 90 |
| 10 | Cairo ▲ | 32 @ 120 (alto), 24 @ 24 ←, 24 @ 216 → | 29 | 1.20 | escarabajos + bloques de arenisca | 65 s | 130 |
| 11 | Moscow ▽ | 24 @ 40 ←, 24 @ 200 →, 16 @ 24 ←, 16 @ 216 → | 20 | 1.20 | presenta carámbanos; RAPID | 45 s | 100 |
| 12 | Moscow ▲ | 32 @ 32 ←, 32 @ 208 →, 16 @ 120 (alto) | 33 | 1.20 | carámbanos + doble cascada | 75 s | 150 |
| 13 | New Delhi ▽ | 32 @ 40 ←, 24 @ 200 → | 22 | 1.25 | presenta MONKEY; DYNAMITE | 45 s | 100 |
| 14 | New Delhi ▲ | 32 @ 32 ←, 32 @ 208 →, 24 @ 120 (alto) | 37 | 1.25 | monos + plataformas de templo | 80 s | 160 |
| 15 | Beijing ▽ | 24 @ 24 ←, 24 @ 64 ←, 24 @ 216 → | 21 | 1.25 | presenta KITE; 1UP garantizado | 45 s | 100 |
| 16 | Beijing ▲ | 32 @ 32 ←, 32 @ 208 →, 24 @ 64 ←, 24 @ 176 → | 44 | 1.25 | cometas + palomas a la vez | 90 s | 180 |
| 17 | Tokyo ▽ | 8 × 16 en fila alta (x 24..216, alternas) | 24 | 1.30 | giro: sin grandes, lluvia de 16 | 50 s | 100 |
| 18 | Tokyo ▲ | 32 @ 32 ←, 32 @ 120 (alto), 32 @ 208 → | 45 | 1.30 | pico del juego: 3 cascadas + ladrillos | 95 s | 190 |
| 19 | Washington ▽ | 32 @ 40 ←, 24 @ 200 → | 22 | 1.30 | descanso antes del jefe; objetos ×2 | 50 s | 110 |
| 20 | Washington ★ | jefe THE BIG BLIMP | — | 1.30 | examen final (regla 11) | 120 s | — |

▲ pico, ▽ descanso, ★ jefe. Cada ▽ tiene menos roturas que el ▲ anterior (10 < 15, 9 < 22, 14 < 22,
15 < 30, 20 < 29, 22 < 33, 21 < 37, 24 < 44, 22 < 45) y su novedad se presenta en ese descanso (regla 3, presentar en
sitio seguro; regla 4, ritmo con silencios). "(alto)" = la bola empieza en el centro pero a la altura máxima, y se
mueve hacia el lado contrario del jugador (que en esos niveles empieza en x = 40). Partida completa ≈ 20 min.

### Niveles 1–5 en detalle (bloque 1)
- **1 MADRID · FIRST POP**: una bola 24 a la izquierda, alejándose. Rebota en la pared y vuelve a los ~2 s de GO.
  Se aprende que el arpón parte la bola y que las hijas saltan. Sin objetos (regla 1: el verbo solo). Medir: < 30 s
  al primer intento.
- **2 MADRID · BIG ONE**: una 32 a la derecha. La **primera rotura suelta siempre DOUBLE**, con la bola aún lejos
  (presentar en sitio seguro). Con dos arpones la cascada de 8 bolas de 8 se vuelve manejable: el objeto se entiende
  al usarlo. Idea: no romper todo a la vez. Pico de Madrid.
- **3 PARIS · SIDE BY SIDE**: una 24 a la izquierda y una 16 a la derecha, ambas alejándose. La primera rotura suelta
  siempre **SHIELD**; el jugador lo coge sin peligro y aprende que lo protege una vez. Baja la carga (10 roturas).
- **4 PARIS · TOWER**: 32 izquierda + 24 derecha, convergen hacia el centro. Objetos al azar (DOUBLE, SHIELD). Pico.
- **5 LONDON · PIGEONS**: tres 16 bajas. A los 8 s de GO aparece la primera **PIGEON** (con aviso, ver §6) cuando
  quedan pocas bolas y nunca con DOUBLE recién perdido. Idea: sincronizar el disparo con el paso del pájaro. Descanso
  en roturas (9), giro en la amenaza.

## 4. Primer minuto
1. Título: "POP PANIC" y "PRESS START" parpadeando. START → tarjeta de etapa.
2. Tarjeta (90 f): "STAGE 1 - MADRID" sobre el fondo de la ciudad; en el bloque 2, mapa del mundo con la ruta
   punteada y un avión que vuela hasta la ciudad (sólo al cambiar de ciudad; entre los dos niveles de una ciudad,
   tarjeta de 45 f). A o START la salta.
3. Nivel 1: jugador en el centro; la bola 24 arriba a la izquierda, quieta. Rótulo "A: SHOOT" (regla 6: una línea en
   el momento). "READY" 60 f → "GO!" 30 f. En READY el jugador **puede moverse** pero no disparar; las bolas no se
   mueven. El rótulo se va al primer disparo.
4. Cada rotura: destello de 4 f + "+40" flotando 30 f + hitstop; los puntos del HUD cambian.
5. Al morir: hitstop 30 f con la bola culpable parpadeando (se ve qué mató), el jugador cae 30 f, −1 vida. El nivel se
   **reinicia con sus bolas iniciales** (niveles cortos; posición segura garantizada) → READY/GO → 60 f de
   invulnerabilidad. Del golpe al control ≈ 2.5 s (regla 10, < 5 s). Se pierde el arma especial, no el escudo sin usar.
6. Sin vidas: "GAME OVER" 90 f → "PRESS A TO RETRY": vuelve al **mismo nivel** con 3 vidas y puntos a 0 (regla 10).
   START en GAME OVER → título.
7. Fin del bloque 1 (nivel 5): "TO BE CONTINUED" con los puntos; A → título. Juego completo: "YOU WIN!".

Pantallas: título, tarjeta de etapa, juego, pausa ("PAUSE"; START reanuda), CLEAR! (90 f, con bonus), GAME OVER,
final. HUD (sobre el suelo, y 152, fuente 8×8 de Butano): vidas (iconos, máx. 5) · icono del arma/escudo · "ST 1" · puntos 6 cifras
(· tiempo desde el bloque 2). Color: bolas saturadas cálidas con borde oscuro, un color por tamaño (32 rojo, 24
naranja, 16 magenta, 8 amarillo); objetos en cápsula blanca con letra; enemigos verdes/morados; fondos de ciudad en
tonos fríos apagados para que bolas y peligros destaquen. Greybox primero: círculos y rectángulos con esta paleta.

## 5. Objetos (power-ups)
Al romper una bola (no la de 8) hay probabilidad de soltar un objeto; máximo **1 objeto en pantalla**. Cae a 1 px/f,
espera en el suelo 300 f y parpadea los últimos 90 f. Se coge tocándolo con la caja del jugador + 4 px. Armas
(DOUBLE, STICKY, RAPID) se sustituyen entre sí y se pierden al morir. Los niveles ▽ que presentan uno lo sueltan
**garantizado** en la primera rotura; después sale al azar con el peso indicado.

| Objeto (letra) | Efecto | Duración | Peso al azar | Se ve llevándolo | Presenta | Principio |
|---|---|---|---|---|---|---|
| DOUBLE (D) | 2 arpones a la vez | hasta morir o cambiar | 30 % | HUD "D", arpón azul | nivel 2 | andamiaje: la cascada grande se vuelve abarcable |
| SHIELD (S) | absorbe 1 golpe (+60 f invulnerable) | hasta golpe | 20 % | anillo parpadeante alrededor | nivel 3 | lo opcional regula la dificultad (regla 12) |
| STICKY (H) | el arpón se clava en el techo 90 f y sigue golpeando | hasta morir o cambiar | 15 % | HUD "H", punta de gancho | nivel 7 | riesgo/recompensa: dejar trampas |
| FREEZE (F) | bolas paradas | 180 f; parpadean los 60 últimos | 12 % | bolas tintadas azul, número 3-2-1 | nivel 9 | tensión/alivio |
| RAPID (R) | balas cortas 6 px/f, hasta 3, 12 f entre disparos | hasta morir o cambiar | 12 % | HUD "R", jugador con pistola | nivel 11 | recompensa variable |
| DYNAMITE (X) | todas las bolas pasan al tamaño 8 al instante (sin puntos extra) | instantáneo | 8 % | destello de pantalla 6 f | nivel 13 | riesgo: muchas pequeñas rápidas |
| 1UP (1) | +1 vida (máx. 5) | instantáneo | 3 % (0 si 5 vidas) | icono de vida nuevo | nivel 15 | aversión a la pérdida |

Probabilidad de soltar por rotura: bloque 1 = 1 de 6 (sólo D y S); niveles 6–18 = 1 de 8; nivel 19 = 1 de 4.
Métrica: objetos cogidos / soltados ≥ 60 % (si no, están en sitios peligrosos o caducan pronto).

## 6. Enemigos (no son bolas; molestan, nunca matan)
Ningún enemigo mata: fallar por un enemigo cuesta tiempo, no vidas (regla 10, y el peligro mortal sigue siendo sólo
la bola, lenguaje de color claro). Todos avisan **60 f antes** en el borde por donde entran (regla 5) y no aparecen
en los primeros 5 s tras GO ni durante FREEZE. Máximo 1 enemigo a la vez (2 desde Beijing).

| Enemigo | Zona | Comportamiento | Aviso | Al dispararle | Principio |
|---|---|---|---|---|---|
| PIGEON | London → (y otras) | cruza a y 24–40 a 1.25 px/f; si toca el arpón, lo corta (desaparece) | "!" blanco + sombra en el borde | 300 pts | sincronizar el verbo; amenaza leve |
| SCARAB | Cairo → | anda por el suelo a 0.5 px/f de pared a pared, 2 pasadas; si toca al jugador lo aturde 45 f | nubecilla de arena en el borde | 200 pts (golpe del arpón en su base) | obliga a moverse sin matar |
| MONKEY | New Delhi | baja de arriba por una liana a una x, se lleva el objeto que haya en el suelo y se va | liana que se descuelga 60 f | 500 pts y devuelve el objeto | riesgo/recompensa: correr al objeto |
| KITE | Beijing | flota en seno (amplitud 16 px) cruzando la mitad alta; la bola que lo toca invierte su vx | cuerda visible desde el borde | 400 pts | girar: cambia trayectorias conocidas |

Peligros de escenario (no enemigos): **carámbanos** de Moscow tiemblan 45 f antes de caer (4 px/f) y sí matan; se
rompen con el arpón (100 pts). Es el único peligro mortal no-bola y se ve siempre antes (regla 5).

### Jefe: THE BIG BLIMP (nivel 20, Washington D.C.)
Dirigible de 64×32 que cruza el cielo (y 8–40). Punto débil: la góndola de debajo (16×8, parpadea al recibir).
16 golpes de arpón. Examina lo aprendido (regla 11): bolas, objetos y enemigos.

| Fase | Vida | Comportamiento | Aviso | Qué examina |
|---|---|---|---|---|
| 1 | 16–11 | va de lado a lado a 0.75 px/f; cada 6 s suelta una 24 | la góndola se abre 45 f | cascada y elegir objetivo |
| 2 | 10–5 | 1.1 px/f; cada 5 s suelta dos 16; llama a 1 PIGEON cada 10 s | sirena (parpadeo rojo) 45 f | sincronizar con el pájaro |
| 3 | 4–1 | se para en el centro, baja hasta y 56 y suelta una 32 cada 8 s | se desinfla y tiembla 60 f | manejar lo grande bajo presión |

Muerte del jefe: todas las bolas en pantalla se rompen solas (alivio), 5000 pts, "YOU WIN!". Morir en el jefe
reinicia sólo la fase en curso (regla 10).

## 7. Escenario
| Elemento | Comportamiento | Ciudades | Principio |
|---|---|---|---|
| Ladrillo rompible 16×8 | bolas rebotan; el arpón se para en él y lo rompe (50 pts) | London-2, Cairo-2, Tokyo | el arpón tiene un nuevo uso sin botón nuevo |
| Bloque irrompible 16×8 / columna | bolas rebotan; el arpón se para debajo | Rome, New Delhi-2, Tokyo-2 | girar: obliga a buscar huecos de tiro |
| Carámbano | ver §6 | Moscow | peligro anticipado |
| Escaleras | **no**: piden ↑/↓ y un segundo verbo | — | regla 14 |

Bloques siempre a ≥ 48 px del suelo (el jugador nunca queda encerrado; regla 9) y sobre la rejilla de 8 px.

## 8. Reparto en bloques (alcance pequeño primero)
1. **Bloque 1 (demo jugable)**: niveles 1–5 (Madrid, Paris, London-1), tarjeta de etapa en texto, DOUBLE y SHIELD,
   PIGEON, vidas, puntos, GAME OVER y reintento. Sin escenario, sin tiempo límite, sin sonido obligatorio.
2. **Bloque 2**: sonido (disparo, rotura por tamaño, muerte, CLEAR, música por ciudad), mapa con avión, niveles 6–10
   (ladrillos, columnas, STICKY, FREEZE, SCARAB), tiempo límite, récord en SRAM.
3. **Bloque 3**: niveles 11–16 (carámbanos, RAPID, DYNAMITE, 1UP, MONKEY, KITE).
4. **Bloque 4**: niveles 17–20, jefe, final, kit de itch.io.

## Decisiones de diseño
| Decisión | Principio / regla | Métrica que lo comprobará |
|---|---|---|
| 2 objetos y 1 enemigo en el bloque 1, sin escenario | regla 14 y alcance; el verbo primero (regla 1) | el usuario juega los 5 niveles sin pedir explicación |
| Rebote de altura fija; la dificultad sólo sube vx y nº de bolas | anticipación, flujo | muertes por nivel crecientes en ▲ y bajando en ▽ (bot) |
| Novedad en el nivel ▽ y objeto garantizado al presentarla | reglas 3 y 4 | cada objeto nuevo se coge en ≥ 90 % de las partidas |
| Enemigos que no matan | lenguaje de color: sólo bolas matan; regla 10 | 0 muertes atribuidas a enemigos |
| Reiniciar nivel al morir | reto corto, inicio seguro (reglas 5 y 10) | golpe → control ≤ 3 s medido con `gbarun.py` |
| Retry en el mismo nivel tras GAME OVER | aversión a la pérdida, regla 10 | de GAME OVER a jugar ≤ 4 s pulsando A |
| Mover en READY, no disparar | respuesta inmediata sin romper antes de ver | captura en READY: bola quieta, jugador movido |
| Jugador 1.5 px/f > bola máx. 1.3 px/f | ninguna muerte inevitable | bot sin situaciones sin escape en niveles 1–5 |
| Ruta Madrid → … → Washington, jefe al final | gradiente de meta; capital icónica para el jefe | — |

## Riesgos
- **Números de papel**: medir en la ROM (gravedad, vuelos, ≈30 f del arpón) y corregir esta tabla (regla 13).
- Nivel 5 (tres 16 + paloma) puede ser duro para un descanso: si hay > 2 muertes de media, quitar la 16 de x 56.
- Velocidades fraccionarias (1.5, 0.625 × factor) dan temblor de subpíxel: redondear sólo al dibujar.
- Partición junto a pared, techo o bloque: las hijas deben nacer dentro de la zona libre (recolocar el centro).
- 20 niveles y 10 fondos es mucho arte: greybox con un color de cielo por ciudad hasta que el usuario lo juegue.
