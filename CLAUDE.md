# GBA homebrew — instrucciones de trabajo

Juegos de Game Boy Advance en C++ con **Butano** (`engine/butano/`, v21.10). Cliente: **sólo Claude Code**
(este fichero, `.claude/agents/`, `.claude/skills/`). Nace de lo aprendido en `Desktop\SNES` (resumen en
`docs/guias/lecciones-snes.md`); no se copian sus herramientas ni su tamaño.

## Idioma y contexto del usuario

- Responde **en español** y corto. Código, comentarios, identificadores y mensajes de commit en inglés; documentación
  en español.
- No preguntes "¿sigo?" ni "¿le doy?": si el siguiente paso está en el plan, hazlo e informa.
- Pregunta sólo **decisiones reales**: aspecto, controles, dificultad, dirección artística o gasto no autorizado.
  Lo técnico sin diferencia visible lo decide el agente. Problemas de jugabilidad o aspecto → al diseñador (abajo).
- Nunca te quedes esperando a un agente: si hay trabajo pendiente que no se solapa, hazlo mientras.
- El usuario prueba jugando: quiere ROMs rápido con una comprobación corta, no baterías largas.
- El usuario abandona los proyectos que se complican: **alcance pequeño**, jugable pronto (ver "Alcance").

## Mapa

- `engine/butano/`: Butano (no se modifica; parches sólo con tarea y permiso). Ejemplos en `engine/butano/examples/`.
- `toolchain/devkitPro/`: devkitARM. `source tools/env.sh` (Git Bash) antes de compilar.
- `tools/scripts/`: utilidades propias (`gbarun.py` = Mesen 2 sin ventana). Python siempre con `-I`.
- `templates/`: plantilla de juego nuevo. `projects/<nombre>/`: un juego o laboratorio por carpeta.
- `lib/` (cuando exista): **complemento propio** sobre Butano, sólo con lo que Butano no trae.
- `docs/`: `INDEX.md` es la entrada. Harness en `docs/harness/`, guías en `docs/guias/`, tareas en
  `docs/datos/tareas.jsonl` (sólo vía `tasks.py`).
- `.claude/scripts/`: `tasks.py` (tareas) y `worktree.py` (worktrees de agente).

## Inicio de una tarea

1. `python -I .claude/scripts/tasks.py ready` y `show ID`. Sin tarea no se implementa: créala (`add`) con aceptación.
2. Lee sólo lo que la tarea necesita: `python -I .claude/scripts/kb.py brief --role <rol> --task "<tarea>"` (o
   `docs/INDEX.md`) → la guía o skill concreta → la sección. No leas docs enteras, ni todas las skills, ni ficheros
   enteros cuando basta un trozo (`grep -n`, `Read` con offset).
3. Antes de trabajo de hardware o API: `kb.py query "..." --kind hw|butano` y `kb.py symbol bn::x` (firma exacta;
   skill `gba-kb`). Luego `docs/guias/que-usar-butano.md` y el ejemplo de `engine/butano/examples/`.
   **No inventes funciones**: si `symbol` no la encuentra, no existe.
4. Confirma y reclama: `tasks.py update ID --status confirmed --append-notes "Evidence: ..."` y `--claim`.
   Ciclo completo en `docs/harness/flujo.md`.

## Butano primero

- Antes de escribir un sistema (sprites, fondos, cámara, texto, audio, guardado, input, azar, fundidos, HDMA,
  pools/contenedores), busca en `docs/guias/que-usar-butano.md` y en `engine/butano/examples/`.
- Si Butano cubre casi todo, úsalo tal cual. Un hueco real va a `lib/` (complemento propio, prefijo `gx::`) sólo con
  **tarea + prueba**, nunca resuelto a mano dentro de un juego. Lo aprendido se anota en `que-usar-butano.md`.
- Texto: **sólo** `bn::sprite_text_generator` (o texto en fondo de Butano) con fuentes de `engine/butano/common/` o
  convertidas a ese formato. Prohibidas las fuentes dibujadas a mano y los motores de texto propios. Se decide al crear
  el armazón y todo encargo de interfaz lo dice explícitamente.
- Sin memoria dinámica en el bucle: `bn::vector`, `bn::pool`, `bn::fixed`; nada de `new`/`std::vector` por frame.

## Arte y audio editables por el usuario

- Cada juego guarda sus fuentes en `projects/<juego>/art/*.aseprite` (indexado, paleta del juego, una etiqueta por
  animación) con un `LÉEME.md` en castellano. El build exporta a `graphics/*.bmp` + `.json` de Butano; los
  generadores sólo siembran y nunca sobrescriben el `.aseprite` (skill `gba-gfx`).
- Cada juego lleva `compilar.bat` y `compilar_y_jugar.bat` (doble clic) desde el armazón.
- PixelLab: sólo dentro de la suscripción; nada que gaste créditos extra sin permiso del usuario.

## Diseño primero

- Quien diseñe juego, nivel, enemigo o jefe lee `docs/guias/diseno-juegos.md` y cita las reglas que aplica; nada se
  da por bueno sin su checklist.
- Problemas que afectan a cómo se ve o se juega → tarea con etiqueta `design-question` (problema, opciones, coste)
  para `gba-designer`; el implementador deja un arreglo provisional marcado como tal.
- **Antes de dar una ROM al usuario**, `gba-designer` revisa el primer minuto: cómo se enseñan los controles, con qué
  empieza el jugador, dificultad de la primera sala, que nada le mate sin aviso.
- Controles simples e inmediatos por defecto; esquemas complejos sólo como opción.

## Alcance

- Juegos pequeños: un verbo central, una demo jugable en el primer bloque, luego crecer. Greybox primero (cuadros y
  lógica), arte final cuando el usuario ya lo ha jugado.
- Estima con datos (un `grep` de lo afectado), no a ojo.
- Se termina publicando en itch.io con el kit de la skill `gba-release`.

## Verificación

- Compilar: skill `gba-build`. Ejecutar: `python -I tools/scripts/gbarun.py` (sin ventana; skill `gba-run`).
  **Nunca abras el emulador con ventana**; eso es para el usuario.
- Prueba proporcional: lo que cambió + arranque. Revisa las capturas con `Read` **antes** de decir que funciona.
  Prueba lo que se ve, no sólo variables.
- Batería completa sólo al integrar varias cosas o antes de un push; nunca en cada relevo.
- Mide antes de optimizar (skill `gba-performance`); fidelidad y jugabilidad antes que microoptimizar.
- Un script de reemplazo comprueba que encontró el patrón. Un bug reproducible lleva su prueba.

## Git, worktrees y operaciones peligrosas

- Commit al cerrar cada bloque verificado, añadiendo sólo tus rutas (`git add <rutas>`, nunca `git add -A` con otros
  agentes activos). Mensajes en inglés. Push sólo cuando el usuario lo pida.
- Remoto: `origin` = https://github.com/ominiraindustries/gba-studio.git (privado; rama `main`). Push con
  `git push origin main` (ramas de trabajo: `git push origin <rama>`); nunca `--force`.
- Trabajo paralelo: worktrees **sólo** con `python -I .claude/scripts/worktree.py add|remove|list`
  (disperso; junctions sólo a directorios no versionados). Nunca `git worktree add` a mano ni `rm -rf` de un worktree.
  Al integrar la rama del agente, `worktree.py remove NOMBRE`. Detalle: `docs/harness/worktrees.md`.
- Nada destructivo en bloque sin el OK del usuario: borrados masivos, `git reset --hard`, `git clean`, `push --force`,
  restaurar ficheros, limpiar worktrees ajenos.
- Claves de API nunca en el repo.

## Agentes y presupuesto

- Trabaja directamente en búsquedas, cambios pequeños y tareas secuenciales. Subagentes sólo para líneas paralelas
  con rutas disjuntas, revisión independiente o una especialidad clara. Máximo 2–3 a la vez.
- Agentes: `gba-game-dev`, `gba-engine-dev`, `gba-reviewer`, `gba-designer`, `gba-artist`. Skills: `gba-build`,
  `gba-run`, `gba-gfx`, `gba-audio`, `gba-performance`, `gba-design`, `gba-release`, `gba-kb` (consulta de
  hardware/API). Para estudiar un juego comercial: skill `gba-study`.
- Presupuesto: ~120 llamadas por agente, máximo 150 salvo permiso. A las ~120: commit, nota de relevo en la tarea
  (`--append-notes "Relevo: hecho / falta / cómo probar"`) y terminar. Nunca reanudar un agente largo para otra
  fase: uno nuevo con la nota. Reglas en `docs/harness/coste-tokens.md`.
- Encargo a un agente = tarea + rutas propias + qué no tocar + prueba esperada + recordatorio de fuentes/arte.
- Esperas en segundo plano siempre con límite de tiempo; detén los agentes que ya entregaron.
- Informe final: ficheros, prueba y resultado, riesgos. Sin logs largos.
