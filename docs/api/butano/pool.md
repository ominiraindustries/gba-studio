# Butano 21.10.1 — Pool (`pool`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_generic_pool.h

### `bn::igeneric_pool_base` (class, bn_generic_pool.h:52)

Base class of igeneric_pool.

`class igeneric_pool_base`

- **~igeneric_pool_base** `~igeneric_pool_base() noexcept` (L65): Destructor.
  It doesn't destroy its elements, they must be destroyed manually.
- **size** `size_type size() const` (L128): Returns the current elements count.
- **max_size** `size_type max_size() const` (L136): Returns the maximum possible elements count.
- **available** `size_type available() const` (L144): Returns the remaining element capacity.
- **empty** `bool empty() const` (L152): Indicates if it doesn't contain any element.
- **full** `bool full() const` (L160): Indicates if it can't contain any more elements.
- **contains** `template<typename Type> bool contains(const Type& value) const` (L168): Indicates if the given value belongs to the igeneric_pool or not.
- **create** `template<typename Type, typename... Args> Type& create(Args&&... args)` (L183): Constructs a value inside of the igeneric_pool.
  `Type` Type of the value to construct.; `Args` Type of the arguments of the value to construct.; `args` Parameters of the value to construct.; devuelve: Reference to the new value.
- **destroy** `template<typename Type> void destroy(Type& value)` (L196): Destroys the given value, previously allocated with the create method.
- **generic_pool** `generic_pool() : base_type(reinterpret_cast<char*>(&_data_buffer[0]), MaxSize)` (L266): Default constructor.

## bn_generic_pool_fwd.h

### `bn::igeneric_pool` (class, bn_generic_pool_fwd.h:53)

Base class of generic_pool.

`template<int MaxElementSize> class igeneric_pool`

Can be used as a reference type for all generic_pool containers containing a specific maximum size.

### `bn::generic_pool` (class, bn_generic_pool_fwd.h:65)

igeneric_pool implementation that uses a fixed size buffer.

`template<int MaxElementSize, int MaxSize, int MaxElementAlignment> class generic_pool`


## bn_pool.h

- **~ipool** `~ipool() noexcept = default` (L67): Destructor.
- **~ipool** `~ipool() noexcept requires(is_trivially_destructible_v<Type>)` (L72): Destructor.
- **contains** `bool contains(const Type& value) const` (L81): Indicates if the given value belongs to the ipool or not.
- **create** `template<typename... Args> Type& create(Args&&... args)` (L92): Constructs a value inside of the ipool.
  `args` Parameters of the value to construct.; devuelve: Reference to the new value.
- **destroy** `void destroy(Type& value)` (L105): Destroys the given value, previously allocated with the create method.
- **pool** `pool() : base_type(reinterpret_cast<char*>(&_data_buffer[0]), MaxSize)` (L136): Default constructor.

## bn_pool_fwd.h

### `bn::ipool` (class, bn_pool_fwd.h:53)

Base class of bn::pool.

`template<typename Type> class ipool`

Can be used as a reference type for all pool containers containing a specific type.

### `bn::pool` (class, bn_pool_fwd.h:64)

bn::ipool implementation that uses a fixed size buffer.

`template<typename Type, int MaxSize> class pool`

