# Butano 21.10.1 — Rumble (`rumble`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_rumble.h

### `bn::rumble` (namespace, bn_rumble.h:23)

Rumble related functions.

- **enabled** `bool enabled()` (L31): Indicates if rumble is enabled or not.
  Butano does not know if your cart supports rumble or not,
  so it can report that it is enabled even when it is not supported.
- **set_enabled** `void set_enabled(bool enabled)` (L38): Sets if rumble must be enabled or not.
  Remember that rumble only works if you cart supports it.
