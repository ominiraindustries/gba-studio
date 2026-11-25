# 007_survivors (HEXBOUND) — Hoja de ruta, arte y preguntas (G-47)

Parte del diseño completo (ver `DISENO.md` §20). Qué se construye, en qué orden, cómo se acepta cada bloque y qué arte
pide. Contenido en `DISENO_CONTENIDO.md` (§C), fases y jefes en `DISENO_FASES.md` (§E).

Principio rector: **cada bloque termina en una ROM jugable que el usuario puede probar** (CLAUDE.md "Alcance"; R10,
R14). Ningún bloque empieza sin que el usuario haya jugado el anterior. Antes de cada entrega, revisión del primer
minuto de lo nuevo (`gba-designer`).

## R0. Pantallas del juego completo

```
TITLE (PRESS START) → MENU: PLAY · SHOP · RECORDS · OPTIONS
PLAY → STAGE SELECT (3 cartas de fase; bloqueadas con candado y "Clear Gloam Meadow")
     → [L/R: personaje] [SELECT: HEXES]  → A: partida
Partida → (subida de nivel: cartas) (START: pausa: armas, pasivas, reliquia, evoluciones "?", SEED, Resume/Quit)
       → muerte: YOU FELL + resumen → A: RETRY · B: menú
       → STAGE CLEAR + resumen (+ final la 1.ª vez en F3) → A: menú
SHOP: lista de 11 mejoras (icono, rango ●●○○○, precio); A compra, SELECT: REFUND (confirmación)
RECORDS: por fase: mejor tiempo, muertes, nivel, CLEAR ×N, calaveras de Hex; totales
OPTIONS: música, efectos de sonido, interruptores de efectos (§12), borrar guardado (doble confirmación)
```

Todo texto con `bn::sprite_text_generator` y la fuente de `engine/butano/common/` (§C, CLAUDE.md); textos ≤ 7
caracteres por línea en cartas (§19). Menús: cruceta + A/B, sin combinaciones (R14).

---

## R1. Bloques

| Bloque | Nombre | Se juega | Tamaño aprox. |
|---|---|---|---|
| 1 | Demo F1 0–5 min | **hecho** (§14, §19) | — |
| 2 | F1 acto 2 (5–10) | 10 min con Warchief | medio |
| 3 | F1 completa (10–15) | 15 min, Treant, STAGE CLEAR | medio |
| 4 | Meta | tienda, guardado, desbloqueos, 3 evoluciones | medio |
| 5 | F2 Grim Graveyard | segunda fase entera | grande (reutiliza mucho) |
| 6 | Rejugabilidad | Hexes, reliquias, hitos, 2 personajes, 3 evoluciones | medio |
| 7 | F3 Ashen Keep + final | tercera fase y final | grande |
| 8 | Equilibrio y publicación | pasada de números, itch.io | pequeño |

Corte de seguridad (si el usuario se cansa): **tras el bloque 4 hay un juego completo y publicable** de una fase con
meta-progreso. Las fases 2 y 3 son ampliaciones.

### Bloque 2 — F1 acto 2 (5:00–10:00)

**Entra**: fotograma global compartido para tiles de horda (§C6, tarea de motor, primero y medido) · escalado de HP
por minuto (§C1) · Big Slime (si no entró), Goblin, Wolf, Orc · arquetipos DASH, SPLIT, TANK, FLEE · formaciones
STAMPEDE y SWARM · minutos 5–10 de §E2 · **Orc Warchief** completo · élite Splitting · Treasure Goblin · cofre de jefe
1/3/5 · armas **Ember Toss** y **Storm Chain**, pasivas **Spell Tome** y **Lodestone** (en el sorteo sin desbloqueo
todavía) · menú DBG "Start at mm:ss" · música de F1 y de jefe.
**No entra**: minutos 10–15, tienda, desbloqueos, evoluciones, Moon Lance, Lens, Ward Stone.

| Prueba | Cómo | Esperado |
|---|---|---|
| VRAM | DBG Stress 56 con slime/skeleton/goblin/wolf/orc mezclados + abrir cartas | `test_tiles_used` ≤ 900; goblin y orco miran en 5 direcciones (captura) |
| CPU | perfil C 4:30–10:10 (Start at 4:30) | `test_cpu_max` ≤ 85 %; peor caso Stress ≤ 100 % |
| Wolf avisa | captura en el frame del agacharse | frame `crouch` + ojos rojos 30 f antes de cada embestida; 0 golpes de lobo sin aviso en 3 partidas |
| Warchief | Start at 9:45, perfil C | rótulo, 3 fases vistas (cuerno con 8 goblins, carga doble), aturdido al chocar; TTK 30–60 s |
| Treasure Goblin | semilla con evento (DBG fuerza) | flecha dorada, huye a 1.3, desaparece a 20 s, suelta oro al morir |
| Splitting | DBG élite | 3 copias con poof, 20 f sin daño |
| Tiempos | perfil C semillas 1–5, Novice | llega a 7:00 en ≥ 3/5; Nv20 ±30 % a 10:00 si sobrevive |
| Primer minuto de lo nuevo | `gba-designer` a 5:00–7:30 | cada tipo nuevo se presenta solo |

**Arte (Aseprite, `art/`, paleta maestra, índice 0 transparente):**

| Recurso | Tamaño | Frames / etiquetas | Notas |
|---|---|---|---|
| `big_slime` | 32×32 | hop 0–3, death 4–7 | sembrado ampliando `slime` 2× (vecino más cercano) |
| `wolf` | 32×16 | run 0–3, crouch 4, dash 5, death 6–8 | mira a la derecha; ojos rojos en crouch |
| `orc_warchief` (retoque) | 64×64 | +stomp 15–16, +horn 17–18 al final | arreglar "se sale del lienzo al atacar" (LÉEME) |
| `fx_ember` | 8×8 | fly 0–1 | bola de Ember Toss |
| `fx_explosion` | 32×32 | burst 0–4 | también BURST en F3 |
| `fx_fire` | 16×16 | burn 0–3 | charco de fuego |
| `fx_chain` | 8×8 | zap 0–2 | segmento de Storm Chain (fila de sprites) |
| `fx_dust` | 8×8 | puff 0–2 | pisotón/carga |
| `icons` (+) | 16×16 | +ember_toss 8, +storm_chain 9 | al final del sheet |
| `hud_bossbar` | 32×8 | fill 0–16, notch 17 | sustituye `tmp_bossbar` |
| Audio | — | `music_meadow.xm`, `music_boss.xm`; sfx howl, horn, stomp, explosion, zap, chest, coin_burst | — |

### Revisión bloque 2 (gba-designer, ROM de d4f24d6, 2026-11-24)

Cómo: `gbarun.py` sin ventana, perfil C (`tests/b2run.py`), DBG Start at 4:30 / 5:50 / 7:00 / 8:00 / 9:00 / 9:45
con y sin God, Force Treasure y Split elite, Stress 56; capturas cada 1–5 s y ráfagas cada 4 f (agacharse del lobo)
y cada 45 f (Warchief), revisadas a 1x y 2x.

| Punto | Veredicto | Medida / qué se ve |
|---|---|---|
| Presentación de goblin (6:00) | OK | pareja sola tras el descanso del Slime King; pero el Treasure Goblin salió a las 6:02 (ver 4) |
| Presentación de wolf (7:00) | CAMBIAR | aullido OK, pero de 7:01 a 7:10 la mezcla sustituye al lobo por esqueleto (≈ 50 % esqueletos): a 7:08 hay ~30 esqueletos y la pareja de lobos no se distingue (R3) |
| Presentación de orc (9:00) | CAMBIAR | no se identifica en ninguna captura de 9:00–9:14: el orco es gris oscuro entre lobos gris oscuro (R5, lenguaje de color) |
| Aviso del lobo | CAMBIAR | 30 f de agacharse + ojos rojos existen, pero en la ráfaga del frame 858 el lobo agachado (gris casi negro sobre fondo azul noche) no se lee a 1x; ojos de 1 px. 8:00–9:00: 73 golpes de embestida/min (God, perfil C) × 10 de daño |
| Wolf Pack 8:00 / Goblin Raid 9:15 | OK con 2 y 3 | llegan, se leen como masa; la Raid se lee mejor que el Pack. Raid = peor frame 107,7 % CPU |
| Orc Warchief | CAMBIAR | rótulo y barra OK; TTK 52 s (God) / 47 s (sin God, sobrevive): **justo**, dentro de 30–60 (R11). Pero el jefe sólo está entero en pantalla en 3 de 40 capturas (7,5 %) y **ninguna** de 40 capturas a 45 f muestra la línea de 4 "!" (con una carga de aviso 40 f cada 4 s deberían salir ~6): carga desde fuera de pantalla (R5) |
| Treasure Goblin | CAMBIAR (menor) | rótulo, huida y desaparición OK; la flecha de borde se ve blanca/gris, no dorada; su ventana del acto 2 cayó en 6:02, encima de la presentación del goblin |
| Élite Splitting | OK | copias rosas visibles a 5:51–5:52 |
| Cartas nuevas | OK | Ember Toss / Storm Chain / Spell Tome en 2 líneas, Lodestone en 1 cabe en el pergamino; descripciones ≤ 7 ("+1 jump" = 7) |
| Efectos sobre las cartas | CAMBIAR | con cartas abiertas siguen visibles encima: estallido de Frost Ring, chispas de Storm Chain, fuego de Ember (Stress, 9:16, 10:24) |
| ≤ 4 muertes/poof/gema nuevas por frame | OK | ningún hueco visible en capturas de 56 enemigos |
| Storm Chain 1 salto/frame | OK | 5 saltos = 83 ms: se lee como rayo que viaja |
| Animación sincronizada por arte | OK | filas de esqueletos marchan al paso: se lee como "ejército", no molesta; no vale VRAM separarlas (pico ya > 900) |
| Técnica | CAMBIAR (G-52) | `test_tiles_used` 941 (4:30–7:00), 924 (Warchief + cartas), 905 (7:00) > 900; CPU 107,7 % en 9:00–10:00 |
| Slime King (Start at 4:30) | CAMBIAR | TTK 47 s, objetivo 20–40 |

**Arreglos priorizados** (para el implementador; cada uno con su métrica):

1. **Warchief a la vista** (R5, R11). Correa: si distancia > 100 px, velocidad ×3 hasta 64 px (antes 160/120). La
   carga sólo empieza con el centro del jefe dentro de ±96 × ±56 px de la cámara; si no, camina 1 s más y reintenta.
   Los 4 "!" a 12 px de separación, recortados dentro de pantalla, paleta roja/amarilla que parpadea cada 4 f.
   Métrica: jefe entero en ≥ 30 % de capturas cada 45 f (hoy 7,5 %); `test_charge_offscreen` = 0; ≥ 1 captura con
   los "!" por cada 3 cargas. TTK debe seguir en 30–60 s (hoy 47–52).
2. **Aviso del lobo legible** (R5, anticipación). Durante los 30 f de agacharse, alternar paleta roja/normal cada 4 f
   (como el destello de daño del mago, una paleta compartida); máx. 3 lobos agachados a la vez; tras embestir, 150 f
   sin volver a agacharse; daño de embestida 10 → 8. Métrica: captura del frame de agacharse con silueta roja a 1x;
   `test_wolf_dash_hits` ≤ 25 en 8:00–9:00 perfil C God (hoy 73).
3. **Contraste de goblin, wolf y orc** (lenguaje de color: enemigo = valor medio-alto sobre fondo oscuro). Subir el
   valor del cuerpo para que la luminancia media del sprite sea ≥ 2× la del suelo y añadir 1 px de borde claro como el
   esqueleto. Tono de cada uno: pregunta de arte al usuario (G-53, `design-question`); si no contesta, por defecto goblin verde, wolf
   gris pardo claro, orco verde oliva. Métrica: los 3 tipos se distinguen en una captura 1x de 9:30 (revisión).
4. **Presentaciones limpias** (R3, una idea por tramo). (a) Durante 10 s tras cada presentación (6:00, 7:00, 9:00),
   objetivo de enemigos vivos ×0,6. (b) El tipo retenido se sustituye por otro de la mezcla del minuto según su peso,
   no siempre por esqueleto. (c) Treasure Goblin nunca a ±20 s de 6:00, 7:00, 8:00, 9:00, 9:15: ventana del acto 2 =
   6:25–6:50 o 7:20–7:50. (d) Flecha del Treasure Goblin en dorado (paleta de la moneda). Métrica: en 7:00–7:10 los 2
   lobos visibles en ≥ 3 capturas de 5 con ≤ 15 enemigos más en pantalla.
5. **Ocultar efectos de combate con cartas y pausa abiertas** (escarcha, cadena, ascuas, fuego, explosión). Métrica:
   0 sprites de efecto en las capturas de cartas de Stress y de 9:15.
6. **Técnica (ya en G-52)**: tiles ≤ 900 en Start at 4:30 y Warchief + cartas (hoy 941/924); CPU ≤ 100 % en la Raid
   (hoy 107,7 %) antes de bajar el perfil C a 85 %.
7. **Slime King HP 600 → 450** (R13: DPS medido 600/47 = 12,8 → TTK ≈ 35 s). Métrica: TTK 20–40 en Start at 4:30.
8. **DBG Start at marca los tutoriales como vistos** ("D-PAD: MOVE", "GRAB GEMS", "B: BLINK" reaparecen a 5:50–9:45).
   Sólo depuración, pero ensucia las capturas de revisión.

Dato suelto: perfil C **sin God** desde Start at 5:50 muere a las **6:52** (1 semilla; minuto 6 con goblins y big
slimes), es decir, la prueba Tiempos (llegar a 7:00 en ≥ 3/5) está en riesgo: medirla en G-52 **antes** de tocar
números y, si falla, palanca 1 (tope del minuto 6: 36 → 32). Pendiente también: muertes por lobo sin God desde 7:00.

### Bloque 3 — F1 completa (10:00–15:00)

**Entra**: minutos 10–15 de §E2 · formaciones Double Bone Ring y ELITE PAIR · **Thornwood Treant** · ira de jefe
· cofre maldito · STAGE CLEAR (§E5.4 pasos 1–3) · resumen completo con oro desglosado · jingle de victoria.
**No entra**: tienda, final, fases 2–3.

| Prueba | Cómo | Esperado |
|---|---|---|
| 15 min sin fallos | perfil C con DBG God, semilla 1 | llega a 15:00, vence al Treant, STAGE CLEAR, resumen; 0 cuelgues |
| Treant | Start at 14:45 | raíces con grieta 40 f; muro de zarzas visible entero y se cierra a r 110; semillas; TTK 60–120 s |
| Ira | Start at 14:45 sin atacar (God) | a los 3:00 rótulo "THE HEX MOON RISES" y +50 % velocidad |
| Cofre maldito | DBG fuerza | 2 mejoras, "CURSED!", 20 s de élites (máx. 5 vivas) |
| Pico técnico | minuto 14 con Stampede | `test_cpu_max` ≤ 85 %; sin parpadeo en capturas |
| Equilibrio F1 | C semillas 1–5, Novice | llega a 10:00 en ≥ 3/5 y no vence en ≥ 4/5 (§E6 #3) |

| Recurso | Tamaño | Frames / etiquetas | Notas |
|---|---|---|---|
| `treant` | 64×64 | idle 0–3, walk 4–7, slam 8–10, hurt 11, death 12–15 | jefe nuevo |
| `fx_root` | 16×16 | crack 0–1, spike 2–4 | — |
| `fx_thorn` | 16×16 | idle 0–1 | segmento del muro de zarzas |
| `fx_seed` | 8×8 | fly 0–1 | — |
| `chest` (+) | 16×16 | +cursed_closed 2, +cursed_open 3 | negro con runa roja |
| Audio | — | `jingle_clear`, sfx root_crack, thorn, curse_sting | — |

### Bloque 4 — Meta (juego completo de una fase)

**Entra**: guardado `bn::sram` (§C4) · menú del título (R0) · SHOP con las 11 mejoras y REFUND · RECORDS · OPTIONS ·
desbloqueos de armas/pasivas (§C4) · **Moon Lance**, **Lens of Reach**, **Ward Stone** · evoluciones **Starfall**,
**Blizzard**, **Ward of Ages** · Reroll en cartas · selección de fase (F2/F3 bloqueadas) · DBG "Meta preset".
**No entra**: Hexes, reliquias, personajes.

| Prueba | Cómo | Esperado |
|---|---|---|
| Guardado | partida → morir → reiniciar el emulador (`.sav`) | oro, rangos y récords se conservan |
| Corrupción | `.sav` con bytes al azar | arranca con guardado nuevo, sin colgarse |
| Versión | `.sav` de versión 1 con versión 2 en la ROM | campos nuevos a 0, el resto intacto |
| Tienda | comprar Might 1–5, REFUND | precios 30/60/120/200/300; el oro vuelve entero |
| Desbloqueo | sobrevivir 5:00 | resumen "NEW: EMBER TOSS"; aparece en cartas la partida siguiente; antes no |
| Evolución | DBG Max power + Spell Tome, matar élite | "EVOLVED: STARFALL", el icono cambia en pausa |
| Equilibrio | C Medium (500) semillas 1–5 | vence F1 ≥ 3/5 (§E6 #3) |
| Economía | 10 partidas C Novice→ | oro por partida en 100–250 a partir de la 3.ª |

| Recurso | Tamaño | Frames / etiquetas | Notas |
|---|---|---|---|
| `shop_icons` | 16×16 | might 0, vitality 1, haste 2, magnet 3, greed 4, growth 5, armor 6, recovery 7, reroll 8, charged 9, revival 10 | paleta UI |
| `stage_cards` | 64×64 | meadow 0, graveyard 1, keep 2, locked 3 | miniatura de cada fase |
| `fx_lance` | 32×16 | beam 0–2 | haz por tramos |
| `fx_star` | 16×16 | fall 0–3 | Starfall |
| `fx_rune` (+) | 8×8 | +gold 2–3 | Ward of Ages |
| `icons` (+) | 16×16 | +moon_lance 10, +lens 11, +ward_stone 12, +starfall 13, +blizzard 14, +ward_of_ages 15, +locked 16 | — |
| Audio | — | `music_title.xm`; sfx buy, refund, menu_move, evolve | — |

### Revisión bloque 4 (gba-designer, ROM de 46fd80e, 2026-11-25)

Cómo: copias de HEAD en el scratchpad (`git archive`), `gbarun.py` sin ventana, semillas 1–5, guardado nuevo (Novice,
sin Ember/Chain). `dead_at` = reloj del juego (las cartas lo paran). Capturas de menús `meta1/2/3`, `reroll`, `die`.

#### G-55.1 Tiempos (perfil C Novice no llega a 7:00): CAMBIAR el bot y dos números

| Variante (perfil C, 5 semillas) | Muere a (s) | Media | Lectura |
|---|---|---|---|
| base (HEAD) | 141, 101, 136, 139, 160 | 135 | reproduce G-55 |
| Ember + Chain desbloqueados desde el inicio | 114, 92, 119, 123, 120 | 114 | **no es la build**: con más armas muere antes |
| tope min. 2–4: 26/32/38 → 20/26/34 | 141, 101, 140, 132, 150 | 133 | **no es la densidad**: semillas 1–2 idénticas |
| `player_hp` 100 → 130 | 146, 117, 152, 154, 175 | 149 | +30 HP = +14 s: pierde ~2 HP/s |
| bot con Blink cada 3 s (a ciegas) | 115, 115, 100, 129, 109 | 114 | el Blink a ciegas mete al mago en el grupo |
| bot "K90" (8 dir. de 90 f, círculo r≈118, B cada 7 s) | 145, 183, 155 (3 sem.) | 161 | moverse más ayuda poco |
| **perfil A** (cuadrado 90 f, B cada 7 s), semilla 1 | vivo a 2:02 con **62 HP** | — | el novato pasivo sigue vivo a 2:00 (§19.1 pide ≥ 20) y no es trivial (−38 HP) |

Capturas (`shotC2`, 0:36–1:29): el mago pierde 90 HP con **3–6 enemigos en pantalla**, pegado a arcos y pilares del
mapa (§C5), dando vueltas de r≈50 px sobre el mismo grupo de slimes y esqueletos. Un jugador que ve 5 enemigos los
esquiva; el bot no mira. **Diagnóstico: el bot C no representa al jugador medio** (R13: la entrada reproducible debe
medir lo que dice medir). Es un bucle ciego que choca con muros, deriva y vuelve al grupo. Ningún número razonable lo
arregla: a ~2 HP/s harían falta ~600 HP más para llegar a 7:00, y eso volvería trivial el juego para un humano (flujo).

**Decisión: (c) + un ajuste pequeño.**

1. **Bot C reactivo en la ROM** (sólo con `debug_menu`): fila "DBG Bot C". Cada frame: vector de huida = suma de
   (mago − enemigo)/d² de los enemigos a ≤ 96 px + repulsión de muros y zarzas a ≤ 24 px + tirón suave (peso 1/8)
   hacia una órbita de r 80 alrededor del punto de inicio; se mueve en la más cercana de 8 direcciones; Blink (B) si
   un enemigo está a ≤ 16 px y el Blink está listo; en cartas elige la **2.ª**; Nova en cuanto se llena. Es el
   "kiter" de §E6 (C). Los perfiles ciegos A y B quedan como "novato".
2. **Números** (aplicar junto con el bot, una sola vez):
   - Bat `speed` **1.1 → 0.95** px/f: en los minutos 0–5 ningún monstruo corre más que el mago (1.0), quien se mueve
     puede escapar (flujo, R5). El murciélago sigue pareciendo rápido por la onda.
   - `player_hp` **100 → 120**: la build inicial es más pobre desde §C4 y +30 HP mide +14 s en el peor caso; 120 da
     margen a un humano sin mover la curva (R2, perdona sin que se note).
3. **Métricas** (el implementador mide y apunta en G-55.1): Tiempos con **Bot C**, semillas 1–5: llega a 7:00 en
   **≥ 3/5** y C Novice **no** vence F1 en ≥ 4/5 (§E6 #3); perfil A ciego vivo a 2:00 con **≥ 20 HP** y muerto antes
   de 5:00 en ≥ 3/5 (no trivial); perfil C ciego actual = "suelo del novato": ≥ 2:30 en ≥ 3/5.
4. **Si Bot C no llega** (en este orden, una palanca cada vez, R13): daño de esqueleto 6 → 5 antes de 5:00;
   `player_invulnerable_frames` 40 → 45. El tope por minuto no (medido: no cambia nada).

#### Muro de zarzas: CAMBIAR diseño y métrica

| Variante (Start at 14:45, God, Treant) | Golpes máx. por muro | Daño por muro |
|---|---|---|
| base 6 daño / 30 f / empuje 8 px, bot 40 f | **12** | 72 |
| base, bot 20 f (círculo r≈26) | **12** | 72 |
| **4 daño / 60 f / empuje 12 px**, bot 40 f | **7** | 28 |
| idem, bot 20 f | **9** | 36 |

Con un bucle de 26 px ya choca 12 veces: el bot ciego, frenado por la pared sólida, **deriva** hacia ella en cada
vuelta. Un humano ve la pared y no la empuja, así que "≤ 2" era una métrica para alguien que mira (bot irreal). Pero el
diseño también castigaba doble (sólido + ~9 HP/s apoyado), lo que va contra R10. **Decisión**: zarza **4 daño**,
recarga **60 f**, empuje **12 px**. Métrica nueva: **Bot C ≤ 2 golpes por muro**; bot ciego ≤ 9 golpes y ≤ 36 HP por
muro; `test_thorn_first_hurt` ≥ 60 (hoy 68, OK); TTK del Treant 61 s (OK, 60–120).

#### Lobo: OK `wolf_pack_gap` = 75 f

| Separación (8:00–9:00, God, perfil C) | Agachados | Embestidas | Golpes |
|---|---|---|---|
| **75 f** | 29 | 26 | **0** |
| 45 f | 54 | 47 | 30 |
| 30 f | 65 | 57 | 24 |

75 f se queda (deja de ser provisional). Que no golpee a quien no para de moverse es justo lo que pide el lobo: la
embestida castiga quedarse quieto (R5 anticipación, el aviso de 30 f se lee). Con 45 y 30 f se pasa del objetivo de
25. Métrica de control: perfil A (ciego, God) 8:00–9:00 entre **5 y 25** golpes; si da < 5, bajar a 60 f.

#### Jefes de F2 (G-56): OK 500 / 1100 / 3800

Regla de E5 (HP = TTK × DPS medido): Colossus 800 HP → 56 s ⇒ 14 DPS ⇒ **500 ≈ 35 s** (rango 20–40); Banshee
1400 → 60 s ⇒ 23 DPS ⇒ **1100 ≈ 47 s** (30–60); Lich **3800 = 87 s** medido (60–120). Aprobados. Re-medir los dos
primeros con perfil C (God) en la próxima pasada; si salen del rango, corregir con la misma regla.

#### Primer minuto de la meta (CL1M en menús): CAMBIAR detalles

Flujo título → PLAY/SHOP/RECORDS → fase/personaje → partida → resumen → tienda: **se entiende** (rango n/5, precio,
una línea de descripción del elegido, ayuda de botones abajo, REFUND con confirmación, resumen con oro por fuente y
"NEW: ..."; A: RETRY en < 5 s, R10). Todos los textos caben en 240 px. Fallos:

| Pantalla | Veredicto | Detalle |
|---|---|---|
| RECORDS | CAMBIAR | fases sin partidas muestran "Best 00:00/00:01 Kills 0 Lv 0/1"; deben mostrar "--" (R9) |
| Título | CAMBIAR | con 4 cifras, "G 3000" arriba a la derecha pisa la "D" del logo (`meta1`); abajo a la derecha (`meta2`) está bien |
| Tienda, fila de personaje | CAMBIAR | el retrato tapa el inicio de la descripción ("Cinda: ...") y la flecha; bloqueado muestra "?" pero la descripción lo revela todo |
| Elegir fase | CAMBIAR | el retrato tapa "Best 00:01" |
| Tienda | CAMBIAR | precios no asequibles iguales que los asequibles; 12 filas en 2 páginas de 6 sin indicador de "hay más" |
| OPTIONS | FALTA | estaba en el bloque 4 y no está en el menú |
| Cartas / Reroll | OK | "R: REROLL x1" visible; tras usarlo, "PICK ONE" |
| Resumen | REVISAR | en `die` (Start at, DBG) salió "NEW: STORM CHAIN" sin matar al Slime King: Start at no debe acreditar desbloqueos |

#### Lista para el implementador (por prioridad)

1. **DBG Bot C** reactivo (arriba) + Tiempos con él; aplicar bat 0.95 y `player_hp` 120; apuntar resultados en G-55.1.
2. Zarzas: 4 daño, 60 f, empuje 12 px; medir con Bot C (≤ 2) y con el bot ciego (≤ 9).
3. RECORDS: "Best --  Kills --  Lv --" si la fase no tiene partidas.
4. Título: oro siempre abajo a la derecha (nunca sobre el logo), también con 5 cifras.
5. Tienda: retrato en su columna (texto desde x + 20); personaje bloqueado = **nombre + silueta oscura + condición**
   ("Cinda — Clear stage 1"), no "?" (gradiente de meta: se persigue lo que tiene nombre).
6. Tienda: precio en gris si no alcanza el oro; flechas ▲/▼ cuando hay filas fuera de la página; con 0 compras y
   oro ≥ 30, una línea "Gold buys permanent upgrades" (R6: una línea en el momento).
7. Elegir fase: retrato sin tapar texto (≥ 4 px de hueco).
8. **OPTIONS**: Music vol, SFX vol, Screen shake ON/OFF, Flash ON/OFF, Erase save (mantener A 2 s). Qué efectos del
   menú SELECT pasan a OPTIONS lo decide el usuario (design-question; por defecto, shake y flash).
9. RECORDS: lista de desbloqueos con condición y progreso ("Lens of Reach — 500 kills (212/500)"); los bloqueados
   muestran su condición, no "?".
10. Publicación de F1: `debug_menu = false` en la build de itch.io, Start at sin escribir récords ni desbloqueos, y
    repetir Tiempos + Equilibrio (C Medium vence F1 ≥ 3/5) con la ROM final.

### Bloque 5 — F2 Grim Graveyard

**Entra**: paletas de suelo/atrezzo de F2 y pesos de trozos (§C5) · roster F2 (§C2) · arquetipos SHOOT, PHASE, BLINK,
RISE · proyectiles enemigos (12) · formación GRAVE RING · élite Haunted · 3 jefes de §E5.2 · música F2 · fundido
violeta · rótulo de fase.

| Prueba | Cómo | Esperado |
|---|---|---|
| Presentación | captura en 0:04, 2:00, 3:00, 6:00, 8:00 | cada tipo nuevo llega solo (2) |
| RISE justo | 3 partidas C, `test_*` de golpes | 0 golpes de grave walker en los 60 f de su grieta; nunca a < 64 px |
| Tiradores | 6:00–10:00 | ≤ 6 tiradores; destello 24 f antes de cada flecha (captura) |
| Jefes | Start at 4:45 / 9:45 / 14:45 | cada fase vista; TTK en rango; calaveras destruibles |
| VRAM / CPU | minuto 14 + Lich | `test_tiles_used` ≤ 900; CPU ≤ 85 % |
| Equilibrio | C Veteran (1500) | vence F2 ≥ 2/5 |

| Recurso | Tamaño | Frames / etiquetas | Notas |
|---|---|---|---|
| `ghost` | 16×16 | float 0–3, fade 4–5, death 6–8 | también wraith (paleta) |
| `banshee` | 64×64 | float 0–3, wail 4–5, vanish 6–7, death 8–11 | jefe nuevo |
| `bone_colossus` | 64×64 | walk 0–5, throw 6–7, collapse 8–10, death 11–15 | sembrado ampliando `skeleton_big` 2× |
| `lich` | 64×64 | idle 0–3, cast 4–6, teleport 7–9, death 10–14 | jefe nuevo |
| `fx_arrow` | 8×8 | e 0, ne 1 (resto con volteo) | flecha de archer |
| `fx_bone` | 8×8 | spin 0–3 | huesos del Colossus |
| `fx_skull` | 8×8 | fly 0–1 | calaveras del Lich |
| `fx_grave` | 16×16 | crack 0–2 | aviso RISE |
| Paletas | — | podrido, cuervo, carne gris, hueso rojo, violeta; suelo y props F2 | en `palette_c.json` |
| Audio | — | `music_graveyard.xm`; sfx wail, arrow, rise, skull | — |

### Bloque 6 — Rejugabilidad

**Entra**: Hexes (§C5) en la selección de fase · reliquias (cofre del min 10) e iconos en pausa · hitos de mapa
(Stone Circle, Crypt Gate) · personajes **Cinda** y **Morrow** (paletas) · evoluciones **Phoenix Ember**, **Tempest**,
**Eclipse** · semilla visible en pausa.

| Prueba | Cómo | Esperado |
|---|---|---|
| Hexes | Swarm + Frenzy, C Medium | oro +60 % en el resumen; la partida se nota más densa (Cap +8 medido) |
| Reliquias | DBG, cada una | efecto descrito en §C5 visible (captura / `test_*`) |
| Personajes | cada uno 2 min | arma inicial y paleta correctas; stats aplicados |
| Phoenix | morir con Phoenix Ember | revive al 30 % con estallido, una sola vez |
| Hitos | 10 semillas | ≥ 1 hito a < 600 px del inicio en ≥ 7/10; su evento se cumple |

| Recurso | Tamaño | Frames / etiquetas | Notas |
|---|---|---|---|
| `relics` | 16×16 | 9 iconos en el orden de §C5 | paleta recogibles |
| `hex_icons` | 8×8 | swarm 0, frenzy 1, frail 2, dark_moon 3, famine 4, skull 5 | — |
| `wizard` paletas | — | cinda, morrow | sin tiles nuevos |
| `props` (+) | — | stone_circle (piedras rúnicas en anillo), crypt_gate 64×64 | atrezzo |
| `icons` (+) | 16×16 | +phoenix 17, +tempest 18, +eclipse 19 | — |

### Bloque 7 — F3 Ashen Keep y final

**Entra**: suelo/atrezzo F3 + 3 piezas nuevas · roster F3 · BURST · élite Blazing · 3 jefes de §E5.3 (Iron Champion =
paleta del Warchief) · hito Broken Throne · final (§E5.4 paso 4) y créditos · música F3.

| Prueba | Cómo | Esperado |
|---|---|---|
| BURST | Cinder Ring 4:00 | brillo 30 f antes de cada explosión; ≤ 2 explosiones por frame (el resto encola) |
| Muros | 10 semillas | ningún hueco < 32 px; RING aplazado si < 70 % libre |
| Dragon | Start at 14:45 | aliento con aviso 40 f, meteoritos con sombra 45 f, fuego 5 s; TTK 60–120 s |
| Final | DBG F3 + matar Dragon | 3 pantallas, créditos, estrella en el título; la 2.ª vez sólo resumen |
| Técnico | minuto 14 | CPU ≤ 85 %, tiles ≤ 900, ≤ 12 proyectiles enemigos |
| Equilibrio | C Master (3000) | vence F3 ≥ 2/5 |

| Recurso | Tamaño | Frames / etiquetas | Notas |
|---|---|---|---|
| `imp` | 16×16 | fly 0–3, cast 4, death 5–7 | — |
| `gargoyle` | 32×32 | perch 0, fly 1–4, dive 5, death 6–8 | de lado |
| `gargoyle_lord` | 64×64 | fly 0–3, dive 4, stone 5, death 6–9 | sembrado ampliando `gargoyle` 2× |
| `ember_dragon` | 64×64 | idle 0–3, breath 4–6, fly 7–8, land 9, death 10–14 | jefe nuevo |
| `fx_fireball` | 8×8 | fly 0–1 | imps |
| `fx_breath` | 16×16 | burn 0–2 | cono del aliento (varios sprites) |
| `fx_slash` | 32×32 | arc 0–2 | tajo del Champion (aviso en frame 0) |
| `props` (+) | — | brazier 32×32, banner 16×32, lava_crack 32×32, broken_throne 64×64 | brillo por ciclo de paleta |
| `ending_bg` | 240×160 8 bpp | — | ilustración del final |
| Audio | — | `music_keep.xm`, `music_final.xm`; sfx breath, meteor, burst, slash | — |

### Bloque 8 — Equilibrio y publicación

Pasada completa de §E6 (15 partidas por perfil y fase), ajuste con las palancas en su orden, `debug_menu = false`,
kit de itch.io (skill `gba-release`): capturas, GIF del minuto 14, descripción en inglés.

---

## R2. Preguntas abiertas para el usuario (máx. 5; todo lo demás está decidido)

1. **Nombres de las fases**: *Gloam Meadow · Grim Graveyard · Ashen Keep* (**recomendado**: "Mossy" no casa con el
   estilo C nocturno; coste: 1 texto). Alternativa: mantener "Mossy Meadow".
2. **Personajes alternativos** Cinda y Morrow como paletas de Wendel (**recomendado**: 0 animaciones nuevas, dan 2
   motivos para volver). Alternativa: sin personajes (ahorra parte del bloque 6).
3. **Jefes reciclados** (Bone Colossus, Gargoyle Lord): se siembran ampliando 2× el monstruo en un `.aseprite` 64×64
   y **tú los retocas** cuando quieras (**recomendado**; el juego funciona con la ampliación). Alternativa: dibujarlos
   desde cero (más arte) o mostrarlos con afín (más CPU, se ve borroso).
4. **Música**: módulos `.xm` libres (CC0, OpenGameArt/ModArchive) adaptados a cada fase según §E1 (**recomendado**:
   gratis y caben en Maxmod). Alternativa: los compones o encargas tú con esa descripción. (La música de PixelLab sale
   como audio, no como módulo: no sirve para Maxmod sin pasar por tracker.)
5. **Duración del juego**: un jugador medio lo termina en **15–20 partidas (~5 h)** (**recomendado**, §E6 #5).
   Alternativa: 8–10 partidas (precios de la tienda −40 % y HP de jefes −20 %).

## Revisión pre-entrega (gba-designer, ROM 2026-11-25 02:56 de 7504ed8/7fbff55, guardado nuevo)

Cómo: `gbarun.py` sin ventana, 9 ejecuciones. Primer minuto F1 con guardado nuevo (título → PLAY → fase → partida,
cometa a ciegas 8 direcciones + A cada 2,5 s, y otra quieto + A); aperturas de F2/F3 y jefe de minuto 5 de cada fase
(`b2run.py start`, DBG God, Start at 4:30).

### Checklist del primer minuto (diseno-juegos.md)

| Punto | Resultado | Evidencia / regla |
|---|---|---|
| Qué controlo en 10 s | **OK**. Título "PRESS START" → PLAY/SHOP/RECORDS/OPTIONS → CHOOSE STAGE (sólo Gloam Meadow abierta, "L/R: Wendel", "A: START B: BACK") → partida en ~3 s de menús | R14 simple |
| Controles en su momento | **OK**. "D-PAD: MOVE" 0:00–0:03; "GRAB GEMS" junto a la 1.ª gema (0:07); "B: BLINK" al primer golpe (0:22 quieto); "A: NOVA READY!" al llenar el medidor (~0:40). Una línea cada vez, en el momento de uso | R6 |
| Empiezo con lo necesario, sitio seguro | **OK**. Spark Bolt automático, 120 HP; 12 s sin enemigos; la 1.ª oleada (slimes) entra por el borde y se ve llegar | R3, R4, R5 |
| Dificultad de la 1.ª sala | **OK**. Cometa a ciegas: Lv3, 33 muertes, 64/120 HP a 1:02. Quieto + A: primer golpe 0:22, muere a 1:22 (Lv6, 83 muertes). Enseña a moverse sin castigar al que prueba | flujo |
| Nada mata sin aviso | **OK**. Barra de vida roja bajo el mago, silueta blanca al recibir daño, enemigos visibles antes de tocar; F2 grietas ≥ 72 f (`rise_min`), F3 avisos ≥ 45 f (`warn_min`) | R5 |
| Cada acción responde | **OK con una pega**: A con el medidor vacío no hace nada (ni sonido ni parpadeo) | R14 |
| Reintentar < 5 s | **OK**. YOU FELL con resumen → A: RETRY → partida nueva en ~4 s | R10 |

### Legibilidad a 1x (aperturas y jefes)

- F1: slimes (ojos amarillos, borde claro), gemas cian, corazones rojos y orbes rosas propios se distinguen. Flecha
  dorada de borde OK (roza "G 0" del HUD, menor).
- F2 0:00: ghouls verde-azulados sobre suelo violeta, legibles; **se apelotonan encima del mago** (2–3 muertes en
  26 s quieto o a ciegas). Bot C sí sobrevive (77 muertes en 120 s, G-59): es dificultad de entrada, no fallo.
- F3 0:00: grietas de lava del suelo y fuego enemigo usan el mismo naranja que **Ember Toss** del jugador
  (`fx_fireball` y `fx_ember` son 8×8 naranja/amarillo). Lenguaje de color mezclado (peligro = propio).
- Jefes min 5: King Slime (F1) y Gargoyle Lord (F3) claros con barra arriba y rótulo. **Bone Colossus (F2) sale como
  silueta blanca en 2 de 2 capturas** (5:00 y 5:02): `_boss_flash = hit_flash_frames` (4 f) se reinicia en cada
  golpe, y con 3–4 armas el jefe vive en blanco; tapa sus anticipaciones (R5, R11).

### Veredicto: **OK para entregar** la ROM de la mañana. Nada bloquea el primer minuto.

Cambios pedidos (no bloqueantes, por prioridad; tareas G-60, G-61, G-62 y nota en G-59):
1. Destello de jefe con enfriamiento: blanco 2–4 f como mucho cada 12 f (métrica: en 10 capturas seguidas de un jefe
   con Max power, ≤ 3 en blanco). R5/R11. Coste: ~5 líneas en `hb_game.cpp`.
2. Proyectiles enemigos de F3 con otro lenguaje que Ember Toss: contorno oscuro + núcleo blanco/rojo en
   `fx_fireball` (o Ember Toss hacia lila de la paleta del mago). Métrica: captura 1x de F3 7:00 donde se distingan
   sin mirar el movimiento. Lenguaje de color.
3. A con Nova vacía: parpadeo del medidor + clic sordo (respuesta a cada acción). Coste: 1 sonido existente.
4. Revisar con el pase de rendimiento si un A de 1 frame se pierde en frames lentos (quieto a 1:21, A no lanzó la
   Nova con el medidor lleno). Un humano mantiene A ≥ 3 f; riesgo bajo.

### Preguntas para el usuario tras jugar (máx. 5)

1. **Colores de goblin/lobo/orco (G-53)**: aplicado el clásico (goblin verde, orco oliva, lobo gris-pardo, borde
   claro). **Recomendado: confirmar.** Alternativas: tonos fríos (azul/violeta, menos contraste con el suelo) o
   mantener grises (no se distinguen a 1x).
2. **OPTIONS (G-55.2)**: Music, Sound FX, Screen shake, Flash y Erase save. **Recomendado: así**; el resto de
   efectos fijos como están. Alternativa: añadir "Hit numbers" o "Slow-mo" si al jugar molestan.
3. **Nombre**: existe "Hexbound" en Steam (puzle, 2025). **Recomendado: renombrar antes de itch.io** (búsquedas y
   confusión; coste: logo del título + 3 textos). Candidatos: *Moonwick* (ya en DISENO.md §16), *Hex Moon*
   ("THE HEX MOON RISES" ya existe en el juego); comprobar en itch/Steam antes. Alternativa: mantener en itch.io.
4. **Créditos**: hoy "A game by David". **Recomendado: nombre de estudio** igual a la cuenta de itch.io (no publica
   tu nombre; coste 1 texto). Alternativa: tu nombre o alias.
5. **Música final y SFX** (faltan `music_final` y sfx de aliento/meteoro/estallido/tajo; hay provisionales).
   **Recomendado: .xm CC0 adaptados (R2 #4)** en el bloque 8. Alternativa: PixelLab + paso por tracker, o tuyos.
