# Butano 21.10.1 — Action templates (`template_action`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [action](action.md)

## bn_template_actions.h

### `bn::by_template_action` (class, bn_template_actions.h:30)

Modifies a property by a given delta.

`template<typename Property, class PropertyManager> class by_template_action`

- **reset** `void reset()` (L38): Resets the property to its initial state.
- **update** `void update()` (L46): Adds delta_property to the property.
- **by_template_action** `explicit by_template_action(const Property& delta_property)` (L56): Constructor.
  `delta_property` How much to add to the property when the action is updated.
- **delta_property** `const Property& delta_property() const` (L65): Returns how much to add to the property when the action is updated.
### `bn::cyclic_by_template_action` (class, bn_template_actions.h:85)

Modifies the property by delta from a minimum to a maximum. When the property is over the given maximum, it goes back to the given minimum and vice versa.

`template<typename Property, class PropertyManager> class cyclic_by_template_action`

- **reset** `void reset()` (L93): Resets the property to its initial state.
- **update** `void update()` (L103): Adds delta_property to the property.
  When the property reaches the maximum, it goes back to the minimum and vice versa.
- **cyclic_by_template_action** `cyclic_by_template_action(const Property& delta_property, const Property& min_property, const Property& after_max_property)` (L126): Constructor.
  `delta_property` How much to add to the property when the action is updated.; `min_property` Minimum property.; `after_max_property` Immediate amount after the maximum property.
- **delta_property** `const Property& delta_property() const` (L138): Returns how much to add to the property when the action is updated.
- **set_min_property** `void set_min_property(const Property& min_property)` (L146): Sets the minimum property.
- **set_after_max_property** `void set_after_max_property(const Property& after_max_property)` (L154): Sets the immediate amount after the maximum property.
### `bn::duration_by_template_action` (class, bn_template_actions.h:175)

Modifies the property by delta when the action is updated a given number of times.

`template<typename Property, class PropertyManager> class duration_by_template_action`

- **reset** `void reset()` (L183): Resets the property to its initial state.
- **update** `void update()` (L192): Adds delta_property to the property when the given amount of update calls are done.
- **duration_updates** `int duration_updates() const` (L208): Returns how many times the action has to be updated before updating the property.
- **duration_by_template_action** `duration_by_template_action(int duration_updates, const Property& delta_property)` (L219): Constructor.
  `duration_updates` How many times the action has to be updated before updating the property.; `delta_property` How much to add to the property when the action is updated duration_updates times.
- **delta_property** `const Property& delta_property() const` (L230): Returns how much to add to the property when the action is updated the given number of times.
### `bn::cyclic_duration_by_template_action` (class, bn_template_actions.h:252)

Modifies the property by delta when the action is updated a given number of times. When the property is over the given maximum, it goes back to the given minimum and vice versa.

`template<typename Property, class PropertyManager> class cyclic_duration_by_template_action`

- **reset** `void reset()` (L260): Resets the property to its initial state.
- **update** `void update()` (L270): Adds delta_property to the property when the given amount of update calls are done.
  When the property reaches the maximum, it goes back to the minimum and vice versa.
- **duration_updates** `int duration_updates() const` (L297): Returns how many times the action has to be updated before updating the property.
- **cyclic_duration_by_template_action** `cyclic_duration_by_template_action(int duration_updates, const Property& delta_property, const Property& min_property, const Property& after_max_property)` (L310): Constructor.
  `duration_updates` How many times the action has to be updated before updating the property.; `delta_property` How much to add to the property when the action is updated duration_updates times.; `min_property` Minimum property.; `after_max_property` Immediate amount after the maximum property.
- **delta_property** `const Property& delta_property() const` (L324): Returns how much to add to the property when the action is updated the given number of times.
- **set_min_property** `void set_min_property(const Property& min_property)` (L332): Sets the minimum property.
- **set_after_max_property** `void set_after_max_property(const Property& after_max_property)` (L340): Sets the immediate amount after the maximum property.
### `bn::to_template_action` (class, bn_template_actions.h:363)

Modifies the property until it has a given state.

`template<typename Property, class PropertyManager> class to_template_action`

- **reset** `void reset()` (L371): Resets the property to its initial state.
- **update** `void update()` (L381): Adds ((final_property - initial_property) / duration_updates) to the property until it is equal to final_property.
- **done** `bool done() const` (L400): Indicates if the action must not be updated anymore.
- **duration_updates** `int duration_updates() const` (L409): Returns the number of times that the action must be updated until the property is equal to final_property.
- **to_template_action** `to_template_action(int duration_updates, const Property& final_property)` (L421): Constructor.
  `duration_updates` Number of times that the action must be updated until the property is equal to final_property.; `final_property` Property when the action is updated duration_updates times.
- **final_property** `const Property& final_property() const` (L432): Returns the state of property when the action is updated the given number of times.
### `bn::loop_template_action` (class, bn_template_actions.h:464)

Modifies the property by delta from a minimum to a maximum. When the property is equal to the given final state, it goes back to its initial state and vice versa.

`template<typename Property, class PropertyManager> class loop_template_action`

- **reset** `void reset()` (L472): Resets the property to its initial state.
- **update** `void update()` (L484): Adds or subtracts ((final_property - initial_property) / duration_updates) to the property.
  When the property is equal to final_property, it goes back to its initial state and vice versa.
- **duration_updates** `int duration_updates() const` (L519): Returns how many times the action has to be updated before changing the direction of the property delta.
- **loop_template_action** `loop_template_action(int duration_updates, const Property& final_property)` (L532): Constructor.
  `duration_updates` How many times the action has to be updated before changing the direction of the property delta.; `final_property` When the property is equal to this parameter, it goes back to its initial state and vice versa.
- **final_property** `const Property& final_property() const` (L544): When the property is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::toggle_template_action` (class, bn_template_actions.h:576)

Changes the property when the action is updated a given number of times.

`template<typename Property, class PropertyManager> class toggle_template_action`

- **reset** `void reset()` (L584): Resets the property to its initial state.
- **update** `void update()` (L594): Sets the property to new_property or to its initial state when the given amount of update calls are done.
- **duration_updates** `int duration_updates() const` (L618): Returns how many times the action has to be updated to change the property.
- **toggle_template_action** `toggle_template_action(int duration_updates, const Property& new_property)` (L629): Constructor.
  `duration_updates` How many times the action has to be updated to change the property.; `new_property` New state for the property to set when the action is updated duration_updates times.
- **new_property** `const Property& new_property() const` (L640): Returns the new state for the property when the action is updated the given number of times.
### `bn::bool_toggle_template_action` (class, bn_template_actions.h:661)

Toggles a boolean property when the action is updated a given number of times.

`template<class PropertyManager> class bool_toggle_template_action`

- **reset** `void reset()` (L669): Resets the property to its initial state.
- **update** `void update()` (L679): Toggles the property when the given amount of update calls are done.
- **duration_updates** `int duration_updates() const` (L703): Returns how many times the action has to be updated to toggle the property.
- **bool_toggle_template_action** `explicit bool_toggle_template_action(int duration_updates)` (L713): Constructor.
  `duration_updates` How many times the action has to be updated to toggle the property.

## bn_value_template_actions.h

### `bn::by_value_template_action` (class, bn_value_template_actions.h:32)

Modifies the property of a value by a given delta.

`template<typename Value, typename Property, class PropertyManager> class by_value_template_action`

- **reset** `void reset()` (L40): Resets the property to its initial state.
- **update** `void update()` (L48): Adds delta_property to the property.
- **by_value_template_action** `by_value_template_action(const Value& value, const Property& delta_property)` (L59): Constructor.
  `value` Value to copy.; `delta_property` How much to add to the property when the action is updated.
- **by_value_template_action** `by_value_template_action(Value&& value, const Property& delta_property)` (L71): Constructor.
  `value` Value to move.; `delta_property` How much to add to the property when the action is updated.
- **value** `const Value& value() const` (L81): Returns the value to modify.
- **delta_property** `const Property& delta_property() const` (L89): Returns how much to add to the property when the action is updated.
### `bn::cyclic_by_value_template_action` (class, bn_value_template_actions.h:111)

Modifies the property of a value by delta from a minimum to a maximum. When the property is over the given maximum, it goes back to the given minimum and vice versa.

`template<typename Value, typename Property, class PropertyManager> class cyclic_by_value_template_action`

- **reset** `void reset()` (L119): Resets the property to its initial state.
- **update** `void update()` (L129): Adds delta_property to the property.
  When the property reaches the maximum, it goes back to the minimum and vice versa.
- **cyclic_by_value_template_action** `cyclic_by_value_template_action(const Value& value, const Property& delta_property, const Property& min_property, const Property& after_max_property)` (L153): Constructor.
  `value` Value to copy.; `delta_property` How much to add to the property when the action is updated.; `min_property` Minimum property.; `after_max_property` Immediate amount after the maximum property.
- **cyclic_by_value_template_action** `cyclic_by_value_template_action(Value&& value, const Property& delta_property, const Property& min_property, const Property& after_max_property)` (L170): Constructor.
  `value` Value to move.; `delta_property` How much to add to the property when the action is updated.; `min_property` Minimum property.; `after_max_property` Immediate amount after the maximum property.
- **value** `const Value& value() const` (L183): Returns the value to modify.
- **delta_property** `const Property& delta_property() const` (L191): Returns how much to add to the property when the action is updated.
- **set_min_property** `void set_min_property(const Property& min_property)` (L199): Sets the minimum property.
- **set_after_max_property** `void set_after_max_property(const Property& after_max_property)` (L207): Sets the immediate amount after the maximum property.
### `bn::duration_by_value_template_action` (class, bn_value_template_actions.h:230)

Modifies the property of a value by delta when the action is updated a given number of times.

`template<typename Value, typename Property, class PropertyManager> class duration_by_value_template_action`

- **reset** `void reset()` (L238): Resets the property to its initial state.
- **update** `void update()` (L247): Adds delta_property to the property when the given amount of update calls are done.
- **duration_updates** `int duration_updates() const` (L263): Returns how many times the action has to be updated before updating the property.
- **duration_by_value_template_action** `duration_by_value_template_action(const Value& value, int duration_updates, const Property& delta_property)` (L275): Constructor.
  `value` Value to copy.; `duration_updates` How many times the action has to be updated before updating the property.; `delta_property` How much to add to the property when the action is updated duration_updates times.
- **duration_by_value_template_action** `duration_by_value_template_action(Value&& value, int duration_updates, const Property& delta_property)` (L290): Constructor.
  `value` Value to move.; `duration_updates` How many times the action has to be updated before updating the property.; `delta_property` How much to add to the property when the action is updated duration_updates times.
- **value** `const Value& value() const` (L302): Returns the value to modify.
- **delta_property** `const Property& delta_property() const` (L310): Returns how much to add to the property when the action is updated the given number of times.
### `bn::cyclic_duration_by_value_template_action` (class, bn_value_template_actions.h:334)

Modifies the property of a value by delta when the action is updated a given number of times. When the property is over the given maximum, it goes back to the given minimum and vice versa.

`template<typename Value, typename Property, class PropertyManager> class cyclic_duration_by_value_template_action`

- **reset** `void reset()` (L342): Resets the property to its initial state.
- **update** `void update()` (L352): Adds delta_property to the property when the given amount of update calls are done.
  When the property reaches the maximum, it goes back to the minimum and vice versa.
- **duration_updates** `int duration_updates() const` (L379): Returns how many times the action has to be updated before updating the property.
- **cyclic_duration_by_value_template_action** `cyclic_duration_by_value_template_action(const Value& value, int duration_updates, const Property& delta_property, const Property& min_property, const Property& after_max_property)` (L393): Constructor.
  `value` Value to copy.; `duration_updates` How many times the action has to be updated before updating the property.; `delta_property` How much to add to the property when the action is updated duration_updates times.; `min_property` Minimum property.; `after_max_property` Immediate amount after the maximum property.
- **cyclic_duration_by_value_template_action** `cyclic_duration_by_value_template_action(Value&& value, int duration_updates, const Property& delta_property, const Property& min_property, const Property& after_max_property)` (L413): Constructor.
  `value` Value to move.; `duration_updates` How many times the action has to be updated before updating the property.; `delta_property` How much to add to the property when the action is updated duration_updates times.; `min_property` Minimum property.; `after_max_property` Immediate amount after the maximum property.
- **value** `const Value& value() const` (L428): Returns the value to modify.
- **delta_property** `const Property& delta_property() const` (L436): Returns how much to add to the property when the action is updated the given number of times.
- **set_min_property** `void set_min_property(const Property& min_property)` (L444): Sets the minimum property.
- **set_after_max_property** `void set_after_max_property(const Property& after_max_property)` (L452): Sets the immediate amount after the maximum property.
### `bn::to_value_template_action` (class, bn_value_template_actions.h:477)

Modifies the property of a value until it has a given state.

`template<typename Value, typename Property, class PropertyManager> class to_value_template_action`

- **reset** `void reset()` (L485): Resets the property to its initial state.
- **update** `void update()` (L495): Adds ((final_property - initial_property) / duration_updates) to the property until it is equal to final_property.
- **done** `bool done() const` (L514): Indicates if the action must not be updated anymore.
- **duration_updates** `int duration_updates() const` (L523): Returns the number of times that the action must be updated until the property is equal to final_property.
- **to_value_template_action** `to_value_template_action(const Value& value, int duration_updates, const Property& final_property)` (L536): Constructor.
  `value` Value to copy.; `duration_updates` Number of times that the action must be updated until the property is equal to final_property.; `final_property` Property when the action is updated duration_updates times.
- **to_value_template_action** `to_value_template_action(Value&& value, int duration_updates, const Property& final_property)` (L552): Constructor.
  `value` Value to move.; `duration_updates` Number of times that the action must be updated until the property is equal to final_property.; `final_property` Property when the action is updated duration_updates times.
- **value** `const Value& value() const` (L564): Returns the value to modify.
- **final_property** `const Property& final_property() const` (L572): Returns the state of property when the action is updated the given number of times.
### `bn::loop_value_template_action` (class, bn_value_template_actions.h:606)

Modifies the property of a value by delta from a minimum to a maximum. When the property is equal to the given final state, it goes back to its initial state and vice versa.

`template<typename Value, typename Property, class PropertyManager> class loop_value_template_action`

- **reset** `void reset()` (L614): Resets the property to its initial state.
- **update** `void update()` (L626): Adds or subtracts ((final_property - initial_property) / duration_updates) to the property.
  When the property is equal to final_property, it goes back to its initial state and vice versa.
- **duration_updates** `int duration_updates() const` (L661): Returns how many times the action has to be updated before changing the direction of the property delta.
- **loop_value_template_action** `loop_value_template_action(const Value& value, int duration_updates, const Property& final_property)` (L675): Constructor.
  `value` Value to copy.; `duration_updates` How many times the action has to be updated before changing the direction of the property delta.; `final_property` When the property is equal to this parameter, it goes back to its initial state and vice versa.
- **loop_value_template_action** `loop_value_template_action(Value&& value, int duration_updates, const Property& final_property)` (L692): Constructor.
  `value` Value to move.; `duration_updates` How many times the action has to be updated before changing the direction of the property delta.; `final_property` When the property is equal to this parameter, it goes back to its initial state and vice versa.
- **value** `const Value& value() const` (L704): Returns the value to modify.
- **final_property** `const Property& final_property() const` (L713): When the property is equal to the returned parameter, it goes back to its initial state and vice versa.
### `bn::toggle_value_template_action` (class, bn_value_template_actions.h:747)

Changes the property of a value when the action is updated a given number of times.

`template<typename Value, typename Property, class PropertyManager> class toggle_value_template_action`

- **reset** `void reset()` (L755): Resets the property to its initial state.
- **update** `void update()` (L765): Sets the property to new_property or to its initial state when the given amount of update calls are done.
- **duration_updates** `int duration_updates() const` (L789): Returns how many times the action has to be updated to change the property.
- **toggle_value_template_action** `toggle_value_template_action(const Value& value, int duration_updates, const Property& new_property)` (L801): Constructor.
  `value` Value to copy.; `duration_updates` How many times the action has to be updated to change the property.; `new_property` New state for the property when the action is updated duration_updates times.
- **toggle_value_template_action** `toggle_value_template_action(Value&& value, int duration_updates, const Property& new_property)` (L816): Constructor.
  `value` Value to move.; `duration_updates` How many times the action has to be updated to change the property.; `new_property` New state for the property when the action is updated duration_updates times.
- **value** `const Value& value() const` (L828): Returns the value to modify.
- **new_property** `const Property& new_property() const` (L836): Returns the new state for the property when the action is updated the given number of times.
### `bn::bool_toggle_value_template_action` (class, bn_value_template_actions.h:859)

Toggles a boolean property of a value when the action is updated a given number of times.

`template<typename Value, class PropertyManager> class bool_toggle_value_template_action`

- **reset** `void reset()` (L867): Resets the property to its initial state.
- **update** `void update()` (L877): Toggles the property when the given amount of update calls are done.
- **duration_updates** `int duration_updates() const` (L901): Returns how many times the action has to be updated to toggle the property.
- **bool_toggle_value_template_action** `bool_toggle_value_template_action(const Value& value, int duration_updates)` (L912): Constructor.
  `value` Value to copy.; `duration_updates` How many times the action has to be updated to toggle the property.
- **bool_toggle_value_template_action** `bool_toggle_value_template_action(Value&& value, int duration_updates)` (L925): Constructor.
  `value` Value to move.; `duration_updates` How many times the action has to be updated to toggle the property.
- **value** `const Value& value() const` (L936): Returns the value to modify.
