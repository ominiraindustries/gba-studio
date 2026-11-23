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

## Datos
- `datos/tareas.jsonl`: tareas (sólo con `python -I .claude/scripts/tasks.py`; no editar a mano).

## Fuera de docs/
- Agentes: `.claude/agents/`. Skills: `.claude/skills/` (build, run, gfx, audio, performance, design, release).
- Butano: `engine/butano/butano/include/bn_*.h`, documentación en `engine/butano/butano/include/documentation/`,
  ejemplos en `engine/butano/examples/`.
- Toolchain, emulador y `gbarun.py`: `tools/` (ver su README).
