# Estudiar un juego comercial de GBA

Para qué: entender y **medir** cómo hace algo un juego comercial (comportamiento visible, tiempos, uso de memoria,
VRAM y OAM, modo de vídeo, efectos) y después **reproducir la técnica con código y recursos nuestros**. Skill:
`.claude/skills/gba-study/`. Fichas: `docs/estudios/` (plantilla en `docs/estudios/plantilla-ficha.md`).

## Reglas (usuario y proyecto)

- **Se estudia, no se copia.** Nunca entra en nuestros juegos ni en el repo código, gráficos, música, sonidos, textos,
  mapas, tablas o datos de nivel del original, ni desensamblado, volcados o capturas del juego. En el repo sólo van
  cifras medidas y reglas descritas con nuestras palabras.
- Las **decompilaciones comunitarias** se pueden leer para entender; nunca se copia ni se adapta su código.
- Las ROMs son **volcados propios del usuario** y viven fuera de git (p. ej. `%USERPROFILE%\roms\gba\`). Nada de
  descargar ROMs. Volcados de RAM, registros por frame, capturas y sondas van al **scratchpad** de la sesión.
- Se estudia con un objetivo concreto (una tarea con `tasks.py`), no "todo el juego".
- Lo no medido se escribe "no medido"; lo que venga de memoria o de la red sin comprobar, "sin verificar".

## Flujo

### 1. Observar en mGBA (`tools/mgba/mGBA.exe`, con ventana: lo abre el usuario o el agente le pide la observación)

Menú **Tools**:
- **View tiles / sprites / map / palette**: qué capas usa, tamaños de sprite, paletas por banco, cómo compone el
  personaje (un sprite grande o varios), tiles cargados al vuelo o fijos.
- **View I/O registers**: DISPCNT (modo, capas, mapeo OBJ 1D/2D), BGxCNT, BLDCNT/BLDALPHA/BLDY, WININ/WINOUT.
- **Frame viewer** (0.10): descompone la imagen por capas.
- **Search memory**: búsqueda clásica (valor igual, mayor, menor, cambiado) para localizar x/y, vida, estado.
- **Memory viewer**, **Pause** y **avance de frame** (`Ctrl+N`), rebobinado, savestates, **Lua** (`tools/mgba/scripts`).
- **Debugger** (consola, `mGBA.exe -d`): breakpoints, watchpoints de lectura/escritura (`watch/w`), desensamblado.
  **GDB**: `Tools > Start GDB server` (o `-g`) y un `arm-none-eabi-gdb` (no instalado: devkitPro lo trae como
  paquete aparte; instalarlo sólo cuando haga falta).

### 2. Medir sin ventana (agentes): `tools/scripts/gbastudy.py`

Usa Mesen 2 en modo testrunner como `gbarun.py` (mismo cerrojo, misma BIOS, entrada programada idéntica).

```bash
ROM=~/roms/gba/juego.gba; OUT=<scratchpad>/estudio
# a) localizar una variable: snapshots de EWRAM+IWRAM y filtro por reglas entre cada par
python -I tools/scripts/gbastudy.py search $ROM --snap 300,340,370,410 \
  --input "301-340:right;371-410:left" --rule "+,=,-" --width 16 --out $OUT/s1
# b) registrar un tramo: OAM decodificado, IO de vídeo, paleta y RAM elegida + hoja de contacto
python -I tools/scripts/gbastudy.py record $ROM --from 300 --to 420 --input "301-360:right;361:a" \
  --ram "px=ewram:0x1F40.16s,py=ewram:0x1F44.16s" --shot-every 10 --boxes --out $OUT/r1
```
- `search`: reglas `+ - = !`, delta exacto `+40`, valor `==3`; `--signed`, `--aligned`. Mete siempre un tramo sin
  entrada con `=` para descartar contadores. Si hay demasiados candidatos, añade snapshots y reglas.
- `record`: `oam.csv` (sprites visibles por frame), `io.csv`, `ram.csv`, `frames.jsonl`, `summary.json` (modos de
  vídeo, capas, sprites máximos, bancos de paleta, registros que cambian), `sheet.png`.
- `record --poke "F:LOC=V"` escribe un valor en el frame F: confirma si una dirección hallada **causa** el
  movimiento (en `000_hello`, escribir `test_player_x` no mueve el sprite: es un espejo, no la causa).
- Otras herramientas: `gbarun.py --dump vram|oam|pal|ewram` (volcado puntual) y `--check-at` para afirmar cifras.
- Física: deriva velocidad y aceleración de `ram.csv` (diferencias por frame); el arco de salto sale de py.

### 3. Análisis estático (sólo si la medición no basta)

- **Ghidra** (NSA, libre) + un cargador GBA: *GhidraGBA* (SiD3W4y) u otro equivalente compatible con la versión de
  Ghidra instalada (sin verificar cuál está al día). Necesita **JDK 21**. Tamaño: zip ~450 MB, instalado ~1 GB, más
  ~300 MB del JDK. **No está instalado** por falta de disco: pide permiso al usuario, instálalo fuera del repo
  (p. ej. `%USERPROFILE%\tools\ghidra`), carga la ROM como ARM7TDMI little endian base `0x08000000` y bórralo al
  acabar si hace falta el disco. El proyecto de Ghidra también va fuera del repo.
- **no$gba debug** (gratuito, Windows, ~2 MB): depurador con visores de VRAM/OAM, contador de ciclos y breakpoints
  condicionales; útil para medir el coste en ciclos de una rutina.
- Usa el estático para localizar *quién escribe* una variable hallada en el paso 2 (watchpoint → función) y entender
  el algoritmo; la ficha lo describe con palabras, sin pegar desensamblado.

### 4. Decompilaciones comunitarias (sólo lectura)

- Dónde buscar: `decomp.dev`, la organización `pret` (Pokémon), `zeldaret` (Minish Cap), búsquedas en GitHub
  "<juego> decompilation gba". Estado y licencias sin verificar: compruébalos en cada caso.
- Uso permitido: leer para entender un algoritmo o confirmar una cifra medida. Se cita (repo + fichero + commit) en
  la ficha. Prohibido copiar, traducir o "adaptar" su código o sus tablas; no se clona dentro del repo (si hace falta
  clonarlo, al scratchpad o fuera de `Desktop\GBA`).

### 5. Escribir la ficha

`docs/estudios/<juego>/<tema>.md` desde la plantilla: método, cifras medidas con su prueba (comando), qué técnica
usa, **cómo lo haremos nosotros** con Butano (y qué será distinto), y lo no medido. Añade la fila en
`docs/estudios/README.md`.

## Qué medir por género

| Género / tema | Mide |
|---|---|
| Plataformas | velocidad de andar/correr (px/frame, subpíxel), aceleración y frenado en frames, salto: velocidad inicial, gravedad, velocidad terminal, altura y duración con botón corto y largo, coyote y buffer de salto, cámara (zona muerta, adelanto) |
| Shooter | balas propias y enemigas simultáneas (sprites en OAM), cadencia, velocidades, patrones (ángulos por ráfaga), invulnerabilidad, hitbox frente a sprite |
| RPG / aventura | velocidad del texto (caracteres/frame), latencia de menú, fuente (fondo o sprites, ancho fijo o variable), transiciones, encuentros (pasos) |
| Acción cenital | velocidades por dirección (diagonal normalizada o no), retroceso, frames de invulnerabilidad |
| Efectos y vídeo | modo de vídeo por pantalla, capas activas, mapeo OBJ, sprites visibles máx., bancos de paleta, blending (BLDCNT/BLDALPHA/BLDY), ventanas, mosaico, afín de BG (modo 1/2, PA-PD) y de OBJ (parámetros, doble tamaño), cambios de paleta por frame (ciclos de color), efectos por línea (HBlank) |
| Rendimiento | lag frames (el contador del juego no avanza), carga de tiles por frame (VRAM que cambia) |

## Trampas

- `gbastudy` muestrea al **empezar el VBlank**: lo que el juego cambia por línea (DMA en HBlank, IRQ de HBlank)
  sólo se ve con su valor final. Si un efecto parece imposible con lo registrado, es por línea: compruébalo en mGBA.
- El OAM registrado es la imagen que se acaba de dibujar; la variable del juego suele ir **un frame por delante**.
- Scroll, ventanas y BLDY son de sólo escritura en el bus: `gbastudy` los toma del estado del PPU de Mesen.
- Muchas variables son **coma fija** (8.8, 16.16, 20.12) o con signo: prueba `--width 32 --signed` y divide.
- El arranque es determinista sólo con la misma entrada: cada ensayo debe repetir la misma secuencia desde el reset.
  Las intros y menús varían de juego en juego: busca primero el frame en el que ya se controla al personaje.
- La búsqueda devuelve copias (buffer de OAM, cámara, posición anterior): elige la que **cause** el cambio
  (`record --poke` y mira si el sprite se mueve) antes de darla por buena.
- Mesen y mGBA pueden diferir en detalles de tiempo; una cifra crítica se confirma en los dos.
- Un emulador con ventana es para el usuario: los agentes miden sin ventana y piden observaciones concretas.
