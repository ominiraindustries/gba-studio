# Butano 21.10.1 — List (`list`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_list.h

### `bn::ilist::iterator` (class, bn_list.h:66)

Non const iterator.

`class iterator`

- **iterator** `iterator() = default` (L80): Default constructor.
- **operator++** `iterator& operator++()` (L86): Increments the position.
  devuelve: Reference to this.
- **operator++** `iterator operator++(int)` (L96): Increments the position.
  devuelve: The iterator before being incremented.
- **operator--** `iterator& operator--()` (L107): Decrements the position.
  devuelve: Reference to this.
- **operator--** `iterator operator--(int)` (L117): Decrements the position.
  devuelve: The iterator before being decremented.
- **operator*** `reference operator*() const` (L127): Returns a reference to the pointed value.
- **operator->** `pointer operator->() const` (L135): Returns a pointer to the pointed value.
- **operator==** `friend bool operator==(const iterator& a, const iterator& b) = default` (L143): Default equal operator.
### `bn::ilist::const_iterator` (class, bn_list.h:160)

Const iterator.

`class const_iterator`

- **const_iterator** `const_iterator() = default` (L174): Default constructor.
- **const_iterator** `const_iterator(iterator it)` (L180): Constructor.
  `it` Non const iterator.
- **operator++** `const_iterator& operator++()` (L189): Increments the position.
  devuelve: Reference to this.
- **operator++** `const_iterator operator++(int)` (L199): Increments the position.
  devuelve: The iterator before being incremented.
- **operator--** `const_iterator& operator--()` (L210): Decrements the position.
  devuelve: Reference to this.
- **operator--** `const_iterator operator--(int)` (L220): Decrements the position.
  devuelve: The iterator before being decremented.
- **operator*** `const_reference operator*() const` (L230): Returns a const reference to the pointed value.
- **operator->** `const_pointer operator->() const` (L238): Returns a const pointer to the pointed value.
- **operator==** `friend bool operator==(const const_iterator& a, const const_iterator& b) = default` (L246): Default equal operator.
- **operator=** `ilist& operator=(const ilist& other)` (L269): Copy assignment operator.
  `other` ilist to copy.; devuelve: Reference to this.
- **operator=** `ilist& operator=(ilist&& other) noexcept` (L287): Move assignment operator.
  `other` ilist to move.; devuelve: Reference to this.
- **size** `size_type size() const` (L303): Returns the current size.
- **max_size** `size_type max_size() const` (L311): Returns the maximum possible size.
- **available** `size_type available() const` (L319): Returns the remaining capacity.
- **empty** `bool empty() const` (L327): Indicates if it doesn't contain any element.
- **full** `bool full() const` (L335): Indicates if it can't contain any more elements.
- **begin** `const_iterator begin() const` (L343): Returns a const iterator to the beginning of the ilist.
- **begin** `iterator begin()` (L351): Returns an iterator to the beginning of the ilist.
- **end** `const_iterator end() const` (L359): Returns a const iterator to the end of the ilist.
- **end** `iterator end()` (L367): Returns an iterator to the end of the ilist.
- **cbegin** `const_iterator cbegin() const` (L375): Returns a const iterator to the beginning of the ilist.
- **cend** `const_iterator cend() const` (L383): Returns a const iterator to the end of the ilist.
- **rbegin** `const_reverse_iterator rbegin() const` (L391): Returns a const reverse iterator to the end of the ilist.
- **rbegin** `reverse_iterator rbegin()` (L399): Returns a reverse iterator to the end of the ilist.
- **rend** `const_reverse_iterator rend() const` (L407): Returns a const reverse iterator to the beginning of the ilist.
- **rend** `reverse_iterator rend()` (L415): Returns a reverse iterator to the beginning of the ilist.
- **crbegin** `const_reverse_iterator crbegin() const` (L423): Returns a const reverse iterator to the end of the ilist.
- **crend** `const_reverse_iterator crend() const` (L431): Returns a const reverse iterator to the beginning of the ilist.
- **front** `const_reference front() const` (L439): Returns a const reference to the first element.
- **front** `reference front()` (L449): Returns a reference to the first element.
- **back** `const_reference back() const` (L459): Returns a const reference to the last element.
- **back** `reference back()` (L469): Returns a reference to the last element.
- **push_front** `void push_front(const_reference value)` (L480): Inserts a copy of a value at the beginning of the ilist.
  `value` Value to insert.
- **push_front** `void push_front(value_type&& value)` (L490): Inserts a moved value at the beginning of the ilist.
  `value` Value to insert.
- **emplace_front** `template<typename... Args> reference emplace_front(Args&&... args)` (L501): Constructs and inserts a value at the beginning of the ilist.
  `args` Parameters of the value to insert.; devuelve: Reference to the new value.
- **push_back** `void push_back(const_reference value)` (L513): Inserts a copy of a value at the end of the ilist.
  `value` Value to insert.
- **push_back** `void push_back(value_type&& value)` (L523): Inserts a moved value at the end of the ilist.
  `value` Value to insert.
- **emplace_back** `template<typename... Args> reference emplace_back(Args&&... args)` (L534): Constructs and inserts a value at the end of the ilist.
  `args` Parameters of the value to insert.; devuelve: Reference to the new value.
- **pop_front** `void pop_front()` (L545): Removes the first element of the ilist.
- **pop_back** `void pop_back()` (L555): Removes the last element of the ilist.
- **insert** `iterator insert(const_iterator position, const_reference value)` (L570): Inserts a copy of a value at the specified position.
  `position` The given value is inserted before this position.; `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **insert** `iterator insert(const_iterator position, value_type&& value)` (L583): Inserts a moved value at the specified position.
  `position` The given value is inserted before this position.; `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **insert** `iterator insert(const_iterator position, size_type count, const_reference value)` (L597): Inserts values at the specified position.
  `position` The given value is inserted before this position.; `count` Number of times to insert the given value.; `value` Value to insert.; devuelve: Iterator pointing to the first inserted value.
- **insert** `template<typename Iterator> iterator insert(const_iterator position, const Iterator& first, const Iterator& last)` (L617): Inserts values at the specified position.
  `position` The given values are inserted before this position.; `first` Iterator to the first element to insert.; `last` Iterator to the last element to insert.; devuelve: Iterator pointing to the first inserted value.
- **emplace** `template<typename... Args> iterator emplace(const_iterator position, Args&&... args)` (L634): Constructs and inserts a value at the specified position.
  `position` The new value is inserted before this position.; `args` Parameters of the value to insert.; devuelve: Iterator pointing to the new value.
- **erase** `iterator erase(const_iterator position)` (L647): Erases an element.
  `position` Iterator to the element to erase.; devuelve: Iterator following the erased element.
- **erase** `iterator erase(const_iterator first, const_iterator last)` (L668): Erases a range of elements.
  `first` Iterator to the first element to erase.; `last` Iterator to the last element to erase.; devuelve: Iterator following the last erased element.
  The range includes all the elements between first and last, including the
  element pointed by first, but not the one pointed by last.
- **remove** `size_type remove(const_reference value)` (L692): Erases all elements that are equal to the specified value.
  `value` Element to erase.; devuelve: Number of erased elements.
- **remove_if** `template<class Pred> size_type remove_if(const Pred& pred)` (L722): Erases all elements that satisfy the specified predicate.
  `pred` Unary predicate which returns `true` if the element should be erased.; devuelve: Number of erased elements.
- **assign** `void assign(size_type count, const_reference value)` (L753): Assigns values to the ilist, removing the previous ones.
  `count` Number of elements to insert.; `value` Value to fill new elements with.
- **assign** `template<typename Iterator> void assign(const Iterator& first, const Iterator& last)` (L772): Assigns values to the ilist, removing the previous ones.
  `first` Iterator to the first element to insert.; `last` Iterator following to the last element to insert.
- **clear** `void clear()` (L788): Removes all elements.
- **swap** `void swap(ilist& other)` (L808): Exchanges the contents of this ilist with those of the other one.
  `other` ilist to exchange the contents with.
- **swap** `friend void swap(ilist& a, ilist& b)` (L859): Exchanges the contents of a ilist with those of another one.
  `a` First ilist to exchange the contents with.; `b` Second ilist to exchange the contents with.
- **operator==** `friend bool operator==(const ilist& a, const ilist& b)` (L870): Equal operator.
  `a` First ilist to compare.; `b` Second ilist to compare.; devuelve: `true` if the first ilist is equal to the second one, otherwise `false`.
- **operator<** `friend bool operator<(const ilist& a, const ilist& b)` (L886): Less than operator.
  `a` First ilist to compare.; `b` Second ilist to compare.; devuelve: `true` if the first ilist is lexicographically less than the second one, otherwise `false`.
- **operator>** `friend bool operator>(const ilist& a, const ilist& b)` (L898): Greater than operator.
  `a` First ilist to compare.; `b` Second ilist to compare.; devuelve: `true` if the first ilist is lexicographically greater than the second one, otherwise `false`.
- **operator<=** `friend bool operator<=(const ilist& a, const ilist& b)` (L910): Less than or equal operator.
  `a` First ilist to compare.; `b` Second ilist to compare.; devuelve: `true` if the first ilist is lexicographically less than or equal to the second one, otherwise `false`.
- **operator>=** `friend bool operator>=(const ilist& a, const ilist& b)` (L922): Greater than or equal operator.
  `a` First ilist to compare.; `b` Second ilist to compare.; devuelve: `true` if the first ilist is lexicographically greater than or equal to the second one, otherwise `false`.
- **list** `list() : ilist<Type>(_pool)` (L1016): Default constructor.
- **list** `list(const list& other) : list()` (L1025): Copy constructor.
  `other` list to copy.
- **list** `list(list&& other) noexcept : list()` (L1035): Move constructor.
  `other` list to move.
- **list** `list(const ilist<Type>& other) : list()` (L1045): Copy constructor.
  `other` ilist to copy.
- **list** `list(ilist<Type>&& other) noexcept : list()` (L1057): Move constructor.
  `other` ilist to move.
- **list** `list(size_type count) : list()` (L1069): Size constructor.
  `count` Initial size of the list.
- **list** `list(size_type count, const_reference value) : list()` (L1080): Size constructor.
  `count` Initial size of the list.; `value` Value to fill the list with.
- **operator=** `list& operator=(const list& other)` (L1091): Copy assignment operator.
  `other` list to copy.; devuelve: Reference to this.
- **operator=** `list& operator=(list&& other) noexcept` (L1107): Move assignment operator.
  `other` list to move.; devuelve: Reference to this.
- **operator=** `list& operator=(const ilist<Type>& other)` (L1123): Copy assignment operator.
  `other` ilist to copy.; devuelve: Reference to this.
- **operator=** `list& operator=(ilist<Type>&& other) noexcept` (L1141): Move assignment operator.
  `other` ilist to move.; devuelve: Reference to this.
- **~list** `~list() noexcept = default` (L1157): Destructor.
- **~list** `~list() noexcept requires(! is_trivially_destructible_v<Type>)` (L1162): Destructor.
- **erase** `template<typename Type> typename ilist<Type>::size_type erase(ilist<Type>& list, const Type& value)` (L1179): Erases all elements from a ilist that are equal to the specified value.
  `list` ilist from which to erase.; `value` Element to erase.; devuelve: Number of erased elements.
- **erase_if** `template<typename Type, class Pred> typename ilist<Type>::size_type erase_if(ilist<Type>& list, const Pred& pred)` (L1191): Erases all elements from a ilist that satisfy the specified predicate.
  `list` ilist from which to erase.; `pred` Unary predicate which returns `true` if the element should be erased.; devuelve: Number of erased elements.

## bn_list_fwd.h

### `bn::ilist` (class, bn_list_fwd.h:29)

Base class of bn::list.

`template<typename Type> class ilist`

Can be used as a reference type for all bn::list containers containing a specific type.

### `bn::list` (class, bn_list_fwd.h:42)

`std::list` like container with a fixed size buffer.

`template<typename Type, int MaxSize> class list`

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

