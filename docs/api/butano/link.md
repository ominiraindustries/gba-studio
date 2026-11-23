# Butano 21.10.1 — Link communication (`link`)

Generado por `.claude/scripts/gen_butano_api.py` desde `engine/butano/butano/include/`. No editar a mano.

## bn_config_link.h

### `BN_CFG_LINK_BAUD_RATE` (macro, bn_config_link.h:28)

Specifies transmission rate in bps.

`#define BN_CFG_LINK_BAUD_RATE BN_LINK_BAUD_RATE_9600_BPS`

Values not specified in BN_LINK_BAUD_RATE_* macros are not allowed.

### `BN_CFG_LINK_SEND_WAIT` (macro, bn_config_link.h:42)

Specifies how much time the GBA has to wait before sending pending messages.

`#define BN_CFG_LINK_SEND_WAIT 50`

If this parameter is too low, some messages will be lost,
but if it is too high, CPU usage will increase a lot.

### `BN_CFG_LINK_MAX_MESSAGES` (macro, bn_config_link.h:53)

Specifies the maximum number of stored messages for each player.

`#define BN_CFG_LINK_MAX_MESSAGES 8`

### `BN_CFG_LINK_MAX_MISSING_MESSAGES` (macro, bn_config_link.h:64)

Specifies the maximum number of missing messages before resetting the communication.

`#define BN_CFG_LINK_MAX_MISSING_MESSAGES 4`


## bn_link.h

### `bn::link` (namespace, bn_link.h:28)

Link communication related functions.

- **send** `void send(int data)` (L37): Send a message to the other players.
  `data` Data to send, in the range [0..65533].
  Keep in mind that the message can be lost before being received by other players.
- **receive** `optional<link_state> receive()` (L44): Returns the current state of the communication with other players.
  Keep in mind that some messages can be lost before receiving them.
- **deactivate** `void deactivate()` (L49): Deactivates the communication with other players until send() or receive() are called.

## bn_link_baud_rate.h

### `BN_LINK_BAUD_RATE_9600_BPS` (macro, bn_link_baud_rate.h:25)

9600 bps.

`#define BN_LINK_BAUD_RATE_9600_BPS 0`

### `BN_LINK_BAUD_RATE_38400_BPS` (macro, bn_link_baud_rate.h:34)

38400 bps.

`#define BN_LINK_BAUD_RATE_38400_BPS 1`

### `BN_LINK_BAUD_RATE_57600_BPS` (macro, bn_link_baud_rate.h:43)

57600 bps.

`#define BN_LINK_BAUD_RATE_57600_BPS 2`

### `BN_LINK_BAUD_RATE_115200_BPS` (macro, bn_link_baud_rate.h:52)

115200 bps.

`#define BN_LINK_BAUD_RATE_115200_BPS 3`


## bn_link_player.h

### `bn::link_player` (class, bn_link_player.h:26)

Contains the data of a message provided by a player.

`class link_player`

- **link_player** `constexpr link_player(int id, int data)` (L35): Constructor.
  `id` Player ID, in the range [0..3].; `data` Player data, in the range [0..65533].
- **id** `constexpr int id() const` (L46): Returns the ID of the player.
- **data** `constexpr uint16_t data() const` (L54): Returns the data provided by the player.
- **operator==** `friend bool operator==(const link_player& a, const link_player& b) = default` (L62): Default equal operator.

## bn_link_state.h

### `bn::link_state` (class, bn_link_state.h:27)

Stores the state of a communication through the link cable.

`class link_state`

- **link_state** `link_state(int current_player_id, const vector<link_player, 3>& other_players)` (L36): Constructor.
  `current_player_id` ID of this player, in the range [0..3].; `other_players` Data provided by the other players.
- **link_state** `link_state(int current_player_id, int player_count, const vector<link_player, 3>& other_players)` (L51): Constructor.
  `current_player_id` ID of this player, in the range [0..3].; `player_count` Number of connected players (including this one).; `other_players` Data provided by the other players.
- **current_player_id** `int current_player_id() const` (L65): Returns the ID of this player.
- **player_count** `int player_count() const` (L73): Returns the number of connected players (including this one).
- **other_players** `const ivector<link_player>& other_players() const` (L81): Returns the data provided by the other players.
- **operator==** `friend bool operator==(const link_state& a, const link_state& b) = default` (L89): Default equal operator.
