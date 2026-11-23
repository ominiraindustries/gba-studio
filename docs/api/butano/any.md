# Butano 21.10.1 — Any (`any`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_any.h

### `bn::iany` (class, bn_any.h:33)

Base class of bn::any.

`class iany`

Can be used as a reference type for all bn::any containers.

- **~iany** `~iany() noexcept` (L42): Destructor.
- **operator=** `iany& operator=(const iany& other)` (L52): Copy assignment operator.
  `other` iany to copy.; devuelve: Reference to this.
- **operator=** `iany& operator=(iany&& other) noexcept` (L68): Move assignment operator.
  `other` iany to move.; devuelve: Reference to this.
- **operator=** `template<typename Type> iany& operator=(const Type& value)` (L83): Constructor.
  `value` Constant value to copy.
- **operator=** `template<typename Type> iany& operator=(Type& value)` (L100): Constructor.
  `value` Non constant value to copy.
- **operator=** `template<typename Type> iany& operator=(Type&& value) noexcept` (L117): Constructor.
  `value` Value to move.
- **has_value** `bool has_value() const` (L133): Indicates if it contains a value or not.
- **operator bool** `explicit operator bool() const` (L141): Indicates if it contains a value or not.
- **type** `type_id_t type() const` (L149): Returns the type_id_t of the contained value if any, otherwise type_id_t().
- **max_size** `int max_size() const` (L157): Returns the maximum size in bytes of the managed objects.
- **max_alignment** `int max_alignment() const` (L165): Returns the maximum alignment in bytes of the managed objects.
- **value** `template<typename Type> const Type& value() const` (L174): Returns a const reference to the contained value.
  `Type` Type of the contained value.
- **value** `template<typename Type> Type& value()` (L185): Returns a reference to the contained value.
  `Type` Type of the contained value.
- **value_or** `template<typename Type> Type value_or(const Type& default_value) const` (L198): Returns a copy of the contained value if this iany has one; otherwise it returns a copy of the given default value.
  `Type` Type of the contained value.
- **value_or** `template<typename Type> Type value_or(Type&& default_value) const` (L209): Returns a copy of the contained value if this iany has one; otherwise it returns a moved copy of the given default value.
  `Type` Type of the contained value.
- **emplace** `template<typename Type, typename... Args> Type& emplace(Args&&... args)` (L222): Constructs the contained value in-place.
  `Type` Type of the value to construct.; `Args` Types of the parameters of the value to construct.; `args` Parameters of the value to construct.; devuelve: Reference to the constructed value.
- **reset** `void reset()` (L240): Disposes the contained value.
- **swap** `void swap(iany& other)` (L255): Exchanges the contents of this iany with those of the other one.
  `other` iany to exchange the contents with.
- **swap** `friend void swap(iany& a, iany& b)` (L282): Exchanges the contents of a iany with those of another one.
  `a` First iany to exchange the contents with.; `b` Second iany to exchange the contents with.
- **any** `any() : iany(_storage_buffer, MaxSize, MaxAlignment)` (L450): Default constructor.
- **any** `any(const any& other) : any()` (L459): Copy constructor.
  `other` any to copy.
- **any** `any(any&& other) noexcept : any()` (L469): Move constructor.
  `other` any to move.
- **any** `any(const iany& other) : any()` (L479): Copy constructor.
  `other` iany to copy.
- **any** `any(iany&& other) noexcept : any()` (L489): Move constructor.
  `other` iany to move.
- **any** `template<typename Type> explicit any(const Type& value) : any()` (L499): Constructor.
  `value` Constant value to copy.
- **any** `template<typename Type> explicit any(Type& value) : any()` (L514): Constructor.
  `value` Non constant value to copy.
- **any** `template<typename Type> explicit any(Type&& value) : any()` (L529): Constructor.
  `value` Value to move.
- **operator=** `any& operator=(const any& other)` (L545): Copy assignment operator.
  `other` any to copy.; devuelve: Reference to this.
- **operator=** `any& operator=(any&& other) noexcept` (L561): Move assignment operator.
  `other` any to move.; devuelve: Reference to this.
- **operator=** `any& operator=(const iany& other)` (L577): Copy assignment operator.
  `other` iany to copy.; devuelve: Reference to this.
- **operator=** `any& operator=(iany&& other) noexcept` (L593): Move assignment operator.
  `other` iany to move.; devuelve: Reference to this.
- **operator=** `template<typename Type> any& operator=(const Type& value)` (L609): Assignment operator.
  `value` Constant value to copy.; devuelve: Reference to this.
- **operator=** `template<typename Type> any& operator=(Type& value)` (L626): Assignment operator.
  `value` Non constant value to copy.; devuelve: Reference to this.
- **operator=** `template<typename Type> any& operator=(Type&& value)` (L643): Assignment operator.
  `value` Value to move.; devuelve: Reference to this.
- **emplace** `template<typename Type, typename... Args> Type& emplace(Args&&... args)` (L662): Constructs the contained value in-place.
  `Type` Type of the value to construct.; `Args` Types of the parameters of the value to construct.; `args` Parameters of the value to construct.; devuelve: Reference to the constructed value.
- **any_cast** `template<typename Type> const Type* any_cast(const iany* any)` (L693): Performs type-safe access to the contained object of the given iany.
  `Type` Type of the contained value.; `any` Target iany object.; devuelve: If the given iany pointer is not null, and the type_id_t of the requested Type matches the type_id_t of the given iany, a const pointer to the value contained by operand; otherwise `nullptr`.
- **any_cast** `template<typename Type> Type* any_cast(iany* any)` (L714): Performs type-safe access to the contained object of the given iany.
  `Type` Type of the contained value.; `any` Target iany object.; devuelve: If the given iany pointer is not null, and the type_id_t of the requested Type matches the type_id_t of the given iany, a pointer to the value contained by operand; otherwise `nullptr`.
- **sizeof** `template<typename Type, typename... Args> any<sizeof(Type), alignof(Type)> make_any(Args&&... args)` (L734): Creates an any deducing its size and alignment from the given Type.
  `Type` Type of the value to construct.; `Args` Types of the parameters of the value to construct.; `args` Parameters of the value to construct.; devuelve: any containing to the constructed value.

## bn_any_fwd.h

### `bn::any` (class, bn_any_fwd.h:32)

`std::any` like container with a fixed size buffer.

`template<int MaxSize, int MaxAlignment = alignof(int)> class any`

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

