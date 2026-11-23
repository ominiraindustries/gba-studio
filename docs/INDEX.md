# Índice de documentación

Entrada única. Lee sólo la página de tu tarea y, dentro, la sección que toque.

## Harness (cómo trabajamos)
| Página | Para qué |
|---|---|
| [harness/flujo.md](harness/flujo.md) | Ciclo de una tarea con `tasks.py`: estados, evidencia, cierre, relevos |
| [harness/coste-tokens.md](harness/coste-tokens.md) | Presupuesto de llamadas y contexto de los agentes |
| [harness/worktrees.md](harness/worktrees.md) | Trabajo en paralelo con `worktree.py` (disperso, junctions seguras, retirada) |

## Guías
| Página | Para qué |
|---|---|
| [guias/que-usar-butano.md](guias/que-usar-butano.md) | Necesidad → API de Butano (o de `lib/`). Consultar antes de escribir un sistema |
| [guias/diseno-juegos.md](guias/diseno-juegos.md) | Principios de diseño, reglas de oro y checklists |
| [guias/lecciones-snes.md](guias/lecciones-snes.md) | Lo aprendido en el estudio de SNES que vale aquí |
| [guias/estudiar-juegos.md](guias/estudiar-juegos.md) | Cómo estudiar y medir un juego comercial de GBA (skill `gba-study`) |

## Consulta rápida (antes de trabajo de hardware o API)
`python -I .claude/scripts/kb.py query "..." [--kind hw|butano|guide|lessons|tasks]`, `kb.py symbol bn::x` (firma
exacta de Butano) y `kb.py brief --role R --task "..."` (qué leer al empezar). Skill `gba-kb`.

| Página | Para qué |
|---|---|
| [ref/gba-hw-resumen.md](ref/gba-hw-resumen.md) | Resumen propio del hardware: memoria, vídeo, sprites, DMA, timers, sonido, tiempos, trampas |
| [ref/gbatek-gba.txt](ref/gbatek-gba.txt) | GBATEK (Martin Korth) recortado a GBA; origen y fecha en [ref/README.md](ref/README.md) |
| [api/butano/README.md](api/butano/README.md) | API de Butano generada de sus cabeceras (módulos, `index.jsonl`) y su manual en `api/butano/manual/` |

## Estudios
- [estudios/README.md](estudios/README.md): fichas de lo medido en juegos comerciales; plantilla en
  [estudios/plantilla-ficha.md](estudios/plantilla-ficha.md).

## Datos
- `datos/tareas.jsonl`: tareas (sólo con `python -I .claude/scripts/tasks.py`; no editar a mano).

## Fuera de docs/
- Agentes: `.claude/agents/`. Skills: `.claude/skills/` (build, run, gfx, audio, performance, design, release, kb, study).
- Butano: `engine/butano/butano/include/bn_*.h`, documentación en `engine/butano/butano/include/documentation/`,
  ejemplos en `engine/butano/examples/`.
- Toolchain, emulador y `gbarun.py`: `tools/` (ver su README).
