# Butano 21.10.1 — Audio (`audio`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_audio.h

### `bn::audio` (namespace, bn_audio.h:28)

Audio related functions.

- **available_mixing_rates** `span<const audio_mixing_rate> available_mixing_rates()` (L33): Returns the available Direct Sound mixing rates.
- **mixing_rate** `audio_mixing_rate mixing_rate()` (L38): Returns the current Direct Sound mixing rate.
- **set_mixing_rate** `void set_mixing_rate(audio_mixing_rate mixing_rate)` (L45): Specifies the current Direct Sound mixing rate.
  All Direct Sound audio playback must be stopped before changing the mixing rate.
- **update_on_vblank** `bool update_on_vblank()` (L53): Indicates if audio is updated on the V-Blank interrupt or not.
  Updating audio on the V-Blank interrupt helps to reduce audio noise
  but increases the possibility of visual bugs because of lack of V-Blank time.
- **set_update_on_vblank** `void set_update_on_vblank(bool update_on_vblank)` (L61): Sets if audio must be updated on the V-Blank interrupt or not.
  Updating audio on the V-Blank interrupt helps to reduce audio noise
  but increases the possibility of visual bugs because of lack of V-Blank time.
- **event_handler_enabled** `bool event_handler_enabled()` (L69): Indicates if audio events are registered or not.
  See https://blocksds.skylyrac.net/maxmod/md_documentation_2using__song__events.html
  for more information about audio events.
- **set_event_handler_enabled** `void set_event_handler_enabled(bool enabled)` (L77): Sets if audio events must be registered or not.
  See https://blocksds.skylyrac.net/maxmod/md_documentation_2using__song__events.html
  for more information about audio events.
- **event_ids** `span<uint8_t> event_ids()` (L87): Returns the ID of the audio events registered since the last core::update call.
  To register audio events, the event handler must be enabled with audio::set_event_handler_enabled.
  
  See https://blocksds.skylyrac.net/maxmod/md_documentation_2using__song__events.html
  for more information about audio events.

## bn_audio_mixing_rate.h

### `BN_AUDIO_MIXING_RATE_8_KHZ` (macro, bn_audio_mixing_rate.h:25)

8KHz mixing rate, provides poor quality.

`#define BN_AUDIO_MIXING_RATE_8_KHZ 0`

### `BN_AUDIO_MIXING_RATE_10_KHZ` (macro, bn_audio_mixing_rate.h:34)

10Khz mixing rate, provides low quality.

`#define BN_AUDIO_MIXING_RATE_10_KHZ 1`

### `BN_AUDIO_MIXING_RATE_12_KHZ` (macro, bn_audio_mixing_rate.h:43)

12Khz mixing rate, provides almost OK quality.

`#define BN_AUDIO_MIXING_RATE_12_KHZ 8`

### `BN_AUDIO_MIXING_RATE_13_KHZ` (macro, bn_audio_mixing_rate.h:52)

13Khz mixing rate, provides almost OK quality.

`#define BN_AUDIO_MIXING_RATE_13_KHZ 2`

### `BN_AUDIO_MIXING_RATE_16_KHZ` (macro, bn_audio_mixing_rate.h:61)

16Khz mixing rate, provides OK quality, standard setting.

`#define BN_AUDIO_MIXING_RATE_16_KHZ 3`

### `BN_AUDIO_MIXING_RATE_18_KHZ` (macro, bn_audio_mixing_rate.h:70)

18Khz mixing rate, provides better quality, high CPU load.

`#define BN_AUDIO_MIXING_RATE_18_KHZ 4`

### `BN_AUDIO_MIXING_RATE_20_KHZ` (macro, bn_audio_mixing_rate.h:79)

20Khz mixing rate, provides better quality, high CPU load.

`#define BN_AUDIO_MIXING_RATE_20_KHZ 9`

### `BN_AUDIO_MIXING_RATE_21_KHZ` (macro, bn_audio_mixing_rate.h:88)

21Khz mixing rate, provides even better quality, higher CPU load.

`#define BN_AUDIO_MIXING_RATE_21_KHZ 5`

### `BN_AUDIO_MIXING_RATE_24_KHZ` (macro, bn_audio_mixing_rate.h:97)

24Khz mixing rate, provides even better quality, higher CPU load.

`#define BN_AUDIO_MIXING_RATE_24_KHZ 10`

### `BN_AUDIO_MIXING_RATE_27_KHZ` (macro, bn_audio_mixing_rate.h:106)

27Khz mixing rate, provides almost the best quality, almost the highest CPU load.

`#define BN_AUDIO_MIXING_RATE_27_KHZ 6`

### `BN_AUDIO_MIXING_RATE_28_KHZ` (macro, bn_audio_mixing_rate.h:115)

28Khz mixing rate, provides almost the best quality, almost the highest CPU load.

`#define BN_AUDIO_MIXING_RATE_28_KHZ 11`

### `BN_AUDIO_MIXING_RATE_31_KHZ` (macro, bn_audio_mixing_rate.h:124)

31Khz mixing rate, provides the best quality, highest CPU load.

`#define BN_AUDIO_MIXING_RATE_31_KHZ 7`

### `BN_AUDIO_MIXING_RATE_32_KHZ` (macro, bn_audio_mixing_rate.h:133)

32Khz mixing rate, provides the best quality, highest CPU load.

`#define BN_AUDIO_MIXING_RATE_32_KHZ 12`

### `bn::audio_mixing_rate` (enum, bn_audio_mixing_rate.h:144)

Specifies the available Direct Sound mixing rates.

`enum class audio_mixing_rate : uint8_t { KHZ_8 = BN_AUDIO_MIXING_RATE_8_KHZ /* 8KHz mixing rate, provides poor quality. */, KHZ_10 = BN_AUDIO_MIXING_RATE_10_KHZ /* 10Khz mixing rate, provides low quality. */, KHZ_12 = BN_AUDIO_MIXING_RATE_12_KHZ /* 12Khz mixing rate, provides almost OK quality. */, KHZ_13 = BN_AUDIO_MIXING_RATE_13_KHZ /* 13Khz mixing rate, provides almost OK quality. */, KHZ_16 = BN_AUDIO_MIXING_RATE_16_KHZ /* 16Khz mixing rate, provides OK quality, standard setting. */, KHZ_18 = BN_AUDIO_MIXING_RATE_18_KHZ /* 18Khz mixing rate, provides better quality, high CPU load. */, KHZ_20 = BN_AUDIO_MIXING_RATE_20_KHZ /* 20Khz mixing rate, provides better quality, high CPU load. */, KHZ_21 = BN_AUDIO_MIXING_RATE_21_KHZ /* 21Khz mixing rate, provides even better quality, higher CPU load. */, KHZ_24 = BN_AUDIO_MIXING_RATE_24_KHZ /* 24Khz mixing rate, provides even better quality, higher CPU load. */, KHZ_27 = BN_AUDIO_MIXING_RATE_27_KHZ /* 27Khz mixing rate, provides almost the best quality, almost the highest CPU load. */, KHZ_28 = BN_AUDIO_MIXING_RATE_28_KHZ /* 28Khz mixing rate, provides almost the best quality, almost the highest CPU load. */, KHZ_31 = BN_AUDIO_MIXING_RATE_31_KHZ /* 31Khz mixing rate, provides the best quality, highest CPU load. */, KHZ_32 = BN_AUDIO_MIXING_RATE_32_KHZ /* 32Khz mixing rate, provides the best quality, highest CPU load. */ }`


## bn_config_audio.h

### `BN_CFG_AUDIO_MIXING_RATE` (macro, bn_config_audio.h:28)

Specifies the initial Direct Sound mixing rate.

`#define BN_CFG_AUDIO_MIXING_RATE BN_AUDIO_MIXING_RATE_16_KHZ`

Values not specified in BN_AUDIO_MIXING_RATE_* macros are not allowed.

### `BN_CFG_AUDIO_MAX_MUSIC_CHANNELS` (macro, bn_config_audio.h:39)

Specifies the maximum number of active Direct Sound music channels.

`#define BN_CFG_AUDIO_MAX_MUSIC_CHANNELS 16`

### `BN_CFG_AUDIO_MAX_SOUND_CHANNELS` (macro, bn_config_audio.h:50)

Specifies the maximum number of active sound effects.

`#define BN_CFG_AUDIO_MAX_SOUND_CHANNELS 4`

### `BN_CFG_AUDIO_STEREO` (macro, bn_config_audio.h:63)

Specifies Direct Sound mono/stereo output.

`#define BN_CFG_AUDIO_STEREO true`

Mono output reduces CPU load.

### `BN_CFG_AUDIO_DYNAMIC_MIXING` (macro, bn_config_audio.h:78)

Specifies if Direct Sound dynamic mixing must be enabled or not.

`#define BN_CFG_AUDIO_DYNAMIC_MIXING false`

Dynamic mixing automatically adjusts the mixing rate depending on the frequency of the sounds being played,
up to a maximum of the mixing rate specified. It often yields a significant performance boost
for only a very small drop in quality.

### `BN_CFG_AUDIO_MAX_COMMANDS` (macro, bn_config_audio.h:91)

Specifies the size of the audio commands list.

`#define BN_CFG_AUDIO_MAX_COMMANDS (((BN_CFG_AUDIO_MAX_SOUND_CHANNELS) * 2) + 1)`

This list is processed and cleared when bn::core::update() is called.

### `BN_CFG_AUDIO_MAX_EVENTS` (macro, bn_config_audio.h:105)

Specifies the maximum number of active audio events per update.

`#define BN_CFG_AUDIO_MAX_EVENTS 8`

See https://blocksds.skylyrac.net/maxmod/md_documentation_2using__song__events.html
for more information about audio events.

