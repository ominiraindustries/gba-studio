# Audio de HEXBOUND (G-48)

Todo generado con PixelLab (suscripción). Originales en `art/audio_src/` (MP3/WAV tal cual salen de PixelLab +
`music/loops.txt` con los puntos de bucle). Para regenerar `audio/` desde los originales:

    cd projects/007_survivors && python -I art/audio_src/convert_audio.py

No editar a mano los `.it`/`.wav` de esta carpeta: se rehacen con el script. Los `tmp_*.wav` y `blip.wav` siguen
aquí porque `include/hb_art.h` aún los usa; se pueden borrar cuando el código pase a los `sfx_*`.

## Música (`bn::music_items::<nombre>`)

PixelLab devuelve MP3 estéreo 44,1 kHz; Maxmod sólo reproduce música como módulo tracker. Cada pista es un `.it`
con **dos muestras PCM de 8 bits**: la intro (suena una vez) y el cuerpo del bucle (bucle de muestra hacia delante,
sin costura, repetido por el mezclador). Una nota dispara cada muestra; luego el módulo se queda en un patrón vacío
que salta a sí mismo, así que nunca se redispara nada. Es "WAV en bucle" dentro de un módulo.

| Item | Uso | Intro | Bucle | ROM |
|---|---|---|---|---|
| `music_title` | pantalla de título (misterioso) | 47,3 s @ 15 768 Hz | 90,1 s @ 11 630 Hz | 1,80 MB |
| `music_meadow` | partida, fase 1 (cementerio nocturno, 132 BPM) | 35,9 s @ 15 768 Hz | 68,2 s @ 15 370 Hz | 1,62 MB |
| `music_boss` | jefe (156 BPM) | 29,3 s @ 15 768 Hz | 88,7 s @ 11 820 Hz | 1,51 MB |

Cómo sonarla: `bn::music_items::music_meadow.play(bn::fixed(0.5));` (bucle por defecto; `bn::music::stop()` al
salir). Volumen sugerido 0,5–0,6 para que los efectos se oigan encima. Cambiar de pista = `play()` de la otra.

- La frecuencia del bucle baja en `title` y `boss` porque el mezclador de Maxmod guarda la posición en 20.12 bits:
  una muestra no puede pasar de 2^20 ≈ 1 048 576 frames (66 s a 15 768 Hz). Suena algo más apagada (~6 kHz de
  banda) que la intro.
- Probado en ROM (gbarun): `bn::music::playing()` = 1 desde el frame 1 y a los 48 s el módulo ya está en el orden
  de reposo (pasó de la intro al bucle sin pararse).

## Efectos (`bn::sound_items::<nombre>`)

WAV mono 8 bits 16 kHz, silencio final recortado, 10 ms de fundido y normalizados. Tono: no pasar de x1,24
(bug abierto G-46). Respetar los límites de `_play` (2 nuevos por frame, 6 a la vez).

| Item | Para | Dur. | Vol. sugerido |
|---|---|---|---|
| `sfx_spark` | disparo de chispa (uno por ráfaga, no por chispa) | 0,46 s | 0,3 |
| `sfx_hit` | impacto en enemigo/jefe | 0,46 s | 0,6 |
| `sfx_poof` | muerte de enemigo | 0,56 s | 0,4 |
| `sfx_gem` | recoger gema | 0,46 s | 0,5 |
| `sfx_levelup` | subida de nivel / santuario | 1,21 s | 0,8–1 |
| `sfx_card` | carta elegida / menú | 0,43 s | 0,5 |
| `sfx_blink` | Blink | 0,51 s | 0,8 |
| `sfx_nova` | Hex Nova | 1,32 s | 1 |
| `sfx_hurt` | golpe al mago / muerte | 0,46 s | 1 |
| `sfx_wave` | aviso de oleada (Bat Swarm, etc.) | 1,34 s | 0,8 |
| `sfx_boss` | aparece el jefe | 1,58 s | 1 |
| `sfx_howl` | aullido (bloque 2) | 1,49 s | 0,8 |
| `sfx_horn` | cuerno del orco jefe (bloque 2) | 1,49 s | 1 |
| `sfx_stomp` | pisotón (bloque 2) | 0,77 s | 0,9 |
| `sfx_explosion` | explosión / BURST (bloque 2) | 1,02 s | 0,7 |
| `sfx_zap` | Storm Chain (bloque 2) | 0,46 s | 0,5 |
| `sfx_chest` | abrir cofre (bloque 2) | 1,03 s | 0,9 |
| `sfx_coin_burst` | lluvia de monedas (bloque 2) | 1,02 s | 0,7 |

Total efectos: ~290 KB.

## Coste

- ROM: música 4,9 MB + efectos 0,29 MB. La ROM de prueba pasó de 0,45 MB a 5,5 MB (límite GBA 32 MB).
- CPU: la música ocupa 1 canal del mezclador (2 en el instante del paso intro→bucle, 20 ms). Medido con la misma
  entrada (Max power + Stress 56, 60 s): peor frame 122,3 % sin música → 124,9 % con música (+2,7 puntos); frame
  de la Nova 87,9 % → 90,1 %. OJO: el peor caso ya pasaba de 100 % sin música con el código actual (README decía
  97,7 %); no lo causa el audio.
- Si hace falta CPU: bajar la intro a ~11 kHz no ahorra (el coste es por canal y por frecuencia de mezcla, no por
  la de la muestra); lo que ahorra es `BN_CFG_AUDIO_MIXING_RATE` más bajo o una pista DMG/tracker de verdad.

## Alternativa si se quiere menos ROM

Un `.xm`/`.it` compuesto con instrumentos cortos (ondas cuadradas de pocos bytes) ocuparía <50 KB por pista con
calidad nítida, pero hay que componerlo (PixelLab no lo genera). O música DMG (`dmg_audio/`, GBT Player), casi sin
CPU. Las tomas 2 de cada pista y de cada bucle quedan en la biblioteca de PixelLab por si se prefiere otra versión.
