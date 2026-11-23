// DELVE block 1 (G-35): combat and core loop. See dl_combat.h.

#include "dl_combat.h"

#include "bn_fixed.h"
#include "bn_math.h"
#include "bn_string.h"
#include "bn_timer.h"

#include "bn_sound_items.h"

#include "dl_hud.h"
#include "dl_monster_fx.h"
#include "dl_view.h"

BN_DATA_EWRAM dl::combat_data dl_combat_data;
extern volatile int test_hints;
extern volatile int test_hint_frame[8];

namespace dl
{
    using namespace gen;

    namespace
    {
        constexpr int spawn_clear_cells = 6;    // G-35 brief: no monster within 6 cells (path) of the entry
        constexpr int invuln_frames = 20;       // DISENO.md 3
        constexpr int counter_frames = 30;
        constexpr int attack_buffer_frames = 8;
        constexpr int drink_frames = 20;        // DISENO.md 5
        constexpr int death_frames = 16;        // DISENO.md 4: fades in 16 f
        constexpr int hit_frames = 10;
        constexpr int strike_frames = 10;
        constexpr int sight_cells = 3;
        constexpr int hearing_cells = 4;
        constexpr int chase_depth = 16;         // BFS depth; farther away a chaser gives up
        constexpr int max_level = 10;

        BN_DATA_EWRAM int16_t bfs_queue[N];

        combat_data& cd = dl_combat_data;

        int xp_for_level(int level)
        {
            static constexpr int table[7] = { 0, 0, 10, 25, 45, 70, 100 };
            return level <= 6 ? table[level] : 100 + 35 * (level - 6);
        }

        void sfx(bn::fixed speed, bn::fixed volume = 0.6)
        {
            bn::sound_items::blip.play(volume, speed, 0);
        }
    }

    combat::combat(hud& hud, monster_fx& mfx) :
        _hud(hud),
        _mfx(mfx)
    {
        new_run();
    }

    void combat::new_run()
    {
        cd.p = player_stats();
        cd.p.potions = 1;       // DISENO 10 (G-42 review): 1 potion instead of bread, so B gets taught
        cd.p.reload = weapon_table[0].reload_f;
        cd.m = magic_state();
        _hints = 0;
        _next_hint = nullptr;
        _next_hint_timer = 0;
    }

    player_stats& combat::player()
    {
        return cd.p;
    }

    int combat::reload_max() const
    {
        return weapon_table[cd.p.weapon].reload_f;
    }

    void combat::hint(hint_id id, const char* text)
    {
        if(! (_hints & id))
        {
            _hints |= id;
            _hud.message(text, 240);
            test_hints = _hints;

            for(int b = 0; b < 8; ++b)
            {
                if(id == (1 << b))
                {
                    test_hint_frame[b] = _frame;
                }
            }
        }
    }

    // G-42 (DISENO 10): the first time HP drops below half with a potion in the bag
    void combat::_low_hp_hint()
    {
        const player_stats& p = cd.p;

        if(p.hp > 0 && p.hp * 2 < p.hp_max && p.potions > 0)
        {
            hint(HINT_LOW_HP, "LOW HP! B: DRINK POTION");
        }
    }

    // G-42 DISENO 7: floor retry. The player as he came down (stats, HP / MP, level, spells, belt, bag, equipment);
    // EWRAM, the IWRAM stack has little room (01f3822)
    namespace
    {
        struct floor_snapshot
        {
            player_stats p;
            magic_state m;
        };

        BN_DATA_EWRAM floor_snapshot floor_start;
    }

    void combat::save_floor_start()
    {
        floor_start.p = cd.p;
        floor_start.m = cd.m;
    }

    void combat::restore_floor_start()
    {
        player_stats stats = cd.p;      // metrics of the whole run survive the retry
        cd.p = floor_start.p;
        cd.p.kills = stats.kills;
        cd.p.dodges = stats.dodges;
        cd.p.crits = stats.crits;
        cd.p.hits_taken = stats.hits_taken;
        cd.p.attacks = stats.attacks;
        cd.p.reload = reload_max();
        int casts = cd.m.casts;
        int missile_kills = cd.m.missile_kills;
        cd.m = floor_start.m;
        cd.m.casts = casts;
        cd.m.missile_kills = missile_kills;
        cd.m.light = 0;
        cd.m.heal_fx = 0;
        cd.m.light_fx = 0;
        cd.m.hint_timer = 0;
        _next_hint = nullptr;
        _next_hint_timer = 0;
    }

    void combat::_post(uint8_t type, int ent, int value, bool crit, int side)
    {
        if(! events.full())
        {
            events.push_back(event{ type, crit, int8_t(side), int16_t(ent), int16_t(value) });
        }
    }

    // ---- grid helpers
    bool combat::_edge_pass(int cell, int d, bool doors_closed_block) const
    {
        int idx = edge_index(cell, d);

        if(idx < 0)
        {
            return false;
        }

        switch(_lv->ed[idx] & 15)
        {
        case K_OPEN:
        case K_ARCH:
            return true;

        case K_DOOR:
        case K_LOCKED:
        case K_BARS:
            return ! doors_closed_block || _es->open[idx];

        default:
            return false;
        }
    }

    int combat::_dir_to(int from, int to) const
    {
        for(int d = 0; d < 4; ++d)
        {
            if(neighbour(from, d) == to)
            {
                return d;
            }
        }

        return -1;
    }

    bool combat::_sees_player(const monster& m) const
    {
        int mx = m.cell & (W - 1), my = m.cell >> 5;
        int px = _cell & (W - 1), py = _cell >> 5;
        int dist = bn::abs(mx - px) + bn::abs(my - py);

        if((mx != px && my != py) || dist == 0 || dist > sight_cells)
        {
            return false;
        }

        int d = _dir_to(m.cell, neighbour(m.cell, mx < px ? 1 : mx > px ? 3 : my < py ? 2 : 0));
        int c = m.cell;

        for(int k = 0; k < dist; ++k)
        {
            int idx = edge_index(c, d);
            int kind = idx >= 0 ? (_lv->ed[idx] & 15) : K_WALL;
            bool see = sight_kind[kind] || ((kind == K_DOOR || kind == K_LOCKED) && _es->open[idx]);

            if(! see)
            {
                return false;
            }

            c = neighbour(c, d);
        }

        return true;
    }

    void combat::_bfs(int from, bool doors_pass, int max_depth)
    {
        for(int c = 0; c < N; ++c)
        {
            cd.dist[c] = 255;
        }

        int head = 0, tail = 0;
        cd.dist[from] = 0;
        bfs_queue[tail++] = int16_t(from);

        while(head < tail)
        {
            int c = bfs_queue[head++];
            int nd = cd.dist[c] + 1;

            if(nd > max_depth)
            {
                continue;
            }

            for(int d = 0; d < 4; ++d)
            {
                if(_edge_pass(c, d, ! doors_pass))
                {
                    int n = neighbour(c, d);

                    if(cd.dist[n] == 255)
                    {
                        cd.dist[n] = uint8_t(nd);
                        bfs_queue[tail++] = int16_t(n);
                    }
                }
            }
        }
    }

    // ---- floor
    void combat::load_floor(level& lv, const view::edge_state& es, int16_t* ent_of_cell, int floor, unsigned seed)
    {
        _lv = &lv;
        _es = &es;
        _ent_of_cell = ent_of_cell;
        _floor = floor;
        _rng.set_seed(seed ^ 0x5BD1E995u);
        events.clear();
        cd.nmons = 0;
        cd.p.attack_buffered = false;
        cd.p.invuln = 0;
        cd.p.counter = 0;
        cd.p.busy = 0;
        cd.m.cast_timer = 0;
        cd.m.missile_cell = -1;
        cd.m.missile_hit = 0;
        cd.m.nchests = 0;

        for(int c = 0; c < N; ++c)
        {
            cd.mon_at[c] = -1;
        }

#ifdef DL_DEBUG_TESTROOM
        // G-40 fixed test room (main.cpp build_test_room): monsters stay where they were put
        if(floor == 0)
        {
            for(int i = 0; i < lv.nents; ++i)
            {
                entity& e = lv.ents[i];
                int c = e.y * W + e.x;

                if(e.type == E_MONSTER && cd.nmons < max_monsters)
                {
                    int kind = e.b < MON_KIND_COUNT ? int(e.b) : int(MON_RAT);
                    cd.mons[cd.nmons] = monster{ int16_t(i), int16_t(c), uint8_t(kind), MS_IDLE,
                                                 int16_t(mon_table[kind].hp), 0, int16_t(mon_table[kind].move_f),
                                                 0, -1, 0, 0, 0, 0 };
                    cd.mon_at[c] = int16_t(cd.nmons);
                    ++cd.nmons;
                }
            }

            _setup_chests(true);
            cd.dist_from = -1;
            objects_dirty = true;
            return;
        }
#endif

        _bfs(lv.entry, true, 250);
        int nearest = -1;
        int nearest_dist = 255;
        bool sword = false, potion = false;
        int far_item = -1, near_bread = -1;

        for(int i = 0; i < lv.nents; ++i)
        {
            entity& e = lv.ents[i];
            int c = e.y * W + e.x;

            if(e.type == E_MONSTER)
            {
                if(cd.dist[c] < spawn_clear_cells || cd.nmons >= max_monsters || cd.mon_at[c] >= 0)
                {
                    e.type = E_NONE;
                    continue;
                }

                int r = _rng.get_int(100);
                int kind = floor == 0 ? (r < 70 ? MON_RAT : MON_SLIME) :
                           floor == 1 ? (r < 30 ? MON_RAT : r < 60 ? MON_SLIME : MON_SKELETON) :
                                        (r < 15 ? MON_RAT : r < 40 ? MON_SLIME : MON_SKELETON);
                monster& m = cd.mons[cd.nmons];
                m = monster{ int16_t(i), int16_t(c), uint8_t(kind), MS_IDLE, int16_t(mon_table[kind].hp), 0,
                             int16_t(mon_table[kind].move_f), 0, -1, 0, 0, 0, 0 };
                e.b = uint8_t(kind);
                cd.mon_at[c] = int16_t(cd.nmons);

                if(cd.dist[c] < nearest_dist)
                {
                    nearest_dist = cd.dist[c];
                    nearest = cd.nmons;
                }

                ++cd.nmons;
            }
            else if(e.type == E_ITEM)
            {
                int t = e.b & 7;
                int id = t < 4 ? ITEM_BREAD : t < 7 ? ITEM_POTION : ITEM_SWORD;

                if(id == ITEM_SWORD && (cd.p.weapon > 0 || sword))
                {
                    id = ITEM_POTION;
                }

                sword |= id == ITEM_SWORD;
                potion |= id == ITEM_POTION;
                e.b = uint8_t(id);

                if(cd.dist[c] != 255 && (far_item < 0 || cd.dist[c] > cd.dist[lv.ents[far_item].y * W + lv.ents[far_item].x]))
                {
                    far_item = i;
                }

                if(id == ITEM_BREAD && cd.dist[c] != 255 && (near_bread < 0 ||
                        cd.dist[c] < cd.dist[lv.ents[near_bread].y * W + lv.ents[near_bread].x]))
                {
                    near_bread = i;
                }
            }
        }

        // DISENO.md 8: a lone rat comes first on floor 1
        if(floor == 0 && nearest >= 0)
        {
            monster& m = cd.mons[nearest];
            m.kind = MON_RAT;
            m.hp = int16_t(mon_table[MON_RAT].hp);
            m.move_timer = int16_t(mon_table[MON_RAT].move_f);
            lv.ents[m.ent].b = MON_RAT;
        }

        // the first upgrade comes early (DISENO.md 5: short sword on floors 1-2) and floor 1 always has a potion
        if(floor <= 1 && cd.p.weapon == 0 && ! sword && far_item >= 0)
        {
            if(lv.ents[far_item].b == ITEM_BREAD && far_item == near_bread)
            {
                near_bread = -1;
            }

            lv.ents[far_item].b = ITEM_SWORD;
        }

        if(floor == 0 && ! potion && near_bread >= 0)
        {
            lv.ents[near_bread].b = ITEM_POTION;
        }

        _setup_chests(false);

        cd.dist_from = -1;
        objects_dirty = true;
    }

    // ---- queries
    int combat::monster_ent_at(int cell) const
    {
        int i = cd.mon_at[cell];
        return i >= 0 ? cd.mons[i].ent : -1;
    }

    const monster* combat::monster_of_ent(int ent) const
    {
        const entity& e = _lv->ents[ent];

        if(e.type != E_MONSTER)
        {
            return nullptr;
        }

        int i = cd.mon_at[e.y * W + e.x];
        return i >= 0 && cd.mons[i].ent == ent ? &cd.mons[i] : nullptr;
    }

    int combat::kind_of_ent(int ent) const
    {
        int k = _lv->ents[ent].b;
        return k < MON_KIND_COUNT ? k : MON_RAT;
    }

    bool combat::monster_blocks(int cell) const
    {
        return cell >= 0 && cd.mon_at[cell] >= 0;
    }

    // ---- frame
    void combat::update(int cell, int dir, bool a_pressed, bool b_pressed, bool think, bool select_pressed)
    {
        bn::timer timer;
        player_stats& p = cd.p;
        ++_frame;

        if(cell != _cell)
        {
            cd.dist_from = -1;
        }

        _cell = cell;
        _dir = dir;

        if(p.hp <= 0)
        {
            return;
        }

        int rmax = reload_max();
        p.reload = p.reload < rmax ? p.reload + 1 : rmax;
        p.invuln = p.invuln > 0 ? p.invuln - 1 : 0;
        p.counter = p.counter > 0 ? p.counter - 1 : 0;
        p.busy = p.busy > 0 ? p.busy - 1 : 0;

        _magic_update(b_pressed, select_pressed);

        if(a_pressed)
        {
            if(p.reload >= rmax && ! p.busy)
            {
                _player_attack();
            }
            else if(bn::max(rmax - p.reload, p.busy) <= attack_buffer_frames)
            {
                // R2: kept when the bar is almost full and a cast / drink ends within the same 8 f (G-42); an
                // earlier press is dropped
                p.attack_buffered = true;
            }
        }
        else if(p.attack_buffered && p.reload >= rmax && ! p.busy)
        {
            _player_attack();
        }

        for(int i = cd.nmons - 1; i >= 0; --i)
        {
            _monster_update(i, think);
        }

        _think_ticks = timer.elapsed_ticks();
        _think_max = bn::max(_think_max, _think_ticks);
    }

    void combat::_player_attack()
    {
        player_stats& p = cd.p;
        p.attack_buffered = false;

        if(! _edge_pass(_cell, _dir, true))
        {
            return;
        }

        int f = neighbour(_cell, _dir);
        int mi = cd.mon_at[f];

        if(mi >= 0 && cd.mons[mi].state != MS_DYING)
        {
            monster& m = cd.mons[mi];
            const mon_stats& ms = mon_table[m.kind];
            int damage = bn::max(1, weapon_table[p.weapon].damage + p.str - ms.def);
            bool counter = p.counter > 0;
            bool crit = counter || _rng.get_int(100) < 10;

            if(crit)
            {
                damage *= 2;
                ++p.crits;
            }

            m.hp = int16_t(m.hp - damage);
            m.hit = hit_frames;
            p.reload = 0;
            p.counter = 0;
            ++p.attacks;
            _post(EV_MON_HIT, m.ent, damage, crit);
            sfx(crit ? bn::fixed(1.5) : bn::fixed(1.0), 0.8);

            if(m.state == MS_IDLE)
            {
                m.state = MS_CHASE;
            }

            _wake_near(hearing_cells);

            if(m.hp <= 0)
            {
                _kill(mi);
            }

            return;
        }

        // barrels and crates break (DISENO.md 6b): barrel 1 hit, crate 2
        int e = _ent_of_cell[f];

        if(e >= 0 && _lv->ents[e].type == E_FURN && (_lv->ents[e].c == FURN_BARREL || _lv->ents[e].c == FURN_CRATE))
        {
            entity& fe = _lv->ents[e];
            p.reload = 0;
            ++p.attacks;
            sfx(0.8, 0.7);

            if(fe.c == FURN_CRATE && fe.a < 8)
            {
                fe.a = 8;               // first hit on a crate (a = drawing orientation 0..3, unused by the game)
                return;
            }

            int r = _rng.get_int(100);
            int item = -1;

            if(fe.c == FURN_BARREL ? r < 30 : r < 40)
            {
                item = _rng.get_int(100) < (fe.c == FURN_BARREL ? 70 : 55) ? ITEM_BREAD : ITEM_POTION;
            }

            _drop(e, f, item);
        }
    }

    void combat::_wake_near(int radius)
    {
        int px = _cell & (W - 1), py = _cell >> 5;

        for(int i = 0; i < cd.nmons; ++i)
        {
            monster& m = cd.mons[i];

            if(m.state == MS_IDLE && bn::abs((m.cell & (W - 1)) - px) + bn::abs((m.cell >> 5) - py) <= radius)
            {
                m.state = MS_CHASE;
            }
        }
    }

    void combat::_monster_update(int index, bool think)
    {
        monster& m = cd.mons[index];
        const mon_stats& ms = mon_table[m.kind];
        m.hit = m.hit ? m.hit - 1 : 0;
        m.strike = m.strike ? m.strike - 1 : 0;

        switch(m.state)
        {
        case MS_DYING:
            if(--m.timer <= 0)
            {
                _remove(index);
            }
            return;

        case MS_IDLE:
            if(((_frame + index) & 7) == 0 && _sees_player(m))
            {
                m.state = MS_CHASE;
                m.move_timer = int16_t(ms.move_f / 2);
            }
            return;

        case MS_WARN:
            if(--m.timer <= 0)
            {
                _strike(m);
                m.state = MS_CHASE;
            }
            return;

        default:
            break;
        }

        // chasing
        m.attack_cd = m.attack_cd > 0 ? m.attack_cd - 1 : 0;
        int d = _dir_to(m.cell, _cell);
        bool adjacent = d >= 0 && _edge_pass(m.cell, d, true);

        if(adjacent)
        {
            if(m.attack_cd == 0 && m.hit == 0)
            {
                // telegraph: the blow will land on the cell the player stands on now (DISENO.md 3)
                int rel = (((d + 2) & 3) - _dir) & 3;     // where the monster is, seen from the player
                m.side = int8_t(rel == 0 ? 0 : rel == 1 ? 1 : rel == 3 ? -1 : 2);
                m.warn_len = uint8_t(rel == 0 ? ms.warn_f : ms.warn_f * 3 / 2);
                m.timer = m.warn_len;
                m.target = int16_t(_cell);
                m.state = MS_WARN;
                _mfx.telegraph(m.ent);
                sfx(0.5, 0.5);

                if(m.side != 0)
                {
                    if(m.side != 1)
                    {
                        _hud.show_damage_arrow(-1, m.warn_len);
                    }

                    if(m.side != -1)
                    {
                        _hud.show_damage_arrow(1, m.warn_len);
                    }
                }

                if(m.side == 0)
                {
                    // G-42 (DISENO 10): taught only with the monster in view; from the side the red arrow is enough
                    hint(HINT_DODGE, "STEP AWAY TO DODGE!");
                }
            }

            return;
        }

        if(m.move_timer > 0)
        {
            --m.move_timer;
            return;
        }

        if(cd.dist_from != _cell)
        {
            if(! think)
            {
                return;         // waits for a light frame
            }

            _bfs(_cell, false, chase_depth);
            cd.dist_from = _cell;
        }

        if(cd.dist[m.cell] == 255 && ! _sees_player(m))
        {
            m.state = MS_IDLE;  // lost the player
            return;
        }

        _monster_step(index);
    }

    void combat::_monster_step(int index)
    {
        monster& m = cd.mons[index];
        int best = -1;
        int best_dist = cd.dist[m.cell];

        for(int d = 0; d < 4; ++d)
        {
            if(! _edge_pass(m.cell, d, true))
            {
                continue;
            }

            int n = neighbour(m.cell, d);

            if(n == _cell || cd.mon_at[n] >= 0 || cd.dist[n] >= best_dist)
            {
                continue;
            }

            int e = _ent_of_cell[n];

            if(e >= 0 && _lv->ents[e].type == E_FURN && furn_blocks[_lv->ents[e].c < FURN_COUNT ? _lv->ents[e].c : 0])
            {
                continue;
            }

            best = n;
            best_dist = cd.dist[n];
        }

        m.move_timer = int16_t(mon_table[m.kind].move_f);

        if(best < 0)
        {
            return;
        }

        cd.mon_at[m.cell] = -1;
        cd.mon_at[best] = int16_t(index);
        m.cell = int16_t(best);
        _lv->ents[m.ent].x = uint8_t(best & (W - 1));
        _lv->ents[m.ent].y = uint8_t(best >> 5);
        objects_dirty = true;
    }

    void combat::_strike(monster& m)
    {
        player_stats& p = cd.p;
        m.strike = strike_frames;
        m.attack_cd = int16_t(mon_table[m.kind].recover_f);
        _wake_near(hearing_cells);

        if(_cell != m.target)
        {
            p.counter = counter_frames;     // DISENO.md 3: a blow within 30 f is a sure crit
            ++p.dodges;
            _post(EV_MON_MISS, m.ent, 0);
            return;
        }

        if(p.invuln > 0 || p.hp <= 0)
        {
            return;
        }

        int damage = bn::max(1, mon_table[m.kind].damage - p.def);
        p.hp = bn::max(0, p.hp - damage);
        p.invuln = invuln_frames;
        ++p.hits_taken;
        _post(EV_PLAYER_HIT, m.ent, damage, false, m.side);
        sfx(0.7, 0.9);
        _low_hp_hint();

        if(p.hp <= 0)
        {
            _post(EV_PLAYER_DEAD, -1, 0);
        }
    }

    void combat::trap_hit(int damage)
    {
        player_stats& p = cd.p;

        if(p.invuln > 0 || p.hp <= 0)
        {
            return;
        }

        p.hp = bn::max(0, p.hp - damage);
        p.invuln = invuln_frames;
        ++p.hits_taken;
        _post(EV_PLAYER_HIT, -1, damage, false, 0);
        sfx(0.7, 0.9);
        _low_hp_hint();

        if(p.hp <= 0)
        {
            _post(EV_PLAYER_DEAD, -1, 0);
        }
    }

    void combat::_kill(int index)
    {
        monster& m = cd.mons[index];
        m.state = MS_DYING;
        m.timer = death_frames;
        ++cd.p.kills;
        _post(EV_MON_DIED, m.ent, 0);
        _gain_xp(mon_table[m.kind].xp);
    }

    void combat::_remove(int index)
    {
        monster& m = cd.mons[index];
        const mon_stats& ms = mon_table[m.kind];
        int item = -1;

        if(_rng.get_int(100) < ms.loot_pct)
        {
            item = ms.loot;

            if(item == ITEM_SWORD && cd.p.weapon > 0)
            {
                item = ITEM_POTION;
            }
        }

        cd.mon_at[m.cell] = -1;
        _drop(m.ent, m.cell, item);
        int last = cd.nmons - 1;

        if(index != last)
        {
            cd.mons[index] = cd.mons[last];
            cd.mon_at[cd.mons[index].cell] = int16_t(index);
        }

        cd.nmons = last;
    }

    void combat::_drop(int ent, int cell, int item)
    {
        entity& e = _lv->ents[ent];

        if(_ent_of_cell[cell] == ent)
        {
            _ent_of_cell[cell] = -1;
        }

        if(item >= 0 && _ent_of_cell[cell] < 0)
        {
            e.type = E_ITEM;
            e.b = uint8_t(item);
            _ent_of_cell[cell] = int16_t(ent);
        }
        else
        {
            e.type = E_NONE;
        }

        objects_dirty = true;
    }

    void combat::_gain_xp(int xp)
    {
        player_stats& p = cd.p;
        p.xp += xp;

        while(p.level < max_level && p.xp >= xp_for_level(p.level + 1))
        {
            ++p.level;
            p.hp_max += 6;
            p.str += 1;

            if(! (p.level & 1))
            {
                p.def += 1;
            }

            p.hp = p.hp_max;

            if(cd.m.known)
            {
                cd.m.mp = mp_max();     // MAGIA 1.2: a level up refills the MP
            }

            _post(EV_LEVEL_UP, -1, p.level);
            _hud.message("LEVEL UP!", 150);
            sfx(2.0, 0.7);
        }
    }

    // ---- items
    void combat::pickup(int ent)
    {
        entity& e = _lv->ents[ent];
        int cell = e.y * W + e.x;
        int item = e.type == E_KEY ? int(ITEM_KEY) : int(e.b);

        if(! _give(item))
        {
            return;
        }

        e.type = E_NONE;

        if(_ent_of_cell[cell] == ent)
        {
            _ent_of_cell[cell] = -1;
        }

        objects_dirty = true;
        _post(EV_PICKUP, ent, item);
        sfx(1.6, 0.6);
    }
}

// Layout read by the headless route planner (tests/route.py)
static_assert(sizeof(dl::player_stats) == 76);
static_assert(sizeof(dl::monster) == 20);
static_assert(__builtin_offsetof(dl::combat_data, nmons) == 716);
static_assert(__builtin_offsetof(dl::combat_data, mon_at) == 720);
static_assert(__builtin_offsetof(dl::gen::level, nents) == 5248);
static_assert(__builtin_offsetof(dl::gen::level, entry) == 5372);
static_assert(__builtin_offsetof(dl::gen::level, stairs) == 5380);
