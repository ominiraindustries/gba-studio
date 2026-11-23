---
name: gba-audio
description: Música y efectos en GBA con Butano - backend Maxmod por defecto (módulos .mod/.xm/.s3m/.it y .wav en audio/), sonido DMG (dmg_audio/ con GBT Player o VGM), APIs bn::music, bn::sound, bn::dmg_music y coste de CPU. Úsala al añadir o depurar audio.
---

# Audio con Butano

- Backend por defecto: **Maxmod** (`AUDIOBACKEND := maxmod` en el Makefile; alternativas `aas` o `null`).
- Ficheros en `projects/<juego>/audio/`: música como módulo tracker `.mod`, `.xm`, `.s3m`, `.it`; efectos `.wav`
  (mono, 8 bits, frecuencia baja: 8–16 kHz basta). El código ve `bn::music_items::<nombre>` y
  `bn::sound_items::<nombre>`.
  - `bn::music_items::tema.play(volumen)`, `bn::music::stop()`, `bn::music::set_volume()`.
  - `bn::sound_items::salto.play(volumen)` o `play(volumen, velocidad, panorama)`; devuelve un `bn::sound_handle`.
- Sonido estilo Game Boy (canales DMG, casi sin CPU): `dmg_audio/` con `.mod`/`.s3m` (GBT Player) o `.vgm`
  (`DMGAUDIOBACKEND`), API `bn::dmg_music`. Buen recurso para chiptune y para ahorrar CPU.
- Ejemplos: `engine/butano/examples/audio`, `audio_player`, `dmg_audio`.

## Coste y reglas
- El mezclado de Maxmod gasta CPU según canales y frecuencia (`bn_config_audio.h`, `BN_CFG_AUDIO_MIXING_RATE`):
  mídelo con el profiler (skill `gba-performance`) antes y después de subir canales.
- Música compuesta o con licencia propia: nada sacado de juegos comerciales. Créditos y licencia en el kit de release.
- Antes de integrar una propuesta musical, demos cortas (WAV) para que el usuario elija: en SNES se rechazaron varias
  versiones enteras por estilo. Preferencia aprendida: bases sencillas con groove antes que melodías estridentes.
- PixelLab tiene música/SFX: sólo dentro de la suscripción.
