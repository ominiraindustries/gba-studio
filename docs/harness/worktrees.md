# Worktrees de agente

Sólo para trabajo paralelo con rutas que podrían chocar (dos agentes en el mismo proyecto, `lib/` mientras un juego
avanza). Un agente solo en su carpeta de proyecto puede trabajar en el checkout principal.

## Lecciones (SNES, 2026-11-21/23)
- Cada relevo creaba un worktree completo (~650 MB) y nadie lo quitaba al integrar: **~107 llenaron el disco C**.
  Uno disperso con lo justo ocupa 10–35 MB.
- Una junction a un directorio **versionado** (`tools/vendor`) quedó fuera del cono disperso y git borró los ficheros
  reales del checkout principal *a través* del enlace (53 ficheros). Junctions sólo a directorios **no versionados**.
- Crear junctions con cadenas de shell dejó directorios basura. Se crean desde Python sin shell.

## Uso (siempre con el script)
```text
python -I .claude/scripts/worktree.py add NOMBRE --base main --project projects/<juego> [--paths a,b] [--full]
python -I .claude/scripts/worktree.py list
python -I .claude/scripts/worktree.py remove NOMBRE [--into main] [--force-untracked] [--delete-branch]
```
- `add`: rama `NOMBRE` desde `--base`, cono disperso con `base_paths` + el proyecto + `--paths`; junctions a los
  directorios de `junctions` que existan y que git **no** siga (si git sigue algo dentro, lo salta y lo dice).
  Los valores por defecto están en el script; se ajustan en `.claude/worktree.json`
  (`{"base_paths": [...], "junctions": [...]}`) cuando cambie la estructura del repo.
- `remove`: se niega con cambios sin commit, ficheros sin seguimiento (salvo `--force-untracked`) o rama no fusionada
  en su base. Copia las `.gba` generadas a `.claude/rom_archive/NOMBRE/`, borra primero cada enlace (sólo el enlace),
  comprueba que no queda ninguno y llama a `git worktree remove`.
- Nunca `git worktree add` a mano, nunca `rm -rf` de un worktree, nunca borrar el destino de una junction.
- Limpieza en bloque de worktrees: sólo con permiso del usuario.
- Las tareas (`tasks.py`) se escriben en el checkout principal aunque se ejecute desde el worktree.

## Ciclo
1. Coordinador: crear tarea, `worktree.py add`, encargo con la ruta del worktree y las rutas propias.
2. Agente: trabaja y hace commit en su rama.
3. Coordinador: fusiona en la base, prueba, cierra la tarea y `worktree.py remove NOMBRE --delete-branch`.
