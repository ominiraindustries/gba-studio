# Butano 21.10.1 — Forward list (`forward_list`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_forward_list.h

### `bn::iforward_list::iterator` (class, bn_forward_list.h:65)

Non const iterator.

`class iterator`

- **iterator** `iterator() = default` (L79): Default constructor.
- **operator++** `iterator& operator++()` (L85): Increments the position.
  devuelve: Reference to this.
- **operator++** `iterator operator++(int)` (L95): Increments the position.
  devuelve: The iterator before being incremented.
- **operator*** `reference operator*() const` (L105): Returns a reference to the pointed value.
- **operator->** `pointer operator->() const` (L113): Returns a pointer to the pointed value.
- **operator==** `friend bool operator==(const iterator& a, const iterator& b) = default` (L121): Default equal operator.
### `bn::iforward_list::const_iterator` (class, bn_forward_list.h:138)

Const iterator.

`class const_iterator`

- **const_iterator** `const_iterator() = default` (L152): Default constructor.
- **const_iterator** `const_iterator(iterator it)` (L158): Constructor.
  `it` Non const iterator.
- **operator++** `const_iterator& operator++()` (L167): Increments the position.
  devuelve: Reference to this.
- **operator++** `const_iterator operator++(int)` (L177): Increments the position.
  devuelve: The iterator before being incremented.
- **operator*** `const_reference operator*() const` (L187): Returns a const reference to the pointed value.
- **operator->** `const_pointer operator->() const` (L195): Returns a const pointer to the pointed value.
- **operator==** `friend bool operator==(const const_iterator& a, const const_iterator& b) = default` (L203): Default equal operator.
- **operator=** `iforward_list& operator=(const iforward_list& other)` (L223): Copy assignment operator.
  `other` iforward_list to copy.; devuelve: Reference to this.
- **operator=** `iforward_list& operator=(iforward_list&& other) noexcept` (L241): Move assignment operator.
  `other` iforward_list to move.; devuelve: Reference to this.
- **size** `size_type size() const` (L257): Returns the current size.
- **max_size** `size_type max_size() const` (L265): Returns the maximum possible size.
- **available** `size_type available() const` (L273): Returns the remaining capacity.
- **empty** `bool empty() const` (L281): Indicates if it doesn't contain any element.
- **full** `bool full() const` (L289): Indicates if it can't contain any more elements.
- **before_begin** `const_iterator before_begin() const` (L297): Returns a const iterator to the position before the beginning of the iforward_list.
- **before_begin** `iterator before_begin()` (L305): Returns a const iterator to the beginning of the iforward_list.
- **begin** `const_iterator begin() const` (L313): Returns a const iterator to the beginning of the iforward_list.
- **begin** `iterator begin()` (L321): Returns an iterator to the beginning of the iforward_list.
- **end** `const_iterator end() const` (L329): Returns a const iterator to the end of the iforward_list.
- **end** `iterator end()` (L337): Returns an iterator to the end of the iforward_list.
- **cbefore_begin** `const_iterator cbefore_begin() const` (L345): Returns a const iterator to the position before the beginning of the iforward_list.
- **cbegin** `const_iterator cbegin() const` (L353): Returns a const iterator to the beginning of the iforward_list.
- **cend** `const_iterator cend() const` (L361): Returns a const iterator to the end of the iforward_list.
- **front** `const_reference front() const` (L369): Returns a const reference to the first element.
- **front** `reference front()` (L379): Returns a reference to the first element.
- **push_front** `void push_front(const_reference value)` (L390): Inserts a copy of a value at the beginning of the iforward_list.
  `value` Value to insert.
- **push_front** `void push_front(value_type&& value)` (L400): Inserts a moved value at the beginning of the iforward_list.
  `value` Value to insert.
- **emplace_front** `template<typename... Args> reference emplace_front(Args&&... args)` (L411): Constructs and inserts a value at the beginning of the iforward_list.
  `args` Parameters of the value to insert.; devuelve: Reference to the new value.
- **pop_front** `void pop_front()` (L422): Removes the first element of the iforward_list.
- **insert_after** `iterator insert_after(const_iterator position, const_reference value)` (L435): Inserts a copy of a value at the specified position.
  `position` The given value is inserted after this position.; `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **insert_after** `iterator insert_after(const_iterator position, value_type&& value)` (L448): Inserts a moved value at the specified position.
  `position` The given value is inserted after this position.; `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **insert_after** `iterator insert_after(const_iterator position, size_type count, const_reference value)` (L462): Inserts values at the specified position.
  `position` The given value is inserted after this position.; `count` Number of times to insert the given value.; `value` Value to insert.; devuelve: Iterator pointing to the last inserted value.
- **insert_after** `template<typename Iterator> iterator insert_after(const_iterator position, const Iterator& first, const Iterator& last)` (L484): Inserts values at the specified position.
  `position` The given values are inserted after this position.; `first` Iterator to the first element to insert.; `last` Iterator to the last element to insert.; devuelve: Iterator pointing to the last inserted value.
- **emplace_after** `template<typename... Args> iterator emplace_after(const_iterator position, Args&&... args)` (L504): Constructs and inserts a value at the specified position.
  `position` The new value is inserted after this position.; `args` Parameters of the value to insert.; devuelve: Iterator pointing to the new value.
- **erase_after** `iterator erase_after(const_iterator position)` (L517): Erases an element.
  `position` Iterator to the previous element to the one to erase.; devuelve: Iterator following the erased element.
- **erase_after** `iterator erase_after(const_iterator first, const_iterator last)` (L537): Erases a range of elements.
  `first` Iterator to the first element to erase.; `last` Iterator to the last element to erase.; devuelve: Iterator following the last erased element.
  The range includes all the elements between first and last, including the
  element pointed by last, but not the one pointed by first.
- **remove** `size_type remove(const_reference value)` (L559): Erases all elements that are equal to the specified value.
  `value` Element to erase.; devuelve: Number of erased elements.
- **remove_if** `template<class Pred> size_type remove_if(const Pred& pred)` (L590): Erases all elements that satisfy the specified predicate.
  `pred` Unary predicate which returns `true` if the element should be erased.; devuelve: Number of erased elements.
- **assign** `void assign(size_type count, const_reference value)` (L622): Assigns values to the iforward_list, removing the previous ones.
  `count` Number of elements to insert.; `value` Value to fill new elements with.
- **assign** `template<typename Iterator> void assign(const Iterator& first, const Iterator& last)` (L641): Assigns values to the iforward_list, removing the previous ones.
  `first` Iterator to the first element to insert.; `last` Iterator following to the last element to insert.
- **clear** `void clear()` (L658): Removes all elements.
- **swap** `void swap(iforward_list& other)` (L677): Exchanges the contents of this iforward_list with those of the other one.
  `other` iforward_list to exchange the contents with.
- **swap** `friend void swap(iforward_list& a, iforward_list& b)` (L732): Exchanges the contents of a iforward_list with those of another one.
  `a` First iforward_list to exchange the contents with.; `b` Second iforward_list to exchange the contents with.
- **operator==** `friend bool operator==(const iforward_list& a, const iforward_list& b)` (L743): Equal operator.
  `a` First iforward_list to compare.; `b` Second iforward_list to compare.; devuelve: `true` if the first iforward_list is equal to the second one, otherwise `false`.
- **operator<** `friend bool operator<(const iforward_list& a, const iforward_list& b)` (L759): Less than operator.
  `a` First iforward_list to compare.; `b` Second iforward_list to compare.; devuelve: `true` if the first iforward_list is lexicographically less than the second one, otherwise `false`.
- **operator>** `friend bool operator>(const iforward_list& a, const iforward_list& b)` (L771): Greater than operator.
  `a` First iforward_list to compare.; `b` Second iforward_list to compare.; devuelve: `true` if the first iforward_list is lexicographically greater than the second one, otherwise `false`.
- **operator<=** `friend bool operator<=(const iforward_list& a, const iforward_list& b)` (L783): Less than or equal operator.
  `a` First iforward_list to compare.; `b` Second iforward_list to compare.; devuelve: `true` if the first iforward_list is lexicographically less than or equal to the second one, otherwise `false`.
- **operator>=** `friend bool operator>=(const iforward_list& a, const iforward_list& b)` (L795): Greater than or equal operator.
  `a` First iforward_list to compare.; `b` Second iforward_list to compare.; devuelve: `true` if the first iforward_list is lexicographically greater than or equal to the second one, otherwise `false`.
- **forward_list** `forward_list() : iforward_list<Type>(_pool)` (L883): Default constructor.
- **forward_list** `forward_list(const forward_list& other) : forward_list()` (L892): Copy constructor.
  `other` forward_list to copy.
- **forward_list** `forward_list(forward_list&& other) noexcept : forward_list()` (L902): Move constructor.
  `other` forward_list to move.
- **forward_list** `forward_list(const iforward_list<Type>& other) : forward_list()` (L912): Copy constructor.
  `other` iforward_list to copy.
- **forward_list** `forward_list(iforward_list<Type>&& other) noexcept : forward_list()` (L924): Move constructor.
  `other` iforward_list to move.
- **forward_list** `forward_list(size_type count) : forward_list()` (L936): Size constructor.
  `count` Initial size of the forward_list.
- **forward_list** `forward_list(size_type count, const_reference value) : forward_list()` (L947): Size constructor.
  `count` Initial size of the forward_list.; `value` Value to fill the forward_list with.
- **operator=** `forward_list& operator=(const forward_list& other)` (L958): Copy assignment operator.
  `other` forward_list to copy.; devuelve: Reference to this.
- **operator=** `forward_list& operator=(forward_list&& other) noexcept` (L974): Move assignment operator.
  `other` forward_list to move.; devuelve: Reference to this.
- **operator=** `forward_list& operator=(const iforward_list<Type>& other)` (L990): Copy assignment operator.
  `other` iforward_list to copy.; devuelve: Reference to this.
- **operator=** `forward_list& operator=(iforward_list<Type>&& other) noexcept` (L1008): Move assignment operator.
  `other` iforward_list to move.; devuelve: Reference to this.
- **~forward_list** `~forward_list() noexcept = default` (L1024): Destructor.
- **~forward_list** `~forward_list() noexcept requires(! is_trivially_destructible_v<Type>)` (L1029): Destructor.
- **erase** `template<typename Type> typename iforward_list<Type>::size_type erase(iforward_list<Type>& list, const Type& value)` (L1046): Erases all elements from a iforward_list that are equal to the specified value.
  `list` iforward_list from which to erase.; `value` Element to erase.; devuelve: Number of erased elements.
- **erase_if** `template<typename Type, class Pred> typename iforward_list<Type>::size_type erase_if(iforward_list<Type>& list, const Pred& pred)` (L1058): Erases all elements from a iforward_list that satisfy the specified predicate.
  `list` iforward_list from which to erase.; `pred` Unary predicate which returns `true` if the element should be erased.; devuelve: Number of erased elements.

## bn_forward_list_fwd.h

### `bn::iforward_list` (class, bn_forward_list_fwd.h:29)

Base class of bn::forward_list.

`template<typename Type> class iforward_list`

Can be used as a reference type for all bn::forward_list containers containing a specific type.

### `bn::forward_list` (class, bn_forward_list_fwd.h:42)

`std::forward_list` like container with a fixed size buffer.

`template<typename Type, int MaxSize> class forward_list`

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

