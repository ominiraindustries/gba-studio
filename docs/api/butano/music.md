# Butano 21.10.1 — Music (`music`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [audio](audio.md)

## bn_jingle.h

### `bn::jingle` (namespace, bn_jingle.h:31)

Direct Sound jingles related functions.

Jingles are limited to 4 channels only and can't be paused or stopped.

- **playing** `bool playing()` (L36): Indicates if currently there's any Direct Sound jingle playing or not.
- **playing_item** `optional<music_item> playing_item()` (L41): Returns the active music_item if there's any Direct Sound jingle playing; bn::nullopt otherwise.
- **play** `void play(music_item item)` (L48): Plays the Direct Sound jingle specified by the given music_item with volume = 1.
  Remember that jingles are limited to 4 channels only and can't be paused or stopped.
- **play** `void play(music_item item, fixed volume)` (L58): Plays the Direct Sound jingle specified by the given music_item.
  `item` Specifies the jingle to play.; `volume` Volume level, in the range [0..1].
  Remember that jingles are limited to 4 channels only and can't be paused or stopped.
- **stop** `void stop()` (L63): Stops playback of the active Direct Sound jingle.
- **paused** `bool paused()` (L68): Indicates if the active Direct Sound jingle has been paused or not.
- **pause** `void pause()` (L73): Pauses playback of the active Direct Sound jingle.
- **resume** `void resume()` (L78): Resumes playback of the paused Direct Sound jingle.
- **volume** `fixed volume()` (L83): Returns the volume of the active Direct Sound jingle.
- **set_volume** `void set_volume(fixed volume)` (L89): Sets the volume of the active Direct Sound jingle.
  `volume` Volume level, in the range [0..1].

## bn_music.h

### `bn::music` (namespace, bn_music.h:29)

Direct Sound music related functions.

- **playing** `bool playing()` (L34): Indicates if currently there's any Direct Sound music playing or not.
- **playing_item** `optional<music_item> playing_item()` (L39): Returns the active music_item if there's any Direct Sound music playing; bn::nullopt otherwise.
- **play** `void play(music_item item)` (L46): Plays the Direct Sound music specified by the given music_item with default settings.
  Default settings are volume = 1 and loop enabled.
- **play** `void play(music_item item, fixed volume)` (L53): Plays the Direct Sound music specified by the given music_item.
  `item` Specifies the music to play.; `volume` Volume level, in the range [0..1].
- **play** `void play(music_item item, fixed volume, bool loop)` (L61): Plays the Direct Sound music specified by the given music_item.
  `item` Specifies the music to play.; `volume` Volume level, in the range [0..1].; `loop` Indicates if it must be played until it is stopped manually or until end.
- **stop** `void stop()` (L66): Stops playback of the active Direct Sound music.
- **paused** `bool paused()` (L71): Indicates if the active Direct Sound music has been paused or not.
- **pause** `void pause()` (L76): Pauses playback of the active Direct Sound music.
- **resume** `void resume()` (L81): Resumes playback of the paused Direct Sound music.
- **position** `int position()` (L86): Returns the sequence position of the active Direct Sound music.
- **set_position** `void set_position(int position)` (L91): Sets the sequence position of the active Direct Sound music.
- **volume** `fixed volume()` (L96): Returns the volume of the active Direct Sound music.
- **set_volume** `void set_volume(fixed volume)` (L102): Sets the volume of the active Direct Sound music.
  `volume` Volume level, in the range [0..1].
- **tempo** `fixed tempo()` (L107): Returns the tempo of the active Direct Sound music.
- **set_tempo** `void set_tempo(fixed tempo)` (L113): Sets the tempo of the active Direct Sound music.
  `tempo` Tempo level, in the range [0.5..2].
- **pitch** `fixed pitch()` (L118): Returns the pitch of the active Direct Sound music.
- **set_pitch** `void set_pitch(fixed pitch)` (L124): Sets the pitch of the active Direct Sound music.
  `pitch` Pitch level, in the range [0.5..2].
