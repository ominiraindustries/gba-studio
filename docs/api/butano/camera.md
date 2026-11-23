# Butano 21.10.1 — Cameras (`camera`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [display](display.md)

## bn_camera_ptr.h

### `bn::camera_ptr` (class, bn_camera_ptr.h:31)

std::shared_ptr like smart pointer that retains shared ownership of a camera.

`class camera_ptr`

Several camera_ptr objects may own the same camera.

The camera is released when the last remaining camera_ptr owning it is destroyed.

- **create** `static camera_ptr create()` (L39): Creates a camera_ptr.
  devuelve: The requested camera_ptr.
- **create** `static camera_ptr create(fixed x, fixed y)` (L47): Creates a camera_ptr.
  `x` Horizontal position of the camera.; `y` Vertical position of the camera.; devuelve: The requested camera_ptr.
- **create** `static camera_ptr create(const fixed_point& position)` (L54): Creates a camera_ptr.
  `position` Position of the camera.; devuelve: The requested camera_ptr.
- **create_optional** `static optional<camera_ptr> create_optional()` (L60): Creates a camera_ptr.
  devuelve: The requested camera_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<camera_ptr> create_optional(fixed x, fixed y)` (L68): Creates a camera_ptr.
  `x` Horizontal position of the camera.; `y` Vertical position of the camera.; devuelve: The requested camera_ptr if it could be allocated; bn::nullopt otherwise.
- **create_optional** `static optional<camera_ptr> create_optional(const fixed_point& position)` (L75): Creates a camera_ptr.
  `position` Position of the camera.; devuelve: The requested camera_ptr if it could be allocated; bn::nullopt otherwise.
- **camera_ptr** `camera_ptr(const camera_ptr& other)` (L81): Copy constructor.
  `other` camera_ptr to copy.
- **operator=** `camera_ptr& operator=(const camera_ptr& other)` (L88): Copy assignment operator.
  `other` camera_ptr to copy.; devuelve: Reference to this.
- **camera_ptr** `camera_ptr(camera_ptr&& other) noexcept : camera_ptr(other._id)` (L94): Move constructor.
  `other` camera_ptr to move.
- **operator=** `camera_ptr& operator=(camera_ptr&& other) noexcept` (L105): Move assignment operator.
  `other` camera_ptr to move.; devuelve: Reference to this.
- **~camera_ptr** `~camera_ptr()` (L114): Releases the referenced camera if no more camera_ptr objects reference to it.
- **id** `int id() const` (L119): Returns the internal id.
- **x** `fixed x() const` (L127): Returns the horizontal position of the camera.
- **set_x** `void set_x(fixed x)` (L132): Sets the horizontal position of the camera.
- **y** `fixed y() const` (L137): Returns the vertical position of the camera.
- **set_y** `void set_y(fixed y)` (L142): Sets the vertical position of the camera.
- **position** `const fixed_point& position() const` (L147): Returns the position of the camera.
- **set_position** `void set_position(fixed x, fixed y)` (L154): Sets the position of the camera.
  `x` Horizontal position of the camera.; `y` Vertical position of the camera.
- **set_position** `void set_position(const fixed_point& position)` (L159): Sets the position of the camera.
- **swap** `void swap(camera_ptr& other)` (L165): Exchanges the contents of this camera_ptr with those of the other one.
  `other` camera_ptr to exchange the contents with.
- **swap** `friend void swap(camera_ptr& a, camera_ptr& b)` (L175): Exchanges the contents of a camera_ptr with those of another one.
  `a` First camera_ptr to exchange the contents with.; `b` Second camera_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const camera_ptr& a, const camera_ptr& b) = default` (L183): Default equal operator.

## bn_cameras.h

### `bn::cameras` (namespace, bn_cameras.h:23)

Cameras related functions.

- **used_items_count** `int used_items_count()` (L28): Returns the number of used cameras managed with camera_ptr objects.
- **available_items_count** `int available_items_count()` (L33): Returns the number of available cameras that can be managed with camera_ptr objects.

## bn_config_cameras.h

### `BN_CFG_CAMERA_MAX_ITEMS` (macro, bn_config_cameras.h:26)

Specifies the maximum number of cameras that can be created with bn::camera_ptr static constructors.

`#define BN_CFG_CAMERA_MAX_ITEMS 4`

