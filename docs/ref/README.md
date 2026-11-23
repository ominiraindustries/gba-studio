# Referencias de hardware

| Fichero | Qué es | Origen |
|---|---|---|
| `gba-hw-resumen.md` | Resumen propio en castellano: CPU, mapa de memoria, guardado, vídeo, sprites, efectos, tiempos, DMA, timers, IRQ, mandos, sonido, trampas | escrito a partir de GBATEK, 2026-11-23 |
| `gbatek-gba.txt` | GBATEK recortado a lo que sirve en GBA (≈470 KB) | ver abajo |

## GBATEK

- Autor: Martin Korth ("GBATEK — Gameboy Advance / Nintendo DS Technical Info", extraído de no$gba 3.06).
- Descargado el **2026-11-23** de <https://problemkaputt.de/gbatek.txt> (4,15 MB, texto). Versión HTML:
  <https://problemkaputt.de/gbatek.htm>.
- Recorte: se guardan el índice general, todo el bloque "GBA Reference" (memoria, E/S, vídeo LCD, sonido, timers,
  DMA, comunicaciones, teclado, IRQ, control del sistema, cartuchos y guardado, cosas impredecibles), y de la parte
  común: CPU ARM7TDMI (visión general, registros, flags, excepciones, alineación, resumen de instrucciones ARM,
  memoria, Thumb, pseudoinstrucciones, tiempos de ciclo, versiones), funciones de la BIOS, uso de RAM de la BIOS,
  conectores del cartucho GBA y del cable link, placa de la GBA, Xboo y "About". Se quitan NDS, DSi, 3DS, ARM11/VFP y
  CP15. Los capítulos conservan el texto original sin cambios.
- Si hace falta un capítulo de DS/DSi, descárgalo aparte; no se guarda aquí para no gastar disco.

## Tutorial

- Tonc (Jasper Vijn): <https://www.coranac.com/tonc/text/>. Sólo enlace (no se copia).

## Cómo consultar

`python -I .claude/scripts/kb.py query "OAM affine matrix" --kind hw` devuelve las secciones (fichero:línea) que
mejor encajan; lee luego ese trozo con `Read` y `offset`. Skill `gba-kb`.
