# Butano 21.10.1 — H-Blank effects (`hblank_effect`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [display](display.md)

## bn_affine_bg_attributes_hbe_ptr.h

### `bn::affine_bg_attributes_hbe_ptr` (class, bn_affine_bg_attributes_hbe_ptr.h:31)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes to commit to the GBA register of an affine background in each screen horizontal line.

`class affine_bg_attributes_hbe_ptr : public hbe_ptr`

- **create** `static affine_bg_attributes_hbe_ptr create( affine_bg_ptr bg, const span<const affine_bg_attributes>& attributes_ref)` (L48): Creates an affine_bg_attributes_hbe_ptr which changes the attributes to commit to the GBA register of an affine background in each screen horizontal line.
  `bg` Affine background to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_attributes objects with the attributes to commit to the GBA register of the given affine background in each screen horizontal line.; devuelve: The requested affine_bg_attributes_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_attributes_hbe_ptr> create_optional( affine_bg_ptr bg, const span<const affine_bg_attributes>& attributes_ref)` (L64): Creates an affine_bg_attributes_hbe_ptr which changes the attributes to commit to the GBA register of an affine background in each screen horizontal line.
  `bg` Affine background to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_attributes objects with the attributes to commit to the GBA register of the given affine background in each screen horizontal line.; devuelve: The requested affine_bg_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_attributes_hbe_ptr to avoid dangling references.
- **bg** `const affine_bg_ptr& bg() const` (L70): Returns the affine background modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_attributes> attributes_ref() const` (L83): Returns the referenced array of 160 affine_bg_attributes objects with the attributes to commit to the GBA register of the managed affine background in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_attributes>& attributes_ref)` (L93): Sets the reference to an array of 160 affine_bg_attributes objects with the attributes to commit to the GBA register of the managed affine background in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L102): Rereads the content of the referenced attributes to the GBA register of the managed affine background in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(affine_bg_attributes_hbe_ptr& other)` (L108): Exchanges the contents of this affine_bg_attributes_hbe_ptr with those of the other one.
  `other` affine_bg_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(affine_bg_attributes_hbe_ptr& a, affine_bg_attributes_hbe_ptr& b)` (L115): Exchanges the contents of an affine_bg_attributes_hbe_ptr with those of another one.
  `a` First affine_bg_attributes_hbe_ptr to exchange the contents with.; `b` Second affine_bg_attributes_hbe_ptr to exchange the contents with.

## bn_affine_bg_dx_register_hbe_ptr.h

### `bn::affine_bg_dx_register_hbe_ptr` (class, bn_affine_bg_dx_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the the fifth GBA register of an affine_bg_ptr in each screen horizontal line.

`class affine_bg_dx_register_hbe_ptr : public hbe_ptr`

- **create** `static affine_bg_dx_register_hbe_ptr create( affine_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates an affine_bg_dx_register_hbe_ptr which changes the attributes to commit to the the fifth GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_dx_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_dx_register_hbe_ptr to avoid dangling references.
- **create** `static affine_bg_dx_register_hbe_ptr create( affine_bg_ptr bg, const span<const int>& values_ref)` (L65): Creates an affine_bg_dx_register_hbe_ptr which changes the values to commit to the the fifth GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the fifth GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_dx_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the affine_bg_dx_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_dx_register_hbe_ptr> create_optional( affine_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates an affine_bg_dx_register_hbe_ptr which changes the attributes to commit to the the fifth GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_dx_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_dx_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_dx_register_hbe_ptr> create_optional( affine_bg_ptr bg, const span<const int>& values_ref)` (L96): Creates an affine_bg_dx_register_hbe_ptr which changes the values to commit to the the fifth GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the fifth GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_dx_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the affine_bg_dx_register_hbe_ptr to avoid dangling references.
- **bg** `const affine_bg_ptr& bg() const` (L102): Returns the affine_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_dx_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the the fifth GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_dx_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_dx_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the the fifth GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_dx_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the the fifth GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_dx_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the the fifth GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_dx_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(affine_bg_dx_register_hbe_ptr& other)` (L165): Exchanges the contents of this affine_bg_dx_register_hbe_ptr with those of the other one.
  `other` affine_bg_dx_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(affine_bg_dx_register_hbe_ptr& a, affine_bg_dx_register_hbe_ptr& b)` (L172): Exchanges the contents of an affine_bg_dx_register_hbe_ptr with those of another one.
  `a` First affine_bg_dx_register_hbe_ptr to exchange the contents with.; `b` Second affine_bg_dx_register_hbe_ptr to exchange the contents with.

## bn_affine_bg_dy_register_hbe_ptr.h

### `bn::affine_bg_dy_register_hbe_ptr` (class, bn_affine_bg_dy_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the the sixth GBA register of an affine_bg_ptr in each screen horizontal line.

`class affine_bg_dy_register_hbe_ptr : public hbe_ptr`

- **create** `static affine_bg_dy_register_hbe_ptr create( affine_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates an affine_bg_dy_register_hbe_ptr which changes the attributes to commit to the the sixth GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_dy_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_dy_register_hbe_ptr to avoid dangling references.
- **create** `static affine_bg_dy_register_hbe_ptr create( affine_bg_ptr bg, const span<const int>& values_ref)` (L65): Creates an affine_bg_dy_register_hbe_ptr which changes the values to commit to the the sixth GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the sixth GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_dy_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the affine_bg_dy_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_dy_register_hbe_ptr> create_optional( affine_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates an affine_bg_dy_register_hbe_ptr which changes the attributes to commit to the the sixth GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_dy_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_dy_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_dy_register_hbe_ptr> create_optional( affine_bg_ptr bg, const span<const int>& values_ref)` (L96): Creates an affine_bg_dy_register_hbe_ptr which changes the values to commit to the the sixth GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the sixth GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_dy_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the affine_bg_dy_register_hbe_ptr to avoid dangling references.
- **bg** `const affine_bg_ptr& bg() const` (L102): Returns the affine_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_dy_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the the sixth GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_dy_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_dy_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the the sixth GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_dy_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the the sixth GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_dy_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the the sixth GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_dy_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(affine_bg_dy_register_hbe_ptr& other)` (L165): Exchanges the contents of this affine_bg_dy_register_hbe_ptr with those of the other one.
  `other` affine_bg_dy_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(affine_bg_dy_register_hbe_ptr& a, affine_bg_dy_register_hbe_ptr& b)` (L172): Exchanges the contents of an affine_bg_dy_register_hbe_ptr with those of another one.
  `a` First affine_bg_dy_register_hbe_ptr to exchange the contents with.; `b` Second affine_bg_dy_register_hbe_ptr to exchange the contents with.

## bn_affine_bg_mat_attributes_hbe_ptr.h

### `bn::affine_bg_mat_attributes_hbe_ptr` (class, bn_affine_bg_mat_attributes_hbe_ptr.h:36)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes which define the position and the transformation matrix of an affine_bg_ptr in each screen horizontal line.

`class affine_bg_mat_attributes_hbe_ptr`

- **create** `static affine_bg_mat_attributes_hbe_ptr create( const affine_bg_ptr& bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L53): Creates an affine_bg_mat_attributes_hbe_ptr which changes the attributes which define the position and the transformation matrix of an affine_bg_ptr in each screen horizontal line.
  `bg` affine_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to set to the given affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_mat_attributes_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_mat_attributes_hbe_ptr> create_optional( const affine_bg_ptr& bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L69): Creates an affine_bg_mat_attributes_hbe_ptr which changes the attributes which define the position and the transformation matrix of an affine_bg_ptr in each screen horizontal line.
  `bg` affine_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to set to the given affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_mat_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **id** `int id() const` (L75): Returns the internal id.
- **visible** `bool visible() const` (L83): Indicates if this H-Blank effect must be committed to the GBA or not.
- **set_visible** `void set_visible(bool visible)` (L91): Sets if this H-Blank effect must be committed to the GBA or not.
- **bg** `const affine_bg_ptr& bg() const` (L96): Returns the affine_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L108): Returns the referenced array of 160 affine_bg_mat_attributes objects to set to the given affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L117): Sets the reference to an array of 160 affine_bg_mat_attributes objects to set to the given affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L126): Rereads the content of the referenced attributes to set to the given affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(affine_bg_mat_attributes_hbe_ptr& other)` (L132): Exchanges the contents of this affine_bg_mat_attributes_hbe_ptr with those of the other one.
  `other` affine_bg_mat_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(affine_bg_mat_attributes_hbe_ptr& a, affine_bg_mat_attributes_hbe_ptr& b)` (L139): Exchanges the contents of an affine_bg_mat_attributes_hbe_ptr with those of another one.
  `a` First affine_bg_mat_attributes_hbe_ptr to exchange the contents with.; `b` Second affine_bg_mat_attributes_hbe_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const affine_bg_mat_attributes_hbe_ptr& a, const affine_bg_mat_attributes_hbe_ptr& b)` (L150): Equal operator.
  `a` First affine_bg_mat_attributes_hbe_ptr to compare.; `b` Second affine_bg_mat_attributes_hbe_ptr to compare.; devuelve: `true` if the first affine_bg_mat_attributes_hbe_ptr is equal to the second one, otherwise `false`.
- **operator!=** `friend bool operator!=(const affine_bg_mat_attributes_hbe_ptr& a, const affine_bg_mat_attributes_hbe_ptr& b)` (L162): Not equal operator.
  `a` First affine_bg_mat_attributes_hbe_ptr to compare.; `b` Second affine_bg_mat_attributes_hbe_ptr to compare.; devuelve: `true` if the first affine_bg_mat_attributes_hbe_ptr is not equal to the second one, otherwise `false`.

## bn_affine_bg_pa_register_hbe_ptr.h

### `bn::affine_bg_pa_register_hbe_ptr` (class, bn_affine_bg_pa_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the first GBA register of an affine_bg_ptr in each screen horizontal line.

`class affine_bg_pa_register_hbe_ptr : public hbe_ptr`

- **create** `static affine_bg_pa_register_hbe_ptr create( affine_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L49): Creates an affine_bg_pa_register_hbe_ptr which changes the attributes to commit to the first GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pa_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pa_register_hbe_ptr to avoid dangling references.
- **create** `static affine_bg_pa_register_hbe_ptr create( affine_bg_ptr bg, const span<const int16_t>& values_ref)` (L64): Creates an affine_bg_pa_register_hbe_ptr which changes the values to commit to the first GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the first GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pa_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pa_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_pa_register_hbe_ptr> create_optional( affine_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L80): Creates an affine_bg_pa_register_hbe_ptr which changes the attributes to commit to the first GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pa_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pa_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_pa_register_hbe_ptr> create_optional( affine_bg_ptr bg, const span<const int16_t>& values_ref)` (L95): Creates an affine_bg_pa_register_hbe_ptr which changes the values to commit to the first GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the first GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pa_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pa_register_hbe_ptr to avoid dangling references.
- **bg** `const affine_bg_ptr& bg() const` (L101): Returns the affine_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L113): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pa_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L122): Returns the referenced array of 160 values to commit to the first GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pa_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L131): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pa_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L140): Sets the reference to an array of 160 values to commit to the first GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pa_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L149): Rereads the content of the referenced attributes to commit to the first GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pa_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L158): Rereads the content of the referenced values to commit to the first GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pa_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(affine_bg_pa_register_hbe_ptr& other)` (L164): Exchanges the contents of this affine_bg_pa_register_hbe_ptr with those of the other one.
  `other` affine_bg_pa_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(affine_bg_pa_register_hbe_ptr& a, affine_bg_pa_register_hbe_ptr& b)` (L171): Exchanges the contents of an affine_bg_pa_register_hbe_ptr with those of another one.
  `a` First affine_bg_pa_register_hbe_ptr to exchange the contents with.; `b` Second affine_bg_pa_register_hbe_ptr to exchange the contents with.

## bn_affine_bg_pb_register_hbe_ptr.h

### `bn::affine_bg_pb_register_hbe_ptr` (class, bn_affine_bg_pb_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the second GBA register of an affine_bg_ptr in each screen horizontal line.

`class affine_bg_pb_register_hbe_ptr : public hbe_ptr`

- **create** `static affine_bg_pb_register_hbe_ptr create( affine_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L49): Creates an affine_bg_pb_register_hbe_ptr which changes the attributes to commit to the second GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pb_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pb_register_hbe_ptr to avoid dangling references.
- **create** `static affine_bg_pb_register_hbe_ptr create( affine_bg_ptr bg, const span<const int16_t>& values_ref)` (L64): Creates an affine_bg_pb_register_hbe_ptr which changes the values to commit to the second GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the second GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pb_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pb_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_pb_register_hbe_ptr> create_optional( affine_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L80): Creates an affine_bg_pb_register_hbe_ptr which changes the attributes to commit to the second GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pb_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pb_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_pb_register_hbe_ptr> create_optional( affine_bg_ptr bg, const span<const int16_t>& values_ref)` (L95): Creates an affine_bg_pb_register_hbe_ptr which changes the values to commit to the second GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the second GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pb_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pb_register_hbe_ptr to avoid dangling references.
- **bg** `const affine_bg_ptr& bg() const` (L101): Returns the affine_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L113): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pb_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L122): Returns the referenced array of 160 values to commit to the second GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pb_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L131): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pb_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L140): Sets the reference to an array of 160 values to commit to the second GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pb_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L149): Rereads the content of the referenced attributes to commit to the second GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pb_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L158): Rereads the content of the referenced values to commit to the second GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pb_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(affine_bg_pb_register_hbe_ptr& other)` (L164): Exchanges the contents of this affine_bg_pb_register_hbe_ptr with those of the other one.
  `other` affine_bg_pb_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(affine_bg_pb_register_hbe_ptr& a, affine_bg_pb_register_hbe_ptr& b)` (L171): Exchanges the contents of an affine_bg_pb_register_hbe_ptr with those of another one.
  `a` First affine_bg_pb_register_hbe_ptr to exchange the contents with.; `b` Second affine_bg_pb_register_hbe_ptr to exchange the contents with.

## bn_affine_bg_pc_register_hbe_ptr.h

### `bn::affine_bg_pc_register_hbe_ptr` (class, bn_affine_bg_pc_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the third GBA register of an affine_bg_ptr in each screen horizontal line.

`class affine_bg_pc_register_hbe_ptr : public hbe_ptr`

- **create** `static affine_bg_pc_register_hbe_ptr create( affine_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L49): Creates an affine_bg_pc_register_hbe_ptr which changes the attributes to commit to the third GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pc_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pc_register_hbe_ptr to avoid dangling references.
- **create** `static affine_bg_pc_register_hbe_ptr create( affine_bg_ptr bg, const span<const int16_t>& values_ref)` (L64): Creates an affine_bg_pc_register_hbe_ptr which changes the values to commit to the third GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the third GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pc_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pc_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_pc_register_hbe_ptr> create_optional( affine_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L80): Creates an affine_bg_pc_register_hbe_ptr which changes the attributes to commit to the third GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pc_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pc_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_pc_register_hbe_ptr> create_optional( affine_bg_ptr bg, const span<const int16_t>& values_ref)` (L95): Creates an affine_bg_pc_register_hbe_ptr which changes the values to commit to the third GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the third GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pc_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pc_register_hbe_ptr to avoid dangling references.
- **bg** `const affine_bg_ptr& bg() const` (L101): Returns the affine_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L113): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pc_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L122): Returns the referenced array of 160 values to commit to the third GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pc_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L131): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pc_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L140): Sets the reference to an array of 160 values to commit to the third GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pc_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L149): Rereads the content of the referenced attributes to commit to the third GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pc_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L158): Rereads the content of the referenced values to commit to the third GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pc_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(affine_bg_pc_register_hbe_ptr& other)` (L164): Exchanges the contents of this affine_bg_pc_register_hbe_ptr with those of the other one.
  `other` affine_bg_pc_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(affine_bg_pc_register_hbe_ptr& a, affine_bg_pc_register_hbe_ptr& b)` (L171): Exchanges the contents of an affine_bg_pc_register_hbe_ptr with those of another one.
  `a` First affine_bg_pc_register_hbe_ptr to exchange the contents with.; `b` Second affine_bg_pc_register_hbe_ptr to exchange the contents with.

## bn_affine_bg_pd_register_hbe_ptr.h

### `bn::affine_bg_pd_register_hbe_ptr` (class, bn_affine_bg_pd_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the fourth GBA register of an affine_bg_ptr in each screen horizontal line.

`class affine_bg_pd_register_hbe_ptr : public hbe_ptr`

- **create** `static affine_bg_pd_register_hbe_ptr create( affine_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L49): Creates an affine_bg_pd_register_hbe_ptr which changes the attributes to commit to the fourth GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pd_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pd_register_hbe_ptr to avoid dangling references.
- **create** `static affine_bg_pd_register_hbe_ptr create( affine_bg_ptr bg, const span<const int16_t>& values_ref)` (L64): Creates an affine_bg_pd_register_hbe_ptr which changes the values to commit to the fourth GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the fourth GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pd_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pd_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_pd_register_hbe_ptr> create_optional( affine_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L80): Creates an affine_bg_pd_register_hbe_ptr which changes the attributes to commit to the fourth GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pd_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pd_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<affine_bg_pd_register_hbe_ptr> create_optional( affine_bg_ptr bg, const span<const int16_t>& values_ref)` (L95): Creates an affine_bg_pd_register_hbe_ptr which changes the values to commit to the fourth GBA register of an affine_bg_ptr.
  `bg` affine_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the fourth GBA register of an affine_bg_ptr in each screen horizontal line.; devuelve: The requested affine_bg_pd_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pd_register_hbe_ptr to avoid dangling references.
- **bg** `const affine_bg_ptr& bg() const` (L101): Returns the affine_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L113): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pd_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L122): Returns the referenced array of 160 values to commit to the fourth GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pd_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L131): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pd_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L140): Sets the reference to an array of 160 values to commit to the fourth GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pd_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L149): Rereads the content of the referenced attributes to commit to the fourth GBA register of an affine_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the affine_bg_pd_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L158): Rereads the content of the referenced values to commit to the fourth GBA register of an affine_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the affine_bg_pd_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(affine_bg_pd_register_hbe_ptr& other)` (L164): Exchanges the contents of this affine_bg_pd_register_hbe_ptr with those of the other one.
  `other` affine_bg_pd_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(affine_bg_pd_register_hbe_ptr& a, affine_bg_pd_register_hbe_ptr& b)` (L171): Exchanges the contents of an affine_bg_pd_register_hbe_ptr with those of another one.
  `a` First affine_bg_pd_register_hbe_ptr to exchange the contents with.; `b` Second affine_bg_pd_register_hbe_ptr to exchange the contents with.

## bn_affine_bg_pivot_position_hbe_ptr.h

### `bn::affine_bg_pivot_position_hbe_ptr` (class, bn_affine_bg_pivot_position_hbe_ptr.h:32)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the position of the affine transformations pivot point of an affine background in each screen horizontal line.

`class affine_bg_pivot_position_hbe_ptr : public hbe_ptr`

- **create_horizontal** `static affine_bg_pivot_position_hbe_ptr create_horizontal( affine_bg_ptr bg, const span<const fixed>& deltas_ref)` (L49): Creates an affine_bg_pivot_position_hbe_ptr which changes the horizontal position of the affine transformations pivot point of an affine background in each screen horizontal line.
  `bg` Affine background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the horizontal position of the affine transformations pivot point of the given affine background in each screen horizontal line.; devuelve: The requested affine_bg_pivot_position_hbe_ptr.
  The deltas are not copied but referenced,
  so they should outlive the affine_bg_pivot_position_hbe_ptr to avoid dangling references.
- **create_vertical** `static affine_bg_pivot_position_hbe_ptr create_vertical( affine_bg_ptr bg, const span<const fixed>& deltas_ref)` (L65): Creates an affine_bg_pivot_position_hbe_ptr which changes the vertical position of the affine transformations pivot point of an affine background in each screen horizontal line.
  `bg` Affine background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the vertical position of the affine transformations pivot point of the given affine background in each screen horizontal line.; devuelve: The requested affine_bg_pivot_position_hbe_ptr.
  The deltas are not copied but referenced,
  so they should outlive the affine_bg_pivot_position_hbe_ptr to avoid dangling references.
- **create_horizontal_optional** `static optional<affine_bg_pivot_position_hbe_ptr> create_horizontal_optional( affine_bg_ptr bg, const span<const fixed>& deltas_ref)` (L81): Creates an affine_bg_pivot_position_hbe_ptr which changes the horizontal position of the affine transformations pivot point of an affine background in each screen horizontal line.
  `bg` Affine background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the horizontal position of the affine transformations pivot point of the given affine background in each screen horizontal line.; devuelve: The requested affine_bg_pivot_position_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The deltas are not copied but referenced,
  so they should outlive the affine_bg_pivot_position_hbe_ptr to avoid dangling references.
- **create_vertical_optional** `static optional<affine_bg_pivot_position_hbe_ptr> create_vertical_optional( affine_bg_ptr bg, const span<const fixed>& deltas_ref)` (L97): Creates an affine_bg_pivot_position_hbe_ptr which changes the vertical position of the affine transformations pivot point of an affine background in each screen horizontal line.
  `bg` Affine background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the vertical position of the affine transformations pivot point of the given affine background in each screen horizontal line.; devuelve: The requested affine_bg_pivot_position_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The deltas are not copied but referenced,
  so they should outlive the affine_bg_pivot_position_hbe_ptr to avoid dangling references.
- **bg** `const affine_bg_ptr& bg() const` (L103): Returns the affine background modified by this H-Blank effect.
- **deltas_ref** `span<const fixed> deltas_ref() const` (L115): Returns the referenced array of 160 deltas to add to the horizontal or vertical position of the affine transformations pivot point of the managed affine background in each screen horizontal line.
  The deltas are not copied but referenced,
  so they should outlive the affine_bg_pivot_position_hbe_ptr to avoid dangling references.
- **set_deltas_ref** `void set_deltas_ref(const span<const fixed>& deltas_ref)` (L124): Sets the reference to an array of 160 deltas to add to the horizontal or vertical position of the affine transformations pivot point of the managed affine background in each screen horizontal line.
  The deltas are not copied but referenced,
  so they should outlive the affine_bg_pivot_position_hbe_ptr to avoid dangling references.
- **reload_deltas_ref** `void reload_deltas_ref()` (L133): Rereads the content of the referenced deltas to add to the horizontal or vertical position of the affine transformations pivot point of the managed affine background in each screen horizontal line.
  The deltas are not copied but referenced,
  so they should outlive the affine_bg_pivot_position_hbe_ptr to avoid dangling references.
- **swap** `void swap(affine_bg_pivot_position_hbe_ptr& other)` (L139): Exchanges the contents of this affine_bg_pivot_position_hbe_ptr with those of the other one.
  `other` affine_bg_pivot_position_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(affine_bg_pivot_position_hbe_ptr& a, affine_bg_pivot_position_hbe_ptr& b)` (L146): Exchanges the contents of an affine_bg_pivot_position_hbe_ptr with those of another one.
  `a` First affine_bg_pivot_position_hbe_ptr to exchange the contents with.; `b` Second affine_bg_pivot_position_hbe_ptr to exchange the contents with.

## bn_backdrop_color_hbe_ptr.h

### `bn::backdrop_color_hbe_ptr` (class, bn_backdrop_color_hbe_ptr.h:37)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the backdrop color in each screen horizontal line.

`class backdrop_color_hbe_ptr : public hbe_ptr`

The backdrop color is the transparent color of the backgrounds.

- **create** `static backdrop_color_hbe_ptr create(const span<const color>& colors_ref)` (L51): Creates a backdrop_color_hbe_ptr which changes the backdrop color in each screen horizontal line.
  `colors_ref` Reference to an array of 160 color objects which indicate the backdrop color in each screen horizontal line.; devuelve: The requested backdrop_color_hbe_ptr.
  The colors are not copied but referenced, so they should outlive the backdrop_color_hbe_ptr
  to avoid dangling references.
- **create_optional** `static optional<backdrop_color_hbe_ptr> create_optional(const span<const color>& colors_ref)` (L63): Creates a backdrop_color_hbe_ptr which changes the backdrop color in each screen horizontal line.
  `colors_ref` Reference to an array of 160 color objects which indicate the backdrop color in each screen horizontal line.; devuelve: The requested backdrop_color_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The colors are not copied but referenced, so they should outlive the backdrop_color_hbe_ptr
  to avoid dangling references.
- **colors_ref** `span<const color> colors_ref() const` (L72): Returns the referenced array of 160 color objects which indicate the backdrop color in each screen horizontal line.
  The colors are not copied but referenced, so they should outlive the backdrop_color_hbe_ptr
  to avoid dangling references.
- **set_colors_ref** `void set_colors_ref(const span<const color>& colors_ref)` (L81): Sets the reference to an array of 160 color objects which indicate the backdrop color in each screen horizontal line.
  The colors are not copied but referenced, so they should outlive the backdrop_color_hbe_ptr
  to avoid dangling references.
- **reload_colors_ref** `void reload_colors_ref()` (L90): Rereads the content of the referenced color objects which indicate the backdrop color in each screen horizontal line.
  The colors are not copied but referenced, so they should outlive the backdrop_color_hbe_ptr
  to avoid dangling references.
- **swap** `void swap(backdrop_color_hbe_ptr& other)` (L96): Exchanges the contents of this backdrop_color_hbe_ptr with those of the other one.
  `other` backdrop_color_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(backdrop_color_hbe_ptr& a, backdrop_color_hbe_ptr& b)` (L103): Exchanges the contents of a backdrop_color_hbe_ptr with those of another one.
  `a` First backdrop_color_hbe_ptr to exchange the contents with.; `b` Second backdrop_color_hbe_ptr to exchange the contents with.

## bn_bg_palette_color_hbe_ptr.h

### `bn::bg_palette_color_hbe_ptr` (class, bn_bg_palette_color_hbe_ptr.h:34)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the color of a background color palette in each screen horizontal line.

`class bg_palette_color_hbe_ptr : public hbe_ptr`

- **create** `static bg_palette_color_hbe_ptr create( bg_palette_ptr palette, int color_index, const span<const color>& colors_ref)` (L51): Creates a bg_palette_color_hbe_ptr which changes the color of a background color palette in each screen horizontal line.
  `palette` Background color palette to be modified.; `color_index` Index of the color of the given background color palette to be modified.; `colors_ref` Reference to an array of 160 colors to set to the color of a background color palette in each screen horizontal line.; devuelve: The requested bg_palette_color_hbe_ptr.
  The colors are not copied but referenced, so they should outlive the bg_palette_color_hbe_ptr
  to avoid dangling references.
- **create_optional** `static optional<bg_palette_color_hbe_ptr> create_optional( bg_palette_ptr palette, int color_index, const span<const color>& colors_ref)` (L67): Creates a bg_palette_color_hbe_ptr which changes the color of a background color palette in each screen horizontal line.
  `palette` Background color palette to be modified.; `color_index` Index of the color of the given background color palette to be modified.; `colors_ref` Reference to an array of 160 colors to set to the color of a background color palette in each screen horizontal line.; devuelve: The requested bg_palette_color_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The colors are not copied but referenced, so they should outlive the bg_palette_color_hbe_ptr
  to avoid dangling references.
- **palette** `const bg_palette_ptr& palette() const` (L73): Returns the background color palette modified by this H-Blank effect.
- **color_index** `int color_index() const` (L81): Returns the index of the color of the given background color palette to be modified.
- **colors_ref** `span<const color> colors_ref() const` (L93): Returns the referenced array of 160 colors to set to the color of a background color palette in each screen horizontal line.
  The colors are not copied but referenced, so they should outlive the bg_palette_color_hbe_ptr
  to avoid dangling references.
- **set_colors_ref** `void set_colors_ref(const span<const color>& colors_ref)` (L102): Sets the reference to an array of 160 colors to set to the color of a background color palette in each screen horizontal line.
  The colors are not copied but referenced, so they should outlive the bg_palette_color_hbe_ptr
  to avoid dangling references.
- **reload_colors_ref** `void reload_colors_ref()` (L111): Rereads the content of the referenced colors to set to the color of a background color palette in each screen horizontal line.
  The colors are not copied but referenced, so they should outlive the bg_palette_color_hbe_ptr
  to avoid dangling references.
- **swap** `void swap(bg_palette_color_hbe_ptr& other)` (L117): Exchanges the contents of this bg_palette_color_hbe_ptr with those of the other one.
  `other` bg_palette_color_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(bg_palette_color_hbe_ptr& a, bg_palette_color_hbe_ptr& b)` (L124): Exchanges the contents of a bg_palette_color_hbe_ptr with those of another one.
  `a` First bg_palette_color_hbe_ptr to exchange the contents with.; `b` Second bg_palette_color_hbe_ptr to exchange the contents with.

## bn_bg_palettes_transparent_color_hbe_ptr.h

### `bn::bg_palettes_transparent_color_hbe_ptr` (class, bn_bg_palettes_transparent_color_hbe_ptr.h:35)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the transparent color of the backgrounds in each screen horizontal line.

`class bg_palettes_transparent_color_hbe_ptr : public hbe_ptr`

- **create** `static bg_palettes_transparent_color_hbe_ptr create( const span<const color>& colors_ref)` (L50): Creates a bg_palettes_transparent_color_hbe_ptr which changes the transparent color of the backgrounds in each screen horizontal line.
  `colors_ref` Reference to an array of 160 color objects which indicate the transparent color of the backgrounds in each screen horizontal line.; devuelve: The requested bg_palettes_transparent_color_hbe_ptr.
  The colors are not copied but referenced, so they should outlive
  the bg_palettes_transparent_color_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<bg_palettes_transparent_color_hbe_ptr> create_optional( const span<const color>& colors_ref)` (L64): Creates a bg_palettes_transparent_color_hbe_ptr which changes the transparent color of the backgrounds in each screen horizontal line.
  `colors_ref` Reference to an array of 160 color objects which indicate the transparent color of the backgrounds in each screen horizontal line.; devuelve: The requested bg_palettes_transparent_color_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The colors are not copied but referenced, so they should outlive
  the bg_palettes_transparent_color_hbe_ptr to avoid dangling references.
- **colors_ref** `span<const color> colors_ref() const` (L74): Returns the referenced array of 160 color objects which indicate the transparent color of the backgrounds in each screen horizontal line.
  The colors are not copied but referenced, so they should outlive
  the bg_palettes_transparent_color_hbe_ptr to avoid dangling references.
- **set_colors_ref** `void set_colors_ref(const span<const color>& colors_ref)` (L83): Sets the reference to an array of 160 color objects which indicate the transparent color of the backgrounds in each screen horizontal line.
  The colors are not copied but referenced, so they should outlive
  the bg_palettes_transparent_color_hbe_ptr to avoid dangling references.
- **reload_colors_ref** `void reload_colors_ref()` (L92): Rereads the content of the referenced color objects which indicate the transparent color of the backgrounds in each screen horizontal line.
  The colors are not copied but referenced, so they should outlive
  the bg_palettes_transparent_color_hbe_ptr to avoid dangling references.
- **swap** `void swap(bg_palettes_transparent_color_hbe_ptr& other)` (L99): Exchanges the contents of this bg_palettes_transparent_color_hbe_ptr with those of the other one.
  `other` bg_palettes_transparent_color_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(bg_palettes_transparent_color_hbe_ptr& a, bg_palettes_transparent_color_hbe_ptr& b)` (L106): Exchanges the contents of a bg_palettes_transparent_color_hbe_ptr with those of another one.
  `a` First bg_palettes_transparent_color_hbe_ptr to exchange the contents with.; `b` Second bg_palettes_transparent_color_hbe_ptr to exchange the contents with.

## bn_blending_fade_alpha_hbe_ptr.h

### `bn::blending_fade_alpha_hbe_ptr` (class, bn_blending_fade_alpha_hbe_ptr.h:35)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the weight of the fade blending in each screen horizontal line.

`class blending_fade_alpha_hbe_ptr : public hbe_ptr`

Keep in mind that fade blending and other blendings can't be enabled at the same time.

- **create** `static blending_fade_alpha_hbe_ptr create( const span<const blending_fade_alpha>& alphas_ref)` (L50): Creates a blending_fade_alpha_hbe_ptr which changes the weight of the fade blending in each screen horizontal line.
  `alphas_ref` Reference to an array of 160 blending_fade_alpha objects which indicate the weight of the fade blending in each screen horizontal line.; devuelve: The requested blending_fade_alpha_hbe_ptr.
  The alphas are not copied but referenced, so they should outlive
  the blending_fade_alpha_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<blending_fade_alpha_hbe_ptr> create_optional( const span<const blending_fade_alpha>& alphas_ref)` (L64): Creates a blending_fade_alpha_hbe_ptr which changes the weight of the fade blending in each screen horizontal line.
  `alphas_ref` Reference to an array of 160 blending_fade_alpha objects which indicate the weight of the fade blending in each screen horizontal line.; devuelve: The requested blending_fade_alpha_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The alphas are not copied but referenced, so they should outlive
  the blending_fade_alpha_hbe_ptr to avoid dangling references.
- **alphas_ref** `span<const blending_fade_alpha> alphas_ref() const` (L74): Returns the referenced array of 160 blending_fade_alpha objects which indicate the weight of the fade blending in each screen horizontal line.
  The alphas are not copied but referenced, so they should outlive
  the blending_fade_alpha_hbe_ptr to avoid dangling references.
- **set_alphas_ref** `void set_alphas_ref(const span<const blending_fade_alpha>& alphas_ref)` (L83): Sets the reference to an array of 160 blending_fade_alpha objects which indicate the weight of the fade blending in each screen horizontal line.
  The alphas are not copied but referenced, so they should outlive
  the blending_fade_alpha_hbe_ptr to avoid dangling references.
- **reload_alphas_ref** `void reload_alphas_ref()` (L92): Rereads the content of the referenced blending_fade_alpha objects which indicate the weight of the fade blending in each screen horizontal line.
  The alphas are not copied but referenced, so they should outlive
  the blending_fade_alpha_hbe_ptr to avoid dangling references.
- **swap** `void swap(blending_fade_alpha_hbe_ptr& other)` (L98): Exchanges the contents of this blending_fade_alpha_hbe_ptr with those of the other one.
  `other` blending_fade_alpha_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(blending_fade_alpha_hbe_ptr& a, blending_fade_alpha_hbe_ptr& b)` (L108): Exchanges the contents of a blending_fade_alpha_hbe_ptr with those of another one.
  `a` First blending_fade_alpha_hbe_ptr to exchange the contents with.; `b` Second blending_fade_alpha_hbe_ptr to exchange the contents with.

## bn_blending_transparency_attributes_hbe_ptr.h

### `bn::blending_transparency_attributes_hbe_ptr` (class, bn_blending_transparency_attributes_hbe_ptr.h:35)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the weights of the transparency and intensity blendings in each screen horizontal line.

`class blending_transparency_attributes_hbe_ptr : public hbe_ptr`

Keep in mind that fade blending and other blendings can't be enabled at the same time.

- **create** `static blending_transparency_attributes_hbe_ptr create( const span<const blending_transparency_attributes>& attributes_ref)` (L50): Creates a blending_transparency_attributes_hbe_ptr which changes the weights of the transparency and intensity blendings in each screen horizontal line.
  `attributes_ref` Reference to an array of 160 blending_transparency_attributes objects which indicate the weights of the transparency and intensity blendings in each screen horizontal line.; devuelve: The requested blending_transparency_attributes_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the blending_transparency_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<blending_transparency_attributes_hbe_ptr> create_optional( const span<const blending_transparency_attributes>& attributes_ref)` (L65): Creates a blending_transparency_attributes_hbe_ptr which changes the weights of the transparency and intensity blendings in each screen horizontal line.
  `attributes_ref` Reference to an array of 160 blending_transparency_attributes objects which indicate the weights of the transparency and intensity blendings in each screen horizontal line.; devuelve: The requested blending_transparency_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the blending_transparency_attributes_hbe_ptr to avoid dangling references.
- **attributes_ref** `span<const blending_transparency_attributes> attributes_ref() const` (L75): Returns the referenced array of 160 blending_transparency_attributes objects which indicate the weights of the transparency and intensity blendings in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the blending_transparency_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const blending_transparency_attributes>& attributes_ref)` (L84): Sets the reference to an array of 160 blending_transparency_attributes objects which indicate the weights of the transparency and intensity blendings in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the blending_transparency_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L93): Rereads the content of the referenced blending_transparency_attributes objects which indicate the weights of the transparency and intensity blendings in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the blending_transparency_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(blending_transparency_attributes_hbe_ptr& other)` (L100): Exchanges the contents of this blending_transparency_attributes_hbe_ptr with those of the other one.
  `other` blending_transparency_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(blending_transparency_attributes_hbe_ptr& a, blending_transparency_attributes_hbe_ptr& b)` (L111): Exchanges the contents of a blending_transparency_attributes_hbe_ptr with those of another one.
  `a` First blending_transparency_attributes_hbe_ptr to exchange the contents with.; `b` Second blending_transparency_attributes_hbe_ptr to exchange the contents with.

## bn_config_hbes.h

### `BN_CFG_HBES_MAX_ITEMS` (macro, bn_config_hbes.h:29)

Specifies the maximum number of active H-Blank effects.

`#define BN_CFG_HBES_MAX_ITEMS 6`


## bn_dp_direct_bitmap_bg_attributes_hbe_ptr.h

### `bn::dp_direct_bitmap_bg_attributes_hbe_ptr` (class, bn_dp_direct_bitmap_bg_attributes_hbe_ptr.h:32)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes to commit to the GBA register of a dual page direct bitmap background in each screen horizontal line.

`class dp_direct_bitmap_bg_attributes_hbe_ptr : public hbe_ptr`

- **create** `static dp_direct_bitmap_bg_attributes_hbe_ptr create( dp_direct_bitmap_bg_ptr bg, const span<const bitmap_bg_attributes>& attributes_ref)` (L48): Creates a dp_direct_bitmap_bg_attributes_hbe_ptr which changes the attributes to commit to the GBA register of a dual page direct bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `attributes_ref` Reference to an array of 160 bitmap_bg_attributes objects with the attributes to commit to the GBA register of the given background in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_attributes_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<dp_direct_bitmap_bg_attributes_hbe_ptr> create_optional( dp_direct_bitmap_bg_ptr bg, const span<const bitmap_bg_attributes>& attributes_ref)` (L63): Creates a dp_direct_bitmap_bg_attributes_hbe_ptr which changes the attributes to commit to the GBA register of a dual page direct bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `attributes_ref` Reference to an array of 160 bitmap_bg_attributes objects with the attributes to commit to the GBA register of the given background in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L69): Returns the affine background modified by this H-Blank effect.
- **attributes_ref** `span<const bitmap_bg_attributes> attributes_ref() const` (L81): Returns the referenced array of 160 bitmap_bg_attributes objects with the attributes to commit to the GBA register of the managed background in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const bitmap_bg_attributes>& attributes_ref)` (L90): Sets the reference to an array of 160 bitmap_bg_attributes objects with the attributes to commit to the GBA register of the managed background in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L99): Rereads the content of the referenced attributes to the GBA register of the managed background in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(dp_direct_bitmap_bg_attributes_hbe_ptr& other)` (L105): Exchanges the contents of this dp_direct_bitmap_bg_attributes_hbe_ptr with those of the other one.
  `other` dp_direct_bitmap_bg_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(dp_direct_bitmap_bg_attributes_hbe_ptr& a, dp_direct_bitmap_bg_attributes_hbe_ptr& b)` (L112): Exchanges the contents of a dp_direct_bitmap_bg_attributes_hbe_ptr with those of another one.
  `a` First dp_direct_bitmap_bg_attributes_hbe_ptr to exchange the contents with.; `b` Second dp_direct_bitmap_bg_attributes_hbe_ptr to exchange the contents with.

## bn_dp_direct_bitmap_bg_dx_register_hbe_ptr.h

### `bn::dp_direct_bitmap_bg_dx_register_hbe_ptr` (class, bn_dp_direct_bitmap_bg_dx_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the the fifth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.

`class dp_direct_bitmap_bg_dx_register_hbe_ptr : public hbe_ptr`

- **create** `static dp_direct_bitmap_bg_dx_register_hbe_ptr create( dp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a dp_direct_bitmap_bg_dx_register_hbe_ptr which changes the attributes to commit to the the fifth GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_dx_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **create** `static dp_direct_bitmap_bg_dx_register_hbe_ptr create( dp_direct_bitmap_bg_ptr bg, const span<const int>& values_ref)` (L65): Creates a dp_direct_bitmap_bg_dx_register_hbe_ptr which changes the values to commit to the the fifth GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the fifth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_dx_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<dp_direct_bitmap_bg_dx_register_hbe_ptr> create_optional( dp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a dp_direct_bitmap_bg_dx_register_hbe_ptr which changes the attributes to commit to the the fifth GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_dx_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<dp_direct_bitmap_bg_dx_register_hbe_ptr> create_optional( dp_direct_bitmap_bg_ptr bg, const span<const int>& values_ref)` (L96): Creates a dp_direct_bitmap_bg_dx_register_hbe_ptr which changes the values to commit to the the fifth GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the fifth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_dx_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L102): Returns the dp_direct_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the the fifth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the the fifth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the the fifth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the the fifth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(dp_direct_bitmap_bg_dx_register_hbe_ptr& other)` (L165): Exchanges the contents of this dp_direct_bitmap_bg_dx_register_hbe_ptr with those of the other one.
  `other` dp_direct_bitmap_bg_dx_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(dp_direct_bitmap_bg_dx_register_hbe_ptr& a, dp_direct_bitmap_bg_dx_register_hbe_ptr& b)` (L172): Exchanges the contents of a dp_direct_bitmap_bg_dx_register_hbe_ptr with those of another one.
  `a` First dp_direct_bitmap_bg_dx_register_hbe_ptr to exchange the contents with.; `b` Second dp_direct_bitmap_bg_dx_register_hbe_ptr to exchange the contents with.

## bn_dp_direct_bitmap_bg_dy_register_hbe_ptr.h

### `bn::dp_direct_bitmap_bg_dy_register_hbe_ptr` (class, bn_dp_direct_bitmap_bg_dy_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the the sixth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.

`class dp_direct_bitmap_bg_dy_register_hbe_ptr : public hbe_ptr`

- **create** `static dp_direct_bitmap_bg_dy_register_hbe_ptr create( dp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a dp_direct_bitmap_bg_dy_register_hbe_ptr which changes the attributes to commit to the the sixth GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_dy_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **create** `static dp_direct_bitmap_bg_dy_register_hbe_ptr create( dp_direct_bitmap_bg_ptr bg, const span<const int>& values_ref)` (L65): Creates a dp_direct_bitmap_bg_dy_register_hbe_ptr which changes the values to commit to the the sixth GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the sixth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_dy_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<dp_direct_bitmap_bg_dy_register_hbe_ptr> create_optional( dp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a dp_direct_bitmap_bg_dy_register_hbe_ptr which changes the attributes to commit to the the sixth GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_dy_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<dp_direct_bitmap_bg_dy_register_hbe_ptr> create_optional( dp_direct_bitmap_bg_ptr bg, const span<const int>& values_ref)` (L96): Creates a dp_direct_bitmap_bg_dy_register_hbe_ptr which changes the values to commit to the the sixth GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the sixth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_dy_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L102): Returns the dp_direct_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the the sixth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the the sixth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the the sixth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the the sixth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(dp_direct_bitmap_bg_dy_register_hbe_ptr& other)` (L165): Exchanges the contents of this dp_direct_bitmap_bg_dy_register_hbe_ptr with those of the other one.
  `other` dp_direct_bitmap_bg_dy_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(dp_direct_bitmap_bg_dy_register_hbe_ptr& a, dp_direct_bitmap_bg_dy_register_hbe_ptr& b)` (L172): Exchanges the contents of a dp_direct_bitmap_bg_dy_register_hbe_ptr with those of another one.
  `a` First dp_direct_bitmap_bg_dy_register_hbe_ptr to exchange the contents with.; `b` Second dp_direct_bitmap_bg_dy_register_hbe_ptr to exchange the contents with.

## bn_dp_direct_bitmap_bg_mat_attributes_hbe_ptr.h

### `bn::dp_direct_bitmap_bg_mat_attributes_hbe_ptr` (class, bn_dp_direct_bitmap_bg_mat_attributes_hbe_ptr.h:36)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes which define the position and the transformation matrix of a dp_direct_bitmap_bg_ptr in each screen horizontal line.

`class dp_direct_bitmap_bg_mat_attributes_hbe_ptr`

- **create** `static dp_direct_bitmap_bg_mat_attributes_hbe_ptr create( const dp_direct_bitmap_bg_ptr& bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L53): Creates a dp_direct_bitmap_bg_mat_attributes_hbe_ptr which changes the attributes which define the position and the transformation matrix of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to set to the given dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_mat_attributes_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<dp_direct_bitmap_bg_mat_attributes_hbe_ptr> create_optional( const dp_direct_bitmap_bg_ptr& bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L70): Creates a dp_direct_bitmap_bg_mat_attributes_hbe_ptr which changes the attributes which define the position and the transformation matrix of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to set to the given dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_mat_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **id** `int id() const` (L76): Returns the internal id.
- **visible** `bool visible() const` (L84): Indicates if this H-Blank effect must be committed to the GBA or not.
- **set_visible** `void set_visible(bool visible)` (L92): Sets if this H-Blank effect must be committed to the GBA or not.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L97): Returns the dp_direct_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L109): Returns the referenced array of 160 affine_bg_mat_attributes objects to set to the given dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L118): Sets the reference to an array of 160 affine_bg_mat_attributes objects to set to the given dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L127): Rereads the content of the referenced attributes to set to the given dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(dp_direct_bitmap_bg_mat_attributes_hbe_ptr& other)` (L133): Exchanges the contents of this dp_direct_bitmap_bg_mat_attributes_hbe_ptr with those of the other one.
  `other` dp_direct_bitmap_bg_mat_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(dp_direct_bitmap_bg_mat_attributes_hbe_ptr& a, dp_direct_bitmap_bg_mat_attributes_hbe_ptr& b)` (L140): Exchanges the contents of a dp_direct_bitmap_bg_mat_attributes_hbe_ptr with those of another one.
  `a` First dp_direct_bitmap_bg_mat_attributes_hbe_ptr to exchange the contents with.; `b` Second dp_direct_bitmap_bg_mat_attributes_hbe_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const dp_direct_bitmap_bg_mat_attributes_hbe_ptr& a, const dp_direct_bitmap_bg_mat_attributes_hbe_ptr& b)` (L151): Equal operator.
  `a` First dp_direct_bitmap_bg_mat_attributes_hbe_ptr to compare.; `b` Second dp_direct_bitmap_bg_mat_attributes_hbe_ptr to compare.; devuelve: `true` if the first dp_direct_bitmap_bg_mat_attributes_hbe_ptr is equal to the second one, otherwise `false`.
- **operator!=** `friend bool operator!=(const dp_direct_bitmap_bg_mat_attributes_hbe_ptr& a, const dp_direct_bitmap_bg_mat_attributes_hbe_ptr& b)` (L163): Not equal operator.
  `a` First dp_direct_bitmap_bg_mat_attributes_hbe_ptr to compare.; `b` Second dp_direct_bitmap_bg_mat_attributes_hbe_ptr to compare.; devuelve: `true` if the first dp_direct_bitmap_bg_mat_attributes_hbe_ptr is not equal to the second one, otherwise `false`.

## bn_dp_direct_bitmap_bg_pa_register_hbe_ptr.h

### `bn::dp_direct_bitmap_bg_pa_register_hbe_ptr` (class, bn_dp_direct_bitmap_bg_pa_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the first GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.

`class dp_direct_bitmap_bg_pa_register_hbe_ptr : public hbe_ptr`

- **create** `static dp_direct_bitmap_bg_pa_register_hbe_ptr create( dp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a dp_direct_bitmap_bg_pa_register_hbe_ptr which changes the attributes to commit to the first GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pa_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **create** `static dp_direct_bitmap_bg_pa_register_hbe_ptr create( dp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L65): Creates a dp_direct_bitmap_bg_pa_register_hbe_ptr which changes the values to commit to the first GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the first GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pa_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<dp_direct_bitmap_bg_pa_register_hbe_ptr> create_optional( dp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a dp_direct_bitmap_bg_pa_register_hbe_ptr which changes the attributes to commit to the first GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pa_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<dp_direct_bitmap_bg_pa_register_hbe_ptr> create_optional( dp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L96): Creates a dp_direct_bitmap_bg_pa_register_hbe_ptr which changes the values to commit to the first GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the first GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pa_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L102): Returns the dp_direct_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the first GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the first GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the first GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the first GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(dp_direct_bitmap_bg_pa_register_hbe_ptr& other)` (L165): Exchanges the contents of this dp_direct_bitmap_bg_pa_register_hbe_ptr with those of the other one.
  `other` dp_direct_bitmap_bg_pa_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(dp_direct_bitmap_bg_pa_register_hbe_ptr& a, dp_direct_bitmap_bg_pa_register_hbe_ptr& b)` (L172): Exchanges the contents of a dp_direct_bitmap_bg_pa_register_hbe_ptr with those of another one.
  `a` First dp_direct_bitmap_bg_pa_register_hbe_ptr to exchange the contents with.; `b` Second dp_direct_bitmap_bg_pa_register_hbe_ptr to exchange the contents with.

## bn_dp_direct_bitmap_bg_pb_register_hbe_ptr.h

### `bn::dp_direct_bitmap_bg_pb_register_hbe_ptr` (class, bn_dp_direct_bitmap_bg_pb_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the second GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.

`class dp_direct_bitmap_bg_pb_register_hbe_ptr : public hbe_ptr`

- **create** `static dp_direct_bitmap_bg_pb_register_hbe_ptr create( dp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a dp_direct_bitmap_bg_pb_register_hbe_ptr which changes the attributes to commit to the second GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pb_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **create** `static dp_direct_bitmap_bg_pb_register_hbe_ptr create( dp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L65): Creates a dp_direct_bitmap_bg_pb_register_hbe_ptr which changes the values to commit to the second GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the second GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pb_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<dp_direct_bitmap_bg_pb_register_hbe_ptr> create_optional( dp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a dp_direct_bitmap_bg_pb_register_hbe_ptr which changes the attributes to commit to the second GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pb_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<dp_direct_bitmap_bg_pb_register_hbe_ptr> create_optional( dp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L96): Creates a dp_direct_bitmap_bg_pb_register_hbe_ptr which changes the values to commit to the second GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the second GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pb_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L102): Returns the dp_direct_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the second GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the second GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the second GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the second GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(dp_direct_bitmap_bg_pb_register_hbe_ptr& other)` (L165): Exchanges the contents of this dp_direct_bitmap_bg_pb_register_hbe_ptr with those of the other one.
  `other` dp_direct_bitmap_bg_pb_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(dp_direct_bitmap_bg_pb_register_hbe_ptr& a, dp_direct_bitmap_bg_pb_register_hbe_ptr& b)` (L172): Exchanges the contents of a dp_direct_bitmap_bg_pb_register_hbe_ptr with those of another one.
  `a` First dp_direct_bitmap_bg_pb_register_hbe_ptr to exchange the contents with.; `b` Second dp_direct_bitmap_bg_pb_register_hbe_ptr to exchange the contents with.

## bn_dp_direct_bitmap_bg_pc_register_hbe_ptr.h

### `bn::dp_direct_bitmap_bg_pc_register_hbe_ptr` (class, bn_dp_direct_bitmap_bg_pc_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the third GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.

`class dp_direct_bitmap_bg_pc_register_hbe_ptr : public hbe_ptr`

- **create** `static dp_direct_bitmap_bg_pc_register_hbe_ptr create( dp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a dp_direct_bitmap_bg_pc_register_hbe_ptr which changes the attributes to commit to the third GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pc_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **create** `static dp_direct_bitmap_bg_pc_register_hbe_ptr create( dp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L65): Creates a dp_direct_bitmap_bg_pc_register_hbe_ptr which changes the values to commit to the third GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the third GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pc_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<dp_direct_bitmap_bg_pc_register_hbe_ptr> create_optional( dp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a dp_direct_bitmap_bg_pc_register_hbe_ptr which changes the attributes to commit to the third GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pc_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<dp_direct_bitmap_bg_pc_register_hbe_ptr> create_optional( dp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L96): Creates a dp_direct_bitmap_bg_pc_register_hbe_ptr which changes the values to commit to the third GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the third GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pc_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L102): Returns the dp_direct_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the third GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the third GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the third GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the third GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(dp_direct_bitmap_bg_pc_register_hbe_ptr& other)` (L165): Exchanges the contents of this dp_direct_bitmap_bg_pc_register_hbe_ptr with those of the other one.
  `other` dp_direct_bitmap_bg_pc_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(dp_direct_bitmap_bg_pc_register_hbe_ptr& a, dp_direct_bitmap_bg_pc_register_hbe_ptr& b)` (L172): Exchanges the contents of a dp_direct_bitmap_bg_pc_register_hbe_ptr with those of another one.
  `a` First dp_direct_bitmap_bg_pc_register_hbe_ptr to exchange the contents with.; `b` Second dp_direct_bitmap_bg_pc_register_hbe_ptr to exchange the contents with.

## bn_dp_direct_bitmap_bg_pd_register_hbe_ptr.h

### `bn::dp_direct_bitmap_bg_pd_register_hbe_ptr` (class, bn_dp_direct_bitmap_bg_pd_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the fourth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.

`class dp_direct_bitmap_bg_pd_register_hbe_ptr : public hbe_ptr`

- **create** `static dp_direct_bitmap_bg_pd_register_hbe_ptr create( dp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a dp_direct_bitmap_bg_pd_register_hbe_ptr which changes the attributes to commit to the fourth GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pd_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **create** `static dp_direct_bitmap_bg_pd_register_hbe_ptr create( dp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L65): Creates a dp_direct_bitmap_bg_pd_register_hbe_ptr which changes the values to commit to the fourth GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the fourth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pd_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<dp_direct_bitmap_bg_pd_register_hbe_ptr> create_optional( dp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a dp_direct_bitmap_bg_pd_register_hbe_ptr which changes the attributes to commit to the fourth GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pd_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<dp_direct_bitmap_bg_pd_register_hbe_ptr> create_optional( dp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L96): Creates a dp_direct_bitmap_bg_pd_register_hbe_ptr which changes the values to commit to the fourth GBA register of a dp_direct_bitmap_bg_ptr.
  `bg` dp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the fourth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pd_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L102): Returns the dp_direct_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the fourth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the fourth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the fourth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the fourth GBA register of a dp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the dp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(dp_direct_bitmap_bg_pd_register_hbe_ptr& other)` (L165): Exchanges the contents of this dp_direct_bitmap_bg_pd_register_hbe_ptr with those of the other one.
  `other` dp_direct_bitmap_bg_pd_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(dp_direct_bitmap_bg_pd_register_hbe_ptr& a, dp_direct_bitmap_bg_pd_register_hbe_ptr& b)` (L172): Exchanges the contents of a dp_direct_bitmap_bg_pd_register_hbe_ptr with those of another one.
  `a` First dp_direct_bitmap_bg_pd_register_hbe_ptr to exchange the contents with.; `b` Second dp_direct_bitmap_bg_pd_register_hbe_ptr to exchange the contents with.

## bn_dp_direct_bitmap_bg_pivot_position_hbe_ptr.h

### `bn::dp_direct_bitmap_bg_pivot_position_hbe_ptr` (class, bn_dp_direct_bitmap_bg_pivot_position_hbe_ptr.h:32)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the position of the affine transformations pivot point of a dual page direct bitmap background in each screen horizontal line.

`class dp_direct_bitmap_bg_pivot_position_hbe_ptr : public hbe_ptr`

- **create_horizontal** `static dp_direct_bitmap_bg_pivot_position_hbe_ptr create_horizontal( dp_direct_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)` (L49): Creates a dp_direct_bitmap_bg_pivot_position_hbe_ptr which changes the horizontal position of the affine transformations pivot point of a dual page direct bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the horizontal position of the affine transformations pivot point of the given background in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pivot_position_hbe_ptr.
  The deltas are not copied but referenced,
  so they should outlive the dp_direct_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **create_vertical** `static dp_direct_bitmap_bg_pivot_position_hbe_ptr create_vertical( dp_direct_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)` (L65): Creates a dp_direct_bitmap_bg_pivot_position_hbe_ptr which changes the vertical position of the affine transformations pivot point of a dual page direct bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the vertical position of the affine transformations pivot point of the given background in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pivot_position_hbe_ptr.
  The deltas are not copied but referenced,
  so they should outlive the dp_direct_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **create_horizontal_optional** `static optional<dp_direct_bitmap_bg_pivot_position_hbe_ptr> create_horizontal_optional( dp_direct_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)` (L82): Creates a dp_direct_bitmap_bg_pivot_position_hbe_ptr which changes the horizontal position of the affine transformations pivot point of a dual page direct bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the horizontal position of the affine transformations pivot point of the given background in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pivot_position_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The deltas are not copied but referenced,
  so they should outlive the dp_direct_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **create_vertical_optional** `static optional<dp_direct_bitmap_bg_pivot_position_hbe_ptr> create_vertical_optional( dp_direct_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)` (L99): Creates a dp_direct_bitmap_bg_pivot_position_hbe_ptr which changes the vertical position of the affine transformations pivot point of a dual page direct bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the vertical position of the affine transformations pivot point of the given background in each screen horizontal line.; devuelve: The requested dp_direct_bitmap_bg_pivot_position_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The deltas are not copied but referenced,
  so they should outlive the dp_direct_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **bg** `const dp_direct_bitmap_bg_ptr& bg() const` (L105): Returns the background modified by this H-Blank effect.
- **deltas_ref** `span<const fixed> deltas_ref() const` (L117): Returns the referenced array of 160 deltas to add to the horizontal or vertical position of the affine transformations pivot point of the managed background in each screen horizontal line.
  The deltas are not copied but referenced,
  so they should outlive the dp_direct_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **set_deltas_ref** `void set_deltas_ref(const span<const fixed>& deltas_ref)` (L126): Sets the reference to an array of 160 deltas to add to the horizontal or vertical position of the affine transformations pivot point of the managed background in each screen horizontal line.
  The deltas are not copied but referenced,
  so they should outlive the dp_direct_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **reload_deltas_ref** `void reload_deltas_ref()` (L135): Rereads the content of the referenced deltas to add to the horizontal or vertical position of the affine transformations pivot point of the managed background in each screen horizontal line.
  The deltas are not copied but referenced,
  so they should outlive the dp_direct_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **swap** `void swap(dp_direct_bitmap_bg_pivot_position_hbe_ptr& other)` (L141): Exchanges the contents of this dp_direct_bitmap_bg_pivot_position_hbe_ptr with those of the other one.
  `other` dp_direct_bitmap_bg_pivot_position_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(dp_direct_bitmap_bg_pivot_position_hbe_ptr& a, dp_direct_bitmap_bg_pivot_position_hbe_ptr& b)` (L148): Exchanges the contents of a dp_direct_bitmap_bg_pivot_position_hbe_ptr with those of another one.
  `a` First dp_direct_bitmap_bg_pivot_position_hbe_ptr to exchange the contents with.; `b` Second dp_direct_bitmap_bg_pivot_position_hbe_ptr to exchange the contents with.

## bn_green_swap_hbe_ptr.h

### `bn::green_swap_hbe_ptr` (class, bn_green_swap_hbe_ptr.h:31)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the state of green swap in each screen horizontal line.

`class green_swap_hbe_ptr : public hbe_ptr`

- **create** `static green_swap_hbe_ptr create(const span<const bool>& states_ref)` (L46): Creates a green_swap_hbe_ptr which changes the state of green swap in each screen horizontal line.
  `states_ref` Reference to an array of 160 bool values which indicate the state of green swap in each screen horizontal line.; devuelve: The requested green_swap_hbe_ptr.
  The states are not copied but referenced, so they should outlive
  the green_swap_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<green_swap_hbe_ptr> create_optional(const span<const bool>& states_ref)` (L59): Creates a green_swap_hbe_ptr which changes the state of green swap in each screen horizontal line.
  `states_ref` Reference to an array of 160 bool values which indicate the state of green swap in each screen horizontal line.; devuelve: The requested green_swap_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The states are not copied but referenced, so they should outlive
  the green_swap_hbe_ptr to avoid dangling references.
- **states_ref** `span<const bool> states_ref() const` (L68): Returns the referenced array of 160 bool values which indicate the state of green swap in each screen horizontal line.
  The states are not copied but referenced, so they should outlive
  the green_swap_hbe_ptr to avoid dangling references.
- **set_states_ref** `void set_states_ref(const span<const bool>& states_ref)` (L77): Sets the reference to an array of 160 bool values which indicate the state of green swap in each screen horizontal line.
  The states are not copied but referenced, so they should outlive
  the green_swap_hbe_ptr to avoid dangling references.
- **reload_states_ref** `void reload_states_ref()` (L85): Rereads the content of the referenced green swap states in each screen horizontal line.
  The states are not copied but referenced, so they should outlive
  the green_swap_hbe_ptr to avoid dangling references.
- **swap** `void swap(green_swap_hbe_ptr& other)` (L91): Exchanges the contents of this green_swap_hbe_ptr with those of the other one.
  `other` green_swap_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(green_swap_hbe_ptr& a, green_swap_hbe_ptr& b)` (L98): Exchanges the contents of a green_swap_hbe_ptr with those of another one.
  `a` First green_swap_hbe_ptr to exchange the contents with.; `b` Second green_swap_hbe_ptr to exchange the contents with.

## bn_hbe_ptr.h

### `bn::hbe_ptr` (class, bn_hbe_ptr.h:31)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect.

`class hbe_ptr`

Several hbe_ptr objects may own the same H-Blank effect.

The H-Blank effect is released when the last remaining hbe_ptr owning it is destroyed.

- **~hbe_ptr** `~hbe_ptr()` (L38): Releases the referenced H-Blank effect if no more hbe_ptr objects reference to it.
- **id** `int id() const` (L43): Returns the internal id.
- **visible** `bool visible() const` (L51): Indicates if this H-Blank effect must be committed to the GBA or not.
- **set_visible** `void set_visible(bool visible)` (L56): Sets if this H-Blank effect must be committed to the GBA or not.
- **operator==** `friend bool operator==(const hbe_ptr& a, const hbe_ptr& b) = default` (L61): Default equal operator.

## bn_hbes.h

### `bn::hbes` (namespace, bn_hbes.h:23)

H-Blank effects related functions.

- **used_count** `int used_count()` (L28): Returns the number of active H-Blank effects.
- **available_count** `int available_count()` (L33): Returns the number of available H-Blank effects that can be created.

## bn_mosaic_attributes_hbe_ptr.h

### `bn::mosaic_attributes_hbe_ptr` (class, bn_mosaic_attributes_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes to commit to the GBA mosaic register in each screen horizontal line.

`class mosaic_attributes_hbe_ptr : public hbe_ptr`

- **create** `static mosaic_attributes_hbe_ptr create( const span<const mosaic_attributes>& attributes_ref)` (L48): Creates a mosaic_attributes_hbe_ptr which changes the attributes to commit to the GBA mosaic register in each screen horizontal line.
  `attributes_ref` Reference to an array of 160 mosaic_attributes objects with the attributes to commit to the GBA mosaic register in each screen horizontal line.; devuelve: The requested mosaic_attributes_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the mosaic_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<mosaic_attributes_hbe_ptr> create_optional( const span<const mosaic_attributes>& attributes_ref)` (L62): Creates a mosaic_attributes_hbe_ptr which changes the attributes to commit to the GBA mosaic register in each screen horizontal line.
  `attributes_ref` Reference to an array of 160 mosaic_attributes objects with the attributes to commit to the GBA mosaic register in each screen horizontal line.; devuelve: The requested mosaic_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the mosaic_attributes_hbe_ptr to avoid dangling references.
- **attributes_ref** `span<const mosaic_attributes> attributes_ref() const` (L72): Returns the referenced array of 160 mosaic_attributes objects with the attributes to commit to the GBA mosaic register in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the mosaic_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const mosaic_attributes>& attributes_ref)` (L81): Sets the reference to an array of 160 mosaic_attributes objects with the attributes to commit to the GBA mosaic register in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the mosaic_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L90): Rereads the content of the referenced attributes to the GBA mosaic register in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the mosaic_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(mosaic_attributes_hbe_ptr& other)` (L96): Exchanges the contents of this mosaic_attributes_hbe_ptr with those of the other one.
  `other` mosaic_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(mosaic_attributes_hbe_ptr& a, mosaic_attributes_hbe_ptr& b)` (L106): Exchanges the contents of a mosaic_attributes_hbe_ptr with those of another one.
  `a` First mosaic_attributes_hbe_ptr to exchange the contents with.; `b` Second mosaic_attributes_hbe_ptr to exchange the contents with.

## bn_palette_bitmap_bg_attributes_hbe_ptr.h

### `bn::palette_bitmap_bg_attributes_hbe_ptr` (class, bn_palette_bitmap_bg_attributes_hbe_ptr.h:32)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes to commit to the GBA register of a palette bitmap background in each screen horizontal line.

`class palette_bitmap_bg_attributes_hbe_ptr : public hbe_ptr`

- **create** `static palette_bitmap_bg_attributes_hbe_ptr create( palette_bitmap_bg_ptr bg, const span<const bitmap_bg_attributes>& attributes_ref)` (L48): Creates a palette_bitmap_bg_attributes_hbe_ptr which changes the attributes to commit to the GBA register of a palette bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `attributes_ref` Reference to an array of 160 bitmap_bg_attributes objects with the attributes to commit to the GBA register of the given background in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_attributes_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<palette_bitmap_bg_attributes_hbe_ptr> create_optional( palette_bitmap_bg_ptr bg, const span<const bitmap_bg_attributes>& attributes_ref)` (L63): Creates a palette_bitmap_bg_attributes_hbe_ptr which changes the attributes to commit to the GBA register of a palette bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `attributes_ref` Reference to an array of 160 bitmap_bg_attributes objects with the attributes to commit to the GBA register of the given background in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L69): Returns the affine background modified by this H-Blank effect.
- **attributes_ref** `span<const bitmap_bg_attributes> attributes_ref() const` (L81): Returns the referenced array of 160 bitmap_bg_attributes objects with the attributes to commit to the GBA register of the managed background in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const bitmap_bg_attributes>& attributes_ref)` (L90): Sets the reference to an array of 160 bitmap_bg_attributes objects with the attributes to commit to the GBA register of the managed background in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L99): Rereads the content of the referenced attributes to the GBA register of the managed background in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(palette_bitmap_bg_attributes_hbe_ptr& other)` (L105): Exchanges the contents of this palette_bitmap_bg_attributes_hbe_ptr with those of the other one.
  `other` palette_bitmap_bg_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(palette_bitmap_bg_attributes_hbe_ptr& a, palette_bitmap_bg_attributes_hbe_ptr& b)` (L112): Exchanges the contents of a palette_bitmap_bg_attributes_hbe_ptr with those of another one.
  `a` First palette_bitmap_bg_attributes_hbe_ptr to exchange the contents with.; `b` Second palette_bitmap_bg_attributes_hbe_ptr to exchange the contents with.

## bn_palette_bitmap_bg_dx_register_hbe_ptr.h

### `bn::palette_bitmap_bg_dx_register_hbe_ptr` (class, bn_palette_bitmap_bg_dx_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the the fifth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.

`class palette_bitmap_bg_dx_register_hbe_ptr : public hbe_ptr`

- **create** `static palette_bitmap_bg_dx_register_hbe_ptr create( palette_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a palette_bitmap_bg_dx_register_hbe_ptr which changes the attributes to commit to the the fifth GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_dx_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **create** `static palette_bitmap_bg_dx_register_hbe_ptr create( palette_bitmap_bg_ptr bg, const span<const int>& values_ref)` (L65): Creates a palette_bitmap_bg_dx_register_hbe_ptr which changes the values to commit to the the fifth GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the fifth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_dx_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<palette_bitmap_bg_dx_register_hbe_ptr> create_optional( palette_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a palette_bitmap_bg_dx_register_hbe_ptr which changes the attributes to commit to the the fifth GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_dx_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<palette_bitmap_bg_dx_register_hbe_ptr> create_optional( palette_bitmap_bg_ptr bg, const span<const int>& values_ref)` (L96): Creates a palette_bitmap_bg_dx_register_hbe_ptr which changes the values to commit to the the fifth GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the fifth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_dx_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L102): Returns the palette_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the the fifth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the the fifth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the the fifth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the the fifth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(palette_bitmap_bg_dx_register_hbe_ptr& other)` (L165): Exchanges the contents of this palette_bitmap_bg_dx_register_hbe_ptr with those of the other one.
  `other` palette_bitmap_bg_dx_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(palette_bitmap_bg_dx_register_hbe_ptr& a, palette_bitmap_bg_dx_register_hbe_ptr& b)` (L172): Exchanges the contents of a palette_bitmap_bg_dx_register_hbe_ptr with those of another one.
  `a` First palette_bitmap_bg_dx_register_hbe_ptr to exchange the contents with.; `b` Second palette_bitmap_bg_dx_register_hbe_ptr to exchange the contents with.

## bn_palette_bitmap_bg_dy_register_hbe_ptr.h

### `bn::palette_bitmap_bg_dy_register_hbe_ptr` (class, bn_palette_bitmap_bg_dy_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the the sixth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.

`class palette_bitmap_bg_dy_register_hbe_ptr : public hbe_ptr`

- **create** `static palette_bitmap_bg_dy_register_hbe_ptr create( palette_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a palette_bitmap_bg_dy_register_hbe_ptr which changes the attributes to commit to the the sixth GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_dy_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **create** `static palette_bitmap_bg_dy_register_hbe_ptr create( palette_bitmap_bg_ptr bg, const span<const int>& values_ref)` (L65): Creates a palette_bitmap_bg_dy_register_hbe_ptr which changes the values to commit to the the sixth GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the sixth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_dy_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<palette_bitmap_bg_dy_register_hbe_ptr> create_optional( palette_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a palette_bitmap_bg_dy_register_hbe_ptr which changes the attributes to commit to the the sixth GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_dy_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<palette_bitmap_bg_dy_register_hbe_ptr> create_optional( palette_bitmap_bg_ptr bg, const span<const int>& values_ref)` (L96): Creates a palette_bitmap_bg_dy_register_hbe_ptr which changes the values to commit to the the sixth GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the sixth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_dy_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L102): Returns the palette_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the the sixth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the the sixth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the the sixth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the the sixth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(palette_bitmap_bg_dy_register_hbe_ptr& other)` (L165): Exchanges the contents of this palette_bitmap_bg_dy_register_hbe_ptr with those of the other one.
  `other` palette_bitmap_bg_dy_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(palette_bitmap_bg_dy_register_hbe_ptr& a, palette_bitmap_bg_dy_register_hbe_ptr& b)` (L172): Exchanges the contents of a palette_bitmap_bg_dy_register_hbe_ptr with those of another one.
  `a` First palette_bitmap_bg_dy_register_hbe_ptr to exchange the contents with.; `b` Second palette_bitmap_bg_dy_register_hbe_ptr to exchange the contents with.

## bn_palette_bitmap_bg_mat_attributes_hbe_ptr.h

### `bn::palette_bitmap_bg_mat_attributes_hbe_ptr` (class, bn_palette_bitmap_bg_mat_attributes_hbe_ptr.h:36)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes which define the position and the transformation matrix of a palette_bitmap_bg_ptr in each screen horizontal line.

`class palette_bitmap_bg_mat_attributes_hbe_ptr`

- **create** `static palette_bitmap_bg_mat_attributes_hbe_ptr create( const palette_bitmap_bg_ptr& bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L53): Creates a palette_bitmap_bg_mat_attributes_hbe_ptr which changes the attributes which define the position and the transformation matrix of a palette_bitmap_bg_ptr in each screen horizontal line.
  `bg` palette_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to set to the given palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_mat_attributes_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<palette_bitmap_bg_mat_attributes_hbe_ptr> create_optional( const palette_bitmap_bg_ptr& bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L69): Creates a palette_bitmap_bg_mat_attributes_hbe_ptr which changes the attributes which define the position and the transformation matrix of a palette_bitmap_bg_ptr in each screen horizontal line.
  `bg` palette_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to set to the given palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_mat_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **id** `int id() const` (L75): Returns the internal id.
- **visible** `bool visible() const` (L83): Indicates if this H-Blank effect must be committed to the GBA or not.
- **set_visible** `void set_visible(bool visible)` (L91): Sets if this H-Blank effect must be committed to the GBA or not.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L96): Returns the palette_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L108): Returns the referenced array of 160 affine_bg_mat_attributes objects to set to the given palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L117): Sets the reference to an array of 160 affine_bg_mat_attributes objects to set to the given palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L126): Rereads the content of the referenced attributes to set to the given palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(palette_bitmap_bg_mat_attributes_hbe_ptr& other)` (L132): Exchanges the contents of this palette_bitmap_bg_mat_attributes_hbe_ptr with those of the other one.
  `other` palette_bitmap_bg_mat_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(palette_bitmap_bg_mat_attributes_hbe_ptr& a, palette_bitmap_bg_mat_attributes_hbe_ptr& b)` (L139): Exchanges the contents of a palette_bitmap_bg_mat_attributes_hbe_ptr with those of another one.
  `a` First palette_bitmap_bg_mat_attributes_hbe_ptr to exchange the contents with.; `b` Second palette_bitmap_bg_mat_attributes_hbe_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const palette_bitmap_bg_mat_attributes_hbe_ptr& a, const palette_bitmap_bg_mat_attributes_hbe_ptr& b)` (L150): Equal operator.
  `a` First palette_bitmap_bg_mat_attributes_hbe_ptr to compare.; `b` Second palette_bitmap_bg_mat_attributes_hbe_ptr to compare.; devuelve: `true` if the first palette_bitmap_bg_mat_attributes_hbe_ptr is equal to the second one, otherwise `false`.
- **operator!=** `friend bool operator!=(const palette_bitmap_bg_mat_attributes_hbe_ptr& a, const palette_bitmap_bg_mat_attributes_hbe_ptr& b)` (L162): Not equal operator.
  `a` First palette_bitmap_bg_mat_attributes_hbe_ptr to compare.; `b` Second palette_bitmap_bg_mat_attributes_hbe_ptr to compare.; devuelve: `true` if the first palette_bitmap_bg_mat_attributes_hbe_ptr is not equal to the second one, otherwise `false`.

## bn_palette_bitmap_bg_pa_register_hbe_ptr.h

### `bn::palette_bitmap_bg_pa_register_hbe_ptr` (class, bn_palette_bitmap_bg_pa_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.

`class palette_bitmap_bg_pa_register_hbe_ptr : public hbe_ptr`

- **create** `static palette_bitmap_bg_pa_register_hbe_ptr create( palette_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a palette_bitmap_bg_pa_register_hbe_ptr which changes the attributes to commit to the first GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pa_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **create** `static palette_bitmap_bg_pa_register_hbe_ptr create( palette_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L65): Creates a palette_bitmap_bg_pa_register_hbe_ptr which changes the values to commit to the first GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pa_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<palette_bitmap_bg_pa_register_hbe_ptr> create_optional( palette_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a palette_bitmap_bg_pa_register_hbe_ptr which changes the attributes to commit to the first GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pa_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<palette_bitmap_bg_pa_register_hbe_ptr> create_optional( palette_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L96): Creates a palette_bitmap_bg_pa_register_hbe_ptr which changes the values to commit to the first GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pa_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L102): Returns the palette_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the first GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(palette_bitmap_bg_pa_register_hbe_ptr& other)` (L165): Exchanges the contents of this palette_bitmap_bg_pa_register_hbe_ptr with those of the other one.
  `other` palette_bitmap_bg_pa_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(palette_bitmap_bg_pa_register_hbe_ptr& a, palette_bitmap_bg_pa_register_hbe_ptr& b)` (L172): Exchanges the contents of a palette_bitmap_bg_pa_register_hbe_ptr with those of another one.
  `a` First palette_bitmap_bg_pa_register_hbe_ptr to exchange the contents with.; `b` Second palette_bitmap_bg_pa_register_hbe_ptr to exchange the contents with.

## bn_palette_bitmap_bg_pb_register_hbe_ptr.h

### `bn::palette_bitmap_bg_pb_register_hbe_ptr` (class, bn_palette_bitmap_bg_pb_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the second GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.

`class palette_bitmap_bg_pb_register_hbe_ptr : public hbe_ptr`

- **create** `static palette_bitmap_bg_pb_register_hbe_ptr create( palette_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a palette_bitmap_bg_pb_register_hbe_ptr which changes the attributes to commit to the second GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pb_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **create** `static palette_bitmap_bg_pb_register_hbe_ptr create( palette_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L65): Creates a palette_bitmap_bg_pb_register_hbe_ptr which changes the values to commit to the second GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the second GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pb_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<palette_bitmap_bg_pb_register_hbe_ptr> create_optional( palette_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a palette_bitmap_bg_pb_register_hbe_ptr which changes the attributes to commit to the second GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pb_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<palette_bitmap_bg_pb_register_hbe_ptr> create_optional( palette_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L96): Creates a palette_bitmap_bg_pb_register_hbe_ptr which changes the values to commit to the second GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the second GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pb_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L102): Returns the palette_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the second GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the second GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the second GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the second GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(palette_bitmap_bg_pb_register_hbe_ptr& other)` (L165): Exchanges the contents of this palette_bitmap_bg_pb_register_hbe_ptr with those of the other one.
  `other` palette_bitmap_bg_pb_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(palette_bitmap_bg_pb_register_hbe_ptr& a, palette_bitmap_bg_pb_register_hbe_ptr& b)` (L172): Exchanges the contents of a palette_bitmap_bg_pb_register_hbe_ptr with those of another one.
  `a` First palette_bitmap_bg_pb_register_hbe_ptr to exchange the contents with.; `b` Second palette_bitmap_bg_pb_register_hbe_ptr to exchange the contents with.

## bn_palette_bitmap_bg_pc_register_hbe_ptr.h

### `bn::palette_bitmap_bg_pc_register_hbe_ptr` (class, bn_palette_bitmap_bg_pc_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the third GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.

`class palette_bitmap_bg_pc_register_hbe_ptr : public hbe_ptr`

- **create** `static palette_bitmap_bg_pc_register_hbe_ptr create( palette_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a palette_bitmap_bg_pc_register_hbe_ptr which changes the attributes to commit to the third GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pc_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **create** `static palette_bitmap_bg_pc_register_hbe_ptr create( palette_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L65): Creates a palette_bitmap_bg_pc_register_hbe_ptr which changes the values to commit to the third GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the third GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pc_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<palette_bitmap_bg_pc_register_hbe_ptr> create_optional( palette_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a palette_bitmap_bg_pc_register_hbe_ptr which changes the attributes to commit to the third GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pc_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<palette_bitmap_bg_pc_register_hbe_ptr> create_optional( palette_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L96): Creates a palette_bitmap_bg_pc_register_hbe_ptr which changes the values to commit to the third GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the third GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pc_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L102): Returns the palette_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the third GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the third GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the third GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the third GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(palette_bitmap_bg_pc_register_hbe_ptr& other)` (L165): Exchanges the contents of this palette_bitmap_bg_pc_register_hbe_ptr with those of the other one.
  `other` palette_bitmap_bg_pc_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(palette_bitmap_bg_pc_register_hbe_ptr& a, palette_bitmap_bg_pc_register_hbe_ptr& b)` (L172): Exchanges the contents of a palette_bitmap_bg_pc_register_hbe_ptr with those of another one.
  `a` First palette_bitmap_bg_pc_register_hbe_ptr to exchange the contents with.; `b` Second palette_bitmap_bg_pc_register_hbe_ptr to exchange the contents with.

## bn_palette_bitmap_bg_pd_register_hbe_ptr.h

### `bn::palette_bitmap_bg_pd_register_hbe_ptr` (class, bn_palette_bitmap_bg_pd_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the fourth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.

`class palette_bitmap_bg_pd_register_hbe_ptr : public hbe_ptr`

- **create** `static palette_bitmap_bg_pd_register_hbe_ptr create( palette_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a palette_bitmap_bg_pd_register_hbe_ptr which changes the attributes to commit to the fourth GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pd_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **create** `static palette_bitmap_bg_pd_register_hbe_ptr create( palette_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L65): Creates a palette_bitmap_bg_pd_register_hbe_ptr which changes the values to commit to the fourth GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the fourth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pd_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<palette_bitmap_bg_pd_register_hbe_ptr> create_optional( palette_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a palette_bitmap_bg_pd_register_hbe_ptr which changes the attributes to commit to the fourth GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pd_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<palette_bitmap_bg_pd_register_hbe_ptr> create_optional( palette_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L96): Creates a palette_bitmap_bg_pd_register_hbe_ptr which changes the values to commit to the fourth GBA register of a palette_bitmap_bg_ptr.
  `bg` palette_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the fourth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pd_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L102): Returns the palette_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the fourth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the fourth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the fourth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the fourth GBA register of a palette_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the palette_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(palette_bitmap_bg_pd_register_hbe_ptr& other)` (L165): Exchanges the contents of this palette_bitmap_bg_pd_register_hbe_ptr with those of the other one.
  `other` palette_bitmap_bg_pd_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(palette_bitmap_bg_pd_register_hbe_ptr& a, palette_bitmap_bg_pd_register_hbe_ptr& b)` (L172): Exchanges the contents of a palette_bitmap_bg_pd_register_hbe_ptr with those of another one.
  `a` First palette_bitmap_bg_pd_register_hbe_ptr to exchange the contents with.; `b` Second palette_bitmap_bg_pd_register_hbe_ptr to exchange the contents with.

## bn_palette_bitmap_bg_pivot_position_hbe_ptr.h

### `bn::palette_bitmap_bg_pivot_position_hbe_ptr` (class, bn_palette_bitmap_bg_pivot_position_hbe_ptr.h:32)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the position of the affine transformations pivot point of a palette bitmap background in each screen horizontal line.

`class palette_bitmap_bg_pivot_position_hbe_ptr : public hbe_ptr`

- **create_horizontal** `static palette_bitmap_bg_pivot_position_hbe_ptr create_horizontal( palette_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)` (L49): Creates a palette_bitmap_bg_pivot_position_hbe_ptr which changes the horizontal position of the affine transformations pivot point of a palette bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the horizontal position of the affine transformations pivot point of the given background in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pivot_position_hbe_ptr.
  The deltas are not copied but referenced,
  so they should outlive the palette_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **create_vertical** `static palette_bitmap_bg_pivot_position_hbe_ptr create_vertical( palette_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)` (L65): Creates a palette_bitmap_bg_pivot_position_hbe_ptr which changes the vertical position of the affine transformations pivot point of a palette bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the vertical position of the affine transformations pivot point of the given background in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pivot_position_hbe_ptr.
  The deltas are not copied but referenced,
  so they should outlive the palette_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **create_horizontal_optional** `static optional<palette_bitmap_bg_pivot_position_hbe_ptr> create_horizontal_optional( palette_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)` (L81): Creates a palette_bitmap_bg_pivot_position_hbe_ptr which changes the horizontal position of the affine transformations pivot point of a palette bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the horizontal position of the affine transformations pivot point of the given background in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pivot_position_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The deltas are not copied but referenced,
  so they should outlive the palette_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **create_vertical_optional** `static optional<palette_bitmap_bg_pivot_position_hbe_ptr> create_vertical_optional( palette_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)` (L97): Creates a palette_bitmap_bg_pivot_position_hbe_ptr which changes the vertical position of the affine transformations pivot point of a palette bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the vertical position of the affine transformations pivot point of the given background in each screen horizontal line.; devuelve: The requested palette_bitmap_bg_pivot_position_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The deltas are not copied but referenced,
  so they should outlive the palette_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **bg** `const palette_bitmap_bg_ptr& bg() const` (L103): Returns the background modified by this H-Blank effect.
- **deltas_ref** `span<const fixed> deltas_ref() const` (L115): Returns the referenced array of 160 deltas to add to the horizontal or vertical position of the affine transformations pivot point of the managed background in each screen horizontal line.
  The deltas are not copied but referenced,
  so they should outlive the palette_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **set_deltas_ref** `void set_deltas_ref(const span<const fixed>& deltas_ref)` (L124): Sets the reference to an array of 160 deltas to add to the horizontal or vertical position of the affine transformations pivot point of the managed background in each screen horizontal line.
  The deltas are not copied but referenced,
  so they should outlive the palette_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **reload_deltas_ref** `void reload_deltas_ref()` (L133): Rereads the content of the referenced deltas to add to the horizontal or vertical position of the affine transformations pivot point of the managed background in each screen horizontal line.
  The deltas are not copied but referenced,
  so they should outlive the palette_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **swap** `void swap(palette_bitmap_bg_pivot_position_hbe_ptr& other)` (L139): Exchanges the contents of this palette_bitmap_bg_pivot_position_hbe_ptr with those of the other one.
  `other` palette_bitmap_bg_pivot_position_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(palette_bitmap_bg_pivot_position_hbe_ptr& a, palette_bitmap_bg_pivot_position_hbe_ptr& b)` (L146): Exchanges the contents of a palette_bitmap_bg_pivot_position_hbe_ptr with those of another one.
  `a` First palette_bitmap_bg_pivot_position_hbe_ptr to exchange the contents with.; `b` Second palette_bitmap_bg_pivot_position_hbe_ptr to exchange the contents with.

## bn_rect_window_boundaries_hbe_ptr.h

### `bn::rect_window_boundaries_hbe_ptr` (class, bn_rect_window_boundaries_hbe_ptr.h:31)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the boundaries of a rect window in each screen horizontal line.

`class rect_window_boundaries_hbe_ptr : public hbe_ptr`

- **create_horizontal** `static rect_window_boundaries_hbe_ptr create_horizontal( rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)` (L47): Creates a rect_window_boundaries_hbe_ptr which changes the horizontal boundaries of a rect window in each screen horizontal line.
  `window` Rect window to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the horizontal boundaries of the given rect window in each screen horizontal line.; devuelve: The requested rect_window_boundaries_hbe_ptr.
  The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hbe_ptr
  to avoid dangling references.
- **create_vertical** `static rect_window_boundaries_hbe_ptr create_vertical( rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)` (L62): Creates a rect_window_boundaries_hbe_ptr which changes the vertical boundaries of a rect window in each screen vertical line.
  `window` Rect window to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the vertical boundaries of the given rect window in each screen vertical line.; devuelve: The requested rect_window_boundaries_hbe_ptr.
  The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hbe_ptr
  to avoid dangling references.
- **create_horizontal_optional** `static optional<rect_window_boundaries_hbe_ptr> create_horizontal_optional( rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)` (L77): Creates a rect_window_boundaries_hbe_ptr which changes the horizontal boundaries of a rect window in each screen horizontal line.
  `window` Rect window to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the horizontal boundaries of the given rect window in each screen horizontal line.; devuelve: The requested rect_window_boundaries_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hbe_ptr
  to avoid dangling references.
- **create_vertical_optional** `static optional<rect_window_boundaries_hbe_ptr> create_vertical_optional( rect_window window, const span<const pair<fixed, fixed>>& deltas_ref)` (L92): Creates a rect_window_boundaries_hbe_ptr which changes the vertical boundaries of a rect window in each screen vertical line.
  `window` Rect window to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the vertical boundaries of the given rect window in each screen vertical line.; devuelve: The requested rect_window_boundaries_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hbe_ptr
  to avoid dangling references.
- **window** `const rect_window& window() const` (L98): Returns the rect window modified by this H-Blank effect.
- **deltas_ref** `span<const pair<fixed, fixed>> deltas_ref() const` (L110): Returns the referenced array of 160 deltas to add to the horizontal or vertical boundaries of the managed rect window in each screen horizontal line.
  The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hbe_ptr
  to avoid dangling references.
- **set_deltas_ref** `void set_deltas_ref(const span<const pair<fixed, fixed>>& deltas_ref)` (L119): Sets the reference to an array of 160 deltas to add to the horizontal or vertical boundaries of the managed rect window in each screen horizontal line.
  The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hbe_ptr
  to avoid dangling references.
- **reload_deltas_ref** `void reload_deltas_ref()` (L128): Rereads the content of the referenced deltas to add to the horizontal or vertical boundaries of the managed rect window in each screen horizontal line.
  The deltas are not copied but referenced, so they should outlive the rect_window_boundaries_hbe_ptr
  to avoid dangling references.
- **swap** `void swap(rect_window_boundaries_hbe_ptr& other)` (L134): Exchanges the contents of this rect_window_boundaries_hbe_ptr with those of the other one.
  `other` rect_window_boundaries_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(rect_window_boundaries_hbe_ptr& a, rect_window_boundaries_hbe_ptr& b)` (L141): Exchanges the contents of a rect_window_boundaries_hbe_ptr with those of another one.
  `a` First rect_window_boundaries_hbe_ptr to exchange the contents with.; `b` Second rect_window_boundaries_hbe_ptr to exchange the contents with.

## bn_regular_bg_attributes_hbe_ptr.h

### `bn::regular_bg_attributes_hbe_ptr` (class, bn_regular_bg_attributes_hbe_ptr.h:32)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes to commit to the GBA register of a regular background in each screen horizontal line.

`class regular_bg_attributes_hbe_ptr : public hbe_ptr`

- **create** `static regular_bg_attributes_hbe_ptr create( regular_bg_ptr bg, const span<const regular_bg_attributes>& attributes_ref)` (L49): Creates a regular_bg_attributes_hbe_ptr which changes the attributes to commit to the GBA register of a regular background in each screen horizontal line.
  `bg` Regular background to be modified.; `attributes_ref` Reference to an array of 160 regular_bg_attributes objects with the attributes to commit to the GBA register of the given regular background in each screen horizontal line.; devuelve: The requested regular_bg_attributes_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the regular_bg_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<regular_bg_attributes_hbe_ptr> create_optional( regular_bg_ptr bg, const span<const regular_bg_attributes>& attributes_ref)` (L65): Creates a regular_bg_attributes_hbe_ptr which changes the attributes to commit to the GBA register of a regular background in each screen horizontal line.
  `bg` Regular background to be modified.; `attributes_ref` Reference to an array of 160 regular_bg_attributes objects with the attributes to commit to the GBA register of the given regular background in each screen horizontal line.; devuelve: The requested regular_bg_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the regular_bg_attributes_hbe_ptr to avoid dangling references.
- **bg** `const regular_bg_ptr& bg() const` (L71): Returns the regular background modified by this H-Blank effect.
- **attributes_ref** `span<const regular_bg_attributes> attributes_ref() const` (L84): Returns the referenced array of 160 regular_bg_attributes objects with the attributes to commit to the GBA register of the managed regular background in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the regular_bg_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const regular_bg_attributes>& attributes_ref)` (L94): Sets the reference to an array of 160 regular_bg_attributes objects with the attributes to commit to the GBA register of the managed regular background in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the regular_bg_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L103): Rereads the content of the referenced attributes to the GBA register of the managed regular background in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the regular_bg_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(regular_bg_attributes_hbe_ptr& other)` (L109): Exchanges the contents of this regular_bg_attributes_hbe_ptr with those of the other one.
  `other` regular_bg_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(regular_bg_attributes_hbe_ptr& a, regular_bg_attributes_hbe_ptr& b)` (L116): Exchanges the contents of a regular_bg_attributes_hbe_ptr with those of another one.
  `a` First regular_bg_attributes_hbe_ptr to exchange the contents with.; `b` Second regular_bg_attributes_hbe_ptr to exchange the contents with.

## bn_regular_bg_position_hbe_ptr.h

### `bn::regular_bg_position_hbe_ptr` (class, bn_regular_bg_position_hbe_ptr.h:31)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the position of a regular background in each screen horizontal line.

`class regular_bg_position_hbe_ptr : public hbe_ptr`

- **create_horizontal** `static regular_bg_position_hbe_ptr create_horizontal( regular_bg_ptr bg, const span<const fixed>& deltas_ref)` (L47): Creates a regular_bg_position_hbe_ptr which changes the horizontal position of a regular background in each screen horizontal line.
  `bg` Regular background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the horizontal position of the given regular background in each screen horizontal line.; devuelve: The requested regular_bg_position_hbe_ptr.
  The deltas are not copied but referenced, so they should outlive the regular_bg_position_hbe_ptr
  to avoid dangling references.
- **create_vertical** `static regular_bg_position_hbe_ptr create_vertical( regular_bg_ptr bg, const span<const fixed>& deltas_ref)` (L62): Creates a regular_bg_position_hbe_ptr which changes the vertical position of a regular background in each screen horizontal line.
  `bg` Regular background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the vertical position of the given regular background in each screen horizontal line.; devuelve: The requested regular_bg_position_hbe_ptr.
  The deltas are not copied but referenced, so they should outlive the regular_bg_position_hbe_ptr
  to avoid dangling references.
- **create_horizontal_optional** `static optional<regular_bg_position_hbe_ptr> create_horizontal_optional( regular_bg_ptr bg, const span<const fixed>& deltas_ref)` (L77): Creates a regular_bg_position_hbe_ptr which changes the horizontal position of a regular background in each screen horizontal line.
  `bg` Regular background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the horizontal position of the given regular background in each screen horizontal line.; devuelve: The requested regular_bg_position_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The deltas are not copied but referenced, so they should outlive the regular_bg_position_hbe_ptr
  to avoid dangling references.
- **create_vertical_optional** `static optional<regular_bg_position_hbe_ptr> create_vertical_optional( regular_bg_ptr bg, const span<const fixed>& deltas_ref)` (L92): Creates a regular_bg_position_hbe_ptr which changes the vertical position of a regular background in each screen horizontal line.
  `bg` Regular background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the vertical position of the given regular background in each screen horizontal line.; devuelve: The requested regular_bg_position_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The deltas are not copied but referenced, so they should outlive the regular_bg_position_hbe_ptr
  to avoid dangling references.
- **bg** `const regular_bg_ptr& bg() const` (L98): Returns the regular background modified by this H-Blank effect.
- **deltas_ref** `span<const fixed> deltas_ref() const` (L110): Returns the referenced array of 160 deltas to add to the horizontal or vertical position of the managed regular background in each screen horizontal line.
  The deltas are not copied but referenced, so they should outlive the regular_bg_position_hbe_ptr
  to avoid dangling references.
- **set_deltas_ref** `void set_deltas_ref(const span<const fixed>& deltas_ref)` (L119): Sets the reference to an array of 160 deltas to add to the horizontal or vertical position of the managed regular background in each screen horizontal line.
  The deltas are not copied but referenced, so they should outlive the regular_bg_position_hbe_ptr
  to avoid dangling references.
- **reload_deltas_ref** `void reload_deltas_ref()` (L128): Rereads the content of the referenced deltas to add to the horizontal or vertical position of the managed regular background in each screen horizontal line.
  The deltas are not copied but referenced, so they should outlive the regular_bg_position_hbe_ptr
  to avoid dangling references.
- **swap** `void swap(regular_bg_position_hbe_ptr& other)` (L134): Exchanges the contents of this regular_bg_position_hbe_ptr with those of the other one.
  `other` regular_bg_position_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(regular_bg_position_hbe_ptr& a, regular_bg_position_hbe_ptr& b)` (L141): Exchanges the contents of a regular_bg_position_hbe_ptr with those of another one.
  `a` First regular_bg_position_hbe_ptr to exchange the contents with.; `b` Second regular_bg_position_hbe_ptr to exchange the contents with.

## bn_sp_direct_bitmap_bg_attributes_hbe_ptr.h

### `bn::sp_direct_bitmap_bg_attributes_hbe_ptr` (class, bn_sp_direct_bitmap_bg_attributes_hbe_ptr.h:32)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes to commit to the GBA register of a single page direct bitmap background in each screen horizontal line.

`class sp_direct_bitmap_bg_attributes_hbe_ptr : public hbe_ptr`

- **create** `static sp_direct_bitmap_bg_attributes_hbe_ptr create( sp_direct_bitmap_bg_ptr bg, const span<const bitmap_bg_attributes>& attributes_ref)` (L48): Creates a sp_direct_bitmap_bg_attributes_hbe_ptr which changes the attributes to commit to the GBA register of a single page direct bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `attributes_ref` Reference to an array of 160 bitmap_bg_attributes objects with the attributes to commit to the GBA register of the given background in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_attributes_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sp_direct_bitmap_bg_attributes_hbe_ptr> create_optional( sp_direct_bitmap_bg_ptr bg, const span<const bitmap_bg_attributes>& attributes_ref)` (L63): Creates a sp_direct_bitmap_bg_attributes_hbe_ptr which changes the attributes to commit to the GBA register of a single page direct bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `attributes_ref` Reference to an array of 160 bitmap_bg_attributes objects with the attributes to commit to the GBA register of the given background in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L69): Returns the affine background modified by this H-Blank effect.
- **attributes_ref** `span<const bitmap_bg_attributes> attributes_ref() const` (L81): Returns the referenced array of 160 bitmap_bg_attributes objects with the attributes to commit to the GBA register of the managed background in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const bitmap_bg_attributes>& attributes_ref)` (L90): Sets the reference to an array of 160 bitmap_bg_attributes objects with the attributes to commit to the GBA register of the managed background in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L99): Rereads the content of the referenced attributes to the GBA register of the managed background in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(sp_direct_bitmap_bg_attributes_hbe_ptr& other)` (L105): Exchanges the contents of this sp_direct_bitmap_bg_attributes_hbe_ptr with those of the other one.
  `other` sp_direct_bitmap_bg_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sp_direct_bitmap_bg_attributes_hbe_ptr& a, sp_direct_bitmap_bg_attributes_hbe_ptr& b)` (L112): Exchanges the contents of a sp_direct_bitmap_bg_attributes_hbe_ptr with those of another one.
  `a` First sp_direct_bitmap_bg_attributes_hbe_ptr to exchange the contents with.; `b` Second sp_direct_bitmap_bg_attributes_hbe_ptr to exchange the contents with.

## bn_sp_direct_bitmap_bg_dx_register_hbe_ptr.h

### `bn::sp_direct_bitmap_bg_dx_register_hbe_ptr` (class, bn_sp_direct_bitmap_bg_dx_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the the fifth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.

`class sp_direct_bitmap_bg_dx_register_hbe_ptr : public hbe_ptr`

- **create** `static sp_direct_bitmap_bg_dx_register_hbe_ptr create( sp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a sp_direct_bitmap_bg_dx_register_hbe_ptr which changes the attributes to commit to the the fifth GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_dx_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **create** `static sp_direct_bitmap_bg_dx_register_hbe_ptr create( sp_direct_bitmap_bg_ptr bg, const span<const int>& values_ref)` (L65): Creates a sp_direct_bitmap_bg_dx_register_hbe_ptr which changes the values to commit to the the fifth GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the fifth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_dx_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sp_direct_bitmap_bg_dx_register_hbe_ptr> create_optional( sp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a sp_direct_bitmap_bg_dx_register_hbe_ptr which changes the attributes to commit to the the fifth GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_dx_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sp_direct_bitmap_bg_dx_register_hbe_ptr> create_optional( sp_direct_bitmap_bg_ptr bg, const span<const int>& values_ref)` (L96): Creates a sp_direct_bitmap_bg_dx_register_hbe_ptr which changes the values to commit to the the fifth GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the fifth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_dx_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L102): Returns the sp_direct_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the the fifth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the fifth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the the fifth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the the fifth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the the fifth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dx_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(sp_direct_bitmap_bg_dx_register_hbe_ptr& other)` (L165): Exchanges the contents of this sp_direct_bitmap_bg_dx_register_hbe_ptr with those of the other one.
  `other` sp_direct_bitmap_bg_dx_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sp_direct_bitmap_bg_dx_register_hbe_ptr& a, sp_direct_bitmap_bg_dx_register_hbe_ptr& b)` (L172): Exchanges the contents of a sp_direct_bitmap_bg_dx_register_hbe_ptr with those of another one.
  `a` First sp_direct_bitmap_bg_dx_register_hbe_ptr to exchange the contents with.; `b` Second sp_direct_bitmap_bg_dx_register_hbe_ptr to exchange the contents with.

## bn_sp_direct_bitmap_bg_dy_register_hbe_ptr.h

### `bn::sp_direct_bitmap_bg_dy_register_hbe_ptr` (class, bn_sp_direct_bitmap_bg_dy_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the the sixth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.

`class sp_direct_bitmap_bg_dy_register_hbe_ptr : public hbe_ptr`

- **create** `static sp_direct_bitmap_bg_dy_register_hbe_ptr create( sp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a sp_direct_bitmap_bg_dy_register_hbe_ptr which changes the attributes to commit to the the sixth GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_dy_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **create** `static sp_direct_bitmap_bg_dy_register_hbe_ptr create( sp_direct_bitmap_bg_ptr bg, const span<const int>& values_ref)` (L65): Creates a sp_direct_bitmap_bg_dy_register_hbe_ptr which changes the values to commit to the the sixth GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the sixth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_dy_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sp_direct_bitmap_bg_dy_register_hbe_ptr> create_optional( sp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a sp_direct_bitmap_bg_dy_register_hbe_ptr which changes the attributes to commit to the the sixth GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_dy_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sp_direct_bitmap_bg_dy_register_hbe_ptr> create_optional( sp_direct_bitmap_bg_ptr bg, const span<const int>& values_ref)` (L96): Creates a sp_direct_bitmap_bg_dy_register_hbe_ptr which changes the values to commit to the the sixth GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the the sixth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_dy_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L102): Returns the sp_direct_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the the sixth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the the sixth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the the sixth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the the sixth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the the sixth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_dy_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(sp_direct_bitmap_bg_dy_register_hbe_ptr& other)` (L165): Exchanges the contents of this sp_direct_bitmap_bg_dy_register_hbe_ptr with those of the other one.
  `other` sp_direct_bitmap_bg_dy_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sp_direct_bitmap_bg_dy_register_hbe_ptr& a, sp_direct_bitmap_bg_dy_register_hbe_ptr& b)` (L172): Exchanges the contents of a sp_direct_bitmap_bg_dy_register_hbe_ptr with those of another one.
  `a` First sp_direct_bitmap_bg_dy_register_hbe_ptr to exchange the contents with.; `b` Second sp_direct_bitmap_bg_dy_register_hbe_ptr to exchange the contents with.

## bn_sp_direct_bitmap_bg_mat_attributes_hbe_ptr.h

### `bn::sp_direct_bitmap_bg_mat_attributes_hbe_ptr` (class, bn_sp_direct_bitmap_bg_mat_attributes_hbe_ptr.h:36)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes which define the position and the transformation matrix of a sp_direct_bitmap_bg_ptr in each screen horizontal line.

`class sp_direct_bitmap_bg_mat_attributes_hbe_ptr`

- **create** `static sp_direct_bitmap_bg_mat_attributes_hbe_ptr create( const sp_direct_bitmap_bg_ptr& bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L53): Creates a sp_direct_bitmap_bg_mat_attributes_hbe_ptr which changes the attributes which define the position and the transformation matrix of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to set to the given sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_mat_attributes_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sp_direct_bitmap_bg_mat_attributes_hbe_ptr> create_optional( const sp_direct_bitmap_bg_ptr& bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L70): Creates a sp_direct_bitmap_bg_mat_attributes_hbe_ptr which changes the attributes which define the position and the transformation matrix of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to set to the given sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_mat_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **id** `int id() const` (L76): Returns the internal id.
- **visible** `bool visible() const` (L84): Indicates if this H-Blank effect must be committed to the GBA or not.
- **set_visible** `void set_visible(bool visible)` (L92): Sets if this H-Blank effect must be committed to the GBA or not.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L97): Returns the sp_direct_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L109): Returns the referenced array of 160 affine_bg_mat_attributes objects to set to the given sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L118): Sets the reference to an array of 160 affine_bg_mat_attributes objects to set to the given sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L127): Rereads the content of the referenced attributes to set to the given sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_mat_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(sp_direct_bitmap_bg_mat_attributes_hbe_ptr& other)` (L133): Exchanges the contents of this sp_direct_bitmap_bg_mat_attributes_hbe_ptr with those of the other one.
  `other` sp_direct_bitmap_bg_mat_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sp_direct_bitmap_bg_mat_attributes_hbe_ptr& a, sp_direct_bitmap_bg_mat_attributes_hbe_ptr& b)` (L140): Exchanges the contents of a sp_direct_bitmap_bg_mat_attributes_hbe_ptr with those of another one.
  `a` First sp_direct_bitmap_bg_mat_attributes_hbe_ptr to exchange the contents with.; `b` Second sp_direct_bitmap_bg_mat_attributes_hbe_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const sp_direct_bitmap_bg_mat_attributes_hbe_ptr& a, const sp_direct_bitmap_bg_mat_attributes_hbe_ptr& b)` (L151): Equal operator.
  `a` First sp_direct_bitmap_bg_mat_attributes_hbe_ptr to compare.; `b` Second sp_direct_bitmap_bg_mat_attributes_hbe_ptr to compare.; devuelve: `true` if the first sp_direct_bitmap_bg_mat_attributes_hbe_ptr is equal to the second one, otherwise `false`.
- **operator!=** `friend bool operator!=(const sp_direct_bitmap_bg_mat_attributes_hbe_ptr& a, const sp_direct_bitmap_bg_mat_attributes_hbe_ptr& b)` (L163): Not equal operator.
  `a` First sp_direct_bitmap_bg_mat_attributes_hbe_ptr to compare.; `b` Second sp_direct_bitmap_bg_mat_attributes_hbe_ptr to compare.; devuelve: `true` if the first sp_direct_bitmap_bg_mat_attributes_hbe_ptr is not equal to the second one, otherwise `false`.

## bn_sp_direct_bitmap_bg_pa_register_hbe_ptr.h

### `bn::sp_direct_bitmap_bg_pa_register_hbe_ptr` (class, bn_sp_direct_bitmap_bg_pa_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the first GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.

`class sp_direct_bitmap_bg_pa_register_hbe_ptr : public hbe_ptr`

- **create** `static sp_direct_bitmap_bg_pa_register_hbe_ptr create( sp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a sp_direct_bitmap_bg_pa_register_hbe_ptr which changes the attributes to commit to the first GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pa_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **create** `static sp_direct_bitmap_bg_pa_register_hbe_ptr create( sp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L65): Creates a sp_direct_bitmap_bg_pa_register_hbe_ptr which changes the values to commit to the first GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the first GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pa_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sp_direct_bitmap_bg_pa_register_hbe_ptr> create_optional( sp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a sp_direct_bitmap_bg_pa_register_hbe_ptr which changes the attributes to commit to the first GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pa_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sp_direct_bitmap_bg_pa_register_hbe_ptr> create_optional( sp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L96): Creates a sp_direct_bitmap_bg_pa_register_hbe_ptr which changes the values to commit to the first GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the first GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pa_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L102): Returns the sp_direct_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the first GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the first GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the first GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the first GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the first GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pa_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(sp_direct_bitmap_bg_pa_register_hbe_ptr& other)` (L165): Exchanges the contents of this sp_direct_bitmap_bg_pa_register_hbe_ptr with those of the other one.
  `other` sp_direct_bitmap_bg_pa_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sp_direct_bitmap_bg_pa_register_hbe_ptr& a, sp_direct_bitmap_bg_pa_register_hbe_ptr& b)` (L172): Exchanges the contents of a sp_direct_bitmap_bg_pa_register_hbe_ptr with those of another one.
  `a` First sp_direct_bitmap_bg_pa_register_hbe_ptr to exchange the contents with.; `b` Second sp_direct_bitmap_bg_pa_register_hbe_ptr to exchange the contents with.

## bn_sp_direct_bitmap_bg_pb_register_hbe_ptr.h

### `bn::sp_direct_bitmap_bg_pb_register_hbe_ptr` (class, bn_sp_direct_bitmap_bg_pb_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the second GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.

`class sp_direct_bitmap_bg_pb_register_hbe_ptr : public hbe_ptr`

- **create** `static sp_direct_bitmap_bg_pb_register_hbe_ptr create( sp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a sp_direct_bitmap_bg_pb_register_hbe_ptr which changes the attributes to commit to the second GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pb_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **create** `static sp_direct_bitmap_bg_pb_register_hbe_ptr create( sp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L65): Creates a sp_direct_bitmap_bg_pb_register_hbe_ptr which changes the values to commit to the second GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the second GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pb_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sp_direct_bitmap_bg_pb_register_hbe_ptr> create_optional( sp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a sp_direct_bitmap_bg_pb_register_hbe_ptr which changes the attributes to commit to the second GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pb_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sp_direct_bitmap_bg_pb_register_hbe_ptr> create_optional( sp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L96): Creates a sp_direct_bitmap_bg_pb_register_hbe_ptr which changes the values to commit to the second GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the second GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pb_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L102): Returns the sp_direct_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the second GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the second GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the second GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the second GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the second GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pb_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(sp_direct_bitmap_bg_pb_register_hbe_ptr& other)` (L165): Exchanges the contents of this sp_direct_bitmap_bg_pb_register_hbe_ptr with those of the other one.
  `other` sp_direct_bitmap_bg_pb_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sp_direct_bitmap_bg_pb_register_hbe_ptr& a, sp_direct_bitmap_bg_pb_register_hbe_ptr& b)` (L172): Exchanges the contents of a sp_direct_bitmap_bg_pb_register_hbe_ptr with those of another one.
  `a` First sp_direct_bitmap_bg_pb_register_hbe_ptr to exchange the contents with.; `b` Second sp_direct_bitmap_bg_pb_register_hbe_ptr to exchange the contents with.

## bn_sp_direct_bitmap_bg_pc_register_hbe_ptr.h

### `bn::sp_direct_bitmap_bg_pc_register_hbe_ptr` (class, bn_sp_direct_bitmap_bg_pc_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the third GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.

`class sp_direct_bitmap_bg_pc_register_hbe_ptr : public hbe_ptr`

- **create** `static sp_direct_bitmap_bg_pc_register_hbe_ptr create( sp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a sp_direct_bitmap_bg_pc_register_hbe_ptr which changes the attributes to commit to the third GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pc_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **create** `static sp_direct_bitmap_bg_pc_register_hbe_ptr create( sp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L65): Creates a sp_direct_bitmap_bg_pc_register_hbe_ptr which changes the values to commit to the third GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the third GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pc_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sp_direct_bitmap_bg_pc_register_hbe_ptr> create_optional( sp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a sp_direct_bitmap_bg_pc_register_hbe_ptr which changes the attributes to commit to the third GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pc_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sp_direct_bitmap_bg_pc_register_hbe_ptr> create_optional( sp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L96): Creates a sp_direct_bitmap_bg_pc_register_hbe_ptr which changes the values to commit to the third GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the third GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pc_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L102): Returns the sp_direct_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the third GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the third GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the third GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the third GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the third GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pc_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(sp_direct_bitmap_bg_pc_register_hbe_ptr& other)` (L165): Exchanges the contents of this sp_direct_bitmap_bg_pc_register_hbe_ptr with those of the other one.
  `other` sp_direct_bitmap_bg_pc_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sp_direct_bitmap_bg_pc_register_hbe_ptr& a, sp_direct_bitmap_bg_pc_register_hbe_ptr& b)` (L172): Exchanges the contents of a sp_direct_bitmap_bg_pc_register_hbe_ptr with those of another one.
  `a` First sp_direct_bitmap_bg_pc_register_hbe_ptr to exchange the contents with.; `b` Second sp_direct_bitmap_bg_pc_register_hbe_ptr to exchange the contents with.

## bn_sp_direct_bitmap_bg_pd_register_hbe_ptr.h

### `bn::sp_direct_bitmap_bg_pd_register_hbe_ptr` (class, bn_sp_direct_bitmap_bg_pd_register_hbe_ptr.h:33)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the fourth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.

`class sp_direct_bitmap_bg_pd_register_hbe_ptr : public hbe_ptr`

- **create** `static sp_direct_bitmap_bg_pd_register_hbe_ptr create( sp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L50): Creates a sp_direct_bitmap_bg_pd_register_hbe_ptr which changes the attributes to commit to the fourth GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pd_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **create** `static sp_direct_bitmap_bg_pd_register_hbe_ptr create( sp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L65): Creates a sp_direct_bitmap_bg_pd_register_hbe_ptr which changes the values to commit to the fourth GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the fourth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pd_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sp_direct_bitmap_bg_pd_register_hbe_ptr> create_optional( sp_direct_bitmap_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)` (L81): Creates a sp_direct_bitmap_bg_pd_register_hbe_ptr which changes the attributes to commit to the fourth GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pd_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sp_direct_bitmap_bg_pd_register_hbe_ptr> create_optional( sp_direct_bitmap_bg_ptr bg, const span<const int16_t>& values_ref)` (L96): Creates a sp_direct_bitmap_bg_pd_register_hbe_ptr which changes the values to commit to the fourth GBA register of a sp_direct_bitmap_bg_ptr.
  `bg` sp_direct_bitmap_bg_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the fourth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pd_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L102): Returns the sp_direct_bitmap_bg_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_bg_mat_attributes> attributes_ref() const` (L114): Returns the referenced array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L123): Returns the referenced array of 160 values to commit to the fourth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)` (L132): Sets the reference to an array of 160 affine_bg_mat_attributes objects with the attributes to commit to the fourth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L141): Sets the reference to an array of 160 values to commit to the fourth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L150): Rereads the content of the referenced attributes to commit to the fourth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L159): Rereads the content of the referenced values to commit to the fourth GBA register of a sp_direct_bitmap_bg_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sp_direct_bitmap_bg_pd_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(sp_direct_bitmap_bg_pd_register_hbe_ptr& other)` (L165): Exchanges the contents of this sp_direct_bitmap_bg_pd_register_hbe_ptr with those of the other one.
  `other` sp_direct_bitmap_bg_pd_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sp_direct_bitmap_bg_pd_register_hbe_ptr& a, sp_direct_bitmap_bg_pd_register_hbe_ptr& b)` (L172): Exchanges the contents of a sp_direct_bitmap_bg_pd_register_hbe_ptr with those of another one.
  `a` First sp_direct_bitmap_bg_pd_register_hbe_ptr to exchange the contents with.; `b` Second sp_direct_bitmap_bg_pd_register_hbe_ptr to exchange the contents with.

## bn_sp_direct_bitmap_bg_pivot_position_hbe_ptr.h

### `bn::sp_direct_bitmap_bg_pivot_position_hbe_ptr` (class, bn_sp_direct_bitmap_bg_pivot_position_hbe_ptr.h:32)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the position of the affine transformations pivot point of a single page direct bitmap background in each screen horizontal line.

`class sp_direct_bitmap_bg_pivot_position_hbe_ptr : public hbe_ptr`

- **create_horizontal** `static sp_direct_bitmap_bg_pivot_position_hbe_ptr create_horizontal( sp_direct_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)` (L49): Creates a sp_direct_bitmap_bg_pivot_position_hbe_ptr which changes the horizontal position of the affine transformations pivot point of a single page direct bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the horizontal position of the affine transformations pivot point of the given background in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pivot_position_hbe_ptr.
  The deltas are not copied but referenced,
  so they should outlive the sp_direct_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **create_vertical** `static sp_direct_bitmap_bg_pivot_position_hbe_ptr create_vertical( sp_direct_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)` (L65): Creates a sp_direct_bitmap_bg_pivot_position_hbe_ptr which changes the vertical position of the affine transformations pivot point of a single page direct bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the vertical position of the affine transformations pivot point of the given background in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pivot_position_hbe_ptr.
  The deltas are not copied but referenced,
  so they should outlive the sp_direct_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **create_horizontal_optional** `static optional<sp_direct_bitmap_bg_pivot_position_hbe_ptr> create_horizontal_optional( sp_direct_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)` (L82): Creates a sp_direct_bitmap_bg_pivot_position_hbe_ptr which changes the horizontal position of the affine transformations pivot point of a single page direct bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the horizontal position of the affine transformations pivot point of the given background in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pivot_position_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The deltas are not copied but referenced,
  so they should outlive the sp_direct_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **create_vertical_optional** `static optional<sp_direct_bitmap_bg_pivot_position_hbe_ptr> create_vertical_optional( sp_direct_bitmap_bg_ptr bg, const span<const fixed>& deltas_ref)` (L99): Creates a sp_direct_bitmap_bg_pivot_position_hbe_ptr which changes the vertical position of the affine transformations pivot point of a single page direct bitmap background in each screen horizontal line.
  `bg` Background to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the vertical position of the affine transformations pivot point of the given background in each screen horizontal line.; devuelve: The requested sp_direct_bitmap_bg_pivot_position_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The deltas are not copied but referenced,
  so they should outlive the sp_direct_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **bg** `const sp_direct_bitmap_bg_ptr& bg() const` (L105): Returns the background modified by this H-Blank effect.
- **deltas_ref** `span<const fixed> deltas_ref() const` (L117): Returns the referenced array of 160 deltas to add to the horizontal or vertical position of the affine transformations pivot point of the managed background in each screen horizontal line.
  The deltas are not copied but referenced,
  so they should outlive the sp_direct_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **set_deltas_ref** `void set_deltas_ref(const span<const fixed>& deltas_ref)` (L126): Sets the reference to an array of 160 deltas to add to the horizontal or vertical position of the affine transformations pivot point of the managed background in each screen horizontal line.
  The deltas are not copied but referenced,
  so they should outlive the sp_direct_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **reload_deltas_ref** `void reload_deltas_ref()` (L135): Rereads the content of the referenced deltas to add to the horizontal or vertical position of the affine transformations pivot point of the managed background in each screen horizontal line.
  The deltas are not copied but referenced,
  so they should outlive the sp_direct_bitmap_bg_pivot_position_hbe_ptr to avoid dangling references.
- **swap** `void swap(sp_direct_bitmap_bg_pivot_position_hbe_ptr& other)` (L141): Exchanges the contents of this sp_direct_bitmap_bg_pivot_position_hbe_ptr with those of the other one.
  `other` sp_direct_bitmap_bg_pivot_position_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sp_direct_bitmap_bg_pivot_position_hbe_ptr& a, sp_direct_bitmap_bg_pivot_position_hbe_ptr& b)` (L148): Exchanges the contents of a sp_direct_bitmap_bg_pivot_position_hbe_ptr with those of another one.
  `a` First sp_direct_bitmap_bg_pivot_position_hbe_ptr to exchange the contents with.; `b` Second sp_direct_bitmap_bg_pivot_position_hbe_ptr to exchange the contents with.

## bn_sprite_affine_mat_attributes_hbe_ptr.h

### `bn::sprite_affine_mat_attributes_hbe_ptr` (class, bn_sprite_affine_mat_attributes_hbe_ptr.h:35)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes of a sprite_affine_mat_ptr in each screen horizontal line.

`class sprite_affine_mat_attributes_hbe_ptr`

- **create** `static sprite_affine_mat_attributes_hbe_ptr create( const sprite_affine_mat_ptr& affine_mat, const span<const affine_mat_attributes>& attributes_ref)` (L51): Creates a sprite_affine_mat_attributes_hbe_ptr which changes the attributes of a sprite_affine_mat_ptr in each screen horizontal line.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_mat_attributes objects with the attributes to set to the given sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_attributes_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sprite_affine_mat_attributes_hbe_ptr> create_optional( const sprite_affine_mat_ptr& affine_mat, const span<const affine_mat_attributes>& attributes_ref)` (L67): Creates a sprite_affine_mat_attributes_hbe_ptr which changes the attributes of a sprite_affine_mat_ptr in each screen horizontal line.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_mat_attributes objects with the attributes to set to the given sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_attributes_hbe_ptr to avoid dangling references.
- **id** `int id() const` (L73): Returns the internal id.
- **visible** `bool visible() const` (L81): Indicates if this H-Blank effect must be committed to the GBA or not.
- **set_visible** `void set_visible(bool visible)` (L89): Sets if this H-Blank effect must be committed to the GBA or not.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L94): Returns the sprite_affine_mat_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_mat_attributes> attributes_ref() const` (L106): Returns the referenced array of 160 affine_mat_attributes objects to set to the given sprite_affine_mat_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_mat_attributes>& attributes_ref)` (L115): Sets the reference to an array of 160 affine_mat_attributes objects to set to the given sprite_affine_mat_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L124): Rereads the content of the referenced attributes to set to the given sprite_affine_mat_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(sprite_affine_mat_attributes_hbe_ptr& other)` (L130): Exchanges the contents of this sprite_affine_mat_attributes_hbe_ptr with those of the other one.
  `other` sprite_affine_mat_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sprite_affine_mat_attributes_hbe_ptr& a, sprite_affine_mat_attributes_hbe_ptr& b)` (L137): Exchanges the contents of a sprite_affine_mat_attributes_hbe_ptr with those of another one.
  `a` First sprite_affine_mat_attributes_hbe_ptr to exchange the contents with.; `b` Second sprite_affine_mat_attributes_hbe_ptr to exchange the contents with.
- **operator==** `friend bool operator==(const sprite_affine_mat_attributes_hbe_ptr& a, const sprite_affine_mat_attributes_hbe_ptr& b)` (L149): Equal operator.
  `a` First sprite_affine_mat_attributes_hbe_ptr to compare.; `b` Second sprite_affine_mat_attributes_hbe_ptr to compare.; devuelve: `true` if the first sprite_affine_mat_attributes_hbe_ptr is equal to the second one, otherwise `false`.
- **operator!=** `friend bool operator!=(const sprite_affine_mat_attributes_hbe_ptr& a, const sprite_affine_mat_attributes_hbe_ptr& b)` (L162): Not equal operator.
  `a` First sprite_affine_mat_attributes_hbe_ptr to compare.; `b` Second sprite_affine_mat_attributes_hbe_ptr to compare.; devuelve: `true` if the first sprite_affine_mat_attributes_hbe_ptr is not equal to the second one, otherwise `false`.

## bn_sprite_affine_mat_pa_register_hbe_ptr.h

### `bn::sprite_affine_mat_pa_register_hbe_ptr` (class, bn_sprite_affine_mat_pa_register_hbe_ptr.h:34)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the first GBA register of a sprite_affine_mat_ptr in each screen horizontal line.

`class sprite_affine_mat_pa_register_hbe_ptr : public hbe_ptr`

- **create** `static sprite_affine_mat_pa_register_hbe_ptr create( sprite_affine_mat_ptr affine_mat, const span<const affine_mat_attributes>& attributes_ref)` (L51): Creates a sprite_affine_mat_pa_register_hbe_ptr which changes the attributes to commit to the first GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_mat_attributes objects with the attributes to commit to the first GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pa_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pa_register_hbe_ptr to avoid dangling references.
- **create** `static sprite_affine_mat_pa_register_hbe_ptr create( sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref)` (L66): Creates a sprite_affine_mat_pa_register_hbe_ptr which changes the values to commit to the first GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the first GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pa_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pa_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sprite_affine_mat_pa_register_hbe_ptr> create_optional( sprite_affine_mat_ptr affine_mat, const span<const affine_mat_attributes>& attributes_ref)` (L83): Creates a sprite_affine_mat_pa_register_hbe_ptr which changes the attributes to commit to the first GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_mat_attributes objects with the attributes to commit to the first GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pa_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pa_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sprite_affine_mat_pa_register_hbe_ptr> create_optional( sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref)` (L99): Creates a sprite_affine_mat_pa_register_hbe_ptr which changes the values to commit to the first GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the first GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pa_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pa_register_hbe_ptr to avoid dangling references.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L105): Returns the sprite_affine_mat_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_mat_attributes> attributes_ref() const` (L117): Returns the referenced array of 160 affine_mat_attributes objects with the attributes to commit to the first GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pa_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L126): Returns the referenced array of 160 values to commit to the first GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pa_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_mat_attributes>& attributes_ref)` (L135): Sets the reference to an array of 160 affine_mat_attributes objects with the attributes to commit to the first GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pa_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L144): Sets the reference to an array of 160 values to commit to the first GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pa_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L153): Rereads the content of the referenced attributes to commit to the first GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pa_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L162): Rereads the content of the referenced values to commit to the first GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pa_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(sprite_affine_mat_pa_register_hbe_ptr& other)` (L168): Exchanges the contents of this sprite_affine_mat_pa_register_hbe_ptr with those of the other one.
  `other` sprite_affine_mat_pa_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sprite_affine_mat_pa_register_hbe_ptr& a, sprite_affine_mat_pa_register_hbe_ptr& b)` (L175): Exchanges the contents of a sprite_affine_mat_pa_register_hbe_ptr with those of another one.
  `a` First sprite_affine_mat_pa_register_hbe_ptr to exchange the contents with.; `b` Second sprite_affine_mat_pa_register_hbe_ptr to exchange the contents with.

## bn_sprite_affine_mat_pb_register_hbe_ptr.h

### `bn::sprite_affine_mat_pb_register_hbe_ptr` (class, bn_sprite_affine_mat_pb_register_hbe_ptr.h:34)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the second GBA register of a sprite_affine_mat_ptr in each screen horizontal line.

`class sprite_affine_mat_pb_register_hbe_ptr : public hbe_ptr`

- **create** `static sprite_affine_mat_pb_register_hbe_ptr create( sprite_affine_mat_ptr affine_mat, const span<const affine_mat_attributes>& attributes_ref)` (L51): Creates a sprite_affine_mat_pb_register_hbe_ptr which changes the attributes to commit to the second GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_mat_attributes objects with the attributes to commit to the second GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pb_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pb_register_hbe_ptr to avoid dangling references.
- **create** `static sprite_affine_mat_pb_register_hbe_ptr create( sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref)` (L66): Creates a sprite_affine_mat_pb_register_hbe_ptr which changes the values to commit to the second GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the second GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pb_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pb_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sprite_affine_mat_pb_register_hbe_ptr> create_optional( sprite_affine_mat_ptr affine_mat, const span<const affine_mat_attributes>& attributes_ref)` (L83): Creates a sprite_affine_mat_pb_register_hbe_ptr which changes the attributes to commit to the second GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_mat_attributes objects with the attributes to commit to the second GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pb_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pb_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sprite_affine_mat_pb_register_hbe_ptr> create_optional( sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref)` (L99): Creates a sprite_affine_mat_pb_register_hbe_ptr which changes the values to commit to the second GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the second GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pb_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pb_register_hbe_ptr to avoid dangling references.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L105): Returns the sprite_affine_mat_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_mat_attributes> attributes_ref() const` (L117): Returns the referenced array of 160 affine_mat_attributes objects with the attributes to commit to the second GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pb_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L126): Returns the referenced array of 160 values to commit to the second GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pb_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_mat_attributes>& attributes_ref)` (L135): Sets the reference to an array of 160 affine_mat_attributes objects with the attributes to commit to the second GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pb_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L144): Sets the reference to an array of 160 values to commit to the second GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pb_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L153): Rereads the content of the referenced attributes to commit to the second GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pb_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L162): Rereads the content of the referenced values to commit to the second GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pb_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(sprite_affine_mat_pb_register_hbe_ptr& other)` (L168): Exchanges the contents of this sprite_affine_mat_pb_register_hbe_ptr with those of the other one.
  `other` sprite_affine_mat_pb_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sprite_affine_mat_pb_register_hbe_ptr& a, sprite_affine_mat_pb_register_hbe_ptr& b)` (L175): Exchanges the contents of a sprite_affine_mat_pb_register_hbe_ptr with those of another one.
  `a` First sprite_affine_mat_pb_register_hbe_ptr to exchange the contents with.; `b` Second sprite_affine_mat_pb_register_hbe_ptr to exchange the contents with.

## bn_sprite_affine_mat_pc_register_hbe_ptr.h

### `bn::sprite_affine_mat_pc_register_hbe_ptr` (class, bn_sprite_affine_mat_pc_register_hbe_ptr.h:34)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.

`class sprite_affine_mat_pc_register_hbe_ptr : public hbe_ptr`

- **create** `static sprite_affine_mat_pc_register_hbe_ptr create( sprite_affine_mat_ptr affine_mat, const span<const affine_mat_attributes>& attributes_ref)` (L51): Creates a sprite_affine_mat_pc_register_hbe_ptr which changes the attributes to commit to the third GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_mat_attributes objects with the attributes to commit to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pc_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pc_register_hbe_ptr to avoid dangling references.
- **create** `static sprite_affine_mat_pc_register_hbe_ptr create( sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref)` (L66): Creates a sprite_affine_mat_pc_register_hbe_ptr which changes the values to commit to the third GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pc_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pc_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sprite_affine_mat_pc_register_hbe_ptr> create_optional( sprite_affine_mat_ptr affine_mat, const span<const affine_mat_attributes>& attributes_ref)` (L83): Creates a sprite_affine_mat_pc_register_hbe_ptr which changes the attributes to commit to the third GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_mat_attributes objects with the attributes to commit to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pc_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pc_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sprite_affine_mat_pc_register_hbe_ptr> create_optional( sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref)` (L99): Creates a sprite_affine_mat_pc_register_hbe_ptr which changes the values to commit to the third GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pc_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pc_register_hbe_ptr to avoid dangling references.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L105): Returns the sprite_affine_mat_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_mat_attributes> attributes_ref() const` (L117): Returns the referenced array of 160 affine_mat_attributes objects with the attributes to commit to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pc_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L126): Returns the referenced array of 160 values to commit to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pc_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_mat_attributes>& attributes_ref)` (L135): Sets the reference to an array of 160 affine_mat_attributes objects with the attributes to commit to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pc_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L144): Sets the reference to an array of 160 values to commit to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pc_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L153): Rereads the content of the referenced attributes to commit to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pc_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L162): Rereads the content of the referenced values to commit to the third GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pc_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(sprite_affine_mat_pc_register_hbe_ptr& other)` (L168): Exchanges the contents of this sprite_affine_mat_pc_register_hbe_ptr with those of the other one.
  `other` sprite_affine_mat_pc_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sprite_affine_mat_pc_register_hbe_ptr& a, sprite_affine_mat_pc_register_hbe_ptr& b)` (L175): Exchanges the contents of a sprite_affine_mat_pc_register_hbe_ptr with those of another one.
  `a` First sprite_affine_mat_pc_register_hbe_ptr to exchange the contents with.; `b` Second sprite_affine_mat_pc_register_hbe_ptr to exchange the contents with.

## bn_sprite_affine_mat_pd_register_hbe_ptr.h

### `bn::sprite_affine_mat_pd_register_hbe_ptr` (class, bn_sprite_affine_mat_pd_register_hbe_ptr.h:34)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes or the values to commit to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.

`class sprite_affine_mat_pd_register_hbe_ptr : public hbe_ptr`

- **create** `static sprite_affine_mat_pd_register_hbe_ptr create( sprite_affine_mat_ptr affine_mat, const span<const affine_mat_attributes>& attributes_ref)` (L51): Creates a sprite_affine_mat_pd_register_hbe_ptr which changes the attributes to commit to the fourth GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_mat_attributes objects with the attributes to commit to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pd_register_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
- **create** `static sprite_affine_mat_pd_register_hbe_ptr create( sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref)` (L66): Creates a sprite_affine_mat_pd_register_hbe_ptr which changes the values to commit to the fourth GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pd_register_hbe_ptr.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sprite_affine_mat_pd_register_hbe_ptr> create_optional( sprite_affine_mat_ptr affine_mat, const span<const affine_mat_attributes>& attributes_ref)` (L83): Creates a sprite_affine_mat_pd_register_hbe_ptr which changes the attributes to commit to the fourth GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `attributes_ref` Reference to an array of 160 affine_mat_attributes objects with the attributes to commit to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pd_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sprite_affine_mat_pd_register_hbe_ptr> create_optional( sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref)` (L99): Creates a sprite_affine_mat_pd_register_hbe_ptr which changes the values to commit to the fourth GBA register of a sprite_affine_mat_ptr.
  `affine_mat` sprite_affine_mat_ptr to be modified.; `values_ref` Reference to an array of 160 values to commit to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.; devuelve: The requested sprite_affine_mat_pd_register_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
- **affine_mat** `const sprite_affine_mat_ptr& affine_mat() const` (L105): Returns the sprite_affine_mat_ptr modified by this H-Blank effect.
- **attributes_ref** `span<const affine_mat_attributes> attributes_ref() const` (L117): Returns the referenced array of 160 affine_mat_attributes objects with the attributes to commit to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
- **values_ref** `span<const int16_t> values_ref() const` (L126): Returns the referenced array of 160 values to commit to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const affine_mat_attributes>& attributes_ref)` (L135): Sets the reference to an array of 160 affine_mat_attributes objects with the attributes to commit to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
- **set_values_ref** `void set_values_ref(const span<const int16_t>& values_ref)` (L144): Sets the reference to an array of 160 values to commit to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L153): Rereads the content of the referenced attributes to commit to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
- **reload_values_ref** `void reload_values_ref()` (L162): Rereads the content of the referenced values to commit to the fourth GBA register of a sprite_affine_mat_ptr in each screen horizontal line.
  The values are not copied but referenced, so they should outlive
  the sprite_affine_mat_pd_register_hbe_ptr to avoid dangling references.
- **swap** `void swap(sprite_affine_mat_pd_register_hbe_ptr& other)` (L168): Exchanges the contents of this sprite_affine_mat_pd_register_hbe_ptr with those of the other one.
  `other` sprite_affine_mat_pd_register_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sprite_affine_mat_pd_register_hbe_ptr& a, sprite_affine_mat_pd_register_hbe_ptr& b)` (L175): Exchanges the contents of a sprite_affine_mat_pd_register_hbe_ptr with those of another one.
  `a` First sprite_affine_mat_pd_register_hbe_ptr to exchange the contents with.; `b` Second sprite_affine_mat_pd_register_hbe_ptr to exchange the contents with.

## bn_sprite_affine_second_attributes_hbe_ptr.h

### `bn::sprite_affine_second_attributes_hbe_ptr` (class, bn_sprite_affine_second_attributes_hbe_ptr.h:32)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes to commit to the second GBA register of an affine sprite (with an attached sprite_affine_mat_ptr) in each screen horizontal line.

`class sprite_affine_second_attributes_hbe_ptr : public hbe_ptr`

- **create** `static sprite_affine_second_attributes_hbe_ptr create( sprite_ptr sprite, const span<const sprite_affine_second_attributes>& attributes_ref)` (L52): Creates a sprite_affine_second_attributes_hbe_ptr which changes the attributes to commit to the second GBA register of an affine sprite in each screen horizontal line.
  `sprite` Affine sprite (with an attached sprite_affine_mat_ptr) to be modified.; `attributes_ref` Reference to an array of 160 sprite_affine_second_attributes objects with the attributes to commit to the second GBA register of the given affine sprite in each screen horizontal line.; devuelve: The requested sprite_affine_second_attributes_hbe_ptr.
  The horizontal position of the attributes are added as a delta to the horizontal position of the sprite.
  In other words, they are relative instead of absolute.
  
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_second_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sprite_affine_second_attributes_hbe_ptr> create_optional( sprite_ptr sprite, const span<const sprite_affine_second_attributes>& attributes_ref)` (L72): Creates a sprite_affine_second_attributes_hbe_ptr which changes the attributes to commit to the second GBA register of an affine sprite in each screen horizontal line.
  `sprite` Affine sprite (with an attached sprite_affine_mat_ptr) to be modified.; `attributes_ref` Reference to an array of 160 sprite_affine_second_attributes objects with the attributes to commit to the second GBA register of the given affine sprite in each screen horizontal line.; devuelve: The requested sprite_affine_second_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The horizontal position of the attributes are added as a delta to the horizontal position of the sprite.
  In other words, they are relative instead of absolute.
  
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_second_attributes_hbe_ptr to avoid dangling references.
- **sprite** `const sprite_ptr& sprite() const` (L78): Returns the affine sprite modified by this H-Blank effect.
- **attributes_ref** `span<const sprite_affine_second_attributes> attributes_ref() const` (L94): Returns the referenced array of 160 sprite_affine_second_attributes objects with the attributes to commit to the second GBA register of the given affine sprite in each screen horizontal line.
  The horizontal position of the attributes are added as a delta to the horizontal position of the sprite.
  In other words, they are relative instead of absolute.
  
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_second_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const sprite_affine_second_attributes>& attributes_ref)` (L107): Sets the reference to an array of 160 sprite_affine_second_attributes objects with the attributes to commit to the second GBA register of the given affine sprite in each screen horizontal line.
  The horizontal position of the attributes are added as a delta to the horizontal position of the sprite.
  In other words, they are relative instead of absolute.
  
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_second_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L119): Rereads the content of the referenced attributes to commit to the second GBA register of the given affine sprite in each screen horizontal line.
  The horizontal position of the attributes are added as a delta to the horizontal position of the sprite.
  In other words, they are relative instead of absolute.
  
  The attributes are not copied but referenced, so they should outlive
  the sprite_affine_second_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(sprite_affine_second_attributes_hbe_ptr& other)` (L125): Exchanges the contents of this sprite_affine_second_attributes_hbe_ptr with those of the other one.
  `other` sprite_affine_second_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sprite_affine_second_attributes_hbe_ptr& a, sprite_affine_second_attributes_hbe_ptr& b)` (L132): Exchanges the contents of a sprite_affine_second_attributes_hbe_ptr with those of another one.
  `a` First sprite_affine_second_attributes_hbe_ptr to exchange the contents with.; `b` Second sprite_affine_second_attributes_hbe_ptr to exchange the contents with.

## bn_sprite_first_attributes_hbe_ptr.h

### `bn::sprite_first_attributes_hbe_ptr` (class, bn_sprite_first_attributes_hbe_ptr.h:31)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes to commit to the first GBA register of a sprite in each screen horizontal line.

`class sprite_first_attributes_hbe_ptr : public hbe_ptr`

- **create** `static sprite_first_attributes_hbe_ptr create( sprite_ptr sprite, const span<const sprite_first_attributes>& attributes_ref)` (L47): Creates a sprite_first_attributes_hbe_ptr which changes the attributes to commit to the first GBA register of a sprite in each screen horizontal line.
  `sprite` Sprite to be modified.; `attributes_ref` Reference to an array of 160 sprite_first_attributes objects with the attributes to commit to the first GBA register of the given sprite in each screen horizontal line.; devuelve: The requested sprite_first_attributes_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the sprite_first_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sprite_first_attributes_hbe_ptr> create_optional( sprite_ptr sprite, const span<const sprite_first_attributes>& attributes_ref)` (L62): Creates a sprite_first_attributes_hbe_ptr which changes the attributes to commit to the first GBA register of a sprite in each screen horizontal line.
  `sprite` Sprite to be modified.; `attributes_ref` Reference to an array of 160 sprite_first_attributes objects with the attributes to commit to the first GBA register of the given sprite in each screen horizontal line.; devuelve: The requested sprite_first_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the sprite_first_attributes_hbe_ptr to avoid dangling references.
- **sprite** `const sprite_ptr& sprite() const` (L68): Returns the sprite modified by this H-Blank effect.
- **attributes_ref** `span<const sprite_first_attributes> attributes_ref() const` (L80): Returns the referenced array of 160 sprite_first_attributes objects with the attributes to commit to the first GBA register of the managed sprite in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_first_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const sprite_first_attributes>& attributes_ref)` (L89): Sets the reference to an array of 160 sprite_first_attributes objects with the attributes to commit to the first GBA register of the managed sprite in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_first_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L98): Rereads the content of the referenced attributes to commit to the first GBA register of the managed sprite in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_first_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(sprite_first_attributes_hbe_ptr& other)` (L104): Exchanges the contents of this sprite_first_attributes_hbe_ptr with those of the other one.
  `other` sprite_first_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sprite_first_attributes_hbe_ptr& a, sprite_first_attributes_hbe_ptr& b)` (L111): Exchanges the contents of a sprite_first_attributes_hbe_ptr with those of another one.
  `a` First sprite_first_attributes_hbe_ptr to exchange the contents with.; `b` Second sprite_first_attributes_hbe_ptr to exchange the contents with.

## bn_sprite_palette_color_hbe_ptr.h

### `bn::sprite_palette_color_hbe_ptr` (class, bn_sprite_palette_color_hbe_ptr.h:35)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the color of a sprite color palette in each screen horizontal line.

`class sprite_palette_color_hbe_ptr : public hbe_ptr`

- **create** `static sprite_palette_color_hbe_ptr create( sprite_palette_ptr palette, int color_index, const span<const color>& colors_ref)` (L52): Creates a sprite_palette_color_hbe_ptr which changes the color of a sprite color palette in each screen horizontal line.
  `palette` Sprite color palette to be modified.; `color_index` Index of the color of the given sprite color palette to be modified.; `colors_ref` Reference to an array of 160 colors to set to the color of a sprite color palette in each screen horizontal line.; devuelve: The requested sprite_palette_color_hbe_ptr.
  The colors are not copied but referenced, so they should outlive the sprite_palette_color_hbe_ptr
  to avoid dangling references.
- **create_optional** `static optional<sprite_palette_color_hbe_ptr> create_optional( sprite_palette_ptr palette, int color_index, const span<const color>& colors_ref)` (L68): Creates a sprite_palette_color_hbe_ptr which changes the color of a sprite color palette in each screen horizontal line.
  `palette` Sprite color palette to be modified.; `color_index` Index of the color of the given sprite color palette to be modified.; `colors_ref` Reference to an array of 160 colors to set to the color of a sprite color palette in each screen horizontal line.; devuelve: The requested sprite_palette_color_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The colors are not copied but referenced, so they should outlive the sprite_palette_color_hbe_ptr
  to avoid dangling references.
- **palette** `const sprite_palette_ptr& palette() const` (L74): Returns the sprite color palette modified by this H-Blank effect.
- **color_index** `int color_index() const` (L82): Returns the index of the color of the given sprite color palette to be modified.
- **colors_ref** `span<const color> colors_ref() const` (L94): Returns the referenced array of 160 colors to set to the color of a sprite color palette in each screen horizontal line.
  The colors are not copied but referenced, so they should outlive the sprite_palette_color_hbe_ptr
  to avoid dangling references.
- **set_colors_ref** `void set_colors_ref(const span<const color>& colors_ref)` (L103): Sets the reference to an array of 160 colors to set to the color of a sprite color palette in each screen horizontal line.
  The colors are not copied but referenced, so they should outlive the sprite_palette_color_hbe_ptr
  to avoid dangling references.
- **reload_colors_ref** `void reload_colors_ref()` (L112): Rereads the content of the referenced colors to set to the color of a sprite color palette in each screen horizontal line.
  The colors are not copied but referenced, so they should outlive the sprite_palette_color_hbe_ptr
  to avoid dangling references.
- **swap** `void swap(sprite_palette_color_hbe_ptr& other)` (L118): Exchanges the contents of this sprite_palette_color_hbe_ptr with those of the other one.
  `other` sprite_palette_color_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sprite_palette_color_hbe_ptr& a, sprite_palette_color_hbe_ptr& b)` (L125): Exchanges the contents of a sprite_palette_color_hbe_ptr with those of another one.
  `a` First sprite_palette_color_hbe_ptr to exchange the contents with.; `b` Second sprite_palette_color_hbe_ptr to exchange the contents with.

## bn_sprite_position_hbe_ptr.h

### `bn::sprite_position_hbe_ptr` (class, bn_sprite_position_hbe_ptr.h:31)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the position of a sprite in each screen horizontal line.

`class sprite_position_hbe_ptr : public hbe_ptr`

- **create_horizontal** `static sprite_position_hbe_ptr create_horizontal( sprite_ptr sprite, const span<const fixed>& deltas_ref)` (L47): Creates a sprite_position_hbe_ptr which changes the horizontal position of a sprite in each screen horizontal line.
  `sprite` Sprite to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the horizontal position of the given sprite in each screen horizontal line.; devuelve: The requested sprite_position_hbe_ptr.
  The deltas are not copied but referenced, so they should outlive the sprite_position_hbe_ptr
  to avoid dangling references.
- **create_vertical** `static sprite_position_hbe_ptr create_vertical( sprite_ptr sprite, const span<const fixed>& deltas_ref)` (L62): Creates a sprite_position_hbe_ptr which changes the vertical position of a sprite in each screen horizontal line.
  `sprite` Sprite to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the vertical position of the given sprite in each screen horizontal line.; devuelve: The requested sprite_position_hbe_ptr.
  The deltas are not copied but referenced, so they should outlive the sprite_position_hbe_ptr
  to avoid dangling references.
- **create_horizontal_optional** `static optional<sprite_position_hbe_ptr> create_horizontal_optional( sprite_ptr sprite, const span<const fixed>& deltas_ref)` (L77): Creates a sprite_position_hbe_ptr which changes the horizontal position of a sprite in each screen horizontal line.
  `sprite` Sprite to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the horizontal position of the given sprite in each screen horizontal line.; devuelve: The requested sprite_position_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The deltas are not copied but referenced, so they should outlive the sprite_position_hbe_ptr
  to avoid dangling references.
- **create_vertical_optional** `static optional<sprite_position_hbe_ptr> create_vertical_optional( sprite_ptr sprite, const span<const fixed>& deltas_ref)` (L92): Creates a sprite_position_hbe_ptr which changes the vertical position of a sprite in each screen horizontal line.
  `sprite` Sprite to be modified.; `deltas_ref` Reference to an array of 160 deltas to add to the vertical position of the given sprite in each screen horizontal line.; devuelve: The requested sprite_position_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The deltas are not copied but referenced, so they should outlive the sprite_position_hbe_ptr
  to avoid dangling references.
- **sprite** `const sprite_ptr& sprite() const` (L98): Returns the sprite modified by this H-Blank effect.
- **deltas_ref** `span<const fixed> deltas_ref() const` (L110): Returns the referenced array of 160 deltas to add to the horizontal or vertical position of the managed sprite in each screen horizontal line.
  The deltas are not copied but referenced, so they should outlive the sprite_position_hbe_ptr
  to avoid dangling references.
- **set_deltas_ref** `void set_deltas_ref(const span<const fixed>& deltas_ref)` (L119): Sets the reference to an array of 160 deltas to add to the horizontal or vertical position of the managed sprite in each screen horizontal line.
  The deltas are not copied but referenced, so they should outlive the sprite_position_hbe_ptr
  to avoid dangling references.
- **reload_deltas_ref** `void reload_deltas_ref()` (L128): Rereads the content of the referenced deltas to add to the horizontal or vertical position of the managed sprite in each screen horizontal line.
  The deltas are not copied but referenced, so they should outlive the sprite_position_hbe_ptr
  to avoid dangling references.
- **swap** `void swap(sprite_position_hbe_ptr& other)` (L134): Exchanges the contents of this sprite_position_hbe_ptr with those of the other one.
  `other` sprite_position_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sprite_position_hbe_ptr& a, sprite_position_hbe_ptr& b)` (L141): Exchanges the contents of a sprite_position_hbe_ptr with those of another one.
  `a` First sprite_position_hbe_ptr to exchange the contents with.; `b` Second sprite_position_hbe_ptr to exchange the contents with.

## bn_sprite_regular_second_attributes_hbe_ptr.h

### `bn::sprite_regular_second_attributes_hbe_ptr` (class, bn_sprite_regular_second_attributes_hbe_ptr.h:32)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes to commit to the second GBA register of a regular sprite (without an attached sprite_affine_mat_ptr) in each screen horizontal line.

`class sprite_regular_second_attributes_hbe_ptr : public hbe_ptr`

- **create** `static sprite_regular_second_attributes_hbe_ptr create( sprite_ptr sprite, const span<const sprite_regular_second_attributes>& attributes_ref)` (L52): Creates a sprite_regular_second_attributes_hbe_ptr which changes the attributes to commit to the second GBA register of a regular sprite in each screen horizontal line.
  `sprite` Regular sprite (without an attached sprite_affine_mat_ptr) to be modified.; `attributes_ref` Reference to an array of 160 sprite_regular_second_attributes objects with the attributes to commit to the second GBA register of the given regular sprite in each screen horizontal line.; devuelve: The requested sprite_regular_second_attributes_hbe_ptr.
  The horizontal position of the attributes are added as a delta to the horizontal position of the sprite.
  In other words, they are relative instead of absolute.
  
  The attributes are not copied but referenced, so they should outlive
  the sprite_regular_second_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sprite_regular_second_attributes_hbe_ptr> create_optional( sprite_ptr sprite, const span<const sprite_regular_second_attributes>& attributes_ref)` (L72): Creates a sprite_regular_second_attributes_hbe_ptr which changes the attributes to commit to the second GBA register of a regular sprite in each screen horizontal line.
  `sprite` Regular sprite (without an attached sprite_affine_mat_ptr) to be modified.; `attributes_ref` Reference to an array of 160 sprite_regular_second_attributes objects with the attributes to commit to the second GBA register of the given regular sprite in each screen horizontal line.; devuelve: The requested sprite_regular_second_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The horizontal position of the attributes are added as a delta to the horizontal position of the sprite.
  In other words, they are relative instead of absolute.
  
  The attributes are not copied but referenced, so they should outlive
  the sprite_regular_second_attributes_hbe_ptr to avoid dangling references.
- **sprite** `const sprite_ptr& sprite() const` (L78): Returns the regular sprite modified by this H-Blank effect.
- **attributes_ref** `span<const sprite_regular_second_attributes> attributes_ref() const` (L94): Returns the referenced array of 160 sprite_regular_second_attributes objects with the attributes to commit to the second GBA register of the given regular sprite in each screen horizontal line.
  The horizontal position of the attributes are added as a delta to the horizontal position of the sprite.
  In other words, they are relative instead of absolute.
  
  The attributes are not copied but referenced, so they should outlive
  the sprite_regular_second_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const sprite_regular_second_attributes>& attributes_ref)` (L107): Sets the reference to an array of 160 sprite_regular_second_attributes objects with the attributes to commit to the second GBA register of the given regular sprite in each screen horizontal line.
  The horizontal position of the attributes are added as a delta to the horizontal position of the sprite.
  In other words, they are relative instead of absolute.
  
  The attributes are not copied but referenced, so they should outlive
  the sprite_regular_second_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L119): Rereads the content of the referenced attributes to commit to the second GBA register of the given regular sprite in each screen horizontal line.
  The horizontal position of the attributes are added as a delta to the horizontal position of the sprite.
  In other words, they are relative instead of absolute.
  
  The attributes are not copied but referenced, so they should outlive
  the sprite_regular_second_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(sprite_regular_second_attributes_hbe_ptr& other)` (L125): Exchanges the contents of this sprite_regular_second_attributes_hbe_ptr with those of the other one.
  `other` sprite_regular_second_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sprite_regular_second_attributes_hbe_ptr& a, sprite_regular_second_attributes_hbe_ptr& b)` (L132): Exchanges the contents of a sprite_regular_second_attributes_hbe_ptr with those of another one.
  `a` First sprite_regular_second_attributes_hbe_ptr to exchange the contents with.; `b` Second sprite_regular_second_attributes_hbe_ptr to exchange the contents with.

## bn_sprite_third_attributes_hbe_ptr.h

### `bn::sprite_third_attributes_hbe_ptr` (class, bn_sprite_third_attributes_hbe_ptr.h:31)

std::shared_ptr like smart pointer that retains shared ownership of a H-Blank effect which changes the attributes to commit to the third GBA register of a sprite in each screen horizontal line.

`class sprite_third_attributes_hbe_ptr : public hbe_ptr`

- **create** `static sprite_third_attributes_hbe_ptr create( sprite_ptr sprite, const span<const sprite_third_attributes>& attributes_ref)` (L47): Creates a sprite_third_attributes_hbe_ptr which changes the attributes to commit to the third GBA register of a sprite in each screen horizontal line.
  `sprite` Sprite to be modified.; `attributes_ref` Reference to an array of 160 sprite_third_attributes objects with the attributes to commit to the third GBA register of the given sprite in each screen horizontal line.; devuelve: The requested sprite_third_attributes_hbe_ptr.
  The attributes are not copied but referenced, so they should outlive
  the sprite_third_attributes_hbe_ptr to avoid dangling references.
- **create_optional** `static optional<sprite_third_attributes_hbe_ptr> create_optional( sprite_ptr sprite, const span<const sprite_third_attributes>& attributes_ref)` (L62): Creates a sprite_third_attributes_hbe_ptr which changes the attributes to commit to the third GBA register of a sprite in each screen horizontal line.
  `sprite` Sprite to be modified.; `attributes_ref` Reference to an array of 160 sprite_third_attributes objects with the attributes to commit to the third GBA register of the given sprite in each screen horizontal line.; devuelve: The requested sprite_third_attributes_hbe_ptr if it could be allocated; bn::nullopt otherwise.
  The attributes are not copied but referenced, so they should outlive
  the sprite_third_attributes_hbe_ptr to avoid dangling references.
- **sprite** `const sprite_ptr& sprite() const` (L68): Returns the sprite modified by this H-Blank effect.
- **attributes_ref** `span<const sprite_third_attributes> attributes_ref() const` (L80): Returns the referenced array of 160 sprite_third_attributes objects with the attributes to commit to the third GBA register of the managed sprite in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_third_attributes_hbe_ptr to avoid dangling references.
- **set_attributes_ref** `void set_attributes_ref(const span<const sprite_third_attributes>& attributes_ref)` (L89): Sets the reference to an array of 160 sprite_third_attributes objects with the attributes to commit to the third GBA register of the managed sprite in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_third_attributes_hbe_ptr to avoid dangling references.
- **reload_attributes_ref** `void reload_attributes_ref()` (L98): Rereads the content of the referenced attributes to commit to the third GBA register of the managed sprite in each screen horizontal line.
  The attributes are not copied but referenced, so they should outlive
  the sprite_third_attributes_hbe_ptr to avoid dangling references.
- **swap** `void swap(sprite_third_attributes_hbe_ptr& other)` (L104): Exchanges the contents of this sprite_third_attributes_hbe_ptr with those of the other one.
  `other` sprite_third_attributes_hbe_ptr to exchange the contents with.
- **swap** `friend void swap(sprite_third_attributes_hbe_ptr& a, sprite_third_attributes_hbe_ptr& b)` (L111): Exchanges the contents of a sprite_third_attributes_hbe_ptr with those of another one.
  `a` First sprite_third_attributes_hbe_ptr to exchange the contents with.; `b` Second sprite_third_attributes_hbe_ptr to exchange the contents with.
