# Estudio: <tema> de <Juego> (<compañía>, <año>)

Ficha de aprendizaje. Sin código, gráficos, datos ni capturas del original: sólo cifras medidas y técnica con
nuestras palabras. ROM: volcado propio del usuario (<región/versión>, fuera del repo). Tarea: <ID>.
Datos en bruto y sondas: scratchpad de la sesión (no versionados).

## Pregunta

Qué queremos saber y para qué juego nuestro (una o dos frases).

## Método

- Herramientas: mGBA (visores, búsqueda), `gbastudy.py search/record`, estático (sí/no), decompilación (sí/no, cita).
- Punto de partida reproducible: frame N tras el reset con la entrada `<SPEC>`.
- Variables halladas (dirección, ancho, formato, cómo se confirmó que causa el cambio):

| Variable | Dirección | Formato | Confirmada por |
|---|---|---|---|
| x jugador | `ewram:0x....` | 16.16 con signo | escribirla mueve el sprite |

## Medidas

Cada cifra con su prueba (comando o fichero del scratchpad) y en unidades GBA (px/frame, frames, subpíxeles).

| Medida | Valor | Cómo se midió |
|---|---|---|
| velocidad máx. andando | 1,5 px/frame | `record ... --ram px` diferencias, frames 300-360 |

## Vídeo y recursos (si aplica)

Modo de vídeo, capas y prioridades, mapeo OBJ, sprites visibles máx., bancos de paleta, blending/ventanas/mosaico,
afín, cambios de paleta o tiles por frame, efectos por línea (observados en mGBA).

## Técnica (con nuestras palabras)

Cómo creemos que funciona y en qué medida está comprobado.

## Cómo lo haremos

Qué reproducimos, con qué de Butano (`docs/guias/que-usar-butano.md`) o de `lib/`, qué será **distinto** y por qué
(arte, cifras ajustadas a nuestro diseño, simplificaciones). Prueba con la que verificaremos nuestra versión.

## No medido

Lo que queda sin medir y lo que se intentó, para no repetirlo.
