# Butano 21.10.1 — Random (`random`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_random.h

### `bn::random` (class, bn_random.h:30)

Fast, deterministic random number generator.

`class random`

Its current implementation is a Marsaglia's xorshf generator.

See https://github.com/raylee/xorshf96/blob/master/xorshf96.c

- **get** `constexpr unsigned get()` (L37): Returns a random unsigned integer greater or equal than 0, updating its internal seed in the process.
- **get_bool** `constexpr bool get_bool()` (L46): Returns a random boolean, updating its internal seed in the process.
- **get_int** `constexpr int get_int()` (L54): Returns a random signed integer greater or equal than 0, updating its internal seed in the process.
- **get_fixed** `constexpr fixed get_fixed()` (L62): Returns a random bn::fixed greater or equal than 0, updating its internal seed in the process.
- **get_int** `constexpr int get_int(int limit)` (L78): Returns a random signed integer in the range [0..limit), updating its internal seed in the process.
  `limit` Returned value is lower than this value.; devuelve: Random signed integer in the range [0..limit).
  It's possible that the output will be biased towards smaller numbers due to
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>.
  
  random::get_unbiased_int can be used to avoid modulo bias.
- **get_fixed** `constexpr fixed get_fixed(fixed limit)` (L96): Returns a random bn::fixed in the range [0..limit), updating its internal seed in the process.
  `limit` Returned value is lower than this value.; devuelve: Random bn::fixed in the range [0..limit).
  It's possible that the output will be biased towards smaller numbers due to
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>.
  
  random::get_unbiased_fixed can be used to avoid modulo bias.
- **get_int** `constexpr int get_int(int minimum, int limit)` (L115): Returns a random signed integer in the range [minimum..limit), updating its internal seed in the process.
  `minimum` Returned value is greater or equal than this value.; `limit` Returned value is lower than this value.; devuelve: Random signed integer in the range [minimum..limit).
  It's possible that the output will be biased towards smaller numbers due to
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>.
  
  random::get_unbiased_int can be used to avoid modulo bias.
- **get_fixed** `constexpr fixed get_fixed(fixed minimum, fixed limit)` (L135): Returns a random bn::fixed in the range [minimum..limit), updating its internal seed in the process.
  `minimum` Returned value is greater or equal than this value.; `limit` Returned value is lower than this value.; devuelve: Random bn::fixed in the range [minimum..limit).
  It's possible that the output will be biased towards smaller numbers due to
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>.
  
  random::get_unbiased_fixed can be used to avoid modulo bias.
- **get_unbiased_int** `constexpr int get_unbiased_int(int limit)` (L153): Returns a random signed integer in the range [0..limit), updating its internal seed in the process.
  `limit` Returned value is lower than this value.; devuelve: Random signed integer in the range [0..limit).
  This implementation avoids
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>,
  so it's less likely that the output will be biased towards smaller numbers.
- **get_unbiased_fixed** `constexpr fixed get_unbiased_fixed(fixed limit)` (L170): Returns a random bn::fixed in the range [0..limit), updating its internal seed in the process.
  `limit` Returned value is lower than this value.; devuelve: Random bn::fixed in the range [0..limit).
  This implementation avoids
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>,
  so it's less likely that the output will be biased towards smaller numbers.
- **get_unbiased_int** `constexpr int get_unbiased_int(int minimum, int limit)` (L188): Returns a random signed integer in the range [minimum..limit), updating its internal seed in the process.
  `minimum` Returned value is greater or equal than this value.; `limit` Returned value is lower than this value.; devuelve: Random signed integer in the range [minimum..limit).
  This implementation avoids
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>,
  so it's less likely that the output will be biased towards smaller numbers.
- **get_unbiased_fixed** `constexpr fixed get_unbiased_fixed(fixed minimum, fixed limit)` (L207): Returns a random bn::fixed in the range [minimum..limit), updating its internal seed in the process.
  `minimum` Returned value is greater or equal than this value.; `limit` Returned value is lower than this value.; devuelve: Random bn::fixed in the range [minimum..limit).
  This implementation avoids
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>,
  so it's less likely that the output will be biased towards smaller numbers.
- **update** `constexpr void update()` (L218): Updates the value of the internal seed.

## bn_seed_random.h

### `bn::seed_random` (class, bn_seed_random.h:28)

Deterministic random number generator with a public, configurable seed.

`class seed_random`

See https://github.com/velipso/whisky

- **seed_random** `constexpr seed_random() = default` (L35): Default constructor.
- **seed_random** `constexpr explicit seed_random(unsigned seed)` (L41): Constructor.
  `seed` Initial value of the seed used to generate random numbers.
- **seed** `constexpr unsigned seed() const` (L49): Returns the value of the seed used to generate random numbers.
- **set_seed** `constexpr void set_seed(unsigned seed)` (L57): Sets the value of the seed used to generate random numbers.
- **get** `constexpr unsigned get()` (L65): Returns a random unsigned integer greater or equal than 0, updating its seed in the process.
- **get_bool** `constexpr bool get_bool()` (L79): Returns a random boolean, updating its internal seed in the process.
- **get_int** `constexpr int get_int()` (L87): Returns a random signed integer greater or equal than 0, updating its seed in the process.
- **get_fixed** `constexpr fixed get_fixed()` (L95): Returns a random bn::fixed greater or equal than 0, updating its seed in the process.
- **get_int** `constexpr int get_int(int limit)` (L111): Returns a random signed integer in the range [0..limit), updating its internal seed in the process.
  `limit` Returned value is lower than this value.; devuelve: Random signed integer in the range [0..limit).
  It's possible that the output will be biased towards smaller numbers due to
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>.
  
  seed_random::get_unbiased_int can be used to avoid modulo bias.
- **get_fixed** `constexpr fixed get_fixed(fixed limit)` (L129): Returns a random bn::fixed in the range [0..limit), updating its internal seed in the process.
  `limit` Returned value is lower than this value.; devuelve: Random bn::fixed in the range [0..limit).
  It's possible that the output will be biased towards smaller numbers due to
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>.
  
  seed_random::get_unbiased_fixed can be used to avoid modulo bias.
- **get_int** `constexpr int get_int(int minimum, int limit)` (L148): Returns a random signed integer in the range [minimum..limit), updating its internal seed in the process.
  `minimum` Returned value is greater or equal than this value.; `limit` Returned value is lower than this value.; devuelve: Random signed integer in the range [minimum..limit).
  It's possible that the output will be biased towards smaller numbers due to
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>.
  
  seed_random::get_unbiased_int can be used to avoid modulo bias.
- **get_fixed** `constexpr fixed get_fixed(fixed minimum, fixed limit)` (L168): Returns a random bn::fixed in the range [minimum..limit), updating its internal seed in the process.
  `minimum` Returned value is greater or equal than this value.; `limit` Returned value is lower than this value.; devuelve: Random bn::fixed in the range [minimum..limit).
  It's possible that the output will be biased towards smaller numbers due to
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>.
  
  seed_random::get_unbiased_fixed can be used to avoid modulo bias.
- **get_unbiased_int** `constexpr int get_unbiased_int(int limit)` (L186): Returns a random signed integer in the range [0..limit), updating its internal seed in the process.
  `limit` Returned value is lower than this value.; devuelve: Random signed integer in the range [0..limit).
  This implementation avoids
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>,
  so it's less likely that the output will be biased towards smaller numbers.
- **get_unbiased_fixed** `constexpr fixed get_unbiased_fixed(fixed limit)` (L203): Returns a random bn::fixed in the range [0..limit), updating its internal seed in the process.
  `limit` Returned value is lower than this value.; devuelve: Random bn::fixed in the range [0..limit).
  This implementation avoids
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>,
  so it's less likely that the output will be biased towards smaller numbers.
- **get_unbiased_int** `constexpr int get_unbiased_int(int minimum, int limit)` (L221): Returns a random signed integer in the range [minimum..limit), updating its internal seed in the process.
  `minimum` Returned value is greater or equal than this value.; `limit` Returned value is lower than this value.; devuelve: Random signed integer in the range [minimum..limit).
  This implementation avoids
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>,
  so it's less likely that the output will be biased towards smaller numbers.
- **get_unbiased_fixed** `constexpr fixed get_unbiased_fixed(fixed minimum, fixed limit)` (L240): Returns a random bn::fixed in the range [minimum..limit), updating its internal seed in the process.
  `minimum` Returned value is greater or equal than this value.; `limit` Returned value is lower than this value.; devuelve: Random bn::fixed in the range [minimum..limit).
  This implementation avoids
  <a href="https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/">modulo bias</a>,
  so it's less likely that the output will be biased towards smaller numbers.
- **update** `constexpr void update()` (L251): Updates the value of the seed.
