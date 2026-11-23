# Butano 21.10.1 — Vector (`vector`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_vector.h

- **~ivector** `~ivector() noexcept = default` (L49): Destructor.
- **~ivector** `~ivector() noexcept requires(! is_trivially_destructible_v<Type>)` (L54): Destructor.
- **operator=** `ivector& operator=(const ivector& other)` (L65): Copy assignment operator.
  `other` ivector to copy.; devuelve: Reference to this.
- **operator=** `ivector& operator=(ivector&& other) noexcept` (L83): Move assignment operator.
  `other` ivector to move.; devuelve: Reference to this.
- **data** `const_pointer data() const` (L99): Returns a const pointer to the beginning of the ivector data.
- **data** `pointer data()` (L107): Returns a pointer to the beginning of the ivector data.
- **size** `size_type size() const` (L115): Returns the current elements count.
- **max_size** `size_type max_size() const` (L123): Returns the maximum possible elements count.
- **available** `size_type available() const` (L131): Returns the remaining element capacity.
- **empty** `bool empty() const` (L139): Indicates if it doesn't contain any element.
- **full** `bool full() const` (L147): Indicates if it can't contain any more elements.
- **begin** `const_iterator begin() const` (L155): Returns a const iterator to the beginning of the ivector.
- **begin** `iterator begin()` (L163): Returns an iterator to the beginning of the ivector.
- **end** `const_iterator end() const` (L171): Returns a const iterator to the end of the ivector.
- **end** `iterator end()` (L179): Returns an iterator to the end of the ivector.
- **cbegin** `const_iterator cbegin() const` (L187): Returns a const iterator to the beginning of the ivector.
- **cend** `const_iterator cend() const` (L195): Returns a const iterator to the end of the ivector.
- **rbegin** `const_reverse_iterator rbegin() const` (L203): Returns a const reverse iterator to the end of the ivector.
- **rbegin** `reverse_iterator rbegin()` (L211): Returns a reverse iterator to the end of the ivector.
- **rend** `const_reverse_iterator rend() const` (L219): Returns a const reverse iterator to the beginning of the ivector.
- **rend** `reverse_iterator rend()` (L227): Returns a reverse iterator to the beginning of the ivector.
- **crbegin** `const_reverse_iterator crbegin() const` (L235): Returns a const reverse iterator to the end of the ivector.
- **crend** `const_reverse_iterator crend() const` (L243): Returns a const reverse iterator to the beginning of the ivector.
- **operator[]** `const_reference operator[](size_type index) const` (L251): Returns a const reference to the value stored at the specified index.
- **operator[]** `reference operator[](size_type index)` (L261): Returns a reference to the value stored at the specified index.
- **at** `const_reference at(size_type index) const` (L271): Returns a const reference to the value stored at the specified index.
- **at** `reference at(size_type index)` (L281): Returns a reference to the value stored at the specified index.
- **front** `const_reference front() const` (L291): Returns a const reference to the first element.
- **front** `reference front()` (L301): Returns a reference to the first element.
- **back** `const_reference back() const` (L311): Returns a const reference to the last element.
- **back** `reference back()` (L321): Returns a reference to the last element.
- **push_back** `void push_back(const_reference value)` (L332): Inserts a copy of a value at the end of the ivector.
  `value` Value to insert.
- **push_back** `void push_back(value_type&& value)` (L344): Inserts a moved value at the end of the ivector.
  `value` Value to insert.
- **emplace_back** `template<typename... Args> reference emplace_back(Args&&... args)` (L357): Constructs and inserts a value at the end of the ivector.
  `args` Parameters of the value to insert.; devuelve: Reference to the new value.
- **pop_back** `void pop_back()` (L371): Removes the last element of the ivector.
- **insert** `iterator insert(const_iterator position, const_reference value)` (L385): Inserts a copy of a value at the specified position.
  `position` The given value is inserted before this position.; `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **insert** `iterator insert(const_iterator position, value_type&& value)` (L410): Inserts a moved value at the specified position.
  `position` The given value is inserted before this position.; `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **insert** `iterator insert(const_iterator position, size_type count, const_reference value)` (L436): Inserts values at the specified position.
  `position` The given value is inserted before this position.; `count` Number of times to insert the given value.; `value` Value to insert.; devuelve: Iterator pointing to the first inserted value.
- **insert** `template<typename Iterator> iterator insert(const_iterator position, const Iterator& first, const Iterator& last)` (L482): Inserts values at the specified position.
  `position` The given values are inserted before this position.; `first` Iterator to the first element to insert.; `last` Iterator to the last element to insert.; devuelve: Iterator pointing to the first inserted value.
- **emplace** `template<typename... Args> iterator emplace(const_iterator position, Args&&... args)` (L531): Constructs and inserts a value at the specified position.
  `position` The new value is inserted before this position.; `args` Parameters of the value to insert.; devuelve: Iterator pointing to the new value.
- **erase** `iterator erase(const_iterator position)` (L556): Erases an element.
  `position` Iterator to the element to erase.; devuelve: Iterator following the erased element.
- **erase** `iterator erase(const_iterator first, const_iterator last)` (L588): Erases a range of elements.
  `first` Iterator to the first element to erase.; `last` Iterator to the last element to erase.; devuelve: Iterator following the last erased element.
  The range includes all the elements between first and last, including the
  element pointed by first, but not the one pointed by last.
- **resize** `void resize(size_type count)` (L631): Resizes the ivector.
  `count` New size.
- **resize** `void resize(size_type count, const_reference value)` (L660): Resizes the ivector.
  `count` New size.; `value` Value to fill new elements with.
- **shrink** `void shrink(size_type count)` (L688): Resizes the ivector to a size less or equal than the previous one.
  `count` New size.
- **assign** `void assign(size_type count, const_reference value)` (L707): Assigns values to the ivector, removing the previous ones.
  `count` Number of elements to insert.; `value` Value to fill new elements with.
- **assign** `template<typename Iterator> void assign(const Iterator& first, const Iterator& last)` (L726): Assigns values to the ivector, removing the previous ones.
  `first` Iterator to the first element to insert.; `last` Iterator following to the last element to insert.
- **clear** `void clear()` (L746): Removes all elements.
- **clear** `void clear() requires(! is_trivially_destructible_v<Type>)` (L754): Removes all elements.
- **swap** `void swap(ivector& other)` (L771): Exchanges the contents of this ivector with those of the other one.
  `other` ivector to exchange the contents with.
- **swap** `friend void swap(ivector& a, ivector& b)` (L818): Exchanges the contents of a ivector with those of another one.
  `a` First ivector to exchange the contents with.; `b` Second ivector to exchange the contents with.
- **operator==** `friend bool operator==(const ivector& a, const ivector& b)` (L829): Equal operator.
  `a` First ivector to compare.; `b` Second ivector to compare.; devuelve: `true` if the first ivector is equal to the second one, otherwise `false`.
- **operator<** `friend bool operator<(const ivector& a, const ivector& b)` (L855): Less than operator.
  `a` First ivector to compare.; `b` Second ivector to compare.; devuelve: `true` if the first ivector is lexicographically less than the second one, otherwise `false`.
- **operator>** `friend bool operator>(const ivector& a, const ivector& b)` (L867): Greater than operator.
  `a` First ivector to compare.; `b` Second ivector to compare.; devuelve: `true` if the first ivector is lexicographically greater than the second one, otherwise `false`.
- **operator<=** `friend bool operator<=(const ivector& a, const ivector& b)` (L879): Less than or equal operator.
  `a` First ivector to compare.; `b` Second ivector to compare.; devuelve: `true` if the first ivector is lexicographically less than or equal to the second one, otherwise `false`.
- **operator>=** `friend bool operator>=(const ivector& a, const ivector& b)` (L891): Greater than or equal operator.
  `a` First ivector to compare.; `b` Second ivector to compare.; devuelve: `true` if the first ivector is lexicographically greater than or equal to the second one, otherwise `false`.
- **vector** `vector() : ivector<Type>(*reinterpret_cast<pointer>(_storage_buffer), MaxSize)` (L985): Default constructor.
- **vector** `vector(const vector& other) : vector()` (L994): Copy constructor.
  `other` vector to copy.
- **vector** `vector(vector&& other) noexcept : vector()` (L1004): Move constructor.
  `other` vector to move.
- **vector** `vector(const ivector<Type>& other) : vector()` (L1014): Copy constructor.
  `other` ivector to copy.
- **vector** `vector(ivector<Type>&& other) noexcept : vector()` (L1026): Move constructor.
  `other` ivector to move.
- **vector** `explicit vector(size_type count) : vector()` (L1038): Size constructor.
  `count` Initial size of the vector.
- **vector** `vector(size_type count, const_reference value) : vector()` (L1051): Size constructor.
  `count` Initial size of the vector.; `value` Value to fill the vector with.
- **operator=** `vector& operator=(const vector& other)` (L1064): Copy assignment operator.
  `other` vector to copy.; devuelve: Reference to this.
- **operator=** `vector& operator=(vector&& other) noexcept` (L1080): Move assignment operator.
  `other` vector to move.; devuelve: Reference to this.
- **operator=** `vector& operator=(const ivector<Type>& other)` (L1096): Copy assignment operator.
  `other` ivector to copy.; devuelve: Reference to this.
- **operator=** `vector& operator=(ivector<Type>&& other) noexcept` (L1114): Move assignment operator.
  `other` ivector to move.; devuelve: Reference to this.
- **erase** `template<typename Type> typename ivector<Type>::size_type erase(ivector<Type>& vector, const Type& value)` (L1140): Erases all elements from a ivector that are equal to the specified value.
  `vector` ivector from which to erase.; `value` Element to erase.; devuelve: Number of erased elements.
- **erase_if** `template<typename Type, class Pred> typename ivector<Type>::size_type erase_if(ivector<Type>& vector, const Pred& pred)` (L1154): Erases all elements from a ivector that satisfy the specified predicate.
  `vector` ivector from which to erase.; `pred` Unary predicate which returns `true` if the element should be erased.; devuelve: Number of erased elements.

## bn_vector_fwd.h

### `bn::ivector` (class, bn_vector_fwd.h:29)

Base class of bn::vector.

`template<typename Type> class ivector`

Can be used as a reference type for all bn::vector containers containing a specific type.

### `bn::vector` (class, bn_vector_fwd.h:42)

`std::vector` like container with a fixed size buffer.

`template<typename Type, int MaxSize> class vector`

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

