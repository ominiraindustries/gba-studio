# Butano 21.10.1 — Unordered set (`unordered_set`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_unordered_set.h

### `bn::iunordered_set::iterator` (class, bn_unordered_set.h:48)

Non const iterator.

`class iterator`

In contrast to std::unordered_set::iterator, this one is bidirectional.

- **iterator** `iterator() = default` (L66): Default constructor.
- **operator++** `iterator& operator++()` (L72): Increments the position.
  devuelve: Reference to this.
- **operator++** `iterator operator++(int)` (L97): Increments the position.
  devuelve: The iterator before being incremented.
- **operator--** `iterator& operator--()` (L108): Decrements the position.
  devuelve: Reference to this.
- **operator--** `iterator operator--(int)` (L128): Decrements the position.
  devuelve: The iterator before being decremented.
- **operator*** `reference operator*() const` (L138): Returns a reference to the pointed value.
- **operator->** `pointer operator->() const` (L146): Returns a pointer to the pointed value.
- **operator==** `friend bool operator==(const iterator& a, const iterator& b)` (L157): Equal operator.
  `a` First iterator to compare.; `b` Second iterator to compare.; devuelve: `true` if the first iterator is equal to the second one, otherwise `false`.
- **operator!=** `friend bool operator!=(const iterator& a, const iterator& b)` (L168): Not equal operator.
  `a` First iterator to compare.; `b` Second iterator to compare.; devuelve: `true` if the first iterator is not equal to the second one, otherwise `false`.
### `bn::iunordered_set::const_iterator` (class, bn_unordered_set.h:191)

Const iterator.

`class const_iterator`

In contrast to std::unordered_set::const_iterator, this one is bidirectional.

- **const_iterator** `const_iterator() = default` (L209): Default constructor.
- **const_iterator** `const_iterator(const iterator& it)` (L215): Constructor.
  `it` Non const iterator.
- **operator++** `const_iterator& operator++()` (L225): Increments the position.
  devuelve: Reference to this.
- **operator++** `const_iterator operator++(int)` (L250): Increments the position.
  devuelve: The iterator before being incremented.
- **operator--** `const_iterator& operator--()` (L261): Decrements the position.
  devuelve: Reference to this.
- **operator--** `const_iterator operator--(int)` (L281): Decrements the position.
  devuelve: The iterator before being decremented.
- **operator*** `const_reference operator*() const` (L291): Returns a const reference to the pointed value.
- **operator->** `const_pointer operator->() const` (L299): Returns a const pointer to the pointed value.
- **operator==** `friend bool operator==(const const_iterator& a, const const_iterator& b)` (L310): Equal operator.
  `a` First const_iterator to compare.; `b` Second const_iterator to compare.; devuelve: `true` if the first const_iterator is equal to the second one, otherwise `false`.
- **operator!=** `friend bool operator!=(const const_iterator& a, const const_iterator& b)` (L321): Not equal operator.
  `a` First const_iterator to compare.; `b` Second const_iterator to compare.; devuelve: `true` if the first const_iterator is not equal to the second one, otherwise `false`.
- **~iunordered_set** `~iunordered_set() noexcept = default` (L348): Destructor.
- **~iunordered_set** `~iunordered_set() noexcept requires(! is_trivially_destructible_v<value_type>)` (L353): Destructor.
- **operator=** `iunordered_set& operator=(const iunordered_set& other)` (L378): Copy assignment operator.
  `other` iunordered_set to copy.; devuelve: Reference to this.
- **operator=** `iunordered_set& operator=(iunordered_set&& other) noexcept` (L399): Move assignment operator.
  `other` iunordered_set to move.; devuelve: Reference to this.
  Unlike `std::unordered_set`, it doesn't offer pointer stability.
- **size** `size_type size() const` (L415): Returns the current size.
- **max_size** `size_type max_size() const` (L423): Returns the maximum possible size.
- **available** `size_type available() const` (L431): Returns the remaining capacity.
- **empty** `bool empty() const` (L439): Indicates if it doesn't contain any element.
- **full** `bool full() const` (L447): Indicates if it can't contain any more elements.
- **begin** `const_iterator begin() const` (L455): Returns a const iterator to the beginning of the iunordered_set.
- **begin** `iterator begin()` (L463): Returns an iterator to the beginning of the iunordered_set.
- **end** `const_iterator end() const` (L471): Returns a const iterator to the end of the iunordered_set.
- **end** `iterator end()` (L479): Returns an iterator to the end of the iunordered_set.
- **cbegin** `const_iterator cbegin() const` (L487): Returns a const iterator to the beginning of the iunordered_set.
- **cend** `const_iterator cend() const` (L495): Returns a const iterator to the end of the iunordered_set.
- **rbegin** `const_reverse_iterator rbegin() const` (L503): Returns a const reverse iterator to the end of the iunordered_set.
- **rbegin** `reverse_iterator rbegin()` (L511): Returns a reverse iterator to the end of the iunordered_set.
- **rend** `const_reverse_iterator rend() const` (L519): Returns a const reverse iterator to the beginning of the iunordered_set.
- **rend** `reverse_iterator rend()` (L527): Returns a reverse iterator to the beginning of the iunordered_set.
- **crbegin** `const_reverse_iterator crbegin() const` (L535): Returns a const reverse iterator to the end of the iunordered_set.
- **crend** `const_reverse_iterator crend() const` (L543): Returns a const reverse iterator to the beginning of the iunordered_set.
- **contains** `bool contains(const key_type& key) const` (L553): Indicates if the specified key is contained in this iunordered_set.
  `key` Key to search for.; devuelve: `true` if the specified key is contained in this iunordered_set, otherwise `false`.
- **contains_hash** `bool contains_hash(hash_type key_hash, const key_type& key) const` (L569): Indicates if the specified key is contained in this iunordered_set.
  `key_hash` Hash of the given key to search for.; `key` Key to search for.; devuelve: `true` if the specified key is contained in this iunordered_set, otherwise `false`.
- **count** `size_type count(const key_type& key) const` (L579): Counts the number of keys stored in this iunordered_set are equal to the given one.
  `key` Key to search for.; devuelve: 1 if the specified key is contained in this iunordered_set, otherwise 0.
- **count_hash** `size_type count_hash(hash_type key_hash, const key_type& key) const` (L590): Counts the number of keys stored in this iunordered_set are equal to the given one.
  `key_hash` Hash of the given key to search for.; `key` Key to search for.; devuelve: 1 if the specified key is contained in this iunordered_set, otherwise 0.
- **find** `const_iterator find(const key_type& key) const` (L600): Searches for a given key.
  `key` Key to search for.; devuelve: Const iterator to the key if it exists, otherwise end().
- **find** `iterator find(const key_type& key)` (L610): Searches for a given key.
  `key` Key to search for.; devuelve: Iterator to the key if it exists, otherwise end().
- **find_hash** `const_iterator find_hash(hash_type key_hash, const key_type& key) const` (L626): Searches for a given key.
  `key_hash` Hash of the given key to search for.; `key` Key to search for.; devuelve: Const iterator to the key if it exists, otherwise end().
- **find_hash** `iterator find_hash(hash_type key_hash, const key_type& key)` (L637): Searches for a given key.
  `key_hash` Hash of the given key to search for.; `key` Key to search for.; devuelve: Iterator to the key if it exists, otherwise end().
- **insert** `iterator insert(const value_type& value)` (L670): Inserts a copy of the given value.
  `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **insert** `iterator insert(value_type&& value)` (L680): Inserts a moved value.
  `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **insert_hash** `iterator insert_hash(hash_type value_hash, const value_type& value)` (L691): Inserts a copy of the given value.
  `value_hash` Hash of the value to insert.; `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **insert_hash** `iterator insert_hash(hash_type value_hash, value_type&& value)` (L702): Inserts a moved value.
  `value_hash` Hash of the value to insert.; `value` Value to insert.; devuelve: Iterator pointing to the inserted value.
- **erase** `iterator erase(const const_iterator& position)` (L737): Erases an element.
  `position` Iterator to the element to erase.; devuelve: Iterator following the erased element.
  Unlike `std::unordered_set`, it doesn't offer pointer stability.
- **erase** `bool erase(const key_type& key)` (L856): Erases an element.
  `key` Key to erase.; devuelve: `true` if the elements was erased, otherwise `false`.
  Unlike `std::unordered_set`, it doesn't offer pointer stability.
- **erase_hash** `bool erase_hash(hash_type key_hash, const key_type& key)` (L870): Erases an element.
  `key_hash` Hash of the key to erase.; `key` Key to erase.; devuelve: `true` if the elements was erased, otherwise `false`.
  Unlike `std::unordered_set`, it doesn't offer pointer stability.
- **erase_if** `template<class Pred> size_type erase_if(const Pred& pred)` (L891): Erases all elements that satisfy the specified predicate.
  `pred` Unary predicate which returns ​true if the element should be erased.; devuelve: Number of erased elements.
  Unlike `std::unordered_set`, it doesn't offer pointer stability.
- **merge** `void merge(iunordered_set&& other)` (L929): Moves all elements of the given iunordered_set into this one, leaving the first one empty.
  Unlike `std::unordered_set`, it doesn't offer pointer stability.
- **clear** `void clear()` (L970): Removes all elements.
- **clear** `void clear() requires(! is_trivially_destructible_v<value_type>)` (L985): Removes all elements.
- **swap** `void swap(iunordered_set& other)` (L1017): Exchanges the contents of this iunordered_set with those of the other one.
  `other` iunordered_set to exchange the contents with.
  Unlike `std::unordered_set`, it doesn't offer pointer stability.
- **swap** `friend void swap(iunordered_set& a, iunordered_set& b)` (L1073): Exchanges the contents of a iunordered_set with those of another one.
  `a` First iunordered_set to exchange the contents with.; `b` Second iunordered_set to exchange the contents with.
  Unlike `std::unordered_set`, it doesn't offer pointer stability.
- **operator==** `friend bool operator==(const iunordered_set& a, const iunordered_set& b)` (L1084): Equal operator.
  `a` First iunordered_set to compare.; `b` Second iunordered_set to compare.; devuelve: `true` if the first iunordered_set is equal to the second one, otherwise `false`.
- **operator<** `friend bool operator<(const iunordered_set& a, const iunordered_set& b)` (L1122): Less than operator.
  `a` First iunordered_set to compare.; `b` Second iunordered_set to compare.; devuelve: `true` if the first iunordered_set is lexicographically less than the second one, otherwise `false`.
- **operator>** `friend bool operator>(const iunordered_set& a, const iunordered_set& b)` (L1134): Greater than operator.
  `a` First iunordered_set to compare.; `b` Second iunordered_set to compare.; devuelve: `true` if the first iunordered_set is lexicographically greater than the second one, otherwise `false`.
- **operator<=** `friend bool operator<=(const iunordered_set& a, const iunordered_set& b)` (L1146): Less than or equal operator.
  `a` First iunordered_set to compare.; `b` Second iunordered_set to compare.; devuelve: `true` if the first iunordered_set is lexicographically less than or equal to the second one, otherwise `false`.
- **operator>=** `friend bool operator>=(const iunordered_set& a, const iunordered_set& b)` (L1158): Greater than or equal operator.
  `a` First iunordered_set to compare.; `b` Second iunordered_set to compare.; devuelve: `true` if the first iunordered_set is lexicographically greater than or equal to the second one, otherwise `false`.
- **unordered_set** `unordered_set() : iunordered_set<Key, KeyHash, KeyEqual>(*reinterpret_cast<pointer>(_storage_buffer), *_allocated_buffer, MaxSize)` (L1258): Default constructor.
- **unordered_set** `unordered_set(const unordered_set& other) : unordered_set()` (L1267): Copy constructor.
  `other` unordered_set to copy.
- **unordered_set** `unordered_set(unordered_set&& other) noexcept : unordered_set()` (L1280): Move constructor.
  `other` unordered_set to move.
  Unlike `std::unordered_set`, it doesn't offer pointer stability.
- **unordered_set** `unordered_set(const iunordered_set<Key, KeyHash, KeyEqual>& other) : unordered_set()` (L1290): Copy constructor.
  `other` iunordered_set to copy.
- **unordered_set** `unordered_set(iunordered_set<Key, KeyHash, KeyEqual>&& other) noexcept : unordered_set()` (L1305): Move constructor.
  `other` iunordered_set to move.
  Unlike `std::unordered_set`, it doesn't offer pointer stability.
- **operator=** `unordered_set& operator=(const unordered_set& other)` (L1318): Copy assignment operator.
  `other` unordered_set to copy.; devuelve: Reference to this.
- **operator=** `unordered_set& operator=(unordered_set&& other) noexcept` (L1337): Move assignment operator.
  `other` unordered_set to move.; devuelve: Reference to this.
  Unlike `std::unordered_set`, it doesn't offer pointer stability.
- **operator=** `unordered_set& operator=(const iunordered_set<Key, KeyHash, KeyEqual>& other)` (L1353): Copy assignment operator.
  `other` iunordered_set to copy.; devuelve: Reference to this.
- **operator=** `unordered_set& operator=(iunordered_set<Key, KeyHash, KeyEqual>&& other) noexcept` (L1374): Move assignment operator.
  `other` iunordered_set to move.; devuelve: Reference to this.
  Unlike `std::unordered_set`, it doesn't offer pointer stability.
- **erase_if** `template<typename Key, typename KeyHash, typename KeyEqual, class Pred> typename iunordered_set<Key, KeyHash, KeyEqual>::size_type erase_if( iunordered_set<Key, KeyHash, KeyEqual>& set, const Pred& pred)` (L1404): Erases all elements from a iunordered_set that satisfy the specified predicate.
  `set` iunordered_set from which to erase.; `pred` Unary predicate which returns ​true if the element should be erased.; devuelve: Number of erased elements.
  Unlike `std::unordered_set`, it doesn't offer pointer stability.

## bn_unordered_set_fwd.h

### `bn::iunordered_set` (class, bn_unordered_set_fwd.h:33)

Base class of bn::unordered_set.

`template<typename Key, typename KeyHash = hash<Key>, typename KeyEqual = equal_to<Key>> class iunordered_set`

Can be used as a reference type for all bn::unordered_set containers containing a specific type.

Unlike `std::unordered_set`, it doesn't offer pointer stability when erasing elements.

### `bn::unordered_set` (class, bn_unordered_set_fwd.h:50)

`std::unordered_set` like container with a fixed size buffer.

`template<typename Key, int MaxSize, typename KeyHash = hash<Key>, typename KeyEqual = equal_to<Key>> class unordered_set`

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

Unlike `std::unordered_set`, it doesn't offer pointer stability when erasing elements.

