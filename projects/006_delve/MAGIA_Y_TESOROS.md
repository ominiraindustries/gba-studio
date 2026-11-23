# DELVE — magia, tesoros y equipo (G-39)

Diseño del diseñador para completar el motor con magia, botín, equipo, consumibles e inventario. Amplía `DISENO.md`
(estadísticas §2, combate §3, monstruos §4, objetos §5, generador §6/§6b) y el HUD A de `HUD.md`; donde lo cambia, se
dice. Texto de pantalla en **inglés**, sólo con `bn::sprite_text_generator` y `common_variable_8x8`. Reglas citadas:
R1–R14 y checklists de `docs/guias/diseno-juegos.md`. Temas: `art/themes/README.md` (orden de pisos propuesto:
cloaca → mina → cripta → hielo → templo → espejos → forja; carne secreta). Las tablas van **por tema** (sesgo) y
**por planta** (rareza y aparición), así que cambiar el orden de temas no rompe nada.

Principio que manda en todo el documento: **la magia y el botín sirven al verbo central** (*esquivar y castigar*, R1).
Los conjuros abren combates, rescatan o enseñan a leer la mazmorra; el arma sigue siendo el daño principal.

## 0. Resumen de decisiones

| Decisión | Regla / principio | Métrica |
|---|---|---|
| Conjuros se aprenden **al recoger su pergamino** (sin paso de "leer"); los repetidos son lanzamientos gratis | R14, recompensa variable (ningún hallazgo es inútil) | 0 pergaminos sin usar al acabar la planta 3 en el bot |
| Maná: 10 MP al nivel 1, +2 por nivel, regenera **1 MP / 90 f** | flujo, tensión/alivio: se rellena entre encuentros (12–18 s) | tiempo medio con MP < coste del conjuro más barato: 10–30 % |
| **B = cinturón** (poción, conjuros, frascos), **SELECT = cambiar** de entrada; **no** R+A | R14, R5/R1: R+A choca con el paso lateral (ver §1.3) | 0 pasos laterales involuntarios en las rutas de prueba |
| Daño de conjuros ignora DEF; jefes reciben ×0,5 | R11 (el jefe examina la esquiva, no el maná) | % de daño al jefe con magia ≤ 35 % |
| Cofres: normal, con llave, con trampa, mimic; **todo con aviso** (tell + 24–30 f de anticipación) | R5, riesgo/recompensa | 0 daños de cofre sin aviso visible el frame anterior |
| 3 rarezas: `COMMON` gris, `MAGIC` azul, `RARE` dorado | lenguaje de color, recompensa variable | % de objetos por rareza dentro de ±5 puntos de la tabla en 100 semillas |
| Oro **sí**, gastado en el **Altar of Coin** (sin PNJ) y fuentes; **no** va en el HUD | gradiente de meta, R14 (sólo se mira cuando aplica) | % del oro gastado al acabar la planta 3 ≥ 50 % |
| Sin identificación; sin malditos que se pegan. Objetos de doble filo **a la vista** (Blood) | R14, aversión a la pérdida | — |
| Inventario con START (pausa) y pestañas `BAG` / `SPELLS` / `MAP` (SELECT cambia de pestaña) | R14 | abrir, equipar y cerrar en < 5 s en la ruta M1-b |

**Cambios a `DISENO.md` que trae este documento** (el puntero lo dice; los demás agentes los aplican al tocar el
código): SELECT en mazmorra pasa de "mapa" a "cambiar cinturón" (el mapa pasa a la pestaña `MAP` del inventario;
el minimapa del HUD A sigue siempre visible); B pasa de "objeto rápido" a "cinturón"; Scroll of Fire / Scroll of
Sight pasan a ser los pergaminos que enseñan Fireball / Reveal; el equipo gana anillo y amuleto; "tiendas" deja de
estar en "No entra" en forma de altar sin PNJ. El menú de depuración (SELECT+L) se muda a la pausa (L+R+SELECT)
para no chocar con el cinturón.

## 1. Magia

### 1.1 Cómo se aprende
- **Pergamino = conjuro.** Cada conjuro tiene su pergamino (`Scroll of Magic Missile`…). Se recoge pisándolo, como
  todo (R14). Si el conjuro no se conocía: `LEARNED: MAGIC MISSILE` (22 car.), destello azul del hueco B, el conjuro
  entra en el cinturón y queda **seleccionado** (se enseña en el momento de usarlo, R6) con la línea
  `SELECT: SWITCH   B: CAST` (24 car.) la primera vez de la partida.
- Pergamino de un conjuro ya sabido: va a la bolsa (pila 9) y es un **lanzamiento gratis** (0 MP, mismo tiempo de
  lanzamiento). Se usa desde el cinturón o la bolsa. Así un repetido nunca es basura (recompensa variable).
- **Comprar**: el Altar of Coin (§2.4) ofrece a veces un pergamino del nivel de la planta.
- **Garantía de avance** (gradiente de meta): cada planta trae **1 pergamino garantizado** de un conjuro no sabido de
  su nivel (tabla §1.4) en la zona media, dentro de un cofre normal (nunca mimic ni trampa). El resto, por tabla.
- Los cultists (planta 2–3, `DISENO` §4: 40 % pergamino) sueltan pergaminos del nivel de su planta.

### 1.2 Maná
| Dato | Valor | Porqué |
|---|---|---|
| MP máx. | **10** al nivel 1, **+2 por nivel** (nivel 3: 14, 5: 18, 7: 22, 10: 28) | 1–3 conjuros por combate: decisión, no metralleta |
| Regeneración | **1 MP cada 90 f** (0,67 MP/s), siempre, también en combate | entre encuentros (12–18 s, `DISENO` §6) recuperas 8–12 MP: alivio tras la tensión |
| Subir de nivel | rellena el MP (como el HP) | alivio |
| Mana Potion | +60 % del MP máx. (planta 2+) | — |
| Antes del 1.er conjuro | barra vacía y `MP` en gris; al aprender, se llena en 30 f | la barra vacía es una promesa (gradiente de meta) sin ruido (R14) |

### 1.3 Controles (R14: un botón, un significado)
| Botón | En la mazmorra | Por qué |
|---|---|---|
| **B** | usa la entrada seleccionada del **cinturón**: bebe la poción, lanza el conjuro, tira el frasco | DISENO ya enseña B como "lo de la otra mano" (EOB: dos manos, A = arma, B = la otra) |
| **SELECT** | pasa a la siguiente entrada del cinturón (cíclico); el nombre sale en la franja 60 f (`HEAL  6 MP`) | cambiar no es urgente: un botón aparte, sin acordes |
| A, L/R, cruceta | sin cambios | el verbo central no se toca (R1) |
| START | inventario (pausa) | — |

- **R+A descartado**: R hace el paso lateral **al pulsar** (es la esquiva contra los conjuros enemigos, `DISENO` §3).
  Para leer R+A habría que retrasar el paso hasta saber si viene A (≥ 6 f), y eso rompe la esquiva (R1, R5), o lanzar
  siempre después de dar el paso. Tampoco A+B (pulsación simultánea imprecisa) ni "mantener B" (B tardaría en beber).
- **Cinturón**: 4 entradas, la 1.ª siempre la poción curativa (aunque haya 0: icono gris). Se rellena solo al
  aprender (si está lleno, el nuevo conjuro sustituye a la entrada seleccionada que no sea la poción) y se ordena en
  la pestaña `SPELLS`/`BAG` con R. En el hueco B del HUD: icono de la entrada + `x3` (objeto) o el coste en azul
  (`6`, conjuro); 4 puntos debajo dicen cuál de las 4 es.
- **Lanzar**: con MP suficiente empieza el **tiempo de lanzamiento** (tabla): el hueco B late en azul, no se puede
  atacar ni usar otra cosa (como beber, `DISENO` §5), **sí moverse** (la esquiva nunca se bloquea, R1); el efecto
  sale al final en la dirección que miras entonces. La barra del arma sigue cargando. Recibir un golpe no
  interrumpe (R14). Sin MP: `NOT ENOUGH MANA`, golpe sordo, nada se gasta ni se bloquea.
- Respuesta de cada lanzamiento (checklist "cada acción tiene respuesta"): borde inferior de la vista en el color del
  conjuro 6 f, sonido propio, número de daño sobre el monstruo, MP que baja.

### 1.4 Conjuros
Nivel del jugador = L. Daño **ignora DEF**; jefes ×0,5. Tiempos a 60 fps. "Planta" = primera planta donde puede
salir su pergamino (y la del garantizado). Velocidad de proyectiles en casillas/frame igual que la bola de fuego del
cultist (1 casilla / 8 f) o más rápida, para que el jugador vea su propio disparo (R5 al revés: respuesta legible).

| # | Conjuro | MP | Lanz. | Efecto y números | Aviso / imagen | Planta | Principio |
|---|---|---|---|---|---|---|---|
| 1 | **Magic Missile** | 3 | 12 f | dardo al primer monstruo en línea recta, d ≤ 4, **6 + (L−1)** de daño; avanza 1 casilla / 4 f. Destruye una bola de fuego enemiga que encuentre (las dos desaparecen) | punta de luz azul blanca 16×16 que crece al salir; chispa al impactar | 1 | abre el combate a distancia; a L1 mata una rata (6 HP) de un golpe: respuesta clara |
| 2 | **Light** | 4 | 12 f | **45 s** (2700 f): halo de antorcha 100/100/90 % (en vez de 100/85/65) y brillo de la vista +12 %; las **grietas de SECRET brillan** a d ≤ 3; los avisos de cofres (trampa, mimic) se ven a d ≤ 3 en vez de 2 | destello blanco cálido 8 f; icono de estado; últimos 3 s parpadea | 1 | atmósfera al servicio de leer peligros y secretos (R5, R12) |
| 3 | **Heal** | 6 | 24 f | +**30 %** del HP máx. al instante (L1: 9; L5: 16) | chispas verdes que suben por los bordes de la vista 20 f; número verde sobre el HP | 1 | rescate más lento y caro que la poción (50 %, 20 f): la poción sigue valiendo |
| 4 | **Frost** | 5 | 18 f | rayo al primer monstruo d ≤ 3, **4 + (L−1)** de daño y **lentitud 6 s** (360 f): su movimiento, aviso y recarga ×1,5. Slime: congelado del todo (no se mueve ni ataca) 4 s | rayo cian; el monstruo toma un tinte azul y escarcha (paleta); al acabar, se rompe la escarcha | 2 | alarga los avisos: hace más fácil el verbo central, no lo sustituye |
| 5 | **Fireball** | 8 | 30 f | bola al primer monstruo o pared, d ≤ 5, 1 casilla / 6 f; estalla: **12 + 2(L−1)** en su casilla y la mitad en las 4 vecinas. No daña al jugador. Detona runas explosivas (`DISENO` §6b) a distancia | esfera naranja con estela; estallido 3 frames 32×32; las vecinas se iluminan 6 f | 2 | riesgo/recompensa: caro y lento; premia agrupar monstruos |
| 6 | **Shield** | 6 | 18 f | **+3 DEF 20 s** (1200 f). No sirve contra trampas (ignoran DEF) | marco azul de 1 px alrededor de la vista que parpadea al bloquear daño; icono de estado | 3 | aguantar al jefe; enseña que DEF resta por golpe |
| 7 | **Reveal** | 10 | 36 f | revela la planta en el mapa y el minimapa: paredes, escalera, cofres y **muros secretos** (grieta dibujada) | ola azul que se abre desde el centro de la vista 24 f; el minimapa se pinta en 30 f | 3 | R12: lo opcional se hace visible para quien quiere explorar |
| 8 | **Blink** | 7 | 6 f | salto hasta **3 casillas al frente** por aristas abiertas (para ante pared, puerta cerrada, reja o monstruo); 20 f de invulnerabilidad; no pisa trampas que salta | la vista destella blanco 4 f y aparece ya en la casilla nueva | 4 | esquiva de emergencia; nunca a ciegas (sólo hacia donde miras, R5) |
| 9 | **Haste** | 8 | 18 f | **15 s** (900 f): paso 12 → 8 f, giro 8 → 6 f, recarga del arma ×0,75 | estelas en los bordes laterales de la vista; icono de estado | 5 | flujo: acelera el bucle de esquivar y castigar |
| 10 | **Unlock** | 10 | 30 f | abre la puerta o el cofre con llave que tienes delante (no rejas: esas son de palanca, `DISENO` §6b R7) | cerradura que gira con chispas doradas | 6 | da otra salida a quien no tiene llave sin romper "cada llave abre muchas cerraduras" (R7): cuesta casi todo el maná |

- Plantas 1–3 (demo): 7 conjuros posibles; se espera saber **5–6** al llegar al jefe.
- Máximo **1 proyectil del jugador** a la vez (lo garantiza el tiempo de lanzamiento + vuelo ≤ 30 f): 1 sprite, 1 KB
  de VRAM de sprites como mucho.

### 1.5 Conjuros y temas
Regla: **cada tema toca 1–2 conjuros**, con multiplicadores simples (×0,5 / ×1,5), y lo dice **una vez** en la
franja la primera vez que pasa (R6). La cloaca es la regla base (tema de aprender).

| Tema | Oscuridad (propuesta de halo/brillo) | Conjuros afectados | Mensaje la 1.ª vez | Principio |
|---|---|---|---|---|
| Cloaca | base | ninguno | — | andamiaje |
| Mina entibada | más oscura (halo 100/75/55) | Fireball **quema** los muros secretos entablados (alternativa a andar contra la grieta; los dos valen) | `THE BOARDS BURN!` | Light vale más; R12 |
| Cripta | base | Fireball y Light: **×1,5** contra no muertos (skeleton, Grave Warden); Light hace que los no muertos a d ≤ 2 avisen 25 % más | `THE DEAD FEAR LIGHT` | R11: herramienta para el jefe de la cripta |
| Caverna de hielo | más clara (halo 100/90/75) | Fireball ×1,5 y **derrite** las láminas de hielo de los muros secretos; Frost ×0,5 (lentitud 3 s) | `THE ICE MELTS!` | giro (R3) sobre lo aprendido |
| Templo de raíces | base | Fireball ×0,75 (húmedo); Fireball quema raíces de muros secretos | `ROOTS CATCH FIRE` | — |
| Sala de espejos | más oscura (halo 100/75/55) | todos los conjuros **−1 MP** (mín. 1); Light dura ×0,5 (los espejos se la beben) | `MIRRORS FEED MAGIC` | riesgo/recompensa |
| Forja de lava | más clara (la lava alumbra) | Fireball ×0,5; Frost ×1,5 (daño y lentitud) | `FIRE IS WEAK HERE` | giro: el conjuro favorito deja de valer |
| Madriguera de carne (secreta) | media | Heal ×0,5; regeneración de MP ×2 | `THE FLESH HUMS` | riesgo/recompensa de la planta secreta |

Los multiplicadores son datos de la tabla del tema (`theme_spell_mod`), no código especial: 8 temas × 10 conjuros
× (daño, duración, coste) = 240 bytes.

## 2. Tesoro y botín

### 2.1 Cofres
Se abren **andando contra ellos** (`DISENO` §6b, R14). Bloquean paso; sólo en salas y callejones (reglas duras de
§6b). El botín cae en **tu casilla** y se recoge solo. Un cofre cuenta dentro de los objetos de la planta (§6), no
los suma.

| Cofre | Se ve (tell) | Al abrir | Contenido | Desde | Principio |
|---|---|---|---|---|---|
| **Chest** (normal) | madera con herrajes; tapa cerrada | se abre en 12 f, brillo del color de la rareza | 1 tirada de la tabla de la planta | 1 | R12 |
| **Locked Chest** | bandas de hierro y **candado** grande (el mismo de `tex_door_locked`, R7: un símbolo) | con Iron Key: se gasta, 12 f; sin llave: `LOCKED. FIND A KEY.` | 1 tirada **mínimo MAGIC** + 1 pila de oro | 1 | R7 (cerradura antes que llave), riesgo/recompensa |
| **Trapped Chest** | como el normal + **destello rojo en la cerradura** cada 60 f, visible a d ≤ 2 (d ≤ 3 con Light) | silbido y tapa que tiembla **24 f**, luego nube de agujas sobre la casilla de delante: **6 + 2·planta**, ignora DEF. Si te apartas (atrás o de lado, 12 f), fallo. El botín sale igual tras la nube | 1 tirada, +1 punto de rareza (tabla §2.2) | 2 | R5, verbo central: abrir = esquivar |
| **Mimic** | igual que un cofre normal pero **respira**: la tapa se abre 1 px y se cierra cada 120 f (visible a d ≤ 2, ≤ 3 con Light); además, un golpe con A a un cofre real suena `CLANG` y no pasa nada, a un mimic lo despierta | al tocarlo despierta: **30 f** de aviso (dientes, parpadeo blanco) antes del primer mordisco: retroceder lo esquiva y deja el contraataque | botín **RARE** garantizado + 2 pilas de oro | 3 | R5, riesgo/recompensa, recompensa variable |

Mimic como monstruo (`DISENO` §4): HP **30**, DEF 1, daño **8**, no se mueve, aviso 30 f, recarga 80 f, XP 20.
Reglas duras: nunca el primer cofre de la planta, nunca el cofre garantizado del pergamino, nunca en callejón (siempre
hay a dónde retroceder), máx. 1 por planta.

| Planta | Cofres | Con llave | Con trampa (de los normales) | Mimic |
|---|---|---|---|---|
| 1 | 2–3 | 1 | 0 | 0 |
| 2 | 3 | 1 | 20 % | 0 |
| 3 | 3–4 | 1 | 20 % | 15 % de un cofre (máx. 1) |
| 4+ | 4 | 1–2 | 25 % | 20 % (máx. 1) |

- **Llaves**: los cofres con llave cuentan como cerraduras en la regla "llaves ≥ cerraduras" de `DISENO` §6
  (`lock_free = 1` deja la planta 1 sin puertas con llave, pero su cofre con llave sí lleva su llave: la llave
  siempre más cerca de la entrada que el cofre, R7). Nunca en el camino crítico (no lo bloquean: son cofres).

### 2.2 Rareza y tablas de botín
**Rareza** (color del nombre en la franja y del brillo al aparecer; lenguaje de color):

| Planta | COMMON (gris) | MAGIC (azul) | RARE (dorado) |
|---|---|---|---|
| 1 | 85 | 15 | 0 |
| 2 | 70 | 27 | 3 |
| 3 | 60 | 32 | 8 |
| 4–5 | 50 | 38 | 12 |
| 6+ | 40 | 42 | 18 |

"+1 punto de rareza" = sube un escalón (COMMON → MAGIC → RARE). Amulet of Fortune: +5 a RARE (se quita de COMMON).

**Categoría** (cofre normal; los barriles y cajas siguen con su tabla de `DISENO` §6b):

| Categoría | Peso base | Qué es |
|---|---|---|
| Consumible | 45 | pan, pociones, frasco, pergamino repetido |
| Oro | 20 | 1 pila (×2 en cofre con llave: va aparte) |
| Equipo | 25 | arma, armadura, anillo, amuleto |
| Pergamino nuevo | 10 | conjuro no sabido del nivel de la planta (si no queda ninguno: consumible) |

**Sesgo por tema** (multiplica el peso; identidad, R8):

| Tema | Sesgo | Lectura |
|---|---|---|
| Cloaca | consumible ×1,5 | pociones y pan para aprender |
| Mina | oro ×2, Fire Flask ×2 | mineros |
| Cripta | pergamino ×2, afijo Holy ×3 | biblioteca de los muertos |
| Hielo | Mana Potion ×2, afijo Frost ×3 | — |
| Templo | anillo/amuleto ×2 | ofrendas |
| Espejos | pergamino ×1,5, Ring of Mana ×2 | — |
| Forja | arma/armadura ×2, afijo Keen ×2 | herrería |
| Carne | RARE +10 puntos, objetos Blood ×3 | la planta secreta paga el riesgo |

**Qué hay en cada rareza** (planta mínima entre paréntesis; nada sale antes de su planta):

| Rareza | Equipo | Consumible |
|---|---|---|
| COMMON | Short Sword (1), Leather (1), Mace (2), Chain Mail (2) | Bread, Healing Potion, Mana Potion (2), Fire Flask (2), pergamino repetido |
| MAGIC | arma o armadura **+1**; Mage Robe (2); anillos (todos, §3.3); Amulet of Life, Amulet of Fortune | Healing Potion ×2 |
| RARE | arma **+2 con afijo**; Battle Axe (3), Plate (3); anillos "Greater"; Amulet of Warding, Amulet of Second Wind; Blood (carne o altar) | — |

### 2.3 Oro
- **Sí sirve**: se gasta en el **Altar of Coin** (§2.4) y en las fuentes turbias (§2.6). Al final de la demo, el oro
  total sale en la pantalla `THE DEPTHS AWAIT` (puntuación).
- Pila de oro: **rnd(4..8) + 4·planta** (planta 1: 8–12; 3: 16–20). Se recoge al pisar: `+12 GOLD` en la franja.
- Pilas por planta: **3 + planta** sueltas (zona media y lejos), más las de cofres. Oro esperado: planta 1 ≈ 55,
  planta 2 ≈ 90, planta 3 ≈ 130.
- **No va en el HUD** (R14: prioridad 3, sólo importa en el altar): se ve en la pausa y en el altar.

### 2.4 Altar of Coin (tienda sin PNJ)
Un altar de piedra con un cuenco (mueble FURN nuevo, bloquea, sólo en salas, zona media). **1 por planta desde la
2**. Andar contra él abre su pantalla (pausa): **3 ofertas** fijadas por la semilla (R13), siempre una Healing
Potion. A compra, B sale, cruceta elige.

| Oferta | Precio |
|---|---|
| Bread | 10 |
| Healing Potion / Mana Potion | 30 |
| Fire Flask | 35 |
| Iron Key | 40 |
| Pergamino (no sabido, del nivel de la planta) | 75 |
| Anillo / amuleto MAGIC | 120 |
| Objeto RARE (planta 4+) | 250 |

Con ≈ 55 de oro al llegar a la planta 2 se compra 1 cosa; con lo de la 2, 2–3 en la 3 (gradiente de meta, decisión
real: poción segura o pergamino).

### 2.5 Recompensa de los muros secretos (R12)
Detrás de cada SECRET (grieta, `DISENO` §6b) que lleva a un callejón o sala sin otra entrada, un **alijo**:
- 1 tirada **mínimo MAGIC** de la tabla de la planta, **o** (30 %) el pergamino de un conjuro de la planta
  **siguiente** (adelanto: gradiente de meta);
- + 1 pila de oro ×2.
- Máx. **2 alijos por planta**; el resto de SECRET de la v2 (si hay más) llevan sólo oro. Se medirá cuántas pone la v2
  en 100 semillas antes de fijar el tope.
- Light (grieta que brilla) y Reveal (grieta en el mapa) son las ayudas; andar contra la grieta sigue bastando.

### 2.6 Fuentes y altares de apuesta
Todas: andar contra ella → una línea de pregunta (`DRINK?  A: YES  B: NO`, 20 car.); cualquier otro botón o paso
cancela (aversión a la pérdida: nunca por accidente). **Ninguna mata**: lo peor deja ≥ 1 HP (R10).

| Prop | Se ve | Efecto | Usos | Desde | Principio |
|---|---|---|---|---|---|
| **Clear Fountain** | pila con agua azul clara que brilla | 60 %: HP y MP enteros · 30 %: +3 HP máx. o +2 MP máx. para siempre · 10 %: −25 % del HP actual | 1, luego se seca (gris) | 2 | recompensa variable con apuesta suave |
| **Murky Fountain** | agua verde turbia, burbujas | pide **20 de oro** (`TOSS 20 GOLD?`): 30 % objeto MAGIC · 30 % +6 HP máx. o +4 MP máx. · 40 % −25 % del HP actual | 1 | 3 | riesgo/recompensa leído por color |
| **Blood Altar** | altar oscuro con cuenco rojo que late | pide **40 % del HP actual** (`GIVE BLOOD?`): objeto **RARE** seguro de la tabla de la planta (o Blood) | 1 | 4 (y siempre 1 en la carne) | apuesta en qué, no en si: el coste se ve |

Máx. 1 fuente por planta (50 % clara / 50 % turbia desde la 3) y 1 altar de sangre cada 2 plantas. Nunca a < 6
casillas de la entrada ni en la sala previa al jefe.

## 3. Equipo

**4 huecos**: arma, armadura, anillo, amuleto (uno de cada). Lo mejor se equipa solo al recogerlo si es
estrictamente mejor en el mismo dato (daño o DEF, `DISENO` §5); anillos y amuletos **nunca** se equipan solos si el
hueco está ocupado (no son comparables: decide el jugador en la pausa).

### 3.1 Armas (la barra de recarga es el ritmo)
STR inicial 2. Daño por golpe = arma + STR − DEF del monstruo (mín. 1).

| Arma | Daño | Recarga | Daño/s con STR 2 (contra DEF 1) | Planta | Para quién |
|---|---|---|---|---|---|
| Dagger | 3 | 30 f | 10,0 (8,0) | inicial | perdona: recarga corta, más ventanas de contraataque |
| Short Sword | 5 | 40 f | 10,5 (9,0) | 1 | primera mejora pronto |
| Mace | 8 | 56 f, ignora 1 DEF | 10,7 (10,7) | 2 | contra esqueletos |
| Battle Axe | 11 | 72 f | 10,8 (10,0) | 3 | riesgo/recompensa: el contraataque crítico ×2 vale 26 |

El daño/s es casi igual a propósito: **la diferencia es el ritmo** (cuántas ventanas de esquiva pierdes) y el
crítico de contraataque (×2 sobre un golpe grande), no la cifra (R1). Con STR alta ganan las rápidas (STR se suma
por golpe): el arma lenta compensa con DEF de los monstruos de abajo.

**Afijos** (sólo uno por arma; RARE = +2 y afijo; MAGIC = +1 sin afijo). Nombre ≤ 20 caracteres:
`Keen Short Sword +2`.

| Afijo | Efecto | Principio |
|---|---|---|
| Keen | recarga −15 % | flujo |
| Frost | cada golpe: lentitud 2 s (120 f) como Frost | alarga avisos |
| Holy | ×1,5 contra no muertos | R11 |
| Draining | +1 MP por golpe que acierta | une arma y magia |

### 3.2 Armaduras
| Armadura | DEF | Extra | Planta |
|---|---|---|---|
| Leather | +1 | — | 1 |
| Mage Robe | +1 | **+6 MP máx.**, regeneración 1 MP / 72 f | 2 |
| Chain Mail | +3 | — | 2 |
| Plate | +5 | — | 3 (sala cerrada o RARE) |

Mage Robe frente a Chain Mail es la primera decisión de "estilo" (R12: lo opcional regula la dificultad). Sin
penalización de peso (R14). MAGIC = +1 DEF.

### 3.3 Anillos y amuletos (efecto de una línea)
| Objeto | Efecto | "Greater" (RARE) | Planta |
|---|---|---|---|
| **Ring of Mana** | +5 MP máx., regeneración 1 MP / 72 f | +10, 1 / 60 f | 1 (M1) |
| Ring of Regeneration | +1 HP cada 240 f | cada 150 f | 2 |
| Ring of Protection | +1 DEF | +2 | 2 |
| Ring of Swiftness | recarga del arma −10 % | −20 % | 3 |
| Ring of Light | Light permanente a la mitad (halo 100/92/78) | Light permanente completo | 3 |
| Amulet of Life | +10 HP máx. | — | 2 |
| Amulet of Fortune | oro +50 %, RARE +5 puntos | — | 2 |
| Amulet of Warding | ataques laterales/traseros avisan ×2 en vez de ×1,5 | — | 3 (RARE) |
| Amulet of Second Wind | 1 vez por planta, un golpe mortal te deja a 1 HP + 60 f invulnerable (icono de estado encendido mientras está cargado) | — | 3 (RARE) |

**Blood** (M3, sólo carne y Blood Altar; nombre en rojo, el coste dicho en la franja al verlo):
- `Blood Ring`: +3 STR; pociones, pan y Heal curan la mitad.
- `Blood Amulet`: si no te llega el MP, el conjuro se paga con HP (2 HP por MP, nunca por debajo de 1).

### 3.4 Identificación y malditos
- **Identificación: no.** Todo se ve al recogerlo: nombre, rareza y efecto en la franja. Identificar es una gestión
  sin decisión que choca con R14 y con la pantalla pequeña.
- **Malditos: no.** Un objeto que no se puede quitar castiga sin aviso (R5) y sin decisión (aversión a la pérdida mal
  usada). El sabor "maldito" lo dan los objetos **Blood**: el coste está a la vista y se quitan cuando quieras.

## 4. Consumibles
| Consumible | Efecto | Uso | Pila | Planta |
|---|---|---|---|---|
| Bread | +10 HP | B / bolsa, 20 f | 9 | 1 |
| Healing Potion | +50 % HP máx. | B (1.ª entrada fija del cinturón), 20 f | 9 | 1 |
| Mana Potion | +60 % MP máx. | B / bolsa, 20 f | 9 | 2 |
| Scroll of *X* | sin saber: aprende *X*; sabido: lanza *X* gratis | recoger / B / bolsa | 9 por tipo | según §1.4 |
| Iron Key | abre una puerta o cofre con llave de la planta; al bajar se pierde (`DISENO` §5) | andando | 9 | 1 |
| **Fire Flask** | tirado con B: 18 f de preparación, vuela hasta el primer monstruo o pared d ≤ 3; **12 + planta** en su casilla y la mitad en las 4 vecinas; detona runas; quema muros entablados/raíces como Fireball | B | 5 | 2 |

Sin hambre (`DISENO` §5). Sin flechas ni munición (R14). El pan sigue siendo curación barata.

## 5. Interfaz

### 5.1 Pantalla de inventario: sí (START, pausa)
Hace falta: 4 huecos de equipo, 12 de bolsa, conjuros y el mapa no caben en el HUD (R14: no se lee de reojo lo que
se decide en calma). Estilo HUD A: cajas oscuras con borde de piedra y acero, texto blanco, la franja inferior **no
cambia** (HP, MP, cinturón, mensajes siguen a la vista y la línea de mensajes describe el objeto del cursor).

| Zona (pausa) | Rect (x, y, w, h) | Contenido |
|---|---|---|
| Pestañas | (0, 0, 176, 12) | `BAG` en (6, 2), `SPELLS` en (44, 2), `MAP` en (100, 2); la activa en dorado, las otras en gris; SELECT pasa a la siguiente |
| **BAG**: rejilla 4×3 | (24, 16, 128, 96) | huecos de 32×32 con icono 16×16 centrado; contador `x3` abajo a la derecha; marca dorada `B` de 5×5 arriba a la derecha si está en el cinturón; cursor = marco dorado de 2 px |
| BAG: línea de ayuda | (0, 112, 176, 8) | vacía (la ayuda va en la franja la 1.ª vez: `A: USE  R: BELT  L: DROP`, 23 car.) |
| **SPELLS**: rejilla 5×2 | (8, 18, 160, 64) | 10 huecos 32×32 (icono 16×16); no sabidos: `?` gris (se ve cuántos faltan, gradiente de meta) |
| SPELLS: detalle | (8, 88, 160, 24) | línea 1 `MP 3   RANGE 4` ; línea 2 `6 DAMAGE` (texto blanco, coste en azul) |
| **MAP** | (40, 12, 96, 96) | planta 32×32 a 3 px (`DISENO` §5) |
| Equipo 2×2 | (180, 4, 56, 56) | arma (180, 4, 27, 27), armadura (209, 4, 27, 27), anillo (180, 33, 27, 27), amuleto (209, 33, 27, 27); vacío = silueta gris del tipo |
| Estado | (180, 64, 56, 54) | 5 líneas a 10 px desde y = 66: `LV 3`, `NEXT 20`, `STR 4`, `DEF 3`, `GOLD 120` |
| Franja (sin cambio) | (42, 147, 196, 12) | nombre del objeto del cursor en el color de su rareza + efecto: `Ring of Mana  +5 MP` |

Controles en la pausa: cruceta mueve el cursor (de la columna 4 de la bolsa a la derecha pasa al equipo); **A** usa o
equipa (en el equipo: quitar a la bolsa); **R** pone o quita del cinturón; **L** tira (pide `L AGAIN TO DROP` en
30 f: aversión a la pérdida); **SELECT** cambia de pestaña; **B o START** cierra. Coste estimado de VRAM: 16 iconos
16×16 de 16 colores = 2 KB de sprites con los monstruos ocultos (sobra en los 16 KB del modo 4).

La pantalla del **Altar of Coin** reutiliza la misma plantilla: 3 huecos 32×32 en (40, 40), (72, 40), (104, 40) y el
precio debajo; `GOLD 95` en el panel de estado; franja con nombre y precio.

### 5.2 Cambios en el HUD A (en la mazmorra)
Ninguna caja de `HUD.md` cambia de rect; cambian sus contenidos y el `ui_frame` (dibujo del canal del MP y su
etiqueta).

| Elemento | Caja de HUD A | Rect nuevo | Detalle |
|---|---|---|---|
| Barra de HP | Vida (42, 122, 196, 24) | canal (60, 127, 96, 8), como ahora | `HP` en (46, 127), número en (160, 127) |
| **Barra de MP** | misma caja | canal **(60, 137, 96, 7)**, interior (61, 138)–(154, 142) = 5 px (mínimo de `HUD.md`) | azul (lenguaje de color de `HUD.md`); `MP` en (46, 137); número `12/18` en (160, 137); hueco libre y = 135–136 entre barras; la caja acaba en y = 145 |
| **Hueco B (cinturón)** | objeto rápido (198, 124, 38, 20) | el mismo | icono 16×16 en (200, 126); `x3` (objeto) o coste en azul (conjuro) en (218, 127); `B` dorado en (220, 135) como ahora; sin MP suficiente: icono gris y coste en rojo; 4 puntos de 2×2 en (202 + 4·i, 142), el seleccionado en blanco; durante el lanzamiento el marco late en azul |
| **Estados** | Llaves/estados (178, 94, 60, 24) | llave (181, 97, 16, 16) + `x1` en (198, 101) como ahora; 4 iconos 8×8 en **(214, 97), (224, 97), (214, 107), (224, 107)** | Light, Shield, Haste, Second Wind; parpadean (8 f) los últimos 180 f |
| Oro | — | no se muestra | §2.3 |

Presupuesto añadido: texto `MP` + número (≈ 3 sprites), 4 iconos de estado (4), puntos del cinturón pintados en el
bitmap (0 sprites). Total del HUD en el peor caso ≈ 31 sprites (el límite de `HUD.md` era 24: se pide subirlo a
**32**; si no, los iconos de estado van pintados en el bitmap como la llave).

## 6. Bloques de implementación
Cada bloque es jugable al acabar; pruebas con `gbarun.py --input` (entrada grabada) y `--shot`, capturas revisadas
con `Read`, semilla fija. Las rutas dependen de una **planta de prueba** fija (como `pf_level_test_room` de otro
proyecto) o de una opción de depuración "empezar con …" en el menú de depuración: la decide el motor.

### M1 — porción mínima jugable
- MP (10, +2/nivel, 1/90 f), barra de MP y etiqueta en el HUD.
- Cinturón (B usa, SELECT cambia) con poción + conjuros; pergaminos que enseñan o se lanzan gratis.
- **3 conjuros**: Magic Missile, Light, Heal (ataque, atmósfera, rescate: uno de cada papel).
- **Cofres**: normal y con llave (con su llave según §2.1). Sin trampa ni mimic todavía.
- **1 anillo**: Ring of Mana. Huecos de anillo y amuleto visibles en la pausa (el de amuleto vacío).
- **Inventario** START con pestañas `BAG` y `SPELLS` (`MAP` si el mapa ya existe; si no, M2).
- Planta 1: 3 pergaminos (Missile garantizado en el 1.er tercio, Light y Heal por tabla con garantía de 1), 2–3
  cofres, el Ring of Mana como objeto MAGIC posible (en la planta de prueba, fijo).
- Rótulos: `LEARNED: ...`, `SELECT: SWITCH   B: CAST`, `NOT ENOUGH MANA`, `LOCKED. FIND A KEY.`.

| Prueba M1 | Ruta grabada | Se acepta si |
|---|---|---|
| M1-a Misil | recoger el pergamino → SELECT → B frente a una rata a d = 3 | captura del dardo en vuelo; la rata muere de 1 misil a L1; MP 10 → 7 en el HUD; a los 270 f vuelve a 10 (±1 f por punto) |
| M1-b Cofres e inventario | abrir cofre normal → coger llave → abrir cofre con llave → START → equipar Ring of Mana → cerrar | captura de cofre cerrado/abierto; MP máx. 10 → 15 en el HUD; pausa → equipar → cerrar en < 5 s de entrada grabada |
| M1-c Light y Heal | capturas del mismo pasillo antes / después de Light; recibir 2 golpes → Heal | la segunda captura más clara a d = 2–3 (brillo medio de la vista ≥ +10 %); HP sube 30 % del máx.; icono de estado visible y parpadeo final |
| M1-d Sin maná / sin bloqueo | B con MP 2 → `NOT ENOUGH MANA`; durante un lanzamiento, R | ningún frame bloqueado: el paso lateral sale en el frame siguiente a R durante el lanzamiento; A no ataca durante el lanzamiento |
| M1-e Coste | combate con misil en vuelo + 3 monstruos en vista | HUD ≤ 0,1 f; sprites del HUD ≤ 32; 0 parpadeos |

### M2 — botín completo y magia de la demo
- Conjuros Frost, Fireball, Shield, Reveal (plantas 2–3) y multiplicadores de tema (§1.5) cuando el motor cargue temas.
- Cofre con trampa y mimic; rarezas, afijos, tablas por planta y tema; oro, Altar of Coin, fuentes clara y turbia;
  alijos de muros secretos.
- Resto de anillos, amuletos Life/Fortune/Warding/Second Wind, Mage Robe, Mana Potion, Fire Flask; pestaña `MAP`.

| Prueba M2 | Ruta | Se acepta si |
|---|---|---|
| M2-a Trampa | abrir cofre con trampa y retroceder; otra vez quedándose | 1.ª: 0 daño y botín; 2.ª: daño 6 + 2·planta; aviso visible en la captura 1 f antes |
| M2-b Mimic | golpear con A un cofre real (`CLANG`) y un mimic; retroceder al aviso; contraatacar | captura de la respiración a d = 2; 0 mordiscos sin los 30 f de aviso; crítico tras esquivar |
| M2-c Fireball | dos esqueletos en casillas vecinas | los dos dañados (centro y mitad); runa detonada a distancia |
| M2-d Altar | llegar a la planta 2 con oro de la 1 y comprar | oro descontado; objeto en la bolsa; ofertas iguales con la misma semilla |
| M2-e Tablas | bot de 100 semillas, plantas 1–3 | rareza por planta ±5 puntos de §2.2; nada antes de su planta; 0 mimics en callejón o como primer cofre |

### M3 — profundidad
- Blink, Haste, Unlock; Blood Altar y objetos Blood; Ring of Light; planta secreta (carne) con sus reglas; ajuste de
  oscuridad por tema; pase de equilibrio con el bot (§7).

| Prueba M3 | Ruta | Se acepta si |
|---|---|---|
| M3-a Blink | Blink hacia un pasillo con pinchos y hacia un monstruo a d = 2 | salta los pinchos sin dispararlos; para delante del monstruo; 20 f invulnerable |
| M3-b Unlock | puerta con llave sin llave | abre; MP −10 |
| M3-c Sangre | Blood Altar a 30 HP | HP 30 → 18, objeto RARE; nunca muere |
| M3-d Equilibrio | bot de 50 semillas, partida completa | métricas de §7 en rango |

### Arte necesario
Estilo: el de los temas (pintura oscura retro con sombreado interno y tramado suave, **sin contorno de dibujo
animado**; la luz cálida es la antorcha; la magia en azul frío, el peligro en rojo). Fuentes en
`art/*.aseprite` indexado con la paleta del juego, una etiqueta por animación, `LÉEME.md` (skill `gba-gfx`).

| Pieza | Tamaño | Frames / etiquetas | Bloque |
|---|---|---|---|
| Iconos de conjuro ×3 (Missile, Light, Heal) | 16×16 | 1 | M1 |
| Iconos: pergamino, Ring of Mana, siluetas vacías de los 4 huecos, Mana Potion | 16×16 | 1 | M1 |
| Icono de estado Light | 8×8 | 1 | M1 |
| Cofre (normal; con llave por paleta + candado) | 32×32 (escalado como objetos, `ESPEC_VISTA`) | `closed`, `open` | M1 |
| Misil mágico | 16×16 | `fly` 2, `hit` 2 | M1 |
| Destellos de Light y Heal (borde de la vista) | tiras 8 px o paleta | — | M1 |
| Fondo de la pausa (pestañas, rejillas, huecos de equipo) | 240×160 o sólo vista + panel | — | M1 |
| Canal del MP y etiqueta en `ui_frame` | (60, 137, 96, 7) | — | M1 |
| Iconos de conjuro ×7, armas ×4, armaduras ×4, anillos ×5, amuletos ×4, pan, pociones, llave, frasco, oro | 16×16 | 1 | M2–M3 |
| Estados Shield, Haste, Second Wind | 8×8 | 1 | M2–M3 |
| Cofre con trampa (detalle rojo en la cerradura), mimic | 32×32 | `breathe` 2, `wake` 2, `bite` 2 | M2 |
| Fireball, rayo de Frost, frasco | 32×32 / 16×16 | `fly` 2, `burst` 3 | M2 |
| Pila de oro, Altar of Coin, fuente (clara / turbia / seca por paleta) | 32×32 | 1–2 | M2 |
| Blood Altar | 32×32 | `pulse` 2 | M3 |

## 7. Números de equilibrio y métricas
Coherentes con `DISENO` §2 (HP 30 + 6/nivel, STR 2 + 1/nivel) y §4.

| Momento | Nivel | HP máx. | MP máx. | Missile | Fireball (×0,5 jefe) | Heal | Monstruo típico |
|---|---|---|---|---|---|---|---|
| Inicio planta 1 | 1 | 30 | 10 | 6 | — | 9 | rat 6 HP (1 misil) |
| Fin planta 1 | 3 | 42 | 14 | 8 | — | 12 | slime 16 HP (2 misiles = 6 MP) |
| Fin planta 2 | 5 | 54 | 18 | 10 | 20 | 16 | skeleton 20 HP (1 Fireball) |
| Jefe (planta 3) | 6–7 | 60–66 | 20–22 | 11–12 (6 al jefe) | 22–24 (11–12 al jefe) | 18–19 | Grave Warden 120 HP |

- Con todo el maná, la magia hace al jefe ≈ 22–24 de 120 (≤ 20 %), más la regeneración durante la pelea (≈ 1 MP cada
  1,5 s → ~1 misil cada 4,5 s ≈ +1,3 de daño/s frente a ≈ 13 de daño/s del arma a L6). El jefe se gana esquivando (R11).
- Maná contra arma en combate largo: 0,67 MP/s ≈ 1,3–2,6 de daño/s mágico frente a 10–13 del arma: la magia es
  apoyo; su valor está en el alcance, el primer golpe y el rescate.
- Curación disponible por planta (para que la muerte no dependa del azar): ≥ 2 pociones + pan de §6 + Heal ≈ 2 usos
  por encuentro duro.

| Métrica (R13, bot o entrada grabada) | Objetivo | Si sale fuera |
|---|---|---|
| % de muertes de monstruos con conjuro | 15–30 % | < 15: coste −1 o regeneración 1/75 f; > 30: regeneración 1/105 f |
| % de tiempo con MP < 3 (sin poder lanzar el más barato) | 10–30 % | ajustar regeneración |
| Conjuros lanzados por planta | 8–20 | — |
| Heal + pociones usadas por planta | 2–5 | más de 5: subir pan o bajar daño de la planta |
| Daño de cofre con trampa recibido / cofres con trampa abiertos | 20–50 % (se aprende el tell) | > 50 %: tell más visible (d ≤ 3) |
| Mordiscos de mimic sin aviso | **0** | bug de R5 |
| % de cofres abiertos por planta | ≥ 70 % | cofres más a la vista |
| % de muros secretos encontrados | > 30 % (`DISENO` §10) | más Light / Reveal pronto |
| % de oro gastado al acabar la planta 3 | ≥ 50 % | precios −20 % |
| Conjuros sabidos ante el jefe | 5–6 de 7 | garantías de pergamino |
| Muertes por planta (`DISENO` §10) | ≤ 3 de media | — |
| HUD por frame / sprites del HUD | ≤ 0,1 f / ≤ 32 | iconos de estado al bitmap |

## 8. Riesgos
- **Light** depende de cómo se oscurece la vista: la atenuación por distancia está cocida por `build_view.py`
  (`fade_x1000`). La versión barata es en tiempo de ejecución (halo más fuerte + brillo de la paleta del fondo +
  grietas que brillan); si el motor puede cambiar `fade` por bandas de paleta, mejor. Medir en M1 (prueba M1-c).
- **Cinturón con SELECT**: en la GBA original SELECT está bajo la cruceta (hay que soltar el movimiento para cambiar).
  Cambiar no es urgente, pero si el usuario lo nota incómodo, la alternativa es cambiar sólo en la pausa (pestaña
  `SPELLS`, R). Se decide tras la primera ROM (pregunta 1).
- **Presupuesto de sprites del HUD** sube de 24 a ~31 (§5.2).
- **ROM y VRAM**: proyectiles y cofres escalados como objetos (1 KB por proyectil); 1 proyectil del jugador a la vez.
- **Mimic** en una mazmorra generada: el tell (respiración) tiene que leerse a 1x; si no se lee en la captura, el
  mimic se queda en RARE y sólo en salas con Light posible (más fácil).
- `DISENO` §9 dejaba la magia para el bloque 3; este documento la adelanta a M1 en paralelo a los bloques del juego
  (M1 no necesita los temas; M2 sí, para §1.5).

## 9. Preguntas para el usuario
1. **Cambiar de conjuro con SELECT** (y el mapa pasa a la pausa): ¿te va bien, o prefieres que el cinturón sólo se
   cambie en la pausa (START)? Recomendado: SELECT.
2. **Altar of Coin**: `DISENO` decía "sin tiendas"; esto es una tienda sin PNJ de 3 ofertas. ¿Lo quieres o el oro sólo
   como puntuación y fuentes? Recomendado: altar.
3. **Mimics**: ¿entran (desde la planta 3, siempre con aviso) o los dejas fuera por ser sorpresa? Recomendado: sí.
4. ¿Los objetos **Blood** (doble filo a la vista) te gustan como sustituto de los malditos, o fuera?
