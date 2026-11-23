# Butano 21.10.1 — Inside windows (`inside_window`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [window](window.md)

## bn_inside_window.h

### `bn::inside_window` (class, bn_inside_window.h:26)

Window which region can be covered by other window.

`class inside_window : public window`

- **internal** `static inside_window internal()` (L33): Returns the closest window to the screen.
- **external** `static inside_window external()` (L41): Returns the closest window to the screen not covered by the internal window.
- **sprites** `static inside_window sprites()` (L49): Returns the window made out of the visible pixels of the sprites with the window attribute enabled.
- **visible** `bool visible() const` (L57): Indicates if this window must be committed to the GBA or not.
- **set_visible** `void set_visible(bool visible)` (L62): Sets if this window must be committed to the GBA or not.
