---
name: gba-study
description: Estudiar un juego comercial de GBA para medir cómo hace algo (física, tiempos, OAM/VRAM, modo de vídeo, efectos) y reproducir la técnica con código y arte propios. gbastudy.py (búsqueda de RAM y registro por frame de OAM/IO/paleta), mGBA, Ghidra/no$gba, decompilaciones de sólo lectura y fichas en docs/estudios/. Úsala sólo cuando la tarea sea estudiar una referencia.
---

# Estudiar un juego comercial

Guía completa: `docs/guias/estudiar-juegos.md` (lee sólo la sección que necesites). Ayuda de la herramienta:
`python -I tools/scripts/gbastudy.py --help`.

## Reglas legales (no negociables)
- Se mide y se entiende; **nunca** se copia código, gráficos, música, sonido, texto, mapas ni datos del original a
  nuestros juegos ni al repo. Tampoco desensamblado, volcados ni capturas del juego.
- Decompilaciones comunitarias: sólo lectura para entender, citadas; nada copiado ni adaptado; nunca clonadas en el repo.
- ROMs: volcados propios del usuario, **fuera de git**. No se descargan ROMs. Datos en bruto al scratchpad.
- Ghidra/Java no están instalados (disco justo): instalarlos requiere permiso del usuario y va fuera del repo.

## Pasos
1. Tarea con pregunta concreta (`tasks.py`). Pide al usuario la ruta de su ROM si no está en la tarea.
2. Punto de partida reproducible: `gbarun.py ROM --frames N --shot ... --input ...` hasta llegar al control del
   personaje; revisa las capturas con `Read`.
3. Localiza variables con `gbastudy.py search` (incluye un tramo quieto con `=`); confirma la que causa el cambio.
4. Registra con `gbastudy.py record --ram ... --shot-every N --boxes`; deriva cifras de `ram.csv`/`oam.csv`/`io.csv`;
   mira `sheet.png`.
5. Si no basta: observación en mGBA (la pide al usuario, el agente no abre ventanas), luego estático (Ghidra/no$gba)
   o decompilación de lectura.
6. Ficha en `docs/estudios/<juego>/<tema>.md` desde `plantilla-ficha.md`, con "Cómo lo haremos" y "No medido";
   fila en `docs/estudios/README.md`. Commit sólo de la ficha.

## Trampas rápidas
- Muestreo al inicio del VBlank: efectos por línea (HBlank) sólo con su valor final.
- OAM = imagen recién dibujada; la variable del juego va ~1 frame por delante.
- Coma fija y signo: `--width 32 --signed`. Copias de una variable: la buena es la que, escrita, mueve el sprite.
