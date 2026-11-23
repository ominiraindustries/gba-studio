# Butano 21.10.1 — Span (`span`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_span.h

- **span** `constexpr span() = default` (L42): Constructs an empty span.
- **span** `constexpr span(pointer ptr, size_type size)` (L49): Constructs a span that is a view over the range [ptr, ptr + size).
  `ptr` Pointer to the first element.; `size` Elements count.
- **span** `constexpr span(pointer first, pointer last)` (L60): Constructs a span that is a view over the range [first, last).
  `first` Pointer to the first element.; `last` Pointer to the element after the last.
- **span** `template<size_type ArraySize> constexpr span(value_type (&array)[ArraySize])` (L72): Constructs a span that is a view over the given array.
- **span** `template<class OtherType, size_type ArraySize> constexpr span(const array<OtherType, ArraySize>& array)` (L82): Constructs a span that is a const view over the given bn::array.
- **span** `template<class OtherType, size_type ArraySize> constexpr span(array<OtherType, ArraySize>& array)` (L92): Constructs a span that is a non-const view over the given bn::array.
- **span** `template<class OtherType> constexpr span(const span<OtherType>& other)` (L102): Constructs a span that is a view over a span of another type.
- **data** `constexpr const_pointer data() const` (L112): Returns a const pointer to the beginning of the referenced data.
- **data** `constexpr pointer data()` (L120): Returns a pointer to the beginning of the referenced data.
- **size** `constexpr size_type size() const` (L128): Returns the number of referenced elements.
- **size_bytes** `constexpr size_type size_bytes() const` (L136): Returns the number of referenced bytes.
- **empty** `constexpr bool empty() const` (L144): Indicates if it doesn't reference any element.
- **begin** `constexpr const_iterator begin() const` (L152): Returns a const iterator to the beginning of the span.
- **begin** `constexpr iterator begin()` (L160): Returns an iterator to the beginning of the span.
- **cbegin** `constexpr const_iterator cbegin() const` (L168): Returns a const iterator to the beginning of the span.
- **end** `constexpr const_iterator end() const` (L176): Returns a const iterator to the end of the span.
- **end** `constexpr iterator end()` (L184): Returns an iterator to the end of the span.
- **cend** `constexpr const_iterator cend() const` (L192): Returns a const iterator to the end of the span.
- **rbegin** `constexpr const_reverse_iterator rbegin() const` (L200): Returns a const reverse iterator to the end of the span.
- **rbegin** `constexpr reverse_iterator rbegin()` (L208): Returns a reverse iterator to the end of the span.
- **crbegin** `constexpr const_reverse_iterator crbegin() const` (L216): Returns a const reverse iterator to the end of the span.
- **rend** `constexpr const_reverse_iterator rend() const` (L224): Returns a const reverse iterator to the begin of the span.
- **rend** `constexpr reverse_iterator rend()` (L232): Returns a reverse iterator to the begin of the span.
- **crend** `constexpr const_reverse_iterator crend() const` (L240): Returns a const reverse iterator to the begin of the span.
- **front** `constexpr const_reference front() const` (L248): Returns a const reference to the first element.
- **front** `constexpr reference front()` (L258): Returns a reference to the first element.
- **back** `constexpr const_reference back() const` (L268): Returns a const reference to the last element.
- **back** `constexpr reference back()` (L278): Returns a reference to the last element.
- **operator[]** `constexpr const_reference operator[](size_type index) const` (L288): Returns a const reference to the value stored at the specified index.
- **operator[]** `constexpr reference operator[](size_type index)` (L298): Returns a reference to the value stored at the specified index.
- **at** `constexpr const_reference at(size_type index) const` (L308): Returns a const reference to the value stored at the specified index.
- **at** `constexpr reference at(size_type index)` (L318): Returns a reference to the value stored at the specified index.
- **first** `constexpr span first(size_type count) const` (L328): Returns a span that is a view over the first count elements of this span.
- **last** `constexpr span last(size_type count) const` (L338): Returns a span that is a view over the last count elements of this span.
- **subspan** `constexpr span subspan(size_type offset) const` (L348): Returns a span that is a view over the elements of this span starting at offset.
- **subspan** `constexpr span subspan(size_type offset, size_type count) const` (L358): Obtains a span that is a view over the count elements of this span starting at offset.
- **swap** `constexpr void swap(span& other)` (L372): Exchanges the contents of this span with those of the other one.
  `other` span to exchange the contents with.
- **swap** `constexpr friend void swap(span& a, span& b)` (L383): Exchanges the contents of a span with those of another one.
  `a` First span to exchange the contents with.; `b` Second span to exchange the contents with.
- **operator==** `constexpr friend bool operator==(const span& a, const span& b)` (L394): Equal operator.
  `a` First span to compare.; `b` Second span to compare.; devuelve: `true` if the first span is equal to the second one, otherwise `false`.
- **operator<** `constexpr friend bool operator<(const span& a, const span& b)` (L421): Less than operator.
  `a` First span to compare.; `b` Second span to compare.; devuelve: `true` if the first span is lexicographically less than the second one, otherwise `false`.
- **operator>** `constexpr friend bool operator>(const span& a, const span& b)` (L433): Greater than operator.
  `a` First span to compare.; `b` Second span to compare.; devuelve: `true` if the first span is lexicographically greater than the second one, otherwise `false`.
- **operator<=** `constexpr friend bool operator<=(const span& a, const span& b)` (L445): Less than or equal operator.
  `a` First span to compare.; `b` Second span to compare.; devuelve: `true` if the first span is lexicographically less than or equal to the second one, otherwise `false`.
- **operator>=** `constexpr friend bool operator>=(const span& a, const span& b)` (L457): Greater than or equal operator.
  `a` First span to compare.; `b` Second span to compare.; devuelve: `true` if the first span is lexicographically greater than or equal to the second one, otherwise `false`.

## bn_span_fwd.h

### `bn::span` (class, bn_span_fwd.h:29)

`std::span` like container.

`template<typename Type> class span`

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

