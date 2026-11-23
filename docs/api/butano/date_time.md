# Butano 21.10.1 — Date and time (`date_time`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_date.h

### `bn::date` (class, bn_date.h:27)

Manages a date represented by year, month, month day and week day.

`class date`

- **valid** `constexpr static bool valid(int year, int month, int month_day, int week_day)` (L39): Indicates if the specified date is valid or not.
  `year` Year part of the date.; `month` Month part of the date, in the range [1..12].; `month_day` Month day part of the date, in the range [1..31].; `week_day` Week day part of the date, in the range [0..6].; devuelve: `true` if the specified date is valid; `false` otherwise.
- **active** `static bool active()` (L88): Indicates if the real time clock (RTC) is present or not.
- **current** `static optional<date> current()` (L94): Returns the current date as reported by the real time clock (RTC) if it's present and the reported date is valid; bn::nullopt otherwise.
- **date** `constexpr date(int year, int month, int month_day, int week_day)` (L103): Constructor.
  `year` Year part of the date.; `month` Month part of the date, in the range [1..12].; `month_day` Month day part of the date, in the range [1..31].; `week_day` Week day part of the date, in the range [0..6].
- **year** `constexpr int year() const` (L116): Returns the year part of the date.
- **set_year** `constexpr void set_year(int year)` (L125): Sets the year part of the date.
  `year` Year part of the date.
- **month** `constexpr int month() const` (L136): Returns the month part of the date.
- **set_month** `constexpr void set_month(int month)` (L145): Sets the month part of the date.
  `month` Month part of the date, in the range [1..12].
- **month_day** `constexpr int month_day() const` (L156): Returns the month day part of the date.
- **set_month_day** `constexpr void set_month_day(int month_day)` (L165): Sets the month day part of the date.
  `month_day` Month day part of the date, in the range [1..31].
- **week_day** `constexpr int week_day() const` (L176): Returns the week day part of the date.
- **set_week_day** `constexpr void set_week_day(int week_day)` (L185): Sets the week day part of the date.
  `week_day` Week day part of the date, in the range [0..6].
- **operator==** `constexpr friend bool operator==(date a, date b) = default` (L196): Default equal operator.
- **operator<** `constexpr friend bool operator<(date a, date b)` (L204): Less than operator.
  `a` First date to compare.; `b` Second date to compare.; devuelve: `true` if the first date is earlier than the second one, otherwise `false`.
- **operator>** `constexpr friend bool operator>(date a, date b)` (L225): Greater than operator.
  `a` First date to compare.; `b` Second date to compare.; devuelve: `true` if the first date is later than the second one, otherwise `false`.
- **operator<=** `constexpr friend bool operator<=(date a, date b)` (L236): Less than or equal operator.
  `a` First date to compare.; `b` Second date to compare.; devuelve: `true` if the first date is earlier than or equal to the second one, otherwise `false`.
- **operator>=** `constexpr friend bool operator>=(date a, date b)` (L247): Greater than or equal operator.
  `a` First date to compare.; `b` Second date to compare.; devuelve: `true` if the first date is later than or equal to the second one, otherwise `false`.

## bn_time.h

### `bn::time` (class, bn_time.h:27)

Defines a clock time, which it can express as the numbers of hours, minutes and seconds since midnight.

`class time`

- **valid** `constexpr static bool valid(int hour, int minute, int second)` (L38): Indicates if the specified time is valid or not.
  `hour` Hour part of the time, in the range [0..23].; `minute` Minute part of the time, in the range [0..59].; `second` Second part of the time, in the range [0..59].; devuelve: `true` if the specified time is valid; `false` otherwise.
- **active** `static bool active()` (L46): Indicates if the real time clock (RTC) is present or not.
- **current** `static optional<time> current()` (L52): Returns the current time as reported by the real time clock (RTC) if it's present and the reported time is valid; bn::nullopt otherwise.
- **time** `constexpr time(int hour, int minute, int second)` (L60): Constructor.
  `hour` Hour part of the time, in the range [0..23].; `minute` Minute part of the time, in the range [0..59].; `second` Second part of the time, in the range [0..59].
- **hour** `constexpr int hour() const` (L73): Returns the hour part of the time.
- **set_hour** `constexpr void set_hour(int hour)` (L82): Sets the hour part of the time.
  `hour` Hour part of the time, in the range [0..23].
- **minute** `constexpr int minute() const` (L92): Returns the minute part of the time.
- **set_minute** `constexpr void set_minute(int minute)` (L101): Sets the minute part of the time.
  `minute` Minute part of the time, in the range [0..59].
- **second** `constexpr int second() const` (L111): Returns the second part of the time.
- **set_second** `constexpr void set_second(int second)` (L120): Sets the second part of the time.
  `second` Second part of the time, in the range [0..59].
- **operator==** `constexpr friend bool operator==(time a, time b) = default` (L130): Default equal operator.
- **operator<** `constexpr friend bool operator<(time a, time b)` (L138): Less than operator.
  `a` First time to compare.; `b` Second time to compare.; devuelve: `true` if the first time is earlier than the second one, otherwise `false`.
- **operator>** `constexpr friend bool operator>(time a, time b)` (L159): Greater than operator.
  `a` First time to compare.; `b` Second time to compare.; devuelve: `true` if the first time is later than the second one, otherwise `false`.
- **operator<=** `constexpr friend bool operator<=(time a, time b)` (L170): Less than or equal operator.
  `a` First time to compare.; `b` Second time to compare.; devuelve: `true` if the first time is earlier than or equal to the second one, otherwise `false`.
- **operator>=** `constexpr friend bool operator>=(time a, time b)` (L181): Greater than or equal operator.
  `a` First time to compare.; `b` Second time to compare.; devuelve: `true` if the first time is later than or equal to the second one, otherwise `false`.
