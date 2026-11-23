# Butano 21.10.1 — Array (`array`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_array.h

- **data** `constexpr const_pointer data() const` (L47): Returns a const pointer to the beginning of the array.
- **data** `constexpr pointer data()` (L55): Returns a pointer to the beginning of the array.
- **size** `constexpr size_type size() const` (L63): Returns the elements count.
- **max_size** `constexpr size_type max_size() const` (L71): Returns the maximum possible elements count.
- **available** `constexpr size_type available() const` (L79): Returns the remaining element capacity.
- **empty** `constexpr bool empty() const` (L87): Indicates if it doesn't contain any element.
- **full** `constexpr bool full() const` (L95): Indicates if it can't contain any more elements.
- **begin** `constexpr const_iterator begin() const` (L103): Returns a const iterator to the beginning of the array.
- **begin** `constexpr iterator begin()` (L111): Returns an iterator to the beginning of the array.
- **end** `constexpr const_iterator end() const` (L119): Returns a const iterator to the end of the array.
- **end** `constexpr iterator end()` (L127): Returns an iterator to the end of the array.
- **cbegin** `constexpr const_iterator cbegin() const` (L135): Returns a const iterator to the beginning of the array.
- **cend** `constexpr const_iterator cend() const` (L143): Returns a const iterator to the end of the array.
- **rbegin** `constexpr const_reverse_iterator rbegin() const` (L151): Returns a const reverse iterator to the end of the array.
- **rbegin** `constexpr reverse_iterator rbegin()` (L159): Returns a reverse iterator to the end of the array.
- **rend** `constexpr const_reverse_iterator rend() const` (L167): Returns a const reverse iterator to the beginning of the array.
- **rend** `constexpr reverse_iterator rend()` (L175): Returns a reverse iterator to the beginning of the array.
- **crbegin** `constexpr const_reverse_iterator crbegin() const` (L183): Returns a const reverse iterator to the end of the array.
- **crend** `constexpr const_reverse_iterator crend() const` (L191): Returns a const reverse iterator to the beginning of the array.
- **operator[]** `constexpr const_reference operator[](size_type index) const` (L199): Returns a const reference to the value stored at the specified index.
- **operator[]** `constexpr reference operator[](size_type index)` (L209): Returns a reference to the value stored at the specified index.
- **at** `constexpr const_reference at(size_type index) const` (L219): Returns a const reference to the value stored at the specified index.
- **at** `constexpr reference at(size_type index)` (L229): Returns a reference to the value stored at the specified index.
- **front** `constexpr const_reference front() const` (L239): Returns a const reference to the first element.
- **front** `constexpr reference front()` (L247): Returns a reference to the first element.
- **back** `constexpr const_reference back() const` (L255): Returns a const reference to the last element.
- **back** `constexpr reference back()` (L263): Returns a reference to the last element.
- **fill** `constexpr void fill(const_reference value)` (L271): Fills the array with the given value.
- **swap** `constexpr void swap(array& other)` (L280): Exchanges the contents of this array with those of the other one.
  `other` array to exchange the contents with.
- **swap** `constexpr friend void swap(array& a, array& b)` (L296): Exchanges the contents of an array with those of another one.
  `a` First array to exchange the contents with.; `b` Second array to exchange the contents with.
- **operator==** `constexpr friend bool operator==(const array& a, const array& b)` (L307): Equal operator.
  `a` First array to compare.; `b` Second array to compare.; devuelve: `true` if the first array is equal to the second one, otherwise `false`.
- **operator<** `constexpr friend bool operator<(const array& a, const array& b)` (L323): Less than operator.
  `a` First array to compare.; `b` Second array to compare.; devuelve: `true` if the first array is lexicographically less than the second one, otherwise `false`.
- **operator>** `constexpr friend bool operator>(const array& a, const array& b)` (L335): Greater than operator.
  `a` First array to compare.; `b` Second array to compare.; devuelve: `true` if the first array is lexicographically greater than the second one, otherwise `false`.
- **operator<=** `constexpr friend bool operator<=(const array& a, const array& b)` (L347): Less than or equal operator.
  `a` First array to compare.; `b` Second array to compare.; devuelve: `true` if the first array is lexicographically less than or equal to the second one, otherwise `false`.
- **operator>=** `constexpr friend bool operator>=(const array& a, const array& b)` (L359): Greater than or equal operator.
  `a` First array to compare.; `b` Second array to compare.; devuelve: `true` if the first array is lexicographically greater than or equal to the second one, otherwise `false`.
- **to_array** `template<typename Type, int Size> constexpr array<remove_cv_t<Type>, Size> to_array(Type (&base_array)[Size])` (L657): Creates a bn::array object from the given built-in array.
- **to_array** `template<typename Type, int Size> constexpr array<remove_cv_t<Type>, Size> to_array(Type (&&base_array)[Size])` (L668): Creates a bn::array object from the given built-in array.

## bn_array_fwd.h

### `bn::array` (class, bn_array_fwd.h:30)

`std::array` like container.

`template<typename Type, int Size> class array`

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

