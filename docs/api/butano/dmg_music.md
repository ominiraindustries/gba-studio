# Butano 21.10.1 — DMG music (`dmg_music`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [audio](audio.md)

## bn_dmg_music.h

### `bn::dmg_music` (namespace, bn_dmg_music.h:31)

DMG music related functions.

- **playing** `bool playing()` (L36): Indicates if currently there's any DMG music playing or not.
- **playing_item** `optional<dmg_music_item> playing_item()` (L41): Returns the active dmg_music_item if there's any DMG music playing; bn::nullopt otherwise.
- **play** `void play(const dmg_music_item& item)` (L48): Plays the DMG music specified by the given dmg_music_item with default settings.
  Default settings are speed = 1 and loop enabled.
- **play** `void play(const dmg_music_item& item, int speed)` (L57): Plays the DMG music specified by the given dmg_music_item.
  `item` Specifies the DMG music to play.; `speed` Playback speed, in the range [1..256].
  VGM player only supports the default playback speed (1).
- **play** `void play(const dmg_music_item& item, int speed, bool loop)` (L68): Plays the DMG music specified by the given dmg_music_item.
  `item` Specifies the DMG music to play.; `speed` Playback speed, in the range [1..256].; `loop` Indicates if it must be played until it is stopped manually or until end.
  VGM player only supports the default playback speed (1).
- **stop** `void stop()` (L73): Stops playback of the active DMG music.
- **paused** `bool paused()` (L78): Indicates if the active DMG music has been paused or not.
- **pause** `void pause()` (L83): Pauses playback of the active DMG music.
- **resume** `void resume()` (L88): Resumes playback of the paused DMG music.
- **position** `const dmg_music_position& position()` (L93): Returns the sequence position of the active DMG music.
- **set_position** `void set_position(int pattern, int row)` (L100): Sets the sequence position of the active DMG music.
  `pattern` Pattern order.; `row` Row inside the pattern.
- **set_position** `void set_position(const dmg_music_position& position)` (L105): Sets the sequence position of the active DMG music.
- **left_volume** `fixed left_volume()` (L110): Returns the volume of the active DMG music for the left speaker.
- **right_volume** `fixed right_volume()` (L115): Returns the volume of the active DMG music for the right speaker.
- **set_left_volume** `void set_left_volume(fixed left_volume)` (L124): Sets the volume of the active DMG music for the left speaker.
  `left_volume` Left speaker volume level, in the range [0..1].
  Volume change is not supported by the VGM player.
- **set_right_volume** `void set_right_volume(fixed right_volume)` (L133): Sets the volume of the active DMG music for the right speaker.
  `right_volume` Right speaker volume level, in the range [0..1].
  Volume change is not supported by the VGM player.
- **set_volume** `void set_volume(fixed volume)` (L142): Sets the volume of the active DMG music for both speakers.
  `volume` Volume level, in the range [0..1].
  Volume change is not supported by the VGM player.
- **set_volume** `void set_volume(fixed left_volume, fixed right_volume)` (L152): Sets the volume of the active DMG music for both speakers.
  `left_volume` Left speaker volume level, in the range [0..1].; `right_volume` Right speaker volume level, in the range [0..1].
  Volume change is not supported by the VGM player.
- **master_volume** `dmg_music_master_volume master_volume()` (L157): Returns the master volume of all DMG music.
- **set_master_volume** `void set_master_volume(dmg_music_master_volume volume)` (L162): Sets the master volume of all DMG music.

## bn_dmg_music_master_volume.h

### `bn::dmg_music_master_volume` (enum, bn_dmg_music_master_volume.h:26)

Specifies the available DMG music master volume levels.

`enum class dmg_music_master_volume : uint8_t { QUARTER /* 25% */, HALF /* 50% */, FULL /* 100% */ }`


## bn_dmg_music_position.h

### `bn::dmg_music_position` (class, bn_dmg_music_position.h:27)

Contains the required information to specify the position of an active DMG music.

`class dmg_music_position`

- **dmg_music_position** `constexpr dmg_music_position() = default` (L34): Default constructor.
- **dmg_music_position** `constexpr dmg_music_position(int pattern, int row)` (L41): Constructor.
  `pattern` Pattern order.; `row` Row inside the pattern.
- **pattern** `constexpr int pattern() const` (L53): Returns the pattern order.
- **set_pattern** `constexpr void set_pattern(int pattern)` (L61): Sets the pattern order.
- **row** `constexpr int row() const` (L71): Returns the row inside the pattern.
- **set_row** `constexpr void set_row(int row)` (L79): Sets the row inside the pattern.
- **operator==** `constexpr friend bool operator==( const dmg_music_position& a, const dmg_music_position& b) = default` (L89): Default equal operator.

## bn_dmg_music_type.h

### `bn::dmg_music_type` (enum, bn_dmg_music_type.h:26)

Specifies the available DMG music types.

`enum class dmg_music_type : uint8_t { GBT_PLAYER /* GBT Player module files with *.mod and *.s3m extensions. */, VGM /* VGM audio files with *.vgm extension. */ }`

