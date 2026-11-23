# 005_platformer — Diseño

**Frase**: HERO, un animal con actitud, corre y salta por niveles cortos, pisa a los enemigos y busca 3 secretos
antes de llegar a la meta.
**Verbo central**: saltar (y pisar al caer). Todo lo demás está a su servicio.
**Huecos por decidir (dirección artística, usuario)**: `HERO` (animal y nombre), `COIN` (recogible común),
`TOKEN` (secreto), nombres de enemigos. Los nombres de abajo son provisionales. Todo el texto del juego va en inglés.
**Original**: sólo se toma el género. Ningún personaje, objeto, nivel ni nombre de juegos existentes.

Prioridad nº 1: **sensación de control** (regla 1). El juego anterior se rechazó por el control y por verse feo:
nada de niveles hasta que la sala de pruebas (hito 0) pase las métricas de la tabla de física.

## Controles (regla 14: simples e inmediatos)
| Botón | Acción | Porqué |
|---|---|---|
| Cruceta ←/→ | andar; mantener en suelo acelera solo hasta correr | sin botón de correr (regla 14) |
| A (y también B) | saltar; soltar corta la subida; mantener al caer = planear (sólo con la mejora GLIDE) | un botón, dos usos que salen solos |
| ↓ | agacharse (caja más baja); nada más en el bloque 1 | |
| START | pausa (`PAUSE` / `RESUME` / `QUIT`) | |
**Decisión**: carrera **automática** (regla 14): con la cruceta mantenida 40 f en suelo a velocidad máxima de andar,
el tope sube a correr. B hace lo mismo que A (comodidad, sin segundo verbo). Las mejoras son pasivas: nunca piden
un botón nuevo.

## Física del jugador (bn::fixed, 60 fps, px/frame; tile = 16 px; pantalla = 15×10 tiles)
Orden por frame: entrada → vx → vy → mover X y resolver → mover Y y resolver. Cifras de pos/vel en px/f y px/f².

| Parámetro | Valor | Resultado medible | Principio |
|---|---|---|---|
| Aceleración suelo | 0.09375 (3/32) | 0→1.5 en 16 f (0.27 s) | respuesta inmediata (regla 1) |
| Velocidad andar | 1.5 | cruza la pantalla en 160 f | |
| Paso a correr | tras 40 f a 1.5 en suelo, tope 2.25 con acel. 0.03125 | 1.5→2.25 en 24 f; cruza la pantalla en 107 f (1.8 s) | recompensa al compromiso |
| Deceleración (suelta) | 0.09375 | de 1.5 para en 16 f / 12 px; de 2.25 en 24 f / 27 px | parar donde se quiere |
| Giro en seco (skid) | 0.1875 al pulsar contrario con \|vx\|>1.0, animación y polvo | de 2.25 a 0 en 12 f / 14 px | respuesta visible |
| Aire: acelerar / girar | 0.0625 / 0.125; sin frenado al soltar (0.015625) | conserva el impulso; corrige 1 tile en ~12 f | control en el aire |
| Tope horizontal en aire | el de despegue (1.5 o 2.25) | | el salto respeta la carrera |
| Velocidad inicial de salto | vy = −4.0 | | |
| Gravedad subiendo con A | 0.125 | **máx. 66 px (4.1 tiles)** en 32 f | altura variable |
| Gravedad subiendo sin A | 0.5 (corte al soltar) | **mín. 18 px (1.1 tiles)** con toque de 1 f | |
| Gravedad bajando | 0.25 | caída de 66 px en 24 f; arco total 56 f | caída firme, no flotante |
| Caída máxima | 4.5 | nunca atraviesa tiles de 16 px | |
| Distancia de salto | andar: 84 px (5.2 tiles); correr: 126 px (7.9 tiles) | | |
| Coyote | 6 f tras dejar el borde | salta aunque llegue tarde | perdón (regla 2) |
| Búfer de salto | 8 f antes de tocar suelo | | perdón (regla 2) |
| Corrección de esquina (cabeza) | si el techo solapa ≤ 4 px, desplaza 1 px/f hasta 4 px y sigue subiendo | | perdón (regla 2) |
| Ayuda de bordillo | si los pies quedan ≤ 4 px bajo el borde al chocar de lado, sube al borde | | perdón (regla 2) |
| Rebote al pisar | vy = −4.25 y sigue la regla de A: con A 74 px (4.6 tiles), sin A 20 px (1.25 tiles); A pulsado hasta 8 f antes cuenta | pisar alto es una habilidad | riesgo/recompensa |
| Caja contra el mundo | 12×22 px, centrada abajo en el sprite 32×32; agachado 12×14 | | |
| Caja de daño del jugador | 10×18 (menor que la de mundo) | | regla 2 |
| Pisotón válido | vy > 0 y pies del frame anterior por encima del centro del enemigo; zona ancha +4 px por lado | | regla 2 |

**Reglas de nivel derivadas (con margen sobre lo medido)**: camino crítico con escalones ≤ 3 tiles y huecos
≤ 4 tiles andando; 4 tiles de alto y huecos de 5–6 sólo corriendo o en opcional; 4.5 tiles = sólo con rebote alto.
Si la ROM mide otra cosa, mandan las cifras medidas y se corrigen niveles (antipatrón "números del documento").

## Cámara (regla 5, Keren 2015)
| Aspecto | Valor | Porqué / métrica |
|---|---|---|
| Horizontal | objetivo = x del jugador + adelanto; zona muerta de 16 px alrededor del objetivo | sin temblor al ajustar pasos |
| Adelanto | 48 px hacia donde mira si \|vx\| ≥ 1.0; se mueve como máx. 1 px/f (sin tirones al girar) | corriendo ve 168 px por delante = 75 f (1.25 s) de aviso |
| Vertical | sólo se reajusta **al aterrizar** a otra altura (lerp 1/8, máx. 3 px/f); pies objetivo en y = 112 | 3 tiles visibles bajo los pies: los fosos se ven |
| Vertical, excepciones | si el jugador sale de la franja y 24–128 (salto alto o caída) sigue al instante | nunca pierde de vista al jugador |
| Límites | nunca muestra fuera del nivel; se puede volver atrás | secretos hacia atrás (regla 12) |
| Aparición de enemigos | se activan al entrar a 32 px del borde de pantalla; nunca a < 8 tiles del jugador | nada nace encima (regla 5) |
Métrica: en el recorrido grabado del 1-1, cada enemigo está ≥ 60 f en pantalla antes de poder tocar al jugador.

## Daño, vidas y puntos de control (regla 10)
| Elemento | Valor | Principio |
|---|---|---|
| Vida | 3 corazones (máx. 3); un golpe quita 1 | evita dos tamaños de sprite (coste de arte) y perdona |
| Invulnerabilidad | 90 f, parpadeo cada 4 f | se ve el estado (regla 9) |
| Retroceso | vx 1.5 contra el golpe, vy −2.5, 12 f sin control | respuesta clara sin castigo largo |
| Foso | muerte inmediata; los fosos siempre visibles (cámara) y con borde marcado | regla 5 |
| Vidas | empieza con 3; vida extra con 100 COIN | aversión a la pérdida, suave |
| Continues | infinitos: `CONTINUE?` reinicia el nivel con 3 vidas | el usuario abandona lo frustrante |
| Checkpoint | 1 poste a mitad de nivel; al tocarlo se ilumina y suena; reaparece con 3 corazones | reintento < 5 s |
| Reintento | animación de muerte 45 f + fundido 30 f + reaparición = **75 f (1.25 s)** | checklist primer minuto |

## Objetos (regla 12)
| Objeto | Efecto | Lenguaje de color / aviso | Principio |
|---|---|---|---|
| COIN | +1; 100 = vida extra; sonido agudo que sube cada 5 seguidas | amarillo brillante, gira | recompensa variable, guía de camino |
| Caja de golpe (CRATE) | golpear desde abajo: COIN o mejora; queda gastada (color apagado) | madera clara con marca | curiosidad, se ve el estado |
| Fruta (heart) | +1 corazón | rojo/rosa con brillo | alivio tras un reto |
| Mejora 1: SHELL | escudo pasivo: absorbe 1 golpe y mata a lo que toca al romperse; se ve como burbuja | aura azul | riesgo/recompensa, permite tocar a SPIKY |
| Mejora 2: GLIDE (bloque 2) | mantener A cayendo: caída máx. 1.0 px/f | plumas/orejas abiertas | da un uso nuevo a A, sin botón |
| TOKEN ×3 por nivel | secreto; HUD con 3 huecos; 12 en el mundo abren el nivel extra 1-S | dorado grande, latido | exploración (reglas 7, 12) |
HUD mínimo (8×8 de Butano, fuente de `engine/butano/common/`): corazones arriba izq., `x07` vidas, COIN arriba der.,
3 huecos de TOKEN. Nada más.

## Enemigos del mundo 1 (regla 5: todos avisan antes de dañar)
| Nombre prov. | Comportamiento | Se derrota | Aviso | Principio |
|---|---|---|---|---|
| WALKER | anda 0.5 px/f, gira en paredes y bordes (variante roja cae de bordes, mundo 2) | pisar | anda a la vista, ojos hacia donde va | presentar el pisotón |
| HOPPER | quieto; cada 90 f se agacha 20 f (aviso) y salta 2 tiles en vertical | pisar (también en el aire) | agacharse 20 f + sonido | anticipación, tiempo |
| FLYER | vuela 0.75 px/f en horizontal con seno de ±16 px, periodo 120 f; ida y vuelta de 6 tiles | pisar (cae) → rebote | trayecto fijo y visible | puente para rebotes altos |
| SPIKY (desde 1-2) | anda 0.375 px/f; púas encima | **no se pisa** (hace daño); SHELL o saltar por encima | púas rojas/blancas en silueta | romper la regla aprendida (girar) |
Colores: peligro = rojo/blanco; recogibles = amarillo/dorado; fondo = tonos fríos y apagados (mismo matiz, menos
contraste). Comprobación: captura a 1x, cada clase se distingue en gris (contraste de valor).

## Mundo 1 (bosque; regla 3: presentar → desarrollar → girar → cerrar)
| Nivel | Idea | Presentar | Desarrollar | Girar | Cerrar |
|---|---|---|---|---|---|
| 1-1 Run and jump | saltar y pisar | escalón y WALKER solo | fosos + varios WALKER, HOPPER | pisar alto (A) para llegar a un TOKEN | carrera final a la meta |
| 1-2 Thorns | no todo se pisa | SPIKY solo en llano | SPIKY + WALKER juntos | SPIKY en techo bajo (ir agachado) | SHELL y arrasar |
| 1-3 Treetops | planear | GLIDE junto a un hueco ancho seguro | huecos de 8–10 tiles | corrientes de viento que suben | descenso largo con COIN |
| 1-4 Rotten logs | plataformas que caen (30 f de temblor) | una con suelo debajo | cadenas sobre foso | FLYER para rebotar a la siguiente | huida corta |
| 1-B Jefe | examina pisar y SPIKY (regla 11) | carga en línea; se pisa 3 veces | fase 2: púas tras cada golpe 60 f | fase 3: salta y suelta HOPPER | |

## Nivel 1-1 (bloque 1)
Longitud **200 tiles** (3200 px), alto 12 tiles (fila 0 arriba). Suelo en filas 10–11 (pies en y = 160 px del nivel).
Recorrido: ~24 s mínimo corriendo, ~90 s jugando normal. Cada tramo tiene una pausa segura al final (regla 4).

| Tramo (x tiles) | Contenido | Qué enseña, sin texto | Principio |
|---|---|---|---|
| A 0–19 | HERO en x 3, llano. COIN en fila a ras de suelo x 7–11. Escalón de 1 tile en x 14, de 2 en x 17 | andar recoge; no se pasa sin saltar; el 2º pide mantener A | andamiaje |
| B 20–39 | WALKER en x 34 andando hacia HERO. Arco de 5 COIN sobre él (x 30–34, altura 3). Pared de 2 tiles en x 38 | saltar sobre el enemigo: el arco dibuja la trayectoria y acaba encima | presentar en sitio seguro |
| C 40–59 | Foso de 2 tiles en x 46–47 con línea de COIN encima; escalera 1-2-3 en x 52–57 | primer foso, mucho margen (2 de 5.2 tiles) | flujo |
| D 60–89 | 3 CRATE a 4 tiles de altura en x 64–66 (la del centro da SHELL). Plataformas flotantes x 68–72 altura 4; **TOKEN 1** encima de una columna de 4 en x 74, visible desde abajo. WALKER en x 78 y x 84. HOPPER solo en x 88 | golpear con la cabeza; mirar arriba; primer ritmo | recompensa con pista |
| E 90–109 | Llano seguro, fruta en x 96, **checkpoint en x 102** | pausa (regla 4) | reintento < 5 s |
| F 110–139 | Foso de 3 en x 114–116 con WALKER al otro lado (x 121). FLYER en x 126 a altura 4 sobre llano. Línea de COIN que entra en una pared con grieta en x 134 → hueco transitable a sala de 6 tiles con **TOKEN 2** | rebotar en el FLYER; las grietas esconden | curiosidad, pista visible |
| G 140–169 | Foso de 4 en x 146–149 con 2 plataformas intermedias (camino crítico seguro). HOPPER en x 154 y x 160. WALKER patrullando una plataforma de 4 tiles en x 162–165 bajo un saliente a 4.5 tiles: **TOKEN 3** sólo con rebote alto (A) | giro: pisar con A sube más alto | riesgo/recompensa |
| H 170–200 | Carrera de 20 tiles en llano con 2 WALKER (x 176, 184), meta en x 192 y 8 tiles vacíos detrás | cierre limpio | gradiente de meta |

Meta: poste alto (GOAL) que se toca saltando; cuanto más arriba, más COIN (1/3/5). Rótulo `LEVEL CLEAR`, recuento de
COIN y TOKEN, 3 s y vuelta al título (bloque 1).
Totales: 11 enemigos (7 WALKER, 3 HOPPER, 1 FLYER), ~70 COIN, 3 TOKEN,
1 SHELL, 1 fruta, 1 checkpoint. Ningún enemigo en los 20 primeros tiles, ninguno en el borde de un foso al aterrizar.

## Primer minuto
| t | Qué pasa | Checklist |
|---|---|---|
| 0–3 s | Título `HERO` (provisional) y `PRESS START`; luego `WORLD 1-1` 1.5 s | |
| 3–10 s | HERO aparece en suelo llano, sin enemigos en pantalla; COIN a ras de suelo invitan a ir a la derecha | qué controlo en 10 s |
| 10–20 s | Escalones de 1 y 2 tiles. Si HERO lleva 4 s parado ante el escalón, aparece un icono `A` encima (una línea, regla 6) | controles en su momento |
| 20–35 s | WALKER solo, lento, con 10 tiles de llano para verlo; el arco de COIN acaba en su cabeza. Pisarlo: sonido, aplastado 20 f, +COIN | primer peligro solo; respuesta |
| 35–60 s | Primer foso con margen, CRATE, TOKEN 1 visible | |
Nada mata sin aviso: el primer foso tiene borde con hierba y COIN encima; ningún peligro fuera de cámara.
Cada acción responde: salto (sonido + polvo), aterrizaje (polvo), skid (polvo + sonido), golpe (destello + sonido).

## Bloques (alcance pequeño)
| Bloque | Contenido | Prueba de salida |
|---|---|---|
| 1, hito 0 | Sala de pruebas en greybox: física, cámara, WALKER. Métricas de la tabla con `gbarun.py` (entrada grabada) | apex 66 ± 2 px, mínimo 18 ± 2, 16 f a 1.5, skid 12 f, coyote 6 f |
| 1 (entrega) | 1-1 completo con arte: HERO 32×32 animado (idle, run, skid, jump, fall, hurt, die, cheer), tileset del bosque, WALKER/HOPPER/FLYER, COIN, CRATE, SHELL, TOKEN, checkpoint, meta, HUD, título, pausa, `GAME OVER`/`CONTINUE?`, efectos de sonido | recorrido grabado que acaba el nivel; revisión del primer minuto |
| 2 | 1-2 a 1-4, SPIKY, GLIDE, plataformas que caen, jefe 1-B, mapa de mundo simple, guardado | |
| 3+ | Mundos 2–3 y 1-S; publicar en itch.io | |
**No entra en el bloque 1**: GLIDE, SPIKY, mapa, guardado, música propia de cada nivel (una pista basta), puntuación.

## Decisiones de diseño
| Decisión | Principio | Métrica |
|---|---|---|
| Carrera automática, sin botón B de correr | regla 14 | el jugador cruza el 1-1 sin pulsar B; tiempo ≤ 90 s en la primera partida |
| Salto variable 1.1–4.1 tiles con corte por gravedad (0.5) | reglas 1 y 2 | apex medido 66 ± 2 px y 18 ± 2 px |
| Caída más pesada que la subida (0.25 vs 0.125) | regla 1 (salto firme) | arco total 56 ± 2 f |
| Coyote 6 f, búfer 8 f, esquina 4 px, bordillo 4 px | regla 2 | prueba grabada de cada caso: salta/sube |
| 3 corazones en vez de dos tamaños | regla 10 y coste de arte | un solo juego de sprites de HERO |
| Cámara con adelanto 48 px y vertical al aterrizar | regla 5 | cada enemigo ≥ 60 f en pantalla antes de contacto |
| Rebote alto con A como giro del 1-1 | regla 3 y 12 | TOKEN 3 sólo alcanzable con rebote (comprobado con bot) |
| (Usuario, 2026-11-23) El hito 0 (sala de pruebas sin arte) es **interno**: se mide con `gbarun.py` y no se entrega. La primera ROM que juega el usuario ya lleva a HERO animado y un tileset dibujado (sala de pruebas con arte o el 1-1) | el usuario abandonó DISCO VENDETTA por verse feo y por el control; la primera impresión decide | el usuario pide seguir tras la primera ROM |

## Riesgos
- **Sensación de control**: es la causa del rechazo anterior. Hito 0 obligatorio y el usuario prueba la sala de
  pruebas antes de construir el 1-1 (ROM pequeña).
- **Arte**: "se veía feo". HERO y paleta del bosque los aprueba el usuario sobre una hoja de 4 poses antes de animar.
- Nivel de 200×12 tiles mayor que un fondo de 512 px: necesita mapa grande con escritura por columnas (ya hay trabajo
  `gx` de mapas grandes; reutilizar, no reescribir).
- Caja 12×22 dentro de sprite 32×32: el dibujo debe llenar unos 20×26 px o parecerá que flota o atraviesa.
