# APILA — diseño

Puzle de bloques que caen con las reglas modernas del género. Nombre, colores y arte **propios** (no usamos la marca
"Tetris" ni su aspecto; "Tetris" es marca registrada).

## Verbo y bucle
- **Verbo:** colocar piezas de 4 bloques para completar filas.
- **Bucle:** cae una pieza → la mueves/giras → se fija → las filas completas desaparecen → puntos → la siguiente.
- **Meta:** sobrevivir y puntuar; cada 10 líneas sube el nivel y la caída se acelera.

## Controles (inmediatos, regla 14)
| Botón | Acción |
|---|---|
| Izq / Der | mover (auto-repetición: 10 frames de espera, luego cada 2) |
| Abajo | bajar rápido (×20 la gravedad, +1 punto por fila) |
| Arriba | soltar del todo (+2 puntos por fila) |
| A / B | girar a la derecha / izquierda |
| L / R | reservar la pieza (una vez por pieza) |
| START | empezar / pausa / otra partida |

## Reglas (números)
| Regla | Valor | Principio |
|---|---|---|
| Tablero | 10×20 visibles + 4 ocultas encima | estándar del género |
| Azar | bolsa de 7 (las 7 piezas en cada tanda) | evita rachas injustas (regla 10: fallar cuesta poco) |
| Giro | SRS con 5 pruebas de "kick" | perdona sin que se note (regla 2) |
| Cola | 5 piezas a la vista + reserva | anticipación: planificar |
| Sombra | se ve dónde caerá la pieza | todo se ve antes (regla 5) |
| Retardo al fijar | 30 frames (0,5 s), se reinicia al mover/girar, máx. 15 reinicios | perdón en el suelo, sin poder alargarlo para siempre |
| Gravedad | nivel 1: 1 fila/s … nivel 20: instantánea (fórmula estándar) | flujo: reto ≈ habilidad |
| Borrado de filas | 20 frames de parpadeo | respuesta visible a cada acción |
| Fin | si la pieza nueva no cabe o se fija entera fuera de la vista | — |

## Puntos (× nivel)
Simple 100, doble 300, triple 500, cuádruple 800. T-giro: 400 / 800 / 1200 / 1600 (0–3 líneas); mini: 100 / 200 / 400.
"Seguido" (cuádruple o T-giro con líneas dos veces sin un borrado normal entre medias) ×1,5. Combo: +50 × combo.
Todo limpio: +800 / 1200 / 1800 / 2000 (3200 si es cuádruple seguido).

## Primer minuto
- Pantalla de título con los controles en una línea cada uno y "PULSA START".
- Nivel 1: 1 fila por segundo, con tiempo para probar los botones. La primera pieza aparece arriba en un tablero vacío.
- Cada borrado dice su nombre (SIMPLE, DOBLE, T-GIRO…) bajo los marcadores. Reiniciar desde el fin: START (< 1 s).

## Fuera del bloque 1 (siguientes)
Sonidos propios (girar, fijar, borrar, subir de nivel), música, récord guardado en SRAM, nivel inicial elegible,
arte final en Aseprite, modo "40 líneas" contrarreloj.

## Decisiones de diseño
- 2026-11-23 (revisión del primer minuto): la pieza nace en la fila 3 y baja una al aparecer, así se ve entera desde el
  primer frame (regla 5; métrica: captura 1 frame después de START muestra los 4 bloques). "START: OTRA" sale desde
  el primer frame del fin (regla 10). Título con el objetivo en una línea: "COMPLETA FILAS" (regla 6).
