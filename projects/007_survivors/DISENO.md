# 007_survivors — Diseño (G-43)

Nombre de trabajo: **HEXBOUND** (ver pregunta abierta 1). Textos de pantalla y nombres en inglés; este documento en
castellano. Juego propio del género "horde survivor": nada copiado (nombres, armas, arte y números son nuestros).

Reglas citadas de `docs/guias/diseno-juegos.md` como **R1..R14**; "Psico" = sección "Diseño con psicología";
"CL1M" = checklist del primer minuto. Skill `gba-design` para pantalla y mando.

## 0. Frase y verbo

- **Frase**: un mago de túnica lila sobrevive 15 minutos a hordas de monstruos de fantasía que cambian con el reloj;
  sus hechizos disparan solos y cada nivel le deja elegir 1 de 3 mejoras.
- **Verbo central**: **esquivar moviéndose** (posicionarse). Disparar es automático; la habilidad está en dónde te
  pones y en qué mejoras eliges. (R1: si moverse entre 40 slimes en una sala vacía no gusta, nada lo arregla; R14.)
- **Bucle**: moverse → los hechizos matan → caen gemas → recogerlas (riesgo: están entre los monstruos) → subir de
  nivel → elegir mejora → más poder frente a más monstruos (flujo, riesgo/recompensa).

## 1. Controles (R14: una mano basta)

| Botón | En partida | En menús |
|---|---|---|
| Cruceta | mover en 8 direcciones | elegir |
| A | **Hex Nova**: limpia la pantalla cuando el medidor (abajo izq.) está lleno; se carga al matar (2) y con gemas (1 por XP) | confirmar |
| B | **Blink**: teletransporte corto en la dirección de la cruceta, 10 f invulnerable, recarga 3 s | volver |
| R (mantener) | (bloque 2) fija la mirada para armas direccionales | — |
| L | — | — |
| START | pausa: muestra armas, pasivas y niveles; "Resume / Quit" | — |
| SELECT | sólo build de pruebas: menú de efectos (§12) | — |

**Decisión del usuario (2026-11-24, G-45)**, sustituye a la del 2026-11-23 ("sólo cruceta"):
- **Mirada = dirección de disparo**, no de la marcha: el mago se gira hacia donde apunta su hechizo. Se lee qué
  dispara y hacia dónde (respuesta a cada acción, CL1M).
- **A = Hex Nova** y **B = Blink**, ya en el bloque 1. Ambos son **opcionales**: sólo con la cruceta se puede jugar
  y ganar (R14: la complejidad, opcional). Se enseñan **en su momento** con una línea (R6): "B: BLINK" al recibir el
  primer golpe, "A: NOVA READY!" la primera vez que el medidor se llena.
- La A deja de ser "botón que no sirve" (el antipatrón que motivó la decisión anterior) porque el medidor visible
  avisa de cuándo sirve.

## 2. Números iniciales (60 fps)

| Concepto | Valor | Principio / métrica |
|---|---|---|
| Velocidad mago | **1.0 px/f** (cruza la pantalla en 4 s) | flujo; métrica: los enemigos de 0.35–0.55 se dejan atrás andando, los de ≥1.0 no |
| Aceleración / frenada | 0 → 1.0 en 4 f, frena en 3 f | R1 tacto inmediato |
| Diagonal | normalizada (×0.707) | — |
| Vida | **100 HP**, barra bajo el mago (24×2 px) | Psico: aversión a la pérdida visible |
| Invulnerabilidad tras golpe | **30 f** (parpadeo) | R2 |
| Caja de daño del mago | **8×8** centrada (sprite 16×24) | R2: generosa para el jugador |
| Caja de daño de enemigos (para tus hechizos) | sprite completo (14×14 en 16×16) | R2 |
| Radio de recogida | **24 px** (las gemas vuelan a 3 px/f al entrar) | gradiente de meta |
| Regeneración | 0 | — |
| Retroceso a enemigos al recibir daño | 4 px en 6 f (se desactiva con interruptor) | respuesta a cada acción (CL1M) |

Aparición de enemigos: **siempre fuera de pantalla**, en un anillo a 136–168 px del mago, con preferencia por la
dirección en la que se mueve (R5: nunca nacen encima). Los enemigos que quedan a > 260 px se recolocan delante (así el
mapa infinito no deja huecos vacíos y el contador no se llena de rezagados).

## 3. Partida, fases (stages) y mapa

### Duración
- **Fase completa: 15:00.** Jefe a 5:00, jefe a 10:00, jefe final a 15:00; matarlo = **STAGE CLEAR**.
- 15 min caben en una sesión de GBA y dan tres "actos" de 5 min (R3: presentar → desarrollar → girar → cerrar).
- Morir: pantalla de resumen (tiempo, nivel, muertes, oro) y **A = Retry** en < 3 s (R10, CL1M < 5 s).

### Fases (biomas) — respuesta a "¿las pantallas cambian cada partida?"
Modelo: **cada fase tiene un tema fijo** (fondo, paleta, plantel de monstruos y jefes propios) y se **desbloquea al
superar la anterior**. Dentro de una fase el campo es abierto e infinito con un fondo que se repite, pero el
**atrezzo y los obstáculos (árboles, rocas, ruinas, agua, cofres) se colocan proceduralmente por semilla** en cada
partida (§4). Así cada partida se juega distinta sin pagar arte nuevo.

| Fase | Tema | Monstruos | Jefes 5 / 10 / 15 min |
|---|---|---|---|
| 1 **Mossy Meadow** (demo) | prado y bosque claro | slime, bat, skeleton, goblin, wolf, orc | Slime King / Orc Warchief / Thornwood Treant |
| 2 Grim Graveyard | cementerio de noche | skeleton, zombie, ghost, bat, crow, wraith | Bone Colossus / Banshee / The Lich |
| 3 Ashen Castle | castillo en ruinas | imp, dark knight, gargoyle, fire slime, cultist | Gargoyle Lord / Iron Champion / Ember Dragon |

Pregunta abierta 2 (modelo de fases). Recomendado: este.

### Mapa infinito (presupuesto en §10)
- **Suelo**: un `bn::regular_bg_ptr` de 512×512 px (64×64 tiles), que el hardware repite solo; la cámara
  (`bn::camera_ptr`, `set_camera`) se mueve sin límite. Tile de suelo con 3–4 variantes para que la repetición no se
  note a 240×160.
- **Atrezzo/obstáculos**: segunda capa `bn::regular_bg_ptr` de 512×512 con mapa dinámico (patrón del ejemplo
  `engine/butano/examples/dynamic_regular_bg`): el mundo se divide en trozos de 128×128 px; al entrar un trozo en la
  zona de 512 px que rodea a la cámara se escribe su contenido en las celdas que reutiliza. El contenido sale de
  `hash(semilla, x_trozo, y_trozo)` → mismo sitio, mismo atrezzo al volver.
- Obstáculos **sólidos** (rocas, troncos, ruinas): bloquean al mago y a enemigos de suelo con una consulta de 1 celda
  por actor y frame; los murciélagos y fantasmas los atraviesan. Nunca se genera un obstáculo a < 48 px del punto de
  partida ni cerrando un hueco < 32 px (R9: no encerrar al jugador).
- Paleta del suelo apagada y poco saturada; sprites de enemigos con contorno oscuro; gemas y hechizos brillantes
  (Psico: lenguaje de color — fondo / peligro / recogible).

## 4. Procedural y rejugabilidad (ordenado por valor/coste)

| # | Elemento | Qué hace | Coste | Bloque |
|---|---|---|---|---|
| 1 | **Mapa por trozos aleatorios** | semilla por partida → trozos de 128 px con atrezzo, obstáculos, ruinas, charcas (agua = sólido) | bajo: 1 capa dinámica + 8–10 plantillas de trozo | **1** |
| 2a | **Shrines** (santuarios) | aparece uno cada ~90 s a 160–220 px con flecha en el borde; tocarlo 1 s → elige 1 de 2 bendiciones temporales (60 s): +30 % daño, +30 % velocidad, imán total, curación 40 | bajo: 1 sprite + menú de cartas ya existente | **1** |
| 2b | **Élites con modificador** | desde 2:00, 1 de cada ~40 apariciones es élite: tamaño normal, contorno de color y aura; modificador al azar: **Swift** (+60 % vel.), **Splitting** (al morir, 3 copias pequeñas), **Shielded** (escudo que absorbe 3 golpes, se ve). Suelta cofre pequeño (1 mejora gratis) | bajo-medio | **1** (Swift y Shielded; Splitting en 2) |
| 2c | Cofre maldito | cofre negro: mejora doble, pero llama una oleada élite durante 20 s | bajo | 2 |
| 2d | Treasure goblin | goblin que huye a 1.3 px/f con saco; si muere suelta mucho oro; desaparece a los 20 s | bajo | 2 |
| 3 | Modificadores de partida (**Curses**) | antes de empezar o en un shrine oscuro: "+25 % monstruos, +25 % XP y oro", "enemigos +20 % velocidad, oro ×2"… | bajo (multiplicadores) | 2 |
| 4 | Reliquias | objetos raros de jefe que cambian una regla ("las gemas explotan al recogerlas", "Frost Ring también cura 1") | medio | 3 |
| 5 | Semilla del día | se muestra en el título ("SEED 4F2A"); la diaria necesita reloj (`bn::date_time`, sólo con RTC en cartucho/emulador); sin RTC, semilla introducible | bajo | 3 |

Principio: recompensa variable (shrines, élites) con riesgo/recompensa (hay que ir a por ellos lejos del sitio seguro).
Métrica: % de shrines tocados por partida (objetivo ≥ 60 %) y nº de élites muertas.

## 5. Primer minuto / primeros 2 minutos (R5, R6, CL1M)

| Tiempo | Qué pasa | Cómo se enseña |
|---|---|---|
| 0:00 | Mago en el centro de un claro sin obstáculos ni monstruos. Rótulo de una línea **"D-PAD: MOVE"** 3 s | R6 una línea; CL1M sitio seguro |
| 0:01 | Spark Bolt ya equipado; dispara aunque no haya blanco (chispa al aire) → se ve que es automático | CL1M: empiezo con lo necesario |
| 0:04 | **Un** slime entra por el lado hacia donde se mira, lento (0.35). El rayo lo mata en 2 golpes con destello blanco y "poof" | R3 presentar solo; R5 se ve antes de dañar |
| 0:05 | Cae una gema azul brillante. Rótulo **"GRAB GEMS"** junto a la primera gema, una vez | R6 en el momento |
| 0:10–0:20 | grupos de 2–3 slimes; barra de XP arriba se llena | gradiente de meta |
| ~0:15 | **Primera subida de nivel**: pausa, destello, 3 cartas. Rótulo **"PICK ONE"**. Cruceta + A | R6; Psico andamiaje |
| 0:30 | grupos de 4–6 slimes por dos lados; segundo nivel ~0:35 | desarrollar |
| 0:50 | primer **descanso**: 6 s sin apariciones nuevas | R4 silencios |
| 1:00 | entran **skeletons** (más vida, algo más rápidos): primero 2 solos | R3 nueva idea en sitio seguro |
| 1:30 | primer **shrine** con flecha parpadeante en el borde de pantalla | recompensa con pista (checklist de sala) |
| 2:00 | **Bat Swarm**: 1 s antes, flechas "!" rojas en el borde por donde llegan; 12 murciélagos cruzan en línea | R5 anticipación; R3 girar |

Respuesta a cada acción (CL1M): impacto = destello + 1 sonido; muerte = poof; gema = "tink" con tono que sube si
recoges seguidas; daño recibido = parpadeo rojo + temblor 2 px; subida de nivel = sonido + destello de paleta.

## 6. Línea temporal de oleadas — fase 1 (por TIEMPO, no por nivel)

"Máx." = enemigos vivos objetivo; el director aparece de a grupos hasta llegar a él (tope duro en §10).

| Min | Monstruos que entran (mezcla) | Máx. | Evento a minuto fijo |
|---|---|---|---|
| 0 | slime | 6 → 14 | — |
| 1 | slime, skeleton | 20 | 1:30 shrine |
| 2 | skeleton, bat | 26 | **2:00 Bat Swarm** (aviso 1 s); élites desde aquí |
| 3 | skeleton, bat, slime grande | 32 | 3:00 shrine |
| 4 | skeleton, bat | 38 | **4:00 Bone Ring**: anillo de 16 esqueletos que se cierra a 0.4 px/f (aviso: el anillo aparece a 150 px y tarda 6 s) |
| 5 | (pausa 10 s antes) | 12 | **5:00 JEFE Slime King** |
| 6 | goblin, bat | 36 | descanso 8 s tras el jefe; cofre del jefe |
| 7 | goblin, wolf | 40 | 7:30 shrine |
| 8 | wolf, skeleton | 44 | **8:00 Wolf Pack**: 6 lobos desde un lado, aullido 1.5 s antes |
| 9 | orc, goblin | 46 | 9:00 Goblin Raid (24 goblins en abanico) |
| 10 | (pausa 10 s) | 16 | **10:00 JEFE Orc Warchief** |
| 11 | orc, wolf, bat | 48 | descanso; cofre del jefe |
| 12 | todos | 52 | 12:00 Bone Ring doble |
| 13 | orc, wolf, élites ×2 | 54 | 13:00 shrine |
| 14 | todos, élites ×3 | 56 | 14:00 Stampede (lobos + orcos cruzando) |
| 15 | (pausa 10 s) | 16 | **15:00 JEFE Thornwood Treant** → STAGE CLEAR |

Dientes de sierra (checklist de sala: "sube con descansos"): pausa antes de cada jefe y descanso después.
Métrica: muertes por minuto en 10 partidas grabadas; objetivo bloque 1: un jugador nuevo llega a 3:00 la 1.ª vez y
vence al Slime King en ≤ 3 intentos (flujo).

## 7. Monstruos

Todos: daño por contacto cuando su caja toca la del mago (con los 30 f de invulnerabilidad). Velocidades en px/f.

| Monstruo | Sprite | HP | Vel. | Daño | XP (gema) | Comportamiento / anticipación | Principio | Bloque |
|---|---|---|---|---|---|---|---|---|
| **Slime** | 16×16 | 6 | 0.35 (a saltitos) | 5 | 1 | persigue; el más lento: enseña a moverse | andamiaje | **1** |
| **Bat** | 16×16 | 3 | 1.1 (onda senoidal ±8 px) | 4 | 1 | persigue torpe; ignora obstáculos; en Swarm cruza en línea recta | girar: más rápido que tú | **1** |
| **Skeleton** | 16×16 | 14 | 0.55 | 8 | 1 (20 % → 5) | persigue; al morir se desmonta (huesos) | desarrollar | **1** |
| Big Slime | 32×32 | 30 | 0.3 | 10 | 5 | al morir → 2 slimes | — | 1 si sobra; si no 2 |
| Goblin | 16×16 | 10 | 0.85 | 6 | 1 | llega en parejas; rodea un poco (±30°) | — | 2 |
| Wolf | 16×16 (32×16 ok) | 18 | 0.7, embestida 3.0 durante 20 f | 12 | 5 | **aviso 30 f**: se agacha y ojos rojos, luego embiste en línea recta | R5 | 2 |
| Orc | 32×32 | 60 | 0.45 | 15 | 5 | no recibe retroceso | tensión | 2 |
| Zombie, Ghost, Crow, Imp, Dark Knight… | — | — | — | — | — | fases 2–3 | — | 3+ |

Gemas: **azul 1 XP**, **verde 5**, **roja 25**. Oro: 2 % de las muertes, 100 % de jefes/élites.

## 8. Jefes (R11: examinan lo aprendido, fases que cambian el comportamiento)

Al aparecer: 10 s sin apariciones antes, rótulo "SLIME KING" + mosaico de entrada (§12), barra de vida arriba.
Durante el jefe siguen apareciendo normales al 30 % del máximo (las gemas siguen dando nivel).

| Jefe | Sprite | HP | Fase 1 | Fase 2 | Fase 3 | Examina |
|---|---|---|---|---|---|---|
| **Slime King** (5:00) | 64×64 | 600 | salta hacia ti: **sombra en el suelo 45 f antes** de caer, onda de 32 px al caer (12) | < 50 %: al caer suelta 4 slimes | < 25 %: saltos cada 60 f, sombra 30 f | moverse a tiempo, leer anticipación |
| Orc Warchief (10:00) | 32×32 (escala) | 1800 | camina 0.5 + **carga**: polvo y pie golpeando 40 f, luego 3.5 px/f en línea | cuerno: llama 8 goblins | carga doble | esquivar en línea, control de grupo |
| Thornwood Treant (15:00) | 64×64 | 4000 | lento, raíces que salen del suelo con **grieta 40 f antes** | zona de espinas en el borde (se encoge el campo 10 % — visible) | llama wolves | posicionarse en espacio reducido |

Cofre de jefe: 1–3 mejoras seguidas (recompensa variable) y, si se cumple, la evolución (§9).

## 9. Armas, pasivas, evolución

Huecos: **4 armas + 4 pasivas** (HUD pequeño, decisiones claras; R14). Cada una **5 niveles**. Enfriamientos en
frames, daño base antes de pasivas.

### Armas (6)
| Arma | Qué hace | Nv1 | Mejoras Nv2–5 | Bloque |
|---|---|---|---|---|
| **Spark Bolt** (inicial) | rayo al enemigo más cercano, 3 px/f | 5 daño, 1 rayo, enfr. 50 | +1 rayo · +3 daño · +1 rayo · atraviesa 2 y enfr. 40 | **1** |
| **Rune Orbit** | runas giran a 28 px (1 vuelta/120 f); golpean cada enemigo cada 20 f | 2 runas, 4 daño, 180 f activas / 120 f pausa | +1 runa · radio 36 · +1 runa · siempre activas | **1** |
| **Frost Ring** | pulso circular que crece de 0 a 56 px en 24 f; frena 50 % durante 90 f | 6 daño, enfr. 150 | +3 daño · radio 72 · enfr. 110 · congela 45 f | **1** |
| Ember Toss | bola de fuego en la mirada; cae a 64 px tras 40 f, explota r 24, deja fuego 60 f | 14 daño, enfr. 120 | +1 bola · r 32 · +6 daño · fuego 120 f | 2 |
| Storm Chain | rayo al más cercano que salta 3 veces a ≤ 48 px | 8 daño, enfr. 100 | +1 salto · +4 daño · +2 saltos · enfr. 70 | 2 |
| Moon Lance | haz en la mirada, 120×12 px, 8 f, atraviesa todo | 10 daño, enfr. 90 | +5 daño · ancho 18 · haz también detrás · enfr. 60 | 2 |

### Pasivas
| Pasiva | Por nivel | Bloque |
|---|---|---|
| **Swift Boots** | +8 % velocidad | **1** |
| **Heart Amulet** | +20 HP máx. y cura 20 | **1** |
| Spell Tome | −8 % enfriamiento | 2 |
| Lens of Reach | +10 % área | 2 |
| Lodestone | +25 % radio de recogida | 2 |
| Lucky Clover | +oro, +5 % crítico | 3 |

Cuando no queda nada que subir, las cartas ofrecen **"Heal 30"** y **"+10 Gold"** (nunca menú vacío; R9).

### Evolución (opcional, bloque 2)
Arma a Nv5 + su pasiva (cualquier nivel) → el siguiente cofre de jefe la transforma:
**Spark Bolt + Spell Tome → Starfall** (lluvia de estrellas sobre 6 enemigos cada 30 f);
Frost Ring + Lens of Reach → Blizzard; Rune Orbit + Heart Amulet → Ward of Ages.
La pantalla de pausa muestra la pareja con un icono gris "?" (cerradura antes que llave, R7).

## 10. XP y curva de nivel

- XP para pasar del nivel L al L+1: **need(L) = 5 + 3·(L−1)** hasta L20; luego **+6 por nivel**.
- Previsto: Nv2 ≈ 0:15, Nv5 ≈ 1:45, Nv10 ≈ 4:00, Nv12 ≈ 5:00, ≈ Nv28 a 15:00.
- Subida de nivel: pausa total, 3 cartas (icono 16×16 + nombre + "Lv 2" o "NEW"). Al menos 1 carta es de algo que ya
  tienes si hay huecos llenos. Varias subidas seguidas se encadenan.
- Métrica: tiempo real de cada nivel en partidas grabadas vs esta tabla (±30 %); si Nv2 llega después de 0:25 se baja
  need(1) (flujo).

## 11. Meta-progreso (simple)

- **Oro** que se guarda siempre (también al morir: aversión a la pérdida suavizada, R10). Más oro por minuto
  sobrevivido (5/min) y jefes (20).
- **Tienda** en el título, 5 mejoras permanentes × 5 rangos (coste 30/60/120/200/300): Might +5 % daño, Vitality
  +10 HP, Haste +4 % velocidad, Magnet +15 % recogida, Greed +10 % oro.
- **Desbloqueos por hito** (dan objetivos): sobrevivir 5:00 → Ember Toss entra al sorteo; vencer Slime King → Storm
  Chain; 1000 muertes acumuladas → Moon Lance; vencer fase 1 → fase 2.
- Guardado: `bn::sram::write` / `read` de un struct con versión.
- Bloque 1: **sólo oro acumulado visible** en resumen y título (sin tienda). Tienda y desbloqueos: bloque 2.

## 12. Efectos (vistoso, detrás de interruptores; el usuario elige jugando)

Menú SELECT en build de pruebas; cada uno ON/OFF, valores por defecto marcados. APIs comprobadas con `kb.py symbol`.

| Efecto | Cómo (Butano) | Defecto |
|---|---|---|
| Destello blanco al golpear (4 f) | paleta "blanca" del enemigo / `bn::sprite_palette_ptr::set_fade` | ON |
| Retroceso del enemigo | 4 px en 6 f | ON |
| Números de daño | `bn::sprite_text_generator`, máx. 6 a la vez (gastan sprites) | OFF |
| Temblor de pantalla | `bn::camera_ptr` ±2 px (golpe recibido, jefe cae) | ON |
| Poof de muerte | 4 partículas 8×8, 12 f | ON |
| Estela de gemas al imán | 1 chispa por gema que vuela | ON |
| Destello de subida de nivel | `bn::bg_palettes::set_fade` a blanco 0.5 → 0 en 20 f | ON |
| Atardecer según el tiempo | `bn::bg_palettes::set_fade` hacia azul oscuro 0 → 0.35 en 15 min (el reloj se ve, R9) | ON |
| Frost Ring translúcido | `bn::sprite_ptr::set_blending_enabled` + `bn::blending::set_transparency_alpha` | ON |
| Entrada de jefe con mosaico | `bn::sprites_mosaic::set_stretch` 1 → 0 en 30 f | ON |
| Sombras bajo los actores | sprite 16×8 translúcido (cuesta 1 sprite por actor: sólo mago y jefe) | ON |

## 13. Presupuesto GBA (Butano, sin memoria dinámica en el bucle)

Límite duro: **128 sprites** (`BN_CFG_SPRITES_MAX_ITEMS`) y ~954 ciclos de OBJ por línea (16×16 normal ≈ 16 ciclos →
~59 en la misma línea). Enemigos **sin afín** (el afín cuesta el doble +10).

| Grupo | Máx. en pantalla | Tamaño | Nota |
|---|---|---|---|
| Mago + barra de vida + sombra | 3 | 16×32, 32×8, 16×8 | — |
| Enemigos | **56** (`bn::pool` / `bn::vector<enemy, 56>`) | 16×16; 32×32 cuenta como 1 | jefe 64×64 = 1 sprite |
| Proyectiles del jugador | **24** (runas incluidas) | 8×8 / 16×16 | Frost Ring = 8 sprites en anillo |
| Proyectiles enemigos | 8 | 8×8 | bloque 2 |
| Gemas visibles | **20** | 8×8 | lógicas hasta 64; pasado el tope, la nueva suma su valor a la gema más lejana (pasa a verde/roja) |
| Recogibles (cofre, shrine, oro) | 4 | 16×16 | — |
| Partículas | 6 | 8×8 | se descartan si no hay hueco |
| HUD (reloj "04:59", "LV 12", oro) | 4 | texto Butano | sprite_text_generator con fuente de `engine/butano/common/` |
| **Total** | **125** (3+56+24+8+20+4+6+4) | | menú de cartas: oculta partículas y gemas para liberar sprites |

- Fondos: BG0 suelo 512×512 (repite), BG1 atrezzo dinámico 512×512, BG2 HUD (barra de XP y marcos), BG3 libre.
- Paletas de sprites (16 colores c/u): mago, hechizos, gemas+oro, 1 por cada 2 monstruos de la fase, jefe, blanco
  de destello → ~8 de 16.
- Tiles de sprite: una animación compartida por tipo de enemigo (todos los slimes usan los mismos tiles, se cambia
  el frame por tipo, no por individuo) para no agotar VRAM.
- CPU: 24 proyectiles × 56 enemigos = 1344 pruebas de caja por frame en el peor caso; si el perfilador
  (`gba-performance`) da > 30 % del frame, rejilla de 32 px. **Medir en el bloque 1 con 56 enemigos** antes de
  añadir contenido (R13).
- Enemigos se actualizan IA cada frame (persecución = vector normalizado aproximado); separación entre enemigos
  barata (empujón con un vecino por frame, rotando) para que no se apilen en un único sprite.

## 14. Bloque 1 — demo jugable (5 minutos con jefe)

**Entra** (y nada más):
- 1 fase: **Mossy Meadow**, suelo que se repite + atrezzo procedural por semilla (8 plantillas de trozo).
- Mago con arte real: andar 4 direcciones (espejo para izq./der.), idle, golpe, muerte.
- 3 monstruos: **slime, bat, skeleton** (+ Big Slime si sobra tiempo).
- 3 armas: **Spark Bolt, Rune Orbit, Frost Ring**; 2 pasivas: **Swift Boots, Heart Amulet**.
- Gemas, imán, barra de XP, subida de nivel con 3 cartas.
- Línea temporal 0:00–5:00 de §6 (Bat Swarm 2:00, Bone Ring 4:00) y **Slime King** a 5:00 → "STAGE CLEAR".
- Shrines (cada 90 s) y élites Swift / Shielded.
- Título (Start), pausa, resumen de muerte con Retry en A, oro acumulado (sin tienda).
- Efectos de §12 con su menú SELECT; sonidos básicos (golpe, gema, nivel, daño, jefe).

**No entra**: tienda, desbloqueos, armas 4–6, evolución, minutos 5–15, fases 2–3, cofres malditos, curses,
reliquias, semilla diaria. (Blink y Hex Nova sí entran: decisión del usuario, §1.)

**Aceptación del bloque 1**: 56 enemigos + 24 proyectiles sin parpadeo ni caída de fps (medido); un jugador nuevo
entiende qué hacer en 10 s; muere o gana a los 5 min; Retry < 3 s; revisión de primer minuto (`gba-designer`).

## 15. Lista de arte (bloque 1 en negrita)

Formato: `art/*.aseprite` indexado con paleta del juego, una etiqueta por animación (skill `gba-gfx`).

- **Mago** 16×32 (figura ~16×24, gorro puntiagudo y túnica lila): idle 2 f, walk down/up/side 4 f, hurt 1 f, death 4 f.
- **Slime** 16×16: salto 4 f, muerte 3 f. **Bat** 16×16: aleteo 3 f. **Skeleton** 16×16: walk 4 f, desmontarse 4 f.
- **Slime King** 64×64: idle 2 f, agacharse 1 f, aire 1 f, aplastar 2 f; **sombra** 32×16.
- **Hechizos**: chispa 8×8 (2 f), runa 8×8 (2 f), segmento de hielo 16×16 (3 f), impacto 8×8 (3 f), poof 8×8 (4 f).
- **Gemas** 8×8 ×3 colores (brillo 2 f), **oro** 8×8, **cofre** 16×16, **shrine** 16×32, aura de élite 16×16.
- **Suelo** prado: 4 tiles variantes + flores/hierba; **atrezzo**: árbol 32×48, roca 16×16 y 32×32, tocón, ruina de
  piedra 32×32, charca 32×32 (bordes), arbusto.
- **HUD**: marco de barra de XP, iconos 16×16 de las 5 mejoras del bloque, carta de mejora (marco), flecha de borde,
  "!" de aviso.
- Título: logotipo + mago grande (puede ser bg 240×160).
- Después: goblin, wolf, orc, Big Slime, Orc Warchief, Treant; iconos de armas 4–6 y pasivas; fases 2–3.

## 16. Decisiones de diseño

| Decisión | Principio / regla | Métrica que lo comprobará |
|---|---|---|
| Armas automáticas; cruceta basta para jugar | R14, R1 | 10 s sin dudas en la revisión del primer minuto |
| A = Hex Nova (medidor 2/muerte, 1/XP), B = Blink (3 s) — usuario | R14 opcional; R6 rótulo en su momento; Psico tensión/alivio | Nova llena a ~0:50 (±10 s); Blink se usa ≥1 vez en 2 min en la prueba del usuario |
| Mirada = dirección de disparo — usuario | respuesta a cada acción (CL1M) | el usuario dice qué dispara sin preguntar |
| Monstruos por tiempo; nivel del jugador sólo da mejoras | decisión del usuario; R3 por actos de 5 min | muertes por minuto en dientes de sierra |
| Velocidad 1.0, slime 0.35, skeleton 0.55, bat 1.1 | flujo: unos se esquivan andando, otros obligan a esquivar de verdad | ¿cuántos golpes de bat vs slime recibe un novato? (bats ≥ 2× slimes) |
| Aparición fuera de pantalla, avisos "!" 1 s antes de oleadas | R5 | 0 golpes recibidos en los 30 f tras una aparición |
| Caja de daño del mago 8×8, invulnerable 30 f | R2 | sensación justa en prueba del usuario |
| 1.ª subida de nivel ~0:15 | gradiente de meta, andamiaje | tiempo medido de Nv2 ≤ 0:25 |
| Pausa 10 s antes y descanso tras cada jefe | R4 | — |
| Fases temáticas fijas + atrezzo procedural por semilla | rejugabilidad a bajo coste de arte (R14 alcance) | 2 partidas seguidas con capturas distintas |
| Bloque 1 = 5 min, 3+3+2 | alcance pequeño (CLAUDE.md "Alcance") | ROM jugable antes de crecer |
| Tope 56 enemigos / 24 proyectiles / 20 gemas | 128 sprites, línea de OBJ | perfil con escena de peor caso |
| Retry con A en < 3 s, oro se conserva | R10 | tiempo de muerte → control |

## 17. Riesgos

- **Parpadeo por línea** si 40+ enemigos se alinean en horizontal: separación entre enemigos y anillo de aparición
  distribuido; medir con escena de peor caso.
- **CPU de colisiones** con 56×24: rejilla si hace falta (medir primero, R13).
- **VRAM de tiles**: animaciones compartidas por tipo; el jefe 64×64 ocupa 64 tiles por frame — sólo 1 jefe a la vez.
- **Legibilidad a 240×160**: con 56 enemigos el mago debe distinguirse siempre (lila saturado + sombra + barra);
  revisar en captura a 1x.
- **Alcance**: la tentación es añadir armas. Regla: no se añade nada fuera del bloque hasta que el usuario haya
  jugado el bloque 1 (tareas P4).
- Semilla diaria depende de RTC (muchos cartuchos/emuladores no lo traen): por eso va al final.

## 18. Preguntas abiertas para el usuario (máx. 4)

1. **Título**: **HEXBOUND** (recomendado), "Purple Hat Panic" o "Moonwick". ¿Nombre del mago? Recomendado: *Wendel*.
2. **Modelo de pantallas**: fases temáticas fijas (Meadow → Graveyard → Castle), cada una con su plantel y jefes, que
   se desbloquean al superar la anterior, con atrezzo/obstáculos procedurales por partida. **Recomendado** (§3–4).
   Alternativa: una sola fase que cambia de bioma con el tiempo dentro de la misma partida (más arte por partida).
3. ~~Botón B = Blink~~ **Resuelta** por el usuario el 2026-11-24: Blink en B y Hex Nova en A, ya en el bloque 1 (§1).
4. **Duración de la fase completa**: **15 min** (recomendado; 3 actos de 5) o 10 min (jefe final a 10).

## 19. Revisión primer minuto (bloque 1) — 2026-11-24, ROM de 95adf60

Método (R13): ROM de 95adf60 sin ventana (`gbarun.py`), 3 perfiles de entrada reproducibles con A cada 2.5 s (cartas
y Nova) y B cada 7 s: **A "corro en círculo"** (cuadrado de 90 f por lado, novato que no se aleja), **B "paseante"**
(lados de 240 f, se aleja de sus gemas). Valores leídos de `test_*` cada 20–80 f; capturas revisadas a 1x.

| Punto (CL1M / R) | Medido | Veredicto | Arreglo concreto |
|---|---|---|---|
| Qué controlo y qué hago en 10 s (CL1M) | mago centrado, "D-PAD: MOVE" 3 s, chispa al aire a 0:01, primer slime 0:04, "GRAB GEMS" 0:06 | **OK** | — |
| Empiezo con lo necesario, sitio seguro (CL1M, R5) | Spark Bolt equipado, 100 HP, 0 enemigos hasta 0:04, claro sin atrezzo | **OK** | — |
| Cruceta / A / B enseñados en su momento (R6) | "B: BLINK" al 1.er golpe (0:22); "A: NOVA READY!" al llenarse (1:00) | **OK** con fallo: el rótulo de Nova **sigue 3 s después de lanzarla** (medidor ya a 14) | ocultar el rótulo al lanzar la Nova |
| Elegir carta (R6) | "PICK ONE: D-PAD + A" la 1.ª vez; selección = esquinas azules, poco visible a 1x; mundo sin oscurecer, slimes encima del marco de la carta; texto cortado: "Slow ring" → "low rin" | **CAMBIAR** | carta elegida sube 4 px + parpadeo de marco 8 f, las otras con paleta al 60 %; ocultar sprites del mundo o fundido 50 % detrás de las cartas; textos de carta **≤ 7 caracteres por línea** ("Slows", "+8% SPD") |
| 1.ª subida (métrica ≤ 0:25) | A: **0:22**; B: **0:34** (8 muertes a 0:22 pero 5 gemas abandonadas) | **CAMBIAR** | `need(1)` **5 → 3** (`xp_needed`: caso especial nivel 1; resto igual) y `pickup_radius` **24 → 32 px**. Previsto A ≈ 0:14, B ≈ 0:20 |
| Ritmo de la Nova (diseño ~0:50) | A: llena a **1:00**; B: **1:14** | **CAMBIAR** | `nova_max` **150 → 120** → A ≈ 0:48, B ≈ 0:59 |
| Nada mata sin aviso (R5, flujo) | A muere a **1:13** (Nv5): a 1:00 entran los 2 esqueletos "solos" **y a la vez** el sorteo pasa a 35 % esqueletos; a 1:08 hay 6+; 70 HP perdidos en 7 s (8 por toque, 30 f invulnerable = 16 HP/s rodeado). B sigue vivo a 1:32 con 56 HP | **CAMBIAR** (objetivo §6: un novato llega a 3:00) | esqueletos del sorteo desde **1:10** (no 1:00), proporción **15 % a 1:10 → 35 % a 1:40** lineal; daño del esqueleto **8 → 6** |
| Al cerrar las cartas | A recibe 3 golpes en 1 s (95 → 85) justo al volver al juego | **CAMBIAR** (R2, R5) | al cerrar la carta: **45 f invulnerable** y empujar a 32 px a los enemigos que estén a < 32 px (reusar el destello de subida) |
| Legibilidad del mago (R5, §17) | parpadeo de daño = **invisible 50 % de los frames** (`(_invulnerable & 2)`): rodeado de esqueletos el mago desaparece justo cuando más importa (capturas 0:23 y 1:13). Fuera de eso el z-order es correcto (mago 5 < enemigos 10) | **CAMBIAR** | sustituir la invisibilidad por destello de paleta (blanco 2 f / normal 2 f) o 50 % de transparencia; el mago **nunca** deja de verse. Barra de vida ya sigue visible |
| Cada acción tiene respuesta (CL1M) | impacto, poof, gema, destello de nivel y tinte de Nova (13 f) visibles; Nova aún sin anillo (`fx_nova` provisional) | **OK** (provisional anotado) | — |
| Reintentar < 5 s (R10) | muerte → resumen ~1 s → Retry; total < 3 s | **OK** con riesgo: el resumen **no bloquea la entrada**: una A pulsada para la Nova reintenta sin ver el resumen, y una **B de Blink vuelve al título** (pasó en la prueba) | bloquear A/B **60 f** en el resumen y mostrar "A: RETRY  B: TITLE" al acabar el bloqueo; ocultar el sprite del mago, que tapa "Kills 79" |
| Textos (inglés, `sprite_text_generator`) | correctos; "LEVEL UP!", "YOU FELL", "PRESS START" legibles | **OK** salvo cartas (arriba) | — |

**Veredicto: CAMBIAR antes de dar la ROM.** El primer minuto se entiende y es justo hasta ~0:55; los fallos son de
números y de claridad, no del núcleo: 8 arreglos pequeños (todos datos o detalle de presentación), ningún cambio de
vista ni de mecánica.

Prioridad (implementador; prueba = los dos perfiles de arriba, métricas entre paréntesis):
1. Mago siempre visible al recibir daño (0 capturas sin mago en 1:00–1:20 del perfil A).
2. Esqueletos: sorteo desde 1:10 con rampa 15 → 35 %, daño 6 (perfil A vivo a 1:30 con ≥ 30 HP).
3. Invulnerable 45 f + empuje 32 px al cerrar cartas (0 golpes en los 45 f tras cerrar).
4. `need(1)` = 3 y `pickup_radius` = 32 (Nv2 ≤ 0:25 en ambos perfiles).
5. Resumen de muerte: bloqueo 60 f de A/B, mago oculto (la A/B pulsadas no lo saltan).
6. `nova_max` = 120 (llena a 0:50 ± 10 s en perfil A).
7. Cartas: selección clara, mundo apagado detrás, textos ≤ 7 caracteres.
8. Ocultar "A: NOVA READY!" al lanzarla.

Riesgos que quedan: contraste del mago (túnica lila oscura sobre hierba oscura; los esqueletos blancos destacan más
que él) → pregunta de dirección artística al usuario (tarea `design-question`); la Nova lanzada en pantalla casi vacía
se desperdicia (aceptado: decisión del jugador, el medidor lo enseña); el perfil A es un novato pasivo: si con los
arreglos sigue muriendo antes de 2:00, bajar el tope de §6 del minuto 1 de 20 a 16.
