# Butano 21.10.1 — Sound effects (`sound`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [audio](audio.md)

## bn_sound.h

### `bn::sound` (namespace, bn_sound.h:29)

Sound effects related functions.

- **play** `sound_handle play(sound_item item)` (L38): Plays the sound effect specified by the given sound_item with default settings.
  `item` Specifies the sound effect to play.; devuelve: Sound effect handle.
  Default settings are volume = 1, speed = 1 and panning = 0.
- **play** `sound_handle play(sound_item item, fixed volume)` (L46): Plays the sound effect specified by the given sound_item.
  `item` Specifies the sound effect to play.; `volume` Volume level, in the range [0..1].; devuelve: Sound effect handle.
- **play** `sound_handle play(sound_item item, fixed volume, fixed speed, fixed panning)` (L56): Plays the sound effect specified by the given sound_item.
  `item` Specifies the sound effect to play.; `volume` Volume level, in the range [0..1].; `speed` Playback rate, in the range [0..64].; `panning` Panning level, in the range [-1..1].; devuelve: Sound effect handle.
- **play_with_priority** `sound_handle play_with_priority(int priority, sound_item item)` (L70): Plays the sound effect specified by the given sound_item with default settings and the given priority.
  `priority` Priority in the range [-32767..32767].; `item` Specifies the sound effect to play.; devuelve: Sound effect handle.
  If too many sound effects are being played at the same time,
  sound effects with higher priority are discarded later.
  
  Default settings are volume = 1, speed = 1 and panning = 0.
- **play_with_priority** `sound_handle play_with_priority(int priority, sound_item item, fixed volume)` (L83): Plays the sound effect specified by the given sound_item with the given priority.
  `priority` Priority in the range [-32767..32767].; `item` Specifies the sound effect to play.; `volume` Volume level, in the range [0..1].; devuelve: Sound effect handle.
  If too many sound effects are being played at the same time,
  sound effects with higher priority are discarded later.
- **play_with_priority** `sound_handle play_with_priority(int priority, sound_item item, fixed volume, fixed speed, fixed panning)` (L98): Plays the sound effect specified by the given sound_item with the given priority.
  `priority` Priority in the range [-32767..32767].; `item` Specifies the sound effect to play.; `volume` Volume level, in the range [0..1].; `speed` Playback rate, in the range [0..64].; `panning` Panning level, in the range [-1..1].; devuelve: Sound effect handle.
  If too many sound effects are being played at the same time,
  sound effects with higher priority are discarded later.
- **play_optional** `optional<sound_handle> play_optional(sound_item item)` (L107): Plays the sound effect specified by the given sound_item with default settings.
  `item` Specifies the sound effect to play.; devuelve: Sound effect handle if it could be played; bn::nullopt otherwise.
  Default settings are volume = 1, speed = 1 and panning = 0.
- **play_optional** `optional<sound_handle> play_optional(sound_item item, fixed volume)` (L115): Plays the sound effect specified by the given sound_item.
  `item` Specifies the sound effect to play.; `volume` Volume level, in the range [0..1].; devuelve: Sound effect handle if it could be played; bn::nullopt otherwise.
- **play_optional** `optional<sound_handle> play_optional(sound_item item, fixed volume, fixed speed, fixed panning)` (L125): Plays the sound effect specified by the given sound_item.
  `item` Specifies the sound effect to play.; `volume` Volume level, in the range [0..1].; `speed` Playback rate, in the range [0..64].; `panning` Panning level, in the range [-1..1].; devuelve: Sound effect handle if it could be played; bn::nullopt otherwise.
- **play_with_priority_optional** `optional<sound_handle> play_with_priority_optional(int priority, sound_item item)` (L139): Plays the sound effect specified by the given sound_item with default settings and the given priority.
  `priority` Priority in the range [-32767..32767].; `item` Specifies the sound effect to play.; devuelve: Sound effect handle if it could be played; bn::nullopt otherwise.
  If too many sound effects are being played at the same time,
  sound effects with higher priority are discarded later.
  
  Default settings are volume = 1, speed = 1 and panning = 0.
- **play_with_priority_optional** `optional<sound_handle> play_with_priority_optional(int priority, sound_item item, fixed volume)` (L152): Plays the sound effect specified by the given sound_item with the given priority.
  `priority` Priority in the range [-32767..32767].; `item` Specifies the sound effect to play.; `volume` Volume level, in the range [0..1].; devuelve: Sound effect handle if it could be played; bn::nullopt otherwise.
  If too many sound effects are being played at the same time,
  sound effects with higher priority are discarded later.
- **play_with_priority_optional** `optional<sound_handle> play_with_priority_optional( int priority, sound_item item, fixed volume, fixed speed, fixed panning)` (L167): Plays the sound effect specified by the given sound_item with the given priority.
  `priority` Priority in the range [-32767..32767].; `item` Specifies the sound effect to play.; `volume` Volume level, in the range [0..1].; `speed` Playback rate, in the range [0..64].; `panning` Panning level, in the range [-1..1].; devuelve: Sound effect handle if it could be played; bn::nullopt otherwise.
  If too many sound effects are being played at the same time,
  sound effects with higher priority are discarded later.
- **stop_all** `void stop_all()` (L173): Stops all sound effects that are being played currently.
- **master_volume** `fixed master_volume()` (L178): Returns the master volume of all sound effects.
- **set_master_volume** `void set_master_volume(fixed volume)` (L184): Sets the master volume of all sound effects.
  `volume` Master volume level, in the range [0..1].

## bn_sound_handle.h

### `bn::sound_handle` (class, bn_sound_handle.h:28)

Sound effect handle.

`class sound_handle`

- **id** `int id() const` (L35): Returns the internal id.
- **active** `bool active() const` (L43): Indicates if this sound effect is active or not.
- **item** `sound_item item() const` (L48): Returns the sound_item of this sound effect.
- **stop** `void stop()` (L53): Stops playback of this sound effect.
- **release** `void release()` (L61): Marks this sound effect as unimportant. This enables the sound effect to be interrupted by music and other sound effects if the need arises.
  The handle will be invalidated after calling this method.
- **speed** `fixed speed() const` (L66): Returns the playback rate of this sound effect.
- **set_speed** `void set_speed(fixed speed)` (L72): Sets the playback rate of this sound effect.
  `speed` Playback rate, in the range [0..64].
- **panning** `fixed panning() const` (L77): Returns the panning level of this sound effect.
- **set_panning** `void set_panning(fixed panning)` (L83): Sets the panning level of this sound effect.
  `panning` Panning level, in the range [-1..1].
