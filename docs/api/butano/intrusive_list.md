# Butano 21.10.1 — Intrusive list (`intrusive_list`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_intrusive_list.h

### `bn::intrusive_list_node_type` (class, bn_intrusive_list.h:31)

To be part of an intrusive_list, values must inherit this class.

`class intrusive_list_node_type`

### `bn::intrusive_list::iterator` (class, bn_intrusive_list.h:64)

Non const iterator.

`class iterator`

- **iterator** `iterator() = default` (L78): Default constructor.
- **iterator** `explicit iterator(node_type* node)` (L84): Constructor.
  `node` Intrusive list node.
- **operator++** `iterator& operator++()` (L93): Increments the position.
  devuelve: Reference to this.
- **operator++** `iterator operator++(int)` (L103): Increments the position.
  devuelve: The iterator before being incremented.
- **operator--** `iterator& operator--()` (L114): Decrements the position.
  devuelve: Reference to this.
- **operator--** `iterator operator--(int)` (L124): Decrements the position.
  devuelve: The iterator before being decremented.
- **operator*** `reference operator*() const` (L134): Returns a reference to the pointed value.
- **operator->** `pointer operator->() const` (L142): Returns a pointer to the pointed value.
- **operator==** `friend bool operator==(const iterator& a, const iterator& b) = default` (L150): Default equal operator.
### `bn::intrusive_list::const_iterator` (class, bn_intrusive_list.h:162)

Const iterator.

`class const_iterator`

- **const_iterator** `const_iterator() = default` (L177): Default constructor.
- **const_iterator** `explicit const_iterator(const node_type* node)` (L183): Constructor.
  `node` Intrusive list node.
- **const_iterator** `const_iterator(iterator it)` (L192): Constructor.
  `it` Non const iterator.
- **operator++** `const_iterator& operator++()` (L201): Increments the position.
  devuelve: Reference to this.
- **operator++** `const_iterator operator++(int)` (L211): Increments the position.
  devuelve: The iterator before being incremented.
- **operator--** `const_iterator& operator--()` (L222): Decrements the position.
  devuelve: Reference to this.
- **operator--** `const_iterator operator--(int)` (L232): Decrements the position.
  devuelve: The iterator before being decremented.
- **operator*** `const_reference operator*() const` (L242): Returns a const reference to the pointed value.
- **operator->** `const_pointer operator->() const` (L250): Returns a const pointer to the pointed value.
- **operator==** `friend bool operator==(const const_iterator& a, const const_iterator& b) = default` (L258): Default equal operator.
- **intrusive_list** `intrusive_list()` (L272): Default constructor.
- **intrusive_list** `intrusive_list(intrusive_list&& other) noexcept : intrusive_list()` (L284): Move constructor.
  `other` intrusive_list to move.
- **operator=** `intrusive_list& operator=(intrusive_list&& other) noexcept` (L297): Move assignment operator.
  `other` intrusive_list to move.; devuelve: Reference to this.
- **~intrusive_list** `~intrusive_list() noexcept` (L311): Destructor.
- **size** `size_type size() const` (L319): Returns the current size.
- **empty** `bool empty() const` (L327): Indicates if it doesn't contain any element.
- **begin** `const_iterator begin() const` (L335): Returns a const iterator to the beginning of the intrusive_list.
- **begin** `iterator begin()` (L343): Returns an iterator to the beginning of the intrusive_list.
- **end** `const_iterator end() const` (L351): Returns a const iterator to the end of the intrusive_list.
- **end** `iterator end()` (L359): Returns an iterator to the end of the intrusive_list.
- **cbegin** `const_iterator cbegin() const` (L367): Returns a const iterator to the beginning of the intrusive_list.
- **cend** `const_iterator cend() const` (L375): Returns a const iterator to the end of the intrusive_list.
- **rbegin** `const_reverse_iterator rbegin() const` (L383): Returns a const reverse iterator to the end of the intrusive_list.
- **rbegin** `reverse_iterator rbegin()` (L391): Returns a reverse iterator to the end of the intrusive_list.
- **rend** `const_reverse_iterator rend() const` (L399): Returns a const reverse iterator to the beginning of the intrusive_list.
- **rend** `reverse_iterator rend()` (L407): Returns a reverse iterator to the beginning of the intrusive_list.
- **crbegin** `const_reverse_iterator crbegin() const` (L415): Returns a const reverse iterator to the end of the intrusive_list.
- **crend** `const_reverse_iterator crend() const` (L423): Returns a const reverse iterator to the beginning of the intrusive_list.
- **front** `const_reference front() const` (L431): Returns a const reference to the first element.
- **front** `reference front()` (L441): Returns a reference to the first element.
- **back** `const_reference back() const` (L451): Returns a const reference to the last element.
- **back** `reference back()` (L461): Returns a reference to the last element.
- **push_front** `void push_front(reference value)` (L472): Inserts a value at the beginning of the intrusive_list.
  `value` Value to insert.
- **push_back** `void push_back(reference value)` (L481): Inserts a value at the end of the intrusive_list.
  `value` Value to insert.
- **pop_front** `void pop_front()` (L489): Removes the first element of the intrusive_list.
- **pop_back** `void pop_back()` (L499): Removes the last element of the intrusive_list.
- **insert** `iterator insert(const_iterator position, reference value)` (L514): Inserts a value at the specified position.
  `position` The given value is inserted before this position.; `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **insert** `iterator insert(reference position_value, reference value)` (L526): Inserts a value at the specified position.
  `position_value` The given value is inserted before the position of this value in the intrusive_list.; `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **erase** `iterator erase(const_iterator position)` (L537): Erases an element.
  `position` Iterator to the element to erase.; devuelve: Iterator following the erased element.
- **erase** `iterator erase(reference value)` (L553): Erases an element.
  `value` Element to erase.; devuelve: Iterator following the erased element.
- **erase** `iterator erase(const_iterator first, const_iterator last)` (L568): Erases a range of elements.
  `first` Iterator to the first element to erase.; `last` Iterator to the last element to erase.; devuelve: Iterator following the last erased element.
  The range includes all the elements between first and last, including the
  element pointed by first, but not the one pointed by last.
- **remove** `size_type remove(const_reference value)` (L592): Erases all elements that are equal to the specified value.
  `value` Element to erase.; devuelve: Number of erased elements.
- **remove_if** `template<class Pred> size_type remove_if(const Pred& pred)` (L622): Erases all elements that satisfy the specified predicate.
  `pred` Unary predicate which returns `true` if the element should be erased.; devuelve: Number of erased elements.
- **assign** `template<typename Iterator> void assign(const Iterator& first, const Iterator& last)` (L653): Assigns values to the intrusive_list, removing the previous ones.
  `first` Iterator to the first element to insert.; `last` Iterator following to the last element to insert.
- **clear** `void clear()` (L669): Removes all elements.
- **swap** `void swap(intrusive_list& other)` (L691): Exchanges the contents of this intrusive_list with those of the other one.
  `other` intrusive_list to exchange the contents with.
- **swap** `friend void swap(intrusive_list& a, intrusive_list& b)` (L743): Exchanges the contents of a intrusive_list with those of another one.
  `a` First intrusive_list to exchange the contents with.; `b` Second intrusive_list to exchange the contents with.
- **operator==** `friend bool operator==(const intrusive_list& a, const intrusive_list& b)` (L754): Equal operator.
  `a` First intrusive_list to compare.; `b` Second intrusive_list to compare.; devuelve: `true` if the first intrusive_list is equal to the second one, otherwise `false`.
- **operator<** `friend bool operator<(const intrusive_list& a, const intrusive_list& b)` (L771): Less than operator.
  `a` First intrusive_list to compare.; `b` Second intrusive_list to compare.; devuelve: `true` if the first intrusive_list is lexicographically less than the second one, otherwise `false`.
- **operator>** `friend bool operator>(const intrusive_list& a, const intrusive_list& b)` (L783): Greater than operator.
  `a` First intrusive_list to compare.; `b` Second intrusive_list to compare.; devuelve: `true` if the first intrusive_list is lexicographically greater than the second one, otherwise `false`.
- **operator<=** `friend bool operator<=(const intrusive_list& a, const intrusive_list& b)` (L795): Less than or equal operator.
  `a` First intrusive_list to compare.; `b` Second intrusive_list to compare.; devuelve: `true` if the first intrusive_list is lexicographically less than or equal to the second one, otherwise `false`.
- **operator>=** `friend bool operator>=(const intrusive_list& a, const intrusive_list& b)` (L807): Greater than or equal operator.
  `a` First intrusive_list to compare.; `b` Second intrusive_list to compare.; devuelve: `true` if the first intrusive_list is lexicographically greater than or equal to the second one, otherwise `false`.
- **erase** `template<typename Type> typename intrusive_list<Type>::size_type erase(intrusive_list<Type>& list, const Type& value)` (L869): Erases all elements from a intrusive_list that are equal to the specified value.
  `list` intrusive_list from which to erase.; `value` Element to erase.; devuelve: Number of erased elements.
- **erase_if** `template<typename Type, class Pred> typename intrusive_list<Type>::size_type erase_if(intrusive_list<Type>& list, const Pred& pred)` (L881): Erases all elements from a intrusive_list that satisfy the specified predicate.
  `list` intrusive_list from which to erase.; `pred` Unary predicate which returns `true` if the element should be erased.; devuelve: Number of erased elements.

## bn_intrusive_list_fwd.h

### `bn::intrusive_list_node_type` (class, bn_intrusive_list_fwd.h:25)

To be part of a bn::intrusive_list, values must inherit this class.

`class intrusive_list_node_type`

### `bn::intrusive_list` (class, bn_intrusive_list_fwd.h:36)

`std::list` like container that doesn't contain values, it just references them.

`template<class Type> class intrusive_list`

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

