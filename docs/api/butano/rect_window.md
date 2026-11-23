# Butano 21.10.1 — Rectangle windows (`rect_window`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [window](window.md)

## bn_rect_window.h

### `bn::rect_window` (class, bn_rect_window.h:31)

Rectangular window.

`class rect_window : public inside_window`

- **internal** `static rect_window internal()` (L38): Returns the closest window to the screen.
- **external** `static rect_window external()` (L46): Returns the closest window to the screen not covered by the internal window.
- **top** `fixed top() const` (L54): Returns the position of the top boundary of this window (relative to its camera, if it has one).
- **set_top** `void set_top(fixed top)` (L59): Sets the position of the top boundary of this window (relative to its camera, if it has one).
- **left** `fixed left() const` (L64): Returns the position of the left boundary of this window (relative to its camera, if it has one).
- **set_left** `void set_left(fixed left)` (L69): Sets the position of the left boundary of this window (relative to its camera, if it has one).
- **top_left** `const fixed_point& top_left() const` (L74): Returns the position of the top-left corner of this window (relative to its camera, if it has one).
- **set_top_left** `void set_top_left(fixed top, fixed left)` (L81): Sets the position of the top-left corner of this window (relative to its camera, if it has one).
  `top` Position of the top boundary of this window (relative to its camera, if it has one).; `left` Position of the left boundary of this window (relative to its camera, if it has one).
- **set_top_left** `void set_top_left(const fixed_point& top_left)` (L86): Sets the position of the top-left corner of this window (relative to its camera, if it has one).
- **bottom** `fixed bottom() const` (L91): Returns the position of the bottom boundary of this window (relative to its camera, if it has one).
- **set_bottom** `void set_bottom(fixed bottom)` (L96): Sets the position of the bottom boundary of this window (relative to its camera, if it has one).
- **right** `fixed right() const` (L101): Returns the position of the right boundary of this window (relative to its camera, if it has one).
- **set_right** `void set_right(fixed right)` (L106): Sets the position of the right boundary of this window (relative to its camera, if it has one).
- **bottom_right** `const fixed_point& bottom_right() const` (L111): Returns the position of the bottom-right corner of this window (relative to its camera, if it has one).
- **set_bottom_right** `void set_bottom_right(fixed bottom, fixed right)` (L118): Sets the position of the bottom-right corner of this window (relative to its camera, if it has one).
  `bottom` Position of the bottom boundary of this window (relative to its camera, if it has one).; `right` Position of the right boundary of this window (relative to its camera, if it has one).
- **set_bottom_right** `void set_bottom_right(const fixed_point& bottom_right)` (L123): Sets the position of the bottom-right corner of this window (relative to its camera, if it has one).
- **boundaries** `fixed_rect boundaries() const` (L128): Returns the boundaries of this window (relative to its camera, if it has one) as a fixed_rect.
- **top_left_boundaries** `top_left_fixed_rect top_left_boundaries() const` (L133): Returns the boundaries of this window (relative to its camera, if it has one) as a top_left_fixed_rect.
- **set_boundaries** `void set_boundaries(fixed top, fixed left, fixed bottom, fixed right)` (L142): Sets the boundaries of this window (relative to its camera, if it has one).
  `top` Position of the top boundary of this window (relative to its camera, if it has one).; `left` Position of the left boundary of this window (relative to its camera, if it has one).; `bottom` Position of the bottom boundary of this window (relative to its camera, if it has one).; `right` Position of the right boundary of this window (relative to its camera, if it has one).
- **set_boundaries** `void set_boundaries(const fixed_point& top_left, const fixed_point& bottom_right)` (L149): Sets the boundaries of this window (relative to its camera, if it has one).
  `top_left` Position of the top-left corner of this window (relative to its camera, if it has one).; `bottom_right` Position of the bottom-right corner of this window (relative to its camera, if it has one).
- **set_boundaries** `void set_boundaries(const fixed_rect& boundaries)` (L154): Sets the boundaries of this window (relative to its camera, if it has one).
- **set_boundaries** `void set_boundaries(const top_left_fixed_rect& boundaries)` (L159): Sets the boundaries of this window (relative to its camera, if it has one).
- **restore_boundaries** `void restore_boundaries()` (L164): Restores the boundaries of this window to its initial values.
- **camera** `const optional<camera_ptr>& camera() const` (L169): Returns the camera_ptr attached to this rect window (if any).
- **set_camera** `void set_camera(const camera_ptr& camera)` (L175): Sets the camera_ptr attached to this rect window.
  `camera` camera_ptr to copy to this rect window.
- **set_camera** `void set_camera(camera_ptr&& camera)` (L181): Sets the camera_ptr attached to this rect window.
  `camera` camera_ptr to move to this rect window.
- **set_camera** `void set_camera(const optional<camera_ptr>& camera)` (L187): Sets or removes the camera_ptr attached to this rect window.
  `camera` Optional camera_ptr to copy to this rect window.
- **set_camera** `void set_camera(optional<camera_ptr>&& camera)` (L193): Sets or removes the camera_ptr attached to this rect window.
  `camera` Optional camera_ptr to move to this rect window.
- **remove_camera** `void remove_camera()` (L198): Removes the camera_ptr attached to this rect window (if any).
