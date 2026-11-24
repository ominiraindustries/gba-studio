# 007_survivors

Juego GBA hecho con [Butano](https://github.com/GValiente/butano) (motor C++), creado con
`python -I tools/scripts/new_game.py 007_survivors`.

## Compilar y jugar

- Doble clic en `compilar.bat` (solo compila) o `compilar_y_jugar.bat` (compila y abre mGBA).
- Desde Git Bash, en la raíz del repo:

      source tools/env.sh
      make -C projects/007_survivors -j8

## Controles (bloque 1)

Cruceta: mover (el mago mira hacia donde dispara). A: Hex Nova cuando el medidor de abajo a la izquierda está lleno.
B: Blink (salto corto, 3 s de recarga). START: pausa (armas y pasivas, Resume / Quit). SELECT: menú de efectos.

## Depuración (menú SELECT, `hb::debug_menu` en `include/hb_config.h`)

Filas `DBG` (ver "Bloque 2"): **God mode**, **Max power**, **Start at**, **Stress 56**, **Force**. Antes: **Skip to 4:30** (el jefe sale a
5:00), **Stress 56** (mantiene 56 enemigos para medir). Poner `debug_menu = false` para la versión publicada.

## Probar sin ventana (Mesen 2 headless)

Globales `test_*` de `src/hb_game.cpp` (modo, reloj, nivel, enemigos, CPU del peor frame en 1/10000 de frame, Nova…).
Ejemplo: arranque + 20 s moviéndose en círculo:

    python -I tools/scripts/gbarun.py projects/007_survivors/007_survivors.gba --frames 1300 --shot 30,1200         --input "40:start;100-400:right;400-700:down;700-1000:left" --check "test_time_s.32>=15" --nonblank         --out <scratchpad>/run_007

Para el jefe: SELECT (frame 60), 11 × abajo, A (God), abajo, abajo, A (Skip to 4:30), SELECT; luego moverse y pulsar A
de vez en cuando (cartas).

## Rendimiento (G-45)

Peor frame medido con `test_cpu_max` (1/10000 de frame) y por secciones en `test_prof_worst[14]` / `test_prof_nova[14]`
(ticks de `bn::timer`, 4389 = 1 frame; orden en el comentario de `src/hb_game.cpp`).

| Caso | Antes | Después |
|---|---|---|
| Max power + Stress 56, peor frame | 145 % | 92 % |
| Frame de la Nova | 159 % | 88 % |
| Partida normal 4:30-5:10 (God + Skip) | 76 % | 78 % |

Tras cablear 8 direcciones + fx_nova/fx_blink/hud_nova (0805ce2): Max power + Stress 56 = 97,7 %, frame de la Nova
= 90,3 %, 4:30-5:06 = 77,1 % (todos <= 100 %).

Qué se hizo: tiles y paletas de enemigos cacheados (`sprite_tiles_ptr`/`sprite_palette_ptr`), sprites de chispa, runas
y escarcha reutilizados (crear un sprite cuesta ~5-10 % de frame con 150 vivos), cubos espaciales de 32 px para
armas y separación, golpes de la Nova repartidos en 6 frames (anillo que crece), `_update_enemies` en ARM/IWRAM
(`HB_HOT`), 1 número de daño nuevo por frame y Stress repone 4 por frame.

## Bloque 2 (G-49): F1 acto 2, 5:00-10:00

Goblin (flanco ±30°), Wolf (DASH: se agacha 30 f con ojos rojos sólo en pantalla, embiste 20 f a 3 px/f), Orc (TANK),
Big Slime (HOP + SPLIT en 2 slimes), Treasure Goblin (FLEE 20 s, flecha dorada, 30-60 oro + corazón), élite Splitting
(3 copias al 30 %), Wolf Pack 8:00, Goblin Raid 9:15, Orc Warchief 10:00 (carga con 4 "!", aturdido al chocar con
atrezzo, cuerno con 8 goblins < 60 %, carga doble < 30 %), cofre de jefe 1/3/5, Ember Toss, Storm Chain, Spell Tome,
Lodestone, 4 huecos de arma + 4 de pasiva, vida por minuto (§C1), música (título, prado, jefe) y efectos finales.
Al matar al Warchief: STAGE CLEAR (los minutos 10-15 son el bloque 3).

Menú SELECT, filas DBG: God mode, Max power (4 armas sin Ember + pasivas a Nv5), **Start at** (izq./der.: 4:30,
5:50, 7:00, 8:00, 9:00, 9:45; nivel y armas de un perfil C a ese minuto), Stress 56 (mezcla de todos los monstruos),
**Force** (izq./der.: Treasure, Split elite, Wolves, Warchief, Wolf Pack, Goblin Raid, Big slime).

Pruebas: `python -I projects/007_survivors/tests/b2run.py stress|start <salida>` (variables START_AT, FORCE, GOD,
SECONDS_RUN, SHOTS, PROF=1 para secciones). Imprime `test_tiles_used`, `test_cpu_max`, `test_boss_ttk`, `test_events`
(bits en `src/hb_game.cpp`), `test_wolf_*`.

| Medida (G-49) | Resultado |
|---|---|
| VRAM de sprites, Stress 56 mezclado + cartas | pico 797 tiles con cartas abiertas, 501 en juego (≤ 900): un bloque por dirección y arte, recargado con `set_tiles_ref` |
| Stress 56 + Max power, con música | 118 % → 97,9 % (cadena de un salto por frame, presupuesto de 4 sprites de muerte/poof/gema por frame, relleno de Stress 2 por frame) |
| Perfil C 4:30-10:10 (God) | 116 % → 95,3 % (formaciones entran 2 por frame, 2.ª bola de Ember 8 f después). **Objetivo 85 % aún no** |
| Warchief, perfil C Lv 20 (God) | TTK 48 s con 1000 de vida (3000 daba ~130 s; regla R13 de E5) |

## Bloque 3 (G-52): F1 completa, 10:00-15:00

Minutos 11-15 de E2 (tope y mezcla por minuto, élites 1/20 y 1/15), Warchief deja cofre y 8 s de descanso, 12:00
Double Bone Ring, 13:00 Elite Pair, 14:00 Stampede (lobos, orcos, lobos; cuerno), pausa 14:50, **Thornwood Treant**
15:00 (raíces con grieta 40 f, muro de 16 zarzas r 150 -> 110 en 10 s bajo 66 %, semillas y 4 lobos cada 20 s bajo
33 %), ira (Slime King y Warchief +30 % a 2:00; Treant a 3:00 "THE HEX MOON RISES" +50 % y tope 56), cofre maldito
(1 ventana del 60 % por acto: 2 mejoras, "CURSED!", 20 s de élites, máx. 5), final E5.4 (cámara lenta, poof sin XP,
gemas al mago, STAGE CLEAR 3 s + jingle, resumen con oro por origen). Jingles: jefe caído, victoria, derrota (la
música se para antes). Lodestone >= 3: la Nova atrae todas las gemas. Copias Splitting al 50 % de tamaño lógico.

DBG: Start at añade 10:30, 11:50, 12:50, 13:50, 14:45 (marca los tutoriales como vistos); Force añade Cursed chest
(delante del mago), Treant, Elite pair, Stampede. "Stage N" y "Meta preset" quedan para el bloque 4 (no hay fases
ni meta todavía).

Revisión del bloque 2 (DISENO_RUTA.md) aplicada: correa del Warchief 100 -> 64 px, cargas sólo con el jefe en
pantalla (`test_charge_offscreen`), "!" cada 12 px dentro de pantalla y parpadeo cada 4 f; lobo agachado parpadea
rojo cada 4 f, máx. 3 agachados, embestida 8 de daño; tope x0,6 10 s tras 6:00/7:00/9:00; tipo retenido redibujado
de la mezcla; Treasure Goblin lejos de eventos (6:25-6:50 o 7:20-7:50) con flecha en la paleta de la moneda;
efectos de combate y rótulos ocultos con cartas/pausa; Slime King 450 de vida.

Rendimiento (perfil C 4:30-10:10, peor frame): `_update_horde_frames` retargeteaba todas las artes en el mismo
frame (632 ticks): ahora con desfase, sólo artes vivas y 2 por frame (222). Los grupos normales entran por la cola
(2 por frame). `chunk_template` con caché de 16 entradas (lo llaman 2-3 veces por monstruo y frame).

Estabilidad G-52 (bloque 3): Stress + Max power 130 % -> 98 % (fragmentos de Frost Ring y chispas de Spark se crean
al subir el arma, no en el primer disparo; Max power escalona los enfriamientos). Lobo: máx. **2** agachados a la vez y
**300 f** de espera tras embestir (antes 3 y 180 f); golpes de embestida 8:00-9:00 perfil C God: 44 -> 41 (objetivo
<= 25 aún no). Parche de rendimiento en pruebas (perfil C 90 -> 86 %, pero rompe la subida de nivel; sin integrar):
`tests/tmp_g52_perf_wip.patch`.

## Bloque 4 (G-55): meta, juego completo de una fase

Flujo: título (PRESS START) -> menú PLAY / SHOP / RECORDS -> elegir fase (cartas `stage_cards`; Grim Graveyard y
Ashen Keep "COMING SOON") y personaje con L/R -> partida -> resumen (oro ganado y total, NEW BEST, "NEW: ...") ->
A: RETRY / B: MENU. Salir desde la pausa también guarda el oro.

- Guardado `bn::sram` (`include/hb_save.h`, `src/hb_save.cpp`): struct de §C4 con magic "HXB1", versión y checksum;
  cabecera mala -> guardado nuevo; versión antigua -> campos nuevos a 0 (`version_size`). Se escribe al acabar la
  partida, al comprar o devolver. Mesen lo deja en `Documents/Mesen2/Saves/007_survivors.sav`.
- Tienda (`src/hb_meta.cpp`): 11 mejoras de §C4 + Cinda (300, tras superar F1) y Morrow (600, tras F2). A compra,
  SELECT devuelve todo (con confirmación). Efectos en la partida: Might, Vitality, Haste, Magnet, Greed (al banco),
  Growth, Armor, Recovery, Reroll (R en las cartas), Charged, Revival.
- Oro al banco = oro de la partida + 5/minuto + 100 si se supera la fase, x Greed.
- Desbloqueos: Ember Toss (5:00), Storm Chain (Slime King), Lens of Reach (500 muertes), Ward Stone (Warchief),
  Moon Lance (10:00), F2 (superar F1). Antes no salen en las cartas.
- Personajes: paletas `wizard_cinda_palette` / `wizard_morrow_palette` sobre Wendel; Cinda empieza con Ember Toss,
  +10 % área, -10 HP; Morrow con Storm Chain, +10 % velocidad, Blink -20 %, -15 % daño.
- Armas: Moon Lance (haz de 4 tramos `fx_lance` con matriz afín, 8 direcciones, Nv4 también detrás), Lens of Reach
  (+10 % área), Ward Stone (-6 % daño recibido, Nv5 +10 f invulnerable). Evoluciones al abrir cofre de jefe/élite:
  Starfall (estrellas `fx_star`), Blizzard (r 96, congela 60 f), Ward of Ages (6 runas doradas r 40).
- Muro de zarzas G-52.1: elipse de 24 zarzas centrada en el mago, cámara fija (-12 px), brotes 60 f, sólido.
- Lobo: 420 f de espera tras embestir + máx. 2 agachados + **75 f entre dos lobos que empiezan a agacharse**
  (provisional, G-55: sin esto 45 golpes en 8:00-9:00; con él 0).
- VRAM con cartas: partículas y números se descartan al abrir un menú, el destello de subida se libera bajo 3 cartas
  y el reloj se redibuja al cerrar (Start 9:45: 992 -> 898 tiles).

DBG nuevo: "Meta preset" (+3000 oro, desbloqueos de F1 y Moon Lance Nv5; se guarda al momento).

Pruebas (`tests/b2run.py`): `meta` (PHASE=1 compra/devuelve/récords/selección; luego `KEEP_SAVE=1 PHASE=2` comprueba
que lo comprado sigue tras reiniciar), `times` (SEED 1-5, Tiempos), `start` con `MAX=1 META=1 FORCE=1` (evolución y
Moon Lance). Por defecto cada prueba borra el `.sav` de Mesen.

## Bloque 5 (G-56): F2 Grim Graveyard

Se abre al superar F1 (`unlock_stage2`; tras la victoria la siguiente fase queda seleccionada). **DBG**: SELECT en la
pantalla de elegir fase abre F2 sin superar F1. Todo el código de F1 se reutiliza con datos por fase (`_stage`):

- Suelo/atrezzo con `ground_graveyard_palette` / `props_graveyard_palette`; trozo nuevo "lápidas" (4 `log`, 25 % de
  los trozos en F2); fundido violeta 0.2 -> 0.45; `music_graveyard`; rótulo "GRIM GRAVEYARD" 2 s.
- Bestiario §C2 como tipos con paleta (`enemy_def::pal`): ghoul, crow, skeleton, grave walker, ghost, bone archer,
  wraith, grave brute, bone knight. Sólo las artes de la fase tienen bloque de tiles (`enemy_def::stages`).
- Comportamientos: RISE (grieta `fx_grave` 60 f a 64-112 px delante de una lápida, máx. 4 abiertas; 1 de 6 en suelo
  libre; sale inofensivo 20 f), SHOOT (72-96 px, destello blanco 24 f, flecha 1.2 px/f; máx. 6 tiradores), PHASE
  (60 f de cada 240 intangible, alfa 50 %), BLINK (sombra que crece 45 f). Élite Haunted (suelta un ghost), máx. 4.
- Proyectiles enemigos: pool de 12 (`_shots`): flechas, huesos, calaveras (se rompen con chispa, runa o explosión).
- Línea temporal E3 completa: 0:04 ghouls, 0:30 crows, 2:00 walkers de 2 tumbas, 3:00 ghosts, 3:20 Murder of Crows
  (2 LINE de 10 en X), 4:00 Grave Ring 12, 6:00 archers, 7:00 Ghost Swarm, 8:00 wraiths, 9:00 brute, 9:20 Bone Ring
  20, 11:00 knight, 12:00 Murder x2 + Grave Ring, 13:00 Ghost Swarm + Elite Pair, 14:00 Stampede (knights/brutes).
- Jefes E5.2 (avisos >= 30 f): Bone Colossus (abanico de huesos tras 30 f de destello; < 50 % derrumbe 90 f con 6
  huesos en anillo; < 25 % 5 huesos), Banshee (lamento 45 f con 4 "!" -> onda 0-96 px; < 60 % intangible 3 s de 8 y
  4 ghosts; < 30 % teletransporte al borde con marca 45 f), The Lich (calaveras dirigidas tras 30 f; < 66 % alza 4
  walkers; < 33 % campo de muerte 0.5 y teletransporte cada 6 s con humo 40 f). Lich = jefe final: Hex Moon a 3:00,
  STAGE CLEAR y resumen como en F1.
- Paletas (16): un tipo nuevo sólo entra si queda una paleta libre; la élite sólo se tiñe si quedan 2; las muertes de
  variantes usan la paleta del tipo (`test_palette_refusals`).

Pruebas: `STAGE=2` en `tests/b2run.py` (`START_AT=-1` = desde 0:00). Ver tabla de resultados en la nota de G-56.

## Provisionales

- Brotes del muro de zarzas: las mismas zarzas parpadeando (no hay arte de brote). Separación de 75 f entre lobos.

- Sonidos `audio/tmp_*.wav` (`tests/gen_tmp_sfx.py`) y gráficos `graphics/tmp_fx` (chispa de estela), `tmp_wave`
  (onda del rey) y `tmp_bossbar` (`tests/gen_tmp_gfx.py`). Todo el arte se elige en `include/hb_art.h`.
- Mirada en 8 direcciones (`art::dir_*` en `include/hb_art.h`): el mago gira 45° cada 3 frames hacia el apuntado; los
  monstruos con `walk_dirs = 5` (esqueleto; goblin/orco cuando tengan walk_s..walk_n) miran al mago. `test_wizard_dir`.
- DBG Max power también llena la Nova (para probar la Nova sin esperar).
- Título en pantalla: `hb::game_title` (`include/hb_config.h`) y el logo pintado (`art/logo.aseprite`); ROM: `ROMTITLE`.
- Ranuras de armas (`hud_slots`) sólo en la pausa, para no gastar sprites en combate.

## Carpetas

| Carpeta | Contenido |
|---|---|
| `src/`, `include/` | código C++ (Butano) |
| `graphics/` | `.bmp` indexados + `.json` que lee el build |
| `art/` | fuentes `.aseprite` editables (ver `art/LÉEME.md`) |
| `audio/` | música (`.mod/.xm/.s3m/.it`) y efectos (`.wav`) para Maxmod |
| `dmg_audio/` | música del chip de sonido de Game Boy (opcional) |
| `tests/golden/` | capturas de referencia de `gbarun.py` |

## Butano check

| Necesidad | API de Butano revisada | ¿Basta? |
|---|---|---|
| Sprite movible | `bn::sprite_ptr`, `bn::sprite_items` | sí |
| Fondo | `bn::regular_bg_ptr`, `bn::regular_bg_items` | sí |
| Mapa infinito por trozos | `bn::regular_bg_map_item` + `reload_cells_ref` (ejemplo `dynamic_regular_bg`) | sí |
| Cámara | `bn::camera_ptr` | sí |
| Contenedores sin heap | `bn::vector`, `bn::optional` (partida en heap una vez por intento) | sí |
| Destellos, transparencia, mosaico | `set_palette`, `bg_palettes::set_fade`, `bn::blending`, `bn::sprites_mosaic` | sí |
| Medir CPU | `bn::core::last_cpu_usage()` → `test_cpu_max` | sí |
| Texto | `bn::sprite_text_generator` + `common::variable_8x8_sprite_font` | sí |
| Efecto de sonido | `bn::sound_items` (Maxmod) | sí |
| Entrada | `bn::keypad` | sí |
