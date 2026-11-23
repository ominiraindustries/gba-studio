# DELVE — diseño del HUD (G-30)

Propuesta del diseñador para el HUD específico de un dungeon crawler en primera persona y tiempo real en 240×160.
Texto de pantalla en inglés, sólo con `bn::sprite_text_generator` y la fuente `common_variable_8x8` (la que ya usa el
juego; trae su propia sombra de 1 px). Reglas citadas: R1–R14 y checklists de `docs/guias/diseno-juegos.md`.
Bocetos en `art/hud_mockups/` (240×160 y copia ×3); se regeneran con
`python -I projects/006_delve/art/hud_mockups/_src/make_mockups.py` a partir de dos capturas reales del juego.

## 1. Estudio de referencias

| Juego | Vista | Qué muestra y dónde | Qué nos sirve |
|---|---|---|---|
| Dungeon Master (1987) | ≈ 2/3 de la pantalla, arriba a la izquierda | retratos del grupo arriba con 3 barras verticales (vida, aguante, maná); columna derecha con manos/armas, menú de ataque, runas de magia y flechas de movimiento para el ratón; texto que sube bajo la vista; **sin mapa** | la mano con el arma como botón de ataque; barras cortas y gruesas que se leen de reojo |
| Eye of the Beholder (1991) | **176×120** en 320×200 (la misma que la nuestra) | panel derecho con 4–6 retratos, cada uno con **sus dos manos** (el icono del arma se oscurece mientras recarga) y barra de vida; brújula y flechas abajo a la izquierda; línea de mensajes abajo; sin mapa automático en EOB1 | la recarga en el propio icono del arma: es justo nuestro verbo |
| Dungeon Hack (1993) | como EOB | un solo personaje: retrato grande, manos, barras; brújula; mapa automático con tecla | un personaje = HUD más limpio; el mapa es básico en mazmorras generadas |
| Lands of Lore (1993) | **ancha**, casi todo el ancho | barra inferior con retratos y manos, brújula en un medallón centrado sobre la barra, inventario en una tira; mensajes bajo la vista | la vista ancha da atmósfera; toda la información en una banda que se mira de un vistazo |
| Legend of Grimrock (2012) | **pantalla completa** | retratos con manos en la esquina inferior derecha, icono de ataque con barrido de recarga, brújula arriba, minimapa opcional, mensajes flotando | lo mínimo encima de la vista; la recarga como barrido sobre el arma |
| Ports de consola/portátil (EOB de SNES y de GBA, 2002) | marcos fijos con menús | todo con cruceta y botones: los iconos dejan de ser botones y pasan a ser lectura; en la GBA, EOB cambia a combate por turnos aparte | sin ratón el HUD **sólo informa**; nada que haya que "pulsar" en pantalla |

**Conclusiones para 240×160 sin ratón**
- El HUD no es un panel de control: cada botón de la GBA ya tiene su acción (DISENO §1). Sobran flechas, menús de
  ataque y retratos clicables; queda **lectura de reojo** (R14).
- Lo que se mira en combate (vida, recarga) debe estar **cerca del centro de la vista o justo debajo**: el ojo está en
  el monstruo (aviso de ataque, R5). Lo que se mira entre combates (mapa, brújula, planta) puede ir más lejos.
- Tamaños mínimos a 1x en la pantalla de la GBA: texto 8 px con sombra, barras ≥ 5 px de alto, iconos 16×16 con
  contorno oscuro; color de vida rojo, recarga dorado, maná azul (lenguaje de color de la guía).
- La GBA tiene 128 sprites y, en el modo 4, **sólo 16 KB de VRAM de sprites** (la mitad que en modos de tiles): los
  monstruos, adornos, objetos y el texto compiten por ella. Cuanto más HUD vaya en sprites encima de la vista, menos
  sitio queda para monstruos. El marco fijo en el bitmap no gasta VRAM de sprites.

## 2. Información en pantalla

Prioridad: 1 = siempre a la vista y cerca del ojo; 2 = siempre a la vista; 3 = sólo cuando aplica.

| Dato | Prio. | Cuándo se ve | Forma | Regla / principio |
|---|---|---|---|---|
| Vida (HP) | 1 | siempre | barra roja + `24/30`; por debajo del 25 %, la barra parpadea (8 f) | checklist "nada mata sin aviso"; aversión a la pérdida; R10 |
| Recarga del arma | 1 | siempre | barra dorada bajo el icono del arma; llena = el marco del arma brilla 6 f | R1 (verbo central: esquivar y castigar), "cada acción tiene respuesta" |
| Arma (mano) | 2 | siempre | icono 32×32 (16×16 a ×2) del arma equipada; cambia solo al mejorar | gradiente de meta (la mejora se ve), R14 |
| Objeto rápido (B) | 1 | siempre | icono de la poción + `x3` + letra `B` en dorado | R6 (enseña el botón sin texto), aversión a la pérdida |
| Flechas de daño | 1 | mientras dura el aviso de un ataque lateral o trasero | flecha roja 8×16 parpadeante en el borde de la vista (hueco de ESPEC_VISTA) | R5 (todo peligro se ve antes de dañar) |
| Golpe recibido | 1 | 6 f | borde de la vista en rojo + número sobre la barra de vida | respuesta; R5 |
| Mensajes | 2 | 2 s los eventos, los de tutorial hasta que se hace la acción (máx. 6 s) | una línea, ≤ 28 caracteres | R6 (una línea en el momento), DISENO §1 |
| Brújula | 2 | siempre | cinta `E · S · W`: la del centro, la que miras; a izq./der., adonde miras tras girar | R14 (girar sin pensar), orientación en giros de 90° |
| Planta | 3 | siempre (corto: `F1`) | texto | gradiente de meta |
| Minimapa | 2 | siempre en A y C; a petición en B | casillas vistas, puertas, escalera, jugador | checklist de mundo conectado ("mapa con salas visitadas"), R9 |
| Estados (veneno…) | 3 | sólo si está activo | icono 8×8 | R9 (un estado del mundo siempre se ve) |
| Llaves | 3 | sólo con ≥ 1 | icono + `x1` | R7 (cerradura y llave reconocibles) |
| Maná | — (bloque 3) | cuando haya magia | barra azul bajo la de vida; hueco reservado | lenguaje de color |
| Nivel / XP | — | no en el HUD: en el inventario; `LEVEL UP!` en la línea | — | R14 (menos cosas a la vez) |
| `MISS`, daño, crítico | — | sobre el monstruo, en la vista | número en sprite | respuesta; no es HUD |

## 3. Opciones de disposición

Los widgets (vida, recarga, arma, objeto rápido, llaves, estados, mensajes) son **los mismos en las tres**; sólo cambia
dónde van. Así se programan una vez y cambiar de opción es sobre todo marco y vista.

### A. Panel clásico refinado — `art/hud_mockups/hud_a_panel.png`
Vista **176×120** (55 % de la pantalla, igual que ahora y que EOB).

| Zona | Rect (x, y, w, h) | Contenido |
|---|---|---|
| Vista | (0, 0, 176, 120) | mazmorra; flechas de daño en (0, 52, 8, 16) y espejo |
| Brújula + planta | (178, 2, 60, 26) | cinta `E · S · W` en y = 6, `FLOOR 1` en y = 17 |
| Minimapa | (178, 30, 60, 62) | 15×15 casillas a 4 px (como ahora) |
| Llaves / estados | (178, 94, 60, 24) | llave 16×16 + `x1`, estados 8×8 |
| Arma + recarga | (2, 122, 38, 36) | icono 32×32; barra (4, 152, 34, 5) |
| Vida + objeto rápido | (42, 122, 196, 24) | `HP`, barra (60, 127, 96, 8), `24/30`; poción `x3` `B` en (198, 124, 38, 20) |
| Mensajes | (42, 147, 196, 12) | 28 caracteres (≈ 196 px de la fuente variable) |

- **Coste**: ninguno en la vista; nuevo `ui_frame` y reordenar los textos. ≈ 1 día.
- **Lectura**: la mejor: todo en cajas oscuras con texto blanco; barras de 8 px.
- **Arte**: marco de piedra y acero con remaches de bronce, como el actual; muy "EOB". La vista se queda pequeña:
  el monstruo a d = 1 (64 px) llena la mitad del alto, pero el pasillo se ve como por una ventana.

### B. Vista completa con lo mínimo encima — `art/hud_mockups/hud_b_overlay.png`
Vista **240×160** (100 %), estilo Grimrock.

| Zona | Rect | Contenido |
|---|---|---|
| Vista | (0, 0, 240, 160) | horizonte en y = 64 |
| Vida | (2, 2, 76, 12) | placa oscura, barra (4, 5, 40, 6), `24/30` |
| Brújula | (97, 2, 47, 12) | cinta `E · S · W` |
| Planta | (210, 2, 28, 12) | `F1` |
| Mensajes | centrados en y = 18–29 | placa oscura del ancho del texto |
| Mano con antorcha | (8, 94, 56, 72) | sprite, parpadeo de la llama, luz cálida en la vista |
| Mano con arma | (162, 92, 72, 64) | sprite; se baja durante la recarga y sube al estar lista; balanceo al golpear |
| Recarga | (188, 154, 48, 5) | barra dorada bajo el arma |
| Objeto rápido | (97, 140, 35, 18) | poción `x3` |
| Minimapa | — | sólo con SELECT (mapa completo); opción de un 40×40 en la esquina |

- **Coste**: alto. 240×160 = 38 400 px, ×1,82 los de ahora: la vista pasaría de 0,78 f a ≈ 1,4 f y el zoom de paso de
  0,76 f a ≈ 1,4 f por imagen (deja de caber el esquema de fotogramas alternos); `build_view.py` con `view_w/h`
  nuevos, horizonte nuevo y columnas x = ±4; ROM de la vista ≈ ×1,8. Todo el HUD en sprites dentro de los 16 KB de VRAM
  de sprites del modo 4 (dos manos de 64×64 = 4 KB, que se quitan a los monstruos). Sin minimapa a la vista.
- **Lectura**: la peor: el texto necesita placas oscuras y las manos tapan las esquinas inferiores (el suelo de
  d = 0 y los lados).
- **Arte**: la más atmosférica (antorcha que alumbra, arma en la mano), pero dos sprites grandes de manos que dibujar y
  animar, y suelo y techo cercanos muy estirados.

### C. Vista ancha con barra inferior (Lands of Lore) — `art/hud_mockups/hud_c_bar.png`
Vista **240×112** (70 %), una sola banda de 48 px abajo.

| Zona | Rect | Contenido |
|---|---|---|
| Vista | (0, 0, 240, 112) | horizonte en y = 48 (igual que ahora); flechas de daño en (0, 52, 8, 16), espejo y (116, 96, 16, 8) abajo |
| Arma + recarga | (2, 115, 38, 36) | icono 32×32; barra (2, 152, 38, 6) |
| Vida + objetos | (42, 115, 114, 32) | `HP`, barra (60, 119, 54, 8), `24/30`; fila de iconos 16×16: poción `x3` `B`, llave `x1`, estados |
| Brújula + planta | (158, 115, 38, 32) | cinta `E · S · W`, `F1` |
| Minimapa | (198, 115, 40, 32) | 13×10 casillas a 3 px (≈ 10×8 a 4 px) alrededor del jugador |
| Mensajes | (42, 148, 196, 12) | 28 caracteres |

- **Coste**: medio y acotado. 240×112 = 26 880 px (×1,27): vista ≈ 1,0 f, zoom ≈ 1,0 f por imagen (justo).
  Se **mantienen la focal (96 px por casilla) y el horizonte**: monstruos, adornos y objetos conservan su escala y su
  arte; la vista sólo se ensancha (campo de visión mayor) y pierde 8 px de suelo abajo. En `build_view.py`:
  `view_w = 240`, `view_h = 112`, `vp_x = 120` y piezas para x = ±4 desde d = 2 (a d = 2 la columna x = 4 entra en
  los últimos ≈ 15 px de cada lado). ROM de la vista ≈ ×1,3. Nuevo `ui_frame` 240×160.
- **Lectura**: buena: los datos de combate (vida, recarga, poción) en la banda justo bajo el monstruo; el minimapa es
  pequeño pero el mapa completo sigue en SELECT.
- **Arte**: vista panorámica de mazmorra (las paredes laterales cercanas encuadran el pasillo, como en el boceto), banda
  de ladrillo y acero que continúa el marco actual.

Nota de los bocetos: en B y C la vista ancha es una aproximación (se estiran sólo las paredes, techo y suelo
cercanos de una captura de 176×120); el centro y el esqueleto están a escala real 1:1.

## 4. Recomendación

**C, vista ancha con barra inferior.** Porqué:
1. Atmósfera: +36 % de ancho de vista sin cambiar la escala de nada (focal y horizonte iguales), así que el arte ya
   hecho (monstruos 64×64, adornos, objetos) vale tal cual.
2. Combate (R1, R5): vida, recarga y poción en una sola banda justo debajo del monstruo; el ojo va del aviso a la barra
   en ≈ 75 px, sin cruzar al panel derecho.
3. Coste medido y acotado (×1,27 píxeles, ≈ 1,0 f) frente a B (×1,82, ≈ 1,4 f, VRAM de sprites justa).
4. R14: un solo sitio para mirar; nada encima de la vista salvo flechas de daño y números de golpe.

**Plan para no frenar al motor**: los widgets son los mismos en A y C. Se programan ya sobre el marco A
(vista 176×120, sin tocar el renderizador) y, cuando el agente de la vista termine, se pasa a C con `build_view.py`
y el `ui_frame` nuevo. Lo mejor de B entra en C como **opción con interruptor** (memoria del usuario: probar los
efectos que encajan): la mano con el arma que entra en la vista 8 f al golpear y el halo cálido de la antorcha.

**Métricas que lo comprobarán** (R13): en una grabación de combate con `gbarun.py`, 0 golpes recibidos sin que la
barra de vida y la flecha estuvieran en pantalla el frame anterior; vista + HUD ≤ 1,2 f en el peor paso; ≤ 24 sprites
de HUD en el peor caso (mensaje + números); capturas a 1x donde se lean `24/30`, `x3` y `F1` sin ampliar.

## 5. Preguntas para el usuario
1. ¿Qué disposición: **A** (panel clásico, ya), **B** (pantalla completa) o **C** (vista ancha con barra; recomendada)?
2. Minimapa: ¿siempre visible (pequeño en C) o sólo con SELECT para ganar más vista?
3. ¿Quieres probar las manos dentro de la vista (golpe del arma y antorcha) como opción encendible?
4. Vida: ¿barra con número `24/30` (propuesta) o sólo barra?
