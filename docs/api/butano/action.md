# Butano 21.10.1 — Actions (`action`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_affine_bg_actions.h

### `bn::affine_bg_visible_manager` (class, bn_affine_bg_actions.h:31)

Manages if an affine_bg_ptr must be committed to the GBA or not.

`class affine_bg_visible_manager`

- **get** `static bool get(const affine_bg_ptr& bg)` (L38): Indicates if the given affine_bg_ptr is committed to the GBA or not.
- **set** `static void set(bool visible, affine_bg_ptr& bg)` (L46): Sets if the given affine_bg_ptr must be committed to the GBA or not.
### `bn::affine_bg_visible_toggle_action` (class, bn_affine_bg_actions.h:60)

Toggles if an affine_bg_ptr must be committed to the GBA or not when the action is updated a given number of times.

`class affine_bg_visible_toggle_action : public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_visible_manager>`

- **affine_bg_visible_toggle_action** `affine_bg_visible_toggle_action(const affine_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L71): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given affine_bg_ptr must be committed to the GBA or not.
- **affine_bg_visible_toggle_action** `affine_bg_visible_toggle_action(affine_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L82): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given affine_bg_ptr must be committed to the GBA or not.
- **bg** `const affine_bg_ptr& bg() const` (L90): Returns the affine_bg_ptr to modify.
### `bn::affine_bg_position_manager` (class, bn_affine_bg_actions.h:105)

Manages the position of an affine_bg_ptr.

`class affine_bg_position_manager`

- **get** `static const fixed_point& get(const affine_bg_ptr& bg)` (L112): Returns the position of the given affine_bg_ptr.
- **set** `static void set(const fixed_point& position, affine_bg_ptr& bg)` (L120): Sets the position of the given affine_bg_ptr.
### `bn::affine_bg_move_by_action` (class, bn_affine_bg_actions.h:133)

Modifies the position of an affine_bg_ptr by a given delta.

`class affine_bg_move_by_action : public by_value_template_action<affine_bg_ptr, fixed_point, affine_bg_position_manager>`

- **affine_bg_move_by_action** `affine_bg_move_by_action(const affine_bg_ptr& bg, fixed delta_x, fixed delta_y) : by_value_template_action(bg, fixed_point(delta_x, delta_y))` (L146): Constructor.
  `bg` affine_bg_ptr to copy.; `delta_x` How much to add to the horizontal position of the given affine_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the given affine_bg_ptr when the action is updated.
- **affine_bg_move_by_action** `affine_bg_move_by_action(affine_bg_ptr&& bg, fixed delta_x, fixed delta_y) : by_value_template_action(move(bg), fixed_point(delta_x, delta_y))` (L159): Constructor.
  `bg` affine_bg_ptr to move.; `delta_x` How much to add to the horizontal position of the given affine_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the given affine_bg_ptr when the action is updated.
- **affine_bg_move_by_action** `affine_bg_move_by_action(const affine_bg_ptr& bg, const fixed_point& delta_position) : by_value_template_action(bg, delta_position)` (L169): Constructor.
  `bg` affine_bg_ptr to copy.; `delta_position` How much to add to the position of the given affine_bg_ptr when the action is updated.
- **affine_bg_move_by_action** `affine_bg_move_by_action(affine_bg_ptr&& bg, const fixed_point& delta_position) : by_value_template_action(move(bg), delta_position)` (L179): Constructor.
  `bg` affine_bg_ptr to move.; `delta_position` How much to add to the position of the given affine_bg_ptr when the action is updated.
- **bg** `const affine_bg_ptr& bg() const` (L187): Returns the affine_bg_ptr to modify.
- **delta_position** `const fixed_point& delta_position() const` (L195): Returns how much to add to the position of the given affine_bg_ptr when the action is updated.
### `bn::affine_bg_move_to_action` (class, bn_affine_bg_actions.h:208)

Modifies the position of an affine_bg_ptr until it has a given state.

`class affine_bg_move_to_action : public to_value_template_action<affine_bg_ptr, fixed_point, affine_bg_position_manager>`

- **affine_bg_move_to_action** `affine_bg_move_to_action(const affine_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L223): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the given affine_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the given affine_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the given affine_bg_ptr when the action is updated duration_updates times.
- **affine_bg_move_to_action** `affine_bg_move_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L238): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the given affine_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the given affine_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the given affine_bg_ptr when the action is updated duration_updates times.
- **affine_bg_move_to_action** `affine_bg_move_to_action(const affine_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(bg, duration_updates, final_position)` (L251): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the given affine_bg_ptr is equal to final_position.; `final_position` Position of the given affine_bg_ptr when the action is updated duration_updates times.
- **affine_bg_move_to_action** `affine_bg_move_to_action(affine_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(move(bg), duration_updates, final_position)` (L263): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the given affine_bg_ptr is equal to final_position.; `final_position` Position of the given affine_bg_ptr when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L271): Returns the affine_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L279): Returns the position of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_move_loop_action` (class, bn_affine_bg_actions.h:293)

Modifies the position of an affine_bg_ptr from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class affine_bg_move_loop_action : public loop_value_template_action<affine_bg_ptr, fixed_point, affine_bg_position_manager>`

- **affine_bg_move_loop_action** `affine_bg_move_loop_action(const affine_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L308): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_bg_move_loop_action** `affine_bg_move_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L323): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_bg_move_loop_action** `affine_bg_move_loop_action(const affine_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(bg, duration_updates, final_position)` (L336): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_bg_move_loop_action** `affine_bg_move_loop_action(affine_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(move(bg), duration_updates, final_position)` (L349): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const affine_bg_ptr& bg() const` (L357): Returns the affine_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L366): When the position of the given affine_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::affine_bg_move_toggle_action` (class, bn_affine_bg_actions.h:379)

Changes the position of an affine_bg_ptr when the action is updated a given number of times.

`class affine_bg_move_toggle_action : public toggle_value_template_action<affine_bg_ptr, fixed_point, affine_bg_position_manager>`

- **affine_bg_move_toggle_action** `affine_bg_move_toggle_action(const affine_bg_ptr& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(bg, duration_updates, fixed_point(new_x, new_y))` (L392): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the given affine_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **affine_bg_move_toggle_action** `affine_bg_move_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(move(bg), duration_updates, fixed_point(new_x, new_y))` (L405): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the given affine_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **affine_bg_move_toggle_action** `affine_bg_move_toggle_action(const affine_bg_ptr& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(bg, duration_updates, new_position)` (L417): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the given affine_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **affine_bg_move_toggle_action** `affine_bg_move_toggle_action(affine_bg_ptr&& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(move(bg), duration_updates, new_position)` (L429): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the given affine_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L437): Returns the affine_bg_ptr to modify.
- **new_position** `const fixed_point& new_position() const` (L445): Returns the position of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_top_left_position_manager` (class, bn_affine_bg_actions.h:460)

Manages the top-left position of an affine_bg_ptr.

`class affine_bg_top_left_position_manager`

- **get** `static fixed_point get(const affine_bg_ptr& bg)` (L467): Returns the top-left position of the given affine_bg_ptr.
- **set** `static void set(const fixed_point& top_left_position, affine_bg_ptr& bg)` (L475): Sets the top-left position of the given affine_bg_ptr.
### `bn::affine_bg_top_left_move_to_action` (class, bn_affine_bg_actions.h:488)

Modifies the top-left position of an affine_bg_ptr until it has a given state.

`class affine_bg_top_left_move_to_action : public to_value_template_action<affine_bg_ptr, fixed_point, affine_bg_top_left_position_manager>`

- **affine_bg_top_left_move_to_action** `affine_bg_top_left_move_to_action( const affine_bg_ptr& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : to_value_template_action(bg, duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L503): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the top-left position of the given affine_bg_ptr is equal to (final_x, final_y).; `final_top_left_x` Horizontal top-left position of the given affine_bg_ptr when the action is updated duration_updates times.; `final_top_left_y` Vertical top-left position of the given affine_bg_ptr when the action is updated duration_updates times.
- **affine_bg_top_left_move_to_action** `affine_bg_top_left_move_to_action( affine_bg_ptr&& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : to_value_template_action(move(bg), duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L519): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the top-left position of the given affine_bg_ptr is equal to (final_x, final_y).; `final_top_left_x` Horizontal top-left position of the given affine_bg_ptr when the action is updated duration_updates times.; `final_top_left_y` Vertical top-left position of the given affine_bg_ptr when the action is updated duration_updates times.
- **affine_bg_top_left_move_to_action** `affine_bg_top_left_move_to_action( const affine_bg_ptr& bg, int duration_updates, const fixed_point& final_top_left_position) : to_value_template_action(bg, duration_updates, final_top_left_position)` (L533): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the top-left position of the given affine_bg_ptr is equal to final_top_left_position.; `final_top_left_position` Top-left position of the given affine_bg_ptr when the action is updated duration_updates times.
- **affine_bg_top_left_move_to_action** `affine_bg_top_left_move_to_action( affine_bg_ptr&& bg, int duration_updates, const fixed_point& final_top_left_position) : to_value_template_action(move(bg), duration_updates, final_top_left_position)` (L547): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the top-left position of the given affine_bg_ptr is equal to final_top_left_position.; `final_top_left_position` Top-left position of the given affine_bg_ptr when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L556): Returns the affine_bg_ptr to modify.
- **final_top_left_position** `const fixed_point& final_top_left_position() const` (L565): Returns the top-left position of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_top_left_move_loop_action` (class, bn_affine_bg_actions.h:579)

Modifies the top-left position of an affine_bg_ptr from a minimum to a maximum. When the top-left position is equal to the given final state, it goes back to its initial state and vice versa.

`class affine_bg_top_left_move_loop_action : public loop_value_template_action<affine_bg_ptr, fixed_point, affine_bg_top_left_position_manager>`

- **affine_bg_top_left_move_loop_action** `affine_bg_top_left_move_loop_action( const affine_bg_ptr& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : loop_value_template_action(bg, duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L594): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_x` When the horizontal top-left position of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_top_left_y` When the vertical top-left position of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_bg_top_left_move_loop_action** `affine_bg_top_left_move_loop_action( affine_bg_ptr&& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : loop_value_template_action(move(bg), duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L610): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_x` When the horizontal top-left position of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_top_left_y` When the vertical top-left position of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_bg_top_left_move_loop_action** `affine_bg_top_left_move_loop_action( const affine_bg_ptr& bg, int duration_updates, const fixed_point& final_top_left_position) : loop_value_template_action(bg, duration_updates, final_top_left_position)` (L624): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_position` When the top-left position of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_bg_top_left_move_loop_action** `affine_bg_top_left_move_loop_action( affine_bg_ptr&& bg, int duration_updates, const fixed_point& final_top_left_position) : loop_value_template_action(move(bg), duration_updates, final_top_left_position)` (L638): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_position` When the top-left position of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const affine_bg_ptr& bg() const` (L647): Returns the affine_bg_ptr to modify.
- **final_top_left_position** `const fixed_point& final_top_left_position() const` (L656): When the top-left position of the given affine_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::affine_bg_top_left_move_toggle_action` (class, bn_affine_bg_actions.h:669)

Changes the top-left position of an affine_bg_ptr when the action is updated a given number of times.

`class affine_bg_top_left_move_toggle_action : public toggle_value_template_action<affine_bg_ptr, fixed_point, affine_bg_top_left_position_manager>`

- **affine_bg_top_left_move_toggle_action** `affine_bg_top_left_move_toggle_action( const affine_bg_ptr& bg, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) : toggle_value_template_action(bg, duration_updates, fixed_point(new_top_left_x, new_top_left_y))` (L682): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the top-left position of the given affine_bg_ptr.; `new_top_left_x` New horizontal top-left position when the action is updated duration_updates times.; `new_top_left_y` New vertical top-left position when the action is updated duration_updates times.
- **affine_bg_top_left_move_toggle_action** `affine_bg_top_left_move_toggle_action( affine_bg_ptr&& bg, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) : toggle_value_template_action(move(bg), duration_updates, fixed_point(new_top_left_x, new_top_left_y))` (L696): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the top-left position of the given affine_bg_ptr.; `new_top_left_x` New horizontal top-left position when the action is updated duration_updates times.; `new_top_left_y` New vertical top-left position when the action is updated duration_updates times.
- **affine_bg_top_left_move_toggle_action** `affine_bg_top_left_move_toggle_action( const affine_bg_ptr& bg, int duration_updates, const fixed_point& new_top_left_position) : toggle_value_template_action(bg, duration_updates, new_top_left_position)` (L709): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the top-left position of the given affine_bg_ptr.; `new_top_left_position` New top-left position when the action is updated duration_updates times.
- **affine_bg_top_left_move_toggle_action** `affine_bg_top_left_move_toggle_action( affine_bg_ptr&& bg, int duration_updates, const fixed_point& new_top_left_position) : toggle_value_template_action(move(bg), duration_updates, new_top_left_position)` (L722): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the top-left position of the given affine_bg_ptr.; `new_top_left_position` New top-left position when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L731): Returns the affine_bg_ptr to modify.
- **new_top_left_position** `const fixed_point& new_top_left_position() const` (L740): Returns the top-left position of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_rotation_manager` (class, bn_affine_bg_actions.h:755)

Manages the rotation angle of an affine_bg_ptr.

`class affine_bg_rotation_manager`

- **get** `static fixed get(const affine_bg_ptr& bg)` (L762): Returns the rotation angle of the given affine_bg_ptr.
- **set** `static void set(fixed rotation_angle, affine_bg_ptr& bg)` (L772): Sets the rotation angle of the given affine_bg_ptr.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].; `bg` affine_bg_ptr to modify.
### `bn::affine_bg_rotate_by_action` (class, bn_affine_bg_actions.h:786)

Modifies the rotation angle of an affine_bg_ptr by delta_rotation_angle. When the rotation angle is over 360, it goes back to 0 and vice versa.

`class affine_bg_rotate_by_action : public cyclic_by_value_template_action<affine_bg_ptr, fixed, affine_bg_rotation_manager>`

- **affine_bg_rotate_by_action** `affine_bg_rotate_by_action(const affine_bg_ptr& bg, fixed delta_rotation_angle) : cyclic_by_value_template_action(bg, delta_rotation_angle, 0, 360)` (L799): Constructor.
  `bg` affine_bg_ptr to copy.; `delta_rotation_angle` How many degrees to add to the rotation angle of the given affine_bg_ptr when the action is updated.
  This rotation angle must be in the range [0..360].
- **affine_bg_rotate_by_action** `affine_bg_rotate_by_action(affine_bg_ptr&& bg, fixed delta_rotation_angle) : cyclic_by_value_template_action(move(bg), delta_rotation_angle, 0, 360)` (L814): Constructor.
  `bg` affine_bg_ptr to move.; `delta_rotation_angle` How many degrees to add to the rotation angle of the given affine_bg_ptr when the action is updated.
  This rotation angle must be in the range [0..360].
- **bg** `const affine_bg_ptr& bg() const` (L824): Returns the affine_bg_ptr to modify.
- **delta_rotation_angle** `fixed delta_rotation_angle() const` (L833): Returns how many degrees to add to the rotation angle of the given affine_bg_ptr when the action is updated.
### `bn::affine_bg_rotate_to_action` (class, bn_affine_bg_actions.h:846)

Modifies the rotation angle of an affine_bg_ptr until it has a given state.

`class affine_bg_rotate_to_action : public to_value_template_action<affine_bg_ptr, fixed, affine_bg_rotation_manager>`

- **affine_bg_rotate_to_action** `affine_bg_rotate_to_action(const affine_bg_ptr& bg, int duration_updates, fixed final_rotation_angle) : to_value_template_action(bg, duration_updates, final_rotation_angle)` (L860): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the rotation angle of the given affine_bg_ptr is equal to final_rotation_angle.; `final_rotation_angle` Rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **affine_bg_rotate_to_action** `affine_bg_rotate_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_rotation_angle) : to_value_template_action(move(bg), duration_updates, final_rotation_angle)` (L876): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the rotation angle of the given affine_bg_ptr is equal to final_rotation_angle.; `final_rotation_angle` Rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **bg** `const affine_bg_ptr& bg() const` (L886): Returns the affine_bg_ptr to modify.
- **final_rotation_angle** `fixed final_rotation_angle() const` (L895): Returns the rotation angle of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_rotate_loop_action` (class, bn_affine_bg_actions.h:909)

Modifies the rotation angle of an affine_bg_ptr from a minimum to a maximum. When the rotation angle is equal to the given final state, it goes back to its initial state and vice versa.

`class affine_bg_rotate_loop_action : public loop_value_template_action<affine_bg_ptr, fixed, affine_bg_rotation_manager>`

- **affine_bg_rotate_loop_action** `affine_bg_rotate_loop_action(const affine_bg_ptr& bg, int duration_updates, fixed final_rotation_angle) : loop_value_template_action(bg, duration_updates, final_rotation_angle)` (L924): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the rotation angle delta.; `final_rotation_angle` When the rotation angle of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
  This rotation angle must be in the range [0..360].
- **affine_bg_rotate_loop_action** `affine_bg_rotate_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_rotation_angle) : loop_value_template_action(move(bg), duration_updates, final_rotation_angle)` (L941): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the rotation angle delta.; `final_rotation_angle` When the rotation angle of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
  This rotation angle must be in the range [0..360].
- **bg** `const affine_bg_ptr& bg() const` (L951): Returns the affine_bg_ptr to modify.
- **final_rotation_angle** `fixed final_rotation_angle() const` (L960): When the rotation angle of the given affine_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::affine_bg_rotate_toggle_action` (class, bn_affine_bg_actions.h:973)

Changes the rotation angle of an affine_bg_ptr when the action is updated a given number of times.

`class affine_bg_rotate_toggle_action : public toggle_value_template_action<affine_bg_ptr, fixed, affine_bg_rotation_manager>`

- **affine_bg_rotate_toggle_action** `affine_bg_rotate_toggle_action(const affine_bg_ptr& bg, int duration_updates, fixed new_rotation_angle) : toggle_value_template_action(bg, duration_updates, new_rotation_angle)` (L987): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the rotation angle of the given affine_bg_ptr.; `new_rotation_angle` New rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **affine_bg_rotate_toggle_action** `affine_bg_rotate_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_rotation_angle) : toggle_value_template_action(move(bg), duration_updates, new_rotation_angle)` (L1003): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the rotation angle of the given affine_bg_ptr.; `new_rotation_angle` New rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **bg** `const affine_bg_ptr& bg() const` (L1013): Returns the affine_bg_ptr to modify.
- **new_rotation_angle** `fixed new_rotation_angle() const` (L1022): Returns the rotation angle of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_horizontal_scale_manager` (class, bn_affine_bg_actions.h:1037)

Manages the horizontal scale of an affine_bg_ptr.

`class affine_bg_horizontal_scale_manager`

- **get** `static fixed get(const affine_bg_ptr& bg)` (L1044): Returns the horizontal scale of the given affine_bg_ptr.
- **set** `static void set(fixed horizontal_scale, affine_bg_ptr& bg)` (L1052): Sets the horizontal scale of the given affine_bg_ptr.
### `bn::affine_bg_horizontal_scale_to_action` (class, bn_affine_bg_actions.h:1065)

Modifies the horizontal scale of an affine_bg_ptr until it has a given state.

`class affine_bg_horizontal_scale_to_action : public to_value_template_action<affine_bg_ptr, fixed, affine_bg_horizontal_scale_manager>`

- **affine_bg_horizontal_scale_to_action** `affine_bg_horizontal_scale_to_action(const affine_bg_ptr& bg, int duration_updates, fixed final_horizontal_scale) : to_value_template_action(bg, duration_updates, final_horizontal_scale)` (L1077): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the horizontal scale of the given affine_bg_ptr is equal to final_horizontal_scale.; `final_horizontal_scale` Horizontal scale when the action is updated duration_updates times.
- **affine_bg_horizontal_scale_to_action** `affine_bg_horizontal_scale_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_horizontal_scale) : to_value_template_action(move(bg), duration_updates, final_horizontal_scale)` (L1091): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the horizontal scale of the given affine_bg_ptr is equal to final_horizontal_scale.; `final_horizontal_scale` Horizontal scale when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L1100): Returns the affine_bg_ptr to modify.
- **final_horizontal_scale** `fixed final_horizontal_scale() const` (L1109): Returns the horizontal scale of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_horizontal_scale_loop_action` (class, bn_affine_bg_actions.h:1123)

Modifies the horizontal scale of an affine_bg_ptr from a minimum to a maximum. When the horizontal scale is equal to the given final state, it goes back to its initial state and vice versa.

`class affine_bg_horizontal_scale_loop_action : public loop_value_template_action<affine_bg_ptr, fixed, affine_bg_horizontal_scale_manager>`

- **affine_bg_horizontal_scale_loop_action** `affine_bg_horizontal_scale_loop_action(const affine_bg_ptr& bg, int duration_updates, fixed final_horizontal_scale) : loop_value_template_action(bg, duration_updates, final_horizontal_scale)` (L1136): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal scale delta.; `final_horizontal_scale` When the horizontal scale of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_bg_horizontal_scale_loop_action** `affine_bg_horizontal_scale_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_horizontal_scale) : loop_value_template_action(move(bg), duration_updates, final_horizontal_scale)` (L1151): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal scale delta.; `final_horizontal_scale` When the horizontal scale of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const affine_bg_ptr& bg() const` (L1161): Returns the affine_bg_ptr to modify.
- **final_horizontal_scale** `fixed final_horizontal_scale() const` (L1170): When the horizontal scale of the given affine_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::affine_bg_horizontal_scale_toggle_action` (class, bn_affine_bg_actions.h:1184)

Changes the horizontal scale of an affine_bg_ptr when the action is updated a given number of times.

`class affine_bg_horizontal_scale_toggle_action : public toggle_value_template_action<affine_bg_ptr, fixed, affine_bg_horizontal_scale_manager>`

- **affine_bg_horizontal_scale_toggle_action** `affine_bg_horizontal_scale_toggle_action(const affine_bg_ptr& bg, int duration_updates, fixed new_horizontal_scale) : toggle_value_template_action(bg, duration_updates, new_horizontal_scale)` (L1196): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the horizontal scale of the given affine_bg_ptr.; `new_horizontal_scale` New horizontal scale when the action is updated duration_updates times.
- **affine_bg_horizontal_scale_toggle_action** `affine_bg_horizontal_scale_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_horizontal_scale) : toggle_value_template_action(move(bg), duration_updates, new_horizontal_scale)` (L1210): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the horizontal scale of the given affine_bg_ptr.; `new_horizontal_scale` New horizontal scale when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L1220): Returns the affine_bg_ptr to modify.
- **new_horizontal_scale** `fixed new_horizontal_scale() const` (L1229): Returns the horizontal scale of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_vertical_scale_manager` (class, bn_affine_bg_actions.h:1244)

Manages the vertical scale of an affine_bg_ptr.

`class affine_bg_vertical_scale_manager`

- **get** `static fixed get(const affine_bg_ptr& bg)` (L1251): Returns the vertical scale of the given affine_bg_ptr.
- **set** `static void set(fixed vertical_scale, affine_bg_ptr& bg)` (L1259): Sets the vertical scale of the given affine_bg_ptr.
### `bn::affine_bg_vertical_scale_to_action` (class, bn_affine_bg_actions.h:1272)

Modifies the vertical scale of an affine_bg_ptr until it has a given state.

`class affine_bg_vertical_scale_to_action : public to_value_template_action<affine_bg_ptr, fixed, affine_bg_vertical_scale_manager>`

- **affine_bg_vertical_scale_to_action** `affine_bg_vertical_scale_to_action(const affine_bg_ptr& bg, int duration_updates, fixed final_vertical_scale) : to_value_template_action(bg, duration_updates, final_vertical_scale)` (L1284): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the vertical scale of the given affine_bg_ptr is equal to final_vertical_scale.; `final_vertical_scale` Vertical scale when the action is updated duration_updates times.
- **affine_bg_vertical_scale_to_action** `affine_bg_vertical_scale_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_vertical_scale) : to_value_template_action(move(bg), duration_updates, final_vertical_scale)` (L1298): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the vertical scale of the given affine_bg_ptr is equal to final_vertical_scale.; `final_vertical_scale` Vertical scale when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L1307): Returns the affine_bg_ptr to modify.
- **final_vertical_scale** `fixed final_vertical_scale() const` (L1316): Returns the vertical scale of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_vertical_scale_loop_action` (class, bn_affine_bg_actions.h:1330)

Modifies the vertical scale of an affine_bg_ptr from a minimum to a maximum. When the vertical scale is equal to the given final state, it goes back to its initial state and vice versa.

`class affine_bg_vertical_scale_loop_action : public loop_value_template_action<affine_bg_ptr, fixed, affine_bg_vertical_scale_manager>`

- **affine_bg_vertical_scale_loop_action** `affine_bg_vertical_scale_loop_action(const affine_bg_ptr& bg, int duration_updates, fixed final_vertical_scale) : loop_value_template_action(bg, duration_updates, final_vertical_scale)` (L1343): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical scale delta.; `final_vertical_scale` When the vertical scale of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_bg_vertical_scale_loop_action** `affine_bg_vertical_scale_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_vertical_scale) : loop_value_template_action(move(bg), duration_updates, final_vertical_scale)` (L1358): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical scale delta.; `final_vertical_scale` When the vertical scale of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const affine_bg_ptr& bg() const` (L1367): Returns the affine_bg_ptr to modify.
- **final_vertical_scale** `fixed final_vertical_scale() const` (L1376): When the vertical scale of the given affine_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::affine_bg_vertical_scale_toggle_action` (class, bn_affine_bg_actions.h:1390)

Changes the vertical scale of an affine_bg_ptr when the action is updated a given number of times.

`class affine_bg_vertical_scale_toggle_action : public toggle_value_template_action<affine_bg_ptr, fixed, affine_bg_vertical_scale_manager>`

- **affine_bg_vertical_scale_toggle_action** `affine_bg_vertical_scale_toggle_action(const affine_bg_ptr& bg, int duration_updates, fixed new_vertical_scale) : toggle_value_template_action(bg, duration_updates, new_vertical_scale)` (L1402): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the vertical scale of the given affine_bg_ptr.; `new_vertical_scale` New vertical scale when the action is updated duration_updates times.
- **affine_bg_vertical_scale_toggle_action** `affine_bg_vertical_scale_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_vertical_scale) : toggle_value_template_action(move(bg), duration_updates, new_vertical_scale)` (L1416): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the vertical scale of the given affine_bg_ptr.; `new_vertical_scale` New vertical scale when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L1425): Returns the affine_bg_ptr to modify.
- **new_vertical_scale** `fixed new_vertical_scale() const` (L1434): Returns the vertical scale of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_scale_manager` (class, bn_affine_bg_actions.h:1449)

Manages the scale of an affine_bg_ptr.

`class affine_bg_scale_manager`

- **get** `static fixed get(const affine_bg_ptr& bg)` (L1456): Returns the horizontal scale of the given affine_bg_ptr.
- **set** `static void set(fixed scale, affine_bg_ptr& bg)` (L1464): Sets the scale of the given affine_bg_ptr.
### `bn::affine_bg_scale_to_action` (class, bn_affine_bg_actions.h:1477)

Modifies the scale of an affine_bg_ptr until it has a given state.

`class affine_bg_scale_to_action : public to_value_template_action<affine_bg_ptr, fixed, affine_bg_scale_manager>`

- **affine_bg_scale_to_action** `affine_bg_scale_to_action(const affine_bg_ptr& bg, int duration_updates, fixed final_scale) : to_value_template_action(bg, duration_updates, final_scale)` (L1488): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the scale of the given affine_bg_ptr is equal to final_scale.; `final_scale` scale when the action is updated duration_updates times.
- **affine_bg_scale_to_action** `affine_bg_scale_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_scale) : to_value_template_action(move(bg), duration_updates, final_scale)` (L1501): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the scale of the given affine_bg_ptr is equal to final_scale.; `final_scale` Scale when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L1510): Returns the affine_bg_ptr to modify.
- **final_scale** `fixed final_scale() const` (L1519): Returns the scale of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_scale_loop_action` (class, bn_affine_bg_actions.h:1533)

Modifies the scale of an affine_bg_ptr from a minimum to a maximum. When the scale is equal to the given final state, it goes back to its initial state and vice versa.

`class affine_bg_scale_loop_action : public loop_value_template_action<affine_bg_ptr, fixed, affine_bg_scale_manager>`

- **affine_bg_scale_loop_action** `affine_bg_scale_loop_action(const affine_bg_ptr& bg, int duration_updates, fixed final_scale) : loop_value_template_action(bg, duration_updates, final_scale)` (L1546): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the scale delta.; `final_scale` When the scale of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_bg_scale_loop_action** `affine_bg_scale_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_scale) : loop_value_template_action(move(bg), duration_updates, final_scale)` (L1560): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the scale delta.; `final_scale` When the scale of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const affine_bg_ptr& bg() const` (L1569): Returns the affine_bg_ptr to modify.
- **final_scale** `fixed final_scale() const` (L1578): When the scale of the given affine_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::affine_bg_scale_toggle_action` (class, bn_affine_bg_actions.h:1592)

Changes the scale of an affine_bg_ptr when the action is updated a given number of times.

`class affine_bg_scale_toggle_action : public toggle_value_template_action<affine_bg_ptr, fixed, affine_bg_scale_manager>`

- **affine_bg_scale_toggle_action** `affine_bg_scale_toggle_action(const affine_bg_ptr& bg, int duration_updates, fixed new_scale) : toggle_value_template_action(bg, duration_updates, new_scale)` (L1604): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the scale of the given affine_bg_ptr.; `new_scale` New scale when the action is updated duration_updates times.
- **affine_bg_scale_toggle_action** `affine_bg_scale_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_scale) : toggle_value_template_action(move(bg), duration_updates, new_scale)` (L1617): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the scale of the given affine_bg_ptr.; `new_scale` New scale when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L1626): Returns the affine_bg_ptr to modify.
- **new_scale** `fixed new_scale() const` (L1635): Returns the scale of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_horizontal_shear_manager` (class, bn_affine_bg_actions.h:1650)

Manages the horizontal shear of an affine_bg_ptr.

`class affine_bg_horizontal_shear_manager`

- **get** `static fixed get(const affine_bg_ptr& bg)` (L1657): Returns the horizontal shear of the given affine_bg_ptr.
- **set** `static void set(fixed horizontal_shear, affine_bg_ptr& bg)` (L1665): Sets the horizontal shear of the given affine_bg_ptr.
### `bn::affine_bg_horizontal_shear_to_action` (class, bn_affine_bg_actions.h:1678)

Modifies the horizontal shear of an affine_bg_ptr until it has a given state.

`class affine_bg_horizontal_shear_to_action : public to_value_template_action<affine_bg_ptr, fixed, affine_bg_horizontal_shear_manager>`

- **affine_bg_horizontal_shear_to_action** `affine_bg_horizontal_shear_to_action(const affine_bg_ptr& bg, int duration_updates, fixed final_horizontal_shear) : to_value_template_action(bg, duration_updates, final_horizontal_shear)` (L1690): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the horizontal shear of the given affine_bg_ptr is equal to final_horizontal_shear.; `final_horizontal_shear` Horizontal shear when the action is updated duration_updates times.
- **affine_bg_horizontal_shear_to_action** `affine_bg_horizontal_shear_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_horizontal_shear) : to_value_template_action(move(bg), duration_updates, final_horizontal_shear)` (L1703): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the horizontal shear of the given affine_bg_ptr is equal to final_horizontal_shear.; `final_horizontal_shear` Horizontal shear when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L1711): Returns the affine_bg_ptr to modify.
- **final_horizontal_shear** `fixed final_horizontal_shear() const` (L1720): Returns the horizontal shear of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_horizontal_shear_loop_action` (class, bn_affine_bg_actions.h:1734)

Modifies the horizontal shear of an affine_bg_ptr from a minimum to a maximum. When the horizontal shear is equal to the given final state, it goes back to its initial state and vice versa.

`class affine_bg_horizontal_shear_loop_action : public loop_value_template_action<affine_bg_ptr, fixed, affine_bg_horizontal_shear_manager>`

- **affine_bg_horizontal_shear_loop_action** `affine_bg_horizontal_shear_loop_action(const affine_bg_ptr& bg, int duration_updates, fixed final_horizontal_shear) : loop_value_template_action(bg, duration_updates, final_horizontal_shear)` (L1747): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal shear delta.; `final_horizontal_shear` When the horizontal shear of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_bg_horizontal_shear_loop_action** `affine_bg_horizontal_shear_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_horizontal_shear) : loop_value_template_action(move(bg), duration_updates, final_horizontal_shear)` (L1761): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal shear delta.; `final_horizontal_shear` When the horizontal shear of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const affine_bg_ptr& bg() const` (L1770): Returns the affine_bg_ptr to modify.
- **final_horizontal_shear** `fixed final_horizontal_shear() const` (L1779): When the horizontal shear of the given affine_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::affine_bg_horizontal_shear_toggle_action` (class, bn_affine_bg_actions.h:1793)

Changes the horizontal shear of an affine_bg_ptr when the action is updated a given number of times.

`class affine_bg_horizontal_shear_toggle_action : public toggle_value_template_action<affine_bg_ptr, fixed, affine_bg_horizontal_shear_manager>`

- **affine_bg_horizontal_shear_toggle_action** `affine_bg_horizontal_shear_toggle_action(const affine_bg_ptr& bg, int duration_updates, fixed new_horizontal_shear) : toggle_value_template_action(bg, duration_updates, new_horizontal_shear)` (L1805): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the horizontal shear of the given affine_bg_ptr.; `new_horizontal_shear` New horizontal shear when the action is updated duration_updates times.
- **affine_bg_horizontal_shear_toggle_action** `affine_bg_horizontal_shear_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_horizontal_shear) : toggle_value_template_action(move(bg), duration_updates, new_horizontal_shear)` (L1818): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the horizontal shear of the given affine_bg_ptr.; `new_horizontal_shear` New horizontal shear when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L1827): Returns the affine_bg_ptr to modify.
- **new_horizontal_shear** `fixed new_horizontal_shear() const` (L1836): Returns the horizontal shear of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_vertical_shear_manager` (class, bn_affine_bg_actions.h:1851)

Manages the vertical shear of an affine_bg_ptr.

`class affine_bg_vertical_shear_manager`

- **get** `static fixed get(const affine_bg_ptr& bg)` (L1858): Returns the vertical shear of the given affine_bg_ptr.
- **set** `static void set(fixed vertical_shear, affine_bg_ptr& bg)` (L1866): Sets the vertical shear of the given affine_bg_ptr.
### `bn::affine_bg_vertical_shear_to_action` (class, bn_affine_bg_actions.h:1879)

Modifies the vertical shear of an affine_bg_ptr until it has a given state.

`class affine_bg_vertical_shear_to_action : public to_value_template_action<affine_bg_ptr, fixed, affine_bg_vertical_shear_manager>`

- **affine_bg_vertical_shear_to_action** `affine_bg_vertical_shear_to_action(const affine_bg_ptr& bg, int duration_updates, fixed final_vertical_shear) : to_value_template_action(bg, duration_updates, final_vertical_shear)` (L1891): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the vertical shear of the given affine_bg_ptr is equal to final_vertical_shear.; `final_vertical_shear` Vertical shear when the action is updated duration_updates times.
- **affine_bg_vertical_shear_to_action** `affine_bg_vertical_shear_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_vertical_shear) : to_value_template_action(move(bg), duration_updates, final_vertical_shear)` (L1904): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the vertical shear of the given affine_bg_ptr is equal to final_vertical_shear.; `final_vertical_shear` Vertical shear when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L1912): Returns the affine_bg_ptr to modify.
- **final_vertical_shear** `fixed final_vertical_shear() const` (L1921): Returns the vertical shear of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_vertical_shear_loop_action` (class, bn_affine_bg_actions.h:1935)

Modifies the vertical shear of an affine_bg_ptr from a minimum to a maximum. When the vertical shear is equal to the given final state, it goes back to its initial state and vice versa.

`class affine_bg_vertical_shear_loop_action : public loop_value_template_action<affine_bg_ptr, fixed, affine_bg_vertical_shear_manager>`

- **affine_bg_vertical_shear_loop_action** `affine_bg_vertical_shear_loop_action(const affine_bg_ptr& bg, int duration_updates, fixed final_vertical_shear) : loop_value_template_action(bg, duration_updates, final_vertical_shear)` (L1948): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical shear delta.; `final_vertical_shear` When the vertical shear of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_bg_vertical_shear_loop_action** `affine_bg_vertical_shear_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_vertical_shear) : loop_value_template_action(move(bg), duration_updates, final_vertical_shear)` (L1962): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical shear delta.; `final_vertical_shear` When the vertical shear of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const affine_bg_ptr& bg() const` (L1970): Returns the affine_bg_ptr to modify.
- **final_vertical_shear** `fixed final_vertical_shear() const` (L1979): When the vertical shear of the given affine_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::affine_bg_vertical_shear_toggle_action` (class, bn_affine_bg_actions.h:1993)

Changes the vertical shear of an affine_bg_ptr when the action is updated a given number of times.

`class affine_bg_vertical_shear_toggle_action : public toggle_value_template_action<affine_bg_ptr, fixed, affine_bg_vertical_shear_manager>`

- **affine_bg_vertical_shear_toggle_action** `affine_bg_vertical_shear_toggle_action(const affine_bg_ptr& bg, int duration_updates, fixed new_vertical_shear) : toggle_value_template_action(bg, duration_updates, new_vertical_shear)` (L2005): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the vertical shear of the given affine_bg_ptr.; `new_vertical_shear` New vertical shear when the action is updated duration_updates times.
- **affine_bg_vertical_shear_toggle_action** `affine_bg_vertical_shear_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_vertical_shear) : toggle_value_template_action(move(bg), duration_updates, new_vertical_shear)` (L2018): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the vertical shear of the given affine_bg_ptr.; `new_vertical_shear` New vertical shear when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L2026): Returns the affine_bg_ptr to modify.
- **new_vertical_shear** `fixed new_vertical_shear() const` (L2035): Returns the vertical shear of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_shear_manager` (class, bn_affine_bg_actions.h:2050)

Manages the shear of an affine_bg_ptr.

`class affine_bg_shear_manager`

- **get** `static fixed get(const affine_bg_ptr& bg)` (L2057): Returns the horizontal shear of the given affine_bg_ptr.
- **set** `static void set(fixed shear, affine_bg_ptr& bg)` (L2065): Sets the shear of the given affine_bg_ptr.
### `bn::affine_bg_shear_to_action` (class, bn_affine_bg_actions.h:2078)

Modifies the shear of an affine_bg_ptr until it has a given state.

`class affine_bg_shear_to_action : public to_value_template_action<affine_bg_ptr, fixed, affine_bg_shear_manager>`

- **affine_bg_shear_to_action** `affine_bg_shear_to_action(const affine_bg_ptr& bg, int duration_updates, fixed final_shear) : to_value_template_action(bg, duration_updates, final_shear)` (L2089): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the shear of the given affine_bg_ptr is equal to final_shear.; `final_shear` Shear when the action is updated duration_updates times.
- **affine_bg_shear_to_action** `affine_bg_shear_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_shear) : to_value_template_action(move(bg), duration_updates, final_shear)` (L2101): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the shear of the given affine_bg_ptr is equal to final_shear.; `final_shear` Shear when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L2109): Returns the affine_bg_ptr to modify.
- **final_shear** `fixed final_shear() const` (L2117): Returns the shear of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_shear_loop_action` (class, bn_affine_bg_actions.h:2131)

Modifies the shear of an affine_bg_ptr from a minimum to a maximum. When the shear is equal to the given final state, it goes back to its initial state and vice versa.

`class affine_bg_shear_loop_action : public loop_value_template_action<affine_bg_ptr, fixed, affine_bg_shear_manager>`

- **affine_bg_shear_loop_action** `affine_bg_shear_loop_action(const affine_bg_ptr& bg, int duration_updates, fixed final_shear) : loop_value_template_action(bg, duration_updates, final_shear)` (L2144): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the shear delta.; `final_shear` When the shear of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_bg_shear_loop_action** `affine_bg_shear_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_shear) : loop_value_template_action(move(bg), duration_updates, final_shear)` (L2157): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the shear delta.; `final_shear` When the shear of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const affine_bg_ptr& bg() const` (L2165): Returns the affine_bg_ptr to modify.
- **final_shear** `fixed final_shear() const` (L2174): When the shear of the given affine_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::affine_bg_shear_toggle_action` (class, bn_affine_bg_actions.h:2187)

Changes the shear of an affine_bg_ptr when the action is updated a given number of times.

`class affine_bg_shear_toggle_action : public toggle_value_template_action<affine_bg_ptr, fixed, affine_bg_shear_manager>`

- **affine_bg_shear_toggle_action** `affine_bg_shear_toggle_action(const affine_bg_ptr& bg, int duration_updates, fixed new_shear) : toggle_value_template_action(bg, duration_updates, new_shear)` (L2199): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the shear of the given affine_bg_ptr.; `new_shear` New shear when the action is updated duration_updates times.
- **affine_bg_shear_toggle_action** `affine_bg_shear_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_shear) : toggle_value_template_action(move(bg), duration_updates, new_shear)` (L2211): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the shear of the given affine_bg_ptr.; `new_shear` New shear when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L2219): Returns the affine_bg_ptr to modify.
- **new_shear** `fixed new_shear() const` (L2228): Returns the shear of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_horizontal_flip_manager` (class, bn_affine_bg_actions.h:2243)

Manages if an affine_bg_ptr is flipped in its horizontal axis or not.

`class affine_bg_horizontal_flip_manager`

- **get** `static bool get(const affine_bg_ptr& bg)` (L2250): Indicates if the given affine_bg_ptr is flipped in the horizontal axis or not.
- **set** `static void set(bool horizontal_flip, affine_bg_ptr& bg)` (L2258): Sets if the given affine_bg_ptr must be flipped in the horizontal axis or not.
### `bn::affine_bg_horizontal_flip_toggle_action` (class, bn_affine_bg_actions.h:2272)

Toggles if an affine_bg_ptr must be flipped in the horizontal axis or not when the action is updated a given number of times.

`class affine_bg_horizontal_flip_toggle_action : public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_horizontal_flip_manager>`

- **affine_bg_horizontal_flip_toggle_action** `affine_bg_horizontal_flip_toggle_action(const affine_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2283): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given affine_bg_ptr must be flipped in the horizontal axis or not.
- **affine_bg_horizontal_flip_toggle_action** `affine_bg_horizontal_flip_toggle_action(affine_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2294): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given affine_bg_ptr must be flipped in the horizontal axis or not.
- **bg** `const affine_bg_ptr& bg() const` (L2302): Returns the affine_bg_ptr to modify.
### `bn::affine_bg_vertical_flip_manager` (class, bn_affine_bg_actions.h:2317)

Manages if an affine_bg_ptr is flipped in its vertical axis or not.

`class affine_bg_vertical_flip_manager`

- **get** `static bool get(const affine_bg_ptr& bg)` (L2324): Indicates if the given affine_bg_ptr is flipped in the vertical axis or not.
- **set** `static void set(bool vertical_flip, affine_bg_ptr& bg)` (L2332): Sets if the given affine_bg_ptr must be flipped in the vertical axis or not.
### `bn::affine_bg_vertical_flip_toggle_action` (class, bn_affine_bg_actions.h:2346)

Toggles if an affine_bg_ptr must be flipped in the vertical axis or not when the action is updated a given number of times.

`class affine_bg_vertical_flip_toggle_action : public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_vertical_flip_manager>`

- **affine_bg_vertical_flip_toggle_action** `affine_bg_vertical_flip_toggle_action(const affine_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2357): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given affine_bg_ptr must be flipped in the vertical axis or not.
- **affine_bg_vertical_flip_toggle_action** `affine_bg_vertical_flip_toggle_action(affine_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2368): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given affine_bg_ptr must be flipped in the vertical axis or not.
- **bg** `const affine_bg_ptr& bg() const` (L2376): Returns the affine_bg_ptr to modify.
### `bn::affine_bg_pivot_position_manager` (class, bn_affine_bg_actions.h:2391)

Manages the position of the pivot of an affine_bg_ptr.

`class affine_bg_pivot_position_manager`

- **get** `static const fixed_point& get(const affine_bg_ptr& bg)` (L2398): Returns the position of the pivot of the given affine_bg_ptr.
- **set** `static void set(const fixed_point& position, affine_bg_ptr& bg)` (L2406): Sets the position of the pivot of the given affine_bg_ptr.
### `bn::affine_bg_move_pivot_by_action` (class, bn_affine_bg_actions.h:2419)

Modifies the position of the pivot of an affine_bg_ptr by a given delta.

`class affine_bg_move_pivot_by_action : public by_value_template_action<affine_bg_ptr, fixed_point, affine_bg_pivot_position_manager>`

- **affine_bg_move_pivot_by_action** `affine_bg_move_pivot_by_action(const affine_bg_ptr& bg, fixed delta_x, fixed delta_y) : by_value_template_action(bg, fixed_point(delta_x, delta_y))` (L2432): Constructor.
  `bg` affine_bg_ptr to copy.; `delta_x` How much to add to the horizontal position of the pivot of the given affine_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the pivot of the given affine_bg_ptr when the action is updated.
- **affine_bg_move_pivot_by_action** `affine_bg_move_pivot_by_action(affine_bg_ptr&& bg, fixed delta_x, fixed delta_y) : by_value_template_action(move(bg), fixed_point(delta_x, delta_y))` (L2445): Constructor.
  `bg` affine_bg_ptr to move.; `delta_x` How much to add to the horizontal position of the pivot of the given affine_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the pivot of the given affine_bg_ptr when the action is updated.
- **affine_bg_move_pivot_by_action** `affine_bg_move_pivot_by_action(const affine_bg_ptr& bg, const fixed_point& delta_position) : by_value_template_action(bg, delta_position)` (L2456): Constructor.
  `bg` affine_bg_ptr to copy.; `delta_position` How much to add to the position of the pivot of the given affine_bg_ptr when the action is updated.
- **affine_bg_move_pivot_by_action** `affine_bg_move_pivot_by_action(affine_bg_ptr&& bg, const fixed_point& delta_position) : by_value_template_action(move(bg), delta_position)` (L2467): Constructor.
  `bg` affine_bg_ptr to move.; `delta_position` How much to add to the position of the pivot of the given affine_bg_ptr when the action is updated.
- **bg** `const affine_bg_ptr& bg() const` (L2475): Returns the affine_bg_ptr to modify.
- **delta_position** `const fixed_point& delta_position() const` (L2484): Returns how much to add to the position of the pivot of the given affine_bg_ptr when the action is updated.
### `bn::affine_bg_move_pivot_to_action` (class, bn_affine_bg_actions.h:2497)

Modifies the position of the pivot of an affine_bg_ptr until it has a given state.

`class affine_bg_move_pivot_to_action : public to_value_template_action<affine_bg_ptr, fixed_point, affine_bg_pivot_position_manager>`

- **affine_bg_move_pivot_to_action** `affine_bg_move_pivot_to_action(const affine_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L2512): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given affine_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the pivot of the given affine_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the pivot of the given affine_bg_ptr when the action is updated duration_updates times.
- **affine_bg_move_pivot_to_action** `affine_bg_move_pivot_to_action(affine_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L2527): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given affine_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the pivot of the given affine_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the pivot of the given affine_bg_ptr when the action is updated duration_updates times.
- **affine_bg_move_pivot_to_action** `affine_bg_move_pivot_to_action(const affine_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(bg, duration_updates, final_position)` (L2540): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given affine_bg_ptr is equal to final_position.; `final_position` Position of the pivot of the given affine_bg_ptr when the action is updated duration_updates times.
- **affine_bg_move_pivot_to_action** `affine_bg_move_pivot_to_action(affine_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(move(bg), duration_updates, final_position)` (L2554): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given affine_bg_ptr is equal to final_position.; `final_position` Position of the pivot of the given affine_bg_ptr when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L2562): Returns the affine_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L2571): Returns the position of the pivot of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_move_pivot_loop_action` (class, bn_affine_bg_actions.h:2585)

Modifies the position of the pivot of an affine_bg_ptr from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class affine_bg_move_pivot_loop_action : public loop_value_template_action<affine_bg_ptr, fixed_point, affine_bg_pivot_position_manager>`

- **affine_bg_move_pivot_loop_action** `affine_bg_move_pivot_loop_action(const affine_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L2600): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the pivot of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the pivot of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_bg_move_pivot_loop_action** `affine_bg_move_pivot_loop_action(affine_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L2615): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the pivot of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the pivot of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_bg_move_pivot_loop_action** `affine_bg_move_pivot_loop_action(const affine_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(bg, duration_updates, final_position)` (L2628): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the pivot of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_bg_move_pivot_loop_action** `affine_bg_move_pivot_loop_action(affine_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(move(bg), duration_updates, final_position)` (L2642): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the pivot of the given affine_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const affine_bg_ptr& bg() const` (L2651): Returns the affine_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L2660): When the position of the pivot of the given affine_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::affine_bg_move_pivot_toggle_action` (class, bn_affine_bg_actions.h:2673)

Changes the position of the pivot of an affine_bg_ptr when the action is updated a given number of times.

`class affine_bg_move_pivot_toggle_action : public toggle_value_template_action<affine_bg_ptr, fixed_point, affine_bg_pivot_position_manager>`

- **affine_bg_move_pivot_toggle_action** `affine_bg_move_pivot_toggle_action(const affine_bg_ptr& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(bg, duration_updates, fixed_point(new_x, new_y))` (L2686): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given affine_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **affine_bg_move_pivot_toggle_action** `affine_bg_move_pivot_toggle_action(affine_bg_ptr&& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(move(bg), duration_updates, fixed_point(new_x, new_y))` (L2699): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given affine_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **affine_bg_move_pivot_toggle_action** `affine_bg_move_pivot_toggle_action(const affine_bg_ptr& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(bg, duration_updates, new_position)` (L2711): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given affine_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **affine_bg_move_pivot_toggle_action** `affine_bg_move_pivot_toggle_action(affine_bg_ptr&& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(move(bg), duration_updates, new_position)` (L2724): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given affine_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **bg** `const affine_bg_ptr& bg() const` (L2733): Returns the affine_bg_ptr to modify.
- **new_position** `const fixed_point& new_position() const` (L2742): Returns the position of the pivot of the given affine_bg_ptr when the action is updated the given number of times.
### `bn::affine_bg_wrapping_manager` (class, bn_affine_bg_actions.h:2757)

Manages if an affine background must wrap around at the edges or not.

`class affine_bg_wrapping_manager`

- **get** `static bool get(const affine_bg_ptr& bg)` (L2764): Indicates if the given affine_bg_ptr wraps around at the edges or not.
- **set** `static void set(bool wrapping_enabled, affine_bg_ptr& bg)` (L2772): Sets if the given affine_bg_ptr must wrap around at the edges or not.
### `bn::affine_bg_wrapping_toggle_action` (class, bn_affine_bg_actions.h:2786)

Toggles if an affine_bg_ptr must wrap around at the edges or not when the action is updated a given number of times.

`class affine_bg_wrapping_toggle_action : public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_wrapping_manager>`

- **affine_bg_wrapping_toggle_action** `affine_bg_wrapping_toggle_action(const affine_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2797): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given affine_bg_ptr wraps around at the edges or not.
- **affine_bg_wrapping_toggle_action** `affine_bg_wrapping_toggle_action(affine_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2808): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given affine_bg_ptr wraps around at the edges or not.
- **bg** `const affine_bg_ptr& bg() const` (L2816): Returns the affine_bg_ptr to modify.
### `bn::affine_bg_mosaic_manager` (class, bn_affine_bg_actions.h:2831)

Manages if the mosaic effect must be applied to an affine_bg_ptr or not.

`class affine_bg_mosaic_manager`

- **get** `static bool get(const affine_bg_ptr& bg)` (L2838): Indicates if the mosaic effect is applied to the given affine_bg_ptr or not.
- **set** `static void set(bool mosaic_enabled, affine_bg_ptr& bg)` (L2846): Sets if the mosaic effect must be applied to the given affine_bg_ptr or not.
### `bn::affine_bg_mosaic_toggle_action` (class, bn_affine_bg_actions.h:2860)

Toggles if the mosaic effect must be applied to an affine_bg_ptr or not when the action is updated a given number of times.

`class affine_bg_mosaic_toggle_action : public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_mosaic_manager>`

- **affine_bg_mosaic_toggle_action** `affine_bg_mosaic_toggle_action(const affine_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2871): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the mosaic effect must be applied or not.
- **affine_bg_mosaic_toggle_action** `affine_bg_mosaic_toggle_action(affine_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2882): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the mosaic effect must be applied or not.
- **bg** `const affine_bg_ptr& bg() const` (L2890): Returns the affine_bg_ptr to modify.
### `bn::affine_bg_blending_manager` (class, bn_affine_bg_actions.h:2905)

Manages if blending must be applied to an affine_bg_ptr or not.

`class affine_bg_blending_manager`

- **get** `static bool get(const affine_bg_ptr& bg)` (L2912): Indicates if blending is applied to the given affine_bg_ptr or not.
- **set** `static void set(bool blending_enabled, affine_bg_ptr& bg)` (L2920): Sets if blending must be applied to the given affine_bg_ptr or not.
### `bn::affine_bg_blending_toggle_action` (class, bn_affine_bg_actions.h:2934)

Toggles if blending must be applied to an affine_bg_ptr or not when the action is updated a given number of times.

`class affine_bg_blending_toggle_action : public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_blending_manager>`

- **affine_bg_blending_toggle_action** `affine_bg_blending_toggle_action(const affine_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2945): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if blending must be applied or not.
- **affine_bg_blending_toggle_action** `affine_bg_blending_toggle_action(affine_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2956): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if blending must be applied or not.
- **bg** `const affine_bg_ptr& bg() const` (L2964): Returns the affine_bg_ptr to modify.
### `bn::affine_bg_blending_top_manager` (class, bn_affine_bg_actions.h:2977)

Manages if an affine_bg_ptr must be part of the blending top layer or not.

`class affine_bg_blending_top_manager`

- **get** `static bool get(const affine_bg_ptr& bg)` (L2984): Indicates if the given affine_bg_ptr is part of the blending top layer or not.
- **set** `static void set(bool blending_top_enabled, affine_bg_ptr& bg)` (L2992): Sets if the given affine_bg_ptr must be part of the blending top layer or not.
### `bn::affine_bg_blending_top_toggle_action` (class, bn_affine_bg_actions.h:3006)

Toggles if an affine_bg_ptr is part of the blending top layer or not when the action is updated a given number of times.

`class affine_bg_blending_top_toggle_action : public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_blending_top_manager>`

- **affine_bg_blending_top_toggle_action** `affine_bg_blending_top_toggle_action(const affine_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L3017): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given affine_bg_ptr is part of the blending top layer or not.
- **affine_bg_blending_top_toggle_action** `affine_bg_blending_top_toggle_action(affine_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L3028): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given affine_bg_ptr is part of the blending top layer or not.
- **bg** `const affine_bg_ptr& bg() const` (L3036): Returns the affine_bg_ptr to modify.
### `bn::affine_bg_blending_bottom_manager` (class, bn_affine_bg_actions.h:3049)

Manages if an affine_bg_ptr must be part of the blending bottom layer or not.

`class affine_bg_blending_bottom_manager`

- **get** `static bool get(const affine_bg_ptr& bg)` (L3056): Indicates if the given affine_bg_ptr is part of the blending bottom layer or not.
- **set** `static void set(bool blending_bottom_enabled, affine_bg_ptr& bg)` (L3064): Sets if the given affine_bg_ptr must be part of the blending bottom layer or not.
### `bn::affine_bg_blending_bottom_toggle_action` (class, bn_affine_bg_actions.h:3078)

Toggles if an affine_bg_ptr is part of the blending bottom layer or not when the action is updated a given number of times.

`class affine_bg_blending_bottom_toggle_action : public bool_toggle_value_template_action<affine_bg_ptr, affine_bg_blending_bottom_manager>`

- **affine_bg_blending_bottom_toggle_action** `affine_bg_blending_bottom_toggle_action(const affine_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L3089): Constructor.
  `bg` affine_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given affine_bg_ptr is part of the blending bottom layer or not.
- **affine_bg_blending_bottom_toggle_action** `affine_bg_blending_bottom_toggle_action(affine_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L3100): Constructor.
  `bg` affine_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given affine_bg_ptr is part of the blending bottom layer or not.
- **bg** `const affine_bg_ptr& bg() const` (L3108): Returns the affine_bg_ptr to modify.

## bn_affine_bg_animate_actions.h

### `bn::iaffine_bg_animate_action` (class, bn_affine_bg_animate_actions.h:39)

Base class of bn::affine_bg_animate_action.

`class iaffine_bg_animate_action`

Can be used as a reference type for all bn::affine_bg_animate_action objects.

- **operator=** `iaffine_bg_animate_action& operator=(const iaffine_bg_animate_action& other)` (L50): Copy assignment operator.
  `other` iaffine_bg_animate_action to copy.; devuelve: Reference to this.
- **operator=** `iaffine_bg_animate_action& operator=(iaffine_bg_animate_action&& other) noexcept` (L57): Move assignment operator.
  `other` iaffine_bg_animate_action to move.; devuelve: Reference to this.
- **update** `void update()` (L62): Changes the map of the given affine_bg_ptr when the given amount of update calls are done.
- **done** `bool done() const` (L67): Indicates if the action must not be updated anymore.
- **reset** `void reset()` (L75): Resets the action to its initial state.
- **affine_bg** `const affine_bg_ptr& affine_bg() const` (L84): Returns the affine_bg_ptr to modify.
- **wait_updates** `int wait_updates() const` (L93): Returns the number of times the action must be updated before changing the map of the given affine_bg_ptr.
- **set_wait_updates** `void set_wait_updates(int wait_updates)` (L102): Sets the number of times the action must be updated before changing the map of the given affine_bg_ptr.
- **next_change_updates** `int next_change_updates() const` (L107): Returns the number of times the action must be updated before the next map change.
- **set_next_change_updates** `void set_next_change_updates(int next_change_updates)` (L115): Sets the number of times the action must be updated before the next map change.
- **map_item** `const affine_bg_map_item& map_item() const` (L120): Returns the affine_bg_map_item used to create the new map to use by the given affine_bg_ptr.
- **map_indexes** `const ivector<uint16_t>& map_indexes() const` (L128): Returns the indexes of the maps to reference in the given affine_bg_map_item.
- **update_forever** `bool update_forever() const` (L136): Indicates if the action can be updated forever or not.
- **set_update_forever** `void set_update_forever(bool forever)` (L144): Specifies if the action must be updated forever or not.
- **current_index** `int current_index() const` (L152): Returns the current index of the given map_indexes (not the current index of the map to reference in the given map_item).
  This index is the one to be used on the next map change. It can be out of bounds if the action is done.
- **set_current_index** `void set_current_index(int current_index)` (L161): Sets the current index of the given map_indexes (not the current index of the map to reference in the given map_item).
- **set_current_index** `void set_current_index(int current_index, bool update_bg_now)` (L172): Sets the current index of the given map_indexes (not the current index of the map to reference in the given map_item).
  `current_index` New index of the given map_indexes.; `update_bg_now` Specifies if the background must be updated now or on the next map change.
- **current_map_index** `int current_map_index() const` (L179): Returns the current index of the map to reference in the given map_item.
  This index is the one to be used on the next map change.
- **once** `static affine_bg_animate_action once( const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L226): Generates an affine_bg_animate_action which loops over the given affine_bg maps only once.
  `affine_bg` affine_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested affine_bg_animate_action.
- **once** `static affine_bg_animate_action once( affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L242): Generates an affine_bg_animate_action which loops over the given affine_bg maps only once.
  `affine_bg` affine_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in the given affine_bg_map_item.; devuelve: The requested affine_bg_animate_action.
- **forever** `static affine_bg_animate_action forever( const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L258): Generates an affine_bg_animate_action which loops over the given affine_bg maps forever.
  `affine_bg` affine_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in the given affine_bg_map_item.; devuelve: The requested affine_bg_animate_action.
- **forever** `static affine_bg_animate_action forever( affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L274): Generates an affine_bg_animate_action which loops over the given affine_bg maps forever.
  `affine_bg` affine_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in the given affine_bg_map_item.; devuelve: The requested affine_bg_animate_action.
- **affine_bg_animate_action** `affine_bg_animate_action(const affine_bg_animate_action& other)` (L285): Copy constructor.
  `other` affine_bg_animate_action to copy.
- **affine_bg_animate_action** `affine_bg_animate_action(affine_bg_animate_action&& other) noexcept : _affine_bg(move(other._affine_bg)), _map_item(other._map_item), _map_indexes(other._map_indexes)` (L298): Move constructor.
  `other` affine_bg_animate_action to move.
- **affine_bg_animate_action** `affine_bg_animate_action(const iaffine_bg_animate_action& other)` (L311): Copy constructor.
  `other` iaffine_bg_animate_action to copy.
- **operator=** `affine_bg_animate_action& operator=(const affine_bg_animate_action& other)` (L328): Copy assignment operator.
  `other` affine_bg_animate_action to copy.; devuelve: Reference to this.
- **operator=** `affine_bg_animate_action& operator=(affine_bg_animate_action&& other) noexcept` (L346): Move assignment operator.
  `other` affine_bg_animate_action to move.; devuelve: Reference to this.
- **operator=** `affine_bg_animate_action& operator=(const iaffine_bg_animate_action& other)` (L364): Copy assignment operator.
  `other` iaffine_bg_animate_action to copy.; devuelve: Reference to this.
- **operator=** `affine_bg_animate_action& operator=(iaffine_bg_animate_action&& other) noexcept` (L375): Move assignment operator.
  `other` ivector to move.; devuelve: Reference to this.
- **create_affine_bg_animate_action_once** `template<typename ...Args> auto create_affine_bg_animate_action_once( const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)` (L423): Generates an affine_bg_animate_action which loops over the given affine_bg maps only once.
  `affine_bg` affine_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested affine_bg_animate_action.
- **create_affine_bg_animate_action_once** `template<typename ...Args> auto create_affine_bg_animate_action_once( affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)` (L444): Generates an affine_bg_animate_action which loops over the given affine_bg maps only once.
  `affine_bg` affine_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested affine_bg_animate_action.
- **create_affine_bg_animate_action_forever** `template<typename ...Args> auto create_affine_bg_animate_action_forever( const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)` (L465): Generates an affine_bg_animate_action which loops over the given affine_bg maps forever.
  `affine_bg` affine_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested affine_bg_animate_action.
- **create_affine_bg_animate_action_forever** `template<typename ...Args> auto create_affine_bg_animate_action_forever( affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)` (L486): Generates an affine_bg_animate_action which loops over the given affine_bg maps forever.
  `affine_bg` affine_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested affine_bg_animate_action.
### `bn::iaffine_bg_cached_animate_action` (class, bn_affine_bg_animate_actions.h:507)

Base class of bn::affine_bg_cached_animate_action.

`class iaffine_bg_cached_animate_action`

Can be used as a reference type for all bn::affine_bg_cached_animate_action objects.

- **operator=** `iaffine_bg_cached_animate_action& operator=(const iaffine_bg_cached_animate_action& other)` (L518): Copy assignment operator.
  `other` iaffine_bg_cached_animate_action to copy.; devuelve: Reference to this.
- **operator=** `iaffine_bg_cached_animate_action& operator=(iaffine_bg_cached_animate_action&& other) noexcept` (L525): Move assignment operator.
  `other` iaffine_bg_cached_animate_action to move.; devuelve: Reference to this.
- **update** `void update()` (L530): Changes the map of the given affine_bg_ptr when the given amount of update calls are done.
- **done** `bool done() const` (L535): Indicates if the action must not be updated anymore.
- **reset** `void reset()` (L543): Resets the action to its initial state.
- **affine_bg** `const affine_bg_ptr& affine_bg() const` (L552): Returns the affine_bg_ptr to modify.
- **wait_updates** `int wait_updates() const` (L561): Returns the number of times the action must be updated before changing the map of the given affine_bg_ptr.
- **set_wait_updates** `void set_wait_updates(int wait_updates)` (L570): Sets the number of times the action must be updated before changing the map of the given affine_bg_ptr.
- **next_change_updates** `int next_change_updates() const` (L575): Returns the number of times the action must be updated before the next map change.
- **set_next_change_updates** `void set_next_change_updates(int next_change_updates)` (L583): Sets the number of times the action must be updated before the next map change.
- **maps** `const ivector<affine_bg_map_ptr>& maps() const` (L588): Returns the maps to use by the given affine_bg_ptr.
- **update_forever** `bool update_forever() const` (L596): Indicates if the action can be updated forever or not.
- **set_update_forever** `void set_update_forever(bool forever)` (L604): Specifies if the action must be updated forever or not.
- **current_index** `int current_index() const` (L611): Returns the current index of the given maps.
  This index is the one to be used on the next map change. It can be out of bounds if the action is done.
- **set_current_index** `void set_current_index(int current_index)` (L619): Sets the current index of the given maps.
- **set_current_index** `void set_current_index(int current_index, bool update_bg_now)` (L629): Sets the current index of the given maps.
  `current_index` New index of the given maps.; `update_bg_now` Specifies if the background must be updated now or on the next map change.
- **current_map** `const affine_bg_map_ptr& current_map() const` (L636): Returns the current map.
  This map is the one to be used on the next map change.
- **once** `static affine_bg_cached_animate_action once( const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L686): Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps only once.
  `affine_bg` affine_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested affine_bg_cached_animate_action.
- **once** `static affine_bg_cached_animate_action once( affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L702): Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps only once.
  `affine_bg` affine_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in the given affine_bg_map_item.; devuelve: The requested affine_bg_cached_animate_action.
- **once** `static affine_bg_cached_animate_action once( const affine_bg_ptr& affine_bg, int wait_updates, span<affine_bg_map_ptr> maps)` (L717): Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps only once.
  `affine_bg` affine_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `maps` List of maps to use by the given affine_bg_ptr.; devuelve: The requested affine_bg_cached_animate_action.
- **once** `static affine_bg_cached_animate_action once( affine_bg_ptr&& affine_bg, int wait_updates, span<affine_bg_map_ptr> maps)` (L731): Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps only once.
  `affine_bg` affine_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `maps` List of maps to use by the given affine_bg_ptr.; devuelve: The requested affine_bg_cached_animate_action.
- **forever** `static affine_bg_cached_animate_action forever( const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L746): Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps forever.
  `affine_bg` affine_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested affine_bg_cached_animate_action.
- **forever** `static affine_bg_cached_animate_action forever( affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L762): Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps forever.
  `affine_bg` affine_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in the given affine_bg_map_item.; devuelve: The requested affine_bg_cached_animate_action.
- **forever** `static affine_bg_cached_animate_action forever( const affine_bg_ptr& affine_bg, int wait_updates, span<affine_bg_map_ptr> maps)` (L777): Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps forever.
  `affine_bg` affine_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `maps` List of maps to use by the given affine_bg_ptr.; devuelve: The requested affine_bg_cached_animate_action.
- **forever** `static affine_bg_cached_animate_action forever( affine_bg_ptr&& affine_bg, int wait_updates, span<affine_bg_map_ptr> maps)` (L791): Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps forever.
  `affine_bg` affine_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `maps` List of maps to use by the given affine_bg_ptr.; devuelve: The requested affine_bg_cached_animate_action.
- **affine_bg_cached_animate_action** `affine_bg_cached_animate_action(const affine_bg_cached_animate_action& other)` (L801): Copy constructor.
  `other` affine_bg_cached_animate_action to copy.
- **affine_bg_cached_animate_action** `affine_bg_cached_animate_action(affine_bg_cached_animate_action&& other) noexcept : _affine_bg(move(other._affine_bg)), _maps(move(other._maps))` (L813): Move constructor.
  `other` affine_bg_cached_animate_action to move.
- **affine_bg_cached_animate_action** `affine_bg_cached_animate_action(const iaffine_bg_cached_animate_action& other)` (L825): Copy constructor.
  `other` iaffine_bg_cached_animate_action to copy.
- **operator=** `affine_bg_cached_animate_action& operator=(const affine_bg_cached_animate_action& other)` (L840): Copy assignment operator.
  `other` affine_bg_cached_animate_action to copy.; devuelve: Reference to this.
- **operator=** `affine_bg_cached_animate_action& operator=(affine_bg_cached_animate_action&& other) noexcept` (L857): Move assignment operator.
  `other` affine_bg_cached_animate_action to move.; devuelve: Reference to this.
- **operator=** `affine_bg_cached_animate_action& operator=(const iaffine_bg_cached_animate_action& other)` (L874): Copy assignment operator.
  `other` iaffine_bg_cached_animate_action to copy.; devuelve: Reference to this.
- **operator=** `affine_bg_cached_animate_action& operator=(iaffine_bg_cached_animate_action&& other) noexcept` (L885): Move assignment operator.
  `other` ivector to move.; devuelve: Reference to this.
- **create_affine_bg_cached_animate_action_once** `template<typename ...Args> auto create_affine_bg_cached_animate_action_once( const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)` (L950): Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps only once.
  `affine_bg` affine_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested affine_bg_cached_animate_action.
- **create_affine_bg_cached_animate_action_once** `template<typename ...Args> auto create_affine_bg_cached_animate_action_once( affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)` (L971): Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps only once.
  `affine_bg` affine_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested affine_bg_cached_animate_action.
- **create_affine_bg_cached_animate_action_forever** `template<typename ...Args> auto create_affine_bg_cached_animate_action_forever( const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)` (L992): Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps forever.
  `affine_bg` affine_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested affine_bg_cached_animate_action.
- **create_affine_bg_cached_animate_action_forever** `template<typename ...Args> auto create_affine_bg_cached_animate_action_forever( affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)` (L1013): Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps forever.
  `affine_bg` affine_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given affine_bg_ptr.; `map_item` It creates the new map to use by the given affine_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested affine_bg_cached_animate_action.

## bn_affine_bg_animate_actions_fwd.h

### `bn::affine_bg_animate_action` (class, bn_affine_bg_animate_actions_fwd.h:38)

Changes the map of an affine_bg_ptr when the action is updated a given number of times.

`template<int MaxSize> class affine_bg_animate_action`

Each time the map of an affine_bg_ptr must be changed, it is searched for and created if it has not been found,
so the maps are not cached.

### `bn::affine_bg_cached_animate_action` (class, bn_affine_bg_animate_actions_fwd.h:58)

Changes the map of an affine_bg_ptr when the action is updated a given number of times.

`template<int MaxSize> class affine_bg_cached_animate_action`

This action differs from affine_bg_animate_action in that all maps to use are cached,
so it is faster but takes way more VRAM.


## bn_backdrop_actions.h

### `bn::backdrop_blending_bottom_enabled_manager` (class, bn_backdrop_actions.h:29)

Manages if the backdrop must be part of the blending bottom layer or not.

`class backdrop_blending_bottom_enabled_manager`

- **get** `static bool get()` (L36): Indicates if the backdrop is part of the blending bottom layer or not.
- **set** `static void set(bool enabled)` (L44): Sets if the backdrop must be part of the blending bottom layer or not.
### `bn::backdrop_blending_bottom_enabled_toggle_action` (class, bn_backdrop_actions.h:58)

Toggles if the backdrop is part of the blending bottom layer or not when the action is updated a given number of times.

`class backdrop_blending_bottom_enabled_toggle_action : public bool_toggle_template_action<backdrop_blending_bottom_enabled_manager>`

- **backdrop_blending_bottom_enabled_toggle_action** `explicit backdrop_blending_bottom_enabled_toggle_action(int duration_updates) : bool_toggle_template_action(duration_updates)` (L68): Constructor.
  `duration_updates` How many times the action has to be updated to toggle if the backdrop is part of the blending bottom layer or not.

## bn_bg_palettes_actions.h

### `bn::bg_palettes_brightness_manager` (class, bn_bg_palettes_actions.h:34)

Manages the brightness of all background color palettes.

`class bg_palettes_brightness_manager`

- **get** `static fixed get()` (L41): Returns the brightness of all background color palettes.
- **set** `static void set(fixed brightness)` (L50): Sets the brightness of all background color palettes.
  `brightness` New brightness in the range [0..1].
### `bn::bg_palettes_brightness_to_action` (class, bn_bg_palettes_actions.h:64)

Modifies the brightness of all background color palettes until it has a given state.

`class bg_palettes_brightness_to_action : public to_template_action<fixed, bg_palettes_brightness_manager>`

- **bg_palettes_brightness_to_action** `bg_palettes_brightness_to_action(int duration_updates, fixed final_brightness) : to_template_action(duration_updates, final_brightness)` (L76): Constructor.
  `duration_updates` Number of times that the action must be updated until the brightness is equal to final_brightness.; `final_brightness` Brightness when the action is updated duration_updates times.
  This brightness must be in the range [0..1].
- **final_brightness** `fixed final_brightness() const` (L85): Returns the brightness when the action is updated the given number of times.
### `bn::bg_palettes_brightness_loop_action` (class, bn_bg_palettes_actions.h:100)

Modifies the brightness of all background color palettes from a minimum to a maximum. When the brightness is equal to the given final state, it goes back to its initial state and vice versa.

`class bg_palettes_brightness_loop_action : public loop_template_action<fixed, bg_palettes_brightness_manager>`

- **bg_palettes_brightness_loop_action** `bg_palettes_brightness_loop_action(int duration_updates, fixed final_brightness) : loop_template_action(duration_updates, final_brightness)` (L113): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the brightness delta.; `final_brightness` When the brightness is equal to this parameter, it goes back to its initial state and vice versa.
  This brightness must be in the range [0..1].
- **final_brightness** `fixed final_brightness() const` (L123): When the brightness is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::bg_palettes_brightness_toggle_action` (class, bn_bg_palettes_actions.h:138)

Changes the brightness of all background color palettes when the action is updated a given number of times.

`class bg_palettes_brightness_toggle_action : public toggle_template_action<fixed, bg_palettes_brightness_manager>`

- **bg_palettes_brightness_toggle_action** `bg_palettes_brightness_toggle_action(int duration_updates, fixed new_brightness) : toggle_template_action(duration_updates, new_brightness)` (L149): Constructor.
  `duration_updates` How many times the action has to be updated to change the brightness.; `new_brightness` New brightness when the action is updated duration_updates times.
  This brightness must be in the range [0..1].
- **new_brightness** `fixed new_brightness() const` (L158): Returns the brightness when the action is updated the given number of times.
### `bn::bg_palettes_contrast_manager` (class, bn_bg_palettes_actions.h:174)

Manages the contrast of all background color palettes.

`class bg_palettes_contrast_manager`

- **get** `static fixed get()` (L181): Returns the contrast of all background color palettes.
- **set** `static void set(fixed contrast)` (L190): Sets the contrast of all background color palettes.
  `contrast` New contrast in the range [0..1].
### `bn::bg_palettes_contrast_to_action` (class, bn_bg_palettes_actions.h:204)

Modifies the contrast of all background color palettes until it has a given state.

`class bg_palettes_contrast_to_action : public to_template_action<fixed, bg_palettes_contrast_manager>`

- **bg_palettes_contrast_to_action** `bg_palettes_contrast_to_action(int duration_updates, fixed final_contrast) : to_template_action(duration_updates, final_contrast)` (L216): Constructor.
  `duration_updates` Number of times that the action must be updated until the contrast is equal to final_contrast.; `final_contrast` Contrast when the action is updated duration_updates times.
  This contrast must be in the range [0..1].
- **final_contrast** `fixed final_contrast() const` (L225): Returns the contrast when the action is updated the given number of times.
### `bn::bg_palettes_contrast_loop_action` (class, bn_bg_palettes_actions.h:240)

Modifies the contrast of all background color palettes from a minimum to a maximum. When the contrast is equal to the given final state, it goes back to its initial state and vice versa.

`class bg_palettes_contrast_loop_action : public loop_template_action<fixed, bg_palettes_contrast_manager>`

- **bg_palettes_contrast_loop_action** `bg_palettes_contrast_loop_action(int duration_updates, fixed final_contrast) : loop_template_action(duration_updates, final_contrast)` (L253): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the contrast delta.; `final_contrast` When the contrast is equal to this parameter, it goes back to its initial state and vice versa.
  This contrast must be in the range [0..1].
- **final_contrast** `fixed final_contrast() const` (L263): When the contrast is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::bg_palettes_contrast_toggle_action` (class, bn_bg_palettes_actions.h:278)

Changes the contrast of all background color palettes when the action is updated a given number of times.

`class bg_palettes_contrast_toggle_action : public toggle_template_action<fixed, bg_palettes_contrast_manager>`

- **bg_palettes_contrast_toggle_action** `bg_palettes_contrast_toggle_action(int duration_updates, fixed new_contrast) : toggle_template_action(duration_updates, new_contrast)` (L289): Constructor.
  `duration_updates` How many times the action has to be updated to change the contrast.; `new_contrast` New contrast when the action is updated duration_updates times.
  This contrast must be in the range [0..1].
- **new_contrast** `fixed new_contrast() const` (L298): Returns the contrast when the action is updated the given number of times.
### `bn::bg_palettes_intensity_manager` (class, bn_bg_palettes_actions.h:314)

Manages the intensity of all background color palettes.

`class bg_palettes_intensity_manager`

- **get** `static fixed get()` (L321): Returns the intensity of all background color palettes.
- **set** `static void set(fixed intensity)` (L330): Sets the intensity of all background color palettes.
  `intensity` New intensity in the range [0..1].
### `bn::bg_palettes_intensity_to_action` (class, bn_bg_palettes_actions.h:344)

Modifies the intensity of all background color palettes until it has a given state.

`class bg_palettes_intensity_to_action : public to_template_action<fixed, bg_palettes_intensity_manager>`

- **bg_palettes_intensity_to_action** `bg_palettes_intensity_to_action(int duration_updates, fixed final_intensity) : to_template_action(duration_updates, final_intensity)` (L356): Constructor.
  `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L365): Returns the intensity when the action is updated the given number of times.
### `bn::bg_palettes_intensity_loop_action` (class, bn_bg_palettes_actions.h:380)

Modifies the intensity of all background color palettes from a minimum to a maximum. When the intensity is equal to the given final state, it goes back to its initial state and vice versa.

`class bg_palettes_intensity_loop_action : public loop_template_action<fixed, bg_palettes_intensity_manager>`

- **bg_palettes_intensity_loop_action** `bg_palettes_intensity_loop_action(int duration_updates, fixed final_intensity) : loop_template_action(duration_updates, final_intensity)` (L393): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L403): When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::bg_palettes_intensity_toggle_action` (class, bn_bg_palettes_actions.h:418)

Changes the intensity of all background color palettes when the action is updated a given number of times.

`class bg_palettes_intensity_toggle_action : public toggle_template_action<fixed, bg_palettes_intensity_manager>`

- **bg_palettes_intensity_toggle_action** `bg_palettes_intensity_toggle_action(int duration_updates, fixed new_intensity) : toggle_template_action(duration_updates, new_intensity)` (L429): Constructor.
  `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **new_intensity** `fixed new_intensity() const` (L438): Returns the intensity when the action is updated the given number of times.
### `bn::bg_palettes_inverted_manager` (class, bn_bg_palettes_actions.h:454)

Manages if the colors of all background color palettes must be inverted or not.

`class bg_palettes_inverted_manager`

- **get** `static bool get()` (L461): Indicates if the colors of all background color palettes are inverted or not.
- **set** `static void set(bool inverted)` (L469): Sets if the colors of all background color palettes must be inverted or not.
### `bn::bg_palettes_inverted_toggle_action` (class, bn_bg_palettes_actions.h:484)

Toggles if the colors of all background color palettes must be inverted or not when the action is updated a given number of times.

`class bg_palettes_inverted_toggle_action : public bool_toggle_template_action<bg_palettes_inverted_manager>`

- **bg_palettes_inverted_toggle_action** `explicit bg_palettes_inverted_toggle_action(int duration_updates) : bool_toggle_template_action(duration_updates)` (L493): Constructor.
  `duration_updates` How many times the action has to be updated to toggle if the colors of all background color palettes must be inverted or not.
### `bn::bg_palettes_grayscale_manager` (class, bn_bg_palettes_actions.h:509)

Manages the intensity of the grayscale effect applied to all background color palettes.

`class bg_palettes_grayscale_manager`

- **get** `static fixed get()` (L516): Returns the intensity of the grayscale effect applied to all background color palettes.
- **set** `static void set(fixed intensity)` (L525): Sets the intensity of the grayscale effect applied to all background color palettes.
  `intensity` New intensity in the range [0..1].
### `bn::bg_palettes_grayscale_to_action` (class, bn_bg_palettes_actions.h:540)

Modifies the intensity of the grayscale effect applied to all background color palettes until it has a given state.

`class bg_palettes_grayscale_to_action : public to_template_action<fixed, bg_palettes_grayscale_manager>`

- **bg_palettes_grayscale_to_action** `bg_palettes_grayscale_to_action(int duration_updates, fixed final_intensity) : to_template_action(duration_updates, final_intensity)` (L552): Constructor.
  `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L561): Returns the intensity when the action is updated the given number of times.
### `bn::bg_palettes_grayscale_loop_action` (class, bn_bg_palettes_actions.h:577)

Modifies the intensity of the grayscale effect applied to all background color palettes from a minimum to a maximum. When the intensity is equal to the given final state, it goes back to its initial state and vice versa.

`class bg_palettes_grayscale_loop_action : public loop_template_action<fixed, bg_palettes_grayscale_manager>`

- **bg_palettes_grayscale_loop_action** `bg_palettes_grayscale_loop_action(int duration_updates, fixed final_intensity) : loop_template_action(duration_updates, final_intensity)` (L590): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L600): When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::bg_palettes_grayscale_toggle_action` (class, bn_bg_palettes_actions.h:615)

Changes the intensity of the grayscale effect applied to all background color palettes when the action is updated a given number of times.

`class bg_palettes_grayscale_toggle_action : public toggle_template_action<fixed, bg_palettes_grayscale_manager>`

- **bg_palettes_grayscale_toggle_action** `bg_palettes_grayscale_toggle_action(int duration_updates, fixed new_intensity) : toggle_template_action(duration_updates, new_intensity)` (L626): Constructor.
  `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **new_intensity** `fixed new_intensity() const` (L635): Returns the intensity when the action is updated the given number of times.
### `bn::bg_palettes_hue_shift_manager` (class, bn_bg_palettes_actions.h:651)

Manages the intensity of the hue shift effect applied to all background color palettes.

`class bg_palettes_hue_shift_manager`

- **get** `static fixed get()` (L658): Returns the intensity of the hue shift effect applied to all background color palettes.
- **set** `static void set(fixed intensity)` (L667): Sets the intensity of the hue shift effect applied to all background color palettes.
  `intensity` New intensity in the range [0..1].
### `bn::bg_palettes_hue_shift_to_action` (class, bn_bg_palettes_actions.h:682)

Modifies the intensity of the hue shift effect applied to all background color palettes until it has a given state.

`class bg_palettes_hue_shift_to_action : public to_template_action<fixed, bg_palettes_hue_shift_manager>`

- **bg_palettes_hue_shift_to_action** `bg_palettes_hue_shift_to_action(int duration_updates, fixed final_intensity) : to_template_action(duration_updates, final_intensity)` (L694): Constructor.
  `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L703): Returns the intensity when the action is updated the given number of times.
### `bn::bg_palettes_hue_shift_loop_action` (class, bn_bg_palettes_actions.h:719)

Modifies the intensity of the hue shift effect applied to all background color palettes from a minimum to a maximum. When the intensity is equal to the given final state, it goes back to its initial state and vice versa.

`class bg_palettes_hue_shift_loop_action : public loop_template_action<fixed, bg_palettes_hue_shift_manager>`

- **bg_palettes_hue_shift_loop_action** `bg_palettes_hue_shift_loop_action(int duration_updates, fixed final_intensity) : loop_template_action(duration_updates, final_intensity)` (L732): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L742): When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::bg_palettes_hue_shift_toggle_action` (class, bn_bg_palettes_actions.h:757)

Changes the intensity of the hue shift effect applied to all background color palettes when the action is updated a given number of times.

`class bg_palettes_hue_shift_toggle_action : public toggle_template_action<fixed, bg_palettes_hue_shift_manager>`

- **bg_palettes_hue_shift_toggle_action** `bg_palettes_hue_shift_toggle_action(int duration_updates, fixed new_intensity) : toggle_template_action(duration_updates, new_intensity)` (L768): Constructor.
  `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **new_intensity** `fixed new_intensity() const` (L777): Returns the intensity when the action is updated the given number of times.
### `bn::bg_palettes_fade_manager` (class, bn_bg_palettes_actions.h:793)

Manages the intensity of the fade effect applied to all background color palettes.

`class bg_palettes_fade_manager`

- **get** `static fixed get()` (L800): Returns the intensity of the fade effect applied to all background color palettes.
- **set** `static void set(fixed intensity)` (L809): Sets the intensity of the fade effect applied to all background color palettes.
  `intensity` New intensity in the range [0..1].
### `bn::bg_palettes_fade_to_action` (class, bn_bg_palettes_actions.h:824)

Modifies the intensity of the fade effect applied to all background color palettes until it has a given state.

`class bg_palettes_fade_to_action : public to_template_action<fixed, bg_palettes_fade_manager>`

- **bg_palettes_fade_to_action** `bg_palettes_fade_to_action(int duration_updates, fixed final_intensity) : to_template_action(duration_updates, final_intensity)` (L836): Constructor.
  `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L845): Returns the intensity when the action is updated the given number of times.
### `bn::bg_palettes_fade_loop_action` (class, bn_bg_palettes_actions.h:861)

Modifies the intensity of the fade effect applied to all background color palettes from a minimum to a maximum. When the intensity is equal to the given final state, it goes back to its initial state and vice versa.

`class bg_palettes_fade_loop_action : public loop_template_action<fixed, bg_palettes_fade_manager>`

- **bg_palettes_fade_loop_action** `bg_palettes_fade_loop_action(int duration_updates, fixed final_intensity) : loop_template_action(duration_updates, final_intensity)` (L874): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L884): When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::bg_palettes_fade_toggle_action` (class, bn_bg_palettes_actions.h:899)

Changes the intensity of the fade effect applied to all background color palettes when the action is updated a given number of times.

`class bg_palettes_fade_toggle_action : public toggle_template_action<fixed, bg_palettes_fade_manager>`

- **bg_palettes_fade_toggle_action** `bg_palettes_fade_toggle_action(int duration_updates, fixed new_intensity) : toggle_template_action(duration_updates, new_intensity)` (L910): Constructor.
  `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **new_intensity** `fixed new_intensity() const` (L919): Returns the intensity when the action is updated the given number of times.

## bn_bgs_mosaic_actions.h

### `bn::bgs_mosaic_horizontal_stretch_manager` (class, bn_bgs_mosaic_actions.h:34)

Manages the horizontal stretch of the mosaic applied to the backgrounds.

`class bgs_mosaic_horizontal_stretch_manager`

- **get** `static fixed get()` (L41): Returns the horizontal stretch of the mosaic applied to the backgrounds.
- **set** `static void set(fixed horizontal_stretch)` (L50): Sets the horizontal stretch of the mosaic applied to the backgrounds.
  `horizontal_stretch` Horizontal stretch in the range [0..1].
### `bn::bgs_mosaic_horizontal_stretch_to_action` (class, bn_bgs_mosaic_actions.h:64)

Modifies the horizontal stretch of the mosaic applied to the backgrounds until it has a given state.

`class bgs_mosaic_horizontal_stretch_to_action : public to_template_action<fixed, bgs_mosaic_horizontal_stretch_manager>`

- **bgs_mosaic_horizontal_stretch_to_action** `bgs_mosaic_horizontal_stretch_to_action(int duration_updates, fixed final_horizontal_stretch) : to_template_action(duration_updates, final_horizontal_stretch)` (L77): Constructor.
  `duration_updates` Number of times that the action must be updated until the horizontal stretch is equal to final_horizontal_stretch.; `final_horizontal_stretch` Horizontal stretch when the action is updated duration_updates times.
  This horizontal stretch must be in the range [0..1].
- **final_horizontal_stretch** `fixed final_horizontal_stretch() const` (L87): Returns the horizontal stretch when the action is updated the given number of times.
### `bn::bgs_mosaic_horizontal_stretch_loop_action` (class, bn_bgs_mosaic_actions.h:102)

Modifies the horizontal stretch of the mosaic applied to the backgrounds from a minimum to a maximum. When the horizontal stretch is equal to the given final state, it goes back to its initial state and vice versa.

`class bgs_mosaic_horizontal_stretch_loop_action : public loop_template_action<fixed, bgs_mosaic_horizontal_stretch_manager>`

- **bgs_mosaic_horizontal_stretch_loop_action** `bgs_mosaic_horizontal_stretch_loop_action(int duration_updates, fixed final_horizontal_stretch) : loop_template_action(duration_updates, final_horizontal_stretch)` (L116): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the horizontal stretch delta.; `final_horizontal_stretch` When the horizontal stretch is equal to this parameter, it goes back to its initial state and vice versa.
  This horizontal stretch must be in the range [0..1].
- **final_horizontal_stretch** `fixed final_horizontal_stretch() const` (L127): When the horizontal stretch is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::bgs_mosaic_horizontal_stretch_toggle_action` (class, bn_bgs_mosaic_actions.h:142)

Changes the horizontal stretch of the mosaic applied to the backgrounds when the action is updated a given number of times.

`class bgs_mosaic_horizontal_stretch_toggle_action : public toggle_template_action<fixed, bgs_mosaic_horizontal_stretch_manager>`

- **bgs_mosaic_horizontal_stretch_toggle_action** `bgs_mosaic_horizontal_stretch_toggle_action(int duration_updates, fixed new_horizontal_stretch) : toggle_template_action(duration_updates, new_horizontal_stretch)` (L154): Constructor.
  `duration_updates` How many times the action has to be updated to change the horizontal stretch.; `new_horizontal_stretch` New horizontal stretch to set when the action is updated duration_updates times.
  This horizontal stretch must be in the range [0..1].
- **new_horizontal_stretch** `fixed new_horizontal_stretch() const` (L164): Returns the horizontal stretch to set when the action is updated the given number of times.
### `bn::bgs_mosaic_vertical_stretch_manager` (class, bn_bgs_mosaic_actions.h:180)

Manages the vertical stretch of the mosaic applied to the backgrounds.

`class bgs_mosaic_vertical_stretch_manager`

- **get** `static fixed get()` (L187): Returns the vertical stretch of the mosaic applied to the backgrounds.
- **set** `static void set(fixed vertical_stretch)` (L196): Sets the vertical stretch of the mosaic applied to the backgrounds.
  `vertical_stretch` Vertical stretch in the range [0..1].
### `bn::bgs_mosaic_vertical_stretch_to_action` (class, bn_bgs_mosaic_actions.h:210)

Modifies the vertical stretch of the mosaic applied to the backgrounds until it has a given state.

`class bgs_mosaic_vertical_stretch_to_action : public to_template_action<fixed, bgs_mosaic_vertical_stretch_manager>`

- **bgs_mosaic_vertical_stretch_to_action** `bgs_mosaic_vertical_stretch_to_action(int duration_updates, fixed final_vertical_stretch) : to_template_action(duration_updates, final_vertical_stretch)` (L222): Constructor.
  `duration_updates` Number of times that the action must be updated until the vertical stretch is equal to final_vertical_stretch.; `final_vertical_stretch` Vertical stretch when the action is updated duration_updates times.
  This vertical stretch must be in the range [0..1].
- **final_vertical_stretch** `fixed final_vertical_stretch() const` (L232): Returns the vertical stretch when the action is updated the given number of times.
### `bn::bgs_mosaic_vertical_stretch_loop_action` (class, bn_bgs_mosaic_actions.h:247)

Modifies the vertical stretch of the mosaic applied to the backgrounds from a minimum to a maximum. When the vertical stretch is equal to the given final state, it goes back to its initial state and vice versa.

`class bgs_mosaic_vertical_stretch_loop_action : public loop_template_action<fixed, bgs_mosaic_vertical_stretch_manager>`

- **bgs_mosaic_vertical_stretch_loop_action** `bgs_mosaic_vertical_stretch_loop_action(int duration_updates, fixed final_vertical_stretch) : loop_template_action(duration_updates, final_vertical_stretch)` (L261): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the vertical stretch delta.; `final_vertical_stretch` When the vertical stretch is equal to this parameter, it goes back to its initial state and vice versa.
  This vertical stretch must be in the range [0..1].
- **final_vertical_stretch** `fixed final_vertical_stretch() const` (L272): When the vertical stretch is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::bgs_mosaic_vertical_stretch_toggle_action` (class, bn_bgs_mosaic_actions.h:287)

Changes the vertical stretch of the mosaic applied to the backgrounds when the action is updated a given number of times.

`class bgs_mosaic_vertical_stretch_toggle_action : public toggle_template_action<fixed, bgs_mosaic_vertical_stretch_manager>`

- **bgs_mosaic_vertical_stretch_toggle_action** `bgs_mosaic_vertical_stretch_toggle_action(int duration_updates, fixed new_vertical_stretch) : toggle_template_action(duration_updates, new_vertical_stretch)` (L299): Constructor.
  `duration_updates` How many times the action has to be updated to change the vertical stretch.; `new_vertical_stretch` New vertical stretch to set when the action is updated duration_updates times.
  This vertical stretch must be in the range [0..1].
- **new_vertical_stretch** `fixed new_vertical_stretch() const` (L309): Returns the vertical stretch to set when the action is updated the given number of times.
### `bn::bgs_mosaic_stretch_manager` (class, bn_bgs_mosaic_actions.h:325)

Manages the stretch of the mosaic applied to the backgrounds.

`class bgs_mosaic_stretch_manager`

- **get** `static fixed get()` (L332): Returns the horizontal stretch of the mosaic applied to the backgrounds.
- **set** `static void set(fixed stretch)` (L341): Sets the stretch of the mosaic applied to the backgrounds.
  `stretch` Stretch in the range [0..1].
### `bn::bgs_mosaic_stretch_to_action` (class, bn_bgs_mosaic_actions.h:355)

Modifies the stretch of the mosaic applied to the backgrounds until it has a given state.

`class bgs_mosaic_stretch_to_action : public to_template_action<fixed, bgs_mosaic_stretch_manager>`

- **bgs_mosaic_stretch_to_action** `bgs_mosaic_stretch_to_action(int duration_updates, fixed final_stretch) : to_template_action(duration_updates, final_stretch)` (L367): Constructor.
  `duration_updates` Number of times that the action must be updated until the stretch is equal to final_stretch.; `final_stretch` Stretch when the action is updated duration_updates times.
  This stretch must be in the range [0..1].
- **final_stretch** `fixed final_stretch() const` (L376): Returns the stretch when the action is updated the given number of times.
### `bn::bgs_mosaic_stretch_loop_action` (class, bn_bgs_mosaic_actions.h:391)

Modifies the stretch of the mosaic applied to the backgrounds from a minimum to a maximum. When the stretch is equal to the given final state, it goes back to its initial state and vice versa.

`class bgs_mosaic_stretch_loop_action : public loop_template_action<fixed, bgs_mosaic_stretch_manager>`

- **bgs_mosaic_stretch_loop_action** `bgs_mosaic_stretch_loop_action(int duration_updates, fixed final_stretch) : loop_template_action(duration_updates, final_stretch)` (L404): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the stretch delta.; `final_stretch` When the stretch is equal to this parameter, it goes back to its initial state and vice versa.
  This stretch must be in the range [0..1].
- **final_stretch** `fixed final_stretch() const` (L413): When the stretch is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::bgs_mosaic_stretch_toggle_action` (class, bn_bgs_mosaic_actions.h:428)

Changes the stretch of the mosaic applied to the backgrounds when the action is updated a given number of times.

`class bgs_mosaic_stretch_toggle_action : public toggle_template_action<fixed, bgs_mosaic_stretch_manager>`

- **bgs_mosaic_stretch_toggle_action** `bgs_mosaic_stretch_toggle_action(int duration_updates, fixed new_stretch) : toggle_template_action(duration_updates, new_stretch)` (L439): Constructor.
  `duration_updates` How many times the action has to be updated to change the stretch.; `new_stretch` New stretch to set when the action is updated duration_updates times.
  This stretch must be in the range [0..1].
- **new_stretch** `fixed new_stretch() const` (L448): Returns the stretch to set when the action is updated the given number of times.

## bn_blending_actions.h

### `bn::blending_transparency_alpha_manager` (class, bn_blending_actions.h:32)

Manages the weight of the transparency blending.

`class blending_transparency_alpha_manager`

- **get** `static fixed get()` (L40): Returns the weight of the transparency blending, with 0 being fully transparent and 1 being fully visible.
- **set** `static void set(fixed transparency_alpha)` (L53): Sets the weight of the transparency blending, with 0 being fully transparent and 1 being fully visible.
  `transparency_alpha` Transparency weight in the range [0..1].
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
### `bn::blending_transparency_alpha_to_action` (class, bn_blending_actions.h:66)

Modifies the weight of the transparency blending until it has a given state.

`class blending_transparency_alpha_to_action : public to_template_action<fixed, blending_transparency_alpha_manager>`

- **blending_transparency_alpha_to_action** `blending_transparency_alpha_to_action(int duration_updates, fixed final_transparency_alpha) : to_template_action(duration_updates, final_transparency_alpha)` (L81): Constructor.
  `duration_updates` Number of times that the action must be updated until the weight of the transparency blending is equal to final_transparency_alpha.; `final_transparency_alpha` Weight of the transparency blending when the action is updated duration_updates times.
  This weight must be in the range [0..1].
  
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
- **final_transparency_alpha** `fixed final_transparency_alpha() const` (L91): Returns the weight of the transparency blending when the action is updated the given number of times.
### `bn::blending_transparency_alpha_loop_action` (class, bn_blending_actions.h:105)

Modifies the weight of the transparency blending from a minimum to a maximum. When the weight is equal to the given final state, it goes back to its initial state and vice versa.

`class blending_transparency_alpha_loop_action : public loop_template_action<fixed, blending_transparency_alpha_manager>`

- **blending_transparency_alpha_loop_action** `blending_transparency_alpha_loop_action(int duration_updates, fixed final_transparency_alpha) : loop_template_action(duration_updates, final_transparency_alpha)` (L121): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the weight delta.; `final_transparency_alpha` When the weight of the transparency blending is equal to this parameter, it goes back to its initial state and vice versa.
  This weight must be in the range [0..1].
  
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
- **final_transparency_alpha** `fixed final_transparency_alpha() const` (L132): When the weight of the transparency blending is equal to this parameter, it goes back to its initial state and vice versa.
### `bn::blending_transparency_alpha_toggle_action` (class, bn_blending_actions.h:145)

Changes the weight of the transparency blending when the action is updated a given number of times.

`class blending_transparency_alpha_toggle_action : public toggle_template_action<fixed, blending_transparency_alpha_manager>`

- **blending_transparency_alpha_toggle_action** `blending_transparency_alpha_toggle_action(int duration_updates, fixed new_transparency_alpha) : toggle_template_action(duration_updates, new_transparency_alpha)` (L161): Constructor.
  `duration_updates` How many times the action has to be updated to change the weight of the transparency blending.; `new_transparency_alpha` New weight of the transparency blending when the action is updated duration_updates times.
  This weight must be in the range [0..1].
  
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
- **new_transparency_alpha** `fixed new_transparency_alpha() const` (L171): Returns the weight of the transparency blending when the action is updated the given number of times.
### `bn::blending_intensity_alpha_manager` (class, bn_blending_actions.h:186)

Manages the weight of the intensity blending.

`class blending_intensity_alpha_manager`

- **get** `static fixed get()` (L193): Returns the weight of the intensity blending, with 0 being disabled and 1 being fully applied.
- **set** `static void set(fixed intensity_alpha)` (L205): Sets the weight of the intensity blending, with 0 being disabled and 1 being fully applied.
  `intensity_alpha` Intensity weight in the range [0..1].
  Keep in mind that intensity and fade blendings can't be enabled at the same time.
### `bn::blending_intensity_alpha_to_action` (class, bn_blending_actions.h:218)

Modifies the weight of the intensity blending until it has a given state.

`class blending_intensity_alpha_to_action : public to_template_action<fixed, blending_intensity_alpha_manager>`

- **blending_intensity_alpha_to_action** `blending_intensity_alpha_to_action(int duration_updates, fixed final_intensity_alpha) : to_template_action(duration_updates, final_intensity_alpha)` (L233): Constructor.
  `duration_updates` Number of times that the action must be updated until the weight of the intensity blending is equal to final_intensity_alpha.; `final_intensity_alpha` Weight of the intensity blending when the action is updated duration_updates times.
  This weight must be in the range [0..1].
  
  Keep in mind that intensity and fade blendings can't be enabled at the same time.
- **final_intensity_alpha** `fixed final_intensity_alpha() const` (L243): Returns the weight of the intensity blending when the action is updated the given number of times.
### `bn::blending_intensity_alpha_loop_action` (class, bn_blending_actions.h:257)

Modifies the weight of the intensity blending from a minimum to a maximum. When the weight is equal to the given final state, it goes back to its initial state and vice versa.

`class blending_intensity_alpha_loop_action : public loop_template_action<fixed, blending_intensity_alpha_manager>`

- **blending_intensity_alpha_loop_action** `blending_intensity_alpha_loop_action(int duration_updates, fixed final_intensity_alpha) : loop_template_action(duration_updates, final_intensity_alpha)` (L273): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the weight delta.; `final_intensity_alpha` When the weight of the intensity blending is equal to this parameter, it goes back to its initial state and vice versa.
  This weight must be in the range [0..1].
  
  Keep in mind that intensity and fade blendings can't be enabled at the same time.
- **final_intensity_alpha** `fixed final_intensity_alpha() const` (L284): When the weight of the intensity blending is equal to this parameter, it goes back to its initial state and vice versa.
### `bn::blending_intensity_alpha_toggle_action` (class, bn_blending_actions.h:297)

Changes the weight of the intensity blending when the action is updated a given number of times.

`class blending_intensity_alpha_toggle_action : public toggle_template_action<fixed, blending_intensity_alpha_manager>`

- **blending_intensity_alpha_toggle_action** `blending_intensity_alpha_toggle_action(int duration_updates, fixed new_intensity_alpha) : toggle_template_action(duration_updates, new_intensity_alpha)` (L313): Constructor.
  `duration_updates` How many times the action has to be updated to change the weight of the intensity blending.; `new_intensity_alpha` New weight of the intensity blending when the action is updated duration_updates times.
  This weight must be in the range [0..1].
  
  Keep in mind that intensity and fade blendings can't be enabled at the same time.
- **new_intensity_alpha** `fixed new_intensity_alpha() const` (L323): Returns the weight of the intensity blending when the action is updated the given number of times.
### `bn::blending_transparency_top_weight_manager` (class, bn_blending_actions.h:338)

Manages the weight of the transparency blending for the top layer.

`class blending_transparency_top_weight_manager`

- **get** `static fixed get()` (L346): Returns the weight of the transparency blending for the top layer, with 0 being fully transparent and 1 being fully visible.
- **set** `static void set(fixed transparency_top_weight)` (L359): Sets the weight of the transparency blending for the top layer, with 0 being fully transparent and 1 being fully visible.
  `transparency_top_weight` Transparency weight in the range [0..1].
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
### `bn::blending_transparency_top_weight_to_action` (class, bn_blending_actions.h:372)

Modifies the weight of the transparency blending for the top layer until it has a given state.

`class blending_transparency_top_weight_to_action : public to_template_action<fixed, blending_transparency_top_weight_manager>`

- **blending_transparency_top_weight_to_action** `blending_transparency_top_weight_to_action(int duration_updates, fixed final_transparency_top_weight) : to_template_action(duration_updates, final_transparency_top_weight)` (L388): Constructor.
  `duration_updates` Number of times that the action must be updated until the weight of the transparency blending for the top layer is equal to final_transparency_top_weight.; `final_transparency_top_weight` Weight of the transparency blending for the top layer when the action is updated duration_updates times.
  This weight must be in the range [0..1].
  
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
- **final_transparency_top_weight** `fixed final_transparency_top_weight() const` (L399): Returns the weight of the transparency blending for the top layer when the action is updated the given number of times.
### `bn::blending_transparency_top_weight_loop_action` (class, bn_blending_actions.h:413)

Modifies the weight of the transparency blending for the top layer from a minimum to a maximum. When the weight is equal to the given final state, it goes back to its initial state and vice versa.

`class blending_transparency_top_weight_loop_action : public loop_template_action<fixed, blending_transparency_top_weight_manager>`

- **blending_transparency_top_weight_loop_action** `blending_transparency_top_weight_loop_action(int duration_updates, fixed final_transparency_top_weight) : loop_template_action(duration_updates, final_transparency_top_weight)` (L429): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the weight delta.; `final_transparency_top_weight` When the weight of the transparency blending for the top layer is equal to this parameter, it goes back to its initial state and vice versa.
  This weight must be in the range [0..1].
  
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
- **final_transparency_top_weight** `fixed final_transparency_top_weight() const` (L440): When the weight of the transparency blending for the top layer is equal to this parameter, it goes back to its initial state and vice versa.
### `bn::blending_transparency_top_weight_toggle_action` (class, bn_blending_actions.h:454)

Changes the weight of the transparency blending for the top layer when the action is updated a given number of times.

`class blending_transparency_top_weight_toggle_action : public toggle_template_action<fixed, blending_transparency_top_weight_manager>`

- **blending_transparency_top_weight_toggle_action** `blending_transparency_top_weight_toggle_action(int duration_updates, fixed new_transparency_top_weight) : toggle_template_action(duration_updates, new_transparency_top_weight)` (L470): Constructor.
  `duration_updates` How many times the action has to be updated to change the weight of the transparency blending for the top layer.; `new_transparency_top_weight` New weight of the transparency blending for the top layer when the action is updated duration_updates times.
  This weight must be in the range [0..1].
  
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
- **new_transparency_top_weight** `fixed new_transparency_top_weight() const` (L481): Returns the weight of the transparency blending for the top layer when the action is updated the given number of times.
### `bn::blending_transparency_bottom_weight_manager` (class, bn_blending_actions.h:496)

Manages the weight of the transparency blending for the bottom layer.

`class blending_transparency_bottom_weight_manager`

- **get** `static fixed get()` (L504): Returns the weight of the transparency blending for the bottom layer, with 0 being fully transparent and 1 being fully visible.
- **set** `static void set(fixed transparency_bottom_weight)` (L517): Sets the weight of the transparency blending for the bottom layer, with 0 being fully transparent and 1 being fully visible.
  `transparency_bottom_weight` Transparency weight in the range [0..1].
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
### `bn::blending_transparency_bottom_weight_to_action` (class, bn_blending_actions.h:530)

Modifies the weight of the transparency blending for the bottom layer until it has a given state.

`class blending_transparency_bottom_weight_to_action : public to_template_action<fixed, blending_transparency_bottom_weight_manager>`

- **blending_transparency_bottom_weight_to_action** `blending_transparency_bottom_weight_to_action(int duration_updates, fixed final_transparency_bottom_weight) : to_template_action(duration_updates, final_transparency_bottom_weight)` (L547): Constructor.
  `duration_updates` Number of times that the action must be updated until the weight of the transparency blending for the bottom layer is equal to final_transparency_bottom_weight.; `final_transparency_bottom_weight` Weight of the transparency blending for the bottom layer when the action is updated duration_updates times.
  This weight must be in the range [0..1].
  
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
- **final_transparency_bottom_weight** `fixed final_transparency_bottom_weight() const` (L558): Returns the weight of the transparency blending for the bottom layer when the action is updated the given number of times.
### `bn::blending_transparency_bottom_weight_loop_action` (class, bn_blending_actions.h:572)

Modifies the weight of the transparency blending for the bottom layer from a minimum to a maximum. When the weight is equal to the given final state, it goes back to its initial state and vice versa.

`class blending_transparency_bottom_weight_loop_action : public loop_template_action<fixed, blending_transparency_bottom_weight_manager>`

- **blending_transparency_bottom_weight_loop_action** `blending_transparency_bottom_weight_loop_action(int duration_updates, fixed final_transparency_bottom_weight) : loop_template_action(duration_updates, final_transparency_bottom_weight)` (L588): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the weight delta.; `final_transparency_bottom_weight` When the weight of the transparency blending for the bottom layer is equal to this parameter, it goes back to its initial state and vice versa.
  This weight must be in the range [0..1].
  
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
- **final_transparency_bottom_weight** `fixed final_transparency_bottom_weight() const` (L599): When the weight of the transparency blending for the bottom layer is equal to this parameter, it goes back to its initial state and vice versa.
### `bn::blending_transparency_bottom_weight_toggle_action` (class, bn_blending_actions.h:613)

Changes the weight of the transparency blending for the bottom layer when the action is updated a given number of times.

`class blending_transparency_bottom_weight_toggle_action : public toggle_template_action<fixed, blending_transparency_bottom_weight_manager>`

- **blending_transparency_bottom_weight_toggle_action** `blending_transparency_bottom_weight_toggle_action(int duration_updates, fixed new_transparency_bottom_weight) : toggle_template_action(duration_updates, new_transparency_bottom_weight)` (L629): Constructor.
  `duration_updates` How many times the action has to be updated to change the weight of the transparency blending for the bottom layer.; `new_transparency_bottom_weight` New weight of the transparency blending for the bottom layer when the action is updated duration_updates times.
  This weight must be in the range [0..1].
  
  Keep in mind that transparency and fade blendings can't be enabled at the same time.
- **new_transparency_bottom_weight** `fixed new_transparency_bottom_weight() const` (L640): Returns the weight of the transparency blending for the bottom layer when the action is updated the given number of times.
### `bn::blending_fade_alpha_manager` (class, bn_blending_actions.h:655)

Manages the weight of the fade blending.

`class blending_fade_alpha_manager`

- **get** `static fixed get()` (L662): Returns the weight of the fade blending, with 0 being disabled and 1 being fully applied.
- **set** `static void set(fixed fade_alpha)` (L674): Sets the weight of the fade blending, with 0 being disabled and 1 being fully applied.
  `fade_alpha` Fade weight in the range [0..1].
  Keep in mind that fade blending and other blendings can't be enabled at the same time.
### `bn::blending_fade_alpha_to_action` (class, bn_blending_actions.h:687)

Modifies the weight of the fade blending until it has a given state.

`class blending_fade_alpha_to_action : public to_template_action<fixed, blending_fade_alpha_manager>`

- **blending_fade_alpha_to_action** `blending_fade_alpha_to_action(int duration_updates, fixed final_fade_alpha) : to_template_action(duration_updates, final_fade_alpha)` (L701): Constructor.
  `duration_updates` Number of times that the action must be updated until the weight of the fade blending is equal to final_fade_alpha.; `final_fade_alpha` Weight of the fade blending when the action is updated duration_updates times.
  This weight must be in the range [0..1].
  
  Keep in mind that fade blending and other blendings can't be enabled at the same time.
- **final_fade_alpha** `fixed final_fade_alpha() const` (L710): Returns the weight of the fade blending when the action is updated the given number of times.
### `bn::blending_fade_alpha_loop_action` (class, bn_blending_actions.h:724)

Modifies the weight of the fade blending from a minimum to a maximum. When the weight is equal to the given final state, it goes back to its initial state and vice versa.

`class blending_fade_alpha_loop_action : public loop_template_action<fixed, blending_fade_alpha_manager>`

- **blending_fade_alpha_loop_action** `blending_fade_alpha_loop_action(int duration_updates, fixed final_fade_alpha) : loop_template_action(duration_updates, final_fade_alpha)` (L740): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the weight delta.; `final_fade_alpha` When the weight of the fade blending is equal to this parameter, it goes back to its initial state and vice versa.
  This weight must be in the range [0..1].
  
  Keep in mind that fade blending and other blendings can't be enabled at the same time.
- **final_fade_alpha** `fixed final_fade_alpha() const` (L750): When the weight of the fade blending is equal to this parameter, it goes back to its initial state and vice versa.
### `bn::blending_fade_alpha_toggle_action` (class, bn_blending_actions.h:763)

Changes the weight of the fade blending when the action is updated a given number of times.

`class blending_fade_alpha_toggle_action : public toggle_template_action<fixed, blending_fade_alpha_manager>`

- **blending_fade_alpha_toggle_action** `blending_fade_alpha_toggle_action(int duration_updates, fixed new_fade_alpha) : toggle_template_action(duration_updates, new_fade_alpha)` (L778): Constructor.
  `duration_updates` How many times the action has to be updated to change the weight of the fade blending.; `new_fade_alpha` New weight of the fade blending when the action is updated duration_updates times.
  This weight must be in the range [0..1].
  
  Keep in mind that fade blending and other blendings can't be enabled at the same time.
- **new_fade_alpha** `fixed new_fade_alpha() const` (L787): Returns the weight of the fade blending when the action is updated the given number of times.

## bn_camera_actions.h

### `bn::camera_position_manager` (class, bn_camera_actions.h:31)

Manages the position of a camera_ptr.

`class camera_position_manager`

- **get** `static const fixed_point& get(const camera_ptr& camera)` (L38): Returns the position of the given camera_ptr.
- **set** `static void set(const fixed_point& position, camera_ptr& camera)` (L46): Sets the position of the given camera_ptr.
### `bn::camera_move_by_action` (class, bn_camera_actions.h:59)

Modifies the position of a camera_ptr by a given delta.

`class camera_move_by_action : public by_value_template_action<camera_ptr, fixed_point, camera_position_manager>`

- **camera_move_by_action** `camera_move_by_action(const camera_ptr& camera, fixed delta_x, fixed delta_y) : by_value_template_action(camera, fixed_point(delta_x, delta_y))` (L69): Constructor.
  `camera` camera_ptr to copy.; `delta_x` How much to add to the horizontal position of the given camera_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the given camera_ptr when the action is updated.
- **camera_move_by_action** `camera_move_by_action(camera_ptr&& camera, fixed delta_x, fixed delta_y) : by_value_template_action(move(camera), fixed_point(delta_x, delta_y))` (L80): Constructor.
  `camera` camera_ptr to move.; `delta_x` How much to add to the horizontal position of the given camera_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the given camera_ptr when the action is updated.
- **camera_move_by_action** `camera_move_by_action(const camera_ptr& camera, const fixed_point& delta_position) : by_value_template_action(camera, delta_position)` (L90): Constructor.
  `camera` camera_ptr to copy.; `delta_position` How much to add to the position of the given camera_ptr when the action is updated.
- **camera_move_by_action** `camera_move_by_action(camera_ptr&& camera, const fixed_point& delta_position) : by_value_template_action(move(camera), delta_position)` (L100): Constructor.
  `camera` camera_ptr to move.; `delta_position` How much to add to the position of the given camera_ptr when the action is updated.
- **camera** `const camera_ptr& camera() const` (L108): Returns the camera_ptr to modify.
- **delta_position** `const fixed_point& delta_position() const` (L116): Returns how much to add to the position of the given camera_ptr when the action is updated.
### `bn::camera_move_to_action` (class, bn_camera_actions.h:129)

Modifies the position of a camera_ptr until it has a given state.

`class camera_move_to_action : public to_value_template_action<camera_ptr, fixed_point, camera_position_manager>`

- **camera_move_to_action** `camera_move_to_action(const camera_ptr& camera, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(camera, duration_updates, fixed_point(final_x, final_y))` (L141): Constructor.
  `camera` camera_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the given camera_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the given camera_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the given camera_ptr when the action is updated duration_updates times.
- **camera_move_to_action** `camera_move_to_action(camera_ptr&& camera, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(move(camera), duration_updates, fixed_point(final_x, final_y))` (L154): Constructor.
  `camera` camera_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the given camera_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the given camera_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the given camera_ptr when the action is updated duration_updates times.
- **camera_move_to_action** `camera_move_to_action(const camera_ptr& camera, int duration_updates, const fixed_point& final_position) : to_value_template_action(camera, duration_updates, final_position)` (L166): Constructor.
  `camera` camera_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the given camera_ptr is equal to final_position.; `final_position` Position of the given camera_ptr when the action is updated duration_updates times.
- **camera_move_to_action** `camera_move_to_action(camera_ptr&& camera, int duration_updates, const fixed_point& final_position) : to_value_template_action(move(camera), duration_updates, final_position)` (L178): Constructor.
  `camera` camera_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the given camera_ptr is equal to final_position.; `final_position` Position of the given camera_ptr when the action is updated duration_updates times.
- **camera** `const camera_ptr& camera() const` (L186): Returns the camera_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L194): Returns the position of the given camera_ptr when the action is updated the given number of times.
### `bn::camera_move_loop_action` (class, bn_camera_actions.h:208)

Modifies the position of a camera_ptr from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class camera_move_loop_action : public loop_value_template_action<camera_ptr, fixed_point, camera_position_manager>`

- **camera_move_loop_action** `camera_move_loop_action(const camera_ptr& camera, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(camera, duration_updates, fixed_point(final_x, final_y))` (L222): Constructor.
  `camera` camera_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the given camera_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the given camera_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **camera_move_loop_action** `camera_move_loop_action(camera_ptr&& camera, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(move(camera), duration_updates, fixed_point(final_x, final_y))` (L237): Constructor.
  `camera` camera_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the given camera_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the given camera_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **camera_move_loop_action** `camera_move_loop_action(const camera_ptr& camera, int duration_updates, const fixed_point& final_position) : loop_value_template_action(camera, duration_updates, final_position)` (L250): Constructor.
  `camera` camera_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the given camera_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **camera_move_loop_action** `camera_move_loop_action(camera_ptr&& camera, int duration_updates, const fixed_point& final_position) : loop_value_template_action(move(camera), duration_updates, final_position)` (L263): Constructor.
  `camera` camera_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the given camera_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **camera** `const camera_ptr& camera() const` (L271): Returns the camera_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L280): When the position of the given camera_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::camera_move_toggle_action` (class, bn_camera_actions.h:293)

Changes the position of a camera_ptr when the action is updated a given number of times.

`class camera_move_toggle_action : public toggle_value_template_action<camera_ptr, fixed_point, camera_position_manager>`

- **camera_move_toggle_action** `camera_move_toggle_action(const camera_ptr& camera, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(camera, duration_updates, fixed_point(new_x, new_y))` (L306): Constructor.
  `camera` camera_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the given camera_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **camera_move_toggle_action** `camera_move_toggle_action(camera_ptr&& camera, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(move(camera), duration_updates, fixed_point(new_x, new_y))` (L319): Constructor.
  `camera` camera_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the given camera_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **camera_move_toggle_action** `camera_move_toggle_action(const camera_ptr& camera, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(camera, duration_updates, new_position)` (L331): Constructor.
  `camera` camera_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the given camera_ptr.; `new_position` New position when the action is updated duration_updates times.
- **camera_move_toggle_action** `camera_move_toggle_action(camera_ptr&& camera, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(move(camera), duration_updates, new_position)` (L343): Constructor.
  `camera` camera_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the given camera_ptr.; `new_position` New position when the action is updated duration_updates times.
- **camera** `const camera_ptr& camera() const` (L351): Returns the camera_ptr to modify.
- **new_position** `const fixed_point& new_position() const` (L359): Returns the position of the given camera_ptr when the action is updated the given number of times.

## bn_dmg_music_actions.h

### `bn::dmg_music_volume_manager` (class, bn_dmg_music_actions.h:30)

Manages the volume of the active DMG music.

`class dmg_music_volume_manager`

- **get** `static fixed_point get()` (L37): Returns the volume of the active DMG music for both speakers.
- **set** `static void set(const fixed_point& volume)` (L49): Sets the volume of the active DMG music for both speakers.
  `volume` New volume in the range [0..1].
  Volume change is not supported by the VGM player.
### `bn::dmg_music_volume_to_action` (class, bn_dmg_music_actions.h:64)

Modifies the volume of the active DMG music until it has a given state.

`class dmg_music_volume_to_action : public to_template_action<fixed_point, dmg_music_volume_manager>`

Volume change is not supported by the VGM player.

- **dmg_music_volume_to_action** `dmg_music_volume_to_action(int duration_updates, fixed final_volume) : to_template_action(duration_updates, fixed_point(final_volume, final_volume))` (L76): Constructor.
  `duration_updates` Number of times that the action must be updated until the volume is equal to final_volume.; `final_volume` Volume for both speakers when the action is updated duration_updates times.
  This volume must be in the range [0..1].
- **dmg_music_volume_to_action** `dmg_music_volume_to_action(int duration_updates, fixed final_left_volume, fixed final_right_volume) : to_template_action(duration_updates, fixed_point(final_left_volume, final_right_volume))` (L93): Constructor.
  `duration_updates` Number of times that the action must be updated until the volume is equal to final_left_volume and final_right_volume.; `final_left_volume` Volume for the left speaker when the action is updated duration_updates times.; `final_right_volume` Volume for the right speaker when the action is updated duration_updates times.
  This volume must be in the range [0..1].
  This volume must be in the range [0..1].
- **final_left_volume** `fixed final_left_volume() const` (L105): Returns the volume for the left speaker when the action is updated the given number of times.
- **final_right_volume** `fixed final_right_volume() const` (L113): Returns the volume for the right speaker when the action is updated the given number of times.
### `bn::dmg_music_volume_loop_action` (class, bn_dmg_music_actions.h:129)

Modifies the volume of the active DMG music from a minimum to a maximum. When the volume is equal to the given final state, it goes back to its initial state and vice versa.

`class dmg_music_volume_loop_action : public loop_template_action<fixed_point, dmg_music_volume_manager>`

Volume change is not supported by the VGM player.

- **dmg_music_volume_loop_action** `dmg_music_volume_loop_action(int duration_updates, fixed final_volume) : loop_template_action(duration_updates, fixed_point(final_volume, final_volume))` (L142): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the volume delta.; `final_volume` When the volume for both speakers is equal to this parameter, it goes back to its initial state and vice versa.
  This volume must be in the range [0..1].
- **dmg_music_volume_loop_action** `dmg_music_volume_loop_action(int duration_updates, fixed final_left_volume, fixed final_right_volume) : loop_template_action(duration_updates, fixed_point(final_left_volume, final_right_volume))` (L161): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the volume delta.; `final_left_volume` When the volume for the left speaker is equal to this parameter, it goes back to its initial state and vice versa.; `final_right_volume` When the volume for the right speaker is equal to this parameter, it goes back to its initial state and vice versa.
  This volume must be in the range [0..1].
  This volume must be in the range [0..1].
- **final_left_volume** `fixed final_left_volume() const` (L174): When the volume for the left speaker is equal to the returned parameter, it goes back to its initial state and vice versa.
- **final_right_volume** `fixed final_right_volume() const` (L183): When the volume for the right speaker is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::dmg_music_volume_toggle_action` (class, bn_dmg_music_actions.h:198)

Changes the volume of the active DMG music when the action is updated a given number of times.

`class dmg_music_volume_toggle_action : public toggle_template_action<fixed_point, dmg_music_volume_manager>`

Volume change is not supported by the VGM player.

- **dmg_music_volume_toggle_action** `dmg_music_volume_toggle_action(int duration_updates, fixed new_volume) : toggle_template_action(duration_updates, fixed_point(new_volume, new_volume))` (L209): Constructor.
  `duration_updates` How many times the action has to be updated to change the volume.; `new_volume` New volume for both speakers when the action is updated duration_updates times.
  This volume must be in the range [0..1].
- **dmg_music_volume_toggle_action** `dmg_music_volume_toggle_action(int duration_updates, fixed new_left_volume, fixed new_right_volume) : toggle_template_action(duration_updates, fixed_point(new_left_volume, new_right_volume))` (L225): Constructor.
  `duration_updates` How many times the action has to be updated to change the volume.; `new_left_volume` New volume for the left speaker when the action is updated duration_updates times.; `new_right_volume` New volume for the right speaker when the action is updated duration_updates times.
  This volume must be in the range [0..1].
  This volume must be in the range [0..1].
- **new_left_volume** `fixed new_left_volume() const` (L235): Returns the volume for the left speaker when the action is updated the given number of times.
- **new_right_volume** `fixed new_right_volume() const` (L243): Returns the volume for the right speaker when the action is updated the given number of times.

## bn_dp_direct_bitmap_bg_actions.h

### `bn::dp_direct_bitmap_bg_visible_manager` (class, bn_dp_direct_bitmap_bg_actions.h:31)

Manages if a dp_direct_bitmap_bg_ptr must be committed to the GBA or not.

`class dp_direct_bitmap_bg_visible_manager`

- **get** `static bool get(const dp_direct_bitmap_bg_ptr& bg)` (L38): Indicates if the given dp_direct_bitmap_bg_ptr is committed to the GBA or not.
- **set** `static void set(bool visible, dp_direct_bitmap_bg_ptr& bg)` (L46): Sets if the given dp_direct_bitmap_bg_ptr must be committed to the GBA or not.
### `bn::dp_direct_bitmap_bg_visible_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:60)

Toggles if a dp_direct_bitmap_bg_ptr must be committed to the GBA or not when the action is updated a given number of times.

`class dp_direct_bitmap_bg_visible_toggle_action : public bool_toggle_value_template_action<dp_direct_bitmap_bg_ptr, dp_direct_bitmap_bg_visible_manager>`

- **dp_direct_bitmap_bg_visible_toggle_action** `dp_direct_bitmap_bg_visible_toggle_action(const dp_direct_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L71): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given dp_direct_bitmap_bg_ptr must be committed to the GBA or not.
- **dp_direct_bitmap_bg_visible_toggle_action** `dp_direct_bitmap_bg_visible_toggle_action(dp_direct_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L82): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given dp_direct_bitmap_bg_ptr must be committed to the GBA or not.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L90): Returns the dp_direct_bitmap_bg_ptr to modify.
### `bn::dp_direct_bitmap_bg_position_manager` (class, bn_dp_direct_bitmap_bg_actions.h:105)

Manages the position of a dp_direct_bitmap_bg_ptr.

`class dp_direct_bitmap_bg_position_manager`

- **get** `static const fixed_point& get(const dp_direct_bitmap_bg_ptr& bg)` (L112): Returns the position of the given dp_direct_bitmap_bg_ptr.
- **set** `static void set(const fixed_point& position, dp_direct_bitmap_bg_ptr& bg)` (L120): Sets the position of the given dp_direct_bitmap_bg_ptr.
### `bn::dp_direct_bitmap_bg_move_by_action` (class, bn_dp_direct_bitmap_bg_actions.h:133)

Modifies the position of a dp_direct_bitmap_bg_ptr by a given delta.

`class dp_direct_bitmap_bg_move_by_action : public by_value_template_action<dp_direct_bitmap_bg_ptr, fixed_point, dp_direct_bitmap_bg_position_manager>`

- **dp_direct_bitmap_bg_move_by_action** `dp_direct_bitmap_bg_move_by_action(const dp_direct_bitmap_bg_ptr& bg, fixed delta_x, fixed delta_y) : by_value_template_action(bg, fixed_point(delta_x, delta_y))` (L146): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `delta_x` How much to add to the horizontal position of the given dp_direct_bitmap_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the given dp_direct_bitmap_bg_ptr when the action is updated.
- **dp_direct_bitmap_bg_move_by_action** `dp_direct_bitmap_bg_move_by_action(dp_direct_bitmap_bg_ptr&& bg, fixed delta_x, fixed delta_y) : by_value_template_action(move(bg), fixed_point(delta_x, delta_y))` (L159): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `delta_x` How much to add to the horizontal position of the given dp_direct_bitmap_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the given dp_direct_bitmap_bg_ptr when the action is updated.
- **dp_direct_bitmap_bg_move_by_action** `dp_direct_bitmap_bg_move_by_action(const dp_direct_bitmap_bg_ptr& bg, const fixed_point& delta_position) : by_value_template_action(bg, delta_position)` (L170): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `delta_position` How much to add to the position of the given dp_direct_bitmap_bg_ptr when the action is updated.
- **dp_direct_bitmap_bg_move_by_action** `dp_direct_bitmap_bg_move_by_action(dp_direct_bitmap_bg_ptr&& bg, const fixed_point& delta_position) : by_value_template_action(move(bg), delta_position)` (L181): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `delta_position` How much to add to the position of the given dp_direct_bitmap_bg_ptr when the action is updated.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L189): Returns the dp_direct_bitmap_bg_ptr to modify.
- **delta_position** `const fixed_point& delta_position() const` (L197): Returns how much to add to the position of the given dp_direct_bitmap_bg_ptr when the action is updated.
### `bn::dp_direct_bitmap_bg_move_to_action` (class, bn_dp_direct_bitmap_bg_actions.h:210)

Modifies the position of a dp_direct_bitmap_bg_ptr until it has a given state.

`class dp_direct_bitmap_bg_move_to_action : public to_value_template_action<dp_direct_bitmap_bg_ptr, fixed_point, dp_direct_bitmap_bg_position_manager>`

- **dp_direct_bitmap_bg_move_to_action** `dp_direct_bitmap_bg_move_to_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L225): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the given dp_direct_bitmap_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_move_to_action** `dp_direct_bitmap_bg_move_to_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L241): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the given dp_direct_bitmap_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_move_to_action** `dp_direct_bitmap_bg_move_to_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(bg, duration_updates, final_position)` (L255): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the given dp_direct_bitmap_bg_ptr is equal to final_position.; `final_position` Position of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_move_to_action** `dp_direct_bitmap_bg_move_to_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(move(bg), duration_updates, final_position)` (L269): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the given dp_direct_bitmap_bg_ptr is equal to final_position.; `final_position` Position of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L278): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L287): Returns the position of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_move_loop_action` (class, bn_dp_direct_bitmap_bg_actions.h:301)

Modifies the position of a dp_direct_bitmap_bg_ptr from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class dp_direct_bitmap_bg_move_loop_action : public loop_value_template_action<dp_direct_bitmap_bg_ptr, fixed_point, dp_direct_bitmap_bg_position_manager>`

- **dp_direct_bitmap_bg_move_loop_action** `dp_direct_bitmap_bg_move_loop_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L316): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **dp_direct_bitmap_bg_move_loop_action** `dp_direct_bitmap_bg_move_loop_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L332): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **dp_direct_bitmap_bg_move_loop_action** `dp_direct_bitmap_bg_move_loop_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(bg, duration_updates, final_position)` (L346): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **dp_direct_bitmap_bg_move_loop_action** `dp_direct_bitmap_bg_move_loop_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(move(bg), duration_updates, final_position)` (L360): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L369): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L378): When the position of the given dp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::dp_direct_bitmap_bg_move_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:391)

Changes the position of a dp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class dp_direct_bitmap_bg_move_toggle_action : public toggle_value_template_action<dp_direct_bitmap_bg_ptr, fixed_point, dp_direct_bitmap_bg_position_manager>`

- **dp_direct_bitmap_bg_move_toggle_action** `dp_direct_bitmap_bg_move_toggle_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(bg, duration_updates, fixed_point(new_x, new_y))` (L404): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the given dp_direct_bitmap_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_move_toggle_action** `dp_direct_bitmap_bg_move_toggle_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(move(bg), duration_updates, fixed_point(new_x, new_y))` (L418): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the given dp_direct_bitmap_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_move_toggle_action** `dp_direct_bitmap_bg_move_toggle_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(bg, duration_updates, new_position)` (L431): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the given dp_direct_bitmap_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_move_toggle_action** `dp_direct_bitmap_bg_move_toggle_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(move(bg), duration_updates, new_position)` (L444): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the given dp_direct_bitmap_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L453): Returns the dp_direct_bitmap_bg_ptr to modify.
- **new_position** `const fixed_point& new_position() const` (L462): Returns the position of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_top_left_position_manager` (class, bn_dp_direct_bitmap_bg_actions.h:477)

Manages the top-left position of a dp_direct_bitmap_bg_ptr.

`class dp_direct_bitmap_bg_top_left_position_manager`

- **get** `static fixed_point get(const dp_direct_bitmap_bg_ptr& bg)` (L484): Returns the top-left position of the given dp_direct_bitmap_bg_ptr.
- **set** `static void set(const fixed_point& top_left_position, dp_direct_bitmap_bg_ptr& bg)` (L492): Sets the top-left position of the given dp_direct_bitmap_bg_ptr.
### `bn::dp_direct_bitmap_bg_top_left_move_to_action` (class, bn_dp_direct_bitmap_bg_actions.h:505)

Modifies the top-left position of a dp_direct_bitmap_bg_ptr until it has a given state.

`class dp_direct_bitmap_bg_top_left_move_to_action : public to_value_template_action<dp_direct_bitmap_bg_ptr, fixed_point, dp_direct_bitmap_bg_top_left_position_manager>`

- **dp_direct_bitmap_bg_top_left_move_to_action** `dp_direct_bitmap_bg_top_left_move_to_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : to_value_template_action(bg, duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L520): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the top-left position of the given dp_direct_bitmap_bg_ptr is equal to (final_x, final_y).; `final_top_left_x` Horizontal top-left position of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.; `final_top_left_y` Vertical top-left position of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_top_left_move_to_action** `dp_direct_bitmap_bg_top_left_move_to_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : to_value_template_action(move(bg), duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L536): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the top-left position of the given dp_direct_bitmap_bg_ptr is equal to (final_x, final_y).; `final_top_left_x` Horizontal top-left position of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.; `final_top_left_y` Vertical top-left position of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_top_left_move_to_action** `dp_direct_bitmap_bg_top_left_move_to_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_top_left_position) : to_value_template_action(bg, duration_updates, final_top_left_position)` (L550): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the top-left position of the given dp_direct_bitmap_bg_ptr is equal to final_top_left_position.; `final_top_left_position` Top-left position of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_top_left_move_to_action** `dp_direct_bitmap_bg_top_left_move_to_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_top_left_position) : to_value_template_action(move(bg), duration_updates, final_top_left_position)` (L564): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the top-left position of the given dp_direct_bitmap_bg_ptr is equal to final_top_left_position.; `final_top_left_position` Top-left position of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L573): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_top_left_position** `const fixed_point& final_top_left_position() const` (L582): Returns the top-left position of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_top_left_move_loop_action` (class, bn_dp_direct_bitmap_bg_actions.h:596)

Modifies the top-left position of a dp_direct_bitmap_bg_ptr from a minimum to a maximum. When the top-left position is equal to the given final state, it goes back to its initial state and vice versa.

`class dp_direct_bitmap_bg_top_left_move_loop_action : public loop_value_template_action<dp_direct_bitmap_bg_ptr, fixed_point, dp_direct_bitmap_bg_top_left_position_manager>`

- **dp_direct_bitmap_bg_top_left_move_loop_action** `dp_direct_bitmap_bg_top_left_move_loop_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : loop_value_template_action(bg, duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L611): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_x` When the horizontal top-left position of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_top_left_y` When the vertical top-left position of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **dp_direct_bitmap_bg_top_left_move_loop_action** `dp_direct_bitmap_bg_top_left_move_loop_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : loop_value_template_action(move(bg), duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L627): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_x` When the horizontal top-left position of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_top_left_y` When the vertical top-left position of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **dp_direct_bitmap_bg_top_left_move_loop_action** `dp_direct_bitmap_bg_top_left_move_loop_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_top_left_position) : loop_value_template_action(bg, duration_updates, final_top_left_position)` (L641): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_position` When the top-left position of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **dp_direct_bitmap_bg_top_left_move_loop_action** `dp_direct_bitmap_bg_top_left_move_loop_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_top_left_position) : loop_value_template_action(move(bg), duration_updates, final_top_left_position)` (L655): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_position` When the top-left position of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L664): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_top_left_position** `const fixed_point& final_top_left_position() const` (L673): When the top-left position of the given dp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::dp_direct_bitmap_bg_top_left_move_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:686)

Changes the top-left position of a dp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class dp_direct_bitmap_bg_top_left_move_toggle_action : public toggle_value_template_action<dp_direct_bitmap_bg_ptr, fixed_point, dp_direct_bitmap_bg_top_left_position_manager>`

- **dp_direct_bitmap_bg_top_left_move_toggle_action** `dp_direct_bitmap_bg_top_left_move_toggle_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) : toggle_value_template_action(bg, duration_updates, fixed_point(new_top_left_x, new_top_left_y))` (L699): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the top-left position of the given dp_direct_bitmap_bg_ptr.; `new_top_left_x` New horizontal top-left position when the action is updated duration_updates times.; `new_top_left_y` New vertical top-left position when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_top_left_move_toggle_action** `dp_direct_bitmap_bg_top_left_move_toggle_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) : toggle_value_template_action(move(bg), duration_updates, fixed_point(new_top_left_x, new_top_left_y))` (L713): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the top-left position of the given dp_direct_bitmap_bg_ptr.; `new_top_left_x` New horizontal top-left position when the action is updated duration_updates times.; `new_top_left_y` New vertical top-left position when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_top_left_move_toggle_action** `dp_direct_bitmap_bg_top_left_move_toggle_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& new_top_left_position) : toggle_value_template_action(bg, duration_updates, new_top_left_position)` (L726): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the top-left position of the given dp_direct_bitmap_bg_ptr.; `new_top_left_position` New top-left position when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_top_left_move_toggle_action** `dp_direct_bitmap_bg_top_left_move_toggle_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& new_top_left_position) : toggle_value_template_action(move(bg), duration_updates, new_top_left_position)` (L739): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the top-left position of the given dp_direct_bitmap_bg_ptr.; `new_top_left_position` New top-left position when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L748): Returns the dp_direct_bitmap_bg_ptr to modify.
- **new_top_left_position** `const fixed_point& new_top_left_position() const` (L757): Returns the top-left position of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_rotation_manager` (class, bn_dp_direct_bitmap_bg_actions.h:772)

Manages the rotation angle of a dp_direct_bitmap_bg_ptr.

`class dp_direct_bitmap_bg_rotation_manager`

- **get** `static fixed get(const dp_direct_bitmap_bg_ptr& bg)` (L779): Returns the rotation angle of the given dp_direct_bitmap_bg_ptr.
- **set** `static void set(fixed rotation_angle, dp_direct_bitmap_bg_ptr& bg)` (L789): Sets the rotation angle of the given dp_direct_bitmap_bg_ptr.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].; `bg` dp_direct_bitmap_bg_ptr to modify.
### `bn::dp_direct_bitmap_bg_rotate_by_action` (class, bn_dp_direct_bitmap_bg_actions.h:803)

Modifies the rotation angle of a dp_direct_bitmap_bg_ptr by delta_rotation_angle. When the rotation angle is over 360, it goes back to 0 and vice versa.

`class dp_direct_bitmap_bg_rotate_by_action : public cyclic_by_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_rotation_manager>`

- **dp_direct_bitmap_bg_rotate_by_action** `dp_direct_bitmap_bg_rotate_by_action(const dp_direct_bitmap_bg_ptr& bg, fixed delta_rotation_angle) : cyclic_by_value_template_action(bg, delta_rotation_angle, 0, 360)` (L816): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `delta_rotation_angle` How many degrees to add to the rotation angle of the given dp_direct_bitmap_bg_ptr when the action is updated.
  This rotation angle must be in the range [0..360].
- **dp_direct_bitmap_bg_rotate_by_action** `dp_direct_bitmap_bg_rotate_by_action(dp_direct_bitmap_bg_ptr&& bg, fixed delta_rotation_angle) : cyclic_by_value_template_action(move(bg), delta_rotation_angle, 0, 360)` (L831): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `delta_rotation_angle` How many degrees to add to the rotation angle of the given dp_direct_bitmap_bg_ptr when the action is updated.
  This rotation angle must be in the range [0..360].
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L841): Returns the dp_direct_bitmap_bg_ptr to modify.
- **delta_rotation_angle** `fixed delta_rotation_angle() const` (L850): Returns how many degrees to add to the rotation angle of the given dp_direct_bitmap_bg_ptr when the action is updated.
### `bn::dp_direct_bitmap_bg_rotate_to_action` (class, bn_dp_direct_bitmap_bg_actions.h:863)

Modifies the rotation angle of a dp_direct_bitmap_bg_ptr until it has a given state.

`class dp_direct_bitmap_bg_rotate_to_action : public to_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_rotation_manager>`

- **dp_direct_bitmap_bg_rotate_to_action** `dp_direct_bitmap_bg_rotate_to_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_rotation_angle) : to_value_template_action(bg, duration_updates, final_rotation_angle)` (L877): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the rotation angle of the given dp_direct_bitmap_bg_ptr is equal to final_rotation_angle.; `final_rotation_angle` Rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **dp_direct_bitmap_bg_rotate_to_action** `dp_direct_bitmap_bg_rotate_to_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_rotation_angle) : to_value_template_action(move(bg), duration_updates, final_rotation_angle)` (L894): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the rotation angle of the given dp_direct_bitmap_bg_ptr is equal to final_rotation_angle.; `final_rotation_angle` Rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L905): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_rotation_angle** `fixed final_rotation_angle() const` (L914): Returns the rotation angle of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_rotate_loop_action` (class, bn_dp_direct_bitmap_bg_actions.h:928)

Modifies the rotation angle of a dp_direct_bitmap_bg_ptr from a minimum to a maximum. When the rotation angle is equal to the given final state, it goes back to its initial state and vice versa.

`class dp_direct_bitmap_bg_rotate_loop_action : public loop_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_rotation_manager>`

- **dp_direct_bitmap_bg_rotate_loop_action** `dp_direct_bitmap_bg_rotate_loop_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_rotation_angle) : loop_value_template_action(bg, duration_updates, final_rotation_angle)` (L943): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the rotation angle delta.; `final_rotation_angle` When the rotation angle of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
  This rotation angle must be in the range [0..360].
- **dp_direct_bitmap_bg_rotate_loop_action** `dp_direct_bitmap_bg_rotate_loop_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_rotation_angle) : loop_value_template_action(move(bg), duration_updates, final_rotation_angle)` (L961): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the rotation angle delta.; `final_rotation_angle` When the rotation angle of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
  This rotation angle must be in the range [0..360].
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L972): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_rotation_angle** `fixed final_rotation_angle() const` (L981): When the rotation angle of the given dp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::dp_direct_bitmap_bg_rotate_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:994)

Changes the rotation angle of a dp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class dp_direct_bitmap_bg_rotate_toggle_action : public toggle_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_rotation_manager>`

- **dp_direct_bitmap_bg_rotate_toggle_action** `dp_direct_bitmap_bg_rotate_toggle_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_rotation_angle) : toggle_value_template_action(bg, duration_updates, new_rotation_angle)` (L1008): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the rotation angle of the given dp_direct_bitmap_bg_ptr.; `new_rotation_angle` New rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **dp_direct_bitmap_bg_rotate_toggle_action** `dp_direct_bitmap_bg_rotate_toggle_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_rotation_angle) : toggle_value_template_action(move(bg), duration_updates, new_rotation_angle)` (L1025): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the rotation angle of the given dp_direct_bitmap_bg_ptr.; `new_rotation_angle` New rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L1036): Returns the dp_direct_bitmap_bg_ptr to modify.
- **new_rotation_angle** `fixed new_rotation_angle() const` (L1045): Returns the rotation angle of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_horizontal_scale_manager` (class, bn_dp_direct_bitmap_bg_actions.h:1060)

Manages the horizontal scale of a dp_direct_bitmap_bg_ptr.

`class dp_direct_bitmap_bg_horizontal_scale_manager`

- **get** `static fixed get(const dp_direct_bitmap_bg_ptr& bg)` (L1067): Returns the horizontal scale of the given dp_direct_bitmap_bg_ptr.
- **set** `static void set(fixed horizontal_scale, dp_direct_bitmap_bg_ptr& bg)` (L1075): Sets the horizontal scale of the given dp_direct_bitmap_bg_ptr.
### `bn::dp_direct_bitmap_bg_horizontal_scale_to_action` (class, bn_dp_direct_bitmap_bg_actions.h:1088)

Modifies the horizontal scale of a dp_direct_bitmap_bg_ptr until it has a given state.

`class dp_direct_bitmap_bg_horizontal_scale_to_action : public to_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_horizontal_scale_manager>`

- **dp_direct_bitmap_bg_horizontal_scale_to_action** `dp_direct_bitmap_bg_horizontal_scale_to_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_horizontal_scale) : to_value_template_action(bg, duration_updates, final_horizontal_scale)` (L1100): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the horizontal scale of the given dp_direct_bitmap_bg_ptr is equal to final_horizontal_scale.; `final_horizontal_scale` Horizontal scale when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_horizontal_scale_to_action** `dp_direct_bitmap_bg_horizontal_scale_to_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_horizontal_scale) : to_value_template_action(move(bg), duration_updates, final_horizontal_scale)` (L1114): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the horizontal scale of the given dp_direct_bitmap_bg_ptr is equal to final_horizontal_scale.; `final_horizontal_scale` Horizontal scale when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L1124): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_horizontal_scale** `fixed final_horizontal_scale() const` (L1133): Returns the horizontal scale of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_horizontal_scale_loop_action` (class, bn_dp_direct_bitmap_bg_actions.h:1147)

Modifies the horizontal scale of a dp_direct_bitmap_bg_ptr from a minimum to a maximum. When the horizontal scale is equal to the given final state, it goes back to its initial state and vice versa.

`class dp_direct_bitmap_bg_horizontal_scale_loop_action : public loop_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_horizontal_scale_manager>`

- **dp_direct_bitmap_bg_horizontal_scale_loop_action** `dp_direct_bitmap_bg_horizontal_scale_loop_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_horizontal_scale) : loop_value_template_action(bg, duration_updates, final_horizontal_scale)` (L1160): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal scale delta.; `final_horizontal_scale` When the horizontal scale of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **dp_direct_bitmap_bg_horizontal_scale_loop_action** `dp_direct_bitmap_bg_horizontal_scale_loop_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_horizontal_scale) : loop_value_template_action(move(bg), duration_updates, final_horizontal_scale)` (L1175): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal scale delta.; `final_horizontal_scale` When the horizontal scale of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L1185): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_horizontal_scale** `fixed final_horizontal_scale() const` (L1194): When the horizontal scale of the given dp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::dp_direct_bitmap_bg_horizontal_scale_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:1208)

Changes the horizontal scale of a dp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class dp_direct_bitmap_bg_horizontal_scale_toggle_action : public toggle_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_horizontal_scale_manager>`

- **dp_direct_bitmap_bg_horizontal_scale_toggle_action** `dp_direct_bitmap_bg_horizontal_scale_toggle_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_horizontal_scale) : toggle_value_template_action(bg, duration_updates, new_horizontal_scale)` (L1220): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the horizontal scale of the given dp_direct_bitmap_bg_ptr.; `new_horizontal_scale` New horizontal scale when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_horizontal_scale_toggle_action** `dp_direct_bitmap_bg_horizontal_scale_toggle_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_horizontal_scale) : toggle_value_template_action(move(bg), duration_updates, new_horizontal_scale)` (L1234): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the horizontal scale of the given dp_direct_bitmap_bg_ptr.; `new_horizontal_scale` New horizontal scale when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L1244): Returns the dp_direct_bitmap_bg_ptr to modify.
- **new_horizontal_scale** `fixed new_horizontal_scale() const` (L1253): Returns the horizontal scale of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_vertical_scale_manager` (class, bn_dp_direct_bitmap_bg_actions.h:1268)

Manages the vertical scale of a dp_direct_bitmap_bg_ptr.

`class dp_direct_bitmap_bg_vertical_scale_manager`

- **get** `static fixed get(const dp_direct_bitmap_bg_ptr& bg)` (L1275): Returns the vertical scale of the given dp_direct_bitmap_bg_ptr.
- **set** `static void set(fixed vertical_scale, dp_direct_bitmap_bg_ptr& bg)` (L1283): Sets the vertical scale of the given dp_direct_bitmap_bg_ptr.
### `bn::dp_direct_bitmap_bg_vertical_scale_to_action` (class, bn_dp_direct_bitmap_bg_actions.h:1296)

Modifies the vertical scale of a dp_direct_bitmap_bg_ptr until it has a given state.

`class dp_direct_bitmap_bg_vertical_scale_to_action : public to_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_vertical_scale_manager>`

- **dp_direct_bitmap_bg_vertical_scale_to_action** `dp_direct_bitmap_bg_vertical_scale_to_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_vertical_scale) : to_value_template_action(bg, duration_updates, final_vertical_scale)` (L1308): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the vertical scale of the given dp_direct_bitmap_bg_ptr is equal to final_vertical_scale.; `final_vertical_scale` Vertical scale when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_vertical_scale_to_action** `dp_direct_bitmap_bg_vertical_scale_to_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_vertical_scale) : to_value_template_action(move(bg), duration_updates, final_vertical_scale)` (L1322): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the vertical scale of the given dp_direct_bitmap_bg_ptr is equal to final_vertical_scale.; `final_vertical_scale` Vertical scale when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L1332): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_vertical_scale** `fixed final_vertical_scale() const` (L1341): Returns the vertical scale of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_vertical_scale_loop_action` (class, bn_dp_direct_bitmap_bg_actions.h:1355)

Modifies the vertical scale of a dp_direct_bitmap_bg_ptr from a minimum to a maximum. When the vertical scale is equal to the given final state, it goes back to its initial state and vice versa.

`class dp_direct_bitmap_bg_vertical_scale_loop_action : public loop_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_vertical_scale_manager>`

- **dp_direct_bitmap_bg_vertical_scale_loop_action** `dp_direct_bitmap_bg_vertical_scale_loop_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_vertical_scale) : loop_value_template_action(bg, duration_updates, final_vertical_scale)` (L1368): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical scale delta.; `final_vertical_scale` When the vertical scale of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **dp_direct_bitmap_bg_vertical_scale_loop_action** `dp_direct_bitmap_bg_vertical_scale_loop_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_vertical_scale) : loop_value_template_action(move(bg), duration_updates, final_vertical_scale)` (L1383): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical scale delta.; `final_vertical_scale` When the vertical scale of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L1393): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_vertical_scale** `fixed final_vertical_scale() const` (L1402): When the vertical scale of the given dp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::dp_direct_bitmap_bg_vertical_scale_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:1416)

Changes the vertical scale of a dp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class dp_direct_bitmap_bg_vertical_scale_toggle_action : public toggle_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_vertical_scale_manager>`

- **dp_direct_bitmap_bg_vertical_scale_toggle_action** `dp_direct_bitmap_bg_vertical_scale_toggle_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_vertical_scale) : toggle_value_template_action(bg, duration_updates, new_vertical_scale)` (L1428): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the vertical scale of the given dp_direct_bitmap_bg_ptr.; `new_vertical_scale` New vertical scale when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_vertical_scale_toggle_action** `dp_direct_bitmap_bg_vertical_scale_toggle_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_vertical_scale) : toggle_value_template_action(move(bg), duration_updates, new_vertical_scale)` (L1442): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the vertical scale of the given dp_direct_bitmap_bg_ptr.; `new_vertical_scale` New vertical scale when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L1452): Returns the dp_direct_bitmap_bg_ptr to modify.
- **new_vertical_scale** `fixed new_vertical_scale() const` (L1461): Returns the vertical scale of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_scale_manager` (class, bn_dp_direct_bitmap_bg_actions.h:1476)

Manages the scale of a dp_direct_bitmap_bg_ptr.

`class dp_direct_bitmap_bg_scale_manager`

- **get** `static fixed get(const dp_direct_bitmap_bg_ptr& bg)` (L1483): Returns the horizontal scale of the given dp_direct_bitmap_bg_ptr.
- **set** `static void set(fixed scale, dp_direct_bitmap_bg_ptr& bg)` (L1491): Sets the scale of the given dp_direct_bitmap_bg_ptr.
### `bn::dp_direct_bitmap_bg_scale_to_action` (class, bn_dp_direct_bitmap_bg_actions.h:1504)

Modifies the scale of a dp_direct_bitmap_bg_ptr until it has a given state.

`class dp_direct_bitmap_bg_scale_to_action : public to_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_scale_manager>`

- **dp_direct_bitmap_bg_scale_to_action** `dp_direct_bitmap_bg_scale_to_action(const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_scale) : to_value_template_action(bg, duration_updates, final_scale)` (L1516): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the scale of the given dp_direct_bitmap_bg_ptr is equal to final_scale.; `final_scale` scale when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_scale_to_action** `dp_direct_bitmap_bg_scale_to_action(dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_scale) : to_value_template_action(move(bg), duration_updates, final_scale)` (L1529): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the scale of the given dp_direct_bitmap_bg_ptr is equal to final_scale.; `final_scale` Scale when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L1538): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_scale** `fixed final_scale() const` (L1547): Returns the scale of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_scale_loop_action` (class, bn_dp_direct_bitmap_bg_actions.h:1561)

Modifies the scale of a dp_direct_bitmap_bg_ptr from a minimum to a maximum. When the scale is equal to the given final state, it goes back to its initial state and vice versa.

`class dp_direct_bitmap_bg_scale_loop_action : public loop_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_scale_manager>`

- **dp_direct_bitmap_bg_scale_loop_action** `dp_direct_bitmap_bg_scale_loop_action(const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_scale) : loop_value_template_action(bg, duration_updates, final_scale)` (L1574): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the scale delta.; `final_scale` When the scale of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **dp_direct_bitmap_bg_scale_loop_action** `dp_direct_bitmap_bg_scale_loop_action(dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_scale) : loop_value_template_action(move(bg), duration_updates, final_scale)` (L1588): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the scale delta.; `final_scale` When the scale of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L1597): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_scale** `fixed final_scale() const` (L1606): When the scale of the given dp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::dp_direct_bitmap_bg_scale_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:1620)

Changes the scale of a dp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class dp_direct_bitmap_bg_scale_toggle_action : public toggle_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_scale_manager>`

- **dp_direct_bitmap_bg_scale_toggle_action** `dp_direct_bitmap_bg_scale_toggle_action(const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_scale) : toggle_value_template_action(bg, duration_updates, new_scale)` (L1632): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the scale of the given dp_direct_bitmap_bg_ptr.; `new_scale` New scale when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_scale_toggle_action** `dp_direct_bitmap_bg_scale_toggle_action(dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_scale) : toggle_value_template_action(move(bg), duration_updates, new_scale)` (L1645): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the scale of the given dp_direct_bitmap_bg_ptr.; `new_scale` New scale when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L1654): Returns the dp_direct_bitmap_bg_ptr to modify.
- **new_scale** `fixed new_scale() const` (L1663): Returns the scale of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_horizontal_shear_manager` (class, bn_dp_direct_bitmap_bg_actions.h:1678)

Manages the horizontal shear of a dp_direct_bitmap_bg_ptr.

`class dp_direct_bitmap_bg_horizontal_shear_manager`

- **get** `static fixed get(const dp_direct_bitmap_bg_ptr& bg)` (L1685): Returns the horizontal shear of the given dp_direct_bitmap_bg_ptr.
- **set** `static void set(fixed horizontal_shear, dp_direct_bitmap_bg_ptr& bg)` (L1693): Sets the horizontal shear of the given dp_direct_bitmap_bg_ptr.
### `bn::dp_direct_bitmap_bg_horizontal_shear_to_action` (class, bn_dp_direct_bitmap_bg_actions.h:1706)

Modifies the horizontal shear of a dp_direct_bitmap_bg_ptr until it has a given state.

`class dp_direct_bitmap_bg_horizontal_shear_to_action : public to_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_horizontal_shear_manager>`

- **dp_direct_bitmap_bg_horizontal_shear_to_action** `dp_direct_bitmap_bg_horizontal_shear_to_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_horizontal_shear) : to_value_template_action(bg, duration_updates, final_horizontal_shear)` (L1718): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the horizontal shear of the given dp_direct_bitmap_bg_ptr is equal to final_horizontal_shear.; `final_horizontal_shear` Horizontal shear when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_horizontal_shear_to_action** `dp_direct_bitmap_bg_horizontal_shear_to_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_horizontal_shear) : to_value_template_action(move(bg), duration_updates, final_horizontal_shear)` (L1731): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the horizontal shear of the given dp_direct_bitmap_bg_ptr is equal to final_horizontal_shear.; `final_horizontal_shear` Horizontal shear when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L1740): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_horizontal_shear** `fixed final_horizontal_shear() const` (L1749): Returns the horizontal shear of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_horizontal_shear_loop_action` (class, bn_dp_direct_bitmap_bg_actions.h:1763)

Modifies the horizontal shear of a dp_direct_bitmap_bg_ptr from a minimum to a maximum. When the horizontal shear is equal to the given final state, it goes back to its initial state and vice versa.

`class dp_direct_bitmap_bg_horizontal_shear_loop_action : public loop_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_horizontal_shear_manager>`

- **dp_direct_bitmap_bg_horizontal_shear_loop_action** `dp_direct_bitmap_bg_horizontal_shear_loop_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_horizontal_shear) : loop_value_template_action(bg, duration_updates, final_horizontal_shear)` (L1776): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal shear delta.; `final_horizontal_shear` When the horizontal shear of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **dp_direct_bitmap_bg_horizontal_shear_loop_action** `dp_direct_bitmap_bg_horizontal_shear_loop_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_horizontal_shear) : loop_value_template_action(move(bg), duration_updates, final_horizontal_shear)` (L1790): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal shear delta.; `final_horizontal_shear` When the horizontal shear of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L1799): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_horizontal_shear** `fixed final_horizontal_shear() const` (L1808): When the horizontal shear of the given dp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::dp_direct_bitmap_bg_horizontal_shear_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:1822)

Changes the horizontal shear of a dp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class dp_direct_bitmap_bg_horizontal_shear_toggle_action : public toggle_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_horizontal_shear_manager>`

- **dp_direct_bitmap_bg_horizontal_shear_toggle_action** `dp_direct_bitmap_bg_horizontal_shear_toggle_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_horizontal_shear) : toggle_value_template_action(bg, duration_updates, new_horizontal_shear)` (L1834): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the horizontal shear of the given dp_direct_bitmap_bg_ptr.; `new_horizontal_shear` New horizontal shear when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_horizontal_shear_toggle_action** `dp_direct_bitmap_bg_horizontal_shear_toggle_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_horizontal_shear) : toggle_value_template_action(move(bg), duration_updates, new_horizontal_shear)` (L1847): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the horizontal shear of the given dp_direct_bitmap_bg_ptr.; `new_horizontal_shear` New horizontal shear when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L1856): Returns the dp_direct_bitmap_bg_ptr to modify.
- **new_horizontal_shear** `fixed new_horizontal_shear() const` (L1865): Returns the horizontal shear of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_vertical_shear_manager` (class, bn_dp_direct_bitmap_bg_actions.h:1880)

Manages the vertical shear of a dp_direct_bitmap_bg_ptr.

`class dp_direct_bitmap_bg_vertical_shear_manager`

- **get** `static fixed get(const dp_direct_bitmap_bg_ptr& bg)` (L1887): Returns the vertical shear of the given dp_direct_bitmap_bg_ptr.
- **set** `static void set(fixed vertical_shear, dp_direct_bitmap_bg_ptr& bg)` (L1895): Sets the vertical shear of the given dp_direct_bitmap_bg_ptr.
### `bn::dp_direct_bitmap_bg_vertical_shear_to_action` (class, bn_dp_direct_bitmap_bg_actions.h:1908)

Modifies the vertical shear of a dp_direct_bitmap_bg_ptr until it has a given state.

`class dp_direct_bitmap_bg_vertical_shear_to_action : public to_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_vertical_shear_manager>`

- **dp_direct_bitmap_bg_vertical_shear_to_action** `dp_direct_bitmap_bg_vertical_shear_to_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_vertical_shear) : to_value_template_action(bg, duration_updates, final_vertical_shear)` (L1920): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the vertical shear of the given dp_direct_bitmap_bg_ptr is equal to final_vertical_shear.; `final_vertical_shear` Vertical shear when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_vertical_shear_to_action** `dp_direct_bitmap_bg_vertical_shear_to_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_vertical_shear) : to_value_template_action(move(bg), duration_updates, final_vertical_shear)` (L1933): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the vertical shear of the given dp_direct_bitmap_bg_ptr is equal to final_vertical_shear.; `final_vertical_shear` Vertical shear when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L1942): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_vertical_shear** `fixed final_vertical_shear() const` (L1951): Returns the vertical shear of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_vertical_shear_loop_action` (class, bn_dp_direct_bitmap_bg_actions.h:1965)

Modifies the vertical shear of a dp_direct_bitmap_bg_ptr from a minimum to a maximum. When the vertical shear is equal to the given final state, it goes back to its initial state and vice versa.

`class dp_direct_bitmap_bg_vertical_shear_loop_action : public loop_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_vertical_shear_manager>`

- **dp_direct_bitmap_bg_vertical_shear_loop_action** `dp_direct_bitmap_bg_vertical_shear_loop_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_vertical_shear) : loop_value_template_action(bg, duration_updates, final_vertical_shear)` (L1978): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical shear delta.; `final_vertical_shear` When the vertical shear of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **dp_direct_bitmap_bg_vertical_shear_loop_action** `dp_direct_bitmap_bg_vertical_shear_loop_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_vertical_shear) : loop_value_template_action(move(bg), duration_updates, final_vertical_shear)` (L1992): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical shear delta.; `final_vertical_shear` When the vertical shear of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L2001): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_vertical_shear** `fixed final_vertical_shear() const` (L2010): When the vertical shear of the given dp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::dp_direct_bitmap_bg_vertical_shear_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:2024)

Changes the vertical shear of a dp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class dp_direct_bitmap_bg_vertical_shear_toggle_action : public toggle_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_vertical_shear_manager>`

- **dp_direct_bitmap_bg_vertical_shear_toggle_action** `dp_direct_bitmap_bg_vertical_shear_toggle_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_vertical_shear) : toggle_value_template_action(bg, duration_updates, new_vertical_shear)` (L2036): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the vertical shear of the given dp_direct_bitmap_bg_ptr.; `new_vertical_shear` New vertical shear when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_vertical_shear_toggle_action** `dp_direct_bitmap_bg_vertical_shear_toggle_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_vertical_shear) : toggle_value_template_action(move(bg), duration_updates, new_vertical_shear)` (L2049): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the vertical shear of the given dp_direct_bitmap_bg_ptr.; `new_vertical_shear` New vertical shear when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L2058): Returns the dp_direct_bitmap_bg_ptr to modify.
- **new_vertical_shear** `fixed new_vertical_shear() const` (L2067): Returns the vertical shear of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_shear_manager` (class, bn_dp_direct_bitmap_bg_actions.h:2082)

Manages the shear of a dp_direct_bitmap_bg_ptr.

`class dp_direct_bitmap_bg_shear_manager`

- **get** `static fixed get(const dp_direct_bitmap_bg_ptr& bg)` (L2089): Returns the horizontal shear of the given dp_direct_bitmap_bg_ptr.
- **set** `static void set(fixed shear, dp_direct_bitmap_bg_ptr& bg)` (L2097): Sets the shear of the given dp_direct_bitmap_bg_ptr.
### `bn::dp_direct_bitmap_bg_shear_to_action` (class, bn_dp_direct_bitmap_bg_actions.h:2110)

Modifies the shear of a dp_direct_bitmap_bg_ptr until it has a given state.

`class dp_direct_bitmap_bg_shear_to_action : public to_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_shear_manager>`

- **dp_direct_bitmap_bg_shear_to_action** `dp_direct_bitmap_bg_shear_to_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_shear) : to_value_template_action(bg, duration_updates, final_shear)` (L2122): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the shear of the given dp_direct_bitmap_bg_ptr is equal to final_shear.; `final_shear` Shear when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_shear_to_action** `dp_direct_bitmap_bg_shear_to_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_shear) : to_value_template_action(move(bg), duration_updates, final_shear)` (L2135): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the shear of the given dp_direct_bitmap_bg_ptr is equal to final_shear.; `final_shear` Shear when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L2144): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_shear** `fixed final_shear() const` (L2153): Returns the shear of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_shear_loop_action` (class, bn_dp_direct_bitmap_bg_actions.h:2167)

Modifies the shear of a dp_direct_bitmap_bg_ptr from a minimum to a maximum. When the shear is equal to the given final state, it goes back to its initial state and vice versa.

`class dp_direct_bitmap_bg_shear_loop_action : public loop_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_shear_manager>`

- **dp_direct_bitmap_bg_shear_loop_action** `dp_direct_bitmap_bg_shear_loop_action(const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_shear) : loop_value_template_action(bg, duration_updates, final_shear)` (L2180): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the shear delta.; `final_shear` When the shear of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **dp_direct_bitmap_bg_shear_loop_action** `dp_direct_bitmap_bg_shear_loop_action(dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_shear) : loop_value_template_action(move(bg), duration_updates, final_shear)` (L2193): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the shear delta.; `final_shear` When the shear of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L2201): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_shear** `fixed final_shear() const` (L2210): When the shear of the given dp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::dp_direct_bitmap_bg_shear_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:2223)

Changes the shear of a dp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class dp_direct_bitmap_bg_shear_toggle_action : public toggle_value_template_action<dp_direct_bitmap_bg_ptr, fixed, dp_direct_bitmap_bg_shear_manager>`

- **dp_direct_bitmap_bg_shear_toggle_action** `dp_direct_bitmap_bg_shear_toggle_action(const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_shear) : toggle_value_template_action(bg, duration_updates, new_shear)` (L2235): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the shear of the given dp_direct_bitmap_bg_ptr.; `new_shear` New shear when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_shear_toggle_action** `dp_direct_bitmap_bg_shear_toggle_action(dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_shear) : toggle_value_template_action(move(bg), duration_updates, new_shear)` (L2247): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the shear of the given dp_direct_bitmap_bg_ptr.; `new_shear` New shear when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L2255): Returns the dp_direct_bitmap_bg_ptr to modify.
- **new_shear** `fixed new_shear() const` (L2264): Returns the shear of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_horizontal_flip_manager` (class, bn_dp_direct_bitmap_bg_actions.h:2279)

Manages if a dp_direct_bitmap_bg_ptr is flipped in its horizontal axis or not.

`class dp_direct_bitmap_bg_horizontal_flip_manager`

- **get** `static bool get(const dp_direct_bitmap_bg_ptr& bg)` (L2286): Indicates if the given dp_direct_bitmap_bg_ptr is flipped in the horizontal axis or not.
- **set** `static void set(bool horizontal_flip, dp_direct_bitmap_bg_ptr& bg)` (L2294): Sets if the given dp_direct_bitmap_bg_ptr must be flipped in the horizontal axis or not.
### `bn::dp_direct_bitmap_bg_horizontal_flip_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:2308)

Toggles if a dp_direct_bitmap_bg_ptr must be flipped in the horizontal axis or not when the action is updated a given number of times.

`class dp_direct_bitmap_bg_horizontal_flip_toggle_action : public bool_toggle_value_template_action<dp_direct_bitmap_bg_ptr, dp_direct_bitmap_bg_horizontal_flip_manager>`

- **dp_direct_bitmap_bg_horizontal_flip_toggle_action** `dp_direct_bitmap_bg_horizontal_flip_toggle_action(const dp_direct_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2319): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given dp_direct_bitmap_bg_ptr must be flipped in the horizontal axis or not.
- **dp_direct_bitmap_bg_horizontal_flip_toggle_action** `dp_direct_bitmap_bg_horizontal_flip_toggle_action(dp_direct_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2330): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given dp_direct_bitmap_bg_ptr must be flipped in the horizontal axis or not.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L2338): Returns the dp_direct_bitmap_bg_ptr to modify.
### `bn::dp_direct_bitmap_bg_vertical_flip_manager` (class, bn_dp_direct_bitmap_bg_actions.h:2353)

Manages if a dp_direct_bitmap_bg_ptr is flipped in its vertical axis or not.

`class dp_direct_bitmap_bg_vertical_flip_manager`

- **get** `static bool get(const dp_direct_bitmap_bg_ptr& bg)` (L2360): Indicates if the given dp_direct_bitmap_bg_ptr is flipped in the vertical axis or not.
- **set** `static void set(bool vertical_flip, dp_direct_bitmap_bg_ptr& bg)` (L2368): Sets if the given dp_direct_bitmap_bg_ptr must be flipped in the vertical axis or not.
### `bn::dp_direct_bitmap_bg_vertical_flip_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:2382)

Toggles if a dp_direct_bitmap_bg_ptr must be flipped in the vertical axis or not when the action is updated a given number of times.

`class dp_direct_bitmap_bg_vertical_flip_toggle_action : public bool_toggle_value_template_action<dp_direct_bitmap_bg_ptr, dp_direct_bitmap_bg_vertical_flip_manager>`

- **dp_direct_bitmap_bg_vertical_flip_toggle_action** `dp_direct_bitmap_bg_vertical_flip_toggle_action(const dp_direct_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2393): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given dp_direct_bitmap_bg_ptr must be flipped in the vertical axis or not.
- **dp_direct_bitmap_bg_vertical_flip_toggle_action** `dp_direct_bitmap_bg_vertical_flip_toggle_action(dp_direct_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2404): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given dp_direct_bitmap_bg_ptr must be flipped in the vertical axis or not.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L2412): Returns the dp_direct_bitmap_bg_ptr to modify.
### `bn::dp_direct_bitmap_bg_pivot_position_manager` (class, bn_dp_direct_bitmap_bg_actions.h:2427)

Manages the position of the pivot of a dp_direct_bitmap_bg_ptr.

`class dp_direct_bitmap_bg_pivot_position_manager`

- **get** `static const fixed_point& get(const dp_direct_bitmap_bg_ptr& bg)` (L2434): Returns the position of the pivot of the given dp_direct_bitmap_bg_ptr.
- **set** `static void set(const fixed_point& position, dp_direct_bitmap_bg_ptr& bg)` (L2442): Sets the position of the pivot of the given dp_direct_bitmap_bg_ptr.
### `bn::dp_direct_bitmap_bg_move_pivot_by_action` (class, bn_dp_direct_bitmap_bg_actions.h:2455)

Modifies the position of the pivot of a dp_direct_bitmap_bg_ptr by a given delta.

`class dp_direct_bitmap_bg_move_pivot_by_action : public by_value_template_action<dp_direct_bitmap_bg_ptr, fixed_point, dp_direct_bitmap_bg_pivot_position_manager>`

- **dp_direct_bitmap_bg_move_pivot_by_action** `dp_direct_bitmap_bg_move_pivot_by_action(const dp_direct_bitmap_bg_ptr& bg, fixed delta_x, fixed delta_y) : by_value_template_action(bg, fixed_point(delta_x, delta_y))` (L2468): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `delta_x` How much to add to the horizontal position of the pivot of the given dp_direct_bitmap_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the pivot of the given dp_direct_bitmap_bg_ptr when the action is updated.
- **dp_direct_bitmap_bg_move_pivot_by_action** `dp_direct_bitmap_bg_move_pivot_by_action(dp_direct_bitmap_bg_ptr&& bg, fixed delta_x, fixed delta_y) : by_value_template_action(move(bg), fixed_point(delta_x, delta_y))` (L2481): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `delta_x` How much to add to the horizontal position of the pivot of the given dp_direct_bitmap_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the pivot of the given dp_direct_bitmap_bg_ptr when the action is updated.
- **dp_direct_bitmap_bg_move_pivot_by_action** `dp_direct_bitmap_bg_move_pivot_by_action(const dp_direct_bitmap_bg_ptr& bg, const fixed_point& delta_position) : by_value_template_action(bg, delta_position)` (L2492): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `delta_position` How much to add to the position of the pivot of the given dp_direct_bitmap_bg_ptr when the action is updated.
- **dp_direct_bitmap_bg_move_pivot_by_action** `dp_direct_bitmap_bg_move_pivot_by_action(dp_direct_bitmap_bg_ptr&& bg, const fixed_point& delta_position) : by_value_template_action(move(bg), delta_position)` (L2503): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `delta_position` How much to add to the position of the pivot of the given dp_direct_bitmap_bg_ptr when the action is updated.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L2511): Returns the dp_direct_bitmap_bg_ptr to modify.
- **delta_position** `const fixed_point& delta_position() const` (L2520): Returns how much to add to the position of the pivot of the given dp_direct_bitmap_bg_ptr when the action is updated.
### `bn::dp_direct_bitmap_bg_move_pivot_to_action` (class, bn_dp_direct_bitmap_bg_actions.h:2533)

Modifies the position of the pivot of a dp_direct_bitmap_bg_ptr until it has a given state.

`class dp_direct_bitmap_bg_move_pivot_to_action : public to_value_template_action<dp_direct_bitmap_bg_ptr, fixed_point, dp_direct_bitmap_bg_pivot_position_manager>`

- **dp_direct_bitmap_bg_move_pivot_to_action** `dp_direct_bitmap_bg_move_pivot_to_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L2548): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given dp_direct_bitmap_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the pivot of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the pivot of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_move_pivot_to_action** `dp_direct_bitmap_bg_move_pivot_to_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L2564): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given dp_direct_bitmap_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the pivot of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the pivot of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_move_pivot_to_action** `dp_direct_bitmap_bg_move_pivot_to_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(bg, duration_updates, final_position)` (L2578): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given dp_direct_bitmap_bg_ptr is equal to final_position.; `final_position` Position of the pivot of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_move_pivot_to_action** `dp_direct_bitmap_bg_move_pivot_to_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(move(bg), duration_updates, final_position)` (L2592): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given dp_direct_bitmap_bg_ptr is equal to final_position.; `final_position` Position of the pivot of the given dp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L2601): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L2610): Returns the position of the pivot of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_move_pivot_loop_action` (class, bn_dp_direct_bitmap_bg_actions.h:2624)

Modifies the position of the pivot of a dp_direct_bitmap_bg_ptr from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class dp_direct_bitmap_bg_move_pivot_loop_action : public loop_value_template_action<dp_direct_bitmap_bg_ptr, fixed_point, dp_direct_bitmap_bg_pivot_position_manager>`

- **dp_direct_bitmap_bg_move_pivot_loop_action** `dp_direct_bitmap_bg_move_pivot_loop_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L2639): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the pivot of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the pivot of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **dp_direct_bitmap_bg_move_pivot_loop_action** `dp_direct_bitmap_bg_move_pivot_loop_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L2655): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the pivot of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the pivot of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **dp_direct_bitmap_bg_move_pivot_loop_action** `dp_direct_bitmap_bg_move_pivot_loop_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(bg, duration_updates, final_position)` (L2669): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the pivot of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **dp_direct_bitmap_bg_move_pivot_loop_action** `dp_direct_bitmap_bg_move_pivot_loop_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(move(bg), duration_updates, final_position)` (L2683): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the pivot of the given dp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L2692): Returns the dp_direct_bitmap_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L2701): When the position of the pivot of the given dp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::dp_direct_bitmap_bg_move_pivot_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:2715)

Changes the position of the pivot of a dp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class dp_direct_bitmap_bg_move_pivot_toggle_action : public toggle_value_template_action<dp_direct_bitmap_bg_ptr, fixed_point, dp_direct_bitmap_bg_pivot_position_manager>`

- **dp_direct_bitmap_bg_move_pivot_toggle_action** `dp_direct_bitmap_bg_move_pivot_toggle_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(bg, duration_updates, fixed_point(new_x, new_y))` (L2728): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given dp_direct_bitmap_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_move_pivot_toggle_action** `dp_direct_bitmap_bg_move_pivot_toggle_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(move(bg), duration_updates, fixed_point(new_x, new_y))` (L2742): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given dp_direct_bitmap_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_move_pivot_toggle_action** `dp_direct_bitmap_bg_move_pivot_toggle_action( const dp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(bg, duration_updates, new_position)` (L2755): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given dp_direct_bitmap_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **dp_direct_bitmap_bg_move_pivot_toggle_action** `dp_direct_bitmap_bg_move_pivot_toggle_action( dp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(move(bg), duration_updates, new_position)` (L2768): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given dp_direct_bitmap_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L2777): Returns the dp_direct_bitmap_bg_ptr to modify.
- **new_position** `const fixed_point& new_position() const` (L2786): Returns the position of the pivot of the given dp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::dp_direct_bitmap_bg_mosaic_manager` (class, bn_dp_direct_bitmap_bg_actions.h:2801)

Manages if the mosaic effect must be applied to a dp_direct_bitmap_bg_ptr or not.

`class dp_direct_bitmap_bg_mosaic_manager`

- **get** `static bool get(const dp_direct_bitmap_bg_ptr& bg)` (L2808): Indicates if the mosaic effect is applied to the given dp_direct_bitmap_bg_ptr or not.
- **set** `static void set(bool mosaic_enabled, dp_direct_bitmap_bg_ptr& bg)` (L2816): Sets if the mosaic effect must be applied to the given dp_direct_bitmap_bg_ptr or not.
### `bn::dp_direct_bitmap_bg_mosaic_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:2830)

Toggles if the mosaic effect must be applied to a dp_direct_bitmap_bg_ptr or not when the action is updated a given number of times.

`class dp_direct_bitmap_bg_mosaic_toggle_action : public bool_toggle_value_template_action<dp_direct_bitmap_bg_ptr, dp_direct_bitmap_bg_mosaic_manager>`

- **dp_direct_bitmap_bg_mosaic_toggle_action** `dp_direct_bitmap_bg_mosaic_toggle_action(const dp_direct_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2841): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the mosaic effect must be applied or not.
- **dp_direct_bitmap_bg_mosaic_toggle_action** `dp_direct_bitmap_bg_mosaic_toggle_action(dp_direct_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2852): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the mosaic effect must be applied or not.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L2860): Returns the dp_direct_bitmap_bg_ptr to modify.
### `bn::dp_direct_bitmap_bg_blending_manager` (class, bn_dp_direct_bitmap_bg_actions.h:2875)

Manages if blending must be applied to a dp_direct_bitmap_bg_ptr or not.

`class dp_direct_bitmap_bg_blending_manager`

- **get** `static bool get(const dp_direct_bitmap_bg_ptr& bg)` (L2882): Indicates if blending is applied to the given dp_direct_bitmap_bg_ptr or not.
- **set** `static void set(bool blending_enabled, dp_direct_bitmap_bg_ptr& bg)` (L2890): Sets if blending must be applied to the given dp_direct_bitmap_bg_ptr or not.
### `bn::dp_direct_bitmap_bg_blending_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:2904)

Toggles if blending must be applied to a dp_direct_bitmap_bg_ptr or not when the action is updated a given number of times.

`class dp_direct_bitmap_bg_blending_toggle_action : public bool_toggle_value_template_action<dp_direct_bitmap_bg_ptr, dp_direct_bitmap_bg_blending_manager>`

- **dp_direct_bitmap_bg_blending_toggle_action** `dp_direct_bitmap_bg_blending_toggle_action(const dp_direct_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2915): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if blending must be applied or not.
- **dp_direct_bitmap_bg_blending_toggle_action** `dp_direct_bitmap_bg_blending_toggle_action(dp_direct_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2926): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if blending must be applied or not.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L2934): Returns the dp_direct_bitmap_bg_ptr to modify.
### `bn::dp_direct_bitmap_bg_blending_top_manager` (class, bn_dp_direct_bitmap_bg_actions.h:2947)

Manages if a dp_direct_bitmap_bg_ptr must be part of the blending top layer or not.

`class dp_direct_bitmap_bg_blending_top_manager`

- **get** `static bool get(const dp_direct_bitmap_bg_ptr& bg)` (L2954): Indicates if the given dp_direct_bitmap_bg_ptr is part of the blending top layer or not.
- **set** `static void set(bool blending_top_enabled, dp_direct_bitmap_bg_ptr& bg)` (L2962): Sets if the given dp_direct_bitmap_bg_ptr must be part of the blending top layer or not.
### `bn::dp_direct_bitmap_bg_blending_top_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:2976)

Toggles if a dp_direct_bitmap_bg_ptr is part of the blending top layer or not when the action is updated a given number of times.

`class dp_direct_bitmap_bg_blending_top_toggle_action : public bool_toggle_value_template_action<dp_direct_bitmap_bg_ptr, dp_direct_bitmap_bg_blending_top_manager>`

- **dp_direct_bitmap_bg_blending_top_toggle_action** `dp_direct_bitmap_bg_blending_top_toggle_action(const dp_direct_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2987): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given dp_direct_bitmap_bg_ptr is part of the blending top layer or not.
- **dp_direct_bitmap_bg_blending_top_toggle_action** `dp_direct_bitmap_bg_blending_top_toggle_action(dp_direct_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2998): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given dp_direct_bitmap_bg_ptr is part of the blending top layer or not.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L3006): Returns the dp_direct_bitmap_bg_ptr to modify.
### `bn::dp_direct_bitmap_bg_blending_bottom_manager` (class, bn_dp_direct_bitmap_bg_actions.h:3019)

Manages if a dp_direct_bitmap_bg_ptr must be part of the blending bottom layer or not.

`class dp_direct_bitmap_bg_blending_bottom_manager`

- **get** `static bool get(const dp_direct_bitmap_bg_ptr& bg)` (L3026): Indicates if the given dp_direct_bitmap_bg_ptr is part of the blending bottom layer or not.
- **set** `static void set(bool blending_bottom_enabled, dp_direct_bitmap_bg_ptr& bg)` (L3034): Sets if the given dp_direct_bitmap_bg_ptr must be part of the blending bottom layer or not.
### `bn::dp_direct_bitmap_bg_blending_bottom_toggle_action` (class, bn_dp_direct_bitmap_bg_actions.h:3048)

Toggles if a dp_direct_bitmap_bg_ptr is part of the blending bottom layer or not when the action is updated a given number of times.

`class dp_direct_bitmap_bg_blending_bottom_toggle_action : public bool_toggle_value_template_action<dp_direct_bitmap_bg_ptr, dp_direct_bitmap_bg_blending_bottom_manager>`

- **dp_direct_bitmap_bg_blending_bottom_toggle_action** `dp_direct_bitmap_bg_blending_bottom_toggle_action(const dp_direct_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L3059): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given dp_direct_bitmap_bg_ptr is part of the blending bottom layer or not.
- **dp_direct_bitmap_bg_blending_bottom_toggle_action** `dp_direct_bitmap_bg_blending_bottom_toggle_action(dp_direct_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L3070): Constructor.
  `bg` dp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given dp_direct_bitmap_bg_ptr is part of the blending bottom layer or not.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L3078): Returns the dp_direct_bitmap_bg_ptr to modify.

## bn_green_swap_actions.h

### `bn::green_swap_manager` (class, bn_green_swap_actions.h:29)

Manages if green swap must be enabled or not.

`class green_swap_manager`

- **get** `static bool get()` (L36): Indicates if green swap is enabled or not.
- **set** `static void set(bool enabled)` (L44): Sets if green swap must be enabled or not.
### `bn::green_swap_toggle_action` (class, bn_green_swap_actions.h:57)

Toggles if green swap must be enabled or not when the action is updated a given number of times.

`class green_swap_toggle_action : public bool_toggle_template_action<green_swap_manager>`

- **green_swap_toggle_action** `explicit green_swap_toggle_action(int duration_updates) : bool_toggle_template_action(duration_updates)` (L66): Constructor.
  `duration_updates` How many times the action has to be updated to toggle if green swap must be inverted or not.

## bn_inside_window_actions.h

### `bn::inside_window_visible_manager` (class, bn_inside_window_actions.h:31)

Manages if a inside_window must be committed to the GBA or not.

`class inside_window_visible_manager`

- **get** `static bool get(inside_window window)` (L38): Indicates if the given inside_window is committed to the GBA or not.
- **set** `static void set(bool visible, inside_window window)` (L46): Sets if the given inside_window must be committed to the GBA or not.
### `bn::inside_window_visible_toggle_action` (class, bn_inside_window_actions.h:60)

Toggles if a inside_window must be committed to the GBA or not when the action is updated a given number of times.

`class inside_window_visible_toggle_action : public bool_toggle_value_template_action<inside_window, inside_window_visible_manager>`

- **inside_window_visible_toggle_action** `inside_window_visible_toggle_action(inside_window window, int duration_updates) : bool_toggle_value_template_action(window, duration_updates)` (L71): Constructor.
  `window` inside_window to modify.; `duration_updates` How many times the action has to be updated to toggle if the given inside_window must be committed to the GBA or not.
- **window** `inside_window window() const` (L79): Returns the inside_window to modify.

## bn_jingle_actions.h

### `bn::jingle_volume_manager` (class, bn_jingle_actions.h:31)

Manages the volume of the active Direct Sound jingle.

`class jingle_volume_manager`

- **get** `static fixed get()` (L38): Returns the volume of the active Direct Sound jingle.
- **set** `static void set(fixed volume)` (L47): Sets the volume of the active Direct Sound jingle.
  `volume` New volume in the range [0..1].
### `bn::jingle_volume_to_action` (class, bn_jingle_actions.h:60)

Modifies the volume of the active Direct Sound jingle until it has a given state.

`class jingle_volume_to_action : public to_template_action<fixed, jingle_volume_manager>`

- **jingle_volume_to_action** `jingle_volume_to_action(int duration_updates, fixed final_volume) : to_template_action(duration_updates, final_volume)` (L72): Constructor.
  `duration_updates` Number of times that the action must be updated until the volume is equal to final_volume.; `final_volume` Volume when the action is updated duration_updates times.
  This volume must be in the range [0..1].
- **final_volume** `fixed final_volume() const` (L81): Returns the volume when the action is updated the given number of times.
### `bn::jingle_volume_loop_action` (class, bn_jingle_actions.h:95)

Modifies the volume of the active Direct Sound jingle from a minimum to a maximum. When the volume is equal to the given final state, it goes back to its initial state and vice versa.

`class jingle_volume_loop_action : public loop_template_action<fixed, jingle_volume_manager>`

- **jingle_volume_loop_action** `jingle_volume_loop_action(int duration_updates, fixed final_volume) : loop_template_action(duration_updates, final_volume)` (L108): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the volume delta.; `final_volume` When the volume is equal to this parameter, it goes back to its initial state and vice versa.
  This volume must be in the range [0..1].
- **final_volume** `fixed final_volume() const` (L117): When the volume is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::jingle_volume_toggle_action` (class, bn_jingle_actions.h:130)

Changes the volume of the active Direct Sound jingle when the action is updated a given number of times.

`class jingle_volume_toggle_action : public toggle_template_action<fixed, jingle_volume_manager>`

- **jingle_volume_toggle_action** `jingle_volume_toggle_action(int duration_updates, fixed new_volume) : toggle_template_action(duration_updates, new_volume)` (L141): Constructor.
  `duration_updates` How many times the action has to be updated to change the volume.; `new_volume` New volume when the action is updated duration_updates times.
  This volume must be in the range [0..1].
- **new_volume** `fixed new_volume() const` (L150): Returns the volume when the action is updated the given number of times.

## bn_music_actions.h

### `bn::music_volume_manager` (class, bn_music_actions.h:31)

Manages the volume of the active Direct Sound music.

`class music_volume_manager`

- **get** `static fixed get()` (L38): Returns the volume of the active Direct Sound music.
- **set** `static void set(fixed volume)` (L47): Sets the volume of the active Direct Sound music.
  `volume` New volume in the range [0..1].
### `bn::music_volume_to_action` (class, bn_music_actions.h:60)

Modifies the volume of the active Direct Sound music until it has a given state.

`class music_volume_to_action : public to_template_action<fixed, music_volume_manager>`

- **music_volume_to_action** `music_volume_to_action(int duration_updates, fixed final_volume) : to_template_action(duration_updates, final_volume)` (L72): Constructor.
  `duration_updates` Number of times that the action must be updated until the volume is equal to final_volume.; `final_volume` Volume when the action is updated duration_updates times.
  This volume must be in the range [0..1].
- **final_volume** `fixed final_volume() const` (L81): Returns the volume when the action is updated the given number of times.
### `bn::music_volume_loop_action` (class, bn_music_actions.h:95)

Modifies the volume of the active Direct Sound music from a minimum to a maximum. When the volume is equal to the given final state, it goes back to its initial state and vice versa.

`class music_volume_loop_action : public loop_template_action<fixed, music_volume_manager>`

- **music_volume_loop_action** `music_volume_loop_action(int duration_updates, fixed final_volume) : loop_template_action(duration_updates, final_volume)` (L108): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the volume delta.; `final_volume` When the volume is equal to this parameter, it goes back to its initial state and vice versa.
  This volume must be in the range [0..1].
- **final_volume** `fixed final_volume() const` (L117): When the volume is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::music_volume_toggle_action` (class, bn_music_actions.h:130)

Changes the volume of the active Direct Sound music when the action is updated a given number of times.

`class music_volume_toggle_action : public toggle_template_action<fixed, music_volume_manager>`

- **music_volume_toggle_action** `music_volume_toggle_action(int duration_updates, fixed new_volume) : toggle_template_action(duration_updates, new_volume)` (L141): Constructor.
  `duration_updates` How many times the action has to be updated to change the volume.; `new_volume` New volume when the action is updated duration_updates times.
  This volume must be in the range [0..1].
- **new_volume** `fixed new_volume() const` (L150): Returns the volume when the action is updated the given number of times.
### `bn::music_tempo_manager` (class, bn_music_actions.h:165)

Manages the tempo of the active Direct Sound music.

`class music_tempo_manager`

- **get** `static fixed get()` (L172): Returns the tempo of the active Direct Sound music.
- **set** `static void set(fixed tempo)` (L181): Sets the tempo of the active Direct Sound music.
  `tempo` New tempo in the range [0.5..2].
### `bn::music_tempo_to_action` (class, bn_music_actions.h:194)

Modifies the tempo of the active Direct Sound music until it has a given state.

`class music_tempo_to_action : public to_template_action<fixed, music_tempo_manager>`

- **music_tempo_to_action** `music_tempo_to_action(int duration_updates, fixed final_tempo) : to_template_action(duration_updates, final_tempo)` (L206): Constructor.
  `duration_updates` Number of times that the action must be updated until the tempo is equal to final_tempo.; `final_tempo` tempo when the action is updated duration_updates times.
  This tempo must be in the range [0.5..2].
- **final_tempo** `fixed final_tempo() const` (L215): Returns the tempo when the action is updated the given number of times.
### `bn::music_tempo_loop_action` (class, bn_music_actions.h:229)

Modifies the tempo of the active Direct Sound music from a minimum to a maximum. When the tempo is equal to the given final state, it goes back to its initial state and vice versa.

`class music_tempo_loop_action : public loop_template_action<fixed, music_tempo_manager>`

- **music_tempo_loop_action** `music_tempo_loop_action(int duration_updates, fixed final_tempo) : loop_template_action(duration_updates, final_tempo)` (L242): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the tempo delta.; `final_tempo` When the tempo is equal to this parameter, it goes back to its initial state and vice versa.
  This tempo must be in the range [0.5..2].
- **final_tempo** `fixed final_tempo() const` (L251): When the tempo is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::music_tempo_toggle_action` (class, bn_music_actions.h:264)

Changes the tempo of the active Direct Sound music when the action is updated a given number of times.

`class music_tempo_toggle_action : public toggle_template_action<fixed, music_tempo_manager>`

- **music_tempo_toggle_action** `music_tempo_toggle_action(int duration_updates, fixed new_tempo) : toggle_template_action(duration_updates, new_tempo)` (L275): Constructor.
  `duration_updates` How many times the action has to be updated to change the tempo.; `new_tempo` New tempo when the action is updated duration_updates times.
  This tempo must be in the range [0.5..2].
- **new_tempo** `fixed new_tempo() const` (L284): Returns the tempo when the action is updated the given number of times.
### `bn::music_pitch_manager` (class, bn_music_actions.h:299)

Manages the pitch of the active Direct Sound music.

`class music_pitch_manager`

- **get** `static fixed get()` (L306): Returns the pitch of the active Direct Sound music.
- **set** `static void set(fixed pitch)` (L315): Sets the pitch of the active Direct Sound music.
  `pitch` New pitch in the range [0.5..2].
### `bn::music_pitch_to_action` (class, bn_music_actions.h:328)

Modifies the pitch of the active Direct Sound music until it has a given state.

`class music_pitch_to_action : public to_template_action<fixed, music_pitch_manager>`

- **music_pitch_to_action** `music_pitch_to_action(int duration_updates, fixed final_pitch) : to_template_action(duration_updates, final_pitch)` (L340): Constructor.
  `duration_updates` Number of times that the action must be updated until the pitch is equal to final_pitch.; `final_pitch` pitch when the action is updated duration_updates times.
  This pitch must be in the range [0.5..2].
- **final_pitch** `fixed final_pitch() const` (L349): Returns the pitch when the action is updated the given number of times.
### `bn::music_pitch_loop_action` (class, bn_music_actions.h:363)

Modifies the pitch of the active Direct Sound music from a minimum to a maximum. When the pitch is equal to the given final state, it goes back to its initial state and vice versa.

`class music_pitch_loop_action : public loop_template_action<fixed, music_pitch_manager>`

- **music_pitch_loop_action** `music_pitch_loop_action(int duration_updates, fixed final_pitch) : loop_template_action(duration_updates, final_pitch)` (L376): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the pitch delta.; `final_pitch` When the pitch is equal to this parameter, it goes back to its initial state and vice versa.
  This pitch must be in the range [0.5..2].
- **final_pitch** `fixed final_pitch() const` (L385): When the pitch is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::music_pitch_toggle_action` (class, bn_music_actions.h:398)

Changes the pitch of the active Direct Sound music when the action is updated a given number of times.

`class music_pitch_toggle_action : public toggle_template_action<fixed, music_pitch_manager>`

- **music_pitch_toggle_action** `music_pitch_toggle_action(int duration_updates, fixed new_pitch) : toggle_template_action(duration_updates, new_pitch)` (L409): Constructor.
  `duration_updates` How many times the action has to be updated to change the pitch.; `new_pitch` New pitch when the action is updated duration_updates times.
  This pitch must be in the range [0.5..2].
- **new_pitch** `fixed new_pitch() const` (L418): Returns the pitch when the action is updated the given number of times.

## bn_palette_bitmap_bg_actions.h

### `bn::palette_bitmap_bg_visible_manager` (class, bn_palette_bitmap_bg_actions.h:31)

Manages if a palette_bitmap_bg_ptr must be committed to the GBA or not.

`class palette_bitmap_bg_visible_manager`

- **get** `static bool get(const palette_bitmap_bg_ptr& bg)` (L38): Indicates if the given palette_bitmap_bg_ptr is committed to the GBA or not.
- **set** `static void set(bool visible, palette_bitmap_bg_ptr& bg)` (L46): Sets if the given palette_bitmap_bg_ptr must be committed to the GBA or not.
### `bn::palette_bitmap_bg_visible_toggle_action` (class, bn_palette_bitmap_bg_actions.h:60)

Toggles if a palette_bitmap_bg_ptr must be committed to the GBA or not when the action is updated a given number of times.

`class palette_bitmap_bg_visible_toggle_action : public bool_toggle_value_template_action<palette_bitmap_bg_ptr, palette_bitmap_bg_visible_manager>`

- **palette_bitmap_bg_visible_toggle_action** `palette_bitmap_bg_visible_toggle_action(const palette_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L71): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given palette_bitmap_bg_ptr must be committed to the GBA or not.
- **palette_bitmap_bg_visible_toggle_action** `palette_bitmap_bg_visible_toggle_action(palette_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L82): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given palette_bitmap_bg_ptr must be committed to the GBA or not.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L90): Returns the palette_bitmap_bg_ptr to modify.
### `bn::palette_bitmap_bg_position_manager` (class, bn_palette_bitmap_bg_actions.h:105)

Manages the position of a palette_bitmap_bg_ptr.

`class palette_bitmap_bg_position_manager`

- **get** `static const fixed_point& get(const palette_bitmap_bg_ptr& bg)` (L112): Returns the position of the given palette_bitmap_bg_ptr.
- **set** `static void set(const fixed_point& position, palette_bitmap_bg_ptr& bg)` (L120): Sets the position of the given palette_bitmap_bg_ptr.
### `bn::palette_bitmap_bg_move_by_action` (class, bn_palette_bitmap_bg_actions.h:133)

Modifies the position of a palette_bitmap_bg_ptr by a given delta.

`class palette_bitmap_bg_move_by_action : public by_value_template_action<palette_bitmap_bg_ptr, fixed_point, palette_bitmap_bg_position_manager>`

- **palette_bitmap_bg_move_by_action** `palette_bitmap_bg_move_by_action(const palette_bitmap_bg_ptr& bg, fixed delta_x, fixed delta_y) : by_value_template_action(bg, fixed_point(delta_x, delta_y))` (L146): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `delta_x` How much to add to the horizontal position of the given palette_bitmap_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the given palette_bitmap_bg_ptr when the action is updated.
- **palette_bitmap_bg_move_by_action** `palette_bitmap_bg_move_by_action(palette_bitmap_bg_ptr&& bg, fixed delta_x, fixed delta_y) : by_value_template_action(move(bg), fixed_point(delta_x, delta_y))` (L159): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `delta_x` How much to add to the horizontal position of the given palette_bitmap_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the given palette_bitmap_bg_ptr when the action is updated.
- **palette_bitmap_bg_move_by_action** `palette_bitmap_bg_move_by_action(const palette_bitmap_bg_ptr& bg, const fixed_point& delta_position) : by_value_template_action(bg, delta_position)` (L170): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `delta_position` How much to add to the position of the given palette_bitmap_bg_ptr when the action is updated.
- **palette_bitmap_bg_move_by_action** `palette_bitmap_bg_move_by_action(palette_bitmap_bg_ptr&& bg, const fixed_point& delta_position) : by_value_template_action(move(bg), delta_position)` (L181): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `delta_position` How much to add to the position of the given palette_bitmap_bg_ptr when the action is updated.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L189): Returns the palette_bitmap_bg_ptr to modify.
- **delta_position** `const fixed_point& delta_position() const` (L197): Returns how much to add to the position of the given palette_bitmap_bg_ptr when the action is updated.
### `bn::palette_bitmap_bg_move_to_action` (class, bn_palette_bitmap_bg_actions.h:210)

Modifies the position of a palette_bitmap_bg_ptr until it has a given state.

`class palette_bitmap_bg_move_to_action : public to_value_template_action<palette_bitmap_bg_ptr, fixed_point, palette_bitmap_bg_position_manager>`

- **palette_bitmap_bg_move_to_action** `palette_bitmap_bg_move_to_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L225): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the given palette_bitmap_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.
- **palette_bitmap_bg_move_to_action** `palette_bitmap_bg_move_to_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L241): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the given palette_bitmap_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.
- **palette_bitmap_bg_move_to_action** `palette_bitmap_bg_move_to_action( const palette_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(bg, duration_updates, final_position)` (L255): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the given palette_bitmap_bg_ptr is equal to final_position.; `final_position` Position of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.
- **palette_bitmap_bg_move_to_action** `palette_bitmap_bg_move_to_action( palette_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(move(bg), duration_updates, final_position)` (L269): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the given palette_bitmap_bg_ptr is equal to final_position.; `final_position` Position of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L278): Returns the palette_bitmap_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L287): Returns the position of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_move_loop_action` (class, bn_palette_bitmap_bg_actions.h:301)

Modifies the position of a palette_bitmap_bg_ptr from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class palette_bitmap_bg_move_loop_action : public loop_value_template_action<palette_bitmap_bg_ptr, fixed_point, palette_bitmap_bg_position_manager>`

- **palette_bitmap_bg_move_loop_action** `palette_bitmap_bg_move_loop_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L316): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **palette_bitmap_bg_move_loop_action** `palette_bitmap_bg_move_loop_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L332): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **palette_bitmap_bg_move_loop_action** `palette_bitmap_bg_move_loop_action( const palette_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(bg, duration_updates, final_position)` (L346): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **palette_bitmap_bg_move_loop_action** `palette_bitmap_bg_move_loop_action( palette_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(move(bg), duration_updates, final_position)` (L360): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L369): Returns the palette_bitmap_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L378): When the position of the given palette_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::palette_bitmap_bg_move_toggle_action` (class, bn_palette_bitmap_bg_actions.h:391)

Changes the position of a palette_bitmap_bg_ptr when the action is updated a given number of times.

`class palette_bitmap_bg_move_toggle_action : public toggle_value_template_action<palette_bitmap_bg_ptr, fixed_point, palette_bitmap_bg_position_manager>`

- **palette_bitmap_bg_move_toggle_action** `palette_bitmap_bg_move_toggle_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(bg, duration_updates, fixed_point(new_x, new_y))` (L404): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the given palette_bitmap_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **palette_bitmap_bg_move_toggle_action** `palette_bitmap_bg_move_toggle_action(palette_bitmap_bg_ptr&& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(move(bg), duration_updates, fixed_point(new_x, new_y))` (L418): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the given palette_bitmap_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **palette_bitmap_bg_move_toggle_action** `palette_bitmap_bg_move_toggle_action( const palette_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(bg, duration_updates, new_position)` (L430): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the given palette_bitmap_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **palette_bitmap_bg_move_toggle_action** `palette_bitmap_bg_move_toggle_action( palette_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(move(bg), duration_updates, new_position)` (L443): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the given palette_bitmap_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L452): Returns the palette_bitmap_bg_ptr to modify.
- **new_position** `const fixed_point& new_position() const` (L461): Returns the position of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_top_left_position_manager` (class, bn_palette_bitmap_bg_actions.h:476)

Manages the top-left position of a palette_bitmap_bg_ptr.

`class palette_bitmap_bg_top_left_position_manager`

- **get** `static fixed_point get(const palette_bitmap_bg_ptr& bg)` (L483): Returns the top-left position of the given palette_bitmap_bg_ptr.
- **set** `static void set(const fixed_point& top_left_position, palette_bitmap_bg_ptr& bg)` (L491): Sets the top-left position of the given palette_bitmap_bg_ptr.
### `bn::palette_bitmap_bg_top_left_move_to_action` (class, bn_palette_bitmap_bg_actions.h:504)

Modifies the top-left position of a palette_bitmap_bg_ptr until it has a given state.

`class palette_bitmap_bg_top_left_move_to_action : public to_value_template_action<palette_bitmap_bg_ptr, fixed_point, palette_bitmap_bg_top_left_position_manager>`

- **palette_bitmap_bg_top_left_move_to_action** `palette_bitmap_bg_top_left_move_to_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : to_value_template_action(bg, duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L519): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the top-left position of the given palette_bitmap_bg_ptr is equal to (final_x, final_y).; `final_top_left_x` Horizontal top-left position of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.; `final_top_left_y` Vertical top-left position of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.
- **palette_bitmap_bg_top_left_move_to_action** `palette_bitmap_bg_top_left_move_to_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : to_value_template_action(move(bg), duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L535): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the top-left position of the given palette_bitmap_bg_ptr is equal to (final_x, final_y).; `final_top_left_x` Horizontal top-left position of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.; `final_top_left_y` Vertical top-left position of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.
- **palette_bitmap_bg_top_left_move_to_action** `palette_bitmap_bg_top_left_move_to_action( const palette_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_top_left_position) : to_value_template_action(bg, duration_updates, final_top_left_position)` (L549): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the top-left position of the given palette_bitmap_bg_ptr is equal to final_top_left_position.; `final_top_left_position` Top-left position of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.
- **palette_bitmap_bg_top_left_move_to_action** `palette_bitmap_bg_top_left_move_to_action( palette_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_top_left_position) : to_value_template_action(move(bg), duration_updates, final_top_left_position)` (L563): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the top-left position of the given palette_bitmap_bg_ptr is equal to final_top_left_position.; `final_top_left_position` Top-left position of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L572): Returns the palette_bitmap_bg_ptr to modify.
- **final_top_left_position** `const fixed_point& final_top_left_position() const` (L581): Returns the top-left position of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_top_left_move_loop_action` (class, bn_palette_bitmap_bg_actions.h:595)

Modifies the top-left position of a palette_bitmap_bg_ptr from a minimum to a maximum. When the top-left position is equal to the given final state, it goes back to its initial state and vice versa.

`class palette_bitmap_bg_top_left_move_loop_action : public loop_value_template_action<palette_bitmap_bg_ptr, fixed_point, palette_bitmap_bg_top_left_position_manager>`

- **palette_bitmap_bg_top_left_move_loop_action** `palette_bitmap_bg_top_left_move_loop_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : loop_value_template_action(bg, duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L610): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_x` When the horizontal top-left position of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_top_left_y` When the vertical top-left position of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **palette_bitmap_bg_top_left_move_loop_action** `palette_bitmap_bg_top_left_move_loop_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : loop_value_template_action(move(bg), duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L626): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_x` When the horizontal top-left position of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_top_left_y` When the vertical top-left position of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **palette_bitmap_bg_top_left_move_loop_action** `palette_bitmap_bg_top_left_move_loop_action( const palette_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_top_left_position) : loop_value_template_action(bg, duration_updates, final_top_left_position)` (L640): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_position` When the top-left position of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **palette_bitmap_bg_top_left_move_loop_action** `palette_bitmap_bg_top_left_move_loop_action( palette_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_top_left_position) : loop_value_template_action(move(bg), duration_updates, final_top_left_position)` (L654): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_position` When the top-left position of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L663): Returns the palette_bitmap_bg_ptr to modify.
- **final_top_left_position** `const fixed_point& final_top_left_position() const` (L672): When the top-left position of the given palette_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::palette_bitmap_bg_top_left_move_toggle_action` (class, bn_palette_bitmap_bg_actions.h:685)

Changes the top-left position of a palette_bitmap_bg_ptr when the action is updated a given number of times.

`class palette_bitmap_bg_top_left_move_toggle_action : public toggle_value_template_action<palette_bitmap_bg_ptr, fixed_point, palette_bitmap_bg_top_left_position_manager>`

- **palette_bitmap_bg_top_left_move_toggle_action** `palette_bitmap_bg_top_left_move_toggle_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) : toggle_value_template_action(bg, duration_updates, fixed_point(new_top_left_x, new_top_left_y))` (L698): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the top-left position of the given palette_bitmap_bg_ptr.; `new_top_left_x` New horizontal top-left position when the action is updated duration_updates times.; `new_top_left_y` New vertical top-left position when the action is updated duration_updates times.
- **palette_bitmap_bg_top_left_move_toggle_action** `palette_bitmap_bg_top_left_move_toggle_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) : toggle_value_template_action(move(bg), duration_updates, fixed_point(new_top_left_x, new_top_left_y))` (L712): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the top-left position of the given palette_bitmap_bg_ptr.; `new_top_left_x` New horizontal top-left position when the action is updated duration_updates times.; `new_top_left_y` New vertical top-left position when the action is updated duration_updates times.
- **palette_bitmap_bg_top_left_move_toggle_action** `palette_bitmap_bg_top_left_move_toggle_action( const palette_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& new_top_left_position) : toggle_value_template_action(bg, duration_updates, new_top_left_position)` (L725): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the top-left position of the given palette_bitmap_bg_ptr.; `new_top_left_position` New top-left position when the action is updated duration_updates times.
- **palette_bitmap_bg_top_left_move_toggle_action** `palette_bitmap_bg_top_left_move_toggle_action( palette_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& new_top_left_position) : toggle_value_template_action(move(bg), duration_updates, new_top_left_position)` (L738): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the top-left position of the given palette_bitmap_bg_ptr.; `new_top_left_position` New top-left position when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L747): Returns the palette_bitmap_bg_ptr to modify.
- **new_top_left_position** `const fixed_point& new_top_left_position() const` (L756): Returns the top-left position of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_rotation_manager` (class, bn_palette_bitmap_bg_actions.h:771)

Manages the rotation angle of a palette_bitmap_bg_ptr.

`class palette_bitmap_bg_rotation_manager`

- **get** `static fixed get(const palette_bitmap_bg_ptr& bg)` (L778): Returns the rotation angle of the given palette_bitmap_bg_ptr.
- **set** `static void set(fixed rotation_angle, palette_bitmap_bg_ptr& bg)` (L788): Sets the rotation angle of the given palette_bitmap_bg_ptr.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].; `bg` palette_bitmap_bg_ptr to modify.
### `bn::palette_bitmap_bg_rotate_by_action` (class, bn_palette_bitmap_bg_actions.h:802)

Modifies the rotation angle of a palette_bitmap_bg_ptr by delta_rotation_angle. When the rotation angle is over 360, it goes back to 0 and vice versa.

`class palette_bitmap_bg_rotate_by_action : public cyclic_by_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_rotation_manager>`

- **palette_bitmap_bg_rotate_by_action** `palette_bitmap_bg_rotate_by_action(const palette_bitmap_bg_ptr& bg, fixed delta_rotation_angle) : cyclic_by_value_template_action(bg, delta_rotation_angle, 0, 360)` (L815): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `delta_rotation_angle` How many degrees to add to the rotation angle of the given palette_bitmap_bg_ptr when the action is updated.
  This rotation angle must be in the range [0..360].
- **palette_bitmap_bg_rotate_by_action** `palette_bitmap_bg_rotate_by_action(palette_bitmap_bg_ptr&& bg, fixed delta_rotation_angle) : cyclic_by_value_template_action(move(bg), delta_rotation_angle, 0, 360)` (L830): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `delta_rotation_angle` How many degrees to add to the rotation angle of the given palette_bitmap_bg_ptr when the action is updated.
  This rotation angle must be in the range [0..360].
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L840): Returns the palette_bitmap_bg_ptr to modify.
- **delta_rotation_angle** `fixed delta_rotation_angle() const` (L849): Returns how many degrees to add to the rotation angle of the given palette_bitmap_bg_ptr when the action is updated.
### `bn::palette_bitmap_bg_rotate_to_action` (class, bn_palette_bitmap_bg_actions.h:862)

Modifies the rotation angle of a palette_bitmap_bg_ptr until it has a given state.

`class palette_bitmap_bg_rotate_to_action : public to_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_rotation_manager>`

- **palette_bitmap_bg_rotate_to_action** `palette_bitmap_bg_rotate_to_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_rotation_angle) : to_value_template_action(bg, duration_updates, final_rotation_angle)` (L876): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the rotation angle of the given palette_bitmap_bg_ptr is equal to final_rotation_angle.; `final_rotation_angle` Rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **palette_bitmap_bg_rotate_to_action** `palette_bitmap_bg_rotate_to_action(palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_rotation_angle) : to_value_template_action(move(bg), duration_updates, final_rotation_angle)` (L893): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the rotation angle of the given palette_bitmap_bg_ptr is equal to final_rotation_angle.; `final_rotation_angle` Rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L903): Returns the palette_bitmap_bg_ptr to modify.
- **final_rotation_angle** `fixed final_rotation_angle() const` (L912): Returns the rotation angle of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_rotate_loop_action` (class, bn_palette_bitmap_bg_actions.h:926)

Modifies the rotation angle of a palette_bitmap_bg_ptr from a minimum to a maximum. When the rotation angle is equal to the given final state, it goes back to its initial state and vice versa.

`class palette_bitmap_bg_rotate_loop_action : public loop_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_rotation_manager>`

- **palette_bitmap_bg_rotate_loop_action** `palette_bitmap_bg_rotate_loop_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_rotation_angle) : loop_value_template_action(bg, duration_updates, final_rotation_angle)` (L941): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the rotation angle delta.; `final_rotation_angle` When the rotation angle of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
  This rotation angle must be in the range [0..360].
- **palette_bitmap_bg_rotate_loop_action** `palette_bitmap_bg_rotate_loop_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_rotation_angle) : loop_value_template_action(move(bg), duration_updates, final_rotation_angle)` (L959): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the rotation angle delta.; `final_rotation_angle` When the rotation angle of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
  This rotation angle must be in the range [0..360].
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L970): Returns the palette_bitmap_bg_ptr to modify.
- **final_rotation_angle** `fixed final_rotation_angle() const` (L979): When the rotation angle of the given palette_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::palette_bitmap_bg_rotate_toggle_action` (class, bn_palette_bitmap_bg_actions.h:992)

Changes the rotation angle of a palette_bitmap_bg_ptr when the action is updated a given number of times.

`class palette_bitmap_bg_rotate_toggle_action : public toggle_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_rotation_manager>`

- **palette_bitmap_bg_rotate_toggle_action** `palette_bitmap_bg_rotate_toggle_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed new_rotation_angle) : toggle_value_template_action(bg, duration_updates, new_rotation_angle)` (L1006): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the rotation angle of the given palette_bitmap_bg_ptr.; `new_rotation_angle` New rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **palette_bitmap_bg_rotate_toggle_action** `palette_bitmap_bg_rotate_toggle_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed new_rotation_angle) : toggle_value_template_action(move(bg), duration_updates, new_rotation_angle)` (L1023): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the rotation angle of the given palette_bitmap_bg_ptr.; `new_rotation_angle` New rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L1034): Returns the palette_bitmap_bg_ptr to modify.
- **new_rotation_angle** `fixed new_rotation_angle() const` (L1043): Returns the rotation angle of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_horizontal_scale_manager` (class, bn_palette_bitmap_bg_actions.h:1058)

Manages the horizontal scale of a palette_bitmap_bg_ptr.

`class palette_bitmap_bg_horizontal_scale_manager`

- **get** `static fixed get(const palette_bitmap_bg_ptr& bg)` (L1065): Returns the horizontal scale of the given palette_bitmap_bg_ptr.
- **set** `static void set(fixed horizontal_scale, palette_bitmap_bg_ptr& bg)` (L1073): Sets the horizontal scale of the given palette_bitmap_bg_ptr.
### `bn::palette_bitmap_bg_horizontal_scale_to_action` (class, bn_palette_bitmap_bg_actions.h:1086)

Modifies the horizontal scale of a palette_bitmap_bg_ptr until it has a given state.

`class palette_bitmap_bg_horizontal_scale_to_action : public to_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_horizontal_scale_manager>`

- **palette_bitmap_bg_horizontal_scale_to_action** `palette_bitmap_bg_horizontal_scale_to_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_horizontal_scale) : to_value_template_action(bg, duration_updates, final_horizontal_scale)` (L1098): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the horizontal scale of the given palette_bitmap_bg_ptr is equal to final_horizontal_scale.; `final_horizontal_scale` Horizontal scale when the action is updated duration_updates times.
- **palette_bitmap_bg_horizontal_scale_to_action** `palette_bitmap_bg_horizontal_scale_to_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_horizontal_scale) : to_value_template_action(move(bg), duration_updates, final_horizontal_scale)` (L1112): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the horizontal scale of the given palette_bitmap_bg_ptr is equal to final_horizontal_scale.; `final_horizontal_scale` Horizontal scale when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L1122): Returns the palette_bitmap_bg_ptr to modify.
- **final_horizontal_scale** `fixed final_horizontal_scale() const` (L1131): Returns the horizontal scale of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_horizontal_scale_loop_action` (class, bn_palette_bitmap_bg_actions.h:1145)

Modifies the horizontal scale of a palette_bitmap_bg_ptr from a minimum to a maximum. When the horizontal scale is equal to the given final state, it goes back to its initial state and vice versa.

`class palette_bitmap_bg_horizontal_scale_loop_action : public loop_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_horizontal_scale_manager>`

- **palette_bitmap_bg_horizontal_scale_loop_action** `palette_bitmap_bg_horizontal_scale_loop_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_horizontal_scale) : loop_value_template_action(bg, duration_updates, final_horizontal_scale)` (L1158): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal scale delta.; `final_horizontal_scale` When the horizontal scale of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **palette_bitmap_bg_horizontal_scale_loop_action** `palette_bitmap_bg_horizontal_scale_loop_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_horizontal_scale) : loop_value_template_action(move(bg), duration_updates, final_horizontal_scale)` (L1173): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal scale delta.; `final_horizontal_scale` When the horizontal scale of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L1183): Returns the palette_bitmap_bg_ptr to modify.
- **final_horizontal_scale** `fixed final_horizontal_scale() const` (L1192): When the horizontal scale of the given palette_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::palette_bitmap_bg_horizontal_scale_toggle_action` (class, bn_palette_bitmap_bg_actions.h:1206)

Changes the horizontal scale of a palette_bitmap_bg_ptr when the action is updated a given number of times.

`class palette_bitmap_bg_horizontal_scale_toggle_action : public toggle_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_horizontal_scale_manager>`

- **palette_bitmap_bg_horizontal_scale_toggle_action** `palette_bitmap_bg_horizontal_scale_toggle_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed new_horizontal_scale) : toggle_value_template_action(bg, duration_updates, new_horizontal_scale)` (L1218): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the horizontal scale of the given palette_bitmap_bg_ptr.; `new_horizontal_scale` New horizontal scale when the action is updated duration_updates times.
- **palette_bitmap_bg_horizontal_scale_toggle_action** `palette_bitmap_bg_horizontal_scale_toggle_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed new_horizontal_scale) : toggle_value_template_action(move(bg), duration_updates, new_horizontal_scale)` (L1232): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the horizontal scale of the given palette_bitmap_bg_ptr.; `new_horizontal_scale` New horizontal scale when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L1242): Returns the palette_bitmap_bg_ptr to modify.
- **new_horizontal_scale** `fixed new_horizontal_scale() const` (L1251): Returns the horizontal scale of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_vertical_scale_manager` (class, bn_palette_bitmap_bg_actions.h:1266)

Manages the vertical scale of a palette_bitmap_bg_ptr.

`class palette_bitmap_bg_vertical_scale_manager`

- **get** `static fixed get(const palette_bitmap_bg_ptr& bg)` (L1273): Returns the vertical scale of the given palette_bitmap_bg_ptr.
- **set** `static void set(fixed vertical_scale, palette_bitmap_bg_ptr& bg)` (L1281): Sets the vertical scale of the given palette_bitmap_bg_ptr.
### `bn::palette_bitmap_bg_vertical_scale_to_action` (class, bn_palette_bitmap_bg_actions.h:1294)

Modifies the vertical scale of a palette_bitmap_bg_ptr until it has a given state.

`class palette_bitmap_bg_vertical_scale_to_action : public to_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_vertical_scale_manager>`

- **palette_bitmap_bg_vertical_scale_to_action** `palette_bitmap_bg_vertical_scale_to_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_vertical_scale) : to_value_template_action(bg, duration_updates, final_vertical_scale)` (L1306): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the vertical scale of the given palette_bitmap_bg_ptr is equal to final_vertical_scale.; `final_vertical_scale` Vertical scale when the action is updated duration_updates times.
- **palette_bitmap_bg_vertical_scale_to_action** `palette_bitmap_bg_vertical_scale_to_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_vertical_scale) : to_value_template_action(move(bg), duration_updates, final_vertical_scale)` (L1320): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the vertical scale of the given palette_bitmap_bg_ptr is equal to final_vertical_scale.; `final_vertical_scale` Vertical scale when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L1330): Returns the palette_bitmap_bg_ptr to modify.
- **final_vertical_scale** `fixed final_vertical_scale() const` (L1339): Returns the vertical scale of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_vertical_scale_loop_action` (class, bn_palette_bitmap_bg_actions.h:1353)

Modifies the vertical scale of a palette_bitmap_bg_ptr from a minimum to a maximum. When the vertical scale is equal to the given final state, it goes back to its initial state and vice versa.

`class palette_bitmap_bg_vertical_scale_loop_action : public loop_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_vertical_scale_manager>`

- **palette_bitmap_bg_vertical_scale_loop_action** `palette_bitmap_bg_vertical_scale_loop_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_vertical_scale) : loop_value_template_action(bg, duration_updates, final_vertical_scale)` (L1366): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical scale delta.; `final_vertical_scale` When the vertical scale of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **palette_bitmap_bg_vertical_scale_loop_action** `palette_bitmap_bg_vertical_scale_loop_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_vertical_scale) : loop_value_template_action(move(bg), duration_updates, final_vertical_scale)` (L1381): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical scale delta.; `final_vertical_scale` When the vertical scale of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L1391): Returns the palette_bitmap_bg_ptr to modify.
- **final_vertical_scale** `fixed final_vertical_scale() const` (L1400): When the vertical scale of the given palette_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::palette_bitmap_bg_vertical_scale_toggle_action` (class, bn_palette_bitmap_bg_actions.h:1414)

Changes the vertical scale of a palette_bitmap_bg_ptr when the action is updated a given number of times.

`class palette_bitmap_bg_vertical_scale_toggle_action : public toggle_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_vertical_scale_manager>`

- **palette_bitmap_bg_vertical_scale_toggle_action** `palette_bitmap_bg_vertical_scale_toggle_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed new_vertical_scale) : toggle_value_template_action(bg, duration_updates, new_vertical_scale)` (L1426): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the vertical scale of the given palette_bitmap_bg_ptr.; `new_vertical_scale` New vertical scale when the action is updated duration_updates times.
- **palette_bitmap_bg_vertical_scale_toggle_action** `palette_bitmap_bg_vertical_scale_toggle_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed new_vertical_scale) : toggle_value_template_action(move(bg), duration_updates, new_vertical_scale)` (L1440): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the vertical scale of the given palette_bitmap_bg_ptr.; `new_vertical_scale` New vertical scale when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L1450): Returns the palette_bitmap_bg_ptr to modify.
- **new_vertical_scale** `fixed new_vertical_scale() const` (L1459): Returns the vertical scale of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_scale_manager` (class, bn_palette_bitmap_bg_actions.h:1474)

Manages the scale of a palette_bitmap_bg_ptr.

`class palette_bitmap_bg_scale_manager`

- **get** `static fixed get(const palette_bitmap_bg_ptr& bg)` (L1481): Returns the horizontal scale of the given palette_bitmap_bg_ptr.
- **set** `static void set(fixed scale, palette_bitmap_bg_ptr& bg)` (L1489): Sets the scale of the given palette_bitmap_bg_ptr.
### `bn::palette_bitmap_bg_scale_to_action` (class, bn_palette_bitmap_bg_actions.h:1502)

Modifies the scale of a palette_bitmap_bg_ptr until it has a given state.

`class palette_bitmap_bg_scale_to_action : public to_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_scale_manager>`

- **palette_bitmap_bg_scale_to_action** `palette_bitmap_bg_scale_to_action(const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_scale) : to_value_template_action(bg, duration_updates, final_scale)` (L1514): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the scale of the given palette_bitmap_bg_ptr is equal to final_scale.; `final_scale` scale when the action is updated duration_updates times.
- **palette_bitmap_bg_scale_to_action** `palette_bitmap_bg_scale_to_action(palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_scale) : to_value_template_action(move(bg), duration_updates, final_scale)` (L1527): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the scale of the given palette_bitmap_bg_ptr is equal to final_scale.; `final_scale` Scale when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L1536): Returns the palette_bitmap_bg_ptr to modify.
- **final_scale** `fixed final_scale() const` (L1545): Returns the scale of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_scale_loop_action` (class, bn_palette_bitmap_bg_actions.h:1559)

Modifies the scale of a palette_bitmap_bg_ptr from a minimum to a maximum. When the scale is equal to the given final state, it goes back to its initial state and vice versa.

`class palette_bitmap_bg_scale_loop_action : public loop_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_scale_manager>`

- **palette_bitmap_bg_scale_loop_action** `palette_bitmap_bg_scale_loop_action(const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_scale) : loop_value_template_action(bg, duration_updates, final_scale)` (L1572): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the scale delta.; `final_scale` When the scale of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **palette_bitmap_bg_scale_loop_action** `palette_bitmap_bg_scale_loop_action(palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_scale) : loop_value_template_action(move(bg), duration_updates, final_scale)` (L1586): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the scale delta.; `final_scale` When the scale of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L1595): Returns the palette_bitmap_bg_ptr to modify.
- **final_scale** `fixed final_scale() const` (L1604): When the scale of the given palette_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::palette_bitmap_bg_scale_toggle_action` (class, bn_palette_bitmap_bg_actions.h:1618)

Changes the scale of a palette_bitmap_bg_ptr when the action is updated a given number of times.

`class palette_bitmap_bg_scale_toggle_action : public toggle_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_scale_manager>`

- **palette_bitmap_bg_scale_toggle_action** `palette_bitmap_bg_scale_toggle_action(const palette_bitmap_bg_ptr& bg, int duration_updates, fixed new_scale) : toggle_value_template_action(bg, duration_updates, new_scale)` (L1630): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the scale of the given palette_bitmap_bg_ptr.; `new_scale` New scale when the action is updated duration_updates times.
- **palette_bitmap_bg_scale_toggle_action** `palette_bitmap_bg_scale_toggle_action(palette_bitmap_bg_ptr&& bg, int duration_updates, fixed new_scale) : toggle_value_template_action(move(bg), duration_updates, new_scale)` (L1643): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the scale of the given palette_bitmap_bg_ptr.; `new_scale` New scale when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L1652): Returns the palette_bitmap_bg_ptr to modify.
- **new_scale** `fixed new_scale() const` (L1661): Returns the scale of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_horizontal_shear_manager` (class, bn_palette_bitmap_bg_actions.h:1676)

Manages the horizontal shear of a palette_bitmap_bg_ptr.

`class palette_bitmap_bg_horizontal_shear_manager`

- **get** `static fixed get(const palette_bitmap_bg_ptr& bg)` (L1683): Returns the horizontal shear of the given palette_bitmap_bg_ptr.
- **set** `static void set(fixed horizontal_shear, palette_bitmap_bg_ptr& bg)` (L1691): Sets the horizontal shear of the given palette_bitmap_bg_ptr.
### `bn::palette_bitmap_bg_horizontal_shear_to_action` (class, bn_palette_bitmap_bg_actions.h:1704)

Modifies the horizontal shear of a palette_bitmap_bg_ptr until it has a given state.

`class palette_bitmap_bg_horizontal_shear_to_action : public to_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_horizontal_shear_manager>`

- **palette_bitmap_bg_horizontal_shear_to_action** `palette_bitmap_bg_horizontal_shear_to_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_horizontal_shear) : to_value_template_action(bg, duration_updates, final_horizontal_shear)` (L1716): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the horizontal shear of the given palette_bitmap_bg_ptr is equal to final_horizontal_shear.; `final_horizontal_shear` Horizontal shear when the action is updated duration_updates times.
- **palette_bitmap_bg_horizontal_shear_to_action** `palette_bitmap_bg_horizontal_shear_to_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_horizontal_shear) : to_value_template_action(move(bg), duration_updates, final_horizontal_shear)` (L1729): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the horizontal shear of the given palette_bitmap_bg_ptr is equal to final_horizontal_shear.; `final_horizontal_shear` Horizontal shear when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L1738): Returns the palette_bitmap_bg_ptr to modify.
- **final_horizontal_shear** `fixed final_horizontal_shear() const` (L1747): Returns the horizontal shear of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_horizontal_shear_loop_action` (class, bn_palette_bitmap_bg_actions.h:1761)

Modifies the horizontal shear of a palette_bitmap_bg_ptr from a minimum to a maximum. When the horizontal shear is equal to the given final state, it goes back to its initial state and vice versa.

`class palette_bitmap_bg_horizontal_shear_loop_action : public loop_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_horizontal_shear_manager>`

- **palette_bitmap_bg_horizontal_shear_loop_action** `palette_bitmap_bg_horizontal_shear_loop_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_horizontal_shear) : loop_value_template_action(bg, duration_updates, final_horizontal_shear)` (L1774): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal shear delta.; `final_horizontal_shear` When the horizontal shear of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **palette_bitmap_bg_horizontal_shear_loop_action** `palette_bitmap_bg_horizontal_shear_loop_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_horizontal_shear) : loop_value_template_action(move(bg), duration_updates, final_horizontal_shear)` (L1788): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal shear delta.; `final_horizontal_shear` When the horizontal shear of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L1797): Returns the palette_bitmap_bg_ptr to modify.
- **final_horizontal_shear** `fixed final_horizontal_shear() const` (L1806): When the horizontal shear of the given palette_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::palette_bitmap_bg_horizontal_shear_toggle_action` (class, bn_palette_bitmap_bg_actions.h:1820)

Changes the horizontal shear of a palette_bitmap_bg_ptr when the action is updated a given number of times.

`class palette_bitmap_bg_horizontal_shear_toggle_action : public toggle_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_horizontal_shear_manager>`

- **palette_bitmap_bg_horizontal_shear_toggle_action** `palette_bitmap_bg_horizontal_shear_toggle_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed new_horizontal_shear) : toggle_value_template_action(bg, duration_updates, new_horizontal_shear)` (L1832): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the horizontal shear of the given palette_bitmap_bg_ptr.; `new_horizontal_shear` New horizontal shear when the action is updated duration_updates times.
- **palette_bitmap_bg_horizontal_shear_toggle_action** `palette_bitmap_bg_horizontal_shear_toggle_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed new_horizontal_shear) : toggle_value_template_action(move(bg), duration_updates, new_horizontal_shear)` (L1845): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the horizontal shear of the given palette_bitmap_bg_ptr.; `new_horizontal_shear` New horizontal shear when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L1854): Returns the palette_bitmap_bg_ptr to modify.
- **new_horizontal_shear** `fixed new_horizontal_shear() const` (L1863): Returns the horizontal shear of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_vertical_shear_manager` (class, bn_palette_bitmap_bg_actions.h:1878)

Manages the vertical shear of a palette_bitmap_bg_ptr.

`class palette_bitmap_bg_vertical_shear_manager`

- **get** `static fixed get(const palette_bitmap_bg_ptr& bg)` (L1885): Returns the vertical shear of the given palette_bitmap_bg_ptr.
- **set** `static void set(fixed vertical_shear, palette_bitmap_bg_ptr& bg)` (L1893): Sets the vertical shear of the given palette_bitmap_bg_ptr.
### `bn::palette_bitmap_bg_vertical_shear_to_action` (class, bn_palette_bitmap_bg_actions.h:1906)

Modifies the vertical shear of a palette_bitmap_bg_ptr until it has a given state.

`class palette_bitmap_bg_vertical_shear_to_action : public to_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_vertical_shear_manager>`

- **palette_bitmap_bg_vertical_shear_to_action** `palette_bitmap_bg_vertical_shear_to_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_vertical_shear) : to_value_template_action(bg, duration_updates, final_vertical_shear)` (L1918): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the vertical shear of the given palette_bitmap_bg_ptr is equal to final_vertical_shear.; `final_vertical_shear` Vertical shear when the action is updated duration_updates times.
- **palette_bitmap_bg_vertical_shear_to_action** `palette_bitmap_bg_vertical_shear_to_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_vertical_shear) : to_value_template_action(move(bg), duration_updates, final_vertical_shear)` (L1931): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the vertical shear of the given palette_bitmap_bg_ptr is equal to final_vertical_shear.; `final_vertical_shear` Vertical shear when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L1940): Returns the palette_bitmap_bg_ptr to modify.
- **final_vertical_shear** `fixed final_vertical_shear() const` (L1949): Returns the vertical shear of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_vertical_shear_loop_action` (class, bn_palette_bitmap_bg_actions.h:1963)

Modifies the vertical shear of a palette_bitmap_bg_ptr from a minimum to a maximum. When the vertical shear is equal to the given final state, it goes back to its initial state and vice versa.

`class palette_bitmap_bg_vertical_shear_loop_action : public loop_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_vertical_shear_manager>`

- **palette_bitmap_bg_vertical_shear_loop_action** `palette_bitmap_bg_vertical_shear_loop_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_vertical_shear) : loop_value_template_action(bg, duration_updates, final_vertical_shear)` (L1976): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical shear delta.; `final_vertical_shear` When the vertical shear of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **palette_bitmap_bg_vertical_shear_loop_action** `palette_bitmap_bg_vertical_shear_loop_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_vertical_shear) : loop_value_template_action(move(bg), duration_updates, final_vertical_shear)` (L1990): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical shear delta.; `final_vertical_shear` When the vertical shear of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L1999): Returns the palette_bitmap_bg_ptr to modify.
- **final_vertical_shear** `fixed final_vertical_shear() const` (L2008): When the vertical shear of the given palette_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::palette_bitmap_bg_vertical_shear_toggle_action` (class, bn_palette_bitmap_bg_actions.h:2022)

Changes the vertical shear of a palette_bitmap_bg_ptr when the action is updated a given number of times.

`class palette_bitmap_bg_vertical_shear_toggle_action : public toggle_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_vertical_shear_manager>`

- **palette_bitmap_bg_vertical_shear_toggle_action** `palette_bitmap_bg_vertical_shear_toggle_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed new_vertical_shear) : toggle_value_template_action(bg, duration_updates, new_vertical_shear)` (L2034): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the vertical shear of the given palette_bitmap_bg_ptr.; `new_vertical_shear` New vertical shear when the action is updated duration_updates times.
- **palette_bitmap_bg_vertical_shear_toggle_action** `palette_bitmap_bg_vertical_shear_toggle_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed new_vertical_shear) : toggle_value_template_action(move(bg), duration_updates, new_vertical_shear)` (L2047): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the vertical shear of the given palette_bitmap_bg_ptr.; `new_vertical_shear` New vertical shear when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L2056): Returns the palette_bitmap_bg_ptr to modify.
- **new_vertical_shear** `fixed new_vertical_shear() const` (L2065): Returns the vertical shear of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_shear_manager` (class, bn_palette_bitmap_bg_actions.h:2080)

Manages the shear of a palette_bitmap_bg_ptr.

`class palette_bitmap_bg_shear_manager`

- **get** `static fixed get(const palette_bitmap_bg_ptr& bg)` (L2087): Returns the horizontal shear of the given palette_bitmap_bg_ptr.
- **set** `static void set(fixed shear, palette_bitmap_bg_ptr& bg)` (L2095): Sets the shear of the given palette_bitmap_bg_ptr.
### `bn::palette_bitmap_bg_shear_to_action` (class, bn_palette_bitmap_bg_actions.h:2108)

Modifies the shear of a palette_bitmap_bg_ptr until it has a given state.

`class palette_bitmap_bg_shear_to_action : public to_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_shear_manager>`

- **palette_bitmap_bg_shear_to_action** `palette_bitmap_bg_shear_to_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_shear) : to_value_template_action(bg, duration_updates, final_shear)` (L2120): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the shear of the given palette_bitmap_bg_ptr is equal to final_shear.; `final_shear` Shear when the action is updated duration_updates times.
- **palette_bitmap_bg_shear_to_action** `palette_bitmap_bg_shear_to_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_shear) : to_value_template_action(move(bg), duration_updates, final_shear)` (L2133): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the shear of the given palette_bitmap_bg_ptr is equal to final_shear.; `final_shear` Shear when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L2142): Returns the palette_bitmap_bg_ptr to modify.
- **final_shear** `fixed final_shear() const` (L2151): Returns the shear of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_shear_loop_action` (class, bn_palette_bitmap_bg_actions.h:2165)

Modifies the shear of a palette_bitmap_bg_ptr from a minimum to a maximum. When the shear is equal to the given final state, it goes back to its initial state and vice versa.

`class palette_bitmap_bg_shear_loop_action : public loop_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_shear_manager>`

- **palette_bitmap_bg_shear_loop_action** `palette_bitmap_bg_shear_loop_action(const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_shear) : loop_value_template_action(bg, duration_updates, final_shear)` (L2178): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the shear delta.; `final_shear` When the shear of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **palette_bitmap_bg_shear_loop_action** `palette_bitmap_bg_shear_loop_action(palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_shear) : loop_value_template_action(move(bg), duration_updates, final_shear)` (L2191): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the shear delta.; `final_shear` When the shear of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L2199): Returns the palette_bitmap_bg_ptr to modify.
- **final_shear** `fixed final_shear() const` (L2208): When the shear of the given palette_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::palette_bitmap_bg_shear_toggle_action` (class, bn_palette_bitmap_bg_actions.h:2221)

Changes the shear of a palette_bitmap_bg_ptr when the action is updated a given number of times.

`class palette_bitmap_bg_shear_toggle_action : public toggle_value_template_action<palette_bitmap_bg_ptr, fixed, palette_bitmap_bg_shear_manager>`

- **palette_bitmap_bg_shear_toggle_action** `palette_bitmap_bg_shear_toggle_action(const palette_bitmap_bg_ptr& bg, int duration_updates, fixed new_shear) : toggle_value_template_action(bg, duration_updates, new_shear)` (L2233): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the shear of the given palette_bitmap_bg_ptr.; `new_shear` New shear when the action is updated duration_updates times.
- **palette_bitmap_bg_shear_toggle_action** `palette_bitmap_bg_shear_toggle_action(palette_bitmap_bg_ptr&& bg, int duration_updates, fixed new_shear) : toggle_value_template_action(move(bg), duration_updates, new_shear)` (L2245): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the shear of the given palette_bitmap_bg_ptr.; `new_shear` New shear when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L2253): Returns the palette_bitmap_bg_ptr to modify.
- **new_shear** `fixed new_shear() const` (L2262): Returns the shear of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_horizontal_flip_manager` (class, bn_palette_bitmap_bg_actions.h:2277)

Manages if a palette_bitmap_bg_ptr is flipped in its horizontal axis or not.

`class palette_bitmap_bg_horizontal_flip_manager`

- **get** `static bool get(const palette_bitmap_bg_ptr& bg)` (L2284): Indicates if the given palette_bitmap_bg_ptr is flipped in the horizontal axis or not.
- **set** `static void set(bool horizontal_flip, palette_bitmap_bg_ptr& bg)` (L2292): Sets if the given palette_bitmap_bg_ptr must be flipped in the horizontal axis or not.
### `bn::palette_bitmap_bg_horizontal_flip_toggle_action` (class, bn_palette_bitmap_bg_actions.h:2306)

Toggles if a palette_bitmap_bg_ptr must be flipped in the horizontal axis or not when the action is updated a given number of times.

`class palette_bitmap_bg_horizontal_flip_toggle_action : public bool_toggle_value_template_action<palette_bitmap_bg_ptr, palette_bitmap_bg_horizontal_flip_manager>`

- **palette_bitmap_bg_horizontal_flip_toggle_action** `palette_bitmap_bg_horizontal_flip_toggle_action(const palette_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2317): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given palette_bitmap_bg_ptr must be flipped in the horizontal axis or not.
- **palette_bitmap_bg_horizontal_flip_toggle_action** `palette_bitmap_bg_horizontal_flip_toggle_action(palette_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2328): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given palette_bitmap_bg_ptr must be flipped in the horizontal axis or not.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L2336): Returns the palette_bitmap_bg_ptr to modify.
### `bn::palette_bitmap_bg_vertical_flip_manager` (class, bn_palette_bitmap_bg_actions.h:2351)

Manages if a palette_bitmap_bg_ptr is flipped in its vertical axis or not.

`class palette_bitmap_bg_vertical_flip_manager`

- **get** `static bool get(const palette_bitmap_bg_ptr& bg)` (L2358): Indicates if the given palette_bitmap_bg_ptr is flipped in the vertical axis or not.
- **set** `static void set(bool vertical_flip, palette_bitmap_bg_ptr& bg)` (L2366): Sets if the given palette_bitmap_bg_ptr must be flipped in the vertical axis or not.
### `bn::palette_bitmap_bg_vertical_flip_toggle_action` (class, bn_palette_bitmap_bg_actions.h:2380)

Toggles if a palette_bitmap_bg_ptr must be flipped in the vertical axis or not when the action is updated a given number of times.

`class palette_bitmap_bg_vertical_flip_toggle_action : public bool_toggle_value_template_action<palette_bitmap_bg_ptr, palette_bitmap_bg_vertical_flip_manager>`

- **palette_bitmap_bg_vertical_flip_toggle_action** `palette_bitmap_bg_vertical_flip_toggle_action(const palette_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2391): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given palette_bitmap_bg_ptr must be flipped in the vertical axis or not.
- **palette_bitmap_bg_vertical_flip_toggle_action** `palette_bitmap_bg_vertical_flip_toggle_action(palette_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2402): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given palette_bitmap_bg_ptr must be flipped in the vertical axis or not.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L2410): Returns the palette_bitmap_bg_ptr to modify.
### `bn::palette_bitmap_bg_pivot_position_manager` (class, bn_palette_bitmap_bg_actions.h:2425)

Manages the position of the pivot of a palette_bitmap_bg_ptr.

`class palette_bitmap_bg_pivot_position_manager`

- **get** `static const fixed_point& get(const palette_bitmap_bg_ptr& bg)` (L2432): Returns the position of the pivot of the given palette_bitmap_bg_ptr.
- **set** `static void set(const fixed_point& position, palette_bitmap_bg_ptr& bg)` (L2440): Sets the position of the pivot of the given palette_bitmap_bg_ptr.
### `bn::palette_bitmap_bg_move_pivot_by_action` (class, bn_palette_bitmap_bg_actions.h:2453)

Modifies the position of the pivot of a palette_bitmap_bg_ptr by a given delta.

`class palette_bitmap_bg_move_pivot_by_action : public by_value_template_action<palette_bitmap_bg_ptr, fixed_point, palette_bitmap_bg_pivot_position_manager>`

- **palette_bitmap_bg_move_pivot_by_action** `palette_bitmap_bg_move_pivot_by_action(const palette_bitmap_bg_ptr& bg, fixed delta_x, fixed delta_y) : by_value_template_action(bg, fixed_point(delta_x, delta_y))` (L2466): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `delta_x` How much to add to the horizontal position of the pivot of the given palette_bitmap_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the pivot of the given palette_bitmap_bg_ptr when the action is updated.
- **palette_bitmap_bg_move_pivot_by_action** `palette_bitmap_bg_move_pivot_by_action(palette_bitmap_bg_ptr&& bg, fixed delta_x, fixed delta_y) : by_value_template_action(move(bg), fixed_point(delta_x, delta_y))` (L2479): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `delta_x` How much to add to the horizontal position of the pivot of the given palette_bitmap_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the pivot of the given palette_bitmap_bg_ptr when the action is updated.
- **palette_bitmap_bg_move_pivot_by_action** `palette_bitmap_bg_move_pivot_by_action(const palette_bitmap_bg_ptr& bg, const fixed_point& delta_position) : by_value_template_action(bg, delta_position)` (L2490): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `delta_position` How much to add to the position of the pivot of the given palette_bitmap_bg_ptr when the action is updated.
- **palette_bitmap_bg_move_pivot_by_action** `palette_bitmap_bg_move_pivot_by_action(palette_bitmap_bg_ptr&& bg, const fixed_point& delta_position) : by_value_template_action(move(bg), delta_position)` (L2501): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `delta_position` How much to add to the position of the pivot of the given palette_bitmap_bg_ptr when the action is updated.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L2509): Returns the palette_bitmap_bg_ptr to modify.
- **delta_position** `const fixed_point& delta_position() const` (L2518): Returns how much to add to the position of the pivot of the given palette_bitmap_bg_ptr when the action is updated.
### `bn::palette_bitmap_bg_move_pivot_to_action` (class, bn_palette_bitmap_bg_actions.h:2531)

Modifies the position of the pivot of a palette_bitmap_bg_ptr until it has a given state.

`class palette_bitmap_bg_move_pivot_to_action : public to_value_template_action<palette_bitmap_bg_ptr, fixed_point, palette_bitmap_bg_pivot_position_manager>`

- **palette_bitmap_bg_move_pivot_to_action** `palette_bitmap_bg_move_pivot_to_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L2546): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given palette_bitmap_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the pivot of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the pivot of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.
- **palette_bitmap_bg_move_pivot_to_action** `palette_bitmap_bg_move_pivot_to_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L2562): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given palette_bitmap_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the pivot of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the pivot of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.
- **palette_bitmap_bg_move_pivot_to_action** `palette_bitmap_bg_move_pivot_to_action( const palette_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(bg, duration_updates, final_position)` (L2576): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given palette_bitmap_bg_ptr is equal to final_position.; `final_position` Position of the pivot of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.
- **palette_bitmap_bg_move_pivot_to_action** `palette_bitmap_bg_move_pivot_to_action( palette_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(move(bg), duration_updates, final_position)` (L2590): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given palette_bitmap_bg_ptr is equal to final_position.; `final_position` Position of the pivot of the given palette_bitmap_bg_ptr when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L2599): Returns the palette_bitmap_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L2608): Returns the position of the pivot of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_move_pivot_loop_action` (class, bn_palette_bitmap_bg_actions.h:2622)

Modifies the position of the pivot of a palette_bitmap_bg_ptr from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class palette_bitmap_bg_move_pivot_loop_action : public loop_value_template_action<palette_bitmap_bg_ptr, fixed_point, palette_bitmap_bg_pivot_position_manager>`

- **palette_bitmap_bg_move_pivot_loop_action** `palette_bitmap_bg_move_pivot_loop_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L2637): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the pivot of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the pivot of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **palette_bitmap_bg_move_pivot_loop_action** `palette_bitmap_bg_move_pivot_loop_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L2653): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the pivot of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the pivot of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **palette_bitmap_bg_move_pivot_loop_action** `palette_bitmap_bg_move_pivot_loop_action( const palette_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(bg, duration_updates, final_position)` (L2667): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the pivot of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **palette_bitmap_bg_move_pivot_loop_action** `palette_bitmap_bg_move_pivot_loop_action( palette_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(move(bg), duration_updates, final_position)` (L2681): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the pivot of the given palette_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L2690): Returns the palette_bitmap_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L2699): When the position of the pivot of the given palette_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::palette_bitmap_bg_move_pivot_toggle_action` (class, bn_palette_bitmap_bg_actions.h:2713)

Changes the position of the pivot of a palette_bitmap_bg_ptr when the action is updated a given number of times.

`class palette_bitmap_bg_move_pivot_toggle_action : public toggle_value_template_action<palette_bitmap_bg_ptr, fixed_point, palette_bitmap_bg_pivot_position_manager>`

- **palette_bitmap_bg_move_pivot_toggle_action** `palette_bitmap_bg_move_pivot_toggle_action( const palette_bitmap_bg_ptr& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(bg, duration_updates, fixed_point(new_x, new_y))` (L2726): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given palette_bitmap_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **palette_bitmap_bg_move_pivot_toggle_action** `palette_bitmap_bg_move_pivot_toggle_action( palette_bitmap_bg_ptr&& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(move(bg), duration_updates, fixed_point(new_x, new_y))` (L2740): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given palette_bitmap_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **palette_bitmap_bg_move_pivot_toggle_action** `palette_bitmap_bg_move_pivot_toggle_action( const palette_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(bg, duration_updates, new_position)` (L2753): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given palette_bitmap_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **palette_bitmap_bg_move_pivot_toggle_action** `palette_bitmap_bg_move_pivot_toggle_action( palette_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(move(bg), duration_updates, new_position)` (L2766): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given palette_bitmap_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L2775): Returns the palette_bitmap_bg_ptr to modify.
- **new_position** `const fixed_point& new_position() const` (L2784): Returns the position of the pivot of the given palette_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::palette_bitmap_bg_mosaic_manager` (class, bn_palette_bitmap_bg_actions.h:2799)

Manages if the mosaic effect must be applied to a palette_bitmap_bg_ptr or not.

`class palette_bitmap_bg_mosaic_manager`

- **get** `static bool get(const palette_bitmap_bg_ptr& bg)` (L2806): Indicates if the mosaic effect is applied to the given palette_bitmap_bg_ptr or not.
- **set** `static void set(bool mosaic_enabled, palette_bitmap_bg_ptr& bg)` (L2814): Sets if the mosaic effect must be applied to the given palette_bitmap_bg_ptr or not.
### `bn::palette_bitmap_bg_mosaic_toggle_action` (class, bn_palette_bitmap_bg_actions.h:2828)

Toggles if the mosaic effect must be applied to a palette_bitmap_bg_ptr or not when the action is updated a given number of times.

`class palette_bitmap_bg_mosaic_toggle_action : public bool_toggle_value_template_action<palette_bitmap_bg_ptr, palette_bitmap_bg_mosaic_manager>`

- **palette_bitmap_bg_mosaic_toggle_action** `palette_bitmap_bg_mosaic_toggle_action(const palette_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2839): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the mosaic effect must be applied or not.
- **palette_bitmap_bg_mosaic_toggle_action** `palette_bitmap_bg_mosaic_toggle_action(palette_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2850): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the mosaic effect must be applied or not.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L2858): Returns the palette_bitmap_bg_ptr to modify.
### `bn::palette_bitmap_bg_blending_manager` (class, bn_palette_bitmap_bg_actions.h:2873)

Manages if blending must be applied to a palette_bitmap_bg_ptr or not.

`class palette_bitmap_bg_blending_manager`

- **get** `static bool get(const palette_bitmap_bg_ptr& bg)` (L2880): Indicates if blending is applied to the given palette_bitmap_bg_ptr or not.
- **set** `static void set(bool blending_enabled, palette_bitmap_bg_ptr& bg)` (L2888): Sets if blending must be applied to the given palette_bitmap_bg_ptr or not.
### `bn::palette_bitmap_bg_blending_toggle_action` (class, bn_palette_bitmap_bg_actions.h:2902)

Toggles if blending must be applied to a palette_bitmap_bg_ptr or not when the action is updated a given number of times.

`class palette_bitmap_bg_blending_toggle_action : public bool_toggle_value_template_action<palette_bitmap_bg_ptr, palette_bitmap_bg_blending_manager>`

- **palette_bitmap_bg_blending_toggle_action** `palette_bitmap_bg_blending_toggle_action(const palette_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2913): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if blending must be applied or not.
- **palette_bitmap_bg_blending_toggle_action** `palette_bitmap_bg_blending_toggle_action(palette_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2924): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if blending must be applied or not.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L2932): Returns the palette_bitmap_bg_ptr to modify.
### `bn::palette_bitmap_bg_blending_top_manager` (class, bn_palette_bitmap_bg_actions.h:2945)

Manages if a palette_bitmap_bg_ptr must be part of the blending top layer or not.

`class palette_bitmap_bg_blending_top_manager`

- **get** `static bool get(const palette_bitmap_bg_ptr& bg)` (L2952): Indicates if the given palette_bitmap_bg_ptr is part of the blending top layer or not.
- **set** `static void set(bool blending_top_enabled, palette_bitmap_bg_ptr& bg)` (L2960): Sets if the given palette_bitmap_bg_ptr must be part of the blending top layer or not.
### `bn::palette_bitmap_bg_blending_top_toggle_action` (class, bn_palette_bitmap_bg_actions.h:2974)

Toggles if a palette_bitmap_bg_ptr is part of the blending top layer or not when the action is updated a given number of times.

`class palette_bitmap_bg_blending_top_toggle_action : public bool_toggle_value_template_action<palette_bitmap_bg_ptr, palette_bitmap_bg_blending_top_manager>`

- **palette_bitmap_bg_blending_top_toggle_action** `palette_bitmap_bg_blending_top_toggle_action(const palette_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2985): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given palette_bitmap_bg_ptr is part of the blending top layer or not.
- **palette_bitmap_bg_blending_top_toggle_action** `palette_bitmap_bg_blending_top_toggle_action(palette_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2996): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given palette_bitmap_bg_ptr is part of the blending top layer or not.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L3004): Returns the palette_bitmap_bg_ptr to modify.
### `bn::palette_bitmap_bg_blending_bottom_manager` (class, bn_palette_bitmap_bg_actions.h:3017)

Manages if a palette_bitmap_bg_ptr must be part of the blending bottom layer or not.

`class palette_bitmap_bg_blending_bottom_manager`

- **get** `static bool get(const palette_bitmap_bg_ptr& bg)` (L3024): Indicates if the given palette_bitmap_bg_ptr is part of the blending bottom layer or not.
- **set** `static void set(bool blending_bottom_enabled, palette_bitmap_bg_ptr& bg)` (L3032): Sets if the given palette_bitmap_bg_ptr must be part of the blending bottom layer or not.
### `bn::palette_bitmap_bg_blending_bottom_toggle_action` (class, bn_palette_bitmap_bg_actions.h:3046)

Toggles if a palette_bitmap_bg_ptr is part of the blending bottom layer or not when the action is updated a given number of times.

`class palette_bitmap_bg_blending_bottom_toggle_action : public bool_toggle_value_template_action<palette_bitmap_bg_ptr, palette_bitmap_bg_blending_bottom_manager>`

- **palette_bitmap_bg_blending_bottom_toggle_action** `palette_bitmap_bg_blending_bottom_toggle_action(const palette_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L3057): Constructor.
  `bg` palette_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given palette_bitmap_bg_ptr is part of the blending bottom layer or not.
- **palette_bitmap_bg_blending_bottom_toggle_action** `palette_bitmap_bg_blending_bottom_toggle_action(palette_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L3068): Constructor.
  `bg` palette_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given palette_bitmap_bg_ptr is part of the blending bottom layer or not.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L3076): Returns the palette_bitmap_bg_ptr to modify.

## bn_rect_window_actions.h

### `bn::rect_window_top_manager` (class, bn_rect_window_actions.h:31)

Manages the position of the top boundary of a rect_window.

`class rect_window_top_manager`

- **get** `static fixed get(rect_window window)` (L38): Returns the position of the top boundary the given rect_window.
- **set** `static void set(fixed top, rect_window window)` (L46): Sets the position of the top boundary the given rect_window.
### `bn::rect_window_move_top_by_action` (class, bn_rect_window_actions.h:59)

Modifies the position of the top boundary of a rect_window by a given delta.

`class rect_window_move_top_by_action : public by_value_template_action<rect_window, fixed, rect_window_top_manager>`

- **rect_window_move_top_by_action** `rect_window_move_top_by_action(rect_window window, fixed delta_top) : by_value_template_action(window, delta_top)` (L69): Constructor.
  `window` rect_window to modify.; `delta_top` How much to add to the position of the top boundary of the given rect_window when the action is updated.
- **window** `rect_window window() const` (L77): Returns the rect_window to modify.
- **delta_top** `fixed delta_top() const` (L86): Returns how much to add to the position of the top boundary of the given rect_window when the action is updated.
### `bn::rect_window_move_top_to_action` (class, bn_rect_window_actions.h:99)

Modifies the position of the top boundary of a rect_window until it has a given state.

`class rect_window_move_top_to_action : public to_value_template_action<rect_window, fixed, rect_window_top_manager>`

- **rect_window_move_top_to_action** `rect_window_move_top_to_action(rect_window window, int duration_updates, fixed final_top) : to_value_template_action(window, duration_updates, final_top)` (L111): Constructor.
  `window` rect_window to modify.; `duration_updates` Number of times that the action must be updated until the position of the top boundary of the given rect_window is equal to final_top.; `final_top` Position of the top boundary of the given rect_window when the action is updated duration_updates times.
- **window** `rect_window window() const` (L119): Returns the rect_window to modify.
- **final_top** `fixed final_top() const` (L128): Returns the position of the top boundary of the given rect_window when the action is updated the given number of times.
### `bn::rect_window_move_top_loop_action` (class, bn_rect_window_actions.h:142)

Modifies the position of the top boundary of a rect_window from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class rect_window_move_top_loop_action : public loop_value_template_action<rect_window, fixed, rect_window_top_manager>`

- **rect_window_move_top_loop_action** `rect_window_move_top_loop_action(rect_window window, int duration_updates, fixed final_top) : loop_value_template_action(window, duration_updates, final_top)` (L155): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_top` When the position of the top boundary of the given rect_window is equal to this parameter, it goes back to its initial state and vice versa.
- **window** `rect_window window() const` (L163): Returns the rect_window to modify.
- **final_top** `fixed final_top() const` (L172): When the position of the top boundary of the given rect_window is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::rect_window_move_top_toggle_action` (class, bn_rect_window_actions.h:186)

Changes the position of the top boundary of a rect_window when the action is updated a given number of times.

`class rect_window_move_top_toggle_action : public toggle_value_template_action<rect_window, fixed, rect_window_top_manager>`

- **rect_window_move_top_toggle_action** `rect_window_move_top_toggle_action(rect_window window, int duration_updates, fixed new_top) : toggle_value_template_action(window, duration_updates, new_top)` (L199): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated to change the position of the top boundary of the given rect_window.; `new_top` New position of the top boundary of the given rect_window when the action is updated duration_updates times.
- **window** `rect_window window() const` (L207): Returns the rect_window to modify.
- **new_top** `fixed new_top() const` (L216): Returns the position of the top boundary of the given rect_window when the action is updated the given number of times.
### `bn::rect_window_left_manager` (class, bn_rect_window_actions.h:231)

Manages the position of the left boundary of a rect_window.

`class rect_window_left_manager`

- **get** `static fixed get(rect_window window)` (L238): Returns the position of the left boundary the given rect_window.
- **set** `static void set(fixed left, rect_window window)` (L246): Sets the position of the left boundary the given rect_window.
### `bn::rect_window_move_left_by_action` (class, bn_rect_window_actions.h:259)

Modifies the position of the left boundary of a rect_window by a given delta.

`class rect_window_move_left_by_action : public by_value_template_action<rect_window, fixed, rect_window_left_manager>`

- **rect_window_move_left_by_action** `rect_window_move_left_by_action(rect_window window, fixed delta_left) : by_value_template_action(window, delta_left)` (L269): Constructor.
  `window` rect_window to modify.; `delta_left` How much to add to the position of the left boundary of the given rect_window when the action is updated.
- **window** `rect_window window() const` (L277): Returns the rect_window to modify.
- **delta_left** `fixed delta_left() const` (L286): Returns how much to add to the position of the left boundary of the given rect_window when the action is updated.
### `bn::rect_window_move_left_to_action` (class, bn_rect_window_actions.h:299)

Modifies the position of the left boundary of a rect_window until it has a given state.

`class rect_window_move_left_to_action : public to_value_template_action<rect_window, fixed, rect_window_left_manager>`

- **rect_window_move_left_to_action** `rect_window_move_left_to_action(rect_window window, int duration_updates, fixed final_left) : to_value_template_action(window, duration_updates, final_left)` (L311): Constructor.
  `window` rect_window to modify.; `duration_updates` Number of times that the action must be updated until the position of the left boundary of the given rect_window is equal to final_left.; `final_left` Position of the left boundary of the given rect_window when the action is updated duration_updates times.
- **window** `rect_window window() const` (L319): Returns the rect_window to modify.
- **final_left** `fixed final_left() const` (L328): Returns the position of the left boundary of the given rect_window when the action is updated the given number of times.
### `bn::rect_window_move_left_loop_action` (class, bn_rect_window_actions.h:342)

Modifies the position of the left boundary of a rect_window from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class rect_window_move_left_loop_action : public loop_value_template_action<rect_window, fixed, rect_window_left_manager>`

- **rect_window_move_left_loop_action** `rect_window_move_left_loop_action(rect_window window, int duration_updates, fixed final_left) : loop_value_template_action(window, duration_updates, final_left)` (L355): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_left` When the position of the left boundary of the given rect_window is equal to this parameter, it goes back to its initial state and vice versa.
- **window** `rect_window window() const` (L363): Returns the rect_window to modify.
- **final_left** `fixed final_left() const` (L372): When the position of the left boundary of the given rect_window is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::rect_window_move_left_toggle_action` (class, bn_rect_window_actions.h:386)

Changes the position of the left boundary of a rect_window when the action is updated a given number of times.

`class rect_window_move_left_toggle_action : public toggle_value_template_action<rect_window, fixed, rect_window_left_manager>`

- **rect_window_move_left_toggle_action** `rect_window_move_left_toggle_action(rect_window window, int duration_updates, fixed new_left) : toggle_value_template_action(window, duration_updates, new_left)` (L399): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated to change the position of the left boundary of the given rect_window.; `new_left` New position of the left boundary of the given rect_window when the action is updated duration_updates times.
- **window** `rect_window window() const` (L407): Returns the rect_window to modify.
- **new_left** `fixed new_left() const` (L416): Returns the position of the left boundary of the given rect_window when the action is updated the given number of times.
### `bn::rect_window_bottom_manager` (class, bn_rect_window_actions.h:431)

Manages the position of the bottom boundary of a rect_window.

`class rect_window_bottom_manager`

- **get** `static fixed get(rect_window window)` (L438): Returns the position of the bottom boundary the given rect_window.
- **set** `static void set(fixed bottom, rect_window window)` (L446): Sets the position of the bottom boundary the given rect_window.
### `bn::rect_window_move_bottom_by_action` (class, bn_rect_window_actions.h:459)

Modifies the position of the bottom boundary of a rect_window by a given delta.

`class rect_window_move_bottom_by_action : public by_value_template_action<rect_window, fixed, rect_window_bottom_manager>`

- **rect_window_move_bottom_by_action** `rect_window_move_bottom_by_action(rect_window window, fixed delta_bottom) : by_value_template_action(window, delta_bottom)` (L470): Constructor.
  `window` rect_window to modify.; `delta_bottom` How much to add to the position of the bottom boundary of the given rect_window when the action is updated.
- **window** `rect_window window() const` (L478): Returns the rect_window to modify.
- **delta_bottom** `fixed delta_bottom() const` (L487): Returns how much to add to the position of the bottom boundary of the given rect_window when the action is updated.
### `bn::rect_window_move_bottom_to_action` (class, bn_rect_window_actions.h:500)

Modifies the position of the bottom boundary of a rect_window until it has a given state.

`class rect_window_move_bottom_to_action : public to_value_template_action<rect_window, fixed, rect_window_bottom_manager>`

- **rect_window_move_bottom_to_action** `rect_window_move_bottom_to_action(rect_window window, int duration_updates, fixed final_bottom) : to_value_template_action(window, duration_updates, final_bottom)` (L513): Constructor.
  `window` rect_window to modify.; `duration_updates` Number of times that the action must be updated until the position of the bottom boundary of the given rect_window is equal to final_bottom.; `final_bottom` Position of the bottom boundary of the given rect_window when the action is updated duration_updates times.
- **window** `rect_window window() const` (L521): Returns the rect_window to modify.
- **final_bottom** `fixed final_bottom() const` (L530): Returns the position of the bottom boundary of the given rect_window when the action is updated the given number of times.
### `bn::rect_window_move_bottom_loop_action` (class, bn_rect_window_actions.h:544)

Modifies the position of the bottom boundary of a rect_window from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class rect_window_move_bottom_loop_action : public loop_value_template_action<rect_window, fixed, rect_window_bottom_manager>`

- **rect_window_move_bottom_loop_action** `rect_window_move_bottom_loop_action(rect_window window, int duration_updates, fixed final_bottom) : loop_value_template_action(window, duration_updates, final_bottom)` (L557): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_bottom` When the position of the bottom boundary of the given rect_window is equal to this parameter, it goes back to its initial state and vice versa.
- **window** `rect_window window() const` (L565): Returns the rect_window to modify.
- **final_bottom** `fixed final_bottom() const` (L574): When the position of the bottom boundary of the given rect_window is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::rect_window_move_bottom_toggle_action` (class, bn_rect_window_actions.h:588)

Changes the position of the bottom boundary of a rect_window when the action is updated a given number of times.

`class rect_window_move_bottom_toggle_action : public toggle_value_template_action<rect_window, fixed, rect_window_bottom_manager>`

- **rect_window_move_bottom_toggle_action** `rect_window_move_bottom_toggle_action(rect_window window, int duration_updates, fixed new_bottom) : toggle_value_template_action(window, duration_updates, new_bottom)` (L601): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated to change the position of the bottom boundary of the given rect_window.; `new_bottom` New position of the bottom boundary of the given rect_window when the action is updated duration_updates times.
- **window** `rect_window window() const` (L609): Returns the rect_window to modify.
- **new_bottom** `fixed new_bottom() const` (L618): Returns the position of the bottom boundary of the given rect_window when the action is updated the given number of times.
### `bn::rect_window_right_manager` (class, bn_rect_window_actions.h:633)

Manages the position of the right boundary of a rect_window.

`class rect_window_right_manager`

- **get** `static fixed get(rect_window window)` (L640): Returns the position of the right boundary the given rect_window.
- **set** `static void set(fixed right, rect_window window)` (L648): Sets the position of the right boundary the given rect_window.
### `bn::rect_window_move_right_by_action` (class, bn_rect_window_actions.h:661)

Modifies the position of the right boundary of a rect_window by a given delta.

`class rect_window_move_right_by_action : public by_value_template_action<rect_window, fixed, rect_window_right_manager>`

- **rect_window_move_right_by_action** `rect_window_move_right_by_action(rect_window window, fixed delta_right) : by_value_template_action(window, delta_right)` (L672): Constructor.
  `window` rect_window to modify.; `delta_right` How much to add to the position of the right boundary of the given rect_window when the action is updated.
- **window** `rect_window window() const` (L680): Returns the rect_window to modify.
- **delta_right** `fixed delta_right() const` (L689): Returns how much to add to the position of the right boundary of the given rect_window when the action is updated.
### `bn::rect_window_move_right_to_action` (class, bn_rect_window_actions.h:702)

Modifies the position of the right boundary of a rect_window until it has a given state.

`class rect_window_move_right_to_action : public to_value_template_action<rect_window, fixed, rect_window_right_manager>`

- **rect_window_move_right_to_action** `rect_window_move_right_to_action(rect_window window, int duration_updates, fixed final_right) : to_value_template_action(window, duration_updates, final_right)` (L715): Constructor.
  `window` rect_window to modify.; `duration_updates` Number of times that the action must be updated until the position of the right boundary of the given rect_window is equal to final_right.; `final_right` Position of the right boundary of the given rect_window when the action is updated duration_updates times.
- **window** `rect_window window() const` (L723): Returns the rect_window to modify.
- **final_right** `fixed final_right() const` (L732): Returns the position of the right boundary of the given rect_window when the action is updated the given number of times.
### `bn::rect_window_move_right_loop_action` (class, bn_rect_window_actions.h:746)

Modifies the position of the right boundary of a rect_window from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class rect_window_move_right_loop_action : public loop_value_template_action<rect_window, fixed, rect_window_right_manager>`

- **rect_window_move_right_loop_action** `rect_window_move_right_loop_action(rect_window window, int duration_updates, fixed final_right) : loop_value_template_action(window, duration_updates, final_right)` (L759): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_right` When the position of the right boundary of the given rect_window is equal to this parameter, it goes back to its initial state and vice versa.
- **window** `rect_window window() const` (L767): Returns the rect_window to modify.
- **final_right** `fixed final_right() const` (L776): When the position of the right boundary of the given rect_window is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::rect_window_move_right_toggle_action` (class, bn_rect_window_actions.h:790)

Changes the position of the right boundary of a rect_window when the action is updated a given number of times.

`class rect_window_move_right_toggle_action : public toggle_value_template_action<rect_window, fixed, rect_window_right_manager>`

- **rect_window_move_right_toggle_action** `rect_window_move_right_toggle_action(rect_window window, int duration_updates, fixed new_right) : toggle_value_template_action(window, duration_updates, new_right)` (L803): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated to change the position of the right boundary of the given rect_window.; `new_right` New position of the right boundary of the given rect_window when the action is updated duration_updates times.
- **window** `rect_window window() const` (L811): Returns the rect_window to modify.
- **new_right** `fixed new_right() const` (L820): Returns the position of the right boundary of the given rect_window when the action is updated the given number of times.
### `bn::rect_window_top_left_manager` (class, bn_rect_window_actions.h:835)

Manages the position of the top-left corner of a rect_window.

`class rect_window_top_left_manager`

- **get** `static const fixed_point& get(rect_window window)` (L842): Returns the position of the top-left corner the given rect_window.
- **set** `static void set(const fixed_point& top_left, rect_window window)` (L850): Sets the position of the top-left corner the given rect_window.
### `bn::rect_window_move_top_left_by_action` (class, bn_rect_window_actions.h:863)

Modifies the position of the top-left corner of a rect_window by a given delta.

`class rect_window_move_top_left_by_action : public by_value_template_action<rect_window, fixed_point, rect_window_top_left_manager>`

- **rect_window_move_top_left_by_action** `rect_window_move_top_left_by_action(rect_window window, fixed delta_top, fixed delta_left) : by_value_template_action(window, fixed_point(delta_left, delta_top))` (L876): Constructor.
  `window` rect_window to modify.; `delta_top` How much to add to the position of the top boundary of the given rect_window when the action is updated.; `delta_left` How much to add to the position of the left boundary of the given rect_window when the action is updated.
- **rect_window_move_top_left_by_action** `rect_window_move_top_left_by_action(rect_window window, const fixed_point& delta_top_left) : by_value_template_action(window, delta_top_left)` (L887): Constructor.
  `window` rect_window to modify.; `delta_top_left` How much to add to the position of the top-left corner of the given rect_window when the action is updated.
- **window** `rect_window window() const` (L895): Returns the rect_window to modify.
- **delta_top_left** `const fixed_point& delta_top_left() const` (L904): Returns how much to add to the position of the top-left corner of the given rect_window when the action is updated.
### `bn::rect_window_move_top_left_to_action` (class, bn_rect_window_actions.h:917)

Modifies the position of the top-left corner of a rect_window until it has a given state.

`class rect_window_move_top_left_to_action : public to_value_template_action<rect_window, fixed_point, rect_window_top_left_manager>`

- **rect_window_move_top_left_to_action** `rect_window_move_top_left_to_action(rect_window window, int duration_updates, fixed final_top, fixed final_left) : to_value_template_action(window, duration_updates, fixed_point(final_left, final_top))` (L932): Constructor.
  `window` rect_window to modify.; `duration_updates` Number of times that the action must be updated until the position of the top-left corner of the given rect_window is equal to (final_top, final_left).; `final_top` Position of the top boundary of the given rect_window when the action is updated duration_updates times.; `final_left` Position of the left boundary of the given rect_window when the action is updated duration_updates times.
- **rect_window_move_top_left_to_action** `rect_window_move_top_left_to_action(rect_window window, int duration_updates, const fixed_point& final_top_left) : to_value_template_action(window, duration_updates, final_top_left)` (L945): Constructor.
  `window` rect_window to modify.; `duration_updates` Number of times that the action must be updated until the position of the top-left corner of the given rect_window is equal to final_top_left.; `final_top_left` Position of the top-left corner of the given rect_window when the action is updated duration_updates times.
- **window** `rect_window window() const` (L953): Returns the rect_window to modify.
- **final_top_left** `const fixed_point& final_top_left() const` (L962): Returns the position of the top-left corner of the given rect_window when the action is updated the given number of times.
### `bn::rect_window_move_top_left_loop_action` (class, bn_rect_window_actions.h:976)

Modifies the position of the top-left corner of a rect_window from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class rect_window_move_top_left_loop_action : public loop_value_template_action<rect_window, fixed_point, rect_window_top_left_manager>`

- **rect_window_move_top_left_loop_action** `rect_window_move_top_left_loop_action(rect_window window, int duration_updates, fixed final_top, fixed final_left) : loop_value_template_action(window, duration_updates, fixed_point(final_left, final_top))` (L991): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_top` When the position of the top boundary of the given rect_window is equal to this parameter, it goes back to its initial state and vice versa.; `final_left` When the position of the left boundary of the given rect_window is equal to this parameter, it goes back to its initial state and vice versa.
- **rect_window_move_top_left_loop_action** `rect_window_move_top_left_loop_action(rect_window window, int duration_updates, const fixed_point& final_top_left) : loop_value_template_action(window, duration_updates, final_top_left)` (L1005): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_top_left` When the position of the top-left corner of the given rect_window is equal to this parameter, it goes back to its initial state and vice versa.
- **window** `rect_window window() const` (L1014): Returns the rect_window to modify.
- **final_top_left** `const fixed_point& final_top_left() const` (L1023): When the position of the top-left corner of the given rect_window is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::rect_window_move_top_left_toggle_action` (class, bn_rect_window_actions.h:1037)

Changes the position of the top-left corner of a rect_window when the action is updated a given number of times.

`class rect_window_move_top_left_toggle_action : public toggle_value_template_action<rect_window, fixed_point, rect_window_top_left_manager>`

- **rect_window_move_top_left_toggle_action** `rect_window_move_top_left_toggle_action(rect_window window, int duration_updates, fixed new_top, fixed new_left) : toggle_value_template_action(window, duration_updates, fixed_point(new_left, new_top))` (L1052): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated to change the position of the top-left corner of the given rect_window.; `new_top` New position of the top boundary of the given rect_window when the action is updated duration_updates times.; `new_left` New position of the left boundary of the given rect_window when the action is updated duration_updates times.
- **rect_window_move_top_left_toggle_action** `rect_window_move_top_left_toggle_action(rect_window window, int duration_updates, const fixed_point& new_top_left) : toggle_value_template_action(window, duration_updates, new_top_left)` (L1065): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated to change the position of the top-left corner of the given rect_window.; `new_top_left` New position of the top-left corner of the given rect_window when the action is updated duration_updates times.
- **window** `rect_window window() const` (L1074): Returns the rect_window to modify.
- **new_top_left** `const fixed_point& new_top_left() const` (L1083): Returns the position of the top-left corner of the given rect_window when the action is updated the given number of times.
### `bn::rect_window_bottom_right_manager` (class, bn_rect_window_actions.h:1098)

Manages the position of the bottom-right corner of a rect_window.

`class rect_window_bottom_right_manager`

- **get** `static const fixed_point& get(rect_window window)` (L1105): Returns the position of the bottom-right corner the given rect_window.
- **set** `static void set(const fixed_point& bottom_right, rect_window window)` (L1113): Sets the position of the bottom-right corner the given rect_window.
### `bn::rect_window_move_bottom_right_by_action` (class, bn_rect_window_actions.h:1126)

Modifies the position of the bottom-right corner of a rect_window by a given delta.

`class rect_window_move_bottom_right_by_action : public by_value_template_action<rect_window, fixed_point, rect_window_bottom_right_manager>`

- **rect_window_move_bottom_right_by_action** `rect_window_move_bottom_right_by_action(rect_window window, fixed delta_bottom, fixed delta_right) : by_value_template_action(window, fixed_point(delta_right, delta_bottom))` (L1139): Constructor.
  `window` rect_window to modify.; `delta_bottom` How much to add to the position of the bottom boundary of the given rect_window when the action is updated.; `delta_right` How much to add to the position of the right boundary of the given rect_window when the action is updated.
- **rect_window_move_bottom_right_by_action** `rect_window_move_bottom_right_by_action(rect_window window, const fixed_point& delta_bottom_right) : by_value_template_action(window, delta_bottom_right)` (L1150): Constructor.
  `window` rect_window to modify.; `delta_bottom_right` How much to add to the position of the bottom-right corner of the given rect_window when the action is updated.
- **window** `rect_window window() const` (L1158): Returns the rect_window to modify.
- **delta_bottom_right** `const fixed_point& delta_bottom_right() const` (L1167): Returns how much to add to the position of the bottom-right corner of the given rect_window when the action is updated.
### `bn::rect_window_move_bottom_right_to_action` (class, bn_rect_window_actions.h:1180)

Modifies the position of the bottom-right corner of a rect_window until it has a given state.

`class rect_window_move_bottom_right_to_action : public to_value_template_action<rect_window, fixed_point, rect_window_bottom_right_manager>`

- **rect_window_move_bottom_right_to_action** `rect_window_move_bottom_right_to_action(rect_window window, int duration_updates, fixed final_bottom, fixed final_right) : to_value_template_action(window, duration_updates, fixed_point(final_right, final_bottom))` (L1195): Constructor.
  `window` rect_window to modify.; `duration_updates` Number of times that the action must be updated until the position of the bottom-right corner of the given rect_window is equal to (final_bottom, final_right).; `final_bottom` Position of the bottom boundary of the given rect_window when the action is updated duration_updates times.; `final_right` Position of the right boundary of the given rect_window when the action is updated duration_updates times.
- **rect_window_move_bottom_right_to_action** `rect_window_move_bottom_right_to_action(rect_window window, int duration_updates, const fixed_point& final_bottom_right) : to_value_template_action(window, duration_updates, final_bottom_right)` (L1209): Constructor.
  `window` rect_window to modify.; `duration_updates` Number of times that the action must be updated until the position of the bottom-right corner of the given rect_window is equal to final_bottom_right.; `final_bottom_right` Position of the bottom-right corner of the given rect_window when the action is updated duration_updates times.
- **window** `rect_window window() const` (L1218): Returns the rect_window to modify.
- **final_bottom_right** `const fixed_point& final_bottom_right() const` (L1227): Returns the position of the bottom-right corner of the given rect_window when the action is updated the given number of times.
### `bn::rect_window_move_bottom_right_loop_action` (class, bn_rect_window_actions.h:1241)

Modifies the position of the bottom-right corner of a rect_window from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class rect_window_move_bottom_right_loop_action : public loop_value_template_action<rect_window, fixed_point, rect_window_bottom_right_manager>`

- **rect_window_move_bottom_right_loop_action** `rect_window_move_bottom_right_loop_action(rect_window window, int duration_updates, fixed final_bottom, fixed final_right) : loop_value_template_action(window, duration_updates, fixed_point(final_right, final_bottom))` (L1256): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_bottom` When the position of the bottom boundary of the given rect_window is equal to this parameter, it goes back to its initial state and vice versa.; `final_right` When the position of the right boundary of the given rect_window is equal to this parameter, it goes back to its initial state and vice versa.
- **rect_window_move_bottom_right_loop_action** `rect_window_move_bottom_right_loop_action(rect_window window, int duration_updates, const fixed_point& final_bottom_right) : loop_value_template_action(window, duration_updates, final_bottom_right)` (L1270): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_bottom_right` When the position of the bottom-right corner of the given rect_window is equal to this parameter, it goes back to its initial state and vice versa.
- **window** `rect_window window() const` (L1279): Returns the rect_window to modify.
- **final_bottom_right** `const fixed_point& final_bottom_right() const` (L1288): When the position of the bottom-right corner of the given rect_window is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::rect_window_move_bottom_right_toggle_action` (class, bn_rect_window_actions.h:1302)

Changes the position of the bottom-right corner of a rect_window when the action is updated a given number of times.

`class rect_window_move_bottom_right_toggle_action : public toggle_value_template_action<rect_window, fixed_point, rect_window_bottom_right_manager>`

- **rect_window_move_bottom_right_toggle_action** `rect_window_move_bottom_right_toggle_action(rect_window window, int duration_updates, fixed new_bottom, fixed new_right) : toggle_value_template_action(window, duration_updates, fixed_point(new_right, new_bottom))` (L1317): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated to change the position of the bottom-right corner of the given rect_window.; `new_bottom` New position of the bottom boundary of the given rect_window when the action is updated duration_updates times.; `new_right` New position of the right boundary of the given rect_window when the action is updated duration_updates times.
- **rect_window_move_bottom_right_toggle_action** `rect_window_move_bottom_right_toggle_action(rect_window window, int duration_updates, const fixed_point& new_bottom_right) : toggle_value_template_action(window, duration_updates, new_bottom_right)` (L1331): Constructor.
  `window` rect_window to modify.; `duration_updates` How many times the action has to be updated to change the position of the bottom-right corner of the given rect_window.; `new_bottom_right` New position of the bottom-right corner of the given rect_window when the action is updated duration_updates times.
- **window** `rect_window window() const` (L1340): Returns the rect_window to modify.
- **new_bottom_right** `const fixed_point& new_bottom_right() const` (L1349): Returns the position of the bottom-right corner of the given rect_window when the action is updated the given number of times.

## bn_regular_bg_actions.h

### `bn::regular_bg_visible_manager` (class, bn_regular_bg_actions.h:31)

Manages if a regular_bg_ptr must be committed to the GBA or not.

`class regular_bg_visible_manager`

- **get** `static bool get(const regular_bg_ptr& bg)` (L38): Indicates if the given regular_bg_ptr is committed to the GBA or not.
- **set** `static void set(bool visible, regular_bg_ptr& bg)` (L46): Sets if the given regular_bg_ptr must be committed to the GBA or not.
### `bn::regular_bg_visible_toggle_action` (class, bn_regular_bg_actions.h:60)

Toggles if a regular_bg_ptr must be committed to the GBA or not when the action is updated a given number of times.

`class regular_bg_visible_toggle_action : public bool_toggle_value_template_action<regular_bg_ptr, regular_bg_visible_manager>`

- **regular_bg_visible_toggle_action** `regular_bg_visible_toggle_action(const regular_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L71): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given regular_bg_ptr must be committed to the GBA or not.
- **regular_bg_visible_toggle_action** `regular_bg_visible_toggle_action(regular_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L82): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given regular_bg_ptr must be committed to the GBA or not.
- **bg** `const regular_bg_ptr& bg() const` (L90): Returns the regular_bg_ptr to modify.
### `bn::regular_bg_position_manager` (class, bn_regular_bg_actions.h:105)

Manages the position of a regular_bg_ptr.

`class regular_bg_position_manager`

- **get** `static const fixed_point& get(const regular_bg_ptr& bg)` (L112): Returns the position of the given regular_bg_ptr.
- **set** `static void set(const fixed_point& position, regular_bg_ptr& bg)` (L120): Sets the position of the given regular_bg_ptr.
### `bn::regular_bg_move_by_action` (class, bn_regular_bg_actions.h:133)

Modifies the position of a regular_bg_ptr by a given delta.

`class regular_bg_move_by_action : public by_value_template_action<regular_bg_ptr, fixed_point, regular_bg_position_manager>`

- **regular_bg_move_by_action** `regular_bg_move_by_action(const regular_bg_ptr& bg, fixed delta_x, fixed delta_y) : by_value_template_action(bg, fixed_point(delta_x, delta_y))` (L146): Constructor.
  `bg` regular_bg_ptr to copy.; `delta_x` How much to add to the horizontal position of the given regular_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the given regular_bg_ptr when the action is updated.
- **regular_bg_move_by_action** `regular_bg_move_by_action(regular_bg_ptr&& bg, fixed delta_x, fixed delta_y) : by_value_template_action(move(bg), fixed_point(delta_x, delta_y))` (L159): Constructor.
  `bg` regular_bg_ptr to move.; `delta_x` How much to add to the horizontal position of the given regular_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the given regular_bg_ptr when the action is updated.
- **regular_bg_move_by_action** `regular_bg_move_by_action(const regular_bg_ptr& bg, const fixed_point& delta_position) : by_value_template_action(bg, delta_position)` (L169): Constructor.
  `bg` regular_bg_ptr to copy.; `delta_position` How much to add to the position of the given regular_bg_ptr when the action is updated.
- **regular_bg_move_by_action** `regular_bg_move_by_action(regular_bg_ptr&& bg, const fixed_point& delta_position) : by_value_template_action(move(bg), delta_position)` (L179): Constructor.
  `bg` regular_bg_ptr to move.; `delta_position` How much to add to the position of the given regular_bg_ptr when the action is updated.
- **bg** `const regular_bg_ptr& bg() const` (L187): Returns the regular_bg_ptr to modify.
- **delta_position** `const fixed_point& delta_position() const` (L195): Returns how much to add to the position of the given regular_bg_ptr when the action is updated.
### `bn::regular_bg_move_to_action` (class, bn_regular_bg_actions.h:208)

Modifies the position of a regular_bg_ptr until it has a given state.

`class regular_bg_move_to_action : public to_value_template_action<regular_bg_ptr, fixed_point, regular_bg_position_manager>`

- **regular_bg_move_to_action** `regular_bg_move_to_action(const regular_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L223): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the given regular_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the given regular_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the given regular_bg_ptr when the action is updated duration_updates times.
- **regular_bg_move_to_action** `regular_bg_move_to_action(regular_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L238): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the given regular_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the given regular_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the given regular_bg_ptr when the action is updated duration_updates times.
- **regular_bg_move_to_action** `regular_bg_move_to_action(const regular_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(bg, duration_updates, final_position)` (L251): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the given regular_bg_ptr is equal to final_position.; `final_position` Position of the given regular_bg_ptr when the action is updated duration_updates times.
- **regular_bg_move_to_action** `regular_bg_move_to_action(regular_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(move(bg), duration_updates, final_position)` (L263): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the given regular_bg_ptr is equal to final_position.; `final_position` Position of the given regular_bg_ptr when the action is updated duration_updates times.
- **bg** `const regular_bg_ptr& bg() const` (L271): Returns the regular_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L279): Returns the position of the given regular_bg_ptr when the action is updated the given number of times.
### `bn::regular_bg_move_loop_action` (class, bn_regular_bg_actions.h:293)

Modifies the position of a regular_bg_ptr from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class regular_bg_move_loop_action : public loop_value_template_action<regular_bg_ptr, fixed_point, regular_bg_position_manager>`

- **regular_bg_move_loop_action** `regular_bg_move_loop_action(const regular_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L308): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the given regular_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the given regular_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **regular_bg_move_loop_action** `regular_bg_move_loop_action(regular_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L323): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the given regular_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the given regular_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **regular_bg_move_loop_action** `regular_bg_move_loop_action(const regular_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(bg, duration_updates, final_position)` (L336): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the given regular_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **regular_bg_move_loop_action** `regular_bg_move_loop_action(regular_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(move(bg), duration_updates, final_position)` (L349): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the given regular_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const regular_bg_ptr& bg() const` (L357): Returns the regular_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L366): When the position of the given regular_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::regular_bg_move_toggle_action` (class, bn_regular_bg_actions.h:379)

Changes the position of a regular_bg_ptr when the action is updated a given number of times.

`class regular_bg_move_toggle_action : public toggle_value_template_action<regular_bg_ptr, fixed_point, regular_bg_position_manager>`

- **regular_bg_move_toggle_action** `regular_bg_move_toggle_action(const regular_bg_ptr& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(bg, duration_updates, fixed_point(new_x, new_y))` (L392): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the given regular_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **regular_bg_move_toggle_action** `regular_bg_move_toggle_action(regular_bg_ptr&& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(move(bg), duration_updates, fixed_point(new_x, new_y))` (L405): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the given regular_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **regular_bg_move_toggle_action** `regular_bg_move_toggle_action(const regular_bg_ptr& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(bg, duration_updates, new_position)` (L417): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the given regular_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **regular_bg_move_toggle_action** `regular_bg_move_toggle_action(regular_bg_ptr&& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(move(bg), duration_updates, new_position)` (L429): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the given regular_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **bg** `const regular_bg_ptr& bg() const` (L437): Returns the regular_bg_ptr to modify.
- **new_position** `const fixed_point& new_position() const` (L445): Returns the position of the given regular_bg_ptr when the action is updated the given number of times.
### `bn::regular_bg_top_left_position_manager` (class, bn_regular_bg_actions.h:460)

Manages the top-left position of a regular_bg_ptr.

`class regular_bg_top_left_position_manager`

- **get** `static fixed_point get(const regular_bg_ptr& bg)` (L467): Returns the top-left position of the given regular_bg_ptr.
- **set** `static void set(const fixed_point& top_left_position, regular_bg_ptr& bg)` (L475): Sets the top-left position of the given regular_bg_ptr.
### `bn::regular_bg_top_left_move_to_action` (class, bn_regular_bg_actions.h:488)

Modifies the top-left position of a regular_bg_ptr until it has a given state.

`class regular_bg_top_left_move_to_action : public to_value_template_action<regular_bg_ptr, fixed_point, regular_bg_top_left_position_manager>`

- **regular_bg_top_left_move_to_action** `regular_bg_top_left_move_to_action( const regular_bg_ptr& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : to_value_template_action(bg, duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L503): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the top-left position of the given regular_bg_ptr is equal to (final_x, final_y).; `final_top_left_x` Horizontal top-left position of the given regular_bg_ptr when the action is updated duration_updates times.; `final_top_left_y` Vertical top-left position of the given regular_bg_ptr when the action is updated duration_updates times.
- **regular_bg_top_left_move_to_action** `regular_bg_top_left_move_to_action( regular_bg_ptr&& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : to_value_template_action(move(bg), duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L519): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the top-left position of the given regular_bg_ptr is equal to (final_x, final_y).; `final_top_left_x` Horizontal top-left position of the given regular_bg_ptr when the action is updated duration_updates times.; `final_top_left_y` Vertical top-left position of the given regular_bg_ptr when the action is updated duration_updates times.
- **regular_bg_top_left_move_to_action** `regular_bg_top_left_move_to_action( const regular_bg_ptr& bg, int duration_updates, const fixed_point& final_top_left_position) : to_value_template_action(bg, duration_updates, final_top_left_position)` (L533): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the top-left position of the given regular_bg_ptr is equal to final_top_left_position.; `final_top_left_position` Top-left position of the given regular_bg_ptr when the action is updated duration_updates times.
- **regular_bg_top_left_move_to_action** `regular_bg_top_left_move_to_action( regular_bg_ptr&& bg, int duration_updates, const fixed_point& final_top_left_position) : to_value_template_action(move(bg), duration_updates, final_top_left_position)` (L547): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the top-left position of the given regular_bg_ptr is equal to final_top_left_position.; `final_top_left_position` Top-left position of the given regular_bg_ptr when the action is updated duration_updates times.
- **bg** `const regular_bg_ptr& bg() const` (L556): Returns the regular_bg_ptr to modify.
- **final_top_left_position** `const fixed_point& final_top_left_position() const` (L565): Returns the top-left position of the given regular_bg_ptr when the action is updated the given number of times.
### `bn::regular_bg_top_left_move_loop_action` (class, bn_regular_bg_actions.h:579)

Modifies the top-left position of a regular_bg_ptr from a minimum to a maximum. When the top-left position is equal to the given final state, it goes back to its initial state and vice versa.

`class regular_bg_top_left_move_loop_action : public loop_value_template_action<regular_bg_ptr, fixed_point, regular_bg_top_left_position_manager>`

- **regular_bg_top_left_move_loop_action** `regular_bg_top_left_move_loop_action( const regular_bg_ptr& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : loop_value_template_action(bg, duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L594): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_x` When the horizontal top-left position of the given regular_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_top_left_y` When the vertical top-left position of the given regular_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **regular_bg_top_left_move_loop_action** `regular_bg_top_left_move_loop_action( regular_bg_ptr&& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : loop_value_template_action(move(bg), duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L610): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_x` When the horizontal top-left position of the given regular_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_top_left_y` When the vertical top-left position of the given regular_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **regular_bg_top_left_move_loop_action** `regular_bg_top_left_move_loop_action( const regular_bg_ptr& bg, int duration_updates, const fixed_point& final_top_left_position) : loop_value_template_action(bg, duration_updates, final_top_left_position)` (L624): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_position` When the top-left position of the given regular_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **regular_bg_top_left_move_loop_action** `regular_bg_top_left_move_loop_action( regular_bg_ptr&& bg, int duration_updates, const fixed_point& final_top_left_position) : loop_value_template_action(move(bg), duration_updates, final_top_left_position)` (L638): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_position` When the top-left position of the given regular_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const regular_bg_ptr& bg() const` (L647): Returns the regular_bg_ptr to modify.
- **final_top_left_position** `const fixed_point& final_top_left_position() const` (L656): When the top-left position of the given regular_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::regular_bg_top_left_move_toggle_action` (class, bn_regular_bg_actions.h:669)

Changes the top-left position of a regular_bg_ptr when the action is updated a given number of times.

`class regular_bg_top_left_move_toggle_action : public toggle_value_template_action<regular_bg_ptr, fixed_point, regular_bg_top_left_position_manager>`

- **regular_bg_top_left_move_toggle_action** `regular_bg_top_left_move_toggle_action( const regular_bg_ptr& bg, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) : toggle_value_template_action(bg, duration_updates, fixed_point(new_top_left_x, new_top_left_y))` (L682): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the top-left position of the given regular_bg_ptr.; `new_top_left_x` New horizontal top-left position when the action is updated duration_updates times.; `new_top_left_y` New vertical top-left position when the action is updated duration_updates times.
- **regular_bg_top_left_move_toggle_action** `regular_bg_top_left_move_toggle_action( regular_bg_ptr&& bg, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) : toggle_value_template_action(move(bg), duration_updates, fixed_point(new_top_left_x, new_top_left_y))` (L696): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the top-left position of the given regular_bg_ptr.; `new_top_left_x` New horizontal top-left position when the action is updated duration_updates times.; `new_top_left_y` New vertical top-left position when the action is updated duration_updates times.
- **regular_bg_top_left_move_toggle_action** `regular_bg_top_left_move_toggle_action( const regular_bg_ptr& bg, int duration_updates, const fixed_point& new_top_left_position) : toggle_value_template_action(bg, duration_updates, new_top_left_position)` (L709): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the top-left position of the given regular_bg_ptr.; `new_top_left_position` New top-left position when the action is updated duration_updates times.
- **regular_bg_top_left_move_toggle_action** `regular_bg_top_left_move_toggle_action( regular_bg_ptr&& bg, int duration_updates, const fixed_point& new_top_left_position) : toggle_value_template_action(move(bg), duration_updates, new_top_left_position)` (L722): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the top-left position of the given regular_bg_ptr.; `new_top_left_position` New top-left position when the action is updated duration_updates times.
- **bg** `const regular_bg_ptr& bg() const` (L731): Returns the regular_bg_ptr to modify.
- **new_top_left_position** `const fixed_point& new_top_left_position() const` (L740): Returns the top-left position of the given regular_bg_ptr when the action is updated the given number of times.
### `bn::regular_bg_mosaic_manager` (class, bn_regular_bg_actions.h:755)

Manages if the mosaic effect must be applied to a regular_bg_ptr or not.

`class regular_bg_mosaic_manager`

- **get** `static bool get(const regular_bg_ptr& bg)` (L762): Indicates if the mosaic effect is applied to the given regular_bg_ptr or not.
- **set** `static void set(bool mosaic_enabled, regular_bg_ptr& bg)` (L770): Sets if the mosaic effect must be applied to the given regular_bg_ptr or not.
### `bn::regular_bg_mosaic_toggle_action` (class, bn_regular_bg_actions.h:784)

Toggles if the mosaic effect must be applied to a regular_bg_ptr or not when the action is updated a given number of times.

`class regular_bg_mosaic_toggle_action : public bool_toggle_value_template_action<regular_bg_ptr, regular_bg_mosaic_manager>`

- **regular_bg_mosaic_toggle_action** `regular_bg_mosaic_toggle_action(const regular_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L795): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the mosaic effect must be applied or not.
- **regular_bg_mosaic_toggle_action** `regular_bg_mosaic_toggle_action(regular_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L806): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the mosaic effect must be applied or not.
- **bg** `const regular_bg_ptr& bg() const` (L814): Returns the regular_bg_ptr to modify.
### `bn::regular_bg_blending_manager` (class, bn_regular_bg_actions.h:829)

Manages if blending must be applied to a regular_bg_ptr or not.

`class regular_bg_blending_manager`

- **get** `static bool get(const regular_bg_ptr& bg)` (L836): Indicates if blending is applied to the given regular_bg_ptr or not.
- **set** `static void set(bool blending_enabled, regular_bg_ptr& bg)` (L844): Sets if blending must be applied to the given regular_bg_ptr or not.
### `bn::regular_bg_blending_toggle_action` (class, bn_regular_bg_actions.h:858)

Toggles if blending must be applied to a regular_bg_ptr or not when the action is updated a given number of times.

`class regular_bg_blending_toggle_action : public bool_toggle_value_template_action<regular_bg_ptr, regular_bg_blending_manager>`

- **regular_bg_blending_toggle_action** `regular_bg_blending_toggle_action(const regular_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L869): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if blending must be applied or not.
- **regular_bg_blending_toggle_action** `regular_bg_blending_toggle_action(regular_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L880): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if blending must be applied or not.
- **bg** `const regular_bg_ptr& bg() const` (L888): Returns the regular_bg_ptr to modify.
### `bn::regular_bg_blending_top_manager` (class, bn_regular_bg_actions.h:901)

Manages if a regular_bg_ptr must be part of the blending top layer or not.

`class regular_bg_blending_top_manager`

- **get** `static bool get(const regular_bg_ptr& bg)` (L908): Indicates if the given regular_bg_ptr is part of the blending top layer or not.
- **set** `static void set(bool blending_top_enabled, regular_bg_ptr& bg)` (L916): Sets if the given regular_bg_ptr must be part of the blending top layer or not.
### `bn::regular_bg_blending_top_toggle_action` (class, bn_regular_bg_actions.h:930)

Toggles if a regular_bg_ptr is part of the blending top layer or not when the action is updated a given number of times.

`class regular_bg_blending_top_toggle_action : public bool_toggle_value_template_action<regular_bg_ptr, regular_bg_blending_top_manager>`

- **regular_bg_blending_top_toggle_action** `regular_bg_blending_top_toggle_action(const regular_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L941): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given regular_bg_ptr is part of the blending top layer or not.
- **regular_bg_blending_top_toggle_action** `regular_bg_blending_top_toggle_action(regular_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L952): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given regular_bg_ptr is part of the blending top layer or not.
- **bg** `const regular_bg_ptr& bg() const` (L960): Returns the regular_bg_ptr to modify.
### `bn::regular_bg_blending_bottom_manager` (class, bn_regular_bg_actions.h:973)

Manages if a regular_bg_ptr must be part of the blending bottom layer or not.

`class regular_bg_blending_bottom_manager`

- **get** `static bool get(const regular_bg_ptr& bg)` (L980): Indicates if the given regular_bg_ptr is part of the blending bottom layer or not.
- **set** `static void set(bool blending_bottom_enabled, regular_bg_ptr& bg)` (L988): Sets if the given regular_bg_ptr must be part of the blending bottom layer or not.
### `bn::regular_bg_blending_bottom_toggle_action` (class, bn_regular_bg_actions.h:1002)

Toggles if a regular_bg_ptr is part of the blending bottom layer or not when the action is updated a given number of times.

`class regular_bg_blending_bottom_toggle_action : public bool_toggle_value_template_action<regular_bg_ptr, regular_bg_blending_bottom_manager>`

- **regular_bg_blending_bottom_toggle_action** `regular_bg_blending_bottom_toggle_action(const regular_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L1013): Constructor.
  `bg` regular_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given regular_bg_ptr is part of the blending bottom layer or not.
- **regular_bg_blending_bottom_toggle_action** `regular_bg_blending_bottom_toggle_action(regular_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L1024): Constructor.
  `bg` regular_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given regular_bg_ptr is part of the blending bottom layer or not.
- **bg** `const regular_bg_ptr& bg() const` (L1032): Returns the regular_bg_ptr to modify.

## bn_regular_bg_animate_actions.h

### `bn::iregular_bg_animate_action` (class, bn_regular_bg_animate_actions.h:39)

Base class of bn::regular_bg_animate_action.

`class iregular_bg_animate_action`

Can be used as a reference type for all bn::regular_bg_animate_action objects.

- **operator=** `iregular_bg_animate_action& operator=(const iregular_bg_animate_action& other)` (L50): Copy assignment operator.
  `other` iregular_bg_animate_action to copy.; devuelve: Reference to this.
- **operator=** `iregular_bg_animate_action& operator=(iregular_bg_animate_action&& other) noexcept` (L57): Move assignment operator.
  `other` iregular_bg_animate_action to move.; devuelve: Reference to this.
- **update** `void update()` (L62): Changes the map of the given regular_bg_ptr when the given amount of update calls are done.
- **done** `bool done() const` (L67): Indicates if the action must not be updated anymore.
- **reset** `void reset()` (L75): Resets the action to its initial state.
- **regular_bg** `const regular_bg_ptr& regular_bg() const` (L84): Returns the regular_bg_ptr to modify.
- **wait_updates** `int wait_updates() const` (L93): Returns the number of times the action must be updated before changing the map of the given regular_bg_ptr.
- **set_wait_updates** `void set_wait_updates(int wait_updates)` (L102): Sets the number of times the action must be updated before changing the map of the given regular_bg_ptr.
- **next_change_updates** `int next_change_updates() const` (L107): Returns the number of times the action must be updated before the next map change.
- **set_next_change_updates** `void set_next_change_updates(int next_change_updates)` (L115): Sets the number of times the action must be updated before the next map change.
- **map_item** `const regular_bg_map_item& map_item() const` (L120): Returns the regular_bg_map_item used to create the new map to use by the given regular_bg_ptr.
- **map_indexes** `const ivector<uint16_t>& map_indexes() const` (L128): Returns the indexes of the maps to reference in the given regular_bg_map_item.
- **update_forever** `bool update_forever() const` (L136): Indicates if the action can be updated forever or not.
- **set_update_forever** `void set_update_forever(bool forever)` (L144): Specifies if the action must be updated forever or not.
- **current_index** `int current_index() const` (L152): Returns the current index of the given map_indexes (not the current index of the map to reference in the given map_item).
  This index is the one to be used on the next map change. It can be out of bounds if the action is done.
- **set_current_index** `void set_current_index(int current_index)` (L161): Sets the current index of the given map_indexes (not the current index of the map to reference in the given map_item).
- **set_current_index** `void set_current_index(int current_index, bool update_bg_now)` (L172): Sets the current index of the given map_indexes (not the current index of the map to reference in the given map_item).
  `current_index` New index of the given map_indexes.; `update_bg_now` Specifies if the background must be updated now or on the next map change.
- **current_map_index** `int current_map_index() const` (L179): Returns the current index of the map to reference in the given map_item.
  This index is the one to be used on the next map change.
- **once** `static regular_bg_animate_action once( const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L226): Generates a regular_bg_animate_action which loops over the given regular_bg maps only once.
  `regular_bg` regular_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested regular_bg_animate_action.
- **once** `static regular_bg_animate_action once( regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L242): Generates a regular_bg_animate_action which loops over the given regular_bg maps only once.
  `regular_bg` regular_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in the given regular_bg_map_item.; devuelve: The requested regular_bg_animate_action.
- **forever** `static regular_bg_animate_action forever( const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L258): Generates a regular_bg_animate_action which loops over the given regular_bg maps forever.
  `regular_bg` regular_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in the given regular_bg_map_item.; devuelve: The requested regular_bg_animate_action.
- **forever** `static regular_bg_animate_action forever( regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L274): Generates a regular_bg_animate_action which loops over the given regular_bg maps forever.
  `regular_bg` regular_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in the given regular_bg_map_item.; devuelve: The requested regular_bg_animate_action.
- **regular_bg_animate_action** `regular_bg_animate_action(const regular_bg_animate_action& other)` (L285): Copy constructor.
  `other` regular_bg_animate_action to copy.
- **regular_bg_animate_action** `regular_bg_animate_action(regular_bg_animate_action&& other) noexcept : _regular_bg(move(other._regular_bg)), _map_item(other._map_item), _map_indexes(other._map_indexes)` (L298): Move constructor.
  `other` regular_bg_animate_action to move.
- **regular_bg_animate_action** `regular_bg_animate_action(const iregular_bg_animate_action& other)` (L311): Copy constructor.
  `other` iregular_bg_animate_action to copy.
- **operator=** `regular_bg_animate_action& operator=(const regular_bg_animate_action& other)` (L328): Copy assignment operator.
  `other` regular_bg_animate_action to copy.; devuelve: Reference to this.
- **operator=** `regular_bg_animate_action& operator=(regular_bg_animate_action&& other) noexcept` (L346): Move assignment operator.
  `other` regular_bg_animate_action to move.; devuelve: Reference to this.
- **operator=** `regular_bg_animate_action& operator=(const iregular_bg_animate_action& other)` (L364): Copy assignment operator.
  `other` iregular_bg_animate_action to copy.; devuelve: Reference to this.
- **operator=** `regular_bg_animate_action& operator=(iregular_bg_animate_action&& other) noexcept` (L375): Move assignment operator.
  `other` ivector to move.; devuelve: Reference to this.
- **create_regular_bg_animate_action_once** `template<typename ...Args> auto create_regular_bg_animate_action_once( const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)` (L423): Generates a regular_bg_animate_action which loops over the given regular_bg maps only once.
  `regular_bg` regular_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested regular_bg_animate_action.
- **create_regular_bg_animate_action_once** `template<typename ...Args> auto create_regular_bg_animate_action_once( regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)` (L444): Generates a regular_bg_animate_action which loops over the given regular_bg maps only once.
  `regular_bg` regular_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested regular_bg_animate_action.
- **create_regular_bg_animate_action_forever** `template<typename ...Args> auto create_regular_bg_animate_action_forever( const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)` (L465): Generates a regular_bg_animate_action which loops over the given regular_bg maps forever.
  `regular_bg` regular_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested regular_bg_animate_action.
- **create_regular_bg_animate_action_forever** `template<typename ...Args> auto create_regular_bg_animate_action_forever( regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)` (L486): Generates a regular_bg_animate_action which loops over the given regular_bg maps forever.
  `regular_bg` regular_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested regular_bg_animate_action.
### `bn::iregular_bg_cached_animate_action` (class, bn_regular_bg_animate_actions.h:507)

Base class of bn::regular_bg_cached_animate_action.

`class iregular_bg_cached_animate_action`

Can be used as a reference type for all bn::regular_bg_cached_animate_action objects.

- **operator=** `iregular_bg_cached_animate_action& operator=(const iregular_bg_cached_animate_action& other)` (L518): Copy assignment operator.
  `other` iregular_bg_cached_animate_action to copy.; devuelve: Reference to this.
- **operator=** `iregular_bg_cached_animate_action& operator=(iregular_bg_cached_animate_action&& other) noexcept` (L525): Move assignment operator.
  `other` iregular_bg_cached_animate_action to move.; devuelve: Reference to this.
- **update** `void update()` (L530): Changes the map of the given regular_bg_ptr when the given amount of update calls are done.
- **done** `bool done() const` (L535): Indicates if the action must not be updated anymore.
- **reset** `void reset()` (L543): Resets the action to its initial state.
- **regular_bg** `const regular_bg_ptr& regular_bg() const` (L552): Returns the regular_bg_ptr to modify.
- **wait_updates** `int wait_updates() const` (L561): Returns the number of times the action must be updated before changing the map of the given regular_bg_ptr.
- **set_wait_updates** `void set_wait_updates(int wait_updates)` (L570): Sets the number of times the action must be updated before changing the map of the given regular_bg_ptr.
- **next_change_updates** `int next_change_updates() const` (L575): Returns the number of times the action must be updated before the next map change.
- **set_next_change_updates** `void set_next_change_updates(int next_change_updates)` (L583): Sets the number of times the action must be updated before the next map change.
- **maps** `const ivector<regular_bg_map_ptr>& maps() const` (L588): Returns the maps to use by the given regular_bg_ptr.
- **update_forever** `bool update_forever() const` (L596): Indicates if the action can be updated forever or not.
- **set_update_forever** `void set_update_forever(bool forever)` (L604): Specifies if the action must be updated forever or not.
- **current_index** `int current_index() const` (L611): Returns the current index of the given maps.
  This index is the one to be used on the next map change. It can be out of bounds if the action is done.
- **set_current_index** `void set_current_index(int current_index)` (L619): Sets the current index of the given maps.
- **set_current_index** `void set_current_index(int current_index, bool update_bg_now)` (L629): Sets the current index of the given maps.
  `current_index` New index of the given maps.; `update_bg_now` Specifies if the background must be updated now or on the next map change.
- **current_map** `const regular_bg_map_ptr& current_map() const` (L636): Returns the current map.
  This map is the one to be used on the next map change.
- **once** `static regular_bg_cached_animate_action once( const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L686): Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps only once.
  `regular_bg` regular_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested regular_bg_cached_animate_action.
- **once** `static regular_bg_cached_animate_action once( regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L702): Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps only once.
  `regular_bg` regular_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in the given regular_bg_map_item.; devuelve: The requested regular_bg_cached_animate_action.
- **once** `static regular_bg_cached_animate_action once( const regular_bg_ptr& regular_bg, int wait_updates, span<regular_bg_map_ptr> maps)` (L717): Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps only once.
  `regular_bg` regular_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `maps` List of maps to use by the given regular_bg_ptr.; devuelve: The requested regular_bg_cached_animate_action.
- **once** `static regular_bg_cached_animate_action once( regular_bg_ptr&& regular_bg, int wait_updates, span<regular_bg_map_ptr> maps)` (L731): Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps only once.
  `regular_bg` regular_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `maps` List of maps to use by the given regular_bg_ptr.; devuelve: The requested regular_bg_cached_animate_action.
- **forever** `static regular_bg_cached_animate_action forever( const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L746): Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps forever.
  `regular_bg` regular_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested regular_bg_cached_animate_action.
- **forever** `static regular_bg_cached_animate_action forever( regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item, const span<const uint16_t>& map_indexes)` (L762): Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps forever.
  `regular_bg` regular_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in the given regular_bg_map_item.; devuelve: The requested regular_bg_cached_animate_action.
- **forever** `static regular_bg_cached_animate_action forever( const regular_bg_ptr& regular_bg, int wait_updates, span<regular_bg_map_ptr> maps)` (L777): Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps forever.
  `regular_bg` regular_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `maps` List of maps to use by the given regular_bg_ptr.; devuelve: The requested regular_bg_cached_animate_action.
- **forever** `static regular_bg_cached_animate_action forever( regular_bg_ptr&& regular_bg, int wait_updates, span<regular_bg_map_ptr> maps)` (L791): Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps forever.
  `regular_bg` regular_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `maps` List of maps to use by the given regular_bg_ptr.; devuelve: The requested regular_bg_cached_animate_action.
- **regular_bg_cached_animate_action** `regular_bg_cached_animate_action(const regular_bg_cached_animate_action& other)` (L801): Copy constructor.
  `other` regular_bg_cached_animate_action to copy.
- **regular_bg_cached_animate_action** `regular_bg_cached_animate_action(regular_bg_cached_animate_action&& other) noexcept : _regular_bg(move(other._regular_bg)), _maps(move(other._maps))` (L813): Move constructor.
  `other` regular_bg_cached_animate_action to move.
- **regular_bg_cached_animate_action** `regular_bg_cached_animate_action(const iregular_bg_cached_animate_action& other)` (L825): Copy constructor.
  `other` iregular_bg_cached_animate_action to copy.
- **operator=** `regular_bg_cached_animate_action& operator=(const regular_bg_cached_animate_action& other)` (L840): Copy assignment operator.
  `other` regular_bg_cached_animate_action to copy.; devuelve: Reference to this.
- **operator=** `regular_bg_cached_animate_action& operator=(regular_bg_cached_animate_action&& other) noexcept` (L857): Move assignment operator.
  `other` regular_bg_cached_animate_action to move.; devuelve: Reference to this.
- **operator=** `regular_bg_cached_animate_action& operator=(const iregular_bg_cached_animate_action& other)` (L874): Copy assignment operator.
  `other` iregular_bg_cached_animate_action to copy.; devuelve: Reference to this.
- **operator=** `regular_bg_cached_animate_action& operator=(iregular_bg_cached_animate_action&& other) noexcept` (L885): Move assignment operator.
  `other` ivector to move.; devuelve: Reference to this.
- **create_regular_bg_cached_animate_action_once** `template<typename ...Args> auto create_regular_bg_cached_animate_action_once( const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)` (L950): Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps only once.
  `regular_bg` regular_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested regular_bg_cached_animate_action.
- **create_regular_bg_cached_animate_action_once** `template<typename ...Args> auto create_regular_bg_cached_animate_action_once( regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)` (L971): Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps only once.
  `regular_bg` regular_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested regular_bg_cached_animate_action.
- **create_regular_bg_cached_animate_action_forever** `template<typename ...Args> auto create_regular_bg_cached_animate_action_forever( const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)` (L992): Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps forever.
  `regular_bg` regular_bg_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested regular_bg_cached_animate_action.
- **create_regular_bg_cached_animate_action_forever** `template<typename ...Args> auto create_regular_bg_cached_animate_action_forever( regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)` (L1013): Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps forever.
  `regular_bg` regular_bg_ptr to move.; `wait_updates` Number of times the action must be updated before changing the map of the given regular_bg_ptr.; `map_item` It creates the new map to use by the given regular_bg_ptr.; `map_indexes` Indexes of the maps to reference in map_item.; devuelve: The requested regular_bg_cached_animate_action.

## bn_regular_bg_animate_actions_fwd.h

### `bn::regular_bg_animate_action` (class, bn_regular_bg_animate_actions_fwd.h:38)

Changes the map of a regular_bg_ptr when the action is updated a given number of times.

`template<int MaxSize> class regular_bg_animate_action`

Each time the map of a regular_bg_ptr must be changed, it is searched for and created if it has not been found,
so the maps are not cached.

### `bn::regular_bg_cached_animate_action` (class, bn_regular_bg_animate_actions_fwd.h:58)

Changes the map of a regular_bg_ptr when the action is updated a given number of times.

`template<int MaxSize> class regular_bg_cached_animate_action`

This action differs from regular_bg_animate_action in that all maps to use are cached,
so it is faster but takes way more VRAM.


## bn_rumble_actions.h

### `bn::rumble_manager` (class, bn_rumble_actions.h:29)

Manages if rumble must be enabled or not.

`class rumble_manager`

- **get** `static bool get()` (L36): Indicates if rumble is enabled or not.
- **set** `static void set(bool enabled)` (L44): Sets if rumble must be enabled or not.
### `bn::rumble_toggle_action` (class, bn_rumble_actions.h:57)

Toggles if rumble must be enabled or not when the action is updated a given number of times.

`class rumble_toggle_action : public bool_toggle_template_action<rumble_manager>`

- **rumble_toggle_action** `explicit rumble_toggle_action(int duration_updates) : bool_toggle_template_action(duration_updates)` (L66): Constructor.
  `duration_updates` How many times the action has to be updated to toggle if rumble must be inverted or not.

## bn_sound_actions.h

### `bn::sound_speed_manager` (class, bn_sound_actions.h:32)

Manages the speed of a sound effect.

`class sound_speed_manager`

- **get** `static fixed get(sound_handle handle)` (L39): Returns the speed of the given sound_handle.
- **set** `static void set(fixed speed, sound_handle handle)` (L47): Sets the speed of the given sound_handle.
### `bn::sound_speed_to_action` (class, bn_sound_actions.h:60)

Modifies the speed of a sound effect until it has a given state.

`class sound_speed_to_action : public to_value_template_action<sound_handle, fixed, sound_speed_manager>`

- **sound_speed_to_action** `sound_speed_to_action(sound_handle handle, int duration_updates, fixed final_speed) : to_value_template_action(handle, duration_updates, final_speed)` (L71): Constructor.
  `handle` sound_handle to modify.; `duration_updates` Number of times that the action must be updated until the speed of the given sound_handle is equal to final_speed.; `final_speed` Speed when the action is updated duration_updates times.
- **handle** `sound_handle handle() const` (L80): Returns the sound_handle to modify.
- **final_speed** `fixed final_speed() const` (L88): Returns the speed of the given sound_handle when the action is updated the given number of times.
### `bn::sound_speed_loop_action` (class, bn_sound_actions.h:102)

Modifies the speed of a sound effect from a minimum to a maximum. When the speed is equal to the given final state, it goes back to its initial state and vice versa.

`class sound_speed_loop_action : public loop_value_template_action<sound_handle, fixed, sound_speed_manager>`

- **sound_speed_loop_action** `sound_speed_loop_action(sound_handle handle, int duration_updates, fixed final_speed) : loop_value_template_action(handle, duration_updates, final_speed)` (L114): Constructor.
  `handle` sound_handle to modify.; `duration_updates` How many times the action has to be updated before changing the direction of the speed delta.; `final_speed` When the speed of the given sound_handle is equal to this parameter, it goes back to its initial state and vice versa.
- **handle** `sound_handle handle() const` (L123): Returns the sound_handle to modify.
- **final_speed** `fixed final_speed() const` (L132): When the speed of the given sound_handle is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sound_speed_toggle_action` (class, bn_sound_actions.h:145)

Changes the speed of a sound effect when the action is updated a given number of times.

`class sound_speed_toggle_action : public toggle_value_template_action<sound_handle, fixed, sound_speed_manager>`

- **sound_speed_toggle_action** `sound_speed_toggle_action(sound_handle handle, int duration_updates, fixed new_speed) : toggle_value_template_action(handle, duration_updates, new_speed)` (L156): Constructor.
  `handle` sound_handle to modify.; `duration_updates` How many times the action has to be updated to change the speed of the given sound_handle.; `new_speed` New speed when the action is updated duration_updates times.
- **handle** `sound_handle handle() const` (L165): Returns the sound_handle to modify.
- **new_speed** `fixed new_speed() const` (L173): Returns the speed of the given sound_handle when the action is updated the given number of times.
### `bn::sound_panning_manager` (class, bn_sound_actions.h:188)

Manages the panning of a sound effect.

`class sound_panning_manager`

- **get** `static fixed get(sound_handle handle)` (L195): Returns the panning of the given sound_handle.
- **set** `static void set(fixed panning, sound_handle handle)` (L203): Sets the panning of the given sound_handle.
### `bn::sound_panning_to_action` (class, bn_sound_actions.h:216)

Modifies the panning of a sound effect until it has a given state.

`class sound_panning_to_action : public to_value_template_action<sound_handle, fixed, sound_panning_manager>`

- **sound_panning_to_action** `sound_panning_to_action(sound_handle handle, int duration_updates, fixed final_panning) : to_value_template_action(handle, duration_updates, final_panning)` (L227): Constructor.
  `handle` sound_handle to modify.; `duration_updates` Number of times that the action must be updated until the panning of the given sound_handle is equal to final_panning.; `final_panning` Panning when the action is updated duration_updates times.
- **handle** `sound_handle handle() const` (L236): Returns the sound_handle to modify.
- **final_panning** `fixed final_panning() const` (L244): Returns the panning of the given sound_handle when the action is updated the given number of times.
### `bn::sound_panning_loop_action` (class, bn_sound_actions.h:258)

Modifies the panning of a sound effect from a minimum to a maximum. When the panning is equal to the given final state, it goes back to its initial state and vice versa.

`class sound_panning_loop_action : public loop_value_template_action<sound_handle, fixed, sound_panning_manager>`

- **sound_panning_loop_action** `sound_panning_loop_action(sound_handle handle, int duration_updates, fixed final_panning) : loop_value_template_action(handle, duration_updates, final_panning)` (L270): Constructor.
  `handle` sound_handle to modify.; `duration_updates` How many times the action has to be updated before changing the direction of the panning delta.; `final_panning` When the panning of the given sound_handle is equal to this parameter, it goes back to its initial state and vice versa.
- **handle** `sound_handle handle() const` (L279): Returns the sound_handle to modify.
- **final_panning** `fixed final_panning() const` (L288): When the panning of the given sound_handle is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sound_panning_toggle_action` (class, bn_sound_actions.h:301)

Changes the panning of a sound effect when the action is updated a given number of times.

`class sound_panning_toggle_action : public toggle_value_template_action<sound_handle, fixed, sound_panning_manager>`

- **sound_panning_toggle_action** `sound_panning_toggle_action(sound_handle handle, int duration_updates, fixed new_panning) : toggle_value_template_action(handle, duration_updates, new_panning)` (L312): Constructor.
  `handle` sound_handle to modify.; `duration_updates` How many times the action has to be updated to change the panning of the given sound_handle.; `new_panning` New panning when the action is updated duration_updates times.
- **handle** `sound_handle handle() const` (L321): Returns the sound_handle to modify.
- **new_panning** `fixed new_panning() const` (L329): Returns the panning of the given sound_handle when the action is updated the given number of times.
### `bn::sound_master_volume_manager` (class, bn_sound_actions.h:344)

Manages the master volume of all sound effects.

`class sound_master_volume_manager`

- **get** `static fixed get()` (L351): Returns the master volume of all sound effects.
- **set** `static void set(fixed volume)` (L360): Sets the master volume of all sound effects.
  `volume` New volume in the range [0..1].
### `bn::sound_master_volume_to_action` (class, bn_sound_actions.h:373)

Modifies the master volume of all sound effects until it has a given state.

`class sound_master_volume_to_action : public to_template_action<fixed, sound_master_volume_manager>`

- **sound_master_volume_to_action** `sound_master_volume_to_action(int duration_updates, fixed final_volume) : to_template_action(duration_updates, final_volume)` (L385): Constructor.
  `duration_updates` Number of times that the action must be updated until the volume is equal to final_volume.; `final_volume` Volume when the action is updated duration_updates times.
  This volume must be in the range [0..1].
- **final_volume** `fixed final_volume() const` (L394): Returns the volume when the action is updated the given number of times.
### `bn::sound_master_volume_loop_action` (class, bn_sound_actions.h:408)

Modifies the master volume of all sound effects from a minimum to a maximum. When the volume is equal to the given final state, it goes back to its initial state and vice versa.

`class sound_master_volume_loop_action : public loop_template_action<fixed, sound_master_volume_manager>`

- **sound_master_volume_loop_action** `sound_master_volume_loop_action(int duration_updates, fixed final_volume) : loop_template_action(duration_updates, final_volume)` (L421): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the volume delta.; `final_volume` When the volume is equal to this parameter, it goes back to its initial state and vice versa.
  This volume must be in the range [0..1].
- **final_volume** `fixed final_volume() const` (L430): When the volume is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::sound_master_volume_toggle_action` (class, bn_sound_actions.h:443)

Changes the master volume of all sound effects when the action is updated a given number of times.

`class sound_master_volume_toggle_action : public toggle_template_action<fixed, sound_master_volume_manager>`

- **sound_master_volume_toggle_action** `sound_master_volume_toggle_action(int duration_updates, fixed new_volume) : toggle_template_action(duration_updates, new_volume)` (L454): Constructor.
  `duration_updates` How many times the action has to be updated to change the volume.; `new_volume` New volume when the action is updated duration_updates times.
  This volume must be in the range [0..1].
- **new_volume** `fixed new_volume() const` (L463): Returns the volume when the action is updated the given number of times.

## bn_sp_direct_bitmap_bg_actions.h

### `bn::sp_direct_bitmap_bg_visible_manager` (class, bn_sp_direct_bitmap_bg_actions.h:31)

Manages if a sp_direct_bitmap_bg_ptr must be committed to the GBA or not.

`class sp_direct_bitmap_bg_visible_manager`

- **get** `static bool get(const sp_direct_bitmap_bg_ptr& bg)` (L38): Indicates if the given sp_direct_bitmap_bg_ptr is committed to the GBA or not.
- **set** `static void set(bool visible, sp_direct_bitmap_bg_ptr& bg)` (L46): Sets if the given sp_direct_bitmap_bg_ptr must be committed to the GBA or not.
### `bn::sp_direct_bitmap_bg_visible_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:60)

Toggles if a sp_direct_bitmap_bg_ptr must be committed to the GBA or not when the action is updated a given number of times.

`class sp_direct_bitmap_bg_visible_toggle_action : public bool_toggle_value_template_action<sp_direct_bitmap_bg_ptr, sp_direct_bitmap_bg_visible_manager>`

- **sp_direct_bitmap_bg_visible_toggle_action** `sp_direct_bitmap_bg_visible_toggle_action(const sp_direct_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L71): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given sp_direct_bitmap_bg_ptr must be committed to the GBA or not.
- **sp_direct_bitmap_bg_visible_toggle_action** `sp_direct_bitmap_bg_visible_toggle_action(sp_direct_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L82): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given sp_direct_bitmap_bg_ptr must be committed to the GBA or not.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L90): Returns the sp_direct_bitmap_bg_ptr to modify.
### `bn::sp_direct_bitmap_bg_position_manager` (class, bn_sp_direct_bitmap_bg_actions.h:105)

Manages the position of a sp_direct_bitmap_bg_ptr.

`class sp_direct_bitmap_bg_position_manager`

- **get** `static const fixed_point& get(const sp_direct_bitmap_bg_ptr& bg)` (L112): Returns the position of the given sp_direct_bitmap_bg_ptr.
- **set** `static void set(const fixed_point& position, sp_direct_bitmap_bg_ptr& bg)` (L120): Sets the position of the given sp_direct_bitmap_bg_ptr.
### `bn::sp_direct_bitmap_bg_move_by_action` (class, bn_sp_direct_bitmap_bg_actions.h:133)

Modifies the position of a sp_direct_bitmap_bg_ptr by a given delta.

`class sp_direct_bitmap_bg_move_by_action : public by_value_template_action<sp_direct_bitmap_bg_ptr, fixed_point, sp_direct_bitmap_bg_position_manager>`

- **sp_direct_bitmap_bg_move_by_action** `sp_direct_bitmap_bg_move_by_action(const sp_direct_bitmap_bg_ptr& bg, fixed delta_x, fixed delta_y) : by_value_template_action(bg, fixed_point(delta_x, delta_y))` (L146): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `delta_x` How much to add to the horizontal position of the given sp_direct_bitmap_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the given sp_direct_bitmap_bg_ptr when the action is updated.
- **sp_direct_bitmap_bg_move_by_action** `sp_direct_bitmap_bg_move_by_action(sp_direct_bitmap_bg_ptr&& bg, fixed delta_x, fixed delta_y) : by_value_template_action(move(bg), fixed_point(delta_x, delta_y))` (L159): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `delta_x` How much to add to the horizontal position of the given sp_direct_bitmap_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the given sp_direct_bitmap_bg_ptr when the action is updated.
- **sp_direct_bitmap_bg_move_by_action** `sp_direct_bitmap_bg_move_by_action(const sp_direct_bitmap_bg_ptr& bg, const fixed_point& delta_position) : by_value_template_action(bg, delta_position)` (L170): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `delta_position` How much to add to the position of the given sp_direct_bitmap_bg_ptr when the action is updated.
- **sp_direct_bitmap_bg_move_by_action** `sp_direct_bitmap_bg_move_by_action(sp_direct_bitmap_bg_ptr&& bg, const fixed_point& delta_position) : by_value_template_action(move(bg), delta_position)` (L181): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `delta_position` How much to add to the position of the given sp_direct_bitmap_bg_ptr when the action is updated.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L189): Returns the sp_direct_bitmap_bg_ptr to modify.
- **delta_position** `const fixed_point& delta_position() const` (L197): Returns how much to add to the position of the given sp_direct_bitmap_bg_ptr when the action is updated.
### `bn::sp_direct_bitmap_bg_move_to_action` (class, bn_sp_direct_bitmap_bg_actions.h:210)

Modifies the position of a sp_direct_bitmap_bg_ptr until it has a given state.

`class sp_direct_bitmap_bg_move_to_action : public to_value_template_action<sp_direct_bitmap_bg_ptr, fixed_point, sp_direct_bitmap_bg_position_manager>`

- **sp_direct_bitmap_bg_move_to_action** `sp_direct_bitmap_bg_move_to_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L225): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the given sp_direct_bitmap_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_move_to_action** `sp_direct_bitmap_bg_move_to_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L241): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the given sp_direct_bitmap_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_move_to_action** `sp_direct_bitmap_bg_move_to_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(bg, duration_updates, final_position)` (L255): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the given sp_direct_bitmap_bg_ptr is equal to final_position.; `final_position` Position of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_move_to_action** `sp_direct_bitmap_bg_move_to_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(move(bg), duration_updates, final_position)` (L269): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the given sp_direct_bitmap_bg_ptr is equal to final_position.; `final_position` Position of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L278): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L287): Returns the position of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_move_loop_action` (class, bn_sp_direct_bitmap_bg_actions.h:301)

Modifies the position of a sp_direct_bitmap_bg_ptr from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class sp_direct_bitmap_bg_move_loop_action : public loop_value_template_action<sp_direct_bitmap_bg_ptr, fixed_point, sp_direct_bitmap_bg_position_manager>`

- **sp_direct_bitmap_bg_move_loop_action** `sp_direct_bitmap_bg_move_loop_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L316): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sp_direct_bitmap_bg_move_loop_action** `sp_direct_bitmap_bg_move_loop_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L332): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sp_direct_bitmap_bg_move_loop_action** `sp_direct_bitmap_bg_move_loop_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(bg, duration_updates, final_position)` (L346): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sp_direct_bitmap_bg_move_loop_action** `sp_direct_bitmap_bg_move_loop_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(move(bg), duration_updates, final_position)` (L360): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L369): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L378): When the position of the given sp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sp_direct_bitmap_bg_move_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:391)

Changes the position of a sp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class sp_direct_bitmap_bg_move_toggle_action : public toggle_value_template_action<sp_direct_bitmap_bg_ptr, fixed_point, sp_direct_bitmap_bg_position_manager>`

- **sp_direct_bitmap_bg_move_toggle_action** `sp_direct_bitmap_bg_move_toggle_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(bg, duration_updates, fixed_point(new_x, new_y))` (L404): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the given sp_direct_bitmap_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_move_toggle_action** `sp_direct_bitmap_bg_move_toggle_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(move(bg), duration_updates, fixed_point(new_x, new_y))` (L418): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the given sp_direct_bitmap_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_move_toggle_action** `sp_direct_bitmap_bg_move_toggle_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(bg, duration_updates, new_position)` (L431): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the given sp_direct_bitmap_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_move_toggle_action** `sp_direct_bitmap_bg_move_toggle_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(move(bg), duration_updates, new_position)` (L444): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the given sp_direct_bitmap_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L453): Returns the sp_direct_bitmap_bg_ptr to modify.
- **new_position** `const fixed_point& new_position() const` (L462): Returns the position of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_top_left_position_manager` (class, bn_sp_direct_bitmap_bg_actions.h:477)

Manages the top-left position of a sp_direct_bitmap_bg_ptr.

`class sp_direct_bitmap_bg_top_left_position_manager`

- **get** `static fixed_point get(const sp_direct_bitmap_bg_ptr& bg)` (L484): Returns the top-left position of the given sp_direct_bitmap_bg_ptr.
- **set** `static void set(const fixed_point& top_left_position, sp_direct_bitmap_bg_ptr& bg)` (L492): Sets the top-left position of the given sp_direct_bitmap_bg_ptr.
### `bn::sp_direct_bitmap_bg_top_left_move_to_action` (class, bn_sp_direct_bitmap_bg_actions.h:505)

Modifies the top-left position of a sp_direct_bitmap_bg_ptr until it has a given state.

`class sp_direct_bitmap_bg_top_left_move_to_action : public to_value_template_action<sp_direct_bitmap_bg_ptr, fixed_point, sp_direct_bitmap_bg_top_left_position_manager>`

- **sp_direct_bitmap_bg_top_left_move_to_action** `sp_direct_bitmap_bg_top_left_move_to_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : to_value_template_action(bg, duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L520): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the top-left position of the given sp_direct_bitmap_bg_ptr is equal to (final_x, final_y).; `final_top_left_x` Horizontal top-left position of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.; `final_top_left_y` Vertical top-left position of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_top_left_move_to_action** `sp_direct_bitmap_bg_top_left_move_to_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : to_value_template_action(move(bg), duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L536): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the top-left position of the given sp_direct_bitmap_bg_ptr is equal to (final_x, final_y).; `final_top_left_x` Horizontal top-left position of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.; `final_top_left_y` Vertical top-left position of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_top_left_move_to_action** `sp_direct_bitmap_bg_top_left_move_to_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_top_left_position) : to_value_template_action(bg, duration_updates, final_top_left_position)` (L550): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the top-left position of the given sp_direct_bitmap_bg_ptr is equal to final_top_left_position.; `final_top_left_position` Top-left position of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_top_left_move_to_action** `sp_direct_bitmap_bg_top_left_move_to_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_top_left_position) : to_value_template_action(move(bg), duration_updates, final_top_left_position)` (L564): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the top-left position of the given sp_direct_bitmap_bg_ptr is equal to final_top_left_position.; `final_top_left_position` Top-left position of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L573): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_top_left_position** `const fixed_point& final_top_left_position() const` (L582): Returns the top-left position of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_top_left_move_loop_action` (class, bn_sp_direct_bitmap_bg_actions.h:596)

Modifies the top-left position of a sp_direct_bitmap_bg_ptr from a minimum to a maximum. When the top-left position is equal to the given final state, it goes back to its initial state and vice versa.

`class sp_direct_bitmap_bg_top_left_move_loop_action : public loop_value_template_action<sp_direct_bitmap_bg_ptr, fixed_point, sp_direct_bitmap_bg_top_left_position_manager>`

- **sp_direct_bitmap_bg_top_left_move_loop_action** `sp_direct_bitmap_bg_top_left_move_loop_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : loop_value_template_action(bg, duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L611): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_x` When the horizontal top-left position of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_top_left_y` When the vertical top-left position of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sp_direct_bitmap_bg_top_left_move_loop_action** `sp_direct_bitmap_bg_top_left_move_loop_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : loop_value_template_action(move(bg), duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L627): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_x` When the horizontal top-left position of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_top_left_y` When the vertical top-left position of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sp_direct_bitmap_bg_top_left_move_loop_action** `sp_direct_bitmap_bg_top_left_move_loop_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_top_left_position) : loop_value_template_action(bg, duration_updates, final_top_left_position)` (L641): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_position` When the top-left position of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sp_direct_bitmap_bg_top_left_move_loop_action** `sp_direct_bitmap_bg_top_left_move_loop_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_top_left_position) : loop_value_template_action(move(bg), duration_updates, final_top_left_position)` (L655): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_position` When the top-left position of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L664): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_top_left_position** `const fixed_point& final_top_left_position() const` (L673): When the top-left position of the given sp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sp_direct_bitmap_bg_top_left_move_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:686)

Changes the top-left position of a sp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class sp_direct_bitmap_bg_top_left_move_toggle_action : public toggle_value_template_action<sp_direct_bitmap_bg_ptr, fixed_point, sp_direct_bitmap_bg_top_left_position_manager>`

- **sp_direct_bitmap_bg_top_left_move_toggle_action** `sp_direct_bitmap_bg_top_left_move_toggle_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) : toggle_value_template_action(bg, duration_updates, fixed_point(new_top_left_x, new_top_left_y))` (L699): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the top-left position of the given sp_direct_bitmap_bg_ptr.; `new_top_left_x` New horizontal top-left position when the action is updated duration_updates times.; `new_top_left_y` New vertical top-left position when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_top_left_move_toggle_action** `sp_direct_bitmap_bg_top_left_move_toggle_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) : toggle_value_template_action(move(bg), duration_updates, fixed_point(new_top_left_x, new_top_left_y))` (L713): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the top-left position of the given sp_direct_bitmap_bg_ptr.; `new_top_left_x` New horizontal top-left position when the action is updated duration_updates times.; `new_top_left_y` New vertical top-left position when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_top_left_move_toggle_action** `sp_direct_bitmap_bg_top_left_move_toggle_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& new_top_left_position) : toggle_value_template_action(bg, duration_updates, new_top_left_position)` (L726): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the top-left position of the given sp_direct_bitmap_bg_ptr.; `new_top_left_position` New top-left position when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_top_left_move_toggle_action** `sp_direct_bitmap_bg_top_left_move_toggle_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& new_top_left_position) : toggle_value_template_action(move(bg), duration_updates, new_top_left_position)` (L739): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the top-left position of the given sp_direct_bitmap_bg_ptr.; `new_top_left_position` New top-left position when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L748): Returns the sp_direct_bitmap_bg_ptr to modify.
- **new_top_left_position** `const fixed_point& new_top_left_position() const` (L757): Returns the top-left position of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_rotation_manager` (class, bn_sp_direct_bitmap_bg_actions.h:772)

Manages the rotation angle of a sp_direct_bitmap_bg_ptr.

`class sp_direct_bitmap_bg_rotation_manager`

- **get** `static fixed get(const sp_direct_bitmap_bg_ptr& bg)` (L779): Returns the rotation angle of the given sp_direct_bitmap_bg_ptr.
- **set** `static void set(fixed rotation_angle, sp_direct_bitmap_bg_ptr& bg)` (L789): Sets the rotation angle of the given sp_direct_bitmap_bg_ptr.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].; `bg` sp_direct_bitmap_bg_ptr to modify.
### `bn::sp_direct_bitmap_bg_rotate_by_action` (class, bn_sp_direct_bitmap_bg_actions.h:803)

Modifies the rotation angle of a sp_direct_bitmap_bg_ptr by delta_rotation_angle. When the rotation angle is over 360, it goes back to 0 and vice versa.

`class sp_direct_bitmap_bg_rotate_by_action : public cyclic_by_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_rotation_manager>`

- **sp_direct_bitmap_bg_rotate_by_action** `sp_direct_bitmap_bg_rotate_by_action(const sp_direct_bitmap_bg_ptr& bg, fixed delta_rotation_angle) : cyclic_by_value_template_action(bg, delta_rotation_angle, 0, 360)` (L816): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `delta_rotation_angle` How many degrees to add to the rotation angle of the given sp_direct_bitmap_bg_ptr when the action is updated.
  This rotation angle must be in the range [0..360].
- **sp_direct_bitmap_bg_rotate_by_action** `sp_direct_bitmap_bg_rotate_by_action(sp_direct_bitmap_bg_ptr&& bg, fixed delta_rotation_angle) : cyclic_by_value_template_action(move(bg), delta_rotation_angle, 0, 360)` (L831): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `delta_rotation_angle` How many degrees to add to the rotation angle of the given sp_direct_bitmap_bg_ptr when the action is updated.
  This rotation angle must be in the range [0..360].
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L841): Returns the sp_direct_bitmap_bg_ptr to modify.
- **delta_rotation_angle** `fixed delta_rotation_angle() const` (L850): Returns how many degrees to add to the rotation angle of the given sp_direct_bitmap_bg_ptr when the action is updated.
### `bn::sp_direct_bitmap_bg_rotate_to_action` (class, bn_sp_direct_bitmap_bg_actions.h:863)

Modifies the rotation angle of a sp_direct_bitmap_bg_ptr until it has a given state.

`class sp_direct_bitmap_bg_rotate_to_action : public to_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_rotation_manager>`

- **sp_direct_bitmap_bg_rotate_to_action** `sp_direct_bitmap_bg_rotate_to_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_rotation_angle) : to_value_template_action(bg, duration_updates, final_rotation_angle)` (L877): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the rotation angle of the given sp_direct_bitmap_bg_ptr is equal to final_rotation_angle.; `final_rotation_angle` Rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **sp_direct_bitmap_bg_rotate_to_action** `sp_direct_bitmap_bg_rotate_to_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_rotation_angle) : to_value_template_action(move(bg), duration_updates, final_rotation_angle)` (L894): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the rotation angle of the given sp_direct_bitmap_bg_ptr is equal to final_rotation_angle.; `final_rotation_angle` Rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L905): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_rotation_angle** `fixed final_rotation_angle() const` (L914): Returns the rotation angle of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_rotate_loop_action` (class, bn_sp_direct_bitmap_bg_actions.h:928)

Modifies the rotation angle of a sp_direct_bitmap_bg_ptr from a minimum to a maximum. When the rotation angle is equal to the given final state, it goes back to its initial state and vice versa.

`class sp_direct_bitmap_bg_rotate_loop_action : public loop_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_rotation_manager>`

- **sp_direct_bitmap_bg_rotate_loop_action** `sp_direct_bitmap_bg_rotate_loop_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_rotation_angle) : loop_value_template_action(bg, duration_updates, final_rotation_angle)` (L943): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the rotation angle delta.; `final_rotation_angle` When the rotation angle of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
  This rotation angle must be in the range [0..360].
- **sp_direct_bitmap_bg_rotate_loop_action** `sp_direct_bitmap_bg_rotate_loop_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_rotation_angle) : loop_value_template_action(move(bg), duration_updates, final_rotation_angle)` (L961): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the rotation angle delta.; `final_rotation_angle` When the rotation angle of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
  This rotation angle must be in the range [0..360].
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L972): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_rotation_angle** `fixed final_rotation_angle() const` (L981): When the rotation angle of the given sp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sp_direct_bitmap_bg_rotate_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:994)

Changes the rotation angle of a sp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class sp_direct_bitmap_bg_rotate_toggle_action : public toggle_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_rotation_manager>`

- **sp_direct_bitmap_bg_rotate_toggle_action** `sp_direct_bitmap_bg_rotate_toggle_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_rotation_angle) : toggle_value_template_action(bg, duration_updates, new_rotation_angle)` (L1008): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the rotation angle of the given sp_direct_bitmap_bg_ptr.; `new_rotation_angle` New rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **sp_direct_bitmap_bg_rotate_toggle_action** `sp_direct_bitmap_bg_rotate_toggle_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_rotation_angle) : toggle_value_template_action(move(bg), duration_updates, new_rotation_angle)` (L1025): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the rotation angle of the given sp_direct_bitmap_bg_ptr.; `new_rotation_angle` New rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L1036): Returns the sp_direct_bitmap_bg_ptr to modify.
- **new_rotation_angle** `fixed new_rotation_angle() const` (L1045): Returns the rotation angle of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_horizontal_scale_manager` (class, bn_sp_direct_bitmap_bg_actions.h:1060)

Manages the horizontal scale of a sp_direct_bitmap_bg_ptr.

`class sp_direct_bitmap_bg_horizontal_scale_manager`

- **get** `static fixed get(const sp_direct_bitmap_bg_ptr& bg)` (L1067): Returns the horizontal scale of the given sp_direct_bitmap_bg_ptr.
- **set** `static void set(fixed horizontal_scale, sp_direct_bitmap_bg_ptr& bg)` (L1075): Sets the horizontal scale of the given sp_direct_bitmap_bg_ptr.
### `bn::sp_direct_bitmap_bg_horizontal_scale_to_action` (class, bn_sp_direct_bitmap_bg_actions.h:1088)

Modifies the horizontal scale of a sp_direct_bitmap_bg_ptr until it has a given state.

`class sp_direct_bitmap_bg_horizontal_scale_to_action : public to_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_horizontal_scale_manager>`

- **sp_direct_bitmap_bg_horizontal_scale_to_action** `sp_direct_bitmap_bg_horizontal_scale_to_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_horizontal_scale) : to_value_template_action(bg, duration_updates, final_horizontal_scale)` (L1100): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the horizontal scale of the given sp_direct_bitmap_bg_ptr is equal to final_horizontal_scale.; `final_horizontal_scale` Horizontal scale when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_horizontal_scale_to_action** `sp_direct_bitmap_bg_horizontal_scale_to_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_horizontal_scale) : to_value_template_action(move(bg), duration_updates, final_horizontal_scale)` (L1114): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the horizontal scale of the given sp_direct_bitmap_bg_ptr is equal to final_horizontal_scale.; `final_horizontal_scale` Horizontal scale when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L1124): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_horizontal_scale** `fixed final_horizontal_scale() const` (L1133): Returns the horizontal scale of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_horizontal_scale_loop_action` (class, bn_sp_direct_bitmap_bg_actions.h:1147)

Modifies the horizontal scale of a sp_direct_bitmap_bg_ptr from a minimum to a maximum. When the horizontal scale is equal to the given final state, it goes back to its initial state and vice versa.

`class sp_direct_bitmap_bg_horizontal_scale_loop_action : public loop_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_horizontal_scale_manager>`

- **sp_direct_bitmap_bg_horizontal_scale_loop_action** `sp_direct_bitmap_bg_horizontal_scale_loop_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_horizontal_scale) : loop_value_template_action(bg, duration_updates, final_horizontal_scale)` (L1160): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal scale delta.; `final_horizontal_scale` When the horizontal scale of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sp_direct_bitmap_bg_horizontal_scale_loop_action** `sp_direct_bitmap_bg_horizontal_scale_loop_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_horizontal_scale) : loop_value_template_action(move(bg), duration_updates, final_horizontal_scale)` (L1175): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal scale delta.; `final_horizontal_scale` When the horizontal scale of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L1185): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_horizontal_scale** `fixed final_horizontal_scale() const` (L1194): When the horizontal scale of the given sp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sp_direct_bitmap_bg_horizontal_scale_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:1208)

Changes the horizontal scale of a sp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class sp_direct_bitmap_bg_horizontal_scale_toggle_action : public toggle_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_horizontal_scale_manager>`

- **sp_direct_bitmap_bg_horizontal_scale_toggle_action** `sp_direct_bitmap_bg_horizontal_scale_toggle_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_horizontal_scale) : toggle_value_template_action(bg, duration_updates, new_horizontal_scale)` (L1220): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the horizontal scale of the given sp_direct_bitmap_bg_ptr.; `new_horizontal_scale` New horizontal scale when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_horizontal_scale_toggle_action** `sp_direct_bitmap_bg_horizontal_scale_toggle_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_horizontal_scale) : toggle_value_template_action(move(bg), duration_updates, new_horizontal_scale)` (L1234): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the horizontal scale of the given sp_direct_bitmap_bg_ptr.; `new_horizontal_scale` New horizontal scale when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L1244): Returns the sp_direct_bitmap_bg_ptr to modify.
- **new_horizontal_scale** `fixed new_horizontal_scale() const` (L1253): Returns the horizontal scale of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_vertical_scale_manager` (class, bn_sp_direct_bitmap_bg_actions.h:1268)

Manages the vertical scale of a sp_direct_bitmap_bg_ptr.

`class sp_direct_bitmap_bg_vertical_scale_manager`

- **get** `static fixed get(const sp_direct_bitmap_bg_ptr& bg)` (L1275): Returns the vertical scale of the given sp_direct_bitmap_bg_ptr.
- **set** `static void set(fixed vertical_scale, sp_direct_bitmap_bg_ptr& bg)` (L1283): Sets the vertical scale of the given sp_direct_bitmap_bg_ptr.
### `bn::sp_direct_bitmap_bg_vertical_scale_to_action` (class, bn_sp_direct_bitmap_bg_actions.h:1296)

Modifies the vertical scale of a sp_direct_bitmap_bg_ptr until it has a given state.

`class sp_direct_bitmap_bg_vertical_scale_to_action : public to_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_vertical_scale_manager>`

- **sp_direct_bitmap_bg_vertical_scale_to_action** `sp_direct_bitmap_bg_vertical_scale_to_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_vertical_scale) : to_value_template_action(bg, duration_updates, final_vertical_scale)` (L1308): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the vertical scale of the given sp_direct_bitmap_bg_ptr is equal to final_vertical_scale.; `final_vertical_scale` Vertical scale when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_vertical_scale_to_action** `sp_direct_bitmap_bg_vertical_scale_to_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_vertical_scale) : to_value_template_action(move(bg), duration_updates, final_vertical_scale)` (L1322): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the vertical scale of the given sp_direct_bitmap_bg_ptr is equal to final_vertical_scale.; `final_vertical_scale` Vertical scale when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L1332): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_vertical_scale** `fixed final_vertical_scale() const` (L1341): Returns the vertical scale of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_vertical_scale_loop_action` (class, bn_sp_direct_bitmap_bg_actions.h:1355)

Modifies the vertical scale of a sp_direct_bitmap_bg_ptr from a minimum to a maximum. When the vertical scale is equal to the given final state, it goes back to its initial state and vice versa.

`class sp_direct_bitmap_bg_vertical_scale_loop_action : public loop_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_vertical_scale_manager>`

- **sp_direct_bitmap_bg_vertical_scale_loop_action** `sp_direct_bitmap_bg_vertical_scale_loop_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_vertical_scale) : loop_value_template_action(bg, duration_updates, final_vertical_scale)` (L1368): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical scale delta.; `final_vertical_scale` When the vertical scale of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sp_direct_bitmap_bg_vertical_scale_loop_action** `sp_direct_bitmap_bg_vertical_scale_loop_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_vertical_scale) : loop_value_template_action(move(bg), duration_updates, final_vertical_scale)` (L1383): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical scale delta.; `final_vertical_scale` When the vertical scale of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L1393): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_vertical_scale** `fixed final_vertical_scale() const` (L1402): When the vertical scale of the given sp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sp_direct_bitmap_bg_vertical_scale_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:1416)

Changes the vertical scale of a sp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class sp_direct_bitmap_bg_vertical_scale_toggle_action : public toggle_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_vertical_scale_manager>`

- **sp_direct_bitmap_bg_vertical_scale_toggle_action** `sp_direct_bitmap_bg_vertical_scale_toggle_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_vertical_scale) : toggle_value_template_action(bg, duration_updates, new_vertical_scale)` (L1428): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the vertical scale of the given sp_direct_bitmap_bg_ptr.; `new_vertical_scale` New vertical scale when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_vertical_scale_toggle_action** `sp_direct_bitmap_bg_vertical_scale_toggle_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_vertical_scale) : toggle_value_template_action(move(bg), duration_updates, new_vertical_scale)` (L1442): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the vertical scale of the given sp_direct_bitmap_bg_ptr.; `new_vertical_scale` New vertical scale when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L1452): Returns the sp_direct_bitmap_bg_ptr to modify.
- **new_vertical_scale** `fixed new_vertical_scale() const` (L1461): Returns the vertical scale of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_scale_manager` (class, bn_sp_direct_bitmap_bg_actions.h:1476)

Manages the scale of a sp_direct_bitmap_bg_ptr.

`class sp_direct_bitmap_bg_scale_manager`

- **get** `static fixed get(const sp_direct_bitmap_bg_ptr& bg)` (L1483): Returns the horizontal scale of the given sp_direct_bitmap_bg_ptr.
- **set** `static void set(fixed scale, sp_direct_bitmap_bg_ptr& bg)` (L1491): Sets the scale of the given sp_direct_bitmap_bg_ptr.
### `bn::sp_direct_bitmap_bg_scale_to_action` (class, bn_sp_direct_bitmap_bg_actions.h:1504)

Modifies the scale of a sp_direct_bitmap_bg_ptr until it has a given state.

`class sp_direct_bitmap_bg_scale_to_action : public to_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_scale_manager>`

- **sp_direct_bitmap_bg_scale_to_action** `sp_direct_bitmap_bg_scale_to_action(const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_scale) : to_value_template_action(bg, duration_updates, final_scale)` (L1516): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the scale of the given sp_direct_bitmap_bg_ptr is equal to final_scale.; `final_scale` scale when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_scale_to_action** `sp_direct_bitmap_bg_scale_to_action(sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_scale) : to_value_template_action(move(bg), duration_updates, final_scale)` (L1529): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the scale of the given sp_direct_bitmap_bg_ptr is equal to final_scale.; `final_scale` Scale when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L1538): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_scale** `fixed final_scale() const` (L1547): Returns the scale of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_scale_loop_action` (class, bn_sp_direct_bitmap_bg_actions.h:1561)

Modifies the scale of a sp_direct_bitmap_bg_ptr from a minimum to a maximum. When the scale is equal to the given final state, it goes back to its initial state and vice versa.

`class sp_direct_bitmap_bg_scale_loop_action : public loop_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_scale_manager>`

- **sp_direct_bitmap_bg_scale_loop_action** `sp_direct_bitmap_bg_scale_loop_action(const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_scale) : loop_value_template_action(bg, duration_updates, final_scale)` (L1574): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the scale delta.; `final_scale` When the scale of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sp_direct_bitmap_bg_scale_loop_action** `sp_direct_bitmap_bg_scale_loop_action(sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_scale) : loop_value_template_action(move(bg), duration_updates, final_scale)` (L1588): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the scale delta.; `final_scale` When the scale of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L1597): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_scale** `fixed final_scale() const` (L1606): When the scale of the given sp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sp_direct_bitmap_bg_scale_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:1620)

Changes the scale of a sp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class sp_direct_bitmap_bg_scale_toggle_action : public toggle_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_scale_manager>`

- **sp_direct_bitmap_bg_scale_toggle_action** `sp_direct_bitmap_bg_scale_toggle_action(const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_scale) : toggle_value_template_action(bg, duration_updates, new_scale)` (L1632): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the scale of the given sp_direct_bitmap_bg_ptr.; `new_scale` New scale when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_scale_toggle_action** `sp_direct_bitmap_bg_scale_toggle_action(sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_scale) : toggle_value_template_action(move(bg), duration_updates, new_scale)` (L1645): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the scale of the given sp_direct_bitmap_bg_ptr.; `new_scale` New scale when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L1654): Returns the sp_direct_bitmap_bg_ptr to modify.
- **new_scale** `fixed new_scale() const` (L1663): Returns the scale of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_horizontal_shear_manager` (class, bn_sp_direct_bitmap_bg_actions.h:1678)

Manages the horizontal shear of a sp_direct_bitmap_bg_ptr.

`class sp_direct_bitmap_bg_horizontal_shear_manager`

- **get** `static fixed get(const sp_direct_bitmap_bg_ptr& bg)` (L1685): Returns the horizontal shear of the given sp_direct_bitmap_bg_ptr.
- **set** `static void set(fixed horizontal_shear, sp_direct_bitmap_bg_ptr& bg)` (L1693): Sets the horizontal shear of the given sp_direct_bitmap_bg_ptr.
### `bn::sp_direct_bitmap_bg_horizontal_shear_to_action` (class, bn_sp_direct_bitmap_bg_actions.h:1706)

Modifies the horizontal shear of a sp_direct_bitmap_bg_ptr until it has a given state.

`class sp_direct_bitmap_bg_horizontal_shear_to_action : public to_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_horizontal_shear_manager>`

- **sp_direct_bitmap_bg_horizontal_shear_to_action** `sp_direct_bitmap_bg_horizontal_shear_to_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_horizontal_shear) : to_value_template_action(bg, duration_updates, final_horizontal_shear)` (L1718): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the horizontal shear of the given sp_direct_bitmap_bg_ptr is equal to final_horizontal_shear.; `final_horizontal_shear` Horizontal shear when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_horizontal_shear_to_action** `sp_direct_bitmap_bg_horizontal_shear_to_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_horizontal_shear) : to_value_template_action(move(bg), duration_updates, final_horizontal_shear)` (L1731): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the horizontal shear of the given sp_direct_bitmap_bg_ptr is equal to final_horizontal_shear.; `final_horizontal_shear` Horizontal shear when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L1740): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_horizontal_shear** `fixed final_horizontal_shear() const` (L1749): Returns the horizontal shear of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_horizontal_shear_loop_action` (class, bn_sp_direct_bitmap_bg_actions.h:1763)

Modifies the horizontal shear of a sp_direct_bitmap_bg_ptr from a minimum to a maximum. When the horizontal shear is equal to the given final state, it goes back to its initial state and vice versa.

`class sp_direct_bitmap_bg_horizontal_shear_loop_action : public loop_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_horizontal_shear_manager>`

- **sp_direct_bitmap_bg_horizontal_shear_loop_action** `sp_direct_bitmap_bg_horizontal_shear_loop_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_horizontal_shear) : loop_value_template_action(bg, duration_updates, final_horizontal_shear)` (L1776): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal shear delta.; `final_horizontal_shear` When the horizontal shear of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sp_direct_bitmap_bg_horizontal_shear_loop_action** `sp_direct_bitmap_bg_horizontal_shear_loop_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_horizontal_shear) : loop_value_template_action(move(bg), duration_updates, final_horizontal_shear)` (L1790): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal shear delta.; `final_horizontal_shear` When the horizontal shear of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L1799): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_horizontal_shear** `fixed final_horizontal_shear() const` (L1808): When the horizontal shear of the given sp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sp_direct_bitmap_bg_horizontal_shear_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:1822)

Changes the horizontal shear of a sp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class sp_direct_bitmap_bg_horizontal_shear_toggle_action : public toggle_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_horizontal_shear_manager>`

- **sp_direct_bitmap_bg_horizontal_shear_toggle_action** `sp_direct_bitmap_bg_horizontal_shear_toggle_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_horizontal_shear) : toggle_value_template_action(bg, duration_updates, new_horizontal_shear)` (L1834): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the horizontal shear of the given sp_direct_bitmap_bg_ptr.; `new_horizontal_shear` New horizontal shear when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_horizontal_shear_toggle_action** `sp_direct_bitmap_bg_horizontal_shear_toggle_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_horizontal_shear) : toggle_value_template_action(move(bg), duration_updates, new_horizontal_shear)` (L1847): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the horizontal shear of the given sp_direct_bitmap_bg_ptr.; `new_horizontal_shear` New horizontal shear when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L1856): Returns the sp_direct_bitmap_bg_ptr to modify.
- **new_horizontal_shear** `fixed new_horizontal_shear() const` (L1865): Returns the horizontal shear of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_vertical_shear_manager` (class, bn_sp_direct_bitmap_bg_actions.h:1880)

Manages the vertical shear of a sp_direct_bitmap_bg_ptr.

`class sp_direct_bitmap_bg_vertical_shear_manager`

- **get** `static fixed get(const sp_direct_bitmap_bg_ptr& bg)` (L1887): Returns the vertical shear of the given sp_direct_bitmap_bg_ptr.
- **set** `static void set(fixed vertical_shear, sp_direct_bitmap_bg_ptr& bg)` (L1895): Sets the vertical shear of the given sp_direct_bitmap_bg_ptr.
### `bn::sp_direct_bitmap_bg_vertical_shear_to_action` (class, bn_sp_direct_bitmap_bg_actions.h:1908)

Modifies the vertical shear of a sp_direct_bitmap_bg_ptr until it has a given state.

`class sp_direct_bitmap_bg_vertical_shear_to_action : public to_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_vertical_shear_manager>`

- **sp_direct_bitmap_bg_vertical_shear_to_action** `sp_direct_bitmap_bg_vertical_shear_to_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_vertical_shear) : to_value_template_action(bg, duration_updates, final_vertical_shear)` (L1920): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the vertical shear of the given sp_direct_bitmap_bg_ptr is equal to final_vertical_shear.; `final_vertical_shear` Vertical shear when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_vertical_shear_to_action** `sp_direct_bitmap_bg_vertical_shear_to_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_vertical_shear) : to_value_template_action(move(bg), duration_updates, final_vertical_shear)` (L1933): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the vertical shear of the given sp_direct_bitmap_bg_ptr is equal to final_vertical_shear.; `final_vertical_shear` Vertical shear when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L1942): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_vertical_shear** `fixed final_vertical_shear() const` (L1951): Returns the vertical shear of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_vertical_shear_loop_action` (class, bn_sp_direct_bitmap_bg_actions.h:1965)

Modifies the vertical shear of a sp_direct_bitmap_bg_ptr from a minimum to a maximum. When the vertical shear is equal to the given final state, it goes back to its initial state and vice versa.

`class sp_direct_bitmap_bg_vertical_shear_loop_action : public loop_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_vertical_shear_manager>`

- **sp_direct_bitmap_bg_vertical_shear_loop_action** `sp_direct_bitmap_bg_vertical_shear_loop_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_vertical_shear) : loop_value_template_action(bg, duration_updates, final_vertical_shear)` (L1978): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical shear delta.; `final_vertical_shear` When the vertical shear of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sp_direct_bitmap_bg_vertical_shear_loop_action** `sp_direct_bitmap_bg_vertical_shear_loop_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_vertical_shear) : loop_value_template_action(move(bg), duration_updates, final_vertical_shear)` (L1992): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical shear delta.; `final_vertical_shear` When the vertical shear of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L2001): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_vertical_shear** `fixed final_vertical_shear() const` (L2010): When the vertical shear of the given sp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sp_direct_bitmap_bg_vertical_shear_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:2024)

Changes the vertical shear of a sp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class sp_direct_bitmap_bg_vertical_shear_toggle_action : public toggle_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_vertical_shear_manager>`

- **sp_direct_bitmap_bg_vertical_shear_toggle_action** `sp_direct_bitmap_bg_vertical_shear_toggle_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_vertical_shear) : toggle_value_template_action(bg, duration_updates, new_vertical_shear)` (L2036): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the vertical shear of the given sp_direct_bitmap_bg_ptr.; `new_vertical_shear` New vertical shear when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_vertical_shear_toggle_action** `sp_direct_bitmap_bg_vertical_shear_toggle_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_vertical_shear) : toggle_value_template_action(move(bg), duration_updates, new_vertical_shear)` (L2049): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the vertical shear of the given sp_direct_bitmap_bg_ptr.; `new_vertical_shear` New vertical shear when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L2058): Returns the sp_direct_bitmap_bg_ptr to modify.
- **new_vertical_shear** `fixed new_vertical_shear() const` (L2067): Returns the vertical shear of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_shear_manager` (class, bn_sp_direct_bitmap_bg_actions.h:2082)

Manages the shear of a sp_direct_bitmap_bg_ptr.

`class sp_direct_bitmap_bg_shear_manager`

- **get** `static fixed get(const sp_direct_bitmap_bg_ptr& bg)` (L2089): Returns the horizontal shear of the given sp_direct_bitmap_bg_ptr.
- **set** `static void set(fixed shear, sp_direct_bitmap_bg_ptr& bg)` (L2097): Sets the shear of the given sp_direct_bitmap_bg_ptr.
### `bn::sp_direct_bitmap_bg_shear_to_action` (class, bn_sp_direct_bitmap_bg_actions.h:2110)

Modifies the shear of a sp_direct_bitmap_bg_ptr until it has a given state.

`class sp_direct_bitmap_bg_shear_to_action : public to_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_shear_manager>`

- **sp_direct_bitmap_bg_shear_to_action** `sp_direct_bitmap_bg_shear_to_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_shear) : to_value_template_action(bg, duration_updates, final_shear)` (L2122): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the shear of the given sp_direct_bitmap_bg_ptr is equal to final_shear.; `final_shear` Shear when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_shear_to_action** `sp_direct_bitmap_bg_shear_to_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_shear) : to_value_template_action(move(bg), duration_updates, final_shear)` (L2135): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the shear of the given sp_direct_bitmap_bg_ptr is equal to final_shear.; `final_shear` Shear when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L2144): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_shear** `fixed final_shear() const` (L2153): Returns the shear of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_shear_loop_action` (class, bn_sp_direct_bitmap_bg_actions.h:2167)

Modifies the shear of a sp_direct_bitmap_bg_ptr from a minimum to a maximum. When the shear is equal to the given final state, it goes back to its initial state and vice versa.

`class sp_direct_bitmap_bg_shear_loop_action : public loop_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_shear_manager>`

- **sp_direct_bitmap_bg_shear_loop_action** `sp_direct_bitmap_bg_shear_loop_action(const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_shear) : loop_value_template_action(bg, duration_updates, final_shear)` (L2180): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the shear delta.; `final_shear` When the shear of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sp_direct_bitmap_bg_shear_loop_action** `sp_direct_bitmap_bg_shear_loop_action(sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_shear) : loop_value_template_action(move(bg), duration_updates, final_shear)` (L2193): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the shear delta.; `final_shear` When the shear of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L2201): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_shear** `fixed final_shear() const` (L2210): When the shear of the given sp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sp_direct_bitmap_bg_shear_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:2223)

Changes the shear of a sp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class sp_direct_bitmap_bg_shear_toggle_action : public toggle_value_template_action<sp_direct_bitmap_bg_ptr, fixed, sp_direct_bitmap_bg_shear_manager>`

- **sp_direct_bitmap_bg_shear_toggle_action** `sp_direct_bitmap_bg_shear_toggle_action(const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_shear) : toggle_value_template_action(bg, duration_updates, new_shear)` (L2235): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the shear of the given sp_direct_bitmap_bg_ptr.; `new_shear` New shear when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_shear_toggle_action** `sp_direct_bitmap_bg_shear_toggle_action(sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_shear) : toggle_value_template_action(move(bg), duration_updates, new_shear)` (L2247): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the shear of the given sp_direct_bitmap_bg_ptr.; `new_shear` New shear when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L2255): Returns the sp_direct_bitmap_bg_ptr to modify.
- **new_shear** `fixed new_shear() const` (L2264): Returns the shear of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_horizontal_flip_manager` (class, bn_sp_direct_bitmap_bg_actions.h:2279)

Manages if a sp_direct_bitmap_bg_ptr is flipped in its horizontal axis or not.

`class sp_direct_bitmap_bg_horizontal_flip_manager`

- **get** `static bool get(const sp_direct_bitmap_bg_ptr& bg)` (L2286): Indicates if the given sp_direct_bitmap_bg_ptr is flipped in the horizontal axis or not.
- **set** `static void set(bool horizontal_flip, sp_direct_bitmap_bg_ptr& bg)` (L2294): Sets if the given sp_direct_bitmap_bg_ptr must be flipped in the horizontal axis or not.
### `bn::sp_direct_bitmap_bg_horizontal_flip_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:2308)

Toggles if a sp_direct_bitmap_bg_ptr must be flipped in the horizontal axis or not when the action is updated a given number of times.

`class sp_direct_bitmap_bg_horizontal_flip_toggle_action : public bool_toggle_value_template_action<sp_direct_bitmap_bg_ptr, sp_direct_bitmap_bg_horizontal_flip_manager>`

- **sp_direct_bitmap_bg_horizontal_flip_toggle_action** `sp_direct_bitmap_bg_horizontal_flip_toggle_action(const sp_direct_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2319): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given sp_direct_bitmap_bg_ptr must be flipped in the horizontal axis or not.
- **sp_direct_bitmap_bg_horizontal_flip_toggle_action** `sp_direct_bitmap_bg_horizontal_flip_toggle_action(sp_direct_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2330): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given sp_direct_bitmap_bg_ptr must be flipped in the horizontal axis or not.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L2338): Returns the sp_direct_bitmap_bg_ptr to modify.
### `bn::sp_direct_bitmap_bg_vertical_flip_manager` (class, bn_sp_direct_bitmap_bg_actions.h:2353)

Manages if a sp_direct_bitmap_bg_ptr is flipped in its vertical axis or not.

`class sp_direct_bitmap_bg_vertical_flip_manager`

- **get** `static bool get(const sp_direct_bitmap_bg_ptr& bg)` (L2360): Indicates if the given sp_direct_bitmap_bg_ptr is flipped in the vertical axis or not.
- **set** `static void set(bool vertical_flip, sp_direct_bitmap_bg_ptr& bg)` (L2368): Sets if the given sp_direct_bitmap_bg_ptr must be flipped in the vertical axis or not.
### `bn::sp_direct_bitmap_bg_vertical_flip_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:2382)

Toggles if a sp_direct_bitmap_bg_ptr must be flipped in the vertical axis or not when the action is updated a given number of times.

`class sp_direct_bitmap_bg_vertical_flip_toggle_action : public bool_toggle_value_template_action<sp_direct_bitmap_bg_ptr, sp_direct_bitmap_bg_vertical_flip_manager>`

- **sp_direct_bitmap_bg_vertical_flip_toggle_action** `sp_direct_bitmap_bg_vertical_flip_toggle_action(const sp_direct_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2393): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given sp_direct_bitmap_bg_ptr must be flipped in the vertical axis or not.
- **sp_direct_bitmap_bg_vertical_flip_toggle_action** `sp_direct_bitmap_bg_vertical_flip_toggle_action(sp_direct_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2404): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given sp_direct_bitmap_bg_ptr must be flipped in the vertical axis or not.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L2412): Returns the sp_direct_bitmap_bg_ptr to modify.
### `bn::sp_direct_bitmap_bg_pivot_position_manager` (class, bn_sp_direct_bitmap_bg_actions.h:2427)

Manages the position of the pivot of a sp_direct_bitmap_bg_ptr.

`class sp_direct_bitmap_bg_pivot_position_manager`

- **get** `static const fixed_point& get(const sp_direct_bitmap_bg_ptr& bg)` (L2434): Returns the position of the pivot of the given sp_direct_bitmap_bg_ptr.
- **set** `static void set(const fixed_point& position, sp_direct_bitmap_bg_ptr& bg)` (L2442): Sets the position of the pivot of the given sp_direct_bitmap_bg_ptr.
### `bn::sp_direct_bitmap_bg_move_pivot_by_action` (class, bn_sp_direct_bitmap_bg_actions.h:2455)

Modifies the position of the pivot of a sp_direct_bitmap_bg_ptr by a given delta.

`class sp_direct_bitmap_bg_move_pivot_by_action : public by_value_template_action<sp_direct_bitmap_bg_ptr, fixed_point, sp_direct_bitmap_bg_pivot_position_manager>`

- **sp_direct_bitmap_bg_move_pivot_by_action** `sp_direct_bitmap_bg_move_pivot_by_action(const sp_direct_bitmap_bg_ptr& bg, fixed delta_x, fixed delta_y) : by_value_template_action(bg, fixed_point(delta_x, delta_y))` (L2468): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `delta_x` How much to add to the horizontal position of the pivot of the given sp_direct_bitmap_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the pivot of the given sp_direct_bitmap_bg_ptr when the action is updated.
- **sp_direct_bitmap_bg_move_pivot_by_action** `sp_direct_bitmap_bg_move_pivot_by_action(sp_direct_bitmap_bg_ptr&& bg, fixed delta_x, fixed delta_y) : by_value_template_action(move(bg), fixed_point(delta_x, delta_y))` (L2481): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `delta_x` How much to add to the horizontal position of the pivot of the given sp_direct_bitmap_bg_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the pivot of the given sp_direct_bitmap_bg_ptr when the action is updated.
- **sp_direct_bitmap_bg_move_pivot_by_action** `sp_direct_bitmap_bg_move_pivot_by_action(const sp_direct_bitmap_bg_ptr& bg, const fixed_point& delta_position) : by_value_template_action(bg, delta_position)` (L2492): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `delta_position` How much to add to the position of the pivot of the given sp_direct_bitmap_bg_ptr when the action is updated.
- **sp_direct_bitmap_bg_move_pivot_by_action** `sp_direct_bitmap_bg_move_pivot_by_action(sp_direct_bitmap_bg_ptr&& bg, const fixed_point& delta_position) : by_value_template_action(move(bg), delta_position)` (L2503): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `delta_position` How much to add to the position of the pivot of the given sp_direct_bitmap_bg_ptr when the action is updated.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L2511): Returns the sp_direct_bitmap_bg_ptr to modify.
- **delta_position** `const fixed_point& delta_position() const` (L2520): Returns how much to add to the position of the pivot of the given sp_direct_bitmap_bg_ptr when the action is updated.
### `bn::sp_direct_bitmap_bg_move_pivot_to_action` (class, bn_sp_direct_bitmap_bg_actions.h:2533)

Modifies the position of the pivot of a sp_direct_bitmap_bg_ptr until it has a given state.

`class sp_direct_bitmap_bg_move_pivot_to_action : public to_value_template_action<sp_direct_bitmap_bg_ptr, fixed_point, sp_direct_bitmap_bg_pivot_position_manager>`

- **sp_direct_bitmap_bg_move_pivot_to_action** `sp_direct_bitmap_bg_move_pivot_to_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L2548): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given sp_direct_bitmap_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the pivot of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the pivot of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_move_pivot_to_action** `sp_direct_bitmap_bg_move_pivot_to_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L2564): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given sp_direct_bitmap_bg_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the pivot of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the pivot of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_move_pivot_to_action** `sp_direct_bitmap_bg_move_pivot_to_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(bg, duration_updates, final_position)` (L2578): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given sp_direct_bitmap_bg_ptr is equal to final_position.; `final_position` Position of the pivot of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_move_pivot_to_action** `sp_direct_bitmap_bg_move_pivot_to_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : to_value_template_action(move(bg), duration_updates, final_position)` (L2592): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the pivot of the given sp_direct_bitmap_bg_ptr is equal to final_position.; `final_position` Position of the pivot of the given sp_direct_bitmap_bg_ptr when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L2601): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L2610): Returns the position of the pivot of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_move_pivot_loop_action` (class, bn_sp_direct_bitmap_bg_actions.h:2624)

Modifies the position of the pivot of a sp_direct_bitmap_bg_ptr from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class sp_direct_bitmap_bg_move_pivot_loop_action : public loop_value_template_action<sp_direct_bitmap_bg_ptr, fixed_point, sp_direct_bitmap_bg_pivot_position_manager>`

- **sp_direct_bitmap_bg_move_pivot_loop_action** `sp_direct_bitmap_bg_move_pivot_loop_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(bg, duration_updates, fixed_point(final_x, final_y))` (L2639): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the pivot of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the pivot of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sp_direct_bitmap_bg_move_pivot_loop_action** `sp_direct_bitmap_bg_move_pivot_loop_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(move(bg), duration_updates, fixed_point(final_x, final_y))` (L2655): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the pivot of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the pivot of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sp_direct_bitmap_bg_move_pivot_loop_action** `sp_direct_bitmap_bg_move_pivot_loop_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(bg, duration_updates, final_position)` (L2669): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the pivot of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sp_direct_bitmap_bg_move_pivot_loop_action** `sp_direct_bitmap_bg_move_pivot_loop_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& final_position) : loop_value_template_action(move(bg), duration_updates, final_position)` (L2683): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the pivot of the given sp_direct_bitmap_bg_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L2692): Returns the sp_direct_bitmap_bg_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L2701): When the position of the pivot of the given sp_direct_bitmap_bg_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sp_direct_bitmap_bg_move_pivot_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:2715)

Changes the position of the pivot of a sp_direct_bitmap_bg_ptr when the action is updated a given number of times.

`class sp_direct_bitmap_bg_move_pivot_toggle_action : public toggle_value_template_action<sp_direct_bitmap_bg_ptr, fixed_point, sp_direct_bitmap_bg_pivot_position_manager>`

- **sp_direct_bitmap_bg_move_pivot_toggle_action** `sp_direct_bitmap_bg_move_pivot_toggle_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(bg, duration_updates, fixed_point(new_x, new_y))` (L2728): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given sp_direct_bitmap_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_move_pivot_toggle_action** `sp_direct_bitmap_bg_move_pivot_toggle_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(move(bg), duration_updates, fixed_point(new_x, new_y))` (L2742): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given sp_direct_bitmap_bg_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_move_pivot_toggle_action** `sp_direct_bitmap_bg_move_pivot_toggle_action( const sp_direct_bitmap_bg_ptr& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(bg, duration_updates, new_position)` (L2755): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given sp_direct_bitmap_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **sp_direct_bitmap_bg_move_pivot_toggle_action** `sp_direct_bitmap_bg_move_pivot_toggle_action( sp_direct_bitmap_bg_ptr&& bg, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(move(bg), duration_updates, new_position)` (L2768): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the pivot of the given sp_direct_bitmap_bg_ptr.; `new_position` New position when the action is updated duration_updates times.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L2777): Returns the sp_direct_bitmap_bg_ptr to modify.
- **new_position** `const fixed_point& new_position() const` (L2786): Returns the position of the pivot of the given sp_direct_bitmap_bg_ptr when the action is updated the given number of times.
### `bn::sp_direct_bitmap_bg_mosaic_manager` (class, bn_sp_direct_bitmap_bg_actions.h:2801)

Manages if the mosaic effect must be applied to a sp_direct_bitmap_bg_ptr or not.

`class sp_direct_bitmap_bg_mosaic_manager`

- **get** `static bool get(const sp_direct_bitmap_bg_ptr& bg)` (L2808): Indicates if the mosaic effect is applied to the given sp_direct_bitmap_bg_ptr or not.
- **set** `static void set(bool mosaic_enabled, sp_direct_bitmap_bg_ptr& bg)` (L2816): Sets if the mosaic effect must be applied to the given sp_direct_bitmap_bg_ptr or not.
### `bn::sp_direct_bitmap_bg_mosaic_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:2830)

Toggles if the mosaic effect must be applied to a sp_direct_bitmap_bg_ptr or not when the action is updated a given number of times.

`class sp_direct_bitmap_bg_mosaic_toggle_action : public bool_toggle_value_template_action<sp_direct_bitmap_bg_ptr, sp_direct_bitmap_bg_mosaic_manager>`

- **sp_direct_bitmap_bg_mosaic_toggle_action** `sp_direct_bitmap_bg_mosaic_toggle_action(const sp_direct_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2841): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the mosaic effect must be applied or not.
- **sp_direct_bitmap_bg_mosaic_toggle_action** `sp_direct_bitmap_bg_mosaic_toggle_action(sp_direct_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2852): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the mosaic effect must be applied or not.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L2860): Returns the sp_direct_bitmap_bg_ptr to modify.
### `bn::sp_direct_bitmap_bg_blending_manager` (class, bn_sp_direct_bitmap_bg_actions.h:2875)

Manages if blending must be applied to a sp_direct_bitmap_bg_ptr or not.

`class sp_direct_bitmap_bg_blending_manager`

- **get** `static bool get(const sp_direct_bitmap_bg_ptr& bg)` (L2882): Indicates if blending is applied to the given sp_direct_bitmap_bg_ptr or not.
- **set** `static void set(bool blending_enabled, sp_direct_bitmap_bg_ptr& bg)` (L2890): Sets if blending must be applied to the given sp_direct_bitmap_bg_ptr or not.
### `bn::sp_direct_bitmap_bg_blending_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:2904)

Toggles if blending must be applied to a sp_direct_bitmap_bg_ptr or not when the action is updated a given number of times.

`class sp_direct_bitmap_bg_blending_toggle_action : public bool_toggle_value_template_action<sp_direct_bitmap_bg_ptr, sp_direct_bitmap_bg_blending_manager>`

- **sp_direct_bitmap_bg_blending_toggle_action** `sp_direct_bitmap_bg_blending_toggle_action(const sp_direct_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2915): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if blending must be applied or not.
- **sp_direct_bitmap_bg_blending_toggle_action** `sp_direct_bitmap_bg_blending_toggle_action(sp_direct_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2926): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if blending must be applied or not.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L2934): Returns the sp_direct_bitmap_bg_ptr to modify.
### `bn::sp_direct_bitmap_bg_blending_top_manager` (class, bn_sp_direct_bitmap_bg_actions.h:2947)

Manages if a sp_direct_bitmap_bg_ptr must be part of the blending top layer or not.

`class sp_direct_bitmap_bg_blending_top_manager`

- **get** `static bool get(const sp_direct_bitmap_bg_ptr& bg)` (L2954): Indicates if the given sp_direct_bitmap_bg_ptr is part of the blending top layer or not.
- **set** `static void set(bool blending_top_enabled, sp_direct_bitmap_bg_ptr& bg)` (L2962): Sets if the given sp_direct_bitmap_bg_ptr must be part of the blending top layer or not.
### `bn::sp_direct_bitmap_bg_blending_top_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:2976)

Toggles if a sp_direct_bitmap_bg_ptr is part of the blending top layer or not when the action is updated a given number of times.

`class sp_direct_bitmap_bg_blending_top_toggle_action : public bool_toggle_value_template_action<sp_direct_bitmap_bg_ptr, sp_direct_bitmap_bg_blending_top_manager>`

- **sp_direct_bitmap_bg_blending_top_toggle_action** `sp_direct_bitmap_bg_blending_top_toggle_action(const sp_direct_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L2987): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given sp_direct_bitmap_bg_ptr is part of the blending top layer or not.
- **sp_direct_bitmap_bg_blending_top_toggle_action** `sp_direct_bitmap_bg_blending_top_toggle_action(sp_direct_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L2998): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given sp_direct_bitmap_bg_ptr is part of the blending top layer or not.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L3006): Returns the sp_direct_bitmap_bg_ptr to modify.
### `bn::sp_direct_bitmap_bg_blending_bottom_manager` (class, bn_sp_direct_bitmap_bg_actions.h:3019)

Manages if a sp_direct_bitmap_bg_ptr must be part of the blending bottom layer or not.

`class sp_direct_bitmap_bg_blending_bottom_manager`

- **get** `static bool get(const sp_direct_bitmap_bg_ptr& bg)` (L3026): Indicates if the given sp_direct_bitmap_bg_ptr is part of the blending bottom layer or not.
- **set** `static void set(bool blending_bottom_enabled, sp_direct_bitmap_bg_ptr& bg)` (L3034): Sets if the given sp_direct_bitmap_bg_ptr must be part of the blending bottom layer or not.
### `bn::sp_direct_bitmap_bg_blending_bottom_toggle_action` (class, bn_sp_direct_bitmap_bg_actions.h:3048)

Toggles if a sp_direct_bitmap_bg_ptr is part of the blending bottom layer or not when the action is updated a given number of times.

`class sp_direct_bitmap_bg_blending_bottom_toggle_action : public bool_toggle_value_template_action<sp_direct_bitmap_bg_ptr, sp_direct_bitmap_bg_blending_bottom_manager>`

- **sp_direct_bitmap_bg_blending_bottom_toggle_action** `sp_direct_bitmap_bg_blending_bottom_toggle_action(const sp_direct_bitmap_bg_ptr& bg, int duration_updates) : bool_toggle_value_template_action(bg, duration_updates)` (L3059): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given sp_direct_bitmap_bg_ptr is part of the blending bottom layer or not.
- **sp_direct_bitmap_bg_blending_bottom_toggle_action** `sp_direct_bitmap_bg_blending_bottom_toggle_action(sp_direct_bitmap_bg_ptr&& bg, int duration_updates) : bool_toggle_value_template_action(move(bg), duration_updates)` (L3070): Constructor.
  `bg` sp_direct_bitmap_bg_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given sp_direct_bitmap_bg_ptr is part of the blending bottom layer or not.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L3078): Returns the sp_direct_bitmap_bg_ptr to modify.

## bn_sprite_actions.h

### `bn::sprite_visible_manager` (class, bn_sprite_actions.h:31)

Manages if a sprite_ptr must be committed to the GBA or not.

`class sprite_visible_manager`

- **get** `static bool get(const sprite_ptr& sprite)` (L38): Indicates if the given sprite_ptr is committed to the GBA or not.
- **set** `static void set(bool visible, sprite_ptr& sprite)` (L46): Sets if the given sprite_ptr must be committed to the GBA or not.
### `bn::sprite_visible_toggle_action` (class, bn_sprite_actions.h:60)

Toggles if a sprite_ptr must be committed to the GBA or not when the action is updated a given number of times.

`class sprite_visible_toggle_action : public bool_toggle_value_template_action<sprite_ptr, sprite_visible_manager>`

- **sprite_visible_toggle_action** `sprite_visible_toggle_action(const sprite_ptr& sprite, int duration_updates) : bool_toggle_value_template_action(sprite, duration_updates)` (L70): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given sprite_ptr must be committed to the GBA or not.
- **sprite_visible_toggle_action** `sprite_visible_toggle_action(sprite_ptr&& sprite, int duration_updates) : bool_toggle_value_template_action(move(sprite), duration_updates)` (L81): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given sprite_ptr must be committed to the GBA or not.
- **sprite** `const sprite_ptr& sprite() const` (L89): Returns the sprite_ptr to modify.
### `bn::sprite_position_manager` (class, bn_sprite_actions.h:104)

Manages the position of a sprite_ptr.

`class sprite_position_manager`

- **get** `static const fixed_point& get(const sprite_ptr& sprite)` (L111): Returns the position of the given sprite_ptr.
- **set** `static void set(const fixed_point& position, sprite_ptr& sprite)` (L119): Sets the position of the given sprite_ptr.
### `bn::sprite_move_by_action` (class, bn_sprite_actions.h:132)

Modifies the position of a sprite_ptr by a given delta.

`class sprite_move_by_action : public by_value_template_action<sprite_ptr, fixed_point, sprite_position_manager>`

- **sprite_move_by_action** `sprite_move_by_action(const sprite_ptr& sprite, fixed delta_x, fixed delta_y) : by_value_template_action(sprite, fixed_point(delta_x, delta_y))` (L142): Constructor.
  `sprite` sprite_ptr to copy.; `delta_x` How much to add to the horizontal position of the given sprite_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the given sprite_ptr when the action is updated.
- **sprite_move_by_action** `sprite_move_by_action(sprite_ptr&& sprite, fixed delta_x, fixed delta_y) : by_value_template_action(move(sprite), fixed_point(delta_x, delta_y))` (L153): Constructor.
  `sprite` sprite_ptr to move.; `delta_x` How much to add to the horizontal position of the given sprite_ptr when the action is updated.; `delta_y` How much to add to the horizontal position of the given sprite_ptr when the action is updated.
- **sprite_move_by_action** `sprite_move_by_action(const sprite_ptr& sprite, const fixed_point& delta_position) : by_value_template_action(sprite, delta_position)` (L163): Constructor.
  `sprite` sprite_ptr to copy.; `delta_position` How much to add to the position of the given sprite_ptr when the action is updated.
- **sprite_move_by_action** `sprite_move_by_action(sprite_ptr&& sprite, const fixed_point& delta_position) : by_value_template_action(move(sprite), delta_position)` (L173): Constructor.
  `sprite` sprite_ptr to move.; `delta_position` How much to add to the position of the given sprite_ptr when the action is updated.
- **sprite** `const sprite_ptr& sprite() const` (L181): Returns the sprite_ptr to modify.
- **delta_position** `const fixed_point& delta_position() const` (L189): Returns how much to add to the position of the given sprite_ptr when the action is updated.
### `bn::sprite_move_to_action` (class, bn_sprite_actions.h:202)

Modifies the position of a sprite_ptr until it has a given state.

`class sprite_move_to_action : public to_value_template_action<sprite_ptr, fixed_point, sprite_position_manager>`

- **sprite_move_to_action** `sprite_move_to_action(const sprite_ptr& sprite, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(sprite, duration_updates, fixed_point(final_x, final_y))` (L214): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the given sprite_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the given sprite_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the given sprite_ptr when the action is updated duration_updates times.
- **sprite_move_to_action** `sprite_move_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_x, fixed final_y) : to_value_template_action(move(sprite), duration_updates, fixed_point(final_x, final_y))` (L227): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the given sprite_ptr is equal to (final_x, final_y).; `final_x` Horizontal position of the given sprite_ptr when the action is updated duration_updates times.; `final_y` Vertical position of the given sprite_ptr when the action is updated duration_updates times.
- **sprite_move_to_action** `sprite_move_to_action(const sprite_ptr& sprite, int duration_updates, const fixed_point& final_position) : to_value_template_action(sprite, duration_updates, final_position)` (L239): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` Number of times that the action must be updated until the position of the given sprite_ptr is equal to final_position.; `final_position` Position of the given sprite_ptr when the action is updated duration_updates times.
- **sprite_move_to_action** `sprite_move_to_action(sprite_ptr&& sprite, int duration_updates, const fixed_point& final_position) : to_value_template_action(move(sprite), duration_updates, final_position)` (L251): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` Number of times that the action must be updated until the position of the given sprite_ptr is equal to final_position.; `final_position` Position of the given sprite_ptr when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L259): Returns the sprite_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L267): Returns the position of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_move_loop_action` (class, bn_sprite_actions.h:281)

Modifies the position of a sprite_ptr from a minimum to a maximum. When the position is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_move_loop_action : public loop_value_template_action<sprite_ptr, fixed_point, sprite_position_manager>`

- **sprite_move_loop_action** `sprite_move_loop_action(const sprite_ptr& sprite, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(sprite, duration_updates, fixed_point(final_x, final_y))` (L295): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_move_loop_action** `sprite_move_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_x, fixed final_y) : loop_value_template_action(move(sprite), duration_updates, fixed_point(final_x, final_y))` (L310): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_x` When the horizontal position of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_y` When the vertical position of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_move_loop_action** `sprite_move_loop_action(const sprite_ptr& sprite, int duration_updates, const fixed_point& final_position) : loop_value_template_action(sprite, duration_updates, final_position)` (L323): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_move_loop_action** `sprite_move_loop_action(sprite_ptr&& sprite, int duration_updates, const fixed_point& final_position) : loop_value_template_action(move(sprite), duration_updates, final_position)` (L336): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the position delta.; `final_position` When the position of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite** `const sprite_ptr& sprite() const` (L344): Returns the sprite_ptr to modify.
- **final_position** `const fixed_point& final_position() const` (L353): When the position of the given sprite_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_move_toggle_action` (class, bn_sprite_actions.h:366)

Changes the position of a sprite_ptr when the action is updated a given number of times.

`class sprite_move_toggle_action : public toggle_value_template_action<sprite_ptr, fixed_point, sprite_position_manager>`

- **sprite_move_toggle_action** `sprite_move_toggle_action(const sprite_ptr& sprite, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(sprite, duration_updates, fixed_point(new_x, new_y))` (L379): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the given sprite_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **sprite_move_toggle_action** `sprite_move_toggle_action(sprite_ptr&& sprite, int duration_updates, fixed new_x, fixed new_y) : toggle_value_template_action(move(sprite), duration_updates, fixed_point(new_x, new_y))` (L392): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the given sprite_ptr.; `new_x` New horizontal position when the action is updated duration_updates times.; `new_y` New vertical position when the action is updated duration_updates times.
- **sprite_move_toggle_action** `sprite_move_toggle_action(const sprite_ptr& sprite, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(sprite, duration_updates, new_position)` (L404): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to change the position of the given sprite_ptr.; `new_position` New position when the action is updated duration_updates times.
- **sprite_move_toggle_action** `sprite_move_toggle_action(sprite_ptr&& sprite, int duration_updates, const fixed_point& new_position) : toggle_value_template_action(move(sprite), duration_updates, new_position)` (L416): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to change the position of the given sprite_ptr.; `new_position` New position when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L424): Returns the sprite_ptr to modify.
- **new_position** `const fixed_point& new_position() const` (L432): Returns the position of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_top_left_position_manager` (class, bn_sprite_actions.h:447)

Manages the top-left position of a sprite_ptr.

`class sprite_top_left_position_manager`

- **get** `static fixed_point get(const sprite_ptr& sprite)` (L454): Returns the top-left position of the given sprite_ptr.
- **set** `static void set(const fixed_point& top_left_position, sprite_ptr& sprite)` (L462): Sets the top-left position of the given sprite_ptr.
### `bn::sprite_top_left_move_to_action` (class, bn_sprite_actions.h:475)

Modifies the top-left position of a sprite_ptr until it has a given state.

`class sprite_top_left_move_to_action : public to_value_template_action<sprite_ptr, fixed_point, sprite_top_left_position_manager>`

- **sprite_top_left_move_to_action** `sprite_top_left_move_to_action( const sprite_ptr& sprite, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : to_value_template_action(sprite, duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L490): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` Number of times that the action must be updated until the top-left position of the given sprite_ptr is equal to (final_x, final_y).; `final_top_left_x` Horizontal top-left position of the given sprite_ptr when the action is updated duration_updates times.; `final_top_left_y` Vertical top-left position of the given sprite_ptr when the action is updated duration_updates times.
- **sprite_top_left_move_to_action** `sprite_top_left_move_to_action( sprite_ptr&& sprite, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : to_value_template_action(move(sprite), duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L506): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` Number of times that the action must be updated until the top-left position of the given sprite_ptr is equal to (final_x, final_y).; `final_top_left_x` Horizontal top-left position of the given sprite_ptr when the action is updated duration_updates times.; `final_top_left_y` Vertical top-left position of the given sprite_ptr when the action is updated duration_updates times.
- **sprite_top_left_move_to_action** `sprite_top_left_move_to_action( const sprite_ptr& sprite, int duration_updates, const fixed_point& final_top_left_position) : to_value_template_action(sprite, duration_updates, final_top_left_position)` (L520): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` Number of times that the action must be updated until the top-left position of the given sprite_ptr is equal to final_top_left_position.; `final_top_left_position` Top-left position of the given sprite_ptr when the action is updated duration_updates times.
- **sprite_top_left_move_to_action** `sprite_top_left_move_to_action( sprite_ptr&& sprite, int duration_updates, const fixed_point& final_top_left_position) : to_value_template_action(move(sprite), duration_updates, final_top_left_position)` (L534): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` Number of times that the action must be updated until the top-left position of the given sprite_ptr is equal to final_top_left_position.; `final_top_left_position` Top-left position of the given sprite_ptr when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L543): Returns the sprite_ptr to modify.
- **final_top_left_position** `const fixed_point& final_top_left_position() const` (L552): Returns the top-left position of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_top_left_move_loop_action` (class, bn_sprite_actions.h:566)

Modifies the top-left position of a sprite_ptr from a minimum to a maximum. When the top-left position is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_top_left_move_loop_action : public loop_value_template_action<sprite_ptr, fixed_point, sprite_top_left_position_manager>`

- **sprite_top_left_move_loop_action** `sprite_top_left_move_loop_action( const sprite_ptr& sprite, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : loop_value_template_action(sprite, duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L581): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_x` When the horizontal top-left position of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_top_left_y` When the vertical top-left position of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_top_left_move_loop_action** `sprite_top_left_move_loop_action( sprite_ptr&& sprite, int duration_updates, fixed final_top_left_x, fixed final_top_left_y) : loop_value_template_action(move(sprite), duration_updates, fixed_point(final_top_left_x, final_top_left_y))` (L597): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_x` When the horizontal top-left position of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.; `final_top_left_y` When the vertical top-left position of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_top_left_move_loop_action** `sprite_top_left_move_loop_action( const sprite_ptr& sprite, int duration_updates, const fixed_point& final_top_left_position) : loop_value_template_action(sprite, duration_updates, final_top_left_position)` (L611): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_position` When the top-left position of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_top_left_move_loop_action** `sprite_top_left_move_loop_action( sprite_ptr&& sprite, int duration_updates, const fixed_point& final_top_left_position) : loop_value_template_action(move(sprite), duration_updates, final_top_left_position)` (L625): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the top-left position delta.; `final_top_left_position` When the top-left position of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite** `const sprite_ptr& sprite() const` (L634): Returns the sprite_ptr to modify.
- **final_top_left_position** `const fixed_point& final_top_left_position() const` (L643): When the top-left position of the given sprite_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_top_left_move_toggle_action` (class, bn_sprite_actions.h:656)

Changes the top-left position of a sprite_ptr when the action is updated a given number of times.

`class sprite_top_left_move_toggle_action : public toggle_value_template_action<sprite_ptr, fixed_point, sprite_top_left_position_manager>`

- **sprite_top_left_move_toggle_action** `sprite_top_left_move_toggle_action( const sprite_ptr& sprite, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) : toggle_value_template_action(sprite, duration_updates, fixed_point(new_top_left_x, new_top_left_y))` (L669): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to change the top-left position of the given sprite_ptr.; `new_top_left_x` New horizontal top-left position when the action is updated duration_updates times.; `new_top_left_y` New vertical top-left position when the action is updated duration_updates times.
- **sprite_top_left_move_toggle_action** `sprite_top_left_move_toggle_action( sprite_ptr&& sprite, int duration_updates, fixed new_top_left_x, fixed new_top_left_y) : toggle_value_template_action(move(sprite), duration_updates, fixed_point(new_top_left_x, new_top_left_y))` (L683): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to change the top-left position of the given sprite_ptr.; `new_top_left_x` New horizontal top-left position when the action is updated duration_updates times.; `new_top_left_y` New vertical top-left position when the action is updated duration_updates times.
- **sprite_top_left_move_toggle_action** `sprite_top_left_move_toggle_action( const sprite_ptr& sprite, int duration_updates, const fixed_point& new_top_left_position) : toggle_value_template_action(sprite, duration_updates, new_top_left_position)` (L696): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to change the top-left position of the given sprite_ptr.; `new_top_left_position` New top-left position when the action is updated duration_updates times.
- **sprite_top_left_move_toggle_action** `sprite_top_left_move_toggle_action( sprite_ptr&& sprite, int duration_updates, const fixed_point& new_top_left_position) : toggle_value_template_action(move(sprite), duration_updates, new_top_left_position)` (L709): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to change the top-left position of the given sprite_ptr.; `new_top_left_position` New top-left position when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L718): Returns the sprite_ptr to modify.
- **new_top_left_position** `const fixed_point& new_top_left_position() const` (L727): Returns the top-left position of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_rotation_manager` (class, bn_sprite_actions.h:742)

Manages the rotation angle of a sprite_ptr.

`class sprite_rotation_manager`

- **get** `static fixed get(const sprite_ptr& sprite)` (L749): Returns the rotation angle of the given sprite_ptr.
- **set** `static void set(fixed rotation_angle, sprite_ptr& sprite)` (L759): Sets the rotation angle of the given sprite_ptr.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].; `sprite` sprite_ptr to modify.
### `bn::sprite_rotate_by_action` (class, bn_sprite_actions.h:773)

Modifies the rotation angle of a sprite_ptr by delta_rotation_angle. When the rotation angle is over 360, it goes back to 0 and vice versa.

`class sprite_rotate_by_action : public cyclic_by_value_template_action<sprite_ptr, fixed, sprite_rotation_manager>`

- **sprite_rotate_by_action** `sprite_rotate_by_action(const sprite_ptr& sprite, fixed delta_rotation_angle) : cyclic_by_value_template_action(sprite, delta_rotation_angle, 0, 360)` (L785): Constructor.
  `sprite` sprite_ptr to copy.; `delta_rotation_angle` How many degrees to add to the rotation angle of the given sprite_ptr when the action is updated.
  This rotation angle must be in the range [0..360].
- **sprite_rotate_by_action** `sprite_rotate_by_action(sprite_ptr&& sprite, fixed delta_rotation_angle) : cyclic_by_value_template_action(move(sprite), delta_rotation_angle, 0, 360)` (L800): Constructor.
  `sprite` sprite_ptr to move.; `delta_rotation_angle` How many degrees to add to the rotation angle of the given sprite_ptr when the action is updated.
  This rotation angle must be in the range [0..360].
- **sprite** `const sprite_ptr& sprite() const` (L810): Returns the sprite_ptr to modify.
- **delta_rotation_angle** `fixed delta_rotation_angle() const` (L819): Returns how many degrees to add to the rotation angle of the given sprite_ptr when the action is updated.
### `bn::sprite_rotate_to_action` (class, bn_sprite_actions.h:832)

Modifies the rotation angle of a sprite_ptr until it has a given state.

`class sprite_rotate_to_action : public to_value_template_action<sprite_ptr, fixed, sprite_rotation_manager>`

- **sprite_rotate_to_action** `sprite_rotate_to_action(const sprite_ptr& sprite, int duration_updates, fixed final_rotation_angle) : to_value_template_action(sprite, duration_updates, final_rotation_angle)` (L845): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` Number of times that the action must be updated until the rotation angle of the given sprite_ptr is equal to final_rotation_angle.; `final_rotation_angle` Rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **sprite_rotate_to_action** `sprite_rotate_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_rotation_angle) : to_value_template_action(move(sprite), duration_updates, final_rotation_angle)` (L861): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` Number of times that the action must be updated until the rotation angle of the given sprite_ptr is equal to final_rotation_angle.; `final_rotation_angle` Rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **sprite** `const sprite_ptr& sprite() const` (L871): Returns the sprite_ptr to modify.
- **final_rotation_angle** `fixed final_rotation_angle() const` (L880): Returns the rotation angle of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_rotate_loop_action` (class, bn_sprite_actions.h:894)

Modifies the rotation angle of a sprite_ptr from a minimum to a maximum. When the rotation angle is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_rotate_loop_action : public loop_value_template_action<sprite_ptr, fixed, sprite_rotation_manager>`

- **sprite_rotate_loop_action** `sprite_rotate_loop_action(const sprite_ptr& sprite, int duration_updates, fixed final_rotation_angle) : loop_value_template_action(sprite, duration_updates, final_rotation_angle)` (L908): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the rotation angle delta.; `final_rotation_angle` When the rotation angle of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
  This rotation angle must be in the range [0..360].
- **sprite_rotate_loop_action** `sprite_rotate_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_rotation_angle) : loop_value_template_action(move(sprite), duration_updates, final_rotation_angle)` (L925): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the rotation angle delta.; `final_rotation_angle` When the rotation angle of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
  This rotation angle must be in the range [0..360].
- **sprite** `const sprite_ptr& sprite() const` (L935): Returns the sprite_ptr to modify.
- **final_rotation_angle** `fixed final_rotation_angle() const` (L944): When the rotation angle of the given sprite_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_rotate_toggle_action` (class, bn_sprite_actions.h:957)

Changes the rotation angle of a sprite_ptr when the action is updated a given number of times.

`class sprite_rotate_toggle_action : public toggle_value_template_action<sprite_ptr, fixed, sprite_rotation_manager>`

- **sprite_rotate_toggle_action** `sprite_rotate_toggle_action(const sprite_ptr& sprite, int duration_updates, fixed new_rotation_angle) : toggle_value_template_action(sprite, duration_updates, new_rotation_angle)` (L970): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to change the rotation angle of the given sprite_ptr.; `new_rotation_angle` New rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **sprite_rotate_toggle_action** `sprite_rotate_toggle_action(sprite_ptr&& sprite, int duration_updates, fixed new_rotation_angle) : toggle_value_template_action(move(sprite), duration_updates, new_rotation_angle)` (L986): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to change the rotation angle of the given sprite_ptr.; `new_rotation_angle` New rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **sprite** `const sprite_ptr& sprite() const` (L996): Returns the sprite_ptr to modify.
- **new_rotation_angle** `fixed new_rotation_angle() const` (L1005): Returns the rotation angle of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_horizontal_scale_manager` (class, bn_sprite_actions.h:1020)

Manages the horizontal scale of a sprite_ptr.

`class sprite_horizontal_scale_manager`

- **get** `static fixed get(const sprite_ptr& sprite)` (L1027): Returns the horizontal scale of the given sprite_ptr.
- **set** `static void set(fixed horizontal_scale, sprite_ptr& sprite)` (L1035): Sets the horizontal scale of the given sprite_ptr.
### `bn::sprite_horizontal_scale_to_action` (class, bn_sprite_actions.h:1048)

Modifies the horizontal scale of a sprite_ptr until it has a given state.

`class sprite_horizontal_scale_to_action : public to_value_template_action<sprite_ptr, fixed, sprite_horizontal_scale_manager>`

- **sprite_horizontal_scale_to_action** `sprite_horizontal_scale_to_action(const sprite_ptr& sprite, int duration_updates, fixed final_horizontal_scale) : to_value_template_action(sprite, duration_updates, final_horizontal_scale)` (L1060): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` Number of times that the action must be updated until the horizontal scale of the given sprite_ptr is equal to final_horizontal_scale.; `final_horizontal_scale` Horizontal scale when the action is updated duration_updates times.
- **sprite_horizontal_scale_to_action** `sprite_horizontal_scale_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_horizontal_scale) : to_value_template_action(move(sprite), duration_updates, final_horizontal_scale)` (L1074): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` Number of times that the action must be updated until the horizontal scale of the given sprite_ptr is equal to final_horizontal_scale.; `final_horizontal_scale` Horizontal scale when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L1083): Returns the sprite_ptr to modify.
- **final_horizontal_scale** `fixed final_horizontal_scale() const` (L1092): Returns the horizontal scale of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_horizontal_scale_loop_action` (class, bn_sprite_actions.h:1106)

Modifies the horizontal scale of a sprite_ptr from a minimum to a maximum. When the horizontal scale is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_horizontal_scale_loop_action : public loop_value_template_action<sprite_ptr, fixed, sprite_horizontal_scale_manager>`

- **sprite_horizontal_scale_loop_action** `sprite_horizontal_scale_loop_action(const sprite_ptr& sprite, int duration_updates, fixed final_horizontal_scale) : loop_value_template_action(sprite, duration_updates, final_horizontal_scale)` (L1119): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal scale delta.; `final_horizontal_scale` When the horizontal scale of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_horizontal_scale_loop_action** `sprite_horizontal_scale_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_horizontal_scale) : loop_value_template_action(move(sprite), duration_updates, final_horizontal_scale)` (L1134): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal scale delta.; `final_horizontal_scale` When the horizontal scale of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite** `const sprite_ptr& sprite() const` (L1143): Returns the sprite_ptr to modify.
- **final_horizontal_scale** `fixed final_horizontal_scale() const` (L1152): When the horizontal scale of the given sprite_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_horizontal_scale_toggle_action` (class, bn_sprite_actions.h:1166)

Changes the horizontal scale of a sprite_ptr when the action is updated a given number of times.

`class sprite_horizontal_scale_toggle_action : public toggle_value_template_action<sprite_ptr, fixed, sprite_horizontal_scale_manager>`

- **sprite_horizontal_scale_toggle_action** `sprite_horizontal_scale_toggle_action(const sprite_ptr& sprite, int duration_updates, fixed new_horizontal_scale) : toggle_value_template_action(sprite, duration_updates, new_horizontal_scale)` (L1178): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to change the horizontal scale of the given sprite_ptr.; `new_horizontal_scale` New horizontal scale when the action is updated duration_updates times.
- **sprite_horizontal_scale_toggle_action** `sprite_horizontal_scale_toggle_action(sprite_ptr&& sprite, int duration_updates, fixed new_horizontal_scale) : toggle_value_template_action(move(sprite), duration_updates, new_horizontal_scale)` (L1192): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to change the horizontal scale of the given sprite_ptr.; `new_horizontal_scale` New horizontal scale when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L1202): Returns the sprite_ptr to modify.
- **new_horizontal_scale** `fixed new_horizontal_scale() const` (L1211): Returns the horizontal scale of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_vertical_scale_manager` (class, bn_sprite_actions.h:1226)

Manages the vertical scale of a sprite_ptr.

`class sprite_vertical_scale_manager`

- **get** `static fixed get(const sprite_ptr& sprite)` (L1233): Returns the vertical scale of the given sprite_ptr.
- **set** `static void set(fixed vertical_scale, sprite_ptr& sprite)` (L1241): Sets the vertical scale of the given sprite_ptr.
### `bn::sprite_vertical_scale_to_action` (class, bn_sprite_actions.h:1254)

Modifies the vertical scale of a sprite_ptr until it has a given state.

`class sprite_vertical_scale_to_action : public to_value_template_action<sprite_ptr, fixed, sprite_vertical_scale_manager>`

- **sprite_vertical_scale_to_action** `sprite_vertical_scale_to_action(const sprite_ptr& sprite, int duration_updates, fixed final_vertical_scale) : to_value_template_action(sprite, duration_updates, final_vertical_scale)` (L1266): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` Number of times that the action must be updated until the vertical scale of the given sprite_ptr is equal to final_vertical_scale.; `final_vertical_scale` Vertical scale when the action is updated duration_updates times.
- **sprite_vertical_scale_to_action** `sprite_vertical_scale_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_vertical_scale) : to_value_template_action(move(sprite), duration_updates, final_vertical_scale)` (L1279): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` Number of times that the action must be updated until the vertical scale of the given sprite_ptr is equal to final_vertical_scale.; `final_vertical_scale` Vertical scale when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L1288): Returns the sprite_ptr to modify.
- **final_vertical_scale** `fixed final_vertical_scale() const` (L1297): Returns the vertical scale of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_vertical_scale_loop_action` (class, bn_sprite_actions.h:1311)

Modifies the vertical scale of a sprite_ptr from a minimum to a maximum. When the vertical scale is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_vertical_scale_loop_action : public loop_value_template_action<sprite_ptr, fixed, sprite_vertical_scale_manager>`

- **sprite_vertical_scale_loop_action** `sprite_vertical_scale_loop_action(const sprite_ptr& sprite, int duration_updates, fixed final_vertical_scale) : loop_value_template_action(sprite, duration_updates, final_vertical_scale)` (L1324): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical scale delta.; `final_vertical_scale` When the vertical scale of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_vertical_scale_loop_action** `sprite_vertical_scale_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_vertical_scale) : loop_value_template_action(move(sprite), duration_updates, final_vertical_scale)` (L1338): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical scale delta.; `final_vertical_scale` When the vertical scale of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite** `const sprite_ptr& sprite() const` (L1347): Returns the sprite_ptr to modify.
- **final_vertical_scale** `fixed final_vertical_scale() const` (L1356): When the vertical scale of the given sprite_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_vertical_scale_toggle_action` (class, bn_sprite_actions.h:1370)

Changes the vertical scale of a sprite_ptr when the action is updated a given number of times.

`class sprite_vertical_scale_toggle_action : public toggle_value_template_action<sprite_ptr, fixed, sprite_vertical_scale_manager>`

- **sprite_vertical_scale_toggle_action** `sprite_vertical_scale_toggle_action(const sprite_ptr& sprite, int duration_updates, fixed new_vertical_scale) : toggle_value_template_action(sprite, duration_updates, new_vertical_scale)` (L1382): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to change the vertical scale of the given sprite_ptr.; `new_vertical_scale` New vertical scale when the action is updated duration_updates times.
- **sprite_vertical_scale_toggle_action** `sprite_vertical_scale_toggle_action(sprite_ptr&& sprite, int duration_updates, fixed new_vertical_scale) : toggle_value_template_action(move(sprite), duration_updates, new_vertical_scale)` (L1395): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to change the vertical scale of the given sprite_ptr.; `new_vertical_scale` New vertical scale when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L1404): Returns the sprite_ptr to modify.
- **new_vertical_scale** `fixed new_vertical_scale() const` (L1413): Returns the vertical scale of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_scale_manager` (class, bn_sprite_actions.h:1428)

Manages the scale of a sprite_ptr.

`class sprite_scale_manager`

- **get** `static fixed get(const sprite_ptr& sprite)` (L1435): Returns the horizontal scale of the given sprite_ptr.
- **set** `static void set(fixed scale, sprite_ptr& sprite)` (L1443): Sets the scale of the given sprite_ptr.
### `bn::sprite_scale_to_action` (class, bn_sprite_actions.h:1456)

Modifies the scale of a sprite_ptr until it has a given state.

`class sprite_scale_to_action : public to_value_template_action<sprite_ptr, fixed, sprite_scale_manager>`

- **sprite_scale_to_action** `sprite_scale_to_action(const sprite_ptr& sprite, int duration_updates, fixed final_scale) : to_value_template_action(sprite, duration_updates, final_scale)` (L1467): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` Number of times that the action must be updated until the scale of the given sprite_ptr is equal to final_scale.; `final_scale` Scale when the action is updated duration_updates times.
- **sprite_scale_to_action** `sprite_scale_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_scale) : to_value_template_action(move(sprite), duration_updates, final_scale)` (L1480): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` Number of times that the action must be updated until the scale of the given sprite_ptr is equal to final_scale.; `final_scale` Scale when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L1489): Returns the sprite_ptr to modify.
- **final_scale** `fixed final_scale() const` (L1498): Returns the scale of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_scale_loop_action` (class, bn_sprite_actions.h:1512)

Modifies the scale of a sprite_ptr from a minimum to a maximum. When the scale is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_scale_loop_action : public loop_value_template_action<sprite_ptr, fixed, sprite_scale_manager>`

- **sprite_scale_loop_action** `sprite_scale_loop_action(const sprite_ptr& sprite, int duration_updates, fixed final_scale) : loop_value_template_action(sprite, duration_updates, final_scale)` (L1524): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the scale delta.; `final_scale` When the scale of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_scale_loop_action** `sprite_scale_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_scale) : loop_value_template_action(move(sprite), duration_updates, final_scale)` (L1538): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the scale delta.; `final_scale` When the scale of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite** `const sprite_ptr& sprite() const` (L1547): Returns the sprite_ptr to modify.
- **final_scale** `fixed final_scale() const` (L1556): When the scale of the given sprite_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_scale_toggle_action` (class, bn_sprite_actions.h:1570)

Changes the scale of a sprite_ptr when the action is updated a given number of times.

`class sprite_scale_toggle_action : public toggle_value_template_action<sprite_ptr, fixed, sprite_scale_manager>`

- **sprite_scale_toggle_action** `sprite_scale_toggle_action(const sprite_ptr& sprite, int duration_updates, fixed new_scale) : toggle_value_template_action(sprite, duration_updates, new_scale)` (L1581): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to change the scale of the given sprite_ptr.; `new_scale` New scale when the action is updated duration_updates times.
- **sprite_scale_toggle_action** `sprite_scale_toggle_action(sprite_ptr&& sprite, int duration_updates, fixed new_scale) : toggle_value_template_action(move(sprite), duration_updates, new_scale)` (L1594): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to change the scale of the given sprite_ptr.; `new_scale` New scale when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L1603): Returns the sprite_ptr to modify.
- **new_scale** `fixed new_scale() const` (L1612): Returns the scale of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_horizontal_shear_manager` (class, bn_sprite_actions.h:1627)

Manages the horizontal shear of a sprite_ptr.

`class sprite_horizontal_shear_manager`

- **get** `static fixed get(const sprite_ptr& sprite)` (L1634): Returns the horizontal shear of the given sprite_ptr.
- **set** `static void set(fixed horizontal_shear, sprite_ptr& sprite)` (L1642): Sets the horizontal shear of the given sprite_ptr.
### `bn::sprite_horizontal_shear_to_action` (class, bn_sprite_actions.h:1655)

Modifies the horizontal shear of a sprite_ptr until it has a given state.

`class sprite_horizontal_shear_to_action : public to_value_template_action<sprite_ptr, fixed, sprite_horizontal_shear_manager>`

- **sprite_horizontal_shear_to_action** `sprite_horizontal_shear_to_action(const sprite_ptr& sprite, int duration_updates, fixed final_horizontal_shear) : to_value_template_action(sprite, duration_updates, final_horizontal_shear)` (L1667): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` Number of times that the action must be updated until the horizontal shear of the given sprite_ptr is equal to final_horizontal_shear.; `final_horizontal_shear` Horizontal shear when the action is updated duration_updates times.
- **sprite_horizontal_shear_to_action** `sprite_horizontal_shear_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_horizontal_shear) : to_value_template_action(move(sprite), duration_updates, final_horizontal_shear)` (L1680): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` Number of times that the action must be updated until the horizontal shear of the given sprite_ptr is equal to final_horizontal_shear.; `final_horizontal_shear` Horizontal shear when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L1688): Returns the sprite_ptr to modify.
- **final_horizontal_shear** `fixed final_horizontal_shear() const` (L1697): Returns the horizontal shear of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_horizontal_shear_loop_action` (class, bn_sprite_actions.h:1711)

Modifies the horizontal shear of a sprite_ptr from a minimum to a maximum. When the horizontal shear is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_horizontal_shear_loop_action : public loop_value_template_action<sprite_ptr, fixed, sprite_horizontal_shear_manager>`

- **sprite_horizontal_shear_loop_action** `sprite_horizontal_shear_loop_action(const sprite_ptr& sprite, int duration_updates, fixed final_horizontal_shear) : loop_value_template_action(sprite, duration_updates, final_horizontal_shear)` (L1724): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal shear delta.; `final_horizontal_shear` When the horizontal shear of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_horizontal_shear_loop_action** `sprite_horizontal_shear_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_horizontal_shear) : loop_value_template_action(move(sprite), duration_updates, final_horizontal_shear)` (L1738): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal shear delta.; `final_horizontal_shear` When the horizontal shear of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite** `const sprite_ptr& sprite() const` (L1746): Returns the sprite_ptr to modify.
- **final_horizontal_shear** `fixed final_horizontal_shear() const` (L1755): When the horizontal shear of the given sprite_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_horizontal_shear_toggle_action` (class, bn_sprite_actions.h:1769)

Changes the horizontal shear of a sprite_ptr when the action is updated a given number of times.

`class sprite_horizontal_shear_toggle_action : public toggle_value_template_action<sprite_ptr, fixed, sprite_horizontal_shear_manager>`

- **sprite_horizontal_shear_toggle_action** `sprite_horizontal_shear_toggle_action(const sprite_ptr& sprite, int duration_updates, fixed new_horizontal_shear) : toggle_value_template_action(sprite, duration_updates, new_horizontal_shear)` (L1781): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to change the horizontal shear of the given sprite_ptr.; `new_horizontal_shear` New horizontal shear when the action is updated duration_updates times.
- **sprite_horizontal_shear_toggle_action** `sprite_horizontal_shear_toggle_action(sprite_ptr&& sprite, int duration_updates, fixed new_horizontal_shear) : toggle_value_template_action(move(sprite), duration_updates, new_horizontal_shear)` (L1794): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to change the horizontal shear of the given sprite_ptr.; `new_horizontal_shear` New horizontal shear when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L1803): Returns the sprite_ptr to modify.
- **new_horizontal_shear** `fixed new_horizontal_shear() const` (L1812): Returns the horizontal shear of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_vertical_shear_manager` (class, bn_sprite_actions.h:1827)

Manages the vertical shear of a sprite_ptr.

`class sprite_vertical_shear_manager`

- **get** `static fixed get(const sprite_ptr& sprite)` (L1834): Returns the vertical shear of the given sprite_ptr.
- **set** `static void set(fixed vertical_shear, sprite_ptr& sprite)` (L1842): Sets the vertical shear of the given sprite_ptr.
### `bn::sprite_vertical_shear_to_action` (class, bn_sprite_actions.h:1855)

Modifies the vertical shear of a sprite_ptr until it has a given state.

`class sprite_vertical_shear_to_action : public to_value_template_action<sprite_ptr, fixed, sprite_vertical_shear_manager>`

- **sprite_vertical_shear_to_action** `sprite_vertical_shear_to_action(const sprite_ptr& sprite, int duration_updates, fixed final_vertical_shear) : to_value_template_action(sprite, duration_updates, final_vertical_shear)` (L1867): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` Number of times that the action must be updated until the vertical shear of the given sprite_ptr is equal to final_vertical_shear.; `final_vertical_shear` Vertical shear when the action is updated duration_updates times.
- **sprite_vertical_shear_to_action** `sprite_vertical_shear_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_vertical_shear) : to_value_template_action(move(sprite), duration_updates, final_vertical_shear)` (L1879): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` Number of times that the action must be updated until the vertical shear of the given sprite_ptr is equal to final_vertical_shear.; `final_vertical_shear` Vertical shear when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L1887): Returns the sprite_ptr to modify.
- **final_vertical_shear** `fixed final_vertical_shear() const` (L1896): Returns the vertical shear of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_vertical_shear_loop_action` (class, bn_sprite_actions.h:1910)

Modifies the vertical shear of a sprite_ptr from a minimum to a maximum. When the vertical shear is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_vertical_shear_loop_action : public loop_value_template_action<sprite_ptr, fixed, sprite_vertical_shear_manager>`

- **sprite_vertical_shear_loop_action** `sprite_vertical_shear_loop_action(const sprite_ptr& sprite, int duration_updates, fixed final_vertical_shear) : loop_value_template_action(sprite, duration_updates, final_vertical_shear)` (L1923): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical shear delta.; `final_vertical_shear` When the vertical shear of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_vertical_shear_loop_action** `sprite_vertical_shear_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_vertical_shear) : loop_value_template_action(move(sprite), duration_updates, final_vertical_shear)` (L1936): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical shear delta.; `final_vertical_shear` When the vertical shear of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite** `const sprite_ptr& sprite() const` (L1944): Returns the sprite_ptr to modify.
- **final_vertical_shear** `fixed final_vertical_shear() const` (L1953): When the vertical shear of the given sprite_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_vertical_shear_toggle_action` (class, bn_sprite_actions.h:1967)

Changes the vertical shear of a sprite_ptr when the action is updated a given number of times.

`class sprite_vertical_shear_toggle_action : public toggle_value_template_action<sprite_ptr, fixed, sprite_vertical_shear_manager>`

- **sprite_vertical_shear_toggle_action** `sprite_vertical_shear_toggle_action(const sprite_ptr& sprite, int duration_updates, fixed new_vertical_shear) : toggle_value_template_action(sprite, duration_updates, new_vertical_shear)` (L1979): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to change the vertical shear of the given sprite_ptr.; `new_vertical_shear` New vertical shear when the action is updated duration_updates times.
- **sprite_vertical_shear_toggle_action** `sprite_vertical_shear_toggle_action(sprite_ptr&& sprite, int duration_updates, fixed new_vertical_shear) : toggle_value_template_action(move(sprite), duration_updates, new_vertical_shear)` (L1991): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to change the vertical shear of the given sprite_ptr.; `new_vertical_shear` New vertical shear when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L1999): Returns the sprite_ptr to modify.
- **new_vertical_shear** `fixed new_vertical_shear() const` (L2008): Returns the vertical shear of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_shear_manager` (class, bn_sprite_actions.h:2023)

Manages the shear of a sprite_ptr.

`class sprite_shear_manager`

- **get** `static fixed get(const sprite_ptr& sprite)` (L2030): Returns the horizontal shear of the given sprite_ptr.
- **set** `static void set(fixed shear, sprite_ptr& sprite)` (L2038): Sets the shear of the given sprite_ptr.
### `bn::sprite_shear_to_action` (class, bn_sprite_actions.h:2051)

Modifies the shear of a sprite_ptr until it has a given state.

`class sprite_shear_to_action : public to_value_template_action<sprite_ptr, fixed, sprite_shear_manager>`

- **sprite_shear_to_action** `sprite_shear_to_action(const sprite_ptr& sprite, int duration_updates, fixed final_shear) : to_value_template_action(sprite, duration_updates, final_shear)` (L2062): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` Number of times that the action must be updated until the shear of the given sprite_ptr is equal to final_shear.; `final_shear` Shear when the action is updated duration_updates times.
- **sprite_shear_to_action** `sprite_shear_to_action(sprite_ptr&& sprite, int duration_updates, fixed final_shear) : to_value_template_action(move(sprite), duration_updates, final_shear)` (L2074): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` Number of times that the action must be updated until the shear of the given sprite_ptr is equal to final_shear.; `final_shear` Shear when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L2082): Returns the sprite_ptr to modify.
- **final_shear** `fixed final_shear() const` (L2091): Returns the shear of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_shear_loop_action` (class, bn_sprite_actions.h:2105)

Modifies the shear of a sprite_ptr from a minimum to a maximum. When the shear is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_shear_loop_action : public loop_value_template_action<sprite_ptr, fixed, sprite_shear_manager>`

- **sprite_shear_loop_action** `sprite_shear_loop_action(const sprite_ptr& sprite, int duration_updates, fixed final_shear) : loop_value_template_action(sprite, duration_updates, final_shear)` (L2117): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the shear delta.; `final_shear` When the shear of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_shear_loop_action** `sprite_shear_loop_action(sprite_ptr&& sprite, int duration_updates, fixed final_shear) : loop_value_template_action(move(sprite), duration_updates, final_shear)` (L2130): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the shear delta.; `final_shear` When the shear of the given sprite_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite** `const sprite_ptr& sprite() const` (L2138): Returns the sprite_ptr to modify.
- **final_shear** `fixed final_shear() const` (L2147): When the shear of the given sprite_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_shear_toggle_action` (class, bn_sprite_actions.h:2161)

Changes the shear of a sprite_ptr when the action is updated a given number of times.

`class sprite_shear_toggle_action : public toggle_value_template_action<sprite_ptr, fixed, sprite_shear_manager>`

- **sprite_shear_toggle_action** `sprite_shear_toggle_action(const sprite_ptr& sprite, int duration_updates, fixed new_shear) : toggle_value_template_action(sprite, duration_updates, new_shear)` (L2172): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to change the shear of the given sprite_ptr.; `new_shear` New shear when the action is updated duration_updates times.
- **sprite_shear_toggle_action** `sprite_shear_toggle_action(sprite_ptr&& sprite, int duration_updates, fixed new_shear) : toggle_value_template_action(move(sprite), duration_updates, new_shear)` (L2184): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to change the shear of the given sprite_ptr.; `new_shear` New shear when the action is updated duration_updates times.
- **sprite** `const sprite_ptr& sprite() const` (L2192): Returns the sprite_ptr to modify.
- **new_shear** `fixed new_shear() const` (L2201): Returns the shear of the given sprite_ptr when the action is updated the given number of times.
### `bn::sprite_horizontal_flip_manager` (class, bn_sprite_actions.h:2216)

Manages if a sprite_ptr is flipped in its horizontal axis or not.

`class sprite_horizontal_flip_manager`

- **get** `static bool get(const sprite_ptr& sprite)` (L2223): Indicates if the given sprite_ptr is flipped in the horizontal axis or not.
- **set** `static void set(bool horizontal_flip, sprite_ptr& sprite)` (L2231): Sets if the given sprite_ptr must be flipped in the horizontal axis or not.
### `bn::sprite_horizontal_flip_toggle_action` (class, bn_sprite_actions.h:2245)

Toggles if a sprite_ptr must be flipped in the horizontal axis or not when the action is updated a given number of times.

`class sprite_horizontal_flip_toggle_action : public bool_toggle_value_template_action<sprite_ptr, sprite_horizontal_flip_manager>`

- **sprite_horizontal_flip_toggle_action** `sprite_horizontal_flip_toggle_action(const sprite_ptr& sprite, int duration_updates) : bool_toggle_value_template_action(sprite, duration_updates)` (L2256): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given sprite_ptr must be flipped in the horizontal axis or not.
- **sprite_horizontal_flip_toggle_action** `sprite_horizontal_flip_toggle_action(sprite_ptr&& sprite, int duration_updates) : bool_toggle_value_template_action(move(sprite), duration_updates)` (L2267): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given sprite_ptr must be flipped in the horizontal axis or not.
- **sprite** `const sprite_ptr& sprite() const` (L2275): Returns the sprite_ptr to modify.
### `bn::sprite_vertical_flip_manager` (class, bn_sprite_actions.h:2290)

Manages if a sprite_ptr is flipped in its vertical axis or not.

`class sprite_vertical_flip_manager`

- **get** `static bool get(const sprite_ptr& sprite)` (L2297): Indicates if the given sprite_ptr is flipped in the vertical axis or not.
- **set** `static void set(bool vertical_flip, sprite_ptr& sprite)` (L2305): Sets if the given sprite_ptr must be flipped in the vertical axis or not.
### `bn::sprite_vertical_flip_toggle_action` (class, bn_sprite_actions.h:2319)

Toggles if a sprite_ptr must be flipped in the vertical axis or not when the action is updated a given number of times.

`class sprite_vertical_flip_toggle_action : public bool_toggle_value_template_action<sprite_ptr, sprite_vertical_flip_manager>`

- **sprite_vertical_flip_toggle_action** `sprite_vertical_flip_toggle_action(const sprite_ptr& sprite, int duration_updates) : bool_toggle_value_template_action(sprite, duration_updates)` (L2330): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given sprite_ptr must be flipped in the vertical axis or not.
- **sprite_vertical_flip_toggle_action** `sprite_vertical_flip_toggle_action(sprite_ptr&& sprite, int duration_updates) : bool_toggle_value_template_action(move(sprite), duration_updates)` (L2341): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given sprite_ptr must be flipped in the vertical axis or not.
- **sprite** `const sprite_ptr& sprite() const` (L2349): Returns the sprite_ptr to modify.
### `bn::sprite_mosaic_manager` (class, bn_sprite_actions.h:2364)

Manages if the mosaic effect must be applied to a sprite_ptr or not.

`class sprite_mosaic_manager`

- **get** `static bool get(const sprite_ptr& sprite)` (L2371): Indicates if the mosaic effect is applied to the given sprite_ptr or not.
- **set** `static void set(bool mosaic_enabled, sprite_ptr& sprite)` (L2379): Sets if the mosaic effect must be applied to the given sprite_ptr or not.
### `bn::sprite_mosaic_toggle_action` (class, bn_sprite_actions.h:2393)

Toggles if the mosaic effect must be applied to a sprite_ptr or not when the action is updated a given number of times.

`class sprite_mosaic_toggle_action : public bool_toggle_value_template_action<sprite_ptr, sprite_mosaic_manager>`

- **sprite_mosaic_toggle_action** `sprite_mosaic_toggle_action(const sprite_ptr& sprite, int duration_updates) : bool_toggle_value_template_action(sprite, duration_updates)` (L2403): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the mosaic effect must be applied or not.
- **sprite_mosaic_toggle_action** `sprite_mosaic_toggle_action(sprite_ptr&& sprite, int duration_updates) : bool_toggle_value_template_action(move(sprite), duration_updates)` (L2414): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the mosaic effect must be applied or not.
- **sprite** `const sprite_ptr& sprite() const` (L2422): Returns the sprite_ptr to modify.
### `bn::sprite_blending_manager` (class, bn_sprite_actions.h:2437)

Manages if blending must be applied to a sprite_ptr or not.

`class sprite_blending_manager`

- **get** `static bool get(const sprite_ptr& sprite)` (L2444): Indicates if blending is applied to the given sprite_ptr or not.
- **set** `static void set(bool blending_enabled, sprite_ptr& sprite)` (L2454): Sets if blending must be applied to the given sprite_ptr or not.
  Keep in mind that blending and window attributes can't be enabled on a sprite_ptr at the same time.
### `bn::sprite_blending_toggle_action` (class, bn_sprite_actions.h:2468)

Toggles if blending must be applied to a sprite_ptr or not when the action is updated a given number of times.

`class sprite_blending_toggle_action : public bool_toggle_value_template_action<sprite_ptr, sprite_blending_manager>`

- **sprite_blending_toggle_action** `sprite_blending_toggle_action(const sprite_ptr& sprite, int duration_updates) : bool_toggle_value_template_action(sprite, duration_updates)` (L2478): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if blending must be applied or not.
- **sprite_blending_toggle_action** `sprite_blending_toggle_action(sprite_ptr&& sprite, int duration_updates) : bool_toggle_value_template_action(move(sprite), duration_updates)` (L2489): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if blending must be applied or not.
- **sprite** `const sprite_ptr& sprite() const` (L2497): Returns the sprite_ptr to modify.
### `bn::sprite_window_manager` (class, bn_sprite_actions.h:2512)

Manages if a sprite_ptr must be part of the silhouette of the sprite window or not.

`class sprite_window_manager`

- **get** `static bool get(const sprite_ptr& sprite)` (L2519): Indicates if the given sprite_ptr must be part of the silhouette of the sprite window or not.
- **set** `static void set(bool window_enabled, sprite_ptr& sprite)` (L2529): Sets if the given sprite_ptr must be part of the silhouette of the sprite window or not.
  Keep in mind that blending and window attributes can't be enabled on a sprite_ptr at the same time.
### `bn::sprite_window_toggle_action` (class, bn_sprite_actions.h:2543)

Toggles if a sprite_ptr must be part of the silhouette of the sprite window or not when the action is updated a given number of times.

`class sprite_window_toggle_action : public bool_toggle_value_template_action<sprite_ptr, sprite_window_manager>`

- **sprite_window_toggle_action** `sprite_window_toggle_action(const sprite_ptr& sprite, int duration_updates) : bool_toggle_value_template_action(sprite, duration_updates)` (L2553): Constructor.
  `sprite` sprite_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given sprite_ptr must be part of the silhouette of the sprite window or not.
- **sprite_window_toggle_action** `sprite_window_toggle_action(sprite_ptr&& sprite, int duration_updates) : bool_toggle_value_template_action(move(sprite), duration_updates)` (L2564): Constructor.
  `sprite` sprite_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given sprite_ptr must be part of the silhouette of the sprite window or not.
- **sprite** `const sprite_ptr& sprite() const` (L2572): Returns the sprite_ptr to modify.

## bn_sprite_affine_mat_actions.h

### `bn::sprite_affine_mat_rotation_manager` (class, bn_sprite_affine_mat_actions.h:33)

Manages the rotation angle of a sprite_affine_mat_ptr.

`class sprite_affine_mat_rotation_manager`

- **get** `static fixed get(const sprite_affine_mat_ptr& affine_mat)` (L40): Returns the rotation angle of the given sprite_affine_mat_ptr.
- **set** `static void set(fixed rotation_angle, sprite_affine_mat_ptr& affine_mat)` (L50): Sets the rotation angle of the given sprite_affine_mat_ptr.
  `rotation_angle` Rotation angle in degrees, in the range [0..360].; `affine_mat` sprite_affine_mat_ptr to modify.
### `bn::sprite_affine_mat_rotate_by_action` (class, bn_sprite_affine_mat_actions.h:65)

Modifies the rotation angle of a sprite_affine_mat_ptr by delta_rotation_angle. When the rotation angle is over 360, it goes back to 0 and vice versa.

`class sprite_affine_mat_rotate_by_action : public cyclic_by_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_rotation_manager>`

- **sprite_affine_mat_rotate_by_action** `sprite_affine_mat_rotate_by_action(const sprite_affine_mat_ptr& affine_mat, fixed delta_rotation_angle) : cyclic_by_value_template_action(affine_mat, delta_rotation_angle, 0, 360)` (L78): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `delta_rotation_angle` How many degrees to add to the rotation angle of the given sprite_affine_mat_ptr when the action is updated.
  This rotation angle must be in the range [0..360].
- **sprite_affine_mat_rotate_by_action** `sprite_affine_mat_rotate_by_action(sprite_affine_mat_ptr&& affine_mat, fixed delta_rotation_angle) : cyclic_by_value_template_action(move(affine_mat), delta_rotation_angle, 0, 360)` (L93): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `delta_rotation_angle` How many degrees to add to the rotation angle of the given sprite_affine_mat_ptr when the action is updated.
  This rotation angle must be in the range [0..360].
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L103): Returns the sprite_affine_mat_ptr to modify.
- **delta_rotation_angle** `fixed delta_rotation_angle() const` (L112): Returns how many degrees to add to the rotation angle of the given sprite_affine_mat_ptr when the action is updated.
### `bn::sprite_affine_mat_rotate_to_action` (class, bn_sprite_affine_mat_actions.h:126)

Modifies the rotation angle of a sprite_affine_mat_ptr until it has a given state.

`class sprite_affine_mat_rotate_to_action : public to_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_rotation_manager>`

- **sprite_affine_mat_rotate_to_action** `sprite_affine_mat_rotate_to_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed final_rotation_angle) : to_value_template_action(affine_mat, duration_updates, final_rotation_angle)` (L140): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` Number of times that the action must be updated until the rotation angle of the given sprite_affine_mat_ptr is equal to final_rotation_angle.; `final_rotation_angle` Rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **sprite_affine_mat_rotate_to_action** `sprite_affine_mat_rotate_to_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed final_rotation_angle) : to_value_template_action(move(affine_mat), duration_updates, final_rotation_angle)` (L157): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` Number of times that the action must be updated until the rotation angle of the given sprite_affine_mat_ptr is equal to final_rotation_angle.; `final_rotation_angle` Rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L168): Returns the sprite_affine_mat_ptr to modify.
- **final_rotation_angle** `fixed final_rotation_angle() const` (L177): Returns the rotation angle of the given sprite_affine_mat_ptr when the action is updated the given number of times.
### `bn::sprite_affine_mat_rotate_loop_action` (class, bn_sprite_affine_mat_actions.h:192)

Modifies the rotation angle of a sprite_affine_mat_ptr from a minimum to a maximum. When the rotation angle is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_affine_mat_rotate_loop_action : public loop_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_rotation_manager>`

- **sprite_affine_mat_rotate_loop_action** `sprite_affine_mat_rotate_loop_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed final_rotation_angle) : loop_value_template_action(affine_mat, duration_updates, final_rotation_angle)` (L207): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the rotation angle delta.; `final_rotation_angle` When the rotation angle of the given sprite_affine_mat_ptr is equal to this parameter, it goes back to its initial state and vice versa.
  This rotation angle must be in the range [0..360].
- **sprite_affine_mat_rotate_loop_action** `sprite_affine_mat_rotate_loop_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed final_rotation_angle) : loop_value_template_action(move(affine_mat), duration_updates, final_rotation_angle)` (L225): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the rotation angle delta.; `final_rotation_angle` When the rotation angle of the given sprite_affine_mat_ptr is equal to this parameter, it goes back to its initial state and vice versa.
  This rotation angle must be in the range [0..360].
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L236): Returns the sprite_affine_mat_ptr to modify.
- **final_rotation_angle** `fixed final_rotation_angle() const` (L245): When the rotation angle of the given sprite_affine_mat_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_affine_mat_rotate_toggle_action` (class, bn_sprite_affine_mat_actions.h:259)

Changes the rotation angle of a sprite_affine_mat_ptr when the action is updated a given number of times.

`class sprite_affine_mat_rotate_toggle_action : public toggle_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_rotation_manager>`

- **sprite_affine_mat_rotate_toggle_action** `sprite_affine_mat_rotate_toggle_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed new_rotation_angle) : toggle_value_template_action(affine_mat, duration_updates, new_rotation_angle)` (L273): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated to change the rotation angle of the given sprite_affine_mat_ptr.; `new_rotation_angle` New rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **sprite_affine_mat_rotate_toggle_action** `sprite_affine_mat_rotate_toggle_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed new_rotation_angle) : toggle_value_template_action(move(affine_mat), duration_updates, new_rotation_angle)` (L290): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated to change the rotation angle of the given sprite_affine_mat_ptr.; `new_rotation_angle` New rotation angle when the action is updated duration_updates times.
  This rotation angle must be in the range [0..360].
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L301): Returns the sprite_affine_mat_ptr to modify.
- **new_rotation_angle** `fixed new_rotation_angle() const` (L310): Returns the rotation angle of the given sprite_affine_mat_ptr when the action is updated the given number of times.
### `bn::sprite_affine_mat_horizontal_scale_manager` (class, bn_sprite_affine_mat_actions.h:326)

Manages the horizontal scale of a sprite_affine_mat_ptr.

`class sprite_affine_mat_horizontal_scale_manager`

- **get** `static fixed get(const sprite_affine_mat_ptr& affine_mat)` (L333): Returns the horizontal scale of the given sprite_affine_mat_ptr.
- **set** `static void set(fixed horizontal_scale, sprite_affine_mat_ptr& affine_mat)` (L341): Sets the horizontal scale of the given sprite_affine_mat_ptr.
### `bn::sprite_affine_mat_horizontal_scale_to_action` (class, bn_sprite_affine_mat_actions.h:355)

Modifies the horizontal scale of a sprite_affine_mat_ptr until it has a given state.

`class sprite_affine_mat_horizontal_scale_to_action : public to_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_horizontal_scale_manager>`

- **sprite_affine_mat_horizontal_scale_to_action** `sprite_affine_mat_horizontal_scale_to_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed final_horizontal_scale) : to_value_template_action(affine_mat, duration_updates, final_horizontal_scale)` (L367): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` Number of times that the action must be updated until the horizontal scale of the given sprite_affine_mat_ptr is equal to final_horizontal_scale.; `final_horizontal_scale` Horizontal scale when the action is updated duration_updates times.
- **sprite_affine_mat_horizontal_scale_to_action** `sprite_affine_mat_horizontal_scale_to_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed final_horizontal_scale) : to_value_template_action(move(affine_mat), duration_updates, final_horizontal_scale)` (L381): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` Number of times that the action must be updated until the horizontal scale of the given sprite_affine_mat_ptr is equal to final_horizontal_scale.; `final_horizontal_scale` Horizontal scale when the action is updated duration_updates times.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L391): Returns the sprite_affine_mat_ptr to modify.
- **final_horizontal_scale** `fixed final_horizontal_scale() const` (L400): Returns the horizontal scale of the given sprite_affine_mat_ptr when the action is updated the given number of times.
### `bn::sprite_affine_mat_horizontal_scale_loop_action` (class, bn_sprite_affine_mat_actions.h:415)

Modifies the horizontal scale of a sprite_affine_mat_ptr from a minimum to a maximum. When the horizontal scale is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_affine_mat_horizontal_scale_loop_action : public loop_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_horizontal_scale_manager>`

- **sprite_affine_mat_horizontal_scale_loop_action** `sprite_affine_mat_horizontal_scale_loop_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed final_horizontal_scale) : loop_value_template_action(affine_mat, duration_updates, final_horizontal_scale)` (L428): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal scale delta.; `final_horizontal_scale` When the horizontal scale of the given sprite_affine_mat_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_affine_mat_horizontal_scale_loop_action** `sprite_affine_mat_horizontal_scale_loop_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed final_horizontal_scale) : loop_value_template_action(move(affine_mat), duration_updates, final_horizontal_scale)` (L443): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal scale delta.; `final_horizontal_scale` When the horizontal scale of the given sprite_affine_mat_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L453): Returns the sprite_affine_mat_ptr to modify.
- **final_horizontal_scale** `fixed final_horizontal_scale() const` (L462): When the horizontal scale of the given sprite_affine_mat_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_affine_mat_horizontal_scale_toggle_action` (class, bn_sprite_affine_mat_actions.h:477)

Changes the horizontal scale of a sprite_affine_mat_ptr when the action is updated a given number of times.

`class sprite_affine_mat_horizontal_scale_toggle_action : public toggle_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_horizontal_scale_manager>`

- **sprite_affine_mat_horizontal_scale_toggle_action** `sprite_affine_mat_horizontal_scale_toggle_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed new_horizontal_scale) : toggle_value_template_action(affine_mat, duration_updates, new_horizontal_scale)` (L489): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated to change the horizontal scale of the given sprite_affine_mat_ptr.; `new_horizontal_scale` New horizontal scale when the action is updated duration_updates times.
- **sprite_affine_mat_horizontal_scale_toggle_action** `sprite_affine_mat_horizontal_scale_toggle_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed new_horizontal_scale) : toggle_value_template_action(move(affine_mat), duration_updates, new_horizontal_scale)` (L503): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated to change the horizontal scale of the given sprite_affine_mat_ptr.; `new_horizontal_scale` New horizontal scale when the action is updated duration_updates times.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L513): Returns the sprite_affine_mat_ptr to modify.
- **new_horizontal_scale** `fixed new_horizontal_scale() const` (L522): Returns the horizontal scale of the given sprite_affine_mat_ptr when the action is updated the given number of times.
### `bn::sprite_affine_mat_vertical_scale_manager` (class, bn_sprite_affine_mat_actions.h:538)

Manages the vertical scale of a sprite_affine_mat_ptr.

`class sprite_affine_mat_vertical_scale_manager`

- **get** `static fixed get(const sprite_affine_mat_ptr& affine_mat)` (L545): Returns the vertical scale of the given sprite_affine_mat_ptr.
- **set** `static void set(fixed vertical_scale, sprite_affine_mat_ptr& affine_mat)` (L553): Sets the vertical scale of the given sprite_affine_mat_ptr.
### `bn::sprite_affine_mat_vertical_scale_to_action` (class, bn_sprite_affine_mat_actions.h:567)

Modifies the vertical scale of a sprite_affine_mat_ptr until it has a given state.

`class sprite_affine_mat_vertical_scale_to_action : public to_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_vertical_scale_manager>`

- **sprite_affine_mat_vertical_scale_to_action** `sprite_affine_mat_vertical_scale_to_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed final_vertical_scale) : to_value_template_action(affine_mat, duration_updates, final_vertical_scale)` (L579): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` Number of times that the action must be updated until the vertical scale of the given sprite_affine_mat_ptr is equal to final_vertical_scale.; `final_vertical_scale` Vertical scale when the action is updated duration_updates times.
- **sprite_affine_mat_vertical_scale_to_action** `sprite_affine_mat_vertical_scale_to_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed final_vertical_scale) : to_value_template_action(move(affine_mat), duration_updates, final_vertical_scale)` (L593): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` Number of times that the action must be updated until the vertical scale of the given sprite_affine_mat_ptr is equal to final_vertical_scale.; `final_vertical_scale` Vertical scale when the action is updated duration_updates times.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L603): Returns the sprite_affine_mat_ptr to modify.
- **final_vertical_scale** `fixed final_vertical_scale() const` (L612): Returns the vertical scale of the given sprite_affine_mat_ptr when the action is updated the given number of times.
### `bn::sprite_affine_mat_vertical_scale_loop_action` (class, bn_sprite_affine_mat_actions.h:627)

Modifies the vertical scale of a sprite_affine_mat_ptr from a minimum to a maximum. When the vertical scale is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_affine_mat_vertical_scale_loop_action : public loop_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_vertical_scale_manager>`

- **sprite_affine_mat_vertical_scale_loop_action** `sprite_affine_mat_vertical_scale_loop_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed final_vertical_scale) : loop_value_template_action(affine_mat, duration_updates, final_vertical_scale)` (L640): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical scale delta.; `final_vertical_scale` When the vertical scale of the given sprite_affine_mat_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_affine_mat_vertical_scale_loop_action** `sprite_affine_mat_vertical_scale_loop_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed final_vertical_scale) : loop_value_template_action(move(affine_mat), duration_updates, final_vertical_scale)` (L655): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical scale delta.; `final_vertical_scale` When the vertical scale of the given sprite_affine_mat_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L665): Returns the sprite_affine_mat_ptr to modify.
- **final_vertical_scale** `fixed final_vertical_scale() const` (L674): When the vertical scale of the given sprite_affine_mat_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_affine_mat_vertical_scale_toggle_action` (class, bn_sprite_affine_mat_actions.h:689)

Changes the vertical scale of a sprite_affine_mat_ptr when the action is updated a given number of times.

`class sprite_affine_mat_vertical_scale_toggle_action : public toggle_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_vertical_scale_manager>`

- **sprite_affine_mat_vertical_scale_toggle_action** `sprite_affine_mat_vertical_scale_toggle_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed new_vertical_scale) : toggle_value_template_action(affine_mat, duration_updates, new_vertical_scale)` (L701): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated to change the vertical scale of the given sprite_affine_mat_ptr.; `new_vertical_scale` New vertical scale when the action is updated duration_updates times.
- **sprite_affine_mat_vertical_scale_toggle_action** `sprite_affine_mat_vertical_scale_toggle_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed new_vertical_scale) : toggle_value_template_action(move(affine_mat), duration_updates, new_vertical_scale)` (L715): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated to change the vertical scale of the given sprite_affine_mat_ptr.; `new_vertical_scale` New vertical scale when the action is updated duration_updates times.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L725): Returns the sprite_affine_mat_ptr to modify.
- **new_vertical_scale** `fixed new_vertical_scale() const` (L734): Returns the vertical scale of the given sprite_affine_mat_ptr when the action is updated the given number of times.
### `bn::sprite_affine_mat_scale_manager` (class, bn_sprite_affine_mat_actions.h:750)

Manages the scale of a sprite_affine_mat_ptr.

`class sprite_affine_mat_scale_manager`

- **get** `static fixed get(const sprite_affine_mat_ptr& affine_mat)` (L757): Returns the horizontal scale of the given sprite_affine_mat_ptr.
- **set** `static void set(fixed scale, sprite_affine_mat_ptr& affine_mat)` (L765): Sets the scale of the given sprite_affine_mat_ptr.
### `bn::sprite_affine_mat_scale_to_action` (class, bn_sprite_affine_mat_actions.h:779)

Modifies the scale of a sprite_affine_mat_ptr until it has a given state.

`class sprite_affine_mat_scale_to_action : public to_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_scale_manager>`

- **sprite_affine_mat_scale_to_action** `sprite_affine_mat_scale_to_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed final_scale) : to_value_template_action(affine_mat, duration_updates, final_scale)` (L791): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` Number of times that the action must be updated until the scale of the given sprite_affine_mat_ptr is equal to final_scale.; `final_scale` Scale when the action is updated duration_updates times.
- **sprite_affine_mat_scale_to_action** `sprite_affine_mat_scale_to_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed final_scale) : to_value_template_action(move(affine_mat), duration_updates, final_scale)` (L805): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` Number of times that the action must be updated until the scale of the given sprite_affine_mat_ptr is equal to final_scale.; `final_scale` Scale when the action is updated duration_updates times.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L814): Returns the sprite_affine_mat_ptr to modify.
- **final_scale** `fixed final_scale() const` (L823): Returns the scale of the given sprite_affine_mat_ptr when the action is updated the given number of times.
### `bn::sprite_affine_mat_scale_loop_action` (class, bn_sprite_affine_mat_actions.h:838)

Modifies the scale of a sprite_affine_mat_ptr from a minimum to a maximum. When the scale is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_affine_mat_scale_loop_action : public loop_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_scale_manager>`

- **sprite_affine_mat_scale_loop_action** `sprite_affine_mat_scale_loop_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed final_scale) : loop_value_template_action(affine_mat, duration_updates, final_scale)` (L851): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the scale delta.; `final_scale` When the scale of the given sprite_affine_mat_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_affine_mat_scale_loop_action** `sprite_affine_mat_scale_loop_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed final_scale) : loop_value_template_action(move(affine_mat), duration_updates, final_scale)` (L866): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the scale delta.; `final_scale` When the scale of the given sprite_affine_mat_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L875): Returns the sprite_affine_mat_ptr to modify.
- **final_scale** `fixed final_scale() const` (L884): When the scale of the given sprite_affine_mat_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_affine_mat_scale_toggle_action` (class, bn_sprite_affine_mat_actions.h:899)

Changes the scale of a sprite_affine_mat_ptr when the action is updated a given number of times.

`class sprite_affine_mat_scale_toggle_action : public toggle_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_scale_manager>`

- **sprite_affine_mat_scale_toggle_action** `sprite_affine_mat_scale_toggle_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed new_scale) : toggle_value_template_action(affine_mat, duration_updates, new_scale)` (L911): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated to change the scale of the given sprite_affine_mat_ptr.; `new_scale` New scale when the action is updated duration_updates times.
- **sprite_affine_mat_scale_toggle_action** `sprite_affine_mat_scale_toggle_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed new_scale) : toggle_value_template_action(move(affine_mat), duration_updates, new_scale)` (L925): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated to change the scale of the given sprite_affine_mat_ptr.; `new_scale` New scale when the action is updated duration_updates times.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L934): Returns the sprite_affine_mat_ptr to modify.
- **new_scale** `fixed new_scale() const` (L943): Returns the scale of the given sprite_affine_mat_ptr when the action is updated the given number of times.
### `bn::sprite_affine_mat_horizontal_shear_manager` (class, bn_sprite_affine_mat_actions.h:959)

Manages the horizontal shear of a sprite_affine_mat_ptr.

`class sprite_affine_mat_horizontal_shear_manager`

- **get** `static fixed get(const sprite_affine_mat_ptr& affine_mat)` (L966): Returns the horizontal shear of the given sprite_affine_mat_ptr.
- **set** `static void set(fixed horizontal_shear, sprite_affine_mat_ptr& affine_mat)` (L974): Sets the horizontal shear of the given sprite_affine_mat_ptr.
### `bn::sprite_affine_mat_horizontal_shear_to_action` (class, bn_sprite_affine_mat_actions.h:988)

Modifies the horizontal shear of a sprite_affine_mat_ptr until it has a given state.

`class sprite_affine_mat_horizontal_shear_to_action : public to_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_horizontal_shear_manager>`

- **sprite_affine_mat_horizontal_shear_to_action** `sprite_affine_mat_horizontal_shear_to_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed final_horizontal_shear) : to_value_template_action(affine_mat, duration_updates, final_horizontal_shear)` (L1000): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` Number of times that the action must be updated until the horizontal shear of the given sprite_affine_mat_ptr is equal to final_horizontal_shear.; `final_horizontal_shear` Horizontal shear when the action is updated duration_updates times.
- **sprite_affine_mat_horizontal_shear_to_action** `sprite_affine_mat_horizontal_shear_to_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed final_horizontal_shear) : to_value_template_action(move(affine_mat), duration_updates, final_horizontal_shear)` (L1013): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` Number of times that the action must be updated until the horizontal shear of the given sprite_affine_mat_ptr is equal to final_horizontal_shear.; `final_horizontal_shear` Horizontal shear when the action is updated duration_updates times.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L1022): Returns the sprite_affine_mat_ptr to modify.
- **final_horizontal_shear** `fixed final_horizontal_shear() const` (L1031): Returns the horizontal shear of the given sprite_affine_mat_ptr when the action is updated the given number of times.
### `bn::sprite_affine_mat_horizontal_shear_loop_action` (class, bn_sprite_affine_mat_actions.h:1046)

Modifies the horizontal shear of a sprite_affine_mat_ptr from a minimum to a maximum. When the horizontal shear is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_affine_mat_horizontal_shear_loop_action : public loop_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_horizontal_shear_manager>`

- **sprite_affine_mat_horizontal_shear_loop_action** `sprite_affine_mat_horizontal_shear_loop_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed final_horizontal_shear) : loop_value_template_action(affine_mat, duration_updates, final_horizontal_shear)` (L1059): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal shear delta.; `final_horizontal_shear` When the horizontal shear of the given sprite_affine_mat_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_affine_mat_horizontal_shear_loop_action** `sprite_affine_mat_horizontal_shear_loop_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed final_horizontal_shear) : loop_value_template_action(move(affine_mat), duration_updates, final_horizontal_shear)` (L1073): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the horizontal shear delta.; `final_horizontal_shear` When the horizontal shear of the given sprite_affine_mat_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L1082): Returns the sprite_affine_mat_ptr to modify.
- **final_horizontal_shear** `fixed final_horizontal_shear() const` (L1091): When the horizontal shear of the given sprite_affine_mat_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_affine_mat_horizontal_shear_toggle_action` (class, bn_sprite_affine_mat_actions.h:1106)

Changes the horizontal shear of a sprite_affine_mat_ptr when the action is updated a given number of times.

`class sprite_affine_mat_horizontal_shear_toggle_action : public toggle_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_horizontal_shear_manager>`

- **sprite_affine_mat_horizontal_shear_toggle_action** `sprite_affine_mat_horizontal_shear_toggle_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed new_horizontal_shear) : toggle_value_template_action(affine_mat, duration_updates, new_horizontal_shear)` (L1118): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated to change the horizontal shear of the given sprite_affine_mat_ptr.; `new_horizontal_shear` New horizontal shear when the action is updated duration_updates times.
- **sprite_affine_mat_horizontal_shear_toggle_action** `sprite_affine_mat_horizontal_shear_toggle_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed new_horizontal_shear) : toggle_value_template_action(move(affine_mat), duration_updates, new_horizontal_shear)` (L1131): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated to change the horizontal shear of the given sprite_affine_mat_ptr.; `new_horizontal_shear` New horizontal shear when the action is updated duration_updates times.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L1140): Returns the sprite_affine_mat_ptr to modify.
- **new_horizontal_shear** `fixed new_horizontal_shear() const` (L1149): Returns the horizontal shear of the given sprite_affine_mat_ptr when the action is updated the given number of times.
### `bn::sprite_affine_mat_vertical_shear_manager` (class, bn_sprite_affine_mat_actions.h:1165)

Manages the vertical shear of a sprite_affine_mat_ptr.

`class sprite_affine_mat_vertical_shear_manager`

- **get** `static fixed get(const sprite_affine_mat_ptr& affine_mat)` (L1172): Returns the vertical shear of the given sprite_affine_mat_ptr.
- **set** `static void set(fixed vertical_shear, sprite_affine_mat_ptr& affine_mat)` (L1180): Sets the vertical shear of the given sprite_affine_mat_ptr.
### `bn::sprite_affine_mat_vertical_shear_to_action` (class, bn_sprite_affine_mat_actions.h:1194)

Modifies the vertical shear of a sprite_affine_mat_ptr until it has a given state.

`class sprite_affine_mat_vertical_shear_to_action : public to_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_vertical_shear_manager>`

- **sprite_affine_mat_vertical_shear_to_action** `sprite_affine_mat_vertical_shear_to_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed final_vertical_shear) : to_value_template_action(affine_mat, duration_updates, final_vertical_shear)` (L1206): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` Number of times that the action must be updated until the vertical shear of the given sprite_affine_mat_ptr is equal to final_vertical_shear.; `final_vertical_shear` Vertical shear when the action is updated duration_updates times.
- **sprite_affine_mat_vertical_shear_to_action** `sprite_affine_mat_vertical_shear_to_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed final_vertical_shear) : to_value_template_action(move(affine_mat), duration_updates, final_vertical_shear)` (L1219): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` Number of times that the action must be updated until the vertical shear of the given sprite_affine_mat_ptr is equal to final_vertical_shear.; `final_vertical_shear` Vertical shear when the action is updated duration_updates times.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L1228): Returns the sprite_affine_mat_ptr to modify.
- **final_vertical_shear** `fixed final_vertical_shear() const` (L1237): Returns the vertical shear of the given sprite_affine_mat_ptr when the action is updated the given number of times.
### `bn::sprite_affine_mat_vertical_shear_loop_action` (class, bn_sprite_affine_mat_actions.h:1252)

Modifies the vertical shear of a sprite_affine_mat_ptr from a minimum to a maximum. When the vertical shear is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_affine_mat_vertical_shear_loop_action : public loop_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_vertical_shear_manager>`

- **sprite_affine_mat_vertical_shear_loop_action** `sprite_affine_mat_vertical_shear_loop_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed final_vertical_shear) : loop_value_template_action(affine_mat, duration_updates, final_vertical_shear)` (L1265): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical shear delta.; `final_vertical_shear` When the vertical shear of the given sprite_affine_mat_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_affine_mat_vertical_shear_loop_action** `sprite_affine_mat_vertical_shear_loop_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed final_vertical_shear) : loop_value_template_action(move(affine_mat), duration_updates, final_vertical_shear)` (L1279): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the vertical shear delta.; `final_vertical_shear` When the vertical shear of the given sprite_affine_mat_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L1288): Returns the sprite_affine_mat_ptr to modify.
- **final_vertical_shear** `fixed final_vertical_shear() const` (L1297): When the vertical shear of the given sprite_affine_mat_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_affine_mat_vertical_shear_toggle_action` (class, bn_sprite_affine_mat_actions.h:1312)

Changes the vertical shear of a sprite_affine_mat_ptr when the action is updated a given number of times.

`class sprite_affine_mat_vertical_shear_toggle_action : public toggle_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_vertical_shear_manager>`

- **sprite_affine_mat_vertical_shear_toggle_action** `sprite_affine_mat_vertical_shear_toggle_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed new_vertical_shear) : toggle_value_template_action(affine_mat, duration_updates, new_vertical_shear)` (L1324): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated to change the vertical shear of the given sprite_affine_mat_ptr.; `new_vertical_shear` New vertical shear when the action is updated duration_updates times.
- **sprite_affine_mat_vertical_shear_toggle_action** `sprite_affine_mat_vertical_shear_toggle_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed new_vertical_shear) : toggle_value_template_action(move(affine_mat), duration_updates, new_vertical_shear)` (L1337): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated to change the vertical shear of the given sprite_affine_mat_ptr.; `new_vertical_shear` New vertical shear when the action is updated duration_updates times.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L1346): Returns the sprite_affine_mat_ptr to modify.
- **new_vertical_shear** `fixed new_vertical_shear() const` (L1355): Returns the vertical shear of the given sprite_affine_mat_ptr when the action is updated the given number of times.
### `bn::sprite_affine_mat_shear_manager` (class, bn_sprite_affine_mat_actions.h:1371)

Manages the shear of a sprite_affine_mat_ptr.

`class sprite_affine_mat_shear_manager`

- **get** `static fixed get(const sprite_affine_mat_ptr& affine_mat)` (L1378): Returns the horizontal shear of the given sprite_affine_mat_ptr.
- **set** `static void set(fixed shear, sprite_affine_mat_ptr& affine_mat)` (L1386): Sets the shear of the given sprite_affine_mat_ptr.
### `bn::sprite_affine_mat_shear_to_action` (class, bn_sprite_affine_mat_actions.h:1400)

Modifies the shear of a sprite_affine_mat_ptr until it has a given state.

`class sprite_affine_mat_shear_to_action : public to_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_shear_manager>`

- **sprite_affine_mat_shear_to_action** `sprite_affine_mat_shear_to_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed final_shear) : to_value_template_action(affine_mat, duration_updates, final_shear)` (L1412): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` Number of times that the action must be updated until the shear of the given sprite_affine_mat_ptr is equal to final_shear.; `final_shear` Shear when the action is updated duration_updates times.
- **sprite_affine_mat_shear_to_action** `sprite_affine_mat_shear_to_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed final_shear) : to_value_template_action(move(affine_mat), duration_updates, final_shear)` (L1425): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` Number of times that the action must be updated until the shear of the given sprite_affine_mat_ptr is equal to final_shear.; `final_shear` Shear when the action is updated duration_updates times.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L1433): Returns the sprite_affine_mat_ptr to modify.
- **final_shear** `fixed final_shear() const` (L1442): Returns the shear of the given sprite_affine_mat_ptr when the action is updated the given number of times.
### `bn::sprite_affine_mat_shear_loop_action` (class, bn_sprite_affine_mat_actions.h:1457)

Modifies the shear of a sprite_affine_mat_ptr from a minimum to a maximum. When the shear is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_affine_mat_shear_loop_action : public loop_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_shear_manager>`

- **sprite_affine_mat_shear_loop_action** `sprite_affine_mat_shear_loop_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed final_shear) : loop_value_template_action(affine_mat, duration_updates, final_shear)` (L1470): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the shear delta.; `final_shear` When the shear of the given sprite_affine_mat_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **sprite_affine_mat_shear_loop_action** `sprite_affine_mat_shear_loop_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed final_shear) : loop_value_template_action(move(affine_mat), duration_updates, final_shear)` (L1484): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the shear delta.; `final_shear` When the shear of the given sprite_affine_mat_ptr is equal to this parameter, it goes back to its initial state and vice versa.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L1492): Returns the sprite_affine_mat_ptr to modify.
- **final_shear** `fixed final_shear() const` (L1501): When the shear of the given sprite_affine_mat_ptr is equal to this returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_affine_mat_shear_toggle_action` (class, bn_sprite_affine_mat_actions.h:1516)

Changes the shear of a sprite_affine_mat_ptr when the action is updated a given number of times.

`class sprite_affine_mat_shear_toggle_action : public toggle_value_template_action<sprite_affine_mat_ptr, fixed, sprite_affine_mat_shear_manager>`

- **sprite_affine_mat_shear_toggle_action** `sprite_affine_mat_shear_toggle_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates, fixed new_shear) : toggle_value_template_action(affine_mat, duration_updates, new_shear)` (L1528): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated to change the shear of the given sprite_affine_mat_ptr.; `new_shear` New shear when the action is updated duration_updates times.
- **sprite_affine_mat_shear_toggle_action** `sprite_affine_mat_shear_toggle_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates, fixed new_shear) : toggle_value_template_action(move(affine_mat), duration_updates, new_shear)` (L1541): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated to change the shear of the given sprite_affine_mat_ptr.; `new_shear` New shear when the action is updated duration_updates times.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L1549): Returns the sprite_affine_mat_ptr to modify.
- **new_shear** `fixed new_shear() const` (L1558): Returns the shear of the given sprite_affine_mat_ptr when the action is updated the given number of times.
### `bn::sprite_affine_mat_horizontal_flip_manager` (class, bn_sprite_affine_mat_actions.h:1574)

Manages if a sprite_affine_mat_ptr is flipped in its horizontal axis or not.

`class sprite_affine_mat_horizontal_flip_manager`

- **get** `static bool get(const sprite_affine_mat_ptr& affine_mat)` (L1581): Indicates if the given sprite_affine_mat_ptr is flipped in the horizontal axis or not.
- **set** `static void set(bool horizontal_flip, sprite_affine_mat_ptr& affine_mat)` (L1589): Sets if the given sprite_affine_mat_ptr must be flipped in the horizontal axis or not.
### `bn::sprite_affine_mat_horizontal_flip_toggle_action` (class, bn_sprite_affine_mat_actions.h:1604)

Toggles if a sprite_affine_mat_ptr must be flipped in the horizontal axis or not when the action is updated a given number of times.

`class sprite_affine_mat_horizontal_flip_toggle_action : public bool_toggle_value_template_action<sprite_affine_mat_ptr, sprite_affine_mat_horizontal_flip_manager>`

- **sprite_affine_mat_horizontal_flip_toggle_action** `sprite_affine_mat_horizontal_flip_toggle_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates) : bool_toggle_value_template_action(affine_mat, duration_updates)` (L1615): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given sprite_affine_mat_ptr must be flipped in the horizontal axis or not.
- **sprite_affine_mat_horizontal_flip_toggle_action** `sprite_affine_mat_horizontal_flip_toggle_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates) : bool_toggle_value_template_action(move(affine_mat), duration_updates)` (L1626): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given sprite_affine_mat_ptr must be flipped in the horizontal axis or not.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L1634): Returns the sprite_affine_mat_ptr to modify.
### `bn::sprite_affine_mat_vertical_flip_manager` (class, bn_sprite_affine_mat_actions.h:1650)

Manages if a sprite_affine_mat_ptr is flipped in its vertical axis or not.

`class sprite_affine_mat_vertical_flip_manager`

- **get** `static bool get(const sprite_affine_mat_ptr& affine_mat)` (L1657): Indicates if the given sprite_affine_mat_ptr is flipped in the vertical axis or not.
- **set** `static void set(bool vertical_flip, sprite_affine_mat_ptr& affine_mat)` (L1665): Sets if the given sprite_affine_mat_ptr must be flipped in the vertical axis or not.
### `bn::sprite_affine_mat_vertical_flip_toggle_action` (class, bn_sprite_affine_mat_actions.h:1680)

Toggles if a sprite_affine_mat_ptr must be flipped in the vertical axis or not when the action is updated a given number of times.

`class sprite_affine_mat_vertical_flip_toggle_action : public bool_toggle_value_template_action<sprite_affine_mat_ptr, sprite_affine_mat_vertical_flip_manager>`

- **sprite_affine_mat_vertical_flip_toggle_action** `sprite_affine_mat_vertical_flip_toggle_action(const sprite_affine_mat_ptr& affine_mat, int duration_updates) : bool_toggle_value_template_action(affine_mat, duration_updates)` (L1691): Constructor.
  `affine_mat` sprite_affine_mat_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the given sprite_affine_mat_ptr must be flipped in the vertical axis or not.
- **sprite_affine_mat_vertical_flip_toggle_action** `sprite_affine_mat_vertical_flip_toggle_action(sprite_affine_mat_ptr&& affine_mat, int duration_updates) : bool_toggle_value_template_action(move(affine_mat), duration_updates)` (L1702): Constructor.
  `affine_mat` sprite_affine_mat_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the given sprite_affine_mat_ptr must be flipped in the vertical axis or not.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L1710): Returns the sprite_affine_mat_ptr to modify.

## bn_sprite_animate_actions.h

### `bn::isprite_animate_action` (class, bn_sprite_animate_actions.h:39)

Base class of bn::sprite_animate_action.

`class isprite_animate_action`

Can be used as a reference type for all bn::sprite_animate_action objects.

- **operator=** `isprite_animate_action& operator=(const isprite_animate_action& other)` (L50): Copy assignment operator.
  `other` isprite_animate_action to copy.; devuelve: Reference to this.
- **operator=** `isprite_animate_action& operator=(isprite_animate_action&& other) noexcept` (L57): Move assignment operator.
  `other` isprite_animate_action to move.; devuelve: Reference to this.
- **update** `void update()` (L62): Changes the tile set of the given sprite_ptr when the given amount of update calls are done.
- **done** `bool done() const` (L67): Indicates if the action must not be updated anymore.
- **reset** `void reset()` (L75): Resets the action to its initial state.
- **sprite** `const sprite_ptr& sprite() const` (L84): Returns the sprite_ptr to modify.
- **wait_updates** `int wait_updates() const` (L93): Returns the number of times the action must be updated before changing the tiles of the given sprite_ptr.
- **set_wait_updates** `void set_wait_updates(int wait_updates)` (L102): Sets the number of times the action must be updated before changing the tiles of the given sprite_ptr.
- **next_change_updates** `int next_change_updates() const` (L107): Returns the number of times the action must be updated before the next tiles change.
- **set_next_change_updates** `void set_next_change_updates(int next_change_updates)` (L115): Sets the number of times the action must be updated before the next tiles change.
- **tiles_item** `const sprite_tiles_item& tiles_item() const` (L120): Returns the sprite_tiles_item used to create the new sprite tiles to use by the given sprite_ptr.
- **graphics_indexes** `const ivector<uint16_t>& graphics_indexes() const` (L128): Returns the indexes of the tile sets to reference in the given sprite_tiles_item.
- **update_forever** `bool update_forever() const` (L136): Indicates if the action can be updated forever or not.
- **set_update_forever** `void set_update_forever(bool forever)` (L144): Specifies if the action must be updated forever or not.
- **current_index** `int current_index() const` (L152): Returns the current index of the given graphics_indexes (not the current index of the tile set to reference in the given tiles_item).
  This index is the one to be used on the next tiles change. It can be out of bounds if the action is done.
- **set_current_index** `void set_current_index(int current_index)` (L161): Sets the current index of the given graphics_indexes (not the current index of the tile set to reference in the given tiles_item).
- **set_current_index** `void set_current_index(int current_index, bool update_sprite_now)` (L172): Sets the current index of the given graphics_indexes (not the current index of the tile set to reference in the given tiles_item).
  `current_index` New index of the given graphics_indexes.; `update_sprite_now` Specifies if the sprite must be updated now or on the next tiles change.
- **current_graphics_index** `int current_graphics_index() const` (L179): Returns the current index of the tile set to reference in the given tiles_item.
  This index is the one to be used on the next tiles change.
- **once** `static sprite_animate_action once( const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, const span<const uint16_t>& graphics_indexes)` (L225): Generates a sprite_animate_action which loops over the given sprite tile sets only once.
  `sprite` sprite_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in tiles_item.; devuelve: The requested sprite_animate_action.
- **once** `static sprite_animate_action once( sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, const span<const uint16_t>& graphics_indexes)` (L240): Generates a sprite_animate_action which loops over the given sprite tile sets only once.
  `sprite` sprite_ptr to move.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in the given sprite_tiles_item.; devuelve: The requested sprite_animate_action.
- **forever** `static sprite_animate_action forever( const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, const span<const uint16_t>& graphics_indexes)` (L255): Generates a sprite_animate_action which loops over the given sprite tile sets forever.
  `sprite` sprite_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in the given sprite_tiles_item.; devuelve: The requested sprite_animate_action.
- **forever** `static sprite_animate_action forever( sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, const span<const uint16_t>& graphics_indexes)` (L270): Generates a sprite_animate_action which loops over the given sprite tile sets forever.
  `sprite` sprite_ptr to move.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in the given sprite_tiles_item.; devuelve: The requested sprite_animate_action.
- **sprite_animate_action** `sprite_animate_action(const sprite_animate_action& other)` (L281): Copy constructor.
  `other` sprite_animate_action to copy.
- **sprite_animate_action** `sprite_animate_action(sprite_animate_action&& other) noexcept : _sprite(move(other._sprite)), _tiles_item(other._tiles_item), _graphics_indexes(other._graphics_indexes)` (L294): Move constructor.
  `other` sprite_animate_action to move.
- **sprite_animate_action** `sprite_animate_action(const isprite_animate_action& other)` (L307): Copy constructor.
  `other` isprite_animate_action to copy.
- **operator=** `sprite_animate_action& operator=(const sprite_animate_action& other)` (L324): Copy assignment operator.
  `other` sprite_animate_action to copy.; devuelve: Reference to this.
- **operator=** `sprite_animate_action& operator=(sprite_animate_action&& other) noexcept` (L342): Move assignment operator.
  `other` sprite_animate_action to move.; devuelve: Reference to this.
- **operator=** `sprite_animate_action& operator=(const isprite_animate_action& other)` (L360): Copy assignment operator.
  `other` isprite_animate_action to copy.; devuelve: Reference to this.
- **operator=** `sprite_animate_action& operator=(isprite_animate_action&& other) noexcept` (L371): Move assignment operator.
  `other` ivector to move.; devuelve: Reference to this.
- **create_sprite_animate_action_once** `template<typename ...Args> auto create_sprite_animate_action_once( const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)` (L416): Generates a sprite_animate_action which loops over the given sprite tile sets only once.
  `sprite` sprite_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in tiles_item.; devuelve: The requested sprite_animate_action.
- **create_sprite_animate_action_once** `template<typename ...Args> auto create_sprite_animate_action_once( sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)` (L436): Generates a sprite_animate_action which loops over the given sprite tile sets only once.
  `sprite` sprite_ptr to move.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in tiles_item.; devuelve: The requested sprite_animate_action.
- **create_sprite_animate_action_forever** `template<typename ...Args> auto create_sprite_animate_action_forever( const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)` (L456): Generates a sprite_animate_action which loops over the given sprite tile sets forever.
  `sprite` sprite_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in tiles_item.; devuelve: The requested sprite_animate_action.
- **create_sprite_animate_action_forever** `template<typename ...Args> auto create_sprite_animate_action_forever( sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)` (L476): Generates a sprite_animate_action which loops over the given sprite tile sets forever.
  `sprite` sprite_ptr to move.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in tiles_item.; devuelve: The requested sprite_animate_action.
### `bn::isprite_cached_animate_action` (class, bn_sprite_animate_actions.h:497)

Base class of bn::sprite_cached_animate_action.

`class isprite_cached_animate_action`

Can be used as a reference type for all bn::sprite_cached_animate_action objects.

- **operator=** `isprite_cached_animate_action& operator=(const isprite_cached_animate_action& other)` (L508): Copy assignment operator.
  `other` isprite_cached_animate_action to copy.; devuelve: Reference to this.
- **operator=** `isprite_cached_animate_action& operator=(isprite_cached_animate_action&& other) noexcept` (L515): Move assignment operator.
  `other` isprite_cached_animate_action to move.; devuelve: Reference to this.
- **update** `void update()` (L520): Changes the tile set of the given sprite_ptr when the given amount of update calls are done.
- **done** `bool done() const` (L525): Indicates if the action must not be updated anymore.
- **reset** `void reset()` (L533): Resets the action to its initial state.
- **sprite** `const sprite_ptr& sprite() const` (L542): Returns the sprite_ptr to modify.
- **wait_updates** `int wait_updates() const` (L551): Returns the number of times the action must be updated before changing the tiles of the given sprite_ptr.
- **set_wait_updates** `void set_wait_updates(int wait_updates)` (L560): Sets the number of times the action must be updated before changing the tiles of the given sprite_ptr.
- **next_change_updates** `int next_change_updates() const` (L565): Returns the number of times the action must be updated before the next tiles change.
- **set_next_change_updates** `void set_next_change_updates(int next_change_updates)` (L573): Sets the number of times the action must be updated before the next tiles change.
- **tiles_list** `const ivector<sprite_tiles_ptr>& tiles_list() const` (L578): Returns the tile sets to use by the given sprite_ptr.
- **update_forever** `bool update_forever() const` (L586): Indicates if the action can be updated forever or not.
- **set_update_forever** `void set_update_forever(bool forever)` (L594): Specifies if the action must be updated forever or not.
- **current_index** `int current_index() const` (L601): Returns the current index of the given tile sets.
  This index is the one to be used on the next tiles change. It can be out of bounds if the action is done.
- **set_current_index** `void set_current_index(int current_index)` (L609): Sets the current index of the given tile sets.
- **set_current_index** `void set_current_index(int current_index, bool update_sprite_now)` (L619): Sets the current index of the given tile sets.
  `current_index` New index of the given tile sets.; `update_sprite_now` Specifies if the sprite must be updated now or on the next tiles change.
- **current_tiles** `const sprite_tiles_ptr& current_tiles() const` (L626): Returns the current tile set.
  This tile set is the one to be used on the next tiles change.
- **once** `static sprite_cached_animate_action once( const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, const span<const uint16_t>& graphics_indexes)` (L676): Generates a sprite_cached_animate_action which loops over the given sprite tile sets only once.
  `sprite` sprite_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in tiles_item.; devuelve: The requested sprite_cached_animate_action.
- **once** `static sprite_cached_animate_action once( sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, const span<const uint16_t>& graphics_indexes)` (L691): Generates a sprite_cached_animate_action which loops over the given sprite tile sets only once.
  `sprite` sprite_ptr to move.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in the given sprite_tiles_item.; devuelve: The requested sprite_cached_animate_action.
- **once** `static sprite_cached_animate_action once( const sprite_ptr& sprite, int wait_updates, span<sprite_tiles_ptr> tiles_list)` (L705): Generates a sprite_cached_animate_action which loops over the given sprite tile sets only once.
  `sprite` sprite_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_list` List of tile sets to use by the given sprite_ptr.; devuelve: The requested sprite_cached_animate_action.
- **once** `static sprite_cached_animate_action once( sprite_ptr&& sprite, int wait_updates, span<sprite_tiles_ptr> tiles_list)` (L718): Generates a sprite_cached_animate_action which loops over the given sprite tile sets only once.
  `sprite` sprite_ptr to move.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_list` List of tile sets to use by the given sprite_ptr.; devuelve: The requested sprite_cached_animate_action.
- **forever** `static sprite_cached_animate_action forever( const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, const span<const uint16_t>& graphics_indexes)` (L732): Generates a sprite_cached_animate_action which loops over the given sprite tile sets forever.
  `sprite` sprite_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in tiles_item.; devuelve: The requested sprite_cached_animate_action.
- **forever** `static sprite_cached_animate_action forever( sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, const span<const uint16_t>& graphics_indexes)` (L747): Generates a sprite_cached_animate_action which loops over the given sprite tile sets forever.
  `sprite` sprite_ptr to move.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in the given sprite_tiles_item.; devuelve: The requested sprite_cached_animate_action.
- **forever** `static sprite_cached_animate_action forever( const sprite_ptr& sprite, int wait_updates, span<sprite_tiles_ptr> tiles_list)` (L761): Generates a sprite_cached_animate_action which loops over the given sprite tile sets forever.
  `sprite` sprite_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_list` List of tile sets to use by the given sprite_ptr.; devuelve: The requested sprite_cached_animate_action.
- **forever** `static sprite_cached_animate_action forever( sprite_ptr&& sprite, int wait_updates, span<sprite_tiles_ptr> tiles_list)` (L774): Generates a sprite_cached_animate_action which loops over the given sprite tile sets forever.
  `sprite` sprite_ptr to move.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_list` List of tile sets to use by the given sprite_ptr.; devuelve: The requested sprite_cached_animate_action.
- **sprite_cached_animate_action** `sprite_cached_animate_action(const sprite_cached_animate_action& other)` (L784): Copy constructor.
  `other` sprite_cached_animate_action to copy.
- **sprite_cached_animate_action** `sprite_cached_animate_action(sprite_cached_animate_action&& other) noexcept : _sprite(move(other._sprite)), _tiles_list(move(other._tiles_list))` (L796): Move constructor.
  `other` sprite_cached_animate_action to move.
- **sprite_cached_animate_action** `sprite_cached_animate_action(const isprite_cached_animate_action& other)` (L808): Copy constructor.
  `other` isprite_cached_animate_action to copy.
- **operator=** `sprite_cached_animate_action& operator=(const sprite_cached_animate_action& other)` (L824): Copy assignment operator.
  `other` sprite_cached_animate_action to copy.; devuelve: Reference to this.
- **operator=** `sprite_cached_animate_action& operator=(sprite_cached_animate_action&& other) noexcept` (L841): Move assignment operator.
  `other` sprite_cached_animate_action to move.; devuelve: Reference to this.
- **operator=** `sprite_cached_animate_action& operator=(const isprite_cached_animate_action& other)` (L858): Copy assignment operator.
  `other` isprite_cached_animate_action to copy.; devuelve: Reference to this.
- **operator=** `sprite_cached_animate_action& operator=(isprite_cached_animate_action&& other) noexcept` (L869): Move assignment operator.
  `other` ivector to move.; devuelve: Reference to this.
- **create_sprite_cached_animate_action_once** `template<typename ...Args> auto create_sprite_cached_animate_action_once( const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)` (L931): Generates a sprite_cached_animate_action which loops over the given sprite tile sets only once.
  `sprite` sprite_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in tiles_item.; devuelve: The requested sprite_cached_animate_action.
- **create_sprite_cached_animate_action_once** `template<typename ...Args> auto create_sprite_cached_animate_action_once( sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)` (L951): Generates a sprite_cached_animate_action which loops over the given sprite tile sets only once.
  `sprite` sprite_ptr to move.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in tiles_item.; devuelve: The requested sprite_cached_animate_action.
- **create_sprite_cached_animate_action_forever** `template<typename ...Args> auto create_sprite_cached_animate_action_forever( const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)` (L971): Generates a sprite_cached_animate_action which loops over the given sprite tile sets forever.
  `sprite` sprite_ptr to copy.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in tiles_item.; devuelve: The requested sprite_cached_animate_action.
- **create_sprite_cached_animate_action_forever** `template<typename ...Args> auto create_sprite_cached_animate_action_forever( sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)` (L991): Generates a sprite_cached_animate_action which loops over the given sprite tile sets forever.
  `sprite` sprite_ptr to move.; `wait_updates` Number of times the action must be updated before changing the tiles of the given sprite_ptr.; `tiles_item` It creates the new sprite tiles to use by the given sprite_ptr.; `graphics_indexes` Indexes of the tile sets to reference in tiles_item.; devuelve: The requested sprite_cached_animate_action.

## bn_sprite_animate_actions_fwd.h

### `bn::sprite_animate_action` (class, bn_sprite_animate_actions_fwd.h:38)

Changes the tile set of a sprite_ptr when the action is updated a given number of times.

`template<int MaxSize> class sprite_animate_action`

Each time the tile set of a sprite_ptr must be changed, it is searched for and created if it has not been found,
so the tile sets are not cached.

### `bn::sprite_cached_animate_action` (class, bn_sprite_animate_actions_fwd.h:58)

Changes the tile set of a sprite_ptr when the action is updated a given number of times.

`template<int MaxSize> class sprite_cached_animate_action`

This action differs from sprite_animate_action in that all sprite tile sets to use are cached,
so it is faster but takes way more VRAM.


## bn_sprite_palette_actions.h

### `bn::sprite_palette_inverted_manager` (class, bn_sprite_palette_actions.h:34)

Manages if the colors of a sprite_palette_ptr must be inverted or not.

`class sprite_palette_inverted_manager`

- **get** `static bool get(const sprite_palette_ptr& palette)` (L41): Indicates if the colors of the given sprite_palette_ptr are inverted or not.
- **set** `static void set(bool inverted, sprite_palette_ptr& palette)` (L49): Sets if the colors of the given sprite_palette_ptr must be inverted or not.
### `bn::sprite_palette_inverted_toggle_action` (class, bn_sprite_palette_actions.h:64)

Toggles if the colors of a sprite_palette_ptr must be inverted or not when the action is updated a given number of times.

`class sprite_palette_inverted_toggle_action : public bool_toggle_value_template_action<sprite_palette_ptr, sprite_palette_inverted_manager>`

- **sprite_palette_inverted_toggle_action** `sprite_palette_inverted_toggle_action(const sprite_palette_ptr& palette, int duration_updates) : bool_toggle_value_template_action(palette, duration_updates)` (L75): Constructor.
  `palette` sprite_palette_ptr to copy.; `duration_updates` How many times the action has to be updated to toggle if the colors of the given sprite_palette_ptr must be inverted or not.
- **sprite_palette_inverted_toggle_action** `sprite_palette_inverted_toggle_action(sprite_palette_ptr&& palette, int duration_updates) : bool_toggle_value_template_action(move(palette), duration_updates)` (L86): Constructor.
  `palette` sprite_palette_ptr to move.; `duration_updates` How many times the action has to be updated to toggle if the colors of the given sprite_palette_ptr must be inverted or not.
- **palette** `const sprite_palette_ptr& palette() const` (L94): Returns the sprite_palette_ptr to modify.
### `bn::sprite_palette_grayscale_manager` (class, bn_sprite_palette_actions.h:110)

Manages the intensity of the grayscale effect applied to a sprite_palette_ptr.

`class sprite_palette_grayscale_manager`

- **get** `static fixed get(const sprite_palette_ptr& palette)` (L117): Returns the intensity of the grayscale effect applied to the given sprite_palette_ptr.
- **set** `static void set(fixed intensity, sprite_palette_ptr& palette)` (L127): Sets the intensity of the grayscale effect applied to the given sprite_palette_ptr.
  `intensity` New intensity in the range [0..1].; `palette` sprite_palette_ptr to modify.
### `bn::sprite_palette_grayscale_to_action` (class, bn_sprite_palette_actions.h:142)

Modifies the intensity of the grayscale effect applied to a sprite_palette_ptr until it has a given state.

`class sprite_palette_grayscale_to_action : public to_value_template_action<sprite_palette_ptr, fixed, sprite_palette_grayscale_manager>`

- **sprite_palette_grayscale_to_action** `sprite_palette_grayscale_to_action(const sprite_palette_ptr& palette, int duration_updates, fixed final_intensity) : to_value_template_action(palette, duration_updates, final_intensity)` (L156): Constructor.
  `palette` sprite_palette_ptr to copy.; `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **sprite_palette_grayscale_to_action** `sprite_palette_grayscale_to_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) : to_value_template_action(move(palette), duration_updates, final_intensity)` (L172): Constructor.
  `palette` sprite_palette_ptr to move.; `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **palette** `const sprite_palette_ptr& palette() const` (L181): Returns the sprite_palette_ptr to modify.
- **final_intensity** `fixed final_intensity() const` (L189): Returns the intensity when the action is updated the given number of times.
### `bn::sprite_palette_grayscale_loop_action` (class, bn_sprite_palette_actions.h:205)

Modifies the intensity of the grayscale effect applied to a sprite_palette_ptr from a minimum to a maximum. When the intensity is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_palette_grayscale_loop_action : public loop_value_template_action<sprite_palette_ptr, fixed, sprite_palette_grayscale_manager>`

- **sprite_palette_grayscale_loop_action** `sprite_palette_grayscale_loop_action(const sprite_palette_ptr& palette, int duration_updates, fixed final_intensity) : loop_value_template_action(palette, duration_updates, final_intensity)` (L220): Constructor.
  `palette` sprite_palette_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **sprite_palette_grayscale_loop_action** `sprite_palette_grayscale_loop_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) : loop_value_template_action(move(palette), duration_updates, final_intensity)` (L237): Constructor.
  `palette` sprite_palette_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **palette** `const sprite_palette_ptr& palette() const` (L246): Returns the sprite_palette_ptr to modify.
- **final_intensity** `fixed final_intensity() const` (L255): When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_palette_grayscale_toggle_action` (class, bn_sprite_palette_actions.h:270)

Changes the intensity of the grayscale effect applied to a sprite_palette_ptr when the action is updated a given number of times.

`class sprite_palette_grayscale_toggle_action : public toggle_value_template_action<sprite_palette_ptr, fixed, sprite_palette_grayscale_manager>`

- **sprite_palette_grayscale_toggle_action** `sprite_palette_grayscale_toggle_action(const sprite_palette_ptr& palette, int duration_updates, fixed new_intensity) : toggle_value_template_action(palette, duration_updates, new_intensity)` (L283): Constructor.
  `palette` sprite_palette_ptr to copy.; `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **sprite_palette_grayscale_toggle_action** `sprite_palette_grayscale_toggle_action(sprite_palette_ptr&& palette, int duration_updates, fixed new_intensity) : toggle_value_template_action(move(palette), duration_updates, new_intensity)` (L298): Constructor.
  `palette` sprite_palette_ptr to move.; `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **palette** `const sprite_palette_ptr& palette() const` (L307): Returns the sprite_palette_ptr to modify.
- **new_intensity** `fixed new_intensity() const` (L315): Returns the intensity when the action is updated the given number of times.
### `bn::sprite_palette_hue_shift_manager` (class, bn_sprite_palette_actions.h:331)

Manages the intensity of the hue shift effect applied to a sprite_palette_ptr.

`class sprite_palette_hue_shift_manager`

- **get** `static fixed get(const sprite_palette_ptr& palette)` (L338): Returns the intensity of the hue shift effect applied to the given sprite_palette_ptr.
- **set** `static void set(fixed intensity, sprite_palette_ptr& palette)` (L348): Sets the intensity of the hue shift effect applied to the given sprite_palette_ptr.
  `intensity` New intensity in the range [0..1].; `palette` sprite_palette_ptr to modify.
### `bn::sprite_palette_hue_shift_to_action` (class, bn_sprite_palette_actions.h:363)

Modifies the intensity of the hue shift effect applied to a sprite_palette_ptr until it has a given state.

`class sprite_palette_hue_shift_to_action : public to_value_template_action<sprite_palette_ptr, fixed, sprite_palette_hue_shift_manager>`

- **sprite_palette_hue_shift_to_action** `sprite_palette_hue_shift_to_action(const sprite_palette_ptr& palette, int duration_updates, fixed final_intensity) : to_value_template_action(palette, duration_updates, final_intensity)` (L377): Constructor.
  `palette` sprite_palette_ptr to copy.; `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **sprite_palette_hue_shift_to_action** `sprite_palette_hue_shift_to_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) : to_value_template_action(move(palette), duration_updates, final_intensity)` (L393): Constructor.
  `palette` sprite_palette_ptr to move.; `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **palette** `const sprite_palette_ptr& palette() const` (L402): Returns the sprite_palette_ptr to modify.
- **final_intensity** `fixed final_intensity() const` (L410): Returns the intensity when the action is updated the given number of times.
### `bn::sprite_palette_hue_shift_loop_action` (class, bn_sprite_palette_actions.h:426)

Modifies the intensity of the hue shift effect applied to a sprite_palette_ptr from a minimum to a maximum. When the intensity is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_palette_hue_shift_loop_action : public loop_value_template_action<sprite_palette_ptr, fixed, sprite_palette_hue_shift_manager>`

- **sprite_palette_hue_shift_loop_action** `sprite_palette_hue_shift_loop_action(const sprite_palette_ptr& palette, int duration_updates, fixed final_intensity) : loop_value_template_action(palette, duration_updates, final_intensity)` (L441): Constructor.
  `palette` sprite_palette_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **sprite_palette_hue_shift_loop_action** `sprite_palette_hue_shift_loop_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) : loop_value_template_action(move(palette), duration_updates, final_intensity)` (L458): Constructor.
  `palette` sprite_palette_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **palette** `const sprite_palette_ptr& palette() const` (L467): Returns the sprite_palette_ptr to modify.
- **final_intensity** `fixed final_intensity() const` (L476): When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_palette_hue_shift_toggle_action` (class, bn_sprite_palette_actions.h:491)

Changes the intensity of the hue shift effect applied to a sprite_palette_ptr when the action is updated a given number of times.

`class sprite_palette_hue_shift_toggle_action : public toggle_value_template_action<sprite_palette_ptr, fixed, sprite_palette_hue_shift_manager>`

- **sprite_palette_hue_shift_toggle_action** `sprite_palette_hue_shift_toggle_action(const sprite_palette_ptr& palette, int duration_updates, fixed new_intensity) : toggle_value_template_action(palette, duration_updates, new_intensity)` (L504): Constructor.
  `palette` sprite_palette_ptr to copy.; `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **sprite_palette_hue_shift_toggle_action** `sprite_palette_hue_shift_toggle_action(sprite_palette_ptr&& palette, int duration_updates, fixed new_intensity) : toggle_value_template_action(move(palette), duration_updates, new_intensity)` (L519): Constructor.
  `palette` sprite_palette_ptr to move.; `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **palette** `const sprite_palette_ptr& palette() const` (L528): Returns the sprite_palette_ptr to modify.
- **new_intensity** `fixed new_intensity() const` (L536): Returns the intensity when the action is updated the given number of times.
### `bn::sprite_palette_fade_manager` (class, bn_sprite_palette_actions.h:552)

Manages the intensity of the fade effect applied to a sprite_palette_ptr.

`class sprite_palette_fade_manager`

- **get** `static fixed get(const sprite_palette_ptr& palette)` (L559): Returns the intensity of the fade effect applied to the given sprite_palette_ptr.
- **set** `static void set(fixed intensity, sprite_palette_ptr& palette)` (L569): Sets the intensity of the fade effect applied to the given sprite_palette_ptr.
  `intensity` New intensity in the range [0..1].; `palette` sprite_palette_ptr to modify.
### `bn::sprite_palette_fade_to_action` (class, bn_sprite_palette_actions.h:584)

Modifies the intensity of the fade effect applied to a sprite_palette_ptr until it has a given state.

`class sprite_palette_fade_to_action : public to_value_template_action<sprite_palette_ptr, fixed, sprite_palette_fade_manager>`

- **sprite_palette_fade_to_action** `sprite_palette_fade_to_action(const sprite_palette_ptr& palette, int duration_updates, fixed final_intensity) : to_value_template_action(palette, duration_updates, final_intensity)` (L598): Constructor.
  `palette` sprite_palette_ptr to copy.; `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **sprite_palette_fade_to_action** `sprite_palette_fade_to_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) : to_value_template_action(move(palette), duration_updates, final_intensity)` (L613): Constructor.
  `palette` sprite_palette_ptr to move.; `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **palette** `const sprite_palette_ptr& palette() const` (L622): Returns the sprite_palette_ptr to modify.
- **final_intensity** `fixed final_intensity() const` (L630): Returns the intensity when the action is updated the given number of times.
### `bn::sprite_palette_fade_loop_action` (class, bn_sprite_palette_actions.h:646)

Modifies the intensity of the fade effect applied to a sprite_palette_ptr from a minimum to a maximum. When the intensity is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_palette_fade_loop_action : public loop_value_template_action<sprite_palette_ptr, fixed, sprite_palette_fade_manager>`

- **sprite_palette_fade_loop_action** `sprite_palette_fade_loop_action(const sprite_palette_ptr& palette, int duration_updates, fixed final_intensity) : loop_value_template_action(palette, duration_updates, final_intensity)` (L661): Constructor.
  `palette` sprite_palette_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **sprite_palette_fade_loop_action** `sprite_palette_fade_loop_action(sprite_palette_ptr&& palette, int duration_updates, fixed final_intensity) : loop_value_template_action(move(palette), duration_updates, final_intensity)` (L677): Constructor.
  `palette` sprite_palette_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **palette** `const sprite_palette_ptr& palette() const` (L686): Returns the sprite_palette_ptr to modify.
- **final_intensity** `fixed final_intensity() const` (L695): When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_palette_fade_toggle_action` (class, bn_sprite_palette_actions.h:710)

Changes the intensity of the fade effect applied to a sprite_palette_ptr when the action is updated a given number of times.

`class sprite_palette_fade_toggle_action : public toggle_value_template_action<sprite_palette_ptr, fixed, sprite_palette_fade_manager>`

- **sprite_palette_fade_toggle_action** `sprite_palette_fade_toggle_action(const sprite_palette_ptr& palette, int duration_updates, fixed new_intensity) : toggle_value_template_action(palette, duration_updates, new_intensity)` (L723): Constructor.
  `palette` sprite_palette_ptr to copy.; `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **sprite_palette_fade_toggle_action** `sprite_palette_fade_toggle_action(sprite_palette_ptr&& palette, int duration_updates, fixed new_intensity) : toggle_value_template_action(move(palette), duration_updates, new_intensity)` (L737): Constructor.
  `palette` sprite_palette_ptr to move.; `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **palette** `const sprite_palette_ptr& palette() const` (L746): Returns the sprite_palette_ptr to modify.
- **new_intensity** `fixed new_intensity() const` (L754): Returns the intensity when the action is updated the given number of times.
### `bn::sprite_palette_rotate_manager` (class, bn_sprite_palette_actions.h:770)

Manages the number of colors to rotate to the right in a sprite_palette_ptr.

`class sprite_palette_rotate_manager`

- **get** `static int get(const sprite_palette_ptr& palette)` (L777): Returns the number of colors to rotate to the right in the given sprite_palette_ptr.
- **set** `static void set(int count, sprite_palette_ptr& palette)` (L788): Sets the number of colors to rotate to the right in the given sprite_palette_ptr.
  `count` Number of colors to rotate to the right in the range [1 - rotate_range_size() .. rotate_range_size() - 1].; `palette` sprite_palette_ptr to modify.
### `bn::sprite_palette_rotate_by_action` (class, bn_sprite_palette_actions.h:804)

Modifies the number of colors to rotate to the right in a sprite_palette_ptr by delta_count when the action is updated a given number of times. When the property is over the given maximum, it goes back to the given minimum and vice versa.

`class sprite_palette_rotate_by_action : public cyclic_duration_by_value_template_action<sprite_palette_ptr, int, sprite_palette_rotate_manager>`

- **sprite_palette_rotate_by_action** `sprite_palette_rotate_by_action(const sprite_palette_ptr& palette, int duration_updates, int delta_count) : cyclic_duration_by_value_template_action(palette, duration_updates, delta_count, 0, 0)` (L817): Constructor.
  `palette` sprite_palette_ptr to copy.; `duration_updates` How many times the action has to be updated before updating the number of colors to rotate.; `delta_count` How many colors to add to the number of colors to rotate to the right when the action is updated duration_updates times.
- **sprite_palette_rotate_by_action** `sprite_palette_rotate_by_action(sprite_palette_ptr&& palette, int duration_updates, int delta_count) : cyclic_duration_by_value_template_action(move(palette), duration_updates, delta_count, 0, 0)` (L835): Constructor.
  `palette` sprite_palette_ptr to move.; `duration_updates` How many times the action has to be updated before updating the number of colors to rotate.; `delta_count` How many colors to add to the number of colors to rotate to the right when the action is updated duration_updates times.
- **palette** `const sprite_palette_ptr& palette() const` (L848): Returns the sprite_palette_ptr to modify.
- **delta_count** `int delta_count() const` (L857): Returns how many colors to add to the number of colors to rotate to the right when the action is updated the given number of times.
### `bn::sprite_palette_rotate_to_action` (class, bn_sprite_palette_actions.h:871)

Modifies the number of colors to rotate to the right in a sprite_palette_ptr until it has a given state.

`class sprite_palette_rotate_to_action : public to_value_template_action<sprite_palette_ptr, int, sprite_palette_rotate_manager>`

- **sprite_palette_rotate_to_action** `sprite_palette_rotate_to_action(const sprite_palette_ptr& palette, int duration_updates, int final_count) : to_value_template_action(palette, duration_updates, final_count)` (L886): Constructor.
  `palette` sprite_palette_ptr to copy.; `duration_updates` Number of times that the action must be updated until the number of colors to rotate to the right is equal to final_count.; `final_count` Number of colors to rotate to the right when the action is updated duration_updates times.
  This count must be in the range [1 - rotate_range_size() .. rotate_range_size() - 1].
- **sprite_palette_rotate_to_action** `sprite_palette_rotate_to_action(sprite_palette_ptr&& palette, int duration_updates, int final_count) : to_value_template_action(move(palette), duration_updates, final_count)` (L903): Constructor.
  `palette` sprite_palette_ptr to move.; `duration_updates` Number of times that the action must be updated until the number of colors to rotate to the right is equal to final_count.; `final_count` Number of colors to rotate to the right when the action is updated duration_updates times.
  This count must be in the range [1 - rotate_range_size() .. rotate_range_size() - 1].
- **palette** `const sprite_palette_ptr& palette() const` (L913): Returns the sprite_palette_ptr to modify.
- **final_count** `int final_count() const` (L922): Returns the number of colors to rotate to the right when the action is updated the given number of times.
### `bn::sprite_palette_rotate_loop_action` (class, bn_sprite_palette_actions.h:938)

Modifies the number of colors to rotate to the right in a sprite_palette_ptr from a minimum to a maximum. When the number of colors to rotate to the right is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_palette_rotate_loop_action : public loop_value_template_action<sprite_palette_ptr, int, sprite_palette_rotate_manager>`

- **sprite_palette_rotate_loop_action** `sprite_palette_rotate_loop_action(const sprite_palette_ptr& palette, int duration_updates, int final_count) : loop_value_template_action(palette, duration_updates, final_count)` (L953): Constructor.
  `palette` sprite_palette_ptr to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the number of colors to rotate delta.; `final_count` When the the number of colors to rotate to the right is equal to this parameter, it goes back to its initial state and vice versa.
  This count must be in the range [1 - rotate_range_size() .. rotate_range_size() - 1].
- **sprite_palette_rotate_loop_action** `sprite_palette_rotate_loop_action(sprite_palette_ptr&& palette, int duration_updates, int final_count) : loop_value_template_action(move(palette), duration_updates, final_count)` (L970): Constructor.
  `palette` sprite_palette_ptr to move.; `duration_updates` How many times the action has to be updated before changing the direction of the number of colors to rotate delta.; `final_count` When the the number of colors to rotate to the right is equal to this parameter, it goes back to its initial state and vice versa.
  This count must be in the range [1 - rotate_range_size() .. rotate_range_size() - 1].
- **palette** `const sprite_palette_ptr& palette() const` (L980): Returns the sprite_palette_ptr to modify.
- **final_count** `int final_count() const` (L989): When the number of colors to rotate to the right is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_palette_rotate_toggle_action` (class, bn_sprite_palette_actions.h:1004)

Changes the number of colors to rotate to the right in a sprite_palette_ptr when the action is updated a given number of times.

`class sprite_palette_rotate_toggle_action : public toggle_value_template_action<sprite_palette_ptr, int, sprite_palette_rotate_manager>`

- **sprite_palette_rotate_toggle_action** `sprite_palette_rotate_toggle_action(const sprite_palette_ptr& palette, int duration_updates, int new_count) : toggle_value_template_action(palette, duration_updates, new_count)` (L1019): Constructor.
  `palette` sprite_palette_ptr to copy.; `duration_updates` How many times the action has to be updated to change the number of colors to rotate to the right.; `new_count` New number of colors to rotate to the right when the action is updated duration_updates times.
  This count must be in the range [1 - rotate_range_size() .. rotate_range_size() - 1].
- **sprite_palette_rotate_toggle_action** `sprite_palette_rotate_toggle_action(sprite_palette_ptr&& palette, int duration_updates, int new_count) : toggle_value_template_action(move(palette), duration_updates, new_count)` (L1036): Constructor.
  `palette` sprite_palette_ptr to move.; `duration_updates` How many times the action has to be updated to change the number of colors to rotate to the right.; `new_count` New number of colors to rotate to the right when the action is updated duration_updates times.
  This count must be in the range [1 - rotate_range_size() .. rotate_range_size() - 1].
- **palette** `const sprite_palette_ptr& palette() const` (L1046): Returns the sprite_palette_ptr to modify.
- **new_count** `int new_count() const` (L1055): Returns the number of colors to rotate to the right when the action is updated the given number of times.

## bn_sprite_palettes_actions.h

### `bn::sprite_palettes_brightness_manager` (class, bn_sprite_palettes_actions.h:34)

Manages the brightness of all sprite color palettes.

`class sprite_palettes_brightness_manager`

- **get** `static fixed get()` (L41): Returns the brightness of all sprite color palettes.
- **set** `static void set(fixed brightness)` (L50): Sets the brightness of all sprite color palettes.
  `brightness` New brightness in the range [0..1].
### `bn::sprite_palettes_brightness_to_action` (class, bn_sprite_palettes_actions.h:64)

Modifies the brightness of all sprite color palettes until it has a given state.

`class sprite_palettes_brightness_to_action : public to_template_action<fixed, sprite_palettes_brightness_manager>`

- **sprite_palettes_brightness_to_action** `sprite_palettes_brightness_to_action(int duration_updates, fixed final_brightness) : to_template_action(duration_updates, final_brightness)` (L76): Constructor.
  `duration_updates` Number of times that the action must be updated until the brightness is equal to final_brightness.; `final_brightness` Brightness when the action is updated duration_updates times.
  This brightness must be in the range [0..1].
- **final_brightness** `fixed final_brightness() const` (L85): Returns the brightness when the action is updated the given number of times.
### `bn::sprite_palettes_brightness_loop_action` (class, bn_sprite_palettes_actions.h:100)

Modifies the brightness of all sprite color palettes from a minimum to a maximum. When the brightness is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_palettes_brightness_loop_action : public loop_template_action<fixed, sprite_palettes_brightness_manager>`

- **sprite_palettes_brightness_loop_action** `sprite_palettes_brightness_loop_action(int duration_updates, fixed final_brightness) : loop_template_action(duration_updates, final_brightness)` (L113): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the brightness delta.; `final_brightness` When the brightness is equal to this parameter, it goes back to its initial state and vice versa.
  This brightness must be in the range [0..1].
- **final_brightness** `fixed final_brightness() const` (L122): When the brightness is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_palettes_brightness_toggle_action` (class, bn_sprite_palettes_actions.h:136)

Changes the brightness of all sprite color palettes when the action is updated a given number of times.

`class sprite_palettes_brightness_toggle_action : public toggle_template_action<fixed, sprite_palettes_brightness_manager>`

- **sprite_palettes_brightness_toggle_action** `sprite_palettes_brightness_toggle_action(int duration_updates, fixed new_brightness) : toggle_template_action(duration_updates, new_brightness)` (L148): Constructor.
  `duration_updates` How many times the action has to be updated to change the brightness.; `new_brightness` New brightness when the action is updated duration_updates times.
  This brightness must be in the range [0..1].
- **new_brightness** `fixed new_brightness() const` (L157): Returns the brightness when the action is updated the given number of times.
### `bn::sprite_palettes_contrast_manager` (class, bn_sprite_palettes_actions.h:173)

Manages the contrast of all sprite color palettes.

`class sprite_palettes_contrast_manager`

- **get** `static fixed get()` (L180): Returns the contrast of all sprite color palettes.
- **set** `static void set(fixed contrast)` (L189): Sets the contrast of all sprite color palettes.
  `contrast` New contrast in the range [0..1].
### `bn::sprite_palettes_contrast_to_action` (class, bn_sprite_palettes_actions.h:203)

Modifies the contrast of all sprite color palettes until it has a given state.

`class sprite_palettes_contrast_to_action : public to_template_action<fixed, sprite_palettes_contrast_manager>`

- **sprite_palettes_contrast_to_action** `sprite_palettes_contrast_to_action(int duration_updates, fixed final_contrast) : to_template_action(duration_updates, final_contrast)` (L215): Constructor.
  `duration_updates` Number of times that the action must be updated until the contrast is equal to final_contrast.; `final_contrast` Contrast when the action is updated duration_updates times.
  This contrast must be in the range [0..1].
- **final_contrast** `fixed final_contrast() const` (L224): Returns the contrast when the action is updated the given number of times.
### `bn::sprite_palettes_contrast_loop_action` (class, bn_sprite_palettes_actions.h:239)

Modifies the contrast of all sprite color palettes from a minimum to a maximum. When the contrast is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_palettes_contrast_loop_action : public loop_template_action<fixed, sprite_palettes_contrast_manager>`

- **sprite_palettes_contrast_loop_action** `sprite_palettes_contrast_loop_action(int duration_updates, fixed final_contrast) : loop_template_action(duration_updates, final_contrast)` (L252): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the contrast delta.; `final_contrast` When the contrast is equal to this parameter, it goes back to its initial state and vice versa.
  This contrast must be in the range [0..1].
- **final_contrast** `fixed final_contrast() const` (L261): When the contrast is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_palettes_contrast_toggle_action` (class, bn_sprite_palettes_actions.h:275)

Changes the contrast of all sprite color palettes when the action is updated a given number of times.

`class sprite_palettes_contrast_toggle_action : public toggle_template_action<fixed, sprite_palettes_contrast_manager>`

- **sprite_palettes_contrast_toggle_action** `sprite_palettes_contrast_toggle_action(int duration_updates, fixed new_contrast) : toggle_template_action(duration_updates, new_contrast)` (L286): Constructor.
  `duration_updates` How many times the action has to be updated to change the contrast.; `new_contrast` New contrast when the action is updated duration_updates times.
  This contrast must be in the range [0..1].
- **new_contrast** `fixed new_contrast() const` (L295): Returns the contrast when the action is updated the given number of times.
### `bn::sprite_palettes_intensity_manager` (class, bn_sprite_palettes_actions.h:311)

Manages the intensity of all sprite color palettes.

`class sprite_palettes_intensity_manager`

- **get** `static fixed get()` (L318): Returns the intensity of all sprite color palettes.
- **set** `static void set(fixed intensity)` (L327): Sets the intensity of all sprite color palettes.
  `intensity` New intensity in the range [0..1].
### `bn::sprite_palettes_intensity_to_action` (class, bn_sprite_palettes_actions.h:341)

Modifies the intensity of all sprite color palettes until it has a given state.

`class sprite_palettes_intensity_to_action : public to_template_action<fixed, sprite_palettes_intensity_manager>`

- **sprite_palettes_intensity_to_action** `sprite_palettes_intensity_to_action(int duration_updates, fixed final_intensity) : to_template_action(duration_updates, final_intensity)` (L353): Constructor.
  `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L362): Returns the intensity when the action is updated the given number of times.
### `bn::sprite_palettes_intensity_loop_action` (class, bn_sprite_palettes_actions.h:377)

Modifies the intensity of all sprite color palettes from a minimum to a maximum. When the intensity is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_palettes_intensity_loop_action : public loop_template_action<fixed, sprite_palettes_intensity_manager>`

- **sprite_palettes_intensity_loop_action** `sprite_palettes_intensity_loop_action(int duration_updates, fixed final_intensity) : loop_template_action(duration_updates, final_intensity)` (L390): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L399): When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_palettes_intensity_toggle_action` (class, bn_sprite_palettes_actions.h:413)

Changes the intensity of all sprite color palettes when the action is updated a given number of times.

`class sprite_palettes_intensity_toggle_action : public toggle_template_action<fixed, sprite_palettes_intensity_manager>`

- **sprite_palettes_intensity_toggle_action** `sprite_palettes_intensity_toggle_action(int duration_updates, fixed new_intensity) : toggle_template_action(duration_updates, new_intensity)` (L424): Constructor.
  `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **new_intensity** `fixed new_intensity() const` (L433): Returns the intensity when the action is updated the given number of times.
### `bn::sprite_palettes_inverted_manager` (class, bn_sprite_palettes_actions.h:449)

Manages if the colors of all sprite color palettes must be inverted or not.

`class sprite_palettes_inverted_manager`

- **get** `static bool get()` (L456): Indicates if the colors of all sprite color palettes are inverted or not.
- **set** `static void set(bool inverted)` (L464): Sets if the colors of all sprite color palettes must be inverted or not.
### `bn::sprite_palettes_inverted_toggle_action` (class, bn_sprite_palettes_actions.h:479)

Toggles if the colors of all sprite color palettes must be inverted or not when the action is updated a given number of times.

`class sprite_palettes_inverted_toggle_action : public bool_toggle_template_action<sprite_palettes_inverted_manager>`

- **sprite_palettes_inverted_toggle_action** `explicit sprite_palettes_inverted_toggle_action(int duration_updates) : bool_toggle_template_action(duration_updates)` (L488): Constructor.
  `duration_updates` How many times the action has to be updated to toggle if the colors of all sprite color palettes must be inverted or not.
### `bn::sprite_palettes_grayscale_manager` (class, bn_sprite_palettes_actions.h:504)

Manages the intensity of the grayscale effect applied to all sprite color palettes.

`class sprite_palettes_grayscale_manager`

- **get** `static fixed get()` (L511): Returns the intensity of the grayscale effect applied to all sprite color palettes.
- **set** `static void set(fixed intensity)` (L520): Sets the intensity of the grayscale effect applied to all sprite color palettes.
  `intensity` New intensity in the range [0..1].
### `bn::sprite_palettes_grayscale_to_action` (class, bn_sprite_palettes_actions.h:535)

Modifies the intensity of the grayscale effect applied to all sprite color palettes until it has a given state.

`class sprite_palettes_grayscale_to_action : public to_template_action<fixed, sprite_palettes_grayscale_manager>`

- **sprite_palettes_grayscale_to_action** `sprite_palettes_grayscale_to_action(int duration_updates, fixed final_intensity) : to_template_action(duration_updates, final_intensity)` (L547): Constructor.
  `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L556): Returns the intensity when the action is updated the given number of times.
### `bn::sprite_palettes_grayscale_loop_action` (class, bn_sprite_palettes_actions.h:572)

Modifies the intensity of the grayscale effect applied to all sprite color palettes from a minimum to a maximum. When the intensity is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_palettes_grayscale_loop_action : public loop_template_action<fixed, sprite_palettes_grayscale_manager>`

- **sprite_palettes_grayscale_loop_action** `sprite_palettes_grayscale_loop_action(int duration_updates, fixed final_intensity) : loop_template_action(duration_updates, final_intensity)` (L585): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L594): When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_palettes_grayscale_toggle_action` (class, bn_sprite_palettes_actions.h:609)

Changes the intensity of the grayscale effect applied to all sprite color palettes when the action is updated a given number of times.

`class sprite_palettes_grayscale_toggle_action : public toggle_template_action<fixed, sprite_palettes_grayscale_manager>`

- **sprite_palettes_grayscale_toggle_action** `sprite_palettes_grayscale_toggle_action(int duration_updates, fixed new_intensity) : toggle_template_action(duration_updates, new_intensity)` (L620): Constructor.
  `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **new_intensity** `fixed new_intensity() const` (L629): Returns the intensity when the action is updated the given number of times.
### `bn::sprite_palettes_hue_shift_manager` (class, bn_sprite_palettes_actions.h:645)

Manages the intensity of the hue shift effect applied to all sprite color palettes.

`class sprite_palettes_hue_shift_manager`

- **get** `static fixed get()` (L652): Returns the intensity of the hue shift effect applied to all sprite color palettes.
- **set** `static void set(fixed intensity)` (L661): Sets the intensity of the hue shift effect applied to all sprite color palettes.
  `intensity` New intensity in the range [0..1].
### `bn::sprite_palettes_hue_shift_to_action` (class, bn_sprite_palettes_actions.h:676)

Modifies the intensity of the hue shift effect applied to all sprite color palettes until it has a given state.

`class sprite_palettes_hue_shift_to_action : public to_template_action<fixed, sprite_palettes_hue_shift_manager>`

- **sprite_palettes_hue_shift_to_action** `sprite_palettes_hue_shift_to_action(int duration_updates, fixed final_intensity) : to_template_action(duration_updates, final_intensity)` (L688): Constructor.
  `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L697): Returns the intensity when the action is updated the given number of times.
### `bn::sprite_palettes_hue_shift_loop_action` (class, bn_sprite_palettes_actions.h:713)

Modifies the intensity of the hue shift effect applied to all sprite color palettes from a minimum to a maximum. When the intensity is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_palettes_hue_shift_loop_action : public loop_template_action<fixed, sprite_palettes_hue_shift_manager>`

- **sprite_palettes_hue_shift_loop_action** `sprite_palettes_hue_shift_loop_action(int duration_updates, fixed final_intensity) : loop_template_action(duration_updates, final_intensity)` (L726): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L735): When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_palettes_hue_shift_toggle_action` (class, bn_sprite_palettes_actions.h:750)

Changes the intensity of the hue shift effect applied to all sprite color palettes when the action is updated a given number of times.

`class sprite_palettes_hue_shift_toggle_action : public toggle_template_action<fixed, sprite_palettes_hue_shift_manager>`

- **sprite_palettes_hue_shift_toggle_action** `sprite_palettes_hue_shift_toggle_action(int duration_updates, fixed new_intensity) : toggle_template_action(duration_updates, new_intensity)` (L761): Constructor.
  `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **new_intensity** `fixed new_intensity() const` (L770): Returns the intensity when the action is updated the given number of times.
### `bn::sprite_palettes_fade_manager` (class, bn_sprite_palettes_actions.h:786)

Manages the intensity of the fade effect applied to all sprite color palettes.

`class sprite_palettes_fade_manager`

- **get** `static fixed get()` (L793): Returns the intensity of the fade effect applied to all sprite color palettes.
- **set** `static void set(fixed intensity)` (L802): Sets the intensity of the fade effect applied to all sprite color palettes.
  `intensity` New intensity in the range [0..1].
### `bn::sprite_palettes_fade_to_action` (class, bn_sprite_palettes_actions.h:817)

Modifies the intensity of the fade effect applied to all sprite color palettes until it has a given state.

`class sprite_palettes_fade_to_action : public to_template_action<fixed, sprite_palettes_fade_manager>`

- **sprite_palettes_fade_to_action** `sprite_palettes_fade_to_action(int duration_updates, fixed final_intensity) : to_template_action(duration_updates, final_intensity)` (L829): Constructor.
  `duration_updates` Number of times that the action must be updated until the intensity is equal to final_intensity.; `final_intensity` Intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L838): Returns the intensity when the action is updated the given number of times.
### `bn::sprite_palettes_fade_loop_action` (class, bn_sprite_palettes_actions.h:854)

Modifies the intensity of the fade effect applied to all sprite color palettes from a minimum to a maximum. When the intensity is equal to the given final state, it goes back to its initial state and vice versa.

`class sprite_palettes_fade_loop_action : public loop_template_action<fixed, sprite_palettes_fade_manager>`

- **sprite_palettes_fade_loop_action** `sprite_palettes_fade_loop_action(int duration_updates, fixed final_intensity) : loop_template_action(duration_updates, final_intensity)` (L867): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the intensity delta.; `final_intensity` When the intensity is equal to this parameter, it goes back to its initial state and vice versa.
  This intensity must be in the range [0..1].
- **final_intensity** `fixed final_intensity() const` (L876): When the intensity is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::sprite_palettes_fade_toggle_action` (class, bn_sprite_palettes_actions.h:891)

Changes the intensity of the fade effect applied to all sprite color palettes when the action is updated a given number of times.

`class sprite_palettes_fade_toggle_action : public toggle_template_action<fixed, sprite_palettes_fade_manager>`

- **sprite_palettes_fade_toggle_action** `sprite_palettes_fade_toggle_action(int duration_updates, fixed new_intensity) : toggle_template_action(duration_updates, new_intensity)` (L902): Constructor.
  `duration_updates` How many times the action has to be updated to change the intensity.; `new_intensity` New intensity when the action is updated duration_updates times.
  This intensity must be in the range [0..1].
- **new_intensity** `fixed new_intensity() const` (L911): Returns the intensity when the action is updated the given number of times.

## bn_sprites_actions.h

### `bn::sprites_visible_manager` (class, bn_sprites_actions.h:31)

Manages if the sprite layer must be shown or not.

`class sprites_visible_manager`

- **get** `static bool get()` (L38): Indicates if the sprite layer is visible or not.
- **set** `static void set(bool enabled)` (L46): Sets if the sprite layer is visible or not.
### `bn::sprites_visible_toggle_action` (class, bn_sprites_actions.h:59)

Toggles if the sprite layer must be shown or not when the action is updated a given number of times.

`class sprites_visible_toggle_action : public bool_toggle_template_action<sprites_visible_manager>`

- **sprites_visible_toggle_action** `explicit sprites_visible_toggle_action(int duration_updates) : bool_toggle_template_action(duration_updates)` (L68): Constructor.
  `duration_updates` How many times the action has to be updated to toggle if the sprite layer must be shown or not.
### `bn::sprites_blending_bottom_enabled_manager` (class, bn_sprites_actions.h:83)

Manages if the sprite layer must be part of the blending bottom layer or not.

`class sprites_blending_bottom_enabled_manager`

- **get** `static bool get()` (L90): Indicates if the sprite layer is part of the blending bottom layer or not.
- **set** `static void set(bool enabled)` (L98): Sets if the sprite layer must be part of the blending bottom layer or not.
### `bn::sprites_blending_bottom_enabled_toggle_action` (class, bn_sprites_actions.h:112)

Toggles if the sprite layer is part of the blending bottom layer or not when the action is updated a given number of times.

`class sprites_blending_bottom_enabled_toggle_action : public bool_toggle_template_action<sprites_blending_bottom_enabled_manager>`

- **sprites_blending_bottom_enabled_toggle_action** `explicit sprites_blending_bottom_enabled_toggle_action(int duration_updates) : bool_toggle_template_action(duration_updates)` (L122): Constructor.
  `duration_updates` How many times the action has to be updated to toggle if the sprite layer is part of the blending bottom layer or not.

## bn_sprites_mosaic_actions.h

### `bn::sprites_mosaic_horizontal_stretch_manager` (class, bn_sprites_mosaic_actions.h:34)

Manages the horizontal stretch of the mosaic applied to the sprites.

`class sprites_mosaic_horizontal_stretch_manager`

- **get** `static fixed get()` (L41): Returns the horizontal stretch of the mosaic applied to the sprites.
- **set** `static void set(fixed horizontal_stretch)` (L50): Sets the horizontal stretch of the mosaic applied to the sprites.
  `horizontal_stretch` Horizontal stretch in the range [0..1].
### `bn::sprites_mosaic_horizontal_stretch_to_action` (class, bn_sprites_mosaic_actions.h:64)

Modifies the horizontal stretch of the mosaic applied to the sprites until it has a given state.

`class sprites_mosaic_horizontal_stretch_to_action : public to_template_action<fixed, sprites_mosaic_horizontal_stretch_manager>`

- **sprites_mosaic_horizontal_stretch_to_action** `sprites_mosaic_horizontal_stretch_to_action(int duration_updates, fixed final_horizontal_stretch) : to_template_action(duration_updates, final_horizontal_stretch)` (L77): Constructor.
  `duration_updates` Number of times that the action must be updated until the horizontal stretch is equal to final_horizontal_stretch.; `final_horizontal_stretch` Horizontal stretch when the action is updated duration_updates times.
  This horizontal stretch must be in the range [0..1].
- **final_horizontal_stretch** `fixed final_horizontal_stretch() const` (L87): Returns the horizontal stretch when the action is updated the given number of times.
### `bn::sprites_mosaic_horizontal_stretch_loop_action` (class, bn_sprites_mosaic_actions.h:102)

Modifies the horizontal stretch of the mosaic applied to the sprites from a minimum to a maximum. When the horizontal stretch is equal to the given final state, it goes back to its initial state and vice versa.

`class sprites_mosaic_horizontal_stretch_loop_action : public loop_template_action<fixed, sprites_mosaic_horizontal_stretch_manager>`

- **sprites_mosaic_horizontal_stretch_loop_action** `sprites_mosaic_horizontal_stretch_loop_action(int duration_updates, fixed final_horizontal_stretch) : loop_template_action(duration_updates, final_horizontal_stretch)` (L116): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the horizontal stretch delta.; `final_horizontal_stretch` When the horizontal stretch is equal to this parameter, it goes back to its initial state and vice versa.
  This horizontal stretch must be in the range [0..1].
- **final_horizontal_stretch** `fixed final_horizontal_stretch() const` (L127): When the horizontal stretch is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::sprites_mosaic_horizontal_stretch_toggle_action` (class, bn_sprites_mosaic_actions.h:142)

Changes the horizontal stretch of the mosaic applied to the sprites when the action is updated a given number of times.

`class sprites_mosaic_horizontal_stretch_toggle_action : public toggle_template_action<fixed, sprites_mosaic_horizontal_stretch_manager>`

- **sprites_mosaic_horizontal_stretch_toggle_action** `sprites_mosaic_horizontal_stretch_toggle_action(int duration_updates, fixed new_horizontal_stretch) : toggle_template_action(duration_updates, new_horizontal_stretch)` (L154): Constructor.
  `duration_updates` How many times the action has to be updated to change the horizontal stretch.; `new_horizontal_stretch` New horizontal stretch to set when the action is updated duration_updates times.
  This horizontal stretch must be in the range [0..1].
- **new_horizontal_stretch** `fixed new_horizontal_stretch() const` (L164): Returns the horizontal stretch to set when the action is updated the given number of times.
### `bn::sprites_mosaic_vertical_stretch_manager` (class, bn_sprites_mosaic_actions.h:180)

Manages the vertical stretch of the mosaic applied to the sprites.

`class sprites_mosaic_vertical_stretch_manager`

- **get** `static fixed get()` (L187): Returns the vertical stretch of the mosaic applied to the sprites.
- **set** `static void set(fixed vertical_stretch)` (L196): Sets the vertical stretch of the mosaic applied to the sprites.
  `vertical_stretch` Vertical stretch in the range [0..1].
### `bn::sprites_mosaic_vertical_stretch_to_action` (class, bn_sprites_mosaic_actions.h:210)

Modifies the vertical stretch of the mosaic applied to the sprites until it has a given state.

`class sprites_mosaic_vertical_stretch_to_action : public to_template_action<fixed, sprites_mosaic_vertical_stretch_manager>`

- **sprites_mosaic_vertical_stretch_to_action** `sprites_mosaic_vertical_stretch_to_action(int duration_updates, fixed final_vertical_stretch) : to_template_action(duration_updates, final_vertical_stretch)` (L223): Constructor.
  `duration_updates` Number of times that the action must be updated until the vertical stretch is equal to final_vertical_stretch.; `final_vertical_stretch` Vertical stretch when the action is updated duration_updates times.
  This vertical stretch must be in the range [0..1].
- **final_vertical_stretch** `fixed final_vertical_stretch() const` (L233): Returns the vertical stretch when the action is updated the given number of times.
### `bn::sprites_mosaic_vertical_stretch_loop_action` (class, bn_sprites_mosaic_actions.h:248)

Modifies the vertical stretch of the mosaic applied to the sprites from a minimum to a maximum. When the vertical stretch is equal to the given final state, it goes back to its initial state and vice versa.

`class sprites_mosaic_vertical_stretch_loop_action : public loop_template_action<fixed, sprites_mosaic_vertical_stretch_manager>`

- **sprites_mosaic_vertical_stretch_loop_action** `sprites_mosaic_vertical_stretch_loop_action(int duration_updates, fixed final_vertical_stretch) : loop_template_action(duration_updates, final_vertical_stretch)` (L262): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the vertical stretch delta.; `final_vertical_stretch` When the vertical stretch is equal to this parameter, it goes back to its initial state and vice versa.
  This vertical stretch must be in the range [0..1].
- **final_vertical_stretch** `fixed final_vertical_stretch() const` (L273): When the vertical stretch is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::sprites_mosaic_vertical_stretch_toggle_action` (class, bn_sprites_mosaic_actions.h:288)

Changes the vertical stretch of the mosaic applied to the sprites when the action is updated a given number of times.

`class sprites_mosaic_vertical_stretch_toggle_action : public toggle_template_action<fixed, sprites_mosaic_vertical_stretch_manager>`

- **sprites_mosaic_vertical_stretch_toggle_action** `sprites_mosaic_vertical_stretch_toggle_action(int duration_updates, fixed new_vertical_stretch) : toggle_template_action(duration_updates, new_vertical_stretch)` (L300): Constructor.
  `duration_updates` How many times the action has to be updated to change the vertical stretch.; `new_vertical_stretch` New vertical stretch to set when the action is updated duration_updates times.
  This vertical stretch must be in the range [0..1].
- **new_vertical_stretch** `fixed new_vertical_stretch() const` (L310): Returns the vertical stretch to set when the action is updated the given number of times.
### `bn::sprites_mosaic_stretch_manager` (class, bn_sprites_mosaic_actions.h:326)

Manages the stretch of the mosaic applied to the sprites.

`class sprites_mosaic_stretch_manager`

- **get** `static fixed get()` (L333): Returns the horizontal stretch of the mosaic applied to the sprites.
- **set** `static void set(fixed stretch)` (L342): Sets the stretch of the mosaic applied to the sprites.
  `stretch` Stretch in the range [0..1].
### `bn::sprites_mosaic_stretch_to_action` (class, bn_sprites_mosaic_actions.h:356)

Modifies the stretch of the mosaic applied to the sprites until it has a given state.

`class sprites_mosaic_stretch_to_action : public to_template_action<fixed, sprites_mosaic_stretch_manager>`

- **sprites_mosaic_stretch_to_action** `sprites_mosaic_stretch_to_action(int duration_updates, fixed final_stretch) : to_template_action(duration_updates, final_stretch)` (L368): Constructor.
  `duration_updates` Number of times that the action must be updated until the stretch is equal to final_stretch.; `final_stretch` Stretch when the action is updated duration_updates times.
  This stretch must be in the range [0..1].
- **final_stretch** `fixed final_stretch() const` (L377): Returns the stretch when the action is updated the given number of times.
### `bn::sprites_mosaic_stretch_loop_action` (class, bn_sprites_mosaic_actions.h:392)

Modifies the stretch of the mosaic applied to the sprites from a minimum to a maximum. When the stretch is equal to the given final state, it goes back to its initial state and vice versa.

`class sprites_mosaic_stretch_loop_action : public loop_template_action<fixed, sprites_mosaic_stretch_manager>`

- **sprites_mosaic_stretch_loop_action** `sprites_mosaic_stretch_loop_action(int duration_updates, fixed final_stretch) : loop_template_action(duration_updates, final_stretch)` (L405): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the stretch delta.; `final_stretch` When the stretch is equal to this parameter, it goes back to its initial state and vice versa.
  This stretch must be in the range [0..1].
- **final_stretch** `fixed final_stretch() const` (L414): When the stretch is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::sprites_mosaic_stretch_toggle_action` (class, bn_sprites_mosaic_actions.h:429)

Changes the stretch of the mosaic applied to the sprites when the action is updated a given number of times.

`class sprites_mosaic_stretch_toggle_action : public toggle_template_action<fixed, sprites_mosaic_stretch_manager>`

- **sprites_mosaic_stretch_toggle_action** `sprites_mosaic_stretch_toggle_action(int duration_updates, fixed new_stretch) : toggle_template_action(duration_updates, new_stretch)` (L440): Constructor.
  `duration_updates` How many times the action has to be updated to change the stretch.; `new_stretch` New stretch to set when the action is updated duration_updates times.
  This stretch must be in the range [0..1].
- **new_stretch** `fixed new_stretch() const` (L449): Returns the stretch to set when the action is updated the given number of times.
