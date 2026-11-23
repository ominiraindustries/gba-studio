---
name: gba-kb
description: Consulta rápida de la base de conocimiento GBA con .claude/scripts/kb.py - hardware (GBATEK y resumen propio), API de Butano generada de sus cabeceras (firmas, parámetros, header:línea), manual de Butano, guías, lecciones y tareas. Úsala antes de cualquier trabajo de hardware o API, para saber qué leer al empezar una tarea y para comprobar que una función de Butano existe antes de usarla.
---

# Base de conocimiento GBA (`kb.py`)

Siempre `python -I .claude/scripts/kb.py ...` desde la raíz del repo. Sin dependencias; el índice se guarda en
`.claude/cache/` (ignorado por git) y se rehace solo cuando cambia una fuente. Consulta en <1 s.

## Cuándo
- **Al empezar una tarea**: `brief` te dice los pocos ficheros y secciones que leer.
- **Antes de usar una API de Butano**: `symbol` da la firma exacta. Si no sale, no existe: no la inventes.
- **Dudas de hardware** (registros, límites, tiempos, memoria): `query --kind hw`, luego lee ese trozo con `Read` y
  `offset` (no el fichero entero: GBATEK son 470 KB).

## Órdenes
```sh
python -I .claude/scripts/kb.py brief --role game-dev --task "guardar récords en SRAM"
python -I .claude/scripts/kb.py symbol bn::sprite_ptr            # clase: resumen + miembros (--full: firmas)
python -I .claude/scripts/kb.py symbol sprite_ptr::set_position  # sin bn:: también vale; ambiguo -> candidatos
python -I .claude/scripts/kb.py query "OAM affine matrix" --kind hw
python -I .claude/scripts/kb.py query "bn::regular_bg_ptr set_position" --kind butano --limit 3
python -I .claude/scripts/kb.py query "límite de sprites por línea"   # español o inglés
python -I .claude/scripts/kb.py query "DMA timing VBlank" --full     # trozo completo
python -I .claude/scripts/kb.py stats                                 # rebuild fuerza el índice
```
Roles de `brief`: `game-dev`, `engine-dev`, `designer`, `artist`, `reviewer`, `docs` (vale con prefijo `gba-`).
Tipos de `--kind` (lista con comas): `hw`, `butano`, `guide`, `lessons`, `tasks`.

## Fuentes
| Tipo | Qué | Dónde |
|---|---|---|
| hw | GBATEK recortado a GBA; resumen propio | `docs/ref/gbatek-gba.txt`, `docs/ref/gba-hw-resumen.md` |
| butano | API generada (un trozo por símbolo, sobrecargas juntas) y manual | `docs/api/butano/index.jsonl`, `*.md`, `manual/` |
| guide | CLAUDE.md, INDEX, guías, harness, estudios, skills, agentes, README/DISENO de proyectos | `docs/`, `.claude/` |
| lessons | lecciones | `docs/guias/lecciones*.md` |
| tasks | tareas | `docs/datos/tareas.jsonl` |

Los resultados de `butano` apuntan a `engine/butano/butano/include/bn_*.h:línea` (la cabecera real) y al módulo
`docs/api/butano/<módulo>.md`.

## Mantenimiento
- Butano actualizado → `python -I .claude/scripts/gen_butano_api.py` (regenera `docs/api/butano/`) y commit.
- Página nueva en `docs/`: se indexa sola si cae en un patrón de `SOURCES` (arriba del script); si no, añádelo allí.
- Una búsqueda que no encuentra algo que sí está: añade el sinónimo en `SYN` de `kb.py` (español → inglés).
- Las líneas que contienen `kb.py ` no se indexan (son ejemplos de consulta y contaminarían los resultados).
