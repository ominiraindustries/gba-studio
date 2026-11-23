# Butano 21.10.1 — Keypad (`keypad`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_config_keypad.h

### `BN_CFG_KEYPAD_LOG_ENABLED` (macro, bn_config_keypad.h:26)

Specifies if keypad logging is enabled or not.

`#define BN_CFG_KEYPAD_LOG_ENABLED false`


## bn_keypad.h

### `bn::keypad` (namespace, bn_keypad.h:23)

Keypad related functions.

### `bn::keypad::key_type` (enum, bn_keypad.h:28)

Available key types.

`enum class key_type : uint16_t { A = 0x0001 /* A key. */, B = 0x0002 /* B key. */, SELECT = 0x0004 /* Select key. */, START = 0x0008 /* Start key. */, RIGHT = 0x0010 /* Right key. */, LEFT = 0x0020 /* Left key. */, UP = 0x0040 /* Up key. */, DOWN = 0x0080 /* Down key. */, R = 0x0100 /* R key. */, L = 0x0200 /* L key. */ }`

- **held** `bool held(key_type key)` (L45): Indicates if the given key is held or not.
- **pressed** `bool pressed(key_type key)` (L50): Indicates if the given key has been pressed in the current frame or not.
- **released** `bool released(key_type key)` (L55): Indicates if the given key has been released in the current frame or not.
- **a_held** `bool a_held()` (L60): Indicates if the `A` key is held or not.
- **a_pressed** `bool a_pressed()` (L65): Indicates if the `A` key has been pressed in the current frame or not.
- **a_released** `bool a_released()` (L70): Indicates if the `A` key has been released in the current frame or not.
- **b_held** `bool b_held()` (L75): Indicates if the `B` key is held or not.
- **b_pressed** `bool b_pressed()` (L80): Indicates if the `B` key has been pressed in the current frame or not.
- **b_released** `bool b_released()` (L85): Indicates if the `B` key has been released in the current frame or not.
- **select_held** `bool select_held()` (L90): Indicates if the `Select` key is held or not.
- **select_pressed** `bool select_pressed()` (L95): Indicates if the `Select` key has been pressed in the current frame or not.
- **select_released** `bool select_released()` (L100): Indicates if the `Select` key has been released in the current frame or not.
- **start_held** `bool start_held()` (L105): Indicates if the `Start` key is held or not.
- **start_pressed** `bool start_pressed()` (L110): Indicates if the `Start` key has been pressed in the current frame or not.
- **start_released** `bool start_released()` (L115): Indicates if the `Start` key has been released in the current frame or not.
- **right_held** `bool right_held()` (L120): Indicates if the `Right` key is held or not.
- **right_pressed** `bool right_pressed()` (L125): Indicates if the `Right` key has been pressed in the current frame or not.
- **right_released** `bool right_released()` (L130): Indicates if the `Right` key has been released in the current frame or not.
- **left_held** `bool left_held()` (L135): Indicates if the `Left` key is held or not.
- **left_pressed** `bool left_pressed()` (L140): Indicates if the `Left` key has been pressed in the current frame or not.
- **left_released** `bool left_released()` (L145): Indicates if the `Left` key has been released in the current frame or not.
- **up_held** `bool up_held()` (L150): Indicates if the `Up` key is held or not.
- **up_pressed** `bool up_pressed()` (L155): Indicates if the `Up` key has been pressed in the current frame or not.
- **up_released** `bool up_released()` (L160): Indicates if the `Up` key has been released in the current frame or not.
- **down_held** `bool down_held()` (L165): Indicates if the `Down` key is held or not.
- **down_pressed** `bool down_pressed()` (L170): Indicates if the `Down` key has been pressed in the current frame or not.
- **down_released** `bool down_released()` (L175): Indicates if the `Down` key has been released in the current frame or not.
- **r_held** `bool r_held()` (L180): Indicates if the `R` key is held or not.
- **r_pressed** `bool r_pressed()` (L185): Indicates if the `R` key has been pressed in the current frame or not.
- **r_released** `bool r_released()` (L190): Indicates if the `R` key has been released in the current frame or not.
- **l_held** `bool l_held()` (L195): Indicates if the `L` key is held or not.
- **l_pressed** `bool l_pressed()` (L200): Indicates if the `L` key has been pressed in the current frame or not.
- **l_released** `bool l_released()` (L205): Indicates if the `L` key has been released in the current frame or not.
- **any_held** `bool any_held()` (L210): Indicates if any key is held or not.
- **any_pressed** `bool any_pressed()` (L215): Indicates if any key has been pressed in the current frame or not.
- **any_released** `bool any_released()` (L220): Indicates if any key has been released in the current frame or not.
