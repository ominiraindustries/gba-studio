---
name: gba-release
description: Publicar un juego GBA en itch.io - kit de release con reproductor web (EmulatorJS con núcleo mGBA), zip de la ROM, portada, capturas, texto de la página en HTML/MD, licencias y lista de subida. Úsala al preparar una entrega pública.
---

# Kit de release para itch.io

Modelo probado: Mars Lander (SNES, 2026-11-22, `Desktop\SNES\projects\165_mars_lander\itch\`: `ITCH_UPLOAD.md`,
`build_web.py`). Aquí cambia el núcleo: **mGBA** de EmulatorJS (`EJS_core = "gba"`).

## Contenido (`projects/<juego>/itch/`)
- `<JUEGO>_WEB.zip`: `index.html` en la raíz, EmulatorJS (versión fijada, p. ej. 4.2.3, con SHA-256 comprobado) y
  su núcleo mGBA, la ROM, controles bajo el juego, botón de pantalla completa, rutas relativas y licencias
  (`LICENSE-EmulatorJS.txt`, `LICENSE-mGBA.txt`, `THIRD_PARTY_NOTICES.txt`). Los tarballs de EmulatorJS no van a git.
- `<JUEGO>.zip`: la `.gba` + `README.txt` (cómo jugar, controles, emuladores recomendados, créditos).
- `cover-630x500.png`, `banner` (opcional), `screenshots/` reales de la ROM (240×160 escaladas 3x o 4x sin
  suavizado), un GIF de juego.
- `PAGE_COPY_EN.html` + `.md`: texto de la página (controles, cómo jugar en el navegador, créditos, licencias).
- `ITCH_UPLOAD.md`: la lista de abajo rellenada para este juego.

## Comprobaciones del empaquetador
Cabecera de la ROM válida (`gbafix` la arregla en el build de Butano: título y código del Makefile), `index.html` en
la raíz, rutas relativas, tamaño por fichero y total dentro de los límites de itch.io, guardado (SRAM) persistente en
el navegador (IndexedDB de EmulatorJS) si el juego guarda, y borrado de la carpeta de montaje.

## Lista de subida
- Kind `Game`, Released, precio "No payments" salvo decisión del usuario (revisa licencias de núcleo y música).
- Subir el zip web marcado **played in the browser**, viewport 720×480 (3x) o 960×640 (4x) + franja de controles,
  botón Fullscreen. Subir el zip de la ROM como descarga sin marcar plataformas.
- Capturas en la barra lateral; tags del género + `gba`, `retro`, `pixel-art`.
- Declaración de IA si hubo PixelLab: qué partes ("AI-assisted art (PixelLab) for ...; all screenshots show the actual
  game").
- Publicar primero como Draft/Restricted, jugar en el navegador y sólo entonces público (lo hace el usuario).
- Créditos y avisos: Butano (`engine/butano/LICENSE`, zlib) y cada biblioteca de
  `engine/butano/butano/hw/3rd_party/` que lleve la ROM (Maxmod `COPYING`, etc.), copiados a `THIRD_PARTY_NOTICES.txt`.

El empaquetador común es la tarea G-2 (`tools/scripts/itch_pack.py`); mientras no exista, adapta el `build_web.py`
de Mars Lander.
