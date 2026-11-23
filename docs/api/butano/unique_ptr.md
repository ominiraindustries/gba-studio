# Butano 21.10.1 — Unique pointer (`unique_ptr`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_unique_ptr_fwd.h

### `bn::default_delete` (struct, bn_unique_ptr_fwd.h:28)

Default deleter.

`template<typename Type> struct default_delete`

- **default_delete** `constexpr default_delete() = default` (L34): Default constructor.
- **default_delete** `template<typename OtherType> constexpr default_delete(const default_delete<OtherType>&) noexcept` (L39): Copy constructor.
- **operator()** `constexpr void operator()(Type* ptr) const noexcept` (L47): Deletes the object pointed by the given pointer.
### `bn::unique_ptr` (class, bn_unique_ptr_fwd.h:65)

`std::unique_ptr` like container.

`template<typename Type, typename Deleter = default_delete<Type>> class unique_ptr`

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

Unlike most Butano containers, it uses the heap instead of the stack.

