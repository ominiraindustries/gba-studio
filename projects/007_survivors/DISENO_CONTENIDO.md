# 007_survivors (HEXBOUND) — Contenido completo (G-47)

Parte del diseño completo: **núcleo** en `DISENO.md` (§0–§19), **fases, oleadas y jefes** en `DISENO_FASES.md`,
**hoja de ruta, arte y preguntas** en `DISENO_RUTA.md`. Este fichero: bestiario, armas, pasivas, evoluciones, cartas,
meta-progreso, procedural y presupuesto GBA. Cuando un número de aquí choca con §6–§13 de `DISENO.md`, **manda este**
(salvo lo ya medido en §19, que se conserva).

Reglas citadas de `docs/guias/diseno-juegos.md` como **R1..R14**; "Psico" = diseño con psicología; "CL1M" = checklist
del primer minuto. Velocidades en px/frame a 60 fps; tiempos en frames (f) o segundos (s).

---

## C1. Reglas comunes de los monstruos

| Regla | Valor | Principio / métrica |
|---|---|---|
| Daño por contacto | al tocar la caja 8×8 del mago; luego 40 f de invulnerabilidad (§19.1) | R2 |
| Escalado de vida por minuto | `hp = round(hp_base × (1 + 0.07 × minuto))` (min 14 → ×1.98). Jefes exentos | flujo: el poder del jugador crece ~×6 en 15 min; los monstruos ×2 y en número |
| Escalado de daño | **ninguno** por minuto: cada tipo duele siempre lo mismo | Psico lenguaje de amenaza: el jugador aprende "el orco quita 12" |
| Retroceso | 4 px en 6 f; "tanque" ×0.25; jefes 0 | respuesta a cada acción (CL1M) |
| Aparición | fuera de pantalla, anillo 136–168 px (§2). Excepción **RISE** (tumbas): a 64–112 px con grieta 60 f antes, sin daño mientras sale | R5 |
| Gemas | azul/cian 1 XP, magenta 5, oro 25 (art: `gems`). "1 (20 % → 5)" = 20 % de que la gema sea de 5 | gradiente de meta |
| Corazón (cura 30) | 0.4 % de las muertes, 25 % de élites; nunca más de 2 en el suelo | tensión/alivio |
| Moneda (1 oro) | 2 % de las muertes; élite = bolsa de 5 | recompensa variable |
| Topes del director | ≤ 56 vivos (§13); ≤ **10** de 32×32; ≤ **6** tiradores; ≤ **4** tumbas abiertas | §C6 línea de OBJ y CPU |
| Grupos | cada 45 f un grupo de `2 + minuto/3` (máx. 6) hasta el tope del minuto | ritmo |
| Primera vez | cada tipo nuevo entra **solo, 2 ejemplares**, por el lado al que mira el mago, 10 s antes de entrar en el sorteo | R3 presentar en sitio seguro |

### Comportamientos (arquetipos de código; uno por tipo, reutilizados en las 3 fases)

| Id | Qué hace | Anticipación (R5) | Bloque |
|---|---|---|---|
| CHASE | va hacia el mago; los de "flanco" desvían ±30° fijo por individuo | — | 1 |
| HOP | CHASE a saltos (avanza 20 f, para 12 f) | — | 1 |
| WAVE | vuela hacia el mago con onda ±8 px; ignora obstáculos | — | 1 |
| LINE | (oleada) recto a través de la pantalla, desaparece al salir | flechas "!" en el borde **60 f** antes | 1 |
| RING | (oleada) anillo a 150 px que se cierra a 0.4 | flechas "!" en 4 bordes 60 f antes | 1 |
| DASH | anda; a ≤ 96 px se para, **se agacha 30 f** (frame `crouch`, ojos rojos), embiste en línea recta (dirección fijada al empezar el aviso) 20 f; recarga 180 f | 30 f | 2 |
| SPLIT | al morir suelta 2–3 hijos (aparecen con poof, 20 f sin daño) | 20 f | 2 |
| TANK | CHASE con retroceso ×0.25 | — | 2 |
| FLEE | huye del mago; desaparece al acabar su tiempo | flecha dorada en el borde | 2 |
| SHOOT | se queda a 72–96 px; **destello 24 f** y dispara 1 proyectil 8×8 a 1.2 px/f hacia donde estaba el mago; recarga 180 f | 24 f + proyectil lento y brillante | 4 |
| PHASE | WAVE lenta; cada 240 f pasa 60 f **intangible** (50 % alfa): ni recibe ni hace daño | se ve translúcido | 4 |
| BLINK | cada 300 f marca un punto a 56 px del mago (sombra que crece **45 f**) y aparece ahí | 45 f | 4 |
| RISE | sale de una tumba/grieta cerca del mago | grieta **60 f**; inofensivo al salir | 4 |
| BURST | al morir brilla **30 f** y explota r 20 (daño en la tabla) | 30 f (parpadeo rojo) | 6 |

---

## C2. Bestiario

`Arte` = recurso de `art/` (+ paleta). Todas las variantes de paleta comparten tiles: cuesta 1 paleta de 16 colores,
0 tiles (§C6). HP = `hp_base` (antes del escalado por minuto). "Desde" = minuto en que entra solo (presentación).

### Fase 1 — Gloam Meadow

| Monstruo | Arte (paleta) | Tamaño | HP | Vel. | Daño | XP | Comportamiento | Desde | Principio |
|---|---|---|---|---|---|---|---|---|---|
| Slime | `slime` (slime) | 16×16 | 6 | 0.35 | 4 | 1 | HOP | 0:04 | andamiaje: el más lento |
| Skeleton | `skeleton` (no-muertos) | 16×32 | 14 | 0.55 | 6 | 1 (20 %→5) | CHASE, 5 direcciones | 1:10 | desarrollar: "el que duele" |
| Bat | `bat` (no-muertos) | 32×16 | 3 | 1.1 | 4 | 1 | WAVE / LINE en Swarm | 2:00 | girar: más rápido que tú |
| Big Slime | `big_slime` (slime) **nuevo** | 32×32 | 30 | 0.3 | 8 | 5 | HOP + SPLIT (2 slimes) | 3:00 | riesgo/recompensa: gema de 5 |
| Goblin | `goblin` (pieles verdes) | 32×32 | 10 | 0.85 | 6 | 1 | CHASE flanco, llega en parejas | 6:00 | casi tan rápido como tú |
| Wolf | `wolf` (lobo) **nuevo** | 32×16 | 18 | 0.7 / 3.0 | 10 | 5 | DASH | 7:00 | R5 anticipación; enseña Blink |
| Orc | `orc` (pieles verdes) | 32×32 | 60 | 0.45 | 12 | 5 | TANK | 9:00 | tensión: muro que avanza |
| Treasure Goblin | `goblin` (oro) | 32×32 | 40 | 1.3 | 0 | 0 | FLEE 20 s; suelta 30+rnd(31) oro y 1 corazón | evento | riesgo/recompensa |

### Fase 2 — Grim Graveyard (idea nueva: **amenazas a distancia y cosas que salen del suelo**)

| Monstruo | Arte (paleta) | Tamaño | HP | Vel. | Daño | XP | Comportamiento | Desde | Principio |
|---|---|---|---|---|---|---|---|---|---|
| Ghoul | `goblin` (podrido) | 32×32 | 16 | 0.75 | 7 | 1 | CHASE flanco, grupos de 4 | 0:04 | reintroduce el goblin |
| Crow | `bat` (cuervo) | 32×16 | 5 | 1.25 | 5 | 1 | WAVE / LINE ("Murder") | 0:30 | — |
| Skeleton | `skeleton` (no-muertos) | 16×32 | 20 | 0.6 | 7 | 1 (20 %→5) | CHASE | 1:00 | conocido = respiro |
| Grave Walker | `skeleton` (carne gris) | 16×32 | 28 | 0.4 | 9 | 5 | RISE + CHASE | 2:00 | R5: nace cerca pero avisa |
| Ghost | `ghost` **nuevo** | 16×16 | 12 | 0.6 | 6 | 1 | PHASE, atraviesa atrezzo | 3:00 | girar: ignora muros |
| Bone Archer | `skeleton` (hueso rojo) | 16×32 | 16 | 0.5 | 5 / flecha 8 | 5 | SHOOT | 6:00 | romper el "dar vueltas" |
| Wraith | `ghost` (violeta) | 16×16 | 30 | 0.7 | 10 | 5 | PHASE + BLINK | 8:00 | R5 45 f; examina Blink |
| Grave Brute | `orc` (podrido) | 32×32 | 90 | 0.45 | 14 | 5 | TANK | 9:00 | — |
| Bone Knight | `skeleton_big` (no-muertos) | 32×32 | 60 | 0.55 | 12 | 5 | TANK ×0.5, élite natural | 11:00 | — |

### Fase 3 — Ashen Keep (idea nueva: **el fuego castiga matar de cerca**)

| Monstruo | Arte (paleta) | Tamaño | HP | Vel. | Daño | XP | Comportamiento | Desde | Principio |
|---|---|---|---|---|---|---|---|---|---|
| Cinder Slime | `slime` (brasa) | 16×16 | 12 | 0.4 | 6 / explosión 8 | 1 | HOP + BURST | 0:04 | presentar el fuego con el enemigo más lento |
| Ash Bat | `bat` (brasa) | 32×16 | 6 | 1.2 | 6 | 1 | WAVE / LINE | 0:30 | — |
| Imp | `imp` **nuevo** | 16×16 | 14 | 0.9 | 6 / bola 8 | 1 | WAVE + SHOOT (vuela) | 1:30 | — |
| Cultist | `goblin` (túnica ceniza) | 32×32 | 24 | 0.7 | 8 | 1 | CHASE flanco | 2:00 | — |
| Gargoyle | `gargoyle` **nuevo** | 32×32 | 40 | 0.8 / 2.6 | 12 | 5 | DASH volando (ignora atrezzo) | 3:00 | R5; el lobo, girado |
| Ash Knight | `orc` (acero) | 32×32 | 120 | 0.45 | 16 | 5 | TANK | 6:00 | — |
| Ember Warden | `skeleton_big` (brasa) | 32×32 | 80 | 0.6 | 14 / explosión 12 r 24 | 5 | CHASE + BURST | 8:00 | cerrar: fuego + tanque |
| Hellhound | `wolf` (brasa) | 32×16 | 34 | 0.75 / 3.2 | 14 | 5 | DASH | 9:00 | — |

**Paletas nuevas** (todas de `art/palette_c.json`, sólo 4–6 colores cambian; el borde frío y el índice 0 no se
tocan): oro (tesoro), podrido (verde gris + ojos lima), cuervo (negro + ojos rojos), carne gris, hueso rojo, violeta
(wraith), brasa (cuerpo gris oscuro, grietas naranja `#ff7a2a` → aviso de fuego), túnica ceniza, acero (azul hierro
+ ojos cian). Brasa y rojo neón sólo en lo que quema (Psico lenguaje de color: peligro).

### Élites (todas las fases)

| Regla | Valor |
|---|---|
| Probabilidad por aparición | 0 antes de 2:00 · 1/40 de 2:00 a 5:00 · 1/30 de 5:00 a 10:00 · 1/20 de 10:00 a 15:00 |
| Máx. vivas | 3 (F1), 4 (F2), 5 (F3) |
| Vida / XP | ×3 HP (Swift ×2), gema de oro (25) |
| Botín | cofre pequeño (1 mejora) + bolsa de 5 oro; 25 % corazón |
| Aspecto | `fx_aura` + paleta tintada (ya en código) |

| Modificador | Efecto | Fases |
|---|---|---|
| Swift | +60 % velocidad | 1–3 |
| Shielded | escudo visible que absorbe 3 golpes | 1–3 |
| Splitting | al morir, 3 copias al 50 % de tamaño lógico (mismo sprite, HP 30 %) | 1–3 |
| Haunted | al morir suelta 1 Ghost | 2 |
| Blazing | BURST al morir (r 24, 10) | 3 |

---

## C3. Armas (6) — huecos 4

Daño base antes de pasivas y tienda; enfriamiento (enfr.) en frames. "Mirada" = dirección de disparo (§1).
Las tres primeras ya existen (bloque 1): estos números son los del código.

| Arma | Nv1 | Nv2 | Nv3 | Nv4 | Nv5 | Papel |
|---|---|---|---|---|---|---|
| **Spark Bolt** (inicial) — rayo al más cercano, 3 px/f | 5 daño, 1 rayo, enfr. 50 | +1 rayo | +3 daño (8) | +1 rayo (3) | atraviesa 2, enfr. 40 | blanco único fiable |
| **Rune Orbit** — runas a r 28, 1 vuelta/120 f, golpe por enemigo cada 20 f | 2 runas, 4 daño, 180 f on / 120 off | +1 runa | r 36 | +1 runa (4) | siempre activas | defensa de cerca |
| **Frost Ring** — pulso 0→56 px en 24 f, frena 50 % 90 f | 6 daño, enfr. 150 | +3 daño | r 72 | enfr. 110 | congela 45 f (congelado = no hace daño de contacto) | control |
| **Ember Toss** — bola en la mirada, cae a 64 px en 40 f, explota r 24, deja fuego 60 f (2 daño/10 f) | 14 daño, enfr. 120 | +1 bola (±20°) | r 32 | +6 daño (20) | fuego 120 f, enfr. 100 | área diferida |
| **Storm Chain** — rayo al más cercano que salta a ≤ 48 px | 8 daño, 3 saltos, enfr. 100 | +1 salto | +4 daño (12) | +2 saltos (6) | enfr. 70 | grupos dispersos |
| **Moon Lance** — haz en la mirada 120×12, 8 f, atraviesa todo | 10 daño, enfr. 90 | +5 daño | ancho 18 | también detrás | enfr. 60 | línea, jefes |

### Pasivas (6) — huecos 4

| Pasiva | Por nivel (Nv1..Nv5 iguales) | Máx. | Sinergia |
|---|---|---|---|
| **Swift Boots** | +8 % velocidad | +40 % (1.4 px/f) | alcanzar al Treasure Goblin (1.3) |
| **Heart Amulet** | +20 HP máx. y cura 20 | +100 | — |
| **Spell Tome** | −6 % enfriamiento de armas **y de Blink** | −30 % | Blink cada 2.1 s |
| **Lens of Reach** | +10 % área (radios de runa, Frost, Ember, ancho de Lance, salto de Chain) | +50 % | — |
| **Lodestone** | +25 % radio de recogida | +125 % (72 px) | la Nova atrae todas las gemas si Lodestone ≥ 3 |
| **Ward Stone** | −6 % daño recibido; Nv5 además +10 f de invulnerabilidad | −30 % | — |

Fórmulas: `daño = max(1, floor(base × (1 + Might + bendiciones + reliquias)))`; `enfr. = base × (1 − Tome)`;
`daño recibido = max(1, round(contacto × (1 − Ward − Armor)))`.

### Evoluciones (6; arma Nv5 + su pasiva a cualquier nivel → el siguiente cofre de jefe o de élite la transforma)

| Evolución | Receta | Efecto (sustituye al arma) | Bloque |
|---|---|---|---|
| **Starfall** | Spark Bolt + Spell Tome | 4 rayos, 10 daño, atraviesa 3, enfr. 36 **y** una estrella cae cada 30 f sobre un enemigo en pantalla (20 daño, r 12) | 3 |
| **Blizzard** | Frost Ring + Lens of Reach | pulso r 96 cada 90 f, 14 daño, congela 60 f; congelados reciben **+50 %** de cualquier arma | 3 |
| **Ward of Ages** | Rune Orbit + Ward Stone | 6 runas, 8 daño, r 40, siempre; **destruyen proyectiles enemigos**; cada 25 muertes por runa curan 2 HP | 3 |
| **Phoenix Ember** | Ember Toss + Heart Amulet | 3 bolas, r 36, 24 daño, fuego 180 f; **una vez por partida** al morir revive con 30 % HP (estallido como la Nova) | 5 |
| **Tempest** | Storm Chain + Swift Boots | 8 saltos, 14 daño, enfr. 50; moviéndose a velocidad máxima cae un rayo r 16 (10 daño) cada 60 f junto al mago | 5 |
| **Eclipse** | Moon Lance + Lodestone | haz en cruz (4 direcciones), 30 daño, enfr. 60; cada disparo atrae todas las gemas en pantalla | 5 |

Reglas: la pausa muestra cada pareja con un "?" gris hasta cumplirla (R7: cerradura antes que llave). Al evolucionar,
destello de paleta 30 f + rótulo "EVOLVED: STARFALL" 2 s (R6, una línea). Métrica: en partidas de perfil C de
15 min, ≥ 1 evolución a 10:30 en ≥ 60 % (Psico gradiente de meta).

### Cartas

| Regla | Valor | Principio |
|---|---|---|
| Ofertas por subida | 3 (4 con reliquia Lucky Coin) | decisión clara (R14) |
| Pesos | lo que ya tienes sin máx.: 10 · arma nueva (si hay hueco): 7 · pasiva nueva: 6 | coherencia de build |
| Andamiaje | las 2 primeras subidas incluyen ≥ 1 arma nueva | Psico andamiaje |
| Sin duplicados en la mano | sí | — |
| Relleno (todo al máximo o sin opciones) | **Heal 30** · **+10 Gold** (×multiplicador de fase) · **Hex Charge** (Nova llena) · **Old Rune** (+2 % daño el resto de la partida, acumulable) | nunca menú vacío (R9) |
| Reroll | R en el menú de cartas, sólo si se compró (tienda); "R: REROLL ×2" en la carta | opcional (R14) |
| Cofre de jefe | 1 objeto (60 %), 3 (30 %), 5 (10 %); si hay evolución lista, va primero y garantizada | recompensa variable |
| Cofre de élite | 1 objeto; evolución si está lista | — |

### Hex Nova y Blink (sin cambios del núcleo, §1, §19)
Nova: medidor 120 (2/muerte, 1/XP), 40 daño a todo lo que hay en pantalla, a jefes ×3 (120). Blink: recarga 180 f,
10 f invulnerable. Ambas escalan con Might / Tome.

---

## C4. Meta-progreso

### Oro

| Fuente | F1 | F2 | F3 |
|---|---|---|---|
| Por minuto sobrevivido | 5 | 7 | 10 |
| Moneda (2 % de muertes) | 1 | 1 | 2 |
| Élite (bolsa) | 5 | 5 | 8 |
| Treasure Goblin | 30–60 | 45–90 | 60–120 |
| Jefe 5 / 10 / 15 | 20 / 30 / 40 | 30 / 45 / 60 | 40 / 60 / 80 |
| Fase superada | 100 | 150 | 250 |

El oro se conserva al morir (R10). Estimación (para medir): novato muere a 4:00 en F1 ≈ 35 oro; partida media de
10 min ≈ 150; F1 completa ≈ 350.

### Tienda (título → SHOP)

| Mejora | Efecto por rango | Rangos | Precios | Total |
|---|---|---|---|---|
| Might | +5 % daño | 5 | 30 / 60 / 120 / 200 / 300 | 710 |
| Vitality | +10 HP máx. | 5 | 30 / 60 / 120 / 200 / 300 | 710 |
| Haste | +4 % velocidad | 5 | 30 / 60 / 120 / 200 / 300 | 710 |
| Magnet | +15 % recogida | 5 | 30 / 60 / 120 / 200 / 300 | 710 |
| Greed | +10 % oro | 5 | 30 / 60 / 120 / 200 / 300 | 710 |
| Growth | +4 % XP | 5 | 40 / 80 / 160 / 250 / 350 | 880 |
| Armor | −5 % daño recibido | 3 | 100 / 250 / 500 | 850 |
| Recovery | +0.1 HP/s (1 HP cada 10 s por rango) | 3 | 80 / 200 / 400 | 680 |
| Reroll | +1 reroll por partida | 3 | 100 / 250 / 450 | 800 |
| Charged | la Nova empieza al 50 % | 1 | 150 | 150 |
| Revival | revive una vez con 50 % HP | 1 | 600 | 600 |
| **Total** | | | | **8510** |

Se puede devolver todo gratis (botón "REFUND", R10: experimentar no castiga). Con ~150–250 oro por partida, comprar
todo ≈ 40 partidas: para el completista; superar las 3 fases no lo necesita (`DISENO_FASES.md` §E6).

### Desbloqueos (rótulo "NEW: ..." en el resumen; R7: cada hito se ve como objetivo en la tienda con "?")

| Desbloqueo | Condición | Entra en |
|---|---|---|
| Ember Toss | sobrevivir 5:00 | sorteo de cartas |
| Storm Chain | vencer al Slime King | sorteo |
| Lens of Reach | 500 muertes acumuladas | sorteo |
| Ward Stone | vencer al Orc Warchief | sorteo |
| Moon Lance | sobrevivir 10:00 en cualquier fase | sorteo |
| Fase 2 Grim Graveyard | superar F1 | selección de fase |
| Fase 3 Ashen Keep | superar F2 | selección de fase |
| Hexes (maldiciones) | superar F1 | selección de fase |
| Reliquias 4–9 | primera muerte de cada jefe (excepto el Slime King) | pool de reliquias |
| Personaje **Cinda** | superar F1 y pagar 300 | selección |
| Personaje **Morrow** | superar F2 y pagar 600 | selección |

Inicio (partida 1): Spark Bolt, Rune Orbit, Frost Ring, Swift Boots, Heart Amulet, Spell Tome, Lodestone.

### Personajes (máx. 2, baratos: **mismos tiles que Wendel, otra paleta**; 0 animaciones nuevas)

| Personaje | Paleta | Arma inicial | Ventaja | Pega |
|---|---|---|---|---|
| Wendel | lila (actual) | Spark Bolt | — | — |
| Cinda | brasa/rojo, cristal naranja | Ember Toss | +10 % área | −10 HP máx. |
| Morrow | azul noche, cristal cian | Storm Chain | +10 % velocidad, Blink −20 % recarga | −15 % daño |

Principio: rejugabilidad con 0 arte (R14 alcance); métrica: tras desbloquear, ≥ 30 % de partidas con otro personaje.

### Guardado (`bn::sram::write` / `bn::sram::read` de un struct; nunca en mitad de partida)

| Campo | Tipo |
|---|---|
| magic "HXB1", version (u16), checksum (u16 suma de bytes) | cabecera |
| gold (u32), gold_total (u32) | oro |
| shop_ranks[11] (u8) | tienda |
| unlock_bits (u32), relic_bits (u16), boss_bits (u16, 9 jefes), stages_cleared (u8 bits) | desbloqueos |
| records[3]: best_time_s (u16), best_kills (u16), best_level (u8), best_hexes (u8), clears (u16) | récords por fase |
| kills_total (u32), runs (u16), deaths (u16) | totales |
| options: fx_bits (u16), music_vol (u8), sfx_vol (u8), character (u8), last_stage (u8) | opciones |

Se escribe al acabar una partida, al comprar/devolver y al salir de opciones. Cabecera mala o versión desconocida →
partida nueva sin colgarse (R9); versión antigua → campos nuevos a 0. ~120 bytes.

**Récords**: el resumen de muerte/victoria marca "NEW BEST" en tiempo, muertes o nivel. La selección de fase
muestra por fase: mejor tiempo, "CLEAR ×N", y una calavera por cada Hex con la que se ha superado.

---

## C5. Procedural y rejugabilidad

### Trozos de mapa (128×128 px, `hash(semilla, x, y)`, ya en el bloque 1 para F1)

| Plantilla | F1 peso | F2 peso | F3 peso | Sólido |
|---|---|---|---|---|
| vacío (suelo + hierba muerta) | 30 | 20 | 15 | 0 % |
| arboleda (2–3 árboles muertos + matorrales) | 15 | 10 | 0 | 10 % |
| rocas | 15 | 5 | 10 | 10 % |
| setas / cristales | 10 | 5 | 5 | 0 % |
| ruina (arco en ruinas, columna caída) | 10 | 10 | 20 | 15 % |
| charca | 8 | 5 | 0 | 15 % |
| lápidas (3–5) | 2 | 25 | 5 | 10 % (y son tumbas RISE en F2) |
| obelisco rúnico | 5 | 5 | 5 | 5 % |
| muros (2 tramos de ruin_wall en L) | 0 | 5 | 20 | 25 % |
| brasero + estandarte (nuevos F3) | 0 | 0 | 15 | 10 % |
| grieta de lava (nueva F3; decorado, no daña) | 0 | 0 | 5 | 0 % |
| **Hito** (1 de cada 30 trozos, nunca a < 384 px de otro) | Stone Circle | Crypt Gate | Broken Throne | 10 % |

Reglas que ya están en §3 siguen: nada sólido a < 48 px del inicio ni cerrando huecos < 32 px (R9). Los hitos
**garantizan** un evento: Stone Circle = siguiente shrine aparece dentro; Crypt Gate = cofre maldito; Broken Throne
= cofre de élite. Se ven desde lejos (flecha del evento, R5/R12). Suelo y atrezzo de cada fase = mismo `ground` y
`props` con **paleta distinta** + 3 piezas nuevas en F3 (`DISENO_RUTA.md`).

### Eventos

| Evento | Cuándo | Probabilidad | Qué | Principio |
|---|---|---|---|---|
| Shrine | cada 90 s (1:30, 3:00, …); se retrasa si hay jefe vivo | 100 % | bendición 1 de 2 durante 60 s (Fury / Haste / Magnet / Mend) | recompensa con pista |
| Cofre maldito | 1 por acto, minuto al azar entre 1:00 y 3:30 del acto | 60 % | 2 mejoras seguidas; luego "CURSED!" y 20 s de élites (1 cada 2 s, máx. 5) | riesgo/recompensa |
| Treasure Goblin | 1 por acto, entre 0:30 y 3:30 del acto | 50 % | FLEE 20 s; flecha dorada | riesgo/recompensa, persecución |
| Primera partida | el Treasure Goblin del acto 1 sale seguro a 2:30 en la partida 1 | — | enseña el evento | R6 |

Un acto = 0:00–5:00, 5:00–10:00, 10:00–15:00. No hay eventos en los 30 s anteriores a un jefe (R4 silencios).

### Hexes (maldiciones opcionales; selección de fase, tras superar F1)

| Hex | Efecto | Oro extra |
|---|---|---|
| Swarm | tope de cada minuto +8 (sin pasar de 56), grupos ×1.25 | +30 % |
| Frenzy | monstruos +20 % velocidad | +30 % |
| Frail | −30 % HP máx. | +25 % |
| Dark Moon | fundido nocturno 0.5 desde el inicio y monstruos +25 % HP | +30 % |
| Famine | sin corazones; los shrines no ofrecen Mend | +20 % |

Se pueden combinar (máx. +135 %). Principio: R12 lo opcional regula la dificultad; métrica: ≥ 1 Hex en el 30 % de
partidas tras superar F2.

### Reliquias (cofre del jefe del minuto 10: elige 1 de 2 del pool desbloqueado; dura la partida; icono en pausa)

| Reliquia | Regla | Desbloqueo |
|---|---|---|
| Ember Heart | las gemas explotan al recogerlas (r 16, 4 daño) | inicial |
| Blink Feather | Blink recarga 90 f y deja escarcha r 24 (6 daño, frena) | inicial |
| Gold Tooth | +50 % oro, −20 HP máx. | inicial |
| Frost Sigil | cada pulso de Frost Ring que toca ≥ 5 enemigos cura 1 | Orc Warchief |
| Bone Charm | con HP < 30 %, +40 % daño | Thornwood Treant |
| Hex Battery | Nova se carga ×1.5; Blink recarga +60 f | Bone Colossus |
| Lucky Coin | 4 cartas por subida; los shrines dan 3 opciones | Banshee |
| Glass Hourglass | monstruos −10 % velocidad; recibes +10 % daño | The Lich |
| Moon Shard | la Nova también congela 90 f | Gargoyle Lord / Iron Champion / Ember Dragon (la primera de las 3) |

### Semilla
Semilla por partida (aleatoria; se ve en la pausa, "SEED 4F2A"). Semilla diaria sólo con RTC (`bn::date_time`); sin
RTC no hay modo diario (no se promete lo que no hay; §17).

---

## C6. Presupuesto GBA por fase

Base (§13): 128 sprites en OAM, ~954 ciclos de OBJ por línea, **1024 tiles 4 bpp** de sprites, CPU medida 77 % en
partida normal y 97.7 % en el peor caso de prueba (README, G-45). **No se sube el tope de 56 enemigos**: la
dificultad tardía sale de vida, mezcla y élites, no de número.

### VRAM de sprites (el fallo visto: goblin + orco 32×32 en 5 direcciones)

Hoy el código cachea **todos** los frames de andar de cada tipo (`_move_tiles[t]`: direcciones × frames). Coste:

| Tamaño | tiles/frame | 1 dirección × 4 f | 5 direcciones × 4 f |
|---|---|---|---|
| 16×16 | 4 | 16 | 80 |
| 16×32 / 32×16 | 8 | 32 | 160 |
| 32×32 | 16 | 64 | **320** |
| 64×64 (jefe) | 64 | — | sólo el frame actual |

Goblin 320 + orco 320 + esqueleto 160 + resto ≈ 850 tiles sólo en enemigos → se agota (1024 con HUD, cartas
3×96, Nova 64, ráfaga de nivel 64, texto, mago).

**Decisión (técnica con efecto visible, la tomo yo):**
1. **Caché por arte, no por tipo**: las variantes de paleta (ghoul = goblin, crow = bat…) comparten tiles. Coste 0.
2. **Fotograma global compartido** (bloque 2, tarea de motor): todos los ejemplares de un arte animan en sincronía;
   en vez de cachear todos los frames, se guarda **1 `sprite_tiles_ptr` por dirección dibujada** y se recarga su
   contenido cada paso de animación (API a comprobar con `kb.py symbol bn::sprite_tiles_ptr`; p. ej.
   `set_tiles_ref`). Coste de un 32×32 en 5 direcciones: **80 tiles** (en vez de 320). Así goblin y orco **sí**
   usan sus 5 direcciones.
3. **Si (2) no sale** (medido): los 32×32 de horda sólo de lado (`walk_e` + volteo) = 64 tiles. Las 5 direcciones
   quedan para el mago y el esqueleto (16×32).
4. Muertes: frames propios por sprite que muere; **máx. 8 animaciones de muerte a la vez** (el resto, `fx_poof`).
5. Límite medido: `bn::sprite_tiles::used_tiles_count()` → global `test_tiles_used` (pico). **Pico ≤ 900** en la
   escena peor de cada fase (jefe + menú de cartas abierto + 56 enemigos).

| Fase | Artes de horda (tiles con 2 / con 3) | Total con 2 | Total con 3 | 32×32 distintos |
|---|---|---|---|---|
| F1 | slime 4/16 · skeleton 40/160 · bat 8/32 · big_slime 16/64 · goblin 80/64 · wolf 8/40 · orc 80/64 | **236** | 440 | 3 |
| F2 | goblin 80/64 · bat 8/32 · skeleton 40/160 · ghost 4/16 · orc 80/64 · skeleton_big 16/96 | **228** | 432 | 3 |
| F3 | slime 4/16 · bat 8/32 · imp 4/16 · goblin 80/64 · gargoyle 16/64 (sólo de lado) · orc 80/64 · skeleton_big 16/96 · wolf 8/40 | **216** | 392 | 4 |

Regla de contenido (para no repetir el fallo): **por fase ≤ 8 artes de horda, ≤ 4 de 32×32, caché de horda ≤ 448
tiles**; jefe 64×64 único y sin afín; resto (mago, FX, HUD, cartas, texto) ≤ 450.

### Sprites en pantalla (máx. 125 de 128)

| Grupo | F1 | F2 | F3 |
|---|---|---|---|
| Mago + barra + sombra | 3 | 3 | 3 |
| Enemigos (jefe cuenta como 1) | 56 | 56 | 56 |
| Proyectiles del jugador | 24 | 24 | 24 |
| Proyectiles enemigos | 4 (jefe) | 12 | 12 |
| Gemas visibles | 20 | 16 | 16 |
| Recogibles / eventos | 4 | 4 | 4 |
| Partículas | 6 | 6 | 6 |
| HUD texto | 4 | 4 | 4 |
| **Total** | 121 | 125 | 125 |

De 32×32 vivos: **≤ 10** (línea de OBJ: 10×32 + 46×16 ≈ 1056 ciclos si todos coinciden en una línea → el director
reparte la aparición por el anillo; medir parpadeo en la escena peor). Jefes sin afín (64 px de ancho = 64 ciclos).

### Paletas de sprites (16)
Mago, UI, recogibles, FX, blanco de destello, jefe = 6 fijas; quedan **10 para monstruos**: F1 usa 5 (slime,
no-muertos, pieles verdes, lobo, oro), F2 7, F3 7 (+2 de élite tintadas que ya genera el código por tipo: cuenta
en el pico; si se pasa, élite = `set_fade` del sprite en vez de paleta propia).

### Fondos
BG0 suelo (512×512, paleta por fase), BG1 atrezzo dinámico (paleta por fase), BG2 HUD, BG3 libre (F2: niebla de
tiles translúcida opcional detrás de interruptor). Título y final: 8 bpp, ocupan toda la paleta de fondos.

### CPU
Objetivo: partida normal ≤ 85 % del frame en el pico de cada fase; peor caso de prueba ≤ 100 %. Nuevos costes a
medir en su bloque: tiradores (≤ 6 × 1 proyectil), proyectiles enemigos (12 × caja del mago, barato), RISE
(≤ 4), BURST (radio a ≤ 20 enemigos por explosión, ≤ 2 por frame — el resto se encola al frame siguiente).
