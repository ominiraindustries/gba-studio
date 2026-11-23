// DELVE block 1 (G-35): real-time combat and core loop (DISENO.md 2-5). Player stats, monsters on the grid (chase,
// telegraphed attacks that hit a cell, dodge, crits), floor items, XP and level ups.
//
// The game owns the view and the sprites; this module only changes the level and posts events (dl::combat::events)
// that the game turns into numbers, flashes and sounds. Big arrays live in EWRAM (dl_combat_data), so the headless
// planner can read them (tests/route.py).
#ifndef DL_COMBAT_H
#define DL_COMBAT_H

#include <stdint.h>

#include "bn_seed_random.h"
#include "bn_vector.h"

#include "dl_gen.h"

namespace dl
{
    class hud;
    class monster_fx;

    namespace view
    {
        struct edge_state;
    }

    enum item_id : uint8_t { ITEM_BREAD, ITEM_POTION, ITEM_SWORD, ITEM_KEY, ITEM_COUNT };   // = item_floor frames
    enum mon_kind : uint8_t { MON_RAT, MON_SLIME, MON_SKELETON, MON_KIND_COUNT };
    enum mon_state : uint8_t { MS_IDLE, MS_CHASE, MS_WARN, MS_DYING };

    struct mon_stats
    {
        const char* name;
        int hp, def, damage, move_f, warn_f, recover_f, xp, loot_pct, loot;
    };

    // DISENO.md 4 (rat, slime, skeleton). Skeleton loot "weapon / armour": the short sword if not owned, else a potion.
    constexpr mon_stats mon_table[MON_KIND_COUNT] = {
        { "RAT", 6, 0, 3, 30, 24, 70, 3, 20, ITEM_BREAD },
        { "SLIME", 16, 0, 4, 60, 36, 80, 5, 30, ITEM_POTION },
        { "SKELETON", 20, 1, 6, 30, 30, 90, 10, 25, ITEM_SWORD },
    };

    struct weapon_stats
    {
        const char* name;
        int damage, reload_f;
    };

    constexpr weapon_stats weapon_table[2] = { { "DAGGER", 3, 30 }, { "SHORT SWORD", 5, 40 } };

    struct monster
    {
        int16_t ent;        // index in level::ents (x, y kept in sync; b = mon_kind)
        int16_t cell;
        uint8_t kind;
        uint8_t state;      // mon_state
        int16_t hp;
        int16_t timer;      // MS_WARN: frames left; MS_DYING: frames left
        int16_t move_timer;
        int16_t attack_cd;  // recovery after a strike
        int16_t target;     // cell the telegraphed blow lands on
        uint8_t hit;        // hit-frame time left
        uint8_t strike;     // strike-frame time left
        uint8_t warn_len;
        int8_t side;        // -1 left, 1 right, 2 behind, 0 front (relative to the player when the warning started)
    };

    struct player_stats
    {
        int hp = 30;
        int hp_max = 30;
        int str = 2;
        int def = 0;
        int level = 1;
        int xp = 0;
        int potions = 0;
        int keys = 0;
        int weapon = 0;         // weapon_table index
        int reload = 30;        // frames charged (full = weapon reload)
        int invuln = 0;         // frames left
        int counter = 0;        // guaranteed-crit window after a dodge
        int busy = 0;           // drinking: no attack
        bool attack_buffered = false;
        // stats for tests / metrics
        int kills = 0;
        int dodges = 0;
        int crits = 0;
        int hits_taken = 0;
        int attacks = 0;
    };

    constexpr int max_monsters = 32;

    struct combat_data
    {
        player_stats p;
        monster mons[max_monsters];
        int nmons;
        int16_t mon_at[gen::N];     // monster index per cell, -1 none
        uint8_t dist[gen::N];       // BFS distance from the player (monster moves), 255 = far / unreachable
        int dist_from;              // cell the map was computed from (-1 stale)
    };

    enum event_type : uint8_t
    {
        EV_MON_HIT,         // ent, value = damage, crit
        EV_MON_MISS,        // ent: the blow missed (dodged)
        EV_MON_DIED,        // ent
        EV_PLAYER_HIT,      // value = damage, side
        EV_LEVEL_UP,
        EV_PLAYER_DEAD,
        EV_PICKUP,          // value = item_id
    };

    struct event
    {
        uint8_t type;
        bool crit;
        int8_t side;
        int16_t ent;
        int16_t value;
    };

    enum hint_id : uint8_t
    {
        HINT_MONSTER = 1, HINT_DODGE = 2, HINT_POTION = 4, HINT_STAIRS = 8
    };

    class combat
    {
    public:
        combat(hud& hud, monster_fx& mfx);

        void new_run();

        // after generating a floor: monsters from E_MONSTER (none within 6 cells of the entry), items normalised to
        // item_id, ent_of_cell is the game's static index (items, keys, furniture, traps)
        void load_floor(gen::level& lv, const view::edge_state& es, int16_t* ent_of_cell, int floor, unsigned seed);

        // once per frame. think: a light frame (no view redraw pending), so the BFS may run
        void update(int cell, int dir, bool a_pressed, bool b_pressed, bool think);

        [[nodiscard]] int monster_ent_at(int cell) const;
        [[nodiscard]] const monster* monster_of_ent(int ent) const;
        [[nodiscard]] int kind_of_ent(int ent) const;
        [[nodiscard]] bool monster_blocks(int cell) const;

        void pickup(int ent);               // the player stepped on an E_ITEM / E_KEY
        void trap_hit(int damage);          // ignores DEF
        void hint(hint_id id, const char* text);

        // a monster changed cell, appeared or vanished: the game rebuilds its object sprites (on a light frame)
        bool objects_dirty = false;
        bn::vector<event, 12> events;

        [[nodiscard]] player_stats& player();
        [[nodiscard]] int reload_max() const;
        [[nodiscard]] int last_think_ticks() const { return _think_ticks; }
        [[nodiscard]] int max_think_ticks() const { return _think_max; }

    private:
        hud& _hud;
        monster_fx& _mfx;
        gen::level* _lv = nullptr;
        const view::edge_state* _es = nullptr;
        int16_t* _ent_of_cell = nullptr;
        bn::seed_random _rng;
        int _floor = 0;
        int _cell = 0;
        int _dir = 0;
        int _frame = 0;
        int _hints = 0;
        int _think_ticks = 0;
        int _think_max = 0;

        bool _edge_pass(int cell, int d, bool doors_closed_block) const;
        bool _sees_player(const monster& m) const;
        void _bfs(int from, bool doors_pass, int max_depth);
        void _wake_near(int radius);
        void _player_attack();
        void _monster_update(int index, bool think);
        void _monster_step(int index);
        void _strike(monster& m);
        void _kill(int index);
        void _remove(int index);
        void _drop(int ent, int cell, int item);   // item < 0: nothing (the entity goes)
        void _gain_xp(int xp);
        void _post(uint8_t type, int ent, int value, bool crit = false, int side = 0);
        int _dir_to(int from, int to) const;
    };
}

extern dl::combat_data dl_combat_data;

#endif
