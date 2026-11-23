# Butano 21.10.1 — Windows (`window`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [display](display.md)

## bn_window.h

### `bn::window` (class, bn_window.h:32)

Base window class.

`class window`

- **internal** `static window internal()` (L39): Returns the closest window to the screen.
- **external** `static window external()` (L47): Returns the closest window to the screen not covered by the internal window.
- **sprites** `static window sprites()` (L55): Returns the window made out of the visible pixels of the sprites with the window attribute enabled.
- **outside** `static window outside()` (L63): Returns the window of the region not covered by any other window.
- **id** `int id() const` (L71): Returns the internal ID.
- **show_bg** `bool show_bg(const regular_bg_ptr& regular_bg) const` (L81): Indicates if the specified background is shown in this window.
  `regular_bg` regular_bg_ptr to ask for.; devuelve: `true` if the specified background is shown in this window, otherwise `false`.
- **show_bg** `bool show_bg(const affine_bg_ptr& affine_bg) const` (L88): Indicates if the specified background is shown in this window.
  `affine_bg` affine_bg_ptr to ask for.; devuelve: `true` if the specified background is shown in this window, otherwise `false`.
- **show_bg** `bool show_bg(const palette_bitmap_bg_ptr& palette_bitmap_bg) const` (L95): Indicates if the specified background is shown in this window.
  `palette_bitmap_bg` palette_bitmap_bg_ptr to ask for.; devuelve: `true` if the specified background is shown in this window, otherwise `false`.
- **show_bg** `bool show_bg(const sp_direct_bitmap_bg_ptr& sp_direct_bitmap_bg) const` (L102): Indicates if the specified background is shown in this window.
  `sp_direct_bitmap_bg` sp_direct_bitmap_bg_ptr to ask for.; devuelve: `true` if the specified background is shown in this window, otherwise `false`.
- **show_bg** `bool show_bg(const dp_direct_bitmap_bg_ptr& dp_direct_bitmap_bg) const` (L109): Indicates if the specified background is shown in this window.
  `dp_direct_bitmap_bg` dp_direct_bitmap_bg_ptr to ask for.; devuelve: `true` if the specified background is shown in this window, otherwise `false`.
- **set_show_bg** `void set_show_bg(const regular_bg_ptr& regular_bg, bool show)` (L116): Sets if the specified background must be shown in this window.
  `regular_bg` regular_bg_ptr to show or hide.; `show` `true` if the specified background must be shown in this window, otherwise `false`.
- **set_show_bg** `void set_show_bg(const affine_bg_ptr& affine_bg, bool show)` (L123): Sets if the specified background must be shown in this window.
  `affine_bg` affine_bg_ptr to show or hide.; `show` `true` if the specified background must be shown in this window, otherwise `false`.
- **set_show_bg** `void set_show_bg(const palette_bitmap_bg_ptr& palette_bitmap_bg, bool show)` (L130): Sets if the specified background must be shown in this window.
  `palette_bitmap_bg` palette_bitmap_bg_ptr to show or hide.; `show` `true` if the specified background must be shown in this window, otherwise `false`.
- **set_show_bg** `void set_show_bg(const sp_direct_bitmap_bg_ptr& sp_direct_bitmap_bg, bool show)` (L137): Sets if the specified background must be shown in this window.
  `sp_direct_bitmap_bg` sp_direct_bitmap_bg_ptr to show or hide.; `show` `true` if the specified background must be shown in this window, otherwise `false`.
- **set_show_bg** `void set_show_bg(const dp_direct_bitmap_bg_ptr& dp_direct_bitmap_bg, bool show)` (L144): Sets if the specified background must be shown in this window.
  `dp_direct_bitmap_bg` dp_direct_bitmap_bg_ptr to show or hide.; `show` `true` if the specified background must be shown in this window, otherwise `false`.
- **show_sprites** `bool show_sprites() const` (L149): Indicates if sprites are shown in this window.
- **set_show_sprites** `void set_show_sprites(bool show)` (L155): Sets if sprites must be shown in this window.
  `show` `true` if sprites must be shown in this window, otherwise `false`.
- **show_blending** `bool show_blending() const` (L160): Indicates if blending is shown in this window.
- **set_show_blending** `void set_show_blending(bool show)` (L166): Sets if blending must be shown in this window.
  `show` `true` if blending must be shown in this window, otherwise `false`.
- **show_all** `bool show_all() const` (L171): Indicates if sprites, backgrounds and blending are shown in this window.
- **set_show_all** `void set_show_all()` (L176): Show sprites, backgrounds and blending in this window.
- **show_nothing** `bool show_nothing() const` (L181): Indicates if sprites, backgrounds and blending are hidden in this window.
- **set_show_nothing** `void set_show_nothing()` (L186): Hide sprites, backgrounds and blending in this window.
- **restore** `void restore()` (L191): Restores this window to its initial state.
- **operator==** `friend bool operator==(const window& a, const window& b) = default` (L196): Default equal operator.
