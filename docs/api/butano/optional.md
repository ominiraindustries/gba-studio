# Butano 21.10.1 — Optional (`optional`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_optional.h

### `bn::nullopt_t` (class, bn_optional.h:29)

Empty class type used to indicate an optional type with uninitialized state.

`class nullopt_t`

- **nullopt_t** `constexpr explicit nullopt_t(int)` (L36): Default constructor.
- **optional** `constexpr optional()` (L53): Default constructor.
- **optional** `constexpr optional(nullopt_t) : optional()` (L61): Null constructor.
- **optional** `constexpr optional(const optional& other) = default` (L70): Copy constructor.
  `other` optional to copy.
- **optional** `constexpr optional(const optional& other) requires(is_copy_constructible_v<Type> && ! is_trivially_copy_constructible_v<Type>)` (L76): Copy constructor.
  `other` optional to copy.
- **optional** `constexpr optional(optional&& other) noexcept = default` (L91): Move constructor.
  `other` optional to move.
- **optional** `constexpr optional(optional&& other) noexcept requires(is_move_constructible_v<Type> && ! is_trivially_move_constructible_v<Type>)` (L97): Move constructor.
  `other` optional to move.
- **optional** `template<typename OtherType> constexpr optional(const optional<OtherType>& other)` (L112): Copy constructor.
  `other` Different type optional to copy.
- **optional** `template<typename OtherType> constexpr optional(optional<OtherType>&& other) noexcept : _empty(0), _valid(other)` (L127): Move constructor.
  `other` Different type optional to move.
- **optional** `constexpr optional(const Type& value)` (L142): Constructor.
  `value` Value to copy.
- **optional** `constexpr optional(Type&& value)` (L152): Constructor.
  `value` Value to move.
- **~optional** `constexpr ~optional() noexcept` (L161): Destructor.
- **~optional** `constexpr ~optional() noexcept requires(! is_trivially_destructible_v<Type>)` (L168): Destructor.
- **operator=** `constexpr optional& operator=(nullopt_t)` (L181): Null assignment operator.
  devuelve: Reference to this.
- **operator=** `constexpr optional& operator=(const optional& other) = default` (L192): Copy assignment operator.
  `other` optional to copy.; devuelve: Reference to this.
- **operator=** `constexpr optional& operator=(const optional& other) requires(is_copy_assignable_v<Type> && ! is_trivially_copy_assignable_v<Type>)` (L199): Copy assignment operator.
  `other` optional to copy.; devuelve: Reference to this.
- **operator=** `constexpr optional& operator=(optional&& other) noexcept = default` (L227): Move assignment operator.
  `other` optional to move.; devuelve: Reference to this.
- **operator=** `constexpr optional& operator=(optional&& other) noexcept requires(is_move_constructible_v<Type> && ! is_trivially_move_constructible_v<Type>)` (L234): Move assignment operator.
  `other` optional to move.; devuelve: Reference to this.
- **operator=** `template<typename OtherType> constexpr optional& operator=(const optional<OtherType>& other)` (L262): Copy assignment operator.
  `other` Different type optional to copy.; devuelve: Reference to this.
- **operator=** `template<typename OtherType> constexpr optional& operator=(optional<OtherType>&& other) noexcept` (L290): Move assignment operator.
  `other` Different type optional to move.; devuelve: Reference to this.
- **operator=** `constexpr optional& operator=(const Type& value)` (L318): Assignment operator.
  `value` Value to copy.; devuelve: Reference to this.
- **operator=** `constexpr optional& operator=(Type&& value)` (L338): Assignment operator.
  `value` Value to move.; devuelve: Reference to this.
- **has_value** `constexpr bool has_value() const` (L356): Indicates if it contains a value or not.
- **operator bool** `constexpr explicit operator bool() const` (L364): Indicates if it contains a value or not.
- **get** `constexpr const Type* get() const` (L372): Returns a const pointer to the contained value.
- **get** `constexpr Type* get()` (L380): Returns a pointer to the contained value.
- **value** `constexpr const Type& value() const` (L388): Returns a const reference to the contained value.
- **value** `constexpr Type& value()` (L398): Returns a reference to the contained value.
- **value_or** `constexpr Type value_or(const Type& default_value) const` (L409): Returns a copy of the contained value if this optional has one; otherwise it returns a copy of the given default value.
- **value_or** `constexpr Type value_or(Type&& default_value) const` (L418): Returns a copy of the contained value if this optional has one; otherwise it returns a moved copy of the given default value.
- **operator->** `constexpr const Type* operator->() const` (L426): Returns a const pointer to the contained value.
- **operator->** `constexpr Type* operator->()` (L436): Returns a pointer to the contained value.
- **operator*** `constexpr const Type& operator*() const` (L446): Returns a const reference to the contained value.
- **operator*** `constexpr Type& operator*()` (L456): Returns a reference to the contained value.
- **emplace** `template<typename... Args> constexpr Type& emplace(Args&&... args)` (L468): Constructs the contained value in-place.
  `args` Parameters of the value to construct.; devuelve: Reference to the constructed value.
- **reset** `constexpr void reset()` (L487): Disposes the contained value.
- **swap** `constexpr void swap(optional& other)` (L500): Exchanges the contents of this optional with those of the other one.
  `other` optional to exchange the contents with.
- **swap** `constexpr friend void swap(optional& a, optional& b)` (L533): Exchanges the contents of a optional with those of another one.
  `a` First optional to exchange the contents with.; `b` Second optional to exchange the contents with.
- **operator==** `constexpr friend bool operator==(const optional& a, const optional& b)` (L541): Equal operator.
- **operator<** `constexpr friend bool operator<(const optional& a, const optional& b)` (L559): Less than operator.
- **operator>** `constexpr friend bool operator>(const optional& a, const optional& b)` (L577): Greater than operator.
- **operator<=** `constexpr friend bool operator<=(const optional& a, const optional& b)` (L585): Less than or equal operator.
- **operator>=** `constexpr friend bool operator>=(const optional& a, const optional& b)` (L593): Greater than or equal operator.
- **operator==** `constexpr friend bool operator==(const optional& a, nullopt_t)` (L601): Equal operator.
- **operator!=** `constexpr friend bool operator!=(const optional& a, nullopt_t)` (L609): Not equal operator.
- **operator<** `constexpr friend bool operator<(const optional&, nullopt_t)` (L617): Less than operator.
- **operator>** `constexpr friend bool operator>(const optional& a, nullopt_t)` (L625): Greater than operator.
- **operator<=** `constexpr friend bool operator<=(const optional& a, nullopt_t)` (L633): Less than or equal operator.
- **operator>=** `constexpr friend bool operator>=(const optional&, nullopt_t)` (L641): Greater than or equal operator.
- **operator==** `template<typename OtherType> constexpr friend bool operator==(const optional& a, const OtherType& b)` (L649): Equal operator.
- **operator<** `template<typename OtherType> constexpr friend bool operator<(const optional& a, const OtherType& b)` (L658): Less than operator.
- **operator>** `template<typename OtherType> constexpr friend bool operator>(const optional& a, const OtherType& b)` (L667): Greater than operator.
- **operator<=** `template<typename OtherType> constexpr friend bool operator<=(const optional& a, const OtherType& b)` (L676): Less than or equal operator.
- **operator>=** `template<typename OtherType> constexpr friend bool operator>=(const optional& a, const OtherType& b)` (L685): Greater than or equal operator.
- **make_optional** `template<typename Type, class... Args> constexpr optional<Type> make_optional(Args&&... args)` (L713): Constructs an object and wraps it in a optional.
  `Type` Type of the object to construct.; `Args` Type of the arguments of the object to construct.; `args` Parameters of the object to construct.; devuelve: An optional managing the new object.

## bn_optional_fwd.h

### `bn::optional` (class, bn_optional_fwd.h:29)

`std::optional` like container.

`template<typename Type> class optional`

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

