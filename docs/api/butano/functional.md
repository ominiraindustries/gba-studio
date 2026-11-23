# Butano 21.10.1 — Functional (`functional`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [std](std.md)

## bn_functional.h

### `bn::equal_to` (class, bn_functional.h:27)

Function object for performing comparisons using the equal operator.

`template<typename Type> class equal_to`

- **operator()** `constexpr bool operator()(const Type& a, const Type& b) const` (L35): Indicates if a is equal to b or not.
### `bn::less` (class, bn_functional.h:46)

Function object for performing comparisons using the less than operator.

`template<typename Type> class less`

- **operator()** `constexpr bool operator()(const Type& a, const Type& b) const` (L54): Indicates if a is less than b or not.
### `bn::hash` (struct, bn_functional.h:65)

Hash function object.

`template<typename Type> struct hash`

- **requires** `template<typename EnumType> requires(is_enum_v<EnumType>) struct hash<EnumType>` (L231): Hash support for enums.
- **hash_combine** `template<typename Type> constexpr void hash_combine(const Type& value, unsigned& result)` (L251): Combines the hash of the given value with another hash.
  `value` result hash is combined with the hash of this value.; `result` The hash of the given value is combined with this hash, and stored in it.
- **array_hash** `constexpr unsigned array_hash(const void* ptr, int size)` (L267): Calculates the hash of the given plain data array.
  `ptr` Start position of the plain data array.; `size` Size in bytes of the plain data array.
- **make_hash** `template<typename Type> constexpr unsigned make_hash(const Type& value)` (L428): Returns the hash of the given value.
