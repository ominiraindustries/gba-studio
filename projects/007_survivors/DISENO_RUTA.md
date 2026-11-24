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
