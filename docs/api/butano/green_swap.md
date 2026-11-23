# Butano 21.10.1 — Green swap (`green_swap`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [display](display.md)

## bn_green_swap.h

### `bn::green_swap` (namespace, bn_green_swap.h:26)

When it is enabled, green intensity of each two screen pixels are exchanged.

It produces an interesting dirt effect.

- **enabled** `bool enabled()` (L31): Indicates if green swap is enabled or not.
- **set_enabled** `void set_enabled(bool enabled)` (L36): Sets if green swap must be enabled or not.

## bn_green_swap_mode.h

### `bn::green_swap_mode` (enum, bn_green_swap_mode.h:26)

Specifies how a layer must be displayed when green swap is enabled.

`enum class green_swap_mode : uint8_t { DEFAULT /* The default green swap dirt effect. */, HALF_TRANSPARENT_A /* Half of the columns are hidden. */, HALF_TRANSPARENT_B /* The other half of the columns are hidden. */, DUPLICATED /* Pixels are duplicated */ }`

