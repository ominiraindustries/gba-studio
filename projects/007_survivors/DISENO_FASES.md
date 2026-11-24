# 007_survivors (HEXBOUND) — Fases, oleadas y jefes (G-47)

Parte del diseño completo (ver `DISENO.md` §20). Monstruos, armas y números de contenido en `DISENO_CONTENIDO.md`
(§C1–§C6); hoja de ruta y arte en `DISENO_RUTA.md`. Reglas citadas **R1..R14** de `docs/guias/diseno-juegos.md`.
Sustituye a `DISENO.md` §6 y §8 (los minutos 0–5 de la fase 1 se conservan como están en el código tras §19).

## E1. Estructura de una partida

- Una partida = **una fase de 15:00** en 3 actos de 5 min: *presentar* (0–5) → *desarrollar* (5–10) → *girar y
  cerrar* (10–15). Jefe al final de cada acto (R3, R11).
- Cada fase tiene **una idea nueva** sobre la anterior (R3, R8 "volver da algo nuevo"):
  F1 = esquivar cuerpos (enseña todo el núcleo); F2 = amenazas a distancia y cosas que salen del suelo;
  F3 = fuego que castiga matar de cerca y muros que estrechan el campo.
- Dientes de sierra en todas (checklist de sala): **pausa de 10 s sin apariciones antes de cada jefe**, **descanso
  de 8 s tras matarlo** y bajada de tope al empezar el acto siguiente.
- Tope "Cap" = vivos objetivo; las mezclas son pesos del sorteo (%). Un tipo nuevo entra **solo** (2 ejemplares) en
  su minuto "Desde" y pasa al sorteo 10 s después (§C1).

| Fase | Nombre en pantalla | Tema y decorado | Paleta | Música (descripción para el compositor) |
|---|---|---|---|---|
| 1 | **Gloam Meadow** | prado a medianoche: árboles muertos, setas cian, piedras rúnicas, charcas | suelo `#07060c` azul tinta (actual) | 120 bpm, Re menor; arpegio de cuadrada al 25 % + bajo de triángulo; melodía de "flauta" (cuadrada 12.5 %) misteriosa pero ligera; 64 compases en bucle |
| 2 | **Grim Graveyard** | cementerio: lápidas (tumbas activas), criptas, árboles muertos, niebla | suelo tintado violeta, atrezzo frío | 100 bpm, La menor; órgano (onda de pulso lenta) + campana cada 4 compases; percusión de ruido seco; más grave y espaciada |
| 3 | **Ashen Keep** | patio de castillo en ruinas: muros, columnas, braseros, estandartes, grietas de lava | suelo gris ceniza con vetas brasa | 140 bpm, Mi frigio; bajo machacón, percusión de ruido marcada, melodía en cuadrada 50 % agresiva; la más rápida |
| — | jefe (las 3) | — | — | 150 bpm; ostinato de bajo + redobles; 32 compases; entra con el rótulo del jefe |
| — | jefe final (min 15) | — | — | tema del jefe + capa de melodía; acelera a 160 bpm en la 3.ª fase |
| — | jingles | — | — | STAGE CLEAR 4 s (mayor), YOU FELL 3 s (descendente), LEVEL UP 0.5 s, cofre 1 s |

Formato: módulos `.xm`/`.it` para Maxmod (skill `gba-audio`), ≤ 8 canales, ≤ 64 KB cada uno (pregunta abierta 4).
Fundido de fondo nocturno (§12): F1 0 → 0.35 en 15 min; F2 0.2 → 0.45 hacia violeta; F3 0.1 → 0.3 hacia rojo.

### Oleadas especiales (formaciones; todas con aviso, R5)

| Formación | Qué | Aviso | Principio |
|---|---|---|---|
| **LINE** (Swarm, Murder) | N monstruos en fila cruzan la pantalla recto a la velocidad indicada | 60 f de "!" rojos en el borde de entrada | girar: obliga a apartarse |
| **RING** | N monstruos en círculo r 150 alrededor del mago que se cierra a 0.4 (huecos de 1 monstruo cada 4: salida) | 60 f de "!" en los 4 bordes | posicionarse; Blink o Frost |
| **STAMPEDE** | 2–3 LINE seguidas desde el mismo lado, 20 f entre filas | 90 f de "!" + sonido (aullido/cuerno) | tensión |
| **SWARM** | grupo compacto de 12–24 que entra desde un lado en abanico ±30° | 60 f de "!" | control de grupo |
| **GRAVE RING** (F2) | N tumbas se abren en círculo r 80 alrededor del mago | grietas 60 f (huecos: 1 de cada 4) | R5 |
| **ELITE PAIR** | 2 élites a la vez desde lados opuestos | flecha roja de élite | pico de recompensa |

Las formaciones no cuentan para el tope de su minuto mientras están vivas (sí para el de 56: si no caben, se recortan).

---

## E2. Fase 1 — Gloam Meadow

| Min | Cap | Mezcla (pesos %) | Élites | Evento fijo | Ventanas de evento (§C5) |
|---|---|---|---|---|---|
| 0 | 14 | slime 100 | — | tutorial (§5 de `DISENO.md`) | — |
| 1 | 12 | slime 85→65, skeleton 15→35 desde 1:10 | — | 1:30 shrine | — |
| 2 | 26 | slime 40, skeleton 40, bat 20 | 1/40 | **2:00 Bat Swarm** (LINE 12 bats a 1.6) | Treasure Goblin acto 1 |
| 3 | 32 | skeleton 45, bat 25, slime 15, big slime 15 | 1/40 | 3:00 shrine; 3:00 big slime solo | cofre maldito acto 1 |
| 4 | 38 | skeleton 50, bat 25, big slime 25 | 1/40 | **4:00 Bone Ring** (RING 16 skeletons); 4:30 shrine | — |
| 4:50 | 0 | pausa | — | — | — |
| 5 | 12 | slime 50, bat 50 | — | **5:00 SLIME KING** (§E5) | — |
| 6 | 36 | goblin 40, skeleton 25, bat 25, big slime 10 | 1/30 | descanso 8 s; 6:00 goblins solos (pareja) | TG acto 2 |
| 7 | 40 | goblin 35, wolf 25, skeleton 25, bat 15 | 1/30 | 7:00 2 wolves solos (aullido 1.5 s antes); 7:30 shrine | cofre maldito acto 2 |
| 8 | 44 | wolf 30, goblin 30, skeleton 25, bat 15 | 1/30 | **8:00 Wolf Pack** (STAMPEDE 2 filas × 3 wolves en DASH) | — |
| 9 | 46 | goblin 35, wolf 25, skeleton 25, orc 15 | 1/30 | 9:00 orc solo; **9:15 Goblin Raid** (SWARM 20 goblins); 9:00 shrine | — |
| 9:50 | 0 | pausa | — | — | — |
| 10 | 16 | goblin 60, bat 40 | — | **10:00 ORC WARCHIEF** | — |
| 11 | 48 | orc 20, wolf 25, goblin 25, bat 15, skeleton 15 | 1/20 | descanso 8 s; cofre con **reliquia** | TG acto 3 |
| 12 | 52 | orc 25, wolf 25, goblin 20, skeleton 20, big slime 10 | 1/20 | **12:00 Double Bone Ring** (2 × 16, el 2.º a los 3 s); 12:00 shrine | cofre maldito acto 3 |
| 13 | 54 | orc 30, wolf 30, goblin 20, skeleton 20 | 1/20 | **13:00 Elite Pair**; 13:30 shrine | — |
| 14 | 56 | orc 35, wolf 30, goblin 20, skeleton 15 | 1/15 | **14:00 Stampede** (wolves + orcs, 3 filas) | — |
| 14:50 | 0 | pausa | — | — | — |
| 15 | 16 | goblin 50, wolf 50 | — | **15:00 THORNWOOD TREANT** → STAGE CLEAR | — |

## E3. Fase 2 — Grim Graveyard (requiere superar F1)

| Min | Cap | Mezcla (pesos %) | Élites | Evento fijo |
|---|---|---|---|---|
| 0 | 16 | ghoul 70, crow 30 (crow desde 0:30) | — | rótulo "GRIM GRAVEYARD" 2 s; 4 ghouls a 0:04 |
| 1 | 20 | ghoul 50, crow 25, skeleton 25 | — | 1:30 shrine |
| 2 | 28 | ghoul 35, skeleton 35, crow 15, grave walker 15 | 1/40 | 2:00 2 grave walkers solos (2 tumbas a la vista) |
| 3 | 34 | skeleton 35, ghoul 25, ghost 20, grave walker 20 | 1/40 | 3:00 2 ghosts solos; **3:20 Murder of Crows** (2 LINE de 10 en X a 1.6); 3:00 shrine |
| 4 | 40 | skeleton 35, ghost 25, ghoul 20, grave walker 20 | 1/40 | **4:00 Grave Ring** (12 walkers); 4:30 shrine |
| 4:50 | 0 | pausa | — | — |
| 5 | 12 | ghoul 50, crow 50 | — | **5:00 BONE COLOSSUS** |
| 6 | 38 | skeleton 30, ghoul 30, ghost 20, bone archer 20 | 1/30 | 6:00 2 archers solos (máx. 6 tiradores) |
| 7 | 42 | archer 25, ghost 25, skeleton 25, ghoul 25 | 1/30 | **7:00 Ghost Swarm** (SWARM 16 ghosts); 7:30 shrine |
| 8 | 46 | ghost 20, archer 20, skeleton 25, ghoul 20, wraith 15 | 1/30 | 8:00 2 wraiths solos |
| 9 | 48 | skeleton 25, ghoul 25, archer 20, wraith 15, grave brute 15 | 1/30 | 9:00 brute solo; **9:20 Bone Ring** (20 skeletons); 9:00 shrine |
| 9:50 | 0 | pausa | — | — |
| 10 | 16 | ghoul 50, ghost 50 | — | **10:00 BANSHEE** |
| 11 | 50 | skeleton 20, ghoul 20, archer 20, wraith 15, brute 15, bone knight 10 | 1/20 | descanso; reliquia; 11:00 bone knight solo |
| 12 | 52 | igual que 11 | 1/20 | **12:00 Murder ×2 + Grave Ring 12**; 12:00 shrine |
| 13 | 54 | brute 20, knight 15, wraith 20, archer 20, skeleton 25 | 1/20 | **13:00 Ghost Swarm + Elite Pair**; 13:30 shrine |
| 14 | 56 | brute 25, knight 20, wraith 20, archer 15, ghoul 20 | 1/15 | **14:00 Stampede** (brutes + knights) |
| 14:50 | 0 | pausa | — | — |
| 15 | 16 | ghoul 50, ghost 50 | — | **15:00 THE LICH** → STAGE CLEAR |

Tumbas RISE: sólo en trozos "lápidas" a 64–112 px del mago; además, fuera de oleadas, 1 de cada 6 grave walkers sale
de grieta en suelo libre (mismo aviso). Nunca dentro de la pantalla a < 64 px (R5).

## E4. Fase 3 — Ashen Keep (requiere superar F2)

| Min | Cap | Mezcla (pesos %) | Élites | Evento fijo |
|---|---|---|---|---|
| 0 | 18 | cinder slime 70, ash bat 30 (desde 0:30) | — | rótulo; 1 cinder slime a 0:04 que explota lejos (enseña el brillo de 30 f) |
| 1 | 22 | cinder 50, ash bat 30, imp 20 | — | 1:30 2 imps solos; 1:30 shrine |
| 2 | 30 | cultist 30, imp 25, cinder 25, ash bat 20 | 1/40 | 2:00 cultists solos; **2:30 Ash Swarm** (LINE 14 ash bats a 1.7) |
| 3 | 36 | cultist 30, cinder 30, imp 25, gargoyle 15 | 1/40 | 3:00 gargoyle solo; 3:00 shrine |
| 4 | 42 | cultist 30, gargoyle 20, imp 25, cinder 25 | 1/40 | **4:00 Cinder Ring** (RING 16 cinder slimes: explotan en cadena); 4:30 shrine |
| 4:50 | 0 | pausa | — | — |
| 5 | 12 | cinder 50, ash bat 50 | — | **5:00 GARGOYLE LORD** |
| 6 | 40 | cultist 30, imp 25, gargoyle 15, cinder 15, ash knight 15 | 1/30 | 6:00 ash knight solo |
| 7 | 44 | cultist 25, imp 25, knight 20, gargoyle 15, cinder 15 | 1/30 | **7:00 Imp Swarm** (SWARM 12 imps; disparan escalonados, 1 cada 10 f); 7:30 shrine |
| 8 | 48 | knight 20, cultist 25, imp 20, gargoyle 15, ember warden 20 | 1/30 | 8:00 warden solo |
| 9 | 50 | knight 20, warden 15, cultist 20, imp 15, hellhound 15, gargoyle 15 | 1/30 | **9:00 Hound Pack** (STAMPEDE 2 × 3 hellhounds); 9:00 shrine |
| 9:50 | 0 | pausa | — | — |
| 10 | 16 | cultist 50, ash bat 50 | — | **10:00 IRON CHAMPION** |
| 11 | 52 | knight 20, warden 15, hellhound 20, imp 15, cultist 15, gargoyle 15 | 1/20 | descanso; reliquia |
| 12 | 54 | igual que 11 | 1/20 | **12:00 Double Cinder Ring**; 12:00 shrine |
| 13 | 56 | knight 25, hellhound 25, warden 20, gargoyle 15, imp 15 | 1/20 | **13:00 Hound Pack + Elite Pair**; 13:30 shrine |
| 14 | 56 | knight 30, hellhound 25, warden 25, gargoyle 20 | 1/15 | **14:00 Grand Stampede** (knights + hounds, 3 filas) |
| 14:50 | 0 | pausa | — | — |
| 15 | 16 | cultist 50, imp 50 | — | **15:00 EMBER DRAGON** → **FINAL** (§E5.4) |

Muros: plantillas de muros al 20 % (§C5) → pasillos de ≥ 32 px; las formaciones RING sólo se lanzan si hay ≥ 70 % de
celdas libres en r 150 (si no, se aplaza 10 s): nada encierra al jugador (R9).

---

## E5. Jefes (R11: examinan lo aprendido; fases que cambian el comportamiento)

### Reglas comunes

| Regla | Valor | Principio |
|---|---|---|
| Entrada | pausa 10 s; rótulo con el nombre 2 s; mosaico 30 f; entra caminando desde el borde o cae con **sombra 60 f** (nunca encima: ≥ 96 px) | R5 |
| Barra de vida | arriba, con muescas en los cambios de fase | un estado siempre se ve (R9) |
| Cambio de fase | 60 f invulnerable con destello + rugido + empuje de 48 px a monstruos normales | R11: se ve que cambia |
| Normales durante el jefe | el Cap de la fila del jefe en la tabla de su fase (12–16, ≈ 30 % del minuto anterior) | flujo |
| Distancia | si el jefe queda a > 160 px, triplica su velocidad hasta volver a 120 px | sin huidas eternas |
| Nova | 120 de daño a jefes (40 × 3) | — |
| Ira | jefes de min 5 y 10: +30 % velocidad a los 2:00 de combate. Jefe final: a los 3:00 "THE HEX MOON RISES": +50 % velocidad, normales al 100 % del cap | evita bloqueo; R9 |
| Botín | cofre de jefe (§C3) + oro (§C4); 1.ª muerte desbloquea (§C4) | recompensa |
| Arte | 64×64, un sprite, **sin afín** (§C6); los jefes "reciclados" usan un `.aseprite` 64×64 sembrado ampliando 2× el monstruo (pregunta 3) | presupuesto |

Vida: HP inicial y **TTK objetivo** (segundos para matarlo con el perfil C, §E6). Regla de ajuste (R13): tras medir,
`HP = TTK_objetivo × DPS_medido` redondeado a 100.

### E5.1 Fase 1

| Jefe | HP / TTK | Contacto · vel. | Fase 1 (100–50/60 %) | Fase 2 | Fase 3 | Examina |
|---|---|---|---|---|---|---|
| **Slime King** (5:00) — `slime_king` | 600 / 20–40 s | 12 · salta | salta hacia ti: **sombra 45 f** antes de caer, onda r 32 (12) (hecho) | < 50 %: al caer suelta 4 slimes | < 25 %: saltos cada 60 f, sombra 30 f | moverse a tiempo |
| **Orc Warchief** (10:00) — `orc_warchief` | 3000 / 30–60 s | 15 · 0.5 | **Carga**: pisotones 40 f (polvo + línea de 4 "!" en su dirección), embiste a 3.5 px/f hasta 90 f; si choca con atrezzo queda **aturdido 60 f** (ventana); cada 4 s | < 60 %: **Cuerno** 45 f (temblor 1 px) → 8 goblins desde 2 lados; sigue cargando | < 30 %: **carga doble** (segundo aviso de 20 f, reorienta) | esquivar en línea, Blink, grupo |
| **Thornwood Treant** (15:00) — `treant` nuevo | 7000 / 60–120 s | 16 · 0.25 | **Raíces**: 3 grietas bajo/junto al mago (**40 f**), púas r 12 durante 20 f (14); cada 150 f | < 66 %: **Muro de zarzas** (G-52.1): cámara fija + elipse de 24 zarzas centrada en el mago, 104×56 → 88×48 en 10 s, brotes 60 f antes de dañar, sólido (6 daño/30 f + empuje); ver "Decisión G-52.1" | < 33 %: **Semillas**: brilla 30 f → 8 semillas radiales a 1.2 px/f (8); cada 20 s llama 4 wolves; raíces cada 120 f | posicionarse en espacio reducido |

**Decisión G-52.1 — Muro de zarzas del Treant** (sustituye "r 150 → 110 del punto de entrada"):

Problema medido: la pantalla es 240×160 y la barra superior del HUD ocupa ~24 px, así que con la cámara siguiendo al
mago sólo quedan ~56 px visibles hacia arriba y 80 hacia abajo; un anillo de r 150/110 enseña 2–4 espinas con huecos
de ~40 px (capturas `tr1`/`rg2`: el muro no se lee). Un peligro que no se ve rompe **R5** (todo peligro se ve antes de
dañar) y **R9** (un estado del mundo siempre se ve). Opción elegida: mezcla de (a) y (c) con cámara fija.

| Parámetro | Valor | Porqué (regla) |
|---|---|---|
| Centro | posición del **mago** al lanzar (no el punto de entrada) | nunca empieza fuera; elimina el caso "mago fuera" (R9) |
| Cámara | durante el muro, se desliza en 20 f a (centro.x, centro.y − 12) y **queda fija**; vuelve a seguir al mago en 20 f al marchitarse | la arena entera en pantalla; el −12 centra la elipse en el área útil y = 24..160 (legibilidad) |
| Forma | **elipse** rx 104 × ry 56 → se cierra a rx 88 × ry 48 en 600 f (10 s), lineal | borde exterior de las zarzas (r + 8) a ≥ 8 px del borde de pantalla y del HUD; 240×160 no admite un círculo legible |
| Sprites | **24** zarzas 16×16 (+8 OAM sobre lo previsto); paso ~21 px al abrir, ~18 px cerrado | con 16 el hueco sería ~16 px y se leería como puntos sueltos; con 24 se lee como muro. Si OAM no da, 20 (mínimo) |
| Aviso | **60 f** de brotes: 24 retoños pequeños parpadeando (sin daño) en las posiciones iniciales + temblor 1 px del Treant; luego crecen en 8 f y ya dañan | R5 + anticipación; igual que las grietas de raíces (40 f) pero más largo porque encierra |
| Colisión mago | zarzas **sólidas** para el mago (se le limita a la elipse r − 8, incluido Blink); tocar = 6 daño/30 f + empuje 8 px hacia el centro | sin estado "fuera" ni softlock (R9); castigo pequeño (R10) |
| Mago fuera | no puede ocurrir (centro en el mago + sólido); si por un fallo ocurre, se le recoloca dentro en 1 frame | defensa, no mecánica |
| Monstruos y jefe | atraviesan las zarzas; el Treant (0.25 px/f) entra caminando; oleadas sin cambio | coste bajo; la presión viene de dentro |
| Duración | 20 s (10 cerrándose + 10 fijo) → se marchita 30 f (parpadeo, sin daño) → 10 s libre → se repite mientras siga < 66 % y ≥ 33 %; al pasar a < 33 % se marchita en el acto | ritmo con silencios (R4); fase 3 cambia de idea (R11) |

Métricas (R13, perfil C con `gbarun.py`): `test_wall_onscreen` = 24 zarzas dentro de 0..239 × 24..159 en todos los
frames del muro; primer daño de zarza ≥ 60 f tras los brotes; golpes de zarza por muro ≤ 2 con el bot; TTK del Treant
sigue en 60–120 s. Captura obligatoria con el muro cerrado para revisar que se lee como pared.

### E5.2 Fase 2

| Jefe | HP / TTK | Contacto · vel. | Fase 1 | Fase 2 | Fase 3 | Examina |
|---|---|---|---|---|---|---|
| **Bone Colossus** (5:00) — `bone_colossus` (= `skeleton_big` ×2) | 1000 / 20–40 s | 14 · 0.4 | **Huesos**: levanta el brazo 30 f (destello) → 3 huesos en abanico 20°, 1.4 px/f (10); cada 150 f | < 50 %: **Derrumbe**: se desmonta 90 f (invulnerable, se ve el montón), 6 huesos giran en anillo r 40 durante 3 s (8), se rearma | < 25 %: 5 huesos, vel. 0.6 | leer proyectiles, distancia |
| **Banshee** (10:00) — `banshee` nuevo | 4000 / 30–60 s | 12 · flota 0.8 a 90 px | **Lamento**: boca abierta 45 f + "!" alrededor → onda que crece de 0 a 96 px en 30 f (12); se esquiva alejándose o cruzándola con Blink | < 60 %: intangible 3 s de cada 8 (alfa 50 %), llama 4 ghosts | < 30 %: **se teletransporta** junto al borde de la pantalla (marca 45 f), lamento cada 4 s | Blink, intangibles |
| **The Lich** (15:00) — `lich` nuevo | 9000 / 60–120 s | 14 · 0.3 | **Calaveras**: báculo brilla 30 f → 5 calaveras que giran 2°/f hacia ti a 1.2 px/f, 4 s de vida (10); **se destruyen con 1 golpe** de cualquier arma | < 66 %: **Alzar muertos**: 4 grietas (60 f) a 64–96 px → grave walkers; calaveras cada 5 s | < 33 %: **Campo de muerte**: fundido 0.5; se teletransporta cada 6 s (humo 40 f en el destino); calaveras + alzar | todo F2 junto |

### E5.3 Fase 3

| Jefe | HP / TTK | Contacto · vel. | Fase 1 | Fase 2 | Fase 3 | Examina |
|---|---|---|---|---|---|---|
| **Gargoyle Lord** (5:00) — `gargoyle_lord` (= `gargoyle` ×2) | 1400 / 20–40 s | 14 · vuela 0.9 a 100 px | **Picado**: sombra en tu posición 45 f → cae a 3 px/f; al aterrizar 4 esquirlas radiales 1.4 px/f (8); cada 5 s | < 50 %: **Piel de piedra** 4 s (gris, recibe 25 %), llama 4 gargoyles | < 25 %: picado doble | leer sombras |
| **Iron Champion** (10:00) — `orc_warchief` (paleta acero) | 5000 / 30–60 s | 16 · 0.45 | **Escudo**: de frente recibe 25 % (chispa gris al bloquear); **Tajo**: arco rojo 30 f → barrido 90° delante r 40 (16); cada 3 s | < 60 %: **Salto**: se agacha 40 f, sombra en tu posición, cae con onda r 32 (14) | < 30 %: llama 4 ash knights; tajo doble | atacar por detrás: Rune Orbit, Lance trasera, posicionarse |
| **Ember Dragon** (15:00) — `ember_dragon` nuevo | 12000 / 60–120 s | 16 · 0.3 | **Aliento**: cabeza brilla + humo 40 f → cono 60° de 96 px que barre 30° en 60 f (4 cada 10 f) | < 66 %: **Vuelo**: despega (sombra que encoge), 6 meteoritos con sombra **45 f** cerca de ti (r 16, 14) durante 6 s; aterriza con sombra 60 f | < 33 %: los meteoritos dejan fuego 5 s (2/10 f); alterna aliento y vuelo; 4 hellhounds cada 20 s | todo F3 junto |

### E5.4 Final del minuto 15

1. Muere el jefe final → cámara lenta 30 f (lógica a 1/2), destello blanco, todos los monstruos normales hacen poof
   (sin XP), todas las gemas vuelan al mago.
2. "STAGE CLEAR" 3 s + jingle → **resumen**: tiempo, nivel, muertes, oro ganado (desglosado), "NEW BEST", "NEW: ..."
   desbloqueos. Bloqueo de A/B 60 f (§19).
3. A = continuar al título/selección de fase (la siguiente ya desbloqueada y seleccionada).
4. **F3 superada por primera vez → final**: 3 pantallas de texto sobre una ilustración 240×160 (8 bpp, como el
   título): *"The Hex Moon fades."* / *"Wendel walks home at dawn."* / *"THE END — thanks for playing"*; créditos
   20 s (START los salta); luego título con estrella dorada junto a HEXBOUND. Las siguientes veces sólo el resumen.
5. Tras el final: las 3 fases quedan para batir récords y Hexes; no hay modo infinito (alcance).

---

## E6. Curva de dificultad y equilibrio (R13: mide, no opines)

### Perfiles de entrada reproducibles (`gbarun.py`, semillas 1..5)

| Perfil | Qué hace | Representa |
|---|---|---|
| **A** "círculo" | cuadrado de 90 f por lado; A cada 2.5 s (primera carta, Nova); B cada 7 s (§19) | novato pasivo |
| **B** "paseante" | lados de 240 f; mismo A/B | novato que se mueve |
| **C** "kiter" | círculo amplio: 8 direcciones de 40 f cada una en orden (≈ r 50); A cada 2.5 s; B al recibir daño (cada 3 s máx.); en cartas elige la **2.ª** (no siempre la primera) | jugador medio |

Herramientas que pide el bloque 2 al menú DBG: **Stage N**, **Start at mm:ss** con nivel y armas típicos de ese
minuto (tabla de `xp_needed` y la build de C a ese minuto), **Meta preset** (Novice = 0 oro gastado; Medium = 500;
Veteran = 1500; Master = 3000: rangos repartidos Might > Vitality > Haste > Magnet > resto) y lectura de
`test_*`: minuto, HP, nivel, muertes, `test_tiles_used`, `test_cpu_max`, `test_boss_hp`, `test_dead_at`.

### Objetivos

| # | Objetivo | Cómo se mide | Principio |
|---|---|---|---|
| 1 | Novato llega a **3:00** en F1 en su 1.ª partida | A y B, Novice, semillas 1–5: tiempo de muerte ≥ 3:00 en ≥ 3/5 | flujo, CL1M |
| 2 | Novato vence al **Slime King en ≤ 3 partidas** | B con Novice, luego con 35 y 70 oro gastado: vence en ≥ 3/5 con 70 | R10 fallar cuesta poco |
| 3 | Jugador medio **supera F1 en 4–6 partidas** | C con Medium (500): vence F1 en ≥ 3/5; C con Novice: llega a 10:00 en ≥ 3/5 y **no** vence en ≥ 4/5 | R12 la tienda importa |
| 4 | F2 con ~1500 gastado, F3 con ~3000 | C Veteran vence F2 ≥ 2/5; C Master vence F3 ≥ 2/5 | dientes de sierra entre fases |
| 5 | Juego completo en **15–20 partidas** (~5 h) para el jugador medio | suma de 3 + 4 con oro medido por partida | pregunta abierta 5 |
| 6 | Tiempo para matar jefes (TTK) | C en "Start at" justo antes del jefe: dentro del rango de §E5 | R11 jefe que examina, no esponja |
| 7 | Nivel por minuto | C: Nv5 ≈ 1:45, Nv10 ≈ 4:00, Nv12 ≈ 5:00, Nv20 ≈ 10:00, Nv28 ≈ 15:00 (±30 %) | gradiente de meta |
| 8 | Tensión | C: HP mínimo de cada acto entre 25 % y 70 % (nunca ileso, rara vez al borde) | tensión/alivio |
| 9 | Muertes por minuto | histograma de `test_dead_at` en 15 partidas: picos en eventos y jefes, nunca en los 30 s de pausa/descanso | R4 |
| 10 | Técnica | `test_cpu_max` ≤ 85 % partida normal, ≤ 100 % peor caso; `test_tiles_used` ≤ 900; 0 capturas con parpadeo en el minuto 14 | §C6 |

### Palancas de ajuste (en este orden; una cada vez, R13)

1. Tope del minuto (Cap) ±4 · 2. mezcla (peso del tipo que más daño hace) · 3. HP de jefe (regla del TTK) ·
4. escalado `0.07`/min ±0.02 · 5. daño de contacto del tipo concreto ±1 · 6. precios de la tienda ±20 %.
No se tocan la velocidad del mago, la invulnerabilidad (40 f) ni la caja 8×8 sin una design-question (núcleo).
