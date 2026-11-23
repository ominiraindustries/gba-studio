# Ciclo de una tarea

Herramienta: `python -I .claude/scripts/tasks.py` (datos en `docs/datos/tareas.jsonl`, versionado en git). Desde un
worktree se ejecuta igual: el script escribe siempre en el checkout principal.

**Por qué no beads/bd** (decisión 2026-11-23): en SNES `bd` necesitó un adaptador (el shim de PowerShell estaba
bloqueado y `--notes` borraba notas), su base Dolt queda fuera de git y `bd init` escribe `AGENTS.md` y hooks. Un JSONL
de pocas decenas de tareas no lo necesita. Si algún día hay cientos de tareas o varias máquinas, se reconsidera.

## Estados
| Estado | Significa | Evidencia mínima |
|---|---|---|
| `open` | reporte o idea | origen y aceptación |
| `confirmed` | existe de verdad | reproducción, captura o prueba con fecha |
| `in_progress` | reclamada | `--claim --actor NOMBRE` |
| `resolved` | hecha, sin validar del todo | commit o rutas + prueba ejecutada |
| `closed` | verificada o descartada | `--reason "Verification (fecha): comando: resultado"` o motivo + `--discard` |

```text
tasks.py add "Título" -p 2 -l proj:<juego> --acceptance "qué demuestra que está hecha"
tasks.py update ID --status confirmed --append-notes "Evidence: ..."
tasks.py update ID --claim --actor gba-game-dev
tasks.py update ID --status resolved --append-notes "Resolution: commit abc123, prueba X: OK"
tasks.py close ID --reason "Verification (2026-11-23): gbarun ... captura OK"
tasks.py ready                # lo desbloqueado, por prioridad (sin needs:user)
tasks.py dep HIJA BLOQUEADORA
```

- Un commit no cierra una tarea; la prueba sí. Si la prueba aún no se puede ejecutar, se queda en `resolved`.
- **Notas: siempre `--append-notes`.** `--notes` sobre una tarea con notas se convierte en append y avisa;
  sobrescribir sólo con `--replace-notes` a propósito (en SNES se perdieron notas así, 2026-11-23).
- Prioridad: 0 rompe · 1 importante · 2 normal · 3 menor · 4 idea.
- Etiquetas: `proj:<juego>`, `area:lib|tools|harness|docs|art|audio`, `design-question`, `needs:user` (sólo para
  decisiones reales del usuario), `outcome:discarded`.
- Lo que se descubre y no se arregla ahora → tarea nueva (`--parent` si es hija). Lo no obvio y reutilizable → a la
  guía que toque (`que-usar-butano.md`, una skill), no a un buzón.

## Relevo
A las ~120 llamadas el agente hace commit, añade a la tarea
`--append-notes "Relevo: hecho ... / falta ... / cómo probar ..."` y termina. El coordinador lanza un agente nuevo
con esa nota, nunca reanuda el largo.

## Integración (coordinador)
1. Revisa el informe y el diff; si toca al jugador, `gba-reviewer` y, antes de dar la ROM al usuario, el primer
   minuto con `gba-designer`.
2. Fusiona la rama, ejecuta la prueba del bloque en la rama integrada y cierra la tarea con el resultado.
3. `worktree.py remove NOMBRE` (ver `worktrees.md`).
