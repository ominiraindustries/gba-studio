# Butano 21.10.1 — Backdrop (`backdrop`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [display](display.md)

## bn_backdrop.h

### `bn::backdrop` (namespace, bn_backdrop.h:28)

Backdrop related functions.

- **color** `const optional<bn::color>& color()` (L35): Returns the overridden backdrop color if any, bn::nullopt otherwise.
  The backdrop color is the transparent color of the backgrounds.
- **set_color** `void set_color(bn::color color)` (L42): Sets the overridden backdrop color.
  The backdrop color is the transparent color of the backgrounds.
- **set_color** `void set_color(const optional<bn::color>& color)` (L49): Sets or removes the overridden backdrop color.
  The backdrop color is the transparent color of the backgrounds.
- **remove_color** `void remove_color()` (L56): Removes the overridden backdrop color, if any.
  The backdrop color is the transparent color of the backgrounds.
- **blending_bottom_enabled** `bool blending_bottom_enabled()` (L61): Indicates if the backdrop is part of the blending bottom layer or not.
- **set_blending_bottom_enabled** `void set_blending_bottom_enabled(bool blending_bottom_enabled)` (L66): Sets if the backdrop must be part of the blending bottom layer or not.
