---
name: gba-run
description: Ejecutar una ROM GBA sin ventana con tools/scripts/gbarun.py (Mesen 2 testrunner) para capturas, entrada programada, comprobaciones de memoria/símbolos y goldens; también cómo reproducir un fallo con el registro de teclas de Butano. Úsala para verificar cualquier cambio.
---

# Ejecutar y probar sin ventana

`python -I tools/scripts/gbarun.py --help` manda sobre esta página. Resumen:

```text
python -I tools/scripts/gbarun.py projects/<x>/<x>.gba --frames 300 --shot 60,299 --nonblank \
  --input "30-90:right;100:a" --check "test_player_x.32>=140" --out <scratchpad>/run_x --timeout 60
```
- `--shot` frames a capturar; `--sheet PNG` hoja de contacto; `--nonblank` falla si una captura es de un solo color.
- `--check` / `--check-at "F:SPEC"`: `LOC OP VALUE`; LOC es un símbolo del `.elf` (global **no static**, p. ej.
  `test_player_x.32`) o `mem:offset` (`ewram`, `iwram`, `vram`, `pal`, `oam`, `sram`). Expón el estado a probar
  como globales `test_*` en el juego.
- `--golden DIR` compara píxel a píxel (`--update-golden` sólo cuando el cambio visual es intencionado y revisado).
- `--dump` vuelca memoria. Salida siempre al **scratchpad**, no al repo. Códigos: 0 pasa, 1 falla, 2 sin resultado.
- Las ejecuciones se serializan con un cerrojo compartido con `snesrun.py` del repo SNES: no lances varias a la vez.

## Reglas
- **Nunca** abras un emulador con ventana (`compilar_y_jugar.bat`, mGBA de `tools/mgba`): es para el usuario.
- Mira cada captura con `Read` antes de decir que algo funciona: texto legible, sprites en su sitio, sin basura.
- Pruebas deterministas: misma ROM + misma entrada = mismas capturas. Si no, hay un bug (azar sembrado con algo
  variable, memoria sin iniciar). `bn::random` con semilla fija en pruebas.
- No confíes en el estado de arranque: inicializa todo lo que leas (VRAM, OAM, SRAM incluidos).
- `BN_LOG` usa el backend de mGBA y no llega a Mesen: para pruebas usa globales `test_*` + `--check`.

## Reproducir un fallo del usuario
Butano graba y reproduce entrada: keypad logger (`BN_CFG_KEYPAD_LOG_ENABLED`) y `bn::core::init(keypad_commands)`
(ejemplo `engine/butano/examples/keypad_commands`). Para un fallo que sólo ve el usuario, build de registro, que lo
reproduzca, y la cadena va a una build de prueba que `gbarun.py` ejecuta.

## Proporcionalidad
Cambio local: arranque + la prueba de lo cambiado (1–2 capturas, `--nonblank`). Integración o antes de push: la
batería del proyecto. Nunca bucles sobre todas las ROMs por defecto.
