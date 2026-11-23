# Butano 21.10.1 — Deque (`deque`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_deque.h

### `bn::ideque::iterator` (class, bn_deque.h:44)

Non const iterator.

`class iterator`

- **iterator** `iterator() = default` (L58): Default constructor.
- **operator++** `iterator& operator++()` (L64): Increments the position.
  devuelve: Reference to this.
- **operator++** `iterator operator++(int)` (L74): Increments the position.
  devuelve: The iterator before being incremented.
- **operator+=** `iterator& operator+=(size_type value)` (L86): Increments the position the given number of times.
  `value` Number of positions to increment.; devuelve: Reference to this.
- **operator--** `iterator& operator--()` (L96): Decrements the position.
  devuelve: Reference to this.
- **operator--** `iterator operator--(int)` (L106): Decrements the position.
  devuelve: The iterator before being decremented.
- **operator-=** `iterator& operator-=(size_type value)` (L118): Decrements the position the given number of times.
  `value` Number of positions to decrement.; devuelve: Reference to this.
- **operator*** `reference operator*() const` (L127): Returns a reference to the pointed value.
- **operator->** `pointer operator->() const` (L135): Returns a pointer to the pointed value.
- **operator[]** `reference operator[](size_type offset) const` (L143): Returns a reference to the pointed value by this iterator incremented offset times.
- **operator+** `friend iterator operator+(const iterator& it, size_type offset)` (L153): Returns the iterator it incremented offset times.
- **operator+** `friend iterator operator+(size_type offset, const iterator& it)` (L161): Returns the iterator it incremented offset times.
- **operator-** `friend iterator operator-(const iterator& it, size_type offset)` (L169): Returns the iterator it decremented offset times.
- **operator-** `friend iterator operator-(size_type offset, const iterator& it)` (L177): Returns the iterator it decremented offset times.
- **operator+** `friend size_type operator+(const iterator& a, const iterator& b)` (L185): Returns the sum of a and b.
- **operator-** `friend size_type operator-(const iterator& a, const iterator& b)` (L193): Returns b subtracted from a.
- **operator==** `friend bool operator==(const iterator& a, const iterator& b)` (L204): Equal operator.
  `a` First iterator to compare.; `b` Second iterator to compare.; devuelve: `true` if the first iterator is equal to the second one, otherwise `false`.
- **operator!=** `friend bool operator!=(const iterator& a, const iterator& b)` (L215): Not equal operator.
  `a` First iterator to compare.; `b` Second iterator to compare.; devuelve: `true` if the first iterator is not equal to the second one, otherwise `false`.
- **operator<** `friend bool operator<(const iterator& a, const iterator& b)` (L227): Less than operator.
  `a` First iterator to compare.; `b` Second iterator to compare.; devuelve: `true` if the first iterator is lexicographically less than the second one, otherwise `false`.
- **operator>** `friend bool operator>(const iterator& a, const iterator& b)` (L239): Greater than operator.
  `a` First iterator to compare.; `b` Second iterator to compare.; devuelve: `true` if the first iterator is lexicographically greater than the second one, otherwise `false`.
- **operator<=** `friend bool operator<=(const iterator& a, const iterator& b)` (L251): Less than or equal operator.
  `a` First iterator to compare.; `b` Second iterator to compare.; devuelve: `true` if the first iterator is lexicographically less than or equal to the second one, otherwise `false`.
- **operator>=** `friend bool operator>=(const iterator& a, const iterator& b)` (L263): Greater than or equal operator.
  `a` First iterator to compare.; `b` Second iterator to compare.; devuelve: `true` if the first iterator is lexicographically greater than or equal to the second one, otherwise `false`.
### `bn::ideque::const_iterator` (class, bn_deque.h:285)

Const iterator.

`class const_iterator`

- **const_iterator** `const_iterator() = default` (L299): Default constructor.
- **const_iterator** `const_iterator(const iterator& it)` (L305): Constructor.
  `it` Non const iterator.
- **operator++** `const_iterator& operator++()` (L315): Increments the position.
  devuelve: Reference to this.
- **operator++** `const_iterator operator++(int)` (L325): Increments the position.
  devuelve: The iterator before being incremented.
- **operator+=** `const_iterator& operator+=(size_type value)` (L337): Increments the position the given number of times.
  `value` Number of positions to increment.; devuelve: Reference to this.
- **operator--** `const_iterator& operator--()` (L347): Decrements the position.
  devuelve: Reference to this.
- **operator--** `const_iterator operator--(int)` (L357): Decrements the position.
  devuelve: The iterator before being decremented.
- **operator-=** `const_iterator& operator-=(size_type value)` (L369): Decrements the position the given number of times.
  `value` Number of positions to decrement.; devuelve: Reference to this.
- **operator*** `const_reference operator*() const` (L378): Returns a const reference to the pointed value.
- **operator->** `const_pointer operator->() const` (L386): Returns a const pointer to the pointed value.
- **operator[]** `const_reference operator[](size_type offset) const` (L394): Returns a const reference to the pointed value by this iterator incremented offset times.
- **operator+** `friend const_iterator operator+(const const_iterator& it, size_type offset)` (L404): Returns the iterator it incremented offset times.
- **operator+** `friend const_iterator operator+(size_type offset, const const_iterator& it)` (L412): Returns the iterator it incremented offset times.
- **operator-** `friend const_iterator operator-(const const_iterator& it, size_type offset)` (L420): Returns the iterator it decremented offset times.
- **operator-** `friend const_iterator operator-(size_type offset, const const_iterator& it)` (L428): Returns the iterator it decremented offset times.
- **operator+** `friend size_type operator+(const const_iterator& a, const const_iterator& b)` (L436): Returns the sum of a and b.
- **operator-** `friend size_type operator-(const const_iterator& a, const const_iterator& b)` (L444): Returns b subtracted from a.
- **operator==** `friend bool operator==(const const_iterator& a, const const_iterator& b)` (L455): Equal operator.
  `a` First const_iterator to compare.; `b` Second const_iterator to compare.; devuelve: `true` if the first const_iterator is equal to the second one, otherwise `false`.
- **operator!=** `friend bool operator!=(const const_iterator& a, const const_iterator& b)` (L466): Not equal operator.
  `a` First const_iterator to compare.; `b` Second const_iterator to compare.; devuelve: `true` if the first const_iterator is not equal to the second one, otherwise `false`.
- **operator<** `friend bool operator<(const const_iterator& a, const const_iterator& b)` (L478): Less than operator.
  `a` First const_iterator to compare.; `b` Second const_iterator to compare.; devuelve: `true` if the first const_iterator is lexicographically less than the second one, otherwise `false`.
- **operator>** `friend bool operator>(const const_iterator& a, const const_iterator& b)` (L490): Greater than operator.
  `a` First const_iterator to compare.; `b` Second const_iterator to compare.; devuelve: `true` if the first const_iterator is lexicographically greater than the second one, otherwise `false`.
- **operator<=** `friend bool operator<=(const const_iterator& a, const const_iterator& b)` (L502): Less than or equal operator.
  `a` First const_iterator to compare.; `b` Second const_iterator to compare.; devuelve: `true` if the first const_iterator is lexicographically less than or equal to the second one, otherwise `false`.
- **operator>=** `friend bool operator>=(const const_iterator& a, const const_iterator& b)` (L514): Greater than or equal operator.
  `a` First const_iterator to compare.; `b` Second const_iterator to compare.; devuelve: `true` if the first const_iterator is lexicographically greater than or equal to the second one, otherwise `false`.
- **~ideque** `~ideque() noexcept = default` (L540): Destructor.
- **~ideque** `~ideque() noexcept requires(! is_trivially_destructible_v<Type>)` (L545): Destructor.
- **operator=** `ideque& operator=(const ideque& other)` (L556): Copy assignment operator.
  `other` ideque to copy.; devuelve: Reference to this.
- **operator=** `ideque& operator=(ideque&& other) noexcept` (L574): Move assignment operator.
  `other` ideque to move.; devuelve: Reference to this.
- **size** `size_type size() const` (L590): Returns the current elements count.
- **max_size** `size_type max_size() const` (L598): Returns the maximum possible elements count.
- **available** `size_type available() const` (L606): Returns the remaining element capacity.
- **empty** `bool empty() const` (L614): Indicates if it doesn't contain any element.
- **full** `bool full() const` (L622): Indicates if it can't contain any more elements.
- **begin** `const_iterator begin() const` (L630): Returns a const iterator to the beginning of the ideque.
- **begin** `iterator begin()` (L638): Returns an iterator to the beginning of the ideque.
- **end** `const_iterator end() const` (L646): Returns a const iterator to the end of the ideque.
- **end** `iterator end()` (L654): Returns an iterator to the end of the ideque.
- **cbegin** `const_iterator cbegin() const` (L662): Returns a const iterator to the beginning of the ideque.
- **cend** `const_iterator cend() const` (L670): Returns a const iterator to the end of the ideque.
- **rbegin** `const_reverse_iterator rbegin() const` (L678): Returns a const reverse iterator to the end of the ideque.
- **rbegin** `reverse_iterator rbegin()` (L686): Returns a reverse iterator to the end of the ideque.
- **rend** `const_reverse_iterator rend() const` (L694): Returns a const reverse iterator to the beginning of the ideque.
- **rend** `reverse_iterator rend()` (L702): Returns a reverse iterator to the beginning of the ideque.
- **crbegin** `const_reverse_iterator crbegin() const` (L710): Returns a const reverse iterator to the end of the ideque.
- **crend** `const_reverse_iterator crend() const` (L718): Returns a const reverse iterator to the beginning of the ideque.
- **operator[]** `const_reference operator[](size_type index) const` (L726): Returns a const reference to the value stored at the specified index.
- **operator[]** `reference operator[](size_type index)` (L736): Returns a reference to the value stored at the specified index.
- **at** `const_reference at(size_type index) const` (L746): Returns a const reference to the value stored at the specified index.
- **at** `reference at(size_type index)` (L756): Returns a reference to the value stored at the specified index.
- **front** `const_reference front() const` (L766): Returns a const reference to the first element.
- **front** `reference front()` (L776): Returns a reference to the first element.
- **back** `const_reference back() const` (L786): Returns a const reference to the last element.
- **back** `reference back()` (L796): Returns a reference to the last element.
- **push_front** `void push_front(const_reference value)` (L807): Inserts a copy of a value at the beginning of the ideque.
  `value` Value to insert.
- **push_front** `void push_front(value_type&& value)` (L819): Inserts a moved value at the beginning of the ideque.
  `value` Value to insert.
- **emplace_front** `template<typename... Args> reference emplace_front(Args&&... args)` (L832): Constructs and inserts a value at the beginning of the ideque.
  `args` Parameters of the value to insert.; devuelve: Reference to the new value.
- **push_back** `void push_back(const_reference value)` (L848): Inserts a copy of a value at the end of the ideque.
  `value` Value to insert.
- **push_back** `void push_back(value_type&& value)` (L860): Inserts a moved value at the end of the ideque.
  `value` Value to insert.
- **emplace_back** `template<typename... Args> reference emplace_back(Args&&... args)` (L873): Constructs and inserts a value at the end of the ideque.
  `args` Parameters of the value to insert.; devuelve: Reference to the new value.
- **pop_front** `void pop_front()` (L887): Removes the first element of the ideque.
- **pop_back** `void pop_back()` (L897): Removes the last element of the ideque.
- **insert** `iterator insert(const const_iterator& position, const_reference value)` (L911): Inserts a copy of a value at the specified position.
  `position` The given value is inserted before this position.; `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **insert** `iterator insert(const const_iterator& position, value_type&& value)` (L948): Inserts a moved value at the specified position.
  `position` The given value is inserted before this position.; `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **emplace** `template<typename... Args> iterator emplace(const const_iterator& position, Args&&... args)` (L985): Constructs and inserts a value at the specified position.
  `position` The new value is inserted before this position.; `args` Parameters of the value to insert.; devuelve: Iterator pointing to the new value.
- **erase** `iterator erase(const const_iterator& position)` (L1022): Erases an element.
  `position` Iterator to the element to erase.; devuelve: Iterator following the erased element.
- **erase** `iterator erase(const const_iterator& first, const const_iterator& last)` (L1063): Erases a range of elements.
  `first` Iterator to the first element to erase.; `last` Iterator to the last element to erase.; devuelve: Iterator following the last erased element.
  The range includes all the elements between first and last, including the
  element pointed by first, but not the one pointed by last.
- **resize** `void resize(size_type count)` (L1119): Resizes the ideque.
  `count` New size.
- **resize** `void resize(size_type count, const_reference value)` (L1148): Resizes the ideque.
  `count` New size.; `value` Value to fill new elements with.
- **shrink** `void shrink(size_type count)` (L1176): Resizes the ideque to a size less or equal than the previous one.
  `count` New size.
- **assign** `void assign(size_type count, const_reference value)` (L1195): Assigns values to the ideque, removing the previous ones.
  `count` Number of elements to insert.; `value` Value to fill new elements with.
- **assign** `template<typename Iterator> void assign(const Iterator& first, const Iterator& last)` (L1214): Assigns values to the ideque, removing the previous ones.
  `first` Iterator to the first element to insert.; `last` Iterator following to the last element to insert.
- **clear** `void clear()` (L1234): Removes all elements.
- **clear** `void clear() requires(! is_trivially_destructible_v<Type>)` (L1243): Removes all elements.
- **swap** `void swap(ideque& other)` (L1261): Exchanges the contents of this ideque with those of the other one.
  `other` ideque to exchange the contents with.
- **swap** `friend void swap(ideque& a, ideque& b)` (L1308): Exchanges the contents of a ideque with those of another one.
  `a` First ideque to exchange the contents with.; `b` Second ideque to exchange the contents with.
- **operator==** `friend bool operator==(const ideque& a, const ideque& b)` (L1319): Equal operator.
  `a` First ideque to compare.; `b` Second ideque to compare.; devuelve: `true` if the first ideque is equal to the second one, otherwise `false`.
- **operator!=** `friend bool operator!=(const ideque& a, const ideque& b)` (L1340): Not equal operator.
  `a` First ideque to compare.; `b` Second ideque to compare.; devuelve: `true` if the first ideque is not equal to the second one, otherwise `false`.
- **operator<** `friend bool operator<(const ideque& a, const ideque& b)` (L1351): Less than operator.
  `a` First ideque to compare.; `b` Second ideque to compare.; devuelve: `true` if the first ideque is lexicographically less than the second one, otherwise `false`.
- **operator>** `friend bool operator>(const ideque& a, const ideque& b)` (L1363): Greater than operator.
  `a` First ideque to compare.; `b` Second ideque to compare.; devuelve: `true` if the first ideque is lexicographically greater than the second one, otherwise `false`.
- **operator<=** `friend bool operator<=(const ideque& a, const ideque& b)` (L1375): Less than or equal operator.
  `a` First ideque to compare.; `b` Second ideque to compare.; devuelve: `true` if the first ideque is lexicographically less than or equal to the second one, otherwise `false`.
- **operator>=** `friend bool operator>=(const ideque& a, const ideque& b)` (L1387): Greater than or equal operator.
  `a` First ideque to compare.; `b` Second ideque to compare.; devuelve: `true` if the first ideque is lexicographically greater than or equal to the second one, otherwise `false`.
- **deque** `deque() : ideque<Type>(*reinterpret_cast<pointer>(_storage_buffer), MaxSize)` (L1517): Default constructor.
- **deque** `deque(const deque& other) : deque()` (L1526): Copy constructor.
  `other` deque to copy.
- **deque** `deque(deque&& other) noexcept : deque()` (L1536): Move constructor.
  `other` deque to move.
- **deque** `deque(const ideque<Type>& other) : deque()` (L1546): Copy constructor.
  `other` ideque to copy.
- **deque** `deque(ideque<Type>&& other) noexcept : deque()` (L1558): Move constructor.
  `other` ideque to move.
- **deque** `explicit deque(size_type count) : deque()` (L1570): Size constructor.
  `count` Initial size of the deque.
- **deque** `deque(size_type count, const_reference value) : deque()` (L1583): Size constructor.
  `count` Initial size of the deque.; `value` Value to fill the deque with.
- **operator=** `deque& operator=(const deque& other)` (L1596): Copy assignment operator.
  `other` deque to copy.; devuelve: Reference to this.
- **operator=** `deque& operator=(deque&& other) noexcept` (L1612): Move assignment operator.
  `other` deque to move.; devuelve: Reference to this.
- **operator=** `deque& operator=(const ideque<Type>& other)` (L1628): Copy assignment operator.
  `other` ideque to copy.; devuelve: Reference to this.
- **operator=** `deque& operator=(ideque<Type>&& other) noexcept` (L1646): Move assignment operator.
  `other` ideque to move.; devuelve: Reference to this.
- **erase** `template<typename Type> typename ideque<Type>::size_type erase(ideque<Type>& deque, const Type& value)` (L1672): Erases all elements from a ideque that are equal to the specified value.
  `deque` ideque from which to erase.; `value` Element to erase.; devuelve: Number of erased elements.
- **erase_if** `template<typename Type, class Pred> typename ideque<Type>::size_type erase_if(ideque<Type>& deque, const Pred& pred)` (L1686): Erases all elements from a ideque that satisfy the specified predicate.
  `deque` ideque from which to erase.; `pred` Unary predicate which returns `true` if the element should be erased.; devuelve: Number of erased elements.

## bn_deque_fwd.h

### `bn::ideque` (class, bn_deque_fwd.h:29)

Base class of bn::deque.

`template<typename Type> class ideque`

Can be used as a reference type for all bn::deque containers containing a specific type.

### `bn::deque` (class, bn_deque_fwd.h:42)

A `std::deque` like container with a fixed size buffer.

`template<typename Type, int MaxSize> class deque`

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

