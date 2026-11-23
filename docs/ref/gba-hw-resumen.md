# Game Boy Advance: resumen de hardware

Chuleta propia para decidir rápido. La fuente de verdad es GBATEK (`docs/ref/gbatek-gba.txt`); cada sección cita
el capítulo. Busca con `python -I .claude/scripts/kb.py query "..." --kind hw`. Lo que Butano ya gestiona por ti se
marca con **Butano:**.

## CPU

- ARM7TDMI (ARMv4T) a **16,78 MHz** (2^24 Hz; un ciclo ≈ 59,6 ns). Sin caché, sin FPU, **sin división hardware**
  (la división es una llamada a la BIOS o a una rutina: cara, ~100+ ciclos). Multiplicación 1–4 ciclos.
- Dos juegos de instrucciones: **ARM** (32 bits, más potente) y **Thumb** (16 bits). La ROM tiene bus de 16 bits:
  desde ROM, Thumb es lo rápido; el código ARM caliente va en IWRAM (32 bits, sin espera).
  **Butano:** compila en Thumb por defecto; `BN_CODE_IWRAM` / ficheros `*.bn_iwram.cpp` ponen funciones ARM en IWRAM.
- Little-endian siempre. Accesos desalineados: las lecturas de 32 bits rotan, no fallan (bug silencioso).
- GBATEK: "ARM CPU Overview", "ARM CPU Instruction Cycle Times", "THUMB Instruction Summary".

## Mapa de memoria

| Zona | Dirección | Tamaño | Bus | Ciclos 8/16/32 bits | Notas |
|---|---|---|---|---|---|
| BIOS | 0000_0000 | 16 KB | 32 | 1/1/1 | sólo legible mientras se ejecuta la BIOS; SWI |
| EWRAM (on-board) | 0200_0000 | **256 KB** | 16 | 3/3/6 | 2 esperas; datos grandes, buffers |
| IWRAM (on-chip) | 0300_0000 | **32 KB** | 32 | 1/1/1 | sin espera; pila, código ARM caliente. 0300_7F00–7FFF reservado (IRQ, pila BIOS) |
| E/S | 0400_0000 | 1 KB | 32 | 1/1/1 | registros (DISPCNT, DMA, timers, IE/IF…) |
| Paleta | 0500_0000 | 1 KB | 16 | 1/1/2 | 256 colores BG + 256 OBJ, BGR555 |
| VRAM | 0600_0000 | **96 KB** | 16 | 1/1/2 | modos 0–2: 64 KB BG + 32 KB OBJ; modos 3–5: 80 KB BG + 16 KB OBJ |
| OAM | 0700_0000 | 1 KB | 32 | 1/1/1 | 128 OBJ + 32 matrices afines |
| ROM WS0 | 0800_0000 | hasta 32 MB | 16 | 5/5/8 por defecto | réplicas en WS1 0A00_0000 y WS2 0C00_0000 |
| SRAM/Flash | 0E00_0000 | hasta 64 KB | **8** | 5 | sólo accesos de 8 bits, sólo la CPU (no DMA) |

- Paleta/VRAM/OAM: +1 ciclo si el vídeo accede a la vez. Escrituras de **8 bits** a VRAM BG duplican el byte; a
  VRAM OBJ, OAM y paleta se ignoran. Escribe siempre en 16/32 bits.
- WAITCNT (0400_0204): esperas de ROM (primer acceso 4/3/2/8, secuencial 2/1) y prefetch. Los cartuchos comerciales
  usan `0x4317` (WS0 3/1, prefetch activo). **Butano:** `bn_config_game_pak.h` (`BN_CFG_GAME_PAK_WAIT_STATE_*`,
  prefetch) lo fija al arrancar.
- Sólo DMA3 (y la CPU) leen la ROM sin restricciones de destino. Cambio de bloque de 128 KB en ROM = acceso no
  secuencial.
- GBATEK: "GBA Memory Map", "GBA System Control", "GBA GamePak Prefetch".

## Guardado (backup del cartucho)

| Tipo | Tamaño | Acceso | ID en la ROM |
|---|---|---|---|
| SRAM / FRAM | 32 KB (hasta 64 KB) | bytes en 0E00_0000, batería | `SRAM_Vnnn` |
| Flash | 64 KB o 128 KB (2 bancos) | comandos tipo JEDEC, borrado por sectores de 4 KB | `FLASH512_Vnnn`, `FLASH1M_Vnnn` |
| EEPROM | 512 B o 8 KB | serie por DMA3 en 0D00_0000, bloques de 8 bytes | `EEPROM_Vnnn` |

- Emuladores y flashcarts detectan el tipo por la cadena ID alineada a 4 bytes en la ROM.
- **Butano:** `bn::sram` (32 KB, `bn::sram::write/read`, plantillas con `static_assert` de tamaño). No trae Flash ni
  EEPROM: si un juego lo necesita, es un hueco para `lib/`. Valida siempre firma + versión al leer.
- GBATEK: "GBA Cart Backup IDs", "GBA Cart Backup SRAM/FRAM", "...EEPROM", "...Flash ROM".

## Vídeo

- Pantalla **240×160**, color de 15 bits (BGR555, bit 15 ignorado), ~59,73 Hz.
- Modos (DISPCNT bits 0–2):

| Modo | Fondos | Tipo |
|---|---|---|
| 0 | BG0–BG3 | 4 regulares (tiles) |
| 1 | BG0, BG1 regulares + BG2 afín | mezcla |
| 2 | BG2, BG3 afines | rotación/escala (modo 7) |
| 3 | BG2 | bitmap 240×160, 16 bits, un buffer (75 KB) |
| 4 | BG2 | bitmap 240×160, 8 bits con paleta, doble buffer (page flip) |
| 5 | BG2 | bitmap 160×128, 16 bits, doble buffer |

- **Fondo regular**: 256×256, 512×256, 256×512 o 512×512 px; tiles 8×8 en 4 bpp (16 paletas de 16) u 8 bpp;
  entrada de mapa de 16 bits (tile 0–1023, flip H/V, paleta). Scroll por registro (BGxHOFS/VOFS, sólo escritura).
- **Fondo afín**: 128–1024 px cuadrados, siempre 8 bpp, mapa de 8 bits (256 tiles), sin flip; matriz PA–PD +
  origen de 28 bits (8 de fracción); wrap opcional.
- VRAM BG en bloques: 4 *char blocks* de 16 KB (tiles) y 32 *screen blocks* de 2 KB (mapas), solapados.
- Prioridades 0–3 en cada BG y OBJ; a igual prioridad, OBJ por encima de BG.
- **Butano:** `bn::regular_bg_ptr`, `bn::affine_bg_ptr`, `bn::*_bitmap_bg_ptr`; elige el modo según los fondos que
  crees. Máx. `BN_CFG_BGS_MAX_ITEMS` = 4.
- GBATEK: "LCD I/O Display Control", "LCD I/O BG Control", "LCD VRAM Overview", "LCD VRAM BG Screen Data Format".

### Sprites (OBJ)

- **128 OBJ** en OAM; 12 tamaños de 8×8 a 64×64 (cuadrado, ancho, alto); 4 bpp (16 paletas) u 8 bpp.
- **32 matrices afines** compartidas (PA–PD en OAM); un OBJ afín usa una matriz; *double size* duplica el área para no
  recortar al rotar.
- VRAM OBJ: 32 KB = 1024 tiles 4 bpp en modos 0–2 (en modos 3–5 sólo la mitad alta, 512 tiles). Mapeo 1D o 2D.
- **Límite por línea** (ciclos de render OBJ por scanline): 1210 (954 si "H-Blank Interval Free"). Coste: OBJ normal
  = ancho en px; OBJ afín = 10 + 2×ancho (el doble con *double size*). Ejemplos: ~128 OBJ de 8 px, ~18 de 64 px,
  ~4 afines de 64 px con *double size*. Los OBJ fuera de pantalla con prioridad mayor también consumen: pon los
  ocultos al final de OAM o en 8×8 sin afín.
- **Butano:** `bn::sprite_ptr`, `bn::sprite_affine_mat_ptr`, `bn::sprite_tiles_ptr`; ordena OAM, oculta y sube a VRAM
  en VBlank. Límites `BN_CFG_SPRITES_MAX_ITEMS` (128), `BN_CFG_SPRITE_TILES_MAX_ITEMS`.
- GBATEK: "LCD OBJ - Overview", "LCD OBJ - OAM Attributes", "LCD OBJ - OAM Rotation/Scaling Parameters".

### Efectos

- **Ventanas**: WIN0 y WIN1 (rectángulos), ventana OBJ (forma de sprites) y exterior; WININ/WINOUT eligen qué capas y
  si hay efectos en cada una. **Butano:** `bn::rect_window`, `bn::window`, `bn::inside_window`.
- **Mezcla** (BLDCNT/BLDALPHA/BLDY): alpha entre una capa 1ª y una 2ª (EVA/EVB 0–16), aclarar u oscurecer (EVY 0–16),
  OBJ semitransparentes. Sólo un modo de mezcla global a la vez. **Butano:** `bn::blending`.
- **Mosaico**: tamaño 1–16 en H y V, por separado para BG y OBJ. **Butano:** `bn::bgs_mosaic`, `bn::sprites_mosaic`.
- **Efectos por línea**: cambiar registros en cada HBlank (IRQ o DMA en HBlank) → ondas, degradados, pseudo-3D.
  **Butano:** `*_hbe_ptr` (H-Blank effects), `bn::hdma`.
- GBATEK: "LCD I/O Window Feature", "LCD I/O Color Special Effects", "LCD I/O Mosaic Function".

## Tiempos: presupuesto por frame

| | Ciclos |
|---|---|
| Por píxel | 4 |
| Línea: visible 240 px / HBlank 68 px / total 308 | 960 / 272 / **1232** |
| Frame: 160 líneas visibles / 68 de VBlank / 228 total | 197 120 / 83 776 / **280 896** |

- A 60 fps tienes ~280 000 ciclos por frame para todo (lógica + DMA + audio). VBlank ≈ 5 ms: ahí se copia a
  VRAM/OAM/paleta (fuera de VBlank/HBlank el acceso funciona, pero con espera; OAM sólo si H-Blank Interval Free).
- No hay IRQ de HBlank durante VBlank. VCOUNT (0400_0006) da la línea actual; VCount match genera IRQ.
- **Butano:** `bn::core::update()` espera al VBlank y hace las copias; mide con `bn::core::last_cpu_usage()` y
  `BN_PROFILER_*` (skill `gba-performance`).
- GBATEK: "LCD Dimensions and Timings", "LCD I/O Interrupts and Status".

## DMA

- 4 canales; prioridad DMA0 > DMA1 > DMA2 > DMA3. **La CPU se para** mientras hay DMA.
- DMA0: sólo memoria interna (ideal para HBlank). DMA1/DMA2: origen cualquiera, destino interno; también alimentan
  los FIFO de sonido. DMA3: cualquier origen y destino (ROM, EEPROM), el de las copias grandes.
- Arranque: inmediato, VBlank, HBlank o "especial" (FIFO de sonido para DMA1/2, captura de vídeo para DMA3).
  Unidades de 16 o 32 bits; repetición; incremento/decremento/fijo.
- Coste: `2N + 2(n-1)S + xI` (N/S según la memoria: p. ej. IWRAM→VRAM en 32 bits ≈ 2 ciclos por palabra).
  No ocupes los canales de alta prioridad más de ~0,25 ms o el sonido DMA se corta.
- **Butano:** usa DMA internamente (`bn::memory` copia/rellena; `bn::hdma` para efectos por línea).
- GBATEK: "GBA DMA Transfers".

## Timers e interrupciones

- 4 timers de 16 bits (TMxCNT): prescaler 1/64/256/1024 ciclos o *cascade* (cuenta los desbordes del anterior);
  IRQ al desbordar. Timer 0/1 marcan la frecuencia del sonido DirectSound. **Butano:** `bn::timer`.
- IRQ: VBlank, HBlank, VCount, Timer 0–3, Serie, DMA 0–3, Teclado, GamePak. Registros IE (0400_0200), IF (0202,
  se borra escribiendo 1), IME (0208). Halt/Stop por BIOS (SWI 2/3, IntrWait/VBlankIntrWait).
- GBATEK: "GBA Timers", "GBA Interrupt Control", "BIOS Halt Functions".

## Mandos

- 10 botones: A, B, Select, Start, →, ←, ↑, ↓, R, L. KEYINPUT (0400_0130) es **activo a nivel bajo** (0 = pulsado);
  KEYCNT para IRQ de teclado (salir de Stop).
- **Butano:** `bn::keypad::pressed/held/released(bn::keypad::key_type::A)`; grabación/reproducción con
  `keypad_commands`.
- GBATEK: "GBA Keypad Input".

## Sonido

- 4 canales heredados de Game Boy: 1 (cuadrada + barrido), 2 (cuadrada), 3 (onda de 32 muestras de 4 bits),
  4 (ruido). Volumen maestro y panorama izq/der.
- 2 canales **DirectSound A/B**: PCM con signo de 8 bits por FIFO de 32 bytes, alimentados por DMA1/DMA2 y
  sincronizados con el timer 0 o 1. La mezcla de varias voces es por software (CPU).
- Salida PWM de 10 bits (frecuencia de muestreo base 32,768 kHz). SOUNDCNT_L/H/X, SOUNDBIAS.
- **Butano:** música y efectos con Maxmod (`AUDIOBACKEND := maxmod`, también `aas` o `null`):
  `bn::music_items`, `bn::sound_items`; los canales GB con `bn::dmg_music` (casi sin CPU). Límites en
  `bn_config_audio.h` (16 canales de música, 4 de efectos por defecto). Skill `gba-audio`.
- GBATEK: "GBA Sound Controller", "GBA Sound Channel A and B - DMA Sound", "GBA Sound Control Registers".

## Otros periféricos del cartucho

- RTC (fecha/hora, por GPIO en 0800_00C4): **Butano** `bn::date`, `bn::time`. Vibración: `bn::rumble`.
- Cable link (SIO: normal, multijugador 4 GBA, UART, JOY BUS): **Butano** `bn::link`.
- Cabecera de la ROM: 192 bytes en 0800_0000 (salto, logo de Nintendo, título 12 caracteres, código 4, checksum);
  `gbafix` la arregla (el Makefile de Butano usa `ROMTITLE`/`ROMCODE`).
- GBATEK: "GBA Cartridge Header", "GBA Cart Real-Time Clock (RTC)", "GBA Communication Ports".

## Trampas habituales

1. **Escrituras de 8 bits a VRAM/OAM/paleta** (un `uint8_t*` o un `memcpy` de bytes): no funcionan o duplican.
2. **`float`/`double` y división** en el bucle: sin FPU ni divisor; usa `bn::fixed` y precalcula recíprocos.
3. **Código ARM en ROM**: lento (bus de 16 bits + esperas). Thumb en ROM; ARM sólo en IWRAM y sólo lo caliente.
4. **IWRAM llena** (32 KB con la pila): `region IWRAM overflowed` en el enlace; no metas datos grandes ahí.
5. **Sprites por línea**: muchos OBJ anchos o afines en la misma línea parpadean o desaparecen; los ocultos con
   prioridad alta también restan.
6. **Afines sin double size** se recortan al girar; las 32 matrices se comparten.
7. **Paleta índice 0 = transparente** en cada banco; el BMP debe venir indexado (skill `gba-gfx`).
8. **DMA largo** congela la CPU y puede cortar el sonido DirectSound.
9. **Guardado**: el tipo (SRAM/Flash/EEPROM) lo decide la cadena ID; firma + versión + checksum propios al leer.
10. **Memoria dinámica** en el bucle: el heap de EWRAM es pequeño y fragmenta; contenedores fijos de Butano.
11. Las 8 primeras líneas pueden quedar en sombra en la GBA original (sin retroiluminación): no pongas ahí HUD vital.

## Enlaces

- GBATEK (Martin Korth): <https://problemkaputt.de/gbatek.htm> (copia local recortada a GBA en `gbatek-gba.txt`).
- Tonc (tutorial de programación GBA, J. Vijn): <https://www.coranac.com/tonc/text/> — sólo enlace.
- Butano: <https://github.com/GValiente/butano> y referencia local en `docs/api/butano/`.
