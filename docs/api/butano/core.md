# Butano 21.10.1 — Core (`core`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_core.h

### `bn::core` (namespace, bn_core.h:39)

Core related functions.

- **init** `void init()` (L44): This function must be called before using Butano, and it must be called only once.
- **init** `void init(const optional<color>& transparent_color)` (L50): This function must be called before using Butano, and it must be called only once.
  `transparent_color` Initial transparent color of the backgrounds.
- **init** `void init(const string_view& keypad_commands)` (L58): This function must be called before using Butano, and it must be called only once.
  `keypad_commands` Keypad commands recorded with the keypad logger.
  Instead of reading the keypad of the GBA, these keypad commands are replayed.
- **init** `void init(const optional<color>& transparent_color, const string_view& keypad_commands)` (L67): This function must be called before using Butano, and it must be called only once.
  `transparent_color` Initial transparent color of the backgrounds.; `keypad_commands` Keypad commands recorded with the keypad logger.
  Instead of reading the keypad of the GBA, these keypad commands are replayed.
- **skip_frames** `int skip_frames()` (L74): Returns the number of frames to skip.
  0 skip frames means ~60 frames per second, 1 skip frame means ~30 frames per second, and so on.
- **set_skip_frames** `void set_skip_frames(int skip_frames)` (L81): Sets the number of frames to skip.
  0 skip frames means ~60 frames per second, 1 skip frame means ~30 frames per second, and so on.
- **update** `void update()` (L86): Updates the screen and all Butano subsystems.
- **sleep** `void sleep(keypad::key_type wake_up_key)` (L91): Sleeps the GBA until the given keypad key is pressed.
- **sleep** `void sleep(const span<const keypad::key_type>& wake_up_keys)` (L96): Sleeps the GBA until the given keypad keys are pressed.
- **reset** `[[noreturn]] void reset()` (L103): Resets the GBA, going back to the start of main().
  Keep in mind that it doesn't rewind the stack, so alive objects are not destroyed.
- **hard_reset** `[[noreturn]] void hard_reset()` (L110): Resets the GBA, going back to the start of main() after showing the BIOS intro.
  Keep in mind that it doesn't rewind the stack, so alive objects are not destroyed.
- **current_cpu_usage** `fixed current_cpu_usage()` (L117): Returns the CPU usage of the current frame.
  A CPU usage greater than 1 means that at least one screen refresh should have been missed.
- **current_cpu_ticks** `int current_cpu_ticks()` (L125): Returns the CPU timer ticks of the current frame.
  A CPU tick count greater than timers::ticks_per_frame() means that at least one screen refresh
  should have been missed.
- **last_cpu_usage** `fixed last_cpu_usage()` (L135): Returns the CPU usage of the last elapsed frame.
  A CPU usage greater than 1 means that at least one screen refresh should have been missed.
  
  If you only want to retrieve the number of missed screen refreshes,
  core::last_missed_frames is more accurate.
- **last_cpu_ticks** `int last_cpu_ticks()` (L146): Returns the CPU timer ticks of the last elapsed frame.
  A CPU tick count greater than timers::ticks_per_frame() means that at least one screen refresh
  should have been missed.
  
  If you only want to retrieve the number of missed screen refreshes,
  core::last_missed_frames is more accurate.
- **last_vblank_usage** `fixed last_vblank_usage()` (L154): Returns the maximum V-Blank usage of the last elapsed frame.
  A V-Blank usage greater than 1 means that the screen is being redrawn
  before all of GBA display components are updated.
- **last_vblank_ticks** `int last_vblank_ticks()` (L162): Returns the maximum V-Blank timer ticks of the last elapsed frame.
  A V-Blank tick count greater than timers::ticks_per_vblank() means that the screen is being redrawn
  before all of GBA display components are updated.
- **last_missed_frames** `int last_missed_frames()` (L167): Returns the number of screen refreshes that were missed in the last core::update call.
- **update_callback** `update_callback_type update_callback()` (L172): Returns the user function called in core::update, before V-Blank.
- **set_update_callback** `void set_update_callback(update_callback_type update_callback)` (L177): Sets the user function called in core::update, before V-Blank.
- **vblank_callback** `vblank_callback_type vblank_callback()` (L182): Returns the user function called in core::update, during V-Blank.
- **set_vblank_callback** `void set_vblank_callback(vblank_callback_type vblank_callback)` (L187): Sets the user function called in core::update, during V-Blank.
- **slow_game_pak** `bool slow_game_pak()` (L192): Indicates if a slow game pak like the SuperCard SD has been detected or not.
- **system_font** `const system_font& system_font()` (L197): Returns the font used to show assert and profiling messages.
- **set_system_font** `void set_system_font(const bn::system_font& font)` (L202): Sets the font used to show assert and profiling messages.
- **assert_tag** `const string_view& assert_tag()` (L207): Returns the tag displayed in assert messages.
- **set_assert_tag** `void set_assert_tag(const string_view& assert_tag)` (L212): Sets the tag displayed in assert messages.
- **log_stacktrace** `void log_stacktrace()` (L220): Logs the current stack trace.
  The stack trace is mangled, you can demangle it with <a href="http://demangler.com/">demangler.com</a>
  for example.

## bn_core_lock.h

### `bn::core_lock` (class, bn_core_lock.h:29)

All Butano subsystems (including interrupts and HDMA) are disabled as long as an object of this class is active.

`class core_lock`

If a Butano function or method is called while a core_lock object is alive, the behavior is undefined.

- **core_lock** `core_lock()` (L38): Disables all Butano subsystems (including interrupts and HDMA).
  If a Butano function or method is called while a core_lock object is alive, the behavior is undefined.
- **~core_lock** `~core_lock()` (L47): Restores all Butano subsystems (including interrupts and HDMA).

## bn_system_font.h

### `bn::system_font` (class, bn_system_font.h:31)

Contains information of the font used to show assert and profiling messages.

`class system_font`

- **system_font** `system_font()` (L38): Default constructor.
- **system_font** `system_font(const TFont& tte_font)` (L45): Constructor.
  `tte_font` Instance of <a href="https://www.coranac.com/man/tonclib/structTFont.htm">TFont</a> struct.
- **tte_font** `const TFont& tte_font() const` (L51): Returns an instance of <a href="https://www.coranac.com/man/tonclib/structTFont.htm">TFont</a> struct.
