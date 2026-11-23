# Butano 21.10.1 — Intrusive forward list (`intrusive_forward_list`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_intrusive_forward_list.h

### `bn::intrusive_forward_list_node_type` (class, bn_intrusive_forward_list.h:30)

To be part of an intrusive_forward_list, values must inherit this class.

`class intrusive_forward_list_node_type`

### `bn::intrusive_forward_list::iterator` (class, bn_intrusive_forward_list.h:62)

Non const iterator.

`class iterator`

- **iterator** `iterator() = default` (L76): Default constructor.
- **iterator** `explicit iterator(node_type* node)` (L82): Constructor.
  `node` Intrusive forward list node.
- **operator++** `iterator& operator++()` (L91): Increments the position.
  devuelve: Reference to this.
- **operator++** `iterator operator++(int)` (L101): Increments the position.
  devuelve: The iterator before being incremented.
- **operator*** `reference operator*() const` (L111): Returns a reference to the pointed value.
- **operator->** `pointer operator->() const` (L119): Returns a pointer to the pointed value.
- **operator==** `friend bool operator==(const iterator& a, const iterator& b) = default` (L127): Default equal operator.
### `bn::intrusive_forward_list::const_iterator` (class, bn_intrusive_forward_list.h:139)

Const iterator.

`class const_iterator`

- **const_iterator** `const_iterator() = default` (L153): Default constructor.
- **const_iterator** `explicit const_iterator(const node_type* node)` (L159): Constructor.
  `node` Intrusive forward list node.
- **const_iterator** `const_iterator(iterator it)` (L168): Constructor.
  `it` Non const iterator.
- **operator++** `const_iterator& operator++()` (L177): Increments the position.
  devuelve: Reference to this.
- **operator++** `const_iterator operator++(int)` (L187): Increments the position.
  devuelve: The iterator before being incremented.
- **operator*** `const_reference operator*() const` (L197): Returns a const reference to the pointed value.
- **operator->** `const_pointer operator->() const` (L205): Returns a const pointer to the pointed value.
- **operator==** `friend bool operator==(const const_iterator& a, const const_iterator& b) = default` (L213): Default equal operator.
- **intrusive_forward_list** `intrusive_forward_list() = default` (L224): Default constructor.
- **intrusive_forward_list** `intrusive_forward_list(intrusive_forward_list&& other) noexcept : intrusive_forward_list()` (L232): Move constructor.
  `other` intrusive_forward_list to move.
- **operator=** `intrusive_forward_list& operator=(intrusive_forward_list&& other) noexcept` (L245): Move assignment operator.
  `other` intrusive_forward_list to move.; devuelve: Reference to this.
- **~intrusive_forward_list** `~intrusive_forward_list() noexcept` (L259): Destructor.
- **size** `size_type size() const` (L267): Returns the current size.
- **empty** `bool empty() const` (L275): Indicates if it doesn't contain any element.
- **before_begin** `const_iterator before_begin() const` (L283): Returns a const iterator to the position before the beginning of the intrusive_forward_list.
- **before_begin** `iterator before_begin()` (L291): Returns an iterator to the position before the beginning of the intrusive_forward_list.
- **begin** `const_iterator begin() const` (L299): Returns a const iterator to the beginning of the intrusive_forward_list.
- **begin** `iterator begin()` (L307): Returns an iterator to the beginning of the intrusive_forward_list.
- **end** `const_iterator end() const` (L315): Returns a const iterator to the end of the intrusive_forward_list.
- **end** `iterator end()` (L323): Returns an iterator to the end of the intrusive_forward_list.
- **cbefore_begin** `const_iterator cbefore_begin() const` (L331): Returns a const iterator to the position before the beginning of the intrusive_forward_list.
- **cbegin** `const_iterator cbegin() const` (L339): Returns a const iterator to the beginning of the intrusive_forward_list.
- **cend** `const_iterator cend() const` (L347): Returns a const iterator to the end of the intrusive_forward_list.
- **front** `const_reference front() const` (L355): Returns a const reference to the first element.
- **front** `reference front()` (L365): Returns a reference to the first element.
- **push_front** `void push_front(reference value)` (L376): Inserts a value at the beginning of the intrusive_forward_list.
  `value` Value to insert.
- **pop_front** `void pop_front()` (L384): Removes the first element of the intrusive_forward_list.
- **insert_after** `iterator insert_after(const_iterator position, reference value)` (L397): Inserts a value at the specified position.
  `position` The given value is inserted after this position.; `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **insert_after** `iterator insert_after(reference position_value, reference value)` (L410): Inserts a value at the specified position.
  `position_value` The given value is inserted after the position of this value in the intrusive_forward_list.; `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **erase_after** `iterator erase_after(const_iterator position)` (L421): Erases an element.
  `position` Iterator to the previous element to the one to erase.; devuelve: Iterator following the erased element.
- **erase_after** `iterator erase_after(reference value)` (L436): Erases an element.
  `value` Previous element to the one to erase.; devuelve: Iterator following the erased element.
- **erase_after** `iterator erase_after(const_iterator first, const_iterator last)` (L451): Erases a range of elements.
  `first` Iterator to the first element to erase.; `last` Iterator to the last element to erase.; devuelve: Iterator following the last erased element.
  The range includes all the elements between first and last, including the
  element pointed by last, but not the one pointed by first.
- **remove** `size_type remove(const_reference value)` (L473): Erases all elements that are equal to the specified value.
  `value` Element to erase.; devuelve: Number of erased elements.
- **remove_if** `template<class Pred> size_type remove_if(const Pred& pred)` (L504): Erases all elements that satisfy the specified predicate.
  `pred` Unary predicate which returns `true` if the element should be erased.; devuelve: Number of erased elements.
- **assign** `template<typename Iterator> void assign(const Iterator& first, const Iterator& last)` (L536): Assigns values to the intrusive_forward_list, removing the previous ones.
  `first` Iterator to the first element to insert.; `last` Iterator following to the last element to insert.
- **clear** `void clear()` (L553): Removes all elements.
- **swap** `void swap(intrusive_forward_list& other)` (L573): Exchanges the contents of this intrusive_forward_list with those of the other one.
  `other` intrusive_forward_list to exchange the contents with.
- **swap** `friend void swap(intrusive_forward_list& a, intrusive_forward_list& b)` (L631): Exchanges the contents of a intrusive_forward_list with those of another one.
  `a` First intrusive_forward_list to exchange the contents with.; `b` Second intrusive_forward_list to exchange the contents with.
- **operator==** `friend bool operator==(const intrusive_forward_list& a, const intrusive_forward_list& b)` (L642): Equal operator.
  `a` First intrusive_forward_list to compare.; `b` Second intrusive_forward_list to compare.; devuelve: `true` if the first intrusive_forward_list is equal to the second one, otherwise `false`.
- **operator<** `friend bool operator<(const intrusive_forward_list& a, const intrusive_forward_list& b)` (L659): Less than operator.
  `a` First intrusive_forward_list to compare.; `b` Second intrusive_forward_list to compare.; devuelve: `true` if the first intrusive_forward_list is lexicographically less than the second one, otherwise `false`.
- **operator>** `friend bool operator>(const intrusive_forward_list& a, const intrusive_forward_list& b)` (L671): Greater than operator.
  `a` First intrusive_forward_list to compare.; `b` Second intrusive_forward_list to compare.; devuelve: `true` if the first intrusive_forward_list is lexicographically greater than the second one, otherwise `false`.
- **operator<=** `friend bool operator<=(const intrusive_forward_list& a, const intrusive_forward_list& b)` (L683): Less than or equal operator.
  `a` First intrusive_forward_list to compare.; `b` Second intrusive_forward_list to compare.; devuelve: `true` if the first intrusive_forward_list is lexicographically less than or equal to the second one, otherwise `false`.
- **operator>=** `friend bool operator>=(const intrusive_forward_list& a, const intrusive_forward_list& b)` (L695): Greater than or equal operator.
  `a` First intrusive_forward_list to compare.; `b` Second intrusive_forward_list to compare.; devuelve: `true` if the first intrusive_forward_list is lexicographically greater than or equal to the second one, otherwise `false`.
- **erase** `template<typename Type> typename intrusive_forward_list<Type>::size_type erase(intrusive_forward_list<Type>& list, const Type& value)` (L753): Erases all elements from a intrusive_forward_list that are equal to the specified value.
  `list` intrusive_forward_list from which to erase.; `value` Element to erase.; devuelve: Number of erased elements.
- **erase_if** `template<typename Type, class Pred> typename intrusive_forward_list<Type>::size_type erase_if(intrusive_forward_list<Type>& list, const Pred& pred)` (L765): Erases all elements from a intrusive_forward_list that satisfy the specified predicate.
  `list` intrusive_forward_list from which to erase.; `pred` Unary predicate which returns `true` if the element should be erased.; devuelve: Number of erased elements.

## bn_intrusive_forward_list_fwd.h

### `bn::intrusive_forward_list_node_type` (class, bn_intrusive_forward_list_fwd.h:25)

To be part of a bn::intrusive_forward_list, values must inherit this class.

`class intrusive_forward_list_node_type`

### `bn::intrusive_forward_list` (class, bn_intrusive_forward_list_fwd.h:36)

`std::forward_list` like container that doesn't contain values, it just references them.

`template<typename Type> class intrusive_forward_list`

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

