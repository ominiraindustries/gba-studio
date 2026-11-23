# Butano 21.10.1 — Unordered map (`unordered_map`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

Módulo padre: [container](container.md)

## bn_unordered_map.h

### `bn::iunordered_map::iterator` (class, bn_unordered_map.h:49)

Non const iterator.

`class iterator`

In contrast to std::unordered_map::iterator, this one is bidirectional.

- **iterator** `iterator() = default` (L68): Default constructor.
- **operator++** `iterator& operator++()` (L74): Increments the position.
  devuelve: Reference to this.
- **operator++** `iterator operator++(int)` (L99): Increments the position.
  devuelve: The iterator before being incremented.
- **operator--** `iterator& operator--()` (L110): Decrements the position.
  devuelve: Reference to this.
- **operator--** `iterator operator--(int)` (L130): Decrements the position.
  devuelve: The iterator before being decremented.
- **operator*** `reference operator*() const` (L140): Returns a reference to the pointed (Key, Value) pair.
- **operator->** `pointer operator->() const` (L148): Returns a pointer to the pointed (Key, Value) pair.
- **operator==** `friend bool operator==(const iterator& a, const iterator& b)` (L159): Equal operator.
  `a` First iterator to compare.; `b` Second iterator to compare.; devuelve: `true` if the first iterator is equal to the second one, otherwise `false`.
- **operator!=** `friend bool operator!=(const iterator& a, const iterator& b)` (L170): Not equal operator.
  `a` First iterator to compare.; `b` Second iterator to compare.; devuelve: `true` if the first iterator is not equal to the second one, otherwise `false`.
### `bn::iunordered_map::const_iterator` (class, bn_unordered_map.h:193)

Const iterator.

`class const_iterator`

In contrast to std::unordered_map::const_iterator, this one is bidirectional.

- **const_iterator** `const_iterator() = default` (L212): Default constructor.
- **const_iterator** `const_iterator(const iterator& it)` (L218): Constructor.
  `it` Non const iterator.
- **operator++** `const_iterator& operator++()` (L228): Increments the position.
  devuelve: Reference to this.
- **operator++** `const_iterator operator++(int)` (L253): Increments the position.
  devuelve: The iterator before being incremented.
- **operator--** `const_iterator& operator--()` (L264): Decrements the position.
  devuelve: Reference to this.
- **operator--** `const_iterator operator--(int)` (L284): Decrements the position.
  devuelve: The iterator before being decremented.
- **operator*** `const_reference operator*() const` (L294): Returns a const reference to the pointed (Key, Value) pair.
- **operator->** `const_pointer operator->() const` (L302): Returns a const pointer to the pointed (Key, Value) pair.
- **operator==** `friend bool operator==(const const_iterator& a, const const_iterator& b)` (L313): Equal operator.
  `a` First const_iterator to compare.; `b` Second const_iterator to compare.; devuelve: `true` if the first const_iterator is equal to the second one, otherwise `false`.
- **operator!=** `friend bool operator!=(const const_iterator& a, const const_iterator& b)` (L324): Not equal operator.
  `a` First const_iterator to compare.; `b` Second const_iterator to compare.; devuelve: `true` if the first const_iterator is not equal to the second one, otherwise `false`.
- **~iunordered_map** `~iunordered_map() noexcept = default` (L351): Destructor.
- **~iunordered_map** `~iunordered_map() noexcept requires(! is_trivially_destructible_v<value_type>)` (L356): Destructor.
- **operator=** `iunordered_map& operator=(const iunordered_map& other)` (L381): Copy assignment operator.
  `other` iunordered_map to copy.; devuelve: Reference to this.
- **operator=** `iunordered_map& operator=(iunordered_map&& other) noexcept` (L402): Move assignment operator.
  `other` iunordered_map to move.; devuelve: Reference to this.
  Unlike `std::unordered_map`, it doesn't offer pointer stability.
- **size** `size_type size() const` (L418): Returns the current size.
- **max_size** `size_type max_size() const` (L426): Returns the maximum possible size.
- **available** `size_type available() const` (L434): Returns the remaining capacity.
- **empty** `bool empty() const` (L442): Indicates if it doesn't contain any element.
- **full** `bool full() const` (L450): Indicates if it can't contain any more elements.
- **begin** `const_iterator begin() const` (L458): Returns a const iterator to the beginning of the iunordered_map.
- **begin** `iterator begin()` (L466): Returns an iterator to the beginning of the iunordered_map.
- **end** `const_iterator end() const` (L474): Returns a const iterator to the end of the iunordered_map.
- **end** `iterator end()` (L482): Returns an iterator to the end of the iunordered_map.
- **cbegin** `const_iterator cbegin() const` (L490): Returns a const iterator to the beginning of the iunordered_map.
- **cend** `const_iterator cend() const` (L498): Returns a const iterator to the end of the iunordered_map.
- **rbegin** `const_reverse_iterator rbegin() const` (L506): Returns a const reverse iterator to the end of the iunordered_map.
- **rbegin** `reverse_iterator rbegin()` (L514): Returns a reverse iterator to the end of the iunordered_map.
- **rend** `const_reverse_iterator rend() const` (L522): Returns a const reverse iterator to the beginning of the iunordered_map.
- **rend** `reverse_iterator rend()` (L530): Returns a reverse iterator to the beginning of the iunordered_map.
- **crbegin** `const_reverse_iterator crbegin() const` (L538): Returns a const reverse iterator to the end of the iunordered_map.
- **crend** `const_reverse_iterator crend() const` (L546): Returns a const reverse iterator to the beginning of the iunordered_map.
- **contains** `bool contains(const key_type& key) const` (L556): Indicates if the specified key is contained in this iunordered_map.
  `key` Key to search for.; devuelve: `true` if the specified key is contained in this iunordered_map, otherwise `false`.
- **contains_hash** `bool contains_hash(hash_type key_hash, const key_type& key) const` (L572): Indicates if the specified key is contained in this iunordered_map.
  `key_hash` Hash of the given key to search for.; `key` Key to search for.; devuelve: `true` if the specified key is contained in this iunordered_map, otherwise `false`.
- **count** `size_type count(const key_type& key) const` (L582): Counts the number of keys stored in this iunordered_map are equal to the given one.
  `key` Key to search for.; devuelve: 1 if the specified key is contained in this iunordered_map, otherwise 0.
- **count_hash** `size_type count_hash(hash_type key_hash, const key_type& key) const` (L593): Counts the number of keys stored in this iunordered_map are equal to the given one.
  `key_hash` Hash of the given key to search for.; `key` Key to search for.; devuelve: 1 if the specified key is contained in this iunordered_map, otherwise 0.
- **find** `const_iterator find(const key_type& key) const` (L603): Searches for a given key.
  `key` Key to search for.; devuelve: Const iterator to the (Key, Value) pair if it exists, otherwise end().
- **find** `iterator find(const key_type& key)` (L613): Searches for a given key.
  `key` Key to search for.; devuelve: Iterator to the (Key, Value) pair if it exists, otherwise end().
- **find_hash** `const_iterator find_hash(hash_type key_hash, const key_type& key) const` (L629): Searches for a given key.
  `key_hash` Hash of the given key to search for.; `key` Key to search for.; devuelve: Const iterator to the (Key, Value) pair if it exists, otherwise end().
- **find_hash** `iterator find_hash(hash_type key_hash, const key_type& key)` (L640): Searches for a given key.
  `key_hash` Hash of the given key to search for.; `key` Key to search for.; devuelve: Iterator to the (Key, Value) pair if it exists, otherwise end().
- **at** `const mapped_type& at(const key_type& key) const` (L673): Searches for a given key.
  `key` Key to search for.; devuelve: Const reference to the value stored with the specified key.
- **at** `mapped_type& at(const key_type& key)` (L683): Searches for a given key.
  `key` Key to search for.; devuelve: Reference to the value stored with the specified key.
- **at_hash** `const mapped_type& at_hash(hash_type key_hash, const key_type& key) const` (L694): Searches for a given key.
  `key_hash` Hash of the given key to search for.; `key` Key to search for.; devuelve: Const reference to the value stored with the specified key.
- **at_hash** `mapped_type& at_hash(hash_type key_hash, const key_type& key)` (L705): Searches for a given key.
  `key_hash` Hash of the given key to search for.; `key` Key to search for.; devuelve: Reference to the value stored with the specified key.
- **insert** `iterator insert(const value_type& value)` (L718): Inserts a copy of the given (Key, Value) pair.
  `value` (Key, Value) pair to insert.; devuelve: Iterator pointing to the inserted (Key, Value) pair.
- **insert** `iterator insert(value_type&& value)` (L728): Inserts a moved (Key, Value) pair.
  `value` (Key, Value) pair to insert.; devuelve: Iterator pointing to the inserted (Key, Value) pair.
- **insert** `iterator insert(const key_type& key, const mapped_type& mapped_value)` (L739): Inserts a copy of the given (Key, Value) pair.
  `key` Key to insert.; `mapped_value` Value to insert.; devuelve: Iterator pointing to the inserted (Key, Value) pair.
- **insert** `iterator insert(const key_type& key, mapped_type&& mapped_value)` (L750): Inserts a moved (Key, Value) pair.
  `key` Key to insert.; `mapped_value` Value to insert.; devuelve: Iterator pointing to the inserted (Key, Value) pair.
- **insert_hash** `iterator insert_hash(hash_type key_hash, const value_type& value)` (L761): Inserts a copy of the given (Key, Value) pair.
  `key_hash` Hash of the key to insert.; `value` (Key, Value) pair to insert.; devuelve: Iterator pointing to the inserted (Key, Value) pair.
- **insert_hash** `iterator insert_hash(hash_type key_hash, value_type&& value)` (L772): Inserts a moved (Key, Value) pair.
  `key_hash` Hash of the key to insert.; `value` (Key, Value) pair to insert.; devuelve: Iterator pointing to the inserted (Key, Value) pair.
- **insert_hash** `iterator insert_hash(hash_type key_hash, const key_type& key, const mapped_type& mapped_value)` (L806): Inserts a copy of the given (Key, Value) pair.
  `key_hash` Hash of the key to insert.; `key` Key to insert.; `mapped_value` Value to insert.; devuelve: Iterator pointing to the inserted (Key, Value) pair.
- **insert_hash** `iterator insert_hash(hash_type key_hash, const key_type& key, mapped_type&& mapped_value)` (L818): Inserts a moved (Key, Value) pair.
  `key_hash` Hash of the key to insert.; `key` Key to insert.; `mapped_value` Value to insert.; devuelve: Iterator pointing to the inserted (Key, Value) pair.
- **insert_or_assign** `iterator insert_or_assign(const value_type& value)` (L829): Inserts a copy of the given (Key, Value) pair or replaces the value with the given one if the key is found.
  `value` (Key, Value) pair to insert or assign.; devuelve: Iterator pointing to the inserted or assigned (Key, Value) pair.
- **insert_or_assign** `iterator insert_or_assign(value_type&& value)` (L840): Inserts a moved (Key, Value) pair or replaces the value with the given one if the key is found.
  `value` (Key, Value) pair to insert or assign.; devuelve: Iterator pointing to the inserted or assigned (Key, Value) pair.
- **insert_or_assign** `iterator insert_or_assign(const key_type& key, const mapped_type& mapped_value)` (L852): Inserts a copy of the given (Key, Value) pair or replaces the value with the given one if the key is found.
  `key` Key to insert or assign.; `mapped_value` Value to insert or assign.; devuelve: Iterator pointing to the inserted or assigned (Key, Value) pair.
- **insert_or_assign** `iterator insert_or_assign(const key_type& key, mapped_type&& mapped_value)` (L864): Inserts a moved (Key, Value) pair or replaces the value with the given one if the key is found.
  `key` Key to insert or assign.; `mapped_value` Value to insert or assign.; devuelve: Iterator pointing to the inserted or assigned (Key, Value) pair.
- **insert_or_assign_hash** `iterator insert_or_assign_hash(hash_type key_hash, const value_type& value)` (L876): Inserts a copy of the given (Key, Value) pair or replaces the value with the given one if the key is found.
  `key_hash` Hash of the key to insert or assign.; `value` (Key, Value) pair to insert or assign.; devuelve: Iterator pointing to the inserted or assigned (Key, Value) pair.
- **insert_or_assign_hash** `iterator insert_or_assign_hash(hash_type key_hash, value_type&& value)` (L888): Inserts a moved (Key, Value) pair or replaces the value with the given one if the key is found.
  `key_hash` Hash of the key to insert or assign.; `value` (Key, Value) pair to insert or assign.; devuelve: Iterator pointing to the inserted or assigned (Key, Value) pair.
- **insert_or_assign_hash** `iterator insert_or_assign_hash(hash_type key_hash, const key_type& key, const mapped_type& mapped_value)` (L916): Inserts a copy of the given (Key, Value) pair or replaces the value with the given one if the key is found.
  `key_hash` Hash of the key to insert or assign.; `key` Key to insert or assign.; `mapped_value` Value to insert or assign.; devuelve: Iterator pointing to the inserted or assigned (Key, Value) pair.
- **insert_or_assign_hash** `iterator insert_or_assign_hash(hash_type key_hash, const key_type& key, mapped_type&& mapped_value)` (L929): Inserts a moved (Key, Value) pair or replaces the value with the given one if the key is found.
  `key_hash` Hash of the key to insert or assign.; `key` Key to insert or assign.; `mapped_value` Value to insert or assign.; devuelve: Iterator pointing to the inserted or assigned (Key, Value) pair.
- **try_emplace** `template<typename... Args> iterator try_emplace(const key_type& key, Args&&... args)` (L940): Inserts in-place a (Key, Value) pair if the given key does not exist.
  `key` Key to insert.; `args` Parameters of the value to insert.; devuelve: Iterator pointing to the inserted (Key, Value) pair if the key does not exist, otherwise end().
- **try_emplace_hash** `template<typename... Args> iterator try_emplace_hash(hash_type key_hash, const key_type& key, Args&&... args)` (L962): Inserts in-place a (Key, Value) pair if the given key does not exist.
  `key_hash` Hash of the key to insert.; `key` Key to insert.; `args` Parameters of the value to insert.; devuelve: Iterator pointing to the inserted (Key, Value) pair if the key does not exist, otherwise end().
- **erase** `iterator erase(const const_iterator& position)` (L984): Erases an element.
  `position` Iterator to the element to erase.; devuelve: Iterator following the erased element.
  Unlike `std::unordered_map`, it doesn't offer pointer stability.
- **erase** `bool erase(const key_type& key)` (L1103): Erases an element.
  `key` Key to erase.; devuelve: `true` if the elements was erased, otherwise `false`.
  Unlike `std::unordered_map`, it doesn't offer pointer stability.
- **erase_hash** `bool erase_hash(hash_type key_hash, const key_type& key)` (L1117): Erases an element.
  `key_hash` Hash of the key to erase.; `key` Key to erase.; devuelve: `true` if the elements was erased, otherwise `false`.
  Unlike `std::unordered_map`, it doesn't offer pointer stability.
- **erase_if** `template<class Pred> size_type erase_if(const Pred& pred)` (L1138): Erases all elements that satisfy the specified predicate.
  `pred` Unary predicate which returns ​true if the element should be erased.; devuelve: Number of erased elements.
  Unlike `std::unordered_map`, it doesn't offer pointer stability.
- **merge** `void merge(iunordered_map&& other) noexcept` (L1176): Moves all elements of the given iunordered_map into this one, leaving the first one empty.
  Unlike `std::unordered_map`, it doesn't offer pointer stability.
- **clear** `void clear()` (L1217): Removes all elements.
- **clear** `void clear() requires(! is_trivially_destructible_v<value_type>)` (L1232): Removes all elements.
- **operator[]** `mapped_type& operator[](const key_type& key)` (L1263): Returns a reference to the value that is mapped to the given key, performing an insertion if such key does not already exist.
  `key` Key to search for.; devuelve: Reference to the value that is mapped to the given key.
- **operator()** `mapped_type& operator()(const key_type& key)` (L1274): Returns a reference to the value that is mapped to the given key, performing an insertion if such key does not already exist.
  `key` Key to search for.; devuelve: Reference to the value that is mapped to the given key.
- **operator()** `mapped_type& operator()(hash_type key_hash, const key_type& key)` (L1286): Returns a reference to the value that is mapped to the given key, performing an insertion if such key does not already exist.
  `key_hash` Hash of the key to search for.; `key` Key to search for.; devuelve: Reference to the value that is mapped to the given key.
- **swap** `void swap(iunordered_map& other)` (L1306): Exchanges the contents of this iunordered_map with those of the other one.
  `other` iunordered_map to exchange the contents with.
  Unlike `std::unordered_map`, it doesn't offer pointer stability.
- **swap** `friend void swap(iunordered_map& a, iunordered_map& b)` (L1362): Exchanges the contents of a iunordered_map with those of another one.
  `a` First iunordered_map to exchange the contents with.; `b` Second iunordered_map to exchange the contents with.
  Unlike `std::unordered_map`, it doesn't offer pointer stability.
- **operator==** `friend bool operator==(const iunordered_map& a, const iunordered_map& b)` (L1373): Equal operator.
  `a` First iunordered_map to compare.; `b` Second iunordered_map to compare.; devuelve: `true` if the first iunordered_map is equal to the second one, otherwise `false`.
- **operator<** `friend bool operator<(const iunordered_map& a, const iunordered_map& b)` (L1411): Less than operator.
  `a` First iunordered_map to compare.; `b` Second iunordered_map to compare.; devuelve: `true` if the first iunordered_map is lexicographically less than the second one, otherwise `false`.
- **operator>** `friend bool operator>(const iunordered_map& a, const iunordered_map& b)` (L1423): Greater than operator.
  `a` First iunordered_map to compare.; `b` Second iunordered_map to compare.; devuelve: `true` if the first iunordered_map is lexicographically greater than the second one, otherwise `false`.
- **operator<=** `friend bool operator<=(const iunordered_map& a, const iunordered_map& b)` (L1435): Less than or equal operator.
  `a` First iunordered_map to compare.; `b` Second iunordered_map to compare.; devuelve: `true` if the first iunordered_map is lexicographically less than or equal to the second one, otherwise `false`.
- **operator>=** `friend bool operator>=(const iunordered_map& a, const iunordered_map& b)` (L1447): Greater than or equal operator.
  `a` First iunordered_map to compare.; `b` Second iunordered_map to compare.; devuelve: `true` if the first iunordered_map is lexicographically greater than or equal to the second one, otherwise `false`.
- **unordered_map** `unordered_map() : iunordered_map<Key, Value, KeyHash, KeyEqual>( *reinterpret_cast<pointer>(_storage_buffer), *_allocated_buffer, MaxSize)` (L1548): Default constructor.
- **unordered_map** `unordered_map(const unordered_map& other) : unordered_map()` (L1558): Copy constructor.
  `other` unordered_map to copy.
- **unordered_map** `unordered_map(unordered_map&& other) noexcept : unordered_map()` (L1571): Move constructor.
  `other` unordered_map to move.
  Unlike `std::unordered_map`, it doesn't offer pointer stability.
- **unordered_map** `unordered_map(const iunordered_map<Key, Value, KeyHash, KeyEqual>& other) : unordered_map()` (L1581): Copy constructor.
  `other` iunordered_map to copy.
- **unordered_map** `unordered_map(iunordered_map<Key, Value, KeyHash, KeyEqual>&& other) noexcept : unordered_map()` (L1596): Move constructor.
  `other` iunordered_map to move.
  Unlike `std::unordered_map`, it doesn't offer pointer stability.
- **operator=** `unordered_map& operator=(const unordered_map& other)` (L1609): Copy assignment operator.
  `other` unordered_map to copy.; devuelve: Reference to this.
- **operator=** `unordered_map& operator=(unordered_map&& other) noexcept` (L1628): Move assignment operator.
  `other` unordered_map to move.; devuelve: Reference to this.
  Unlike `std::unordered_map`, it doesn't offer pointer stability.
- **operator=** `unordered_map& operator=(const iunordered_map<Key, Value, KeyHash, KeyEqual>& other)` (L1644): Copy assignment operator.
  `other` iunordered_map to copy.; devuelve: Reference to this.
- **operator=** `unordered_map& operator=(iunordered_map<Key, Value, KeyHash, KeyEqual>&& other) noexcept` (L1665): Move assignment operator.
  `other` iunordered_map to move.; devuelve: Reference to this.
  Unlike `std::unordered_map`, it doesn't offer pointer stability.
- **erase_if** `template<typename Type, typename Value, typename KeyHash, typename KeyEqual, class Pred> typename iunordered_map<Type, Value, KeyHash, KeyEqual>::size_type erase_if( iunordered_map<Type, Value, KeyHash, KeyEqual>& map, const Pred& pred)` (L1695): Erases all elements from a iunordered_map that satisfy the specified predicate.
  `map` iunordered_map from which to erase.; `pred` Unary predicate which returns ​true if the element should be erased.; devuelve: Number of erased elements.
  Unlike `std::unordered_map`, it doesn't offer pointer stability.

## bn_unordered_map_fwd.h

### `bn::iunordered_map` (class, bn_unordered_map_fwd.h:34)

Base class of bn::unordered_map.

`template<typename Key, typename Value, typename KeyHash = hash<Key>, typename KeyEqual = equal_to<Key>> class iunordered_map`

Can be used as a reference type for all bn::unordered_map containers containing a specific type.

Unlike `std::unordered_map`, it doesn't offer pointer stability when erasing elements.

### `bn::unordered_map` (class, bn_unordered_map_fwd.h:52)

`std::unordered_map` like container with a fixed size buffer.

`template<typename Key, typename Value, int MaxSize, typename KeyHash = hash<Key>, typename KeyEqual = equal_to<Key>> class unordered_map`

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

Unlike `std::unordered_map`, it doesn't offer pointer stability when erasing elements.

