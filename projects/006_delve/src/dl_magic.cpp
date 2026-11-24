// DELVE G-40 block M1: mana, the belt (B uses, SELECT switches), Magic Missile / Light / Heal, scrolls, the Ring of
// Mana and normal / locked chests (MAGIA_Y_TESOROS.md 1.2-1.4, 2.1, 3.3, 6 M1). Member functions of dl::combat, so
// they share its monsters, events and RNG; the game draws the results (HUD, missile sprite, sparkles, pause).

#include "dl_combat.h"

#include "bn_fixed.h"
#include "bn_math.h"
#include "bn_string.h"

#include "bn_sound_items.h"

#include "dl_hud.h"

namespace dl
{
    using namespace gen;

    namespace
    {
        constexpr int hit_frames = 10;          // as dl_combat.cpp
        constexpr int drink_frames = 20;
        constexpr int mp_regen_f = 90;          // MAGIA 1.2
        constexpr int ring_regen_f = 72;        // MAGIA 3.3
        constexpr int belt_hint_delay = 130;
        constexpr int bag_hint_delay = 120;                 // G-42: after a plain pickup line (120 f)
        constexpr int bag_hint_after_hint_delay = 240;      // ... after a pickup hint (240 f)

        combat_data& cd = dl_combat_data;

        void sfx(bn::fixed speed, bn::fixed volume = 0.6)
        {
            bn::sound_items::blip.play(volume, speed, 0);
        }

        int cell_dist(int c)
        {
            return cd.dist[c];
        }
    }

    magic_state& combat::magic()
    {
        return cd.m;
    }

    int combat::mp_max() const
    {
        return 10 + 2 * (cd.p.level - 1) + (cd.m.ring == 2 ? 5 : 0);
    }

    int combat::mp_regen_frames() const
    {
        return cd.m.ring == 2 ? ring_regen_f : mp_regen_f;
    }

    bool combat::casting() const
    {
        return cd.m.cast_timer > 0;
    }

    const char* combat::item_name(int item)
    {
        switch(item)
        {
        case ITEM_BREAD:
            return "BREAD  +10 HP";
        case ITEM_POTION:
            return "HEALING POTION  +50% HP";
        case ITEM_SWORD:
            return "SHORT SWORD";
        case ITEM_KEY:
            return "IRON KEY";
        case ITEM_SCROLL_MISSILE:
            return "SCROLL: MAGIC MISSILE";
        case ITEM_SCROLL_LIGHT:
            return "SCROLL: LIGHT";
        case ITEM_SCROLL_HEAL:
            return "SCROLL: HEAL";
        case ITEM_RING_MANA:
            return "RING OF MANA  +5 MP";
        default:
            return "";
        }
    }

    // ---- per frame (called from combat::update, before the attack)
    void combat::_magic_update(bool b_pressed, bool select_pressed)
    {
        magic_state& m = cd.m;
        int mmax = mp_max();

        if(m.known)
        {
            if(m.mp >= mmax)
            {
                m.mp = mmax;
                m.mp_timer = 0;
            }
            else if(++m.mp_timer >= mp_regen_frames())
            {
                ++m.mp;
                m.mp_timer = 0;
            }
        }

        m.light = m.light > 0 ? m.light - 1 : 0;
        m.heal_fx = m.heal_fx > 0 ? m.heal_fx - 1 : 0;
        m.light_fx = m.light_fx > 0 ? m.light_fx - 1 : 0;

        if(m.hint_timer > 0 && --m.hint_timer == 0)
        {
            hint(HINT_SWITCH, "SELECT: SWITCH   B: CAST");
        }

        if(_next_hint_timer > 0 && --_next_hint_timer == 0 && _next_hint)
        {
            hint(HINT_BAG, _next_hint);
            _next_hint = nullptr;
        }

        if(m.cast_timer > 0 && --m.cast_timer == 0)
        {
            _cast_done();
        }

        _missile_update();

        if(select_pressed)
        {
            _belt_cycle();
        }

        if(b_pressed)
        {
            _belt_use();
        }
    }

    void combat::drink_potion()
    {
        player_stats& p = cd.p;

        if(p.busy)
        {
            return;
        }

        if(p.potions <= 0)
        {
            _hud.message("NO POTIONS", 90);
        }
        else if(p.hp >= p.hp_max)
        {
            _hud.message("HP IS FULL", 90);
        }
        else
        {
            int heal = bn::min(p.hp_max / 2, p.hp_max - p.hp);
            p.hp += heal;
            --p.potions;
            p.busy = drink_frames;
            bn::string<28> t("POTION: +");
            t.append(bn::to_string<4>(heal));
            t.append(" HP");
            _hud.message(t.c_str(), 120);
            sfx(1.3);
        }
    }

    void combat::_belt_use()
    {
        magic_state& m = cd.m;
        player_stats& p = cd.p;
        int entry = m.belt[m.belt_sel];

        if(entry < 0 || m.cast_timer > 0)
        {
            return;
        }

        if(entry == belt_potion)
        {
            drink_potion();
            return;
        }

        if(p.busy)
        {
            return;
        }

        int s = entry - 1;
        const spell_stats& st = spell_table[s];

        if(s == SPELL_HEAL && p.hp >= p.hp_max)
        {
            _hud.message("HP IS FULL", 90);
            return;
        }

        // MP first; a spare scroll pays when the MP does not reach (DISENO 10, G-42)
        if(m.mp >= st.mp)
        {
            m.mp -= st.mp;
            m.cast_free = false;
        }
        else if(m.scrolls[s] > 0)
        {
            --m.scrolls[s];
            m.cast_free = true;
        }
        else
        {
            _hud.message("NOT ENOUGH MANA", 90);
            sfx(0.5, 0.5);         // dull thud: nothing spent, nothing blocked
            return;
        }

        m.cast_spell = s;
        m.cast_timer = st.cast_f;
        p.busy = st.cast_f;         // no attack and no other use while casting; movement stays free (R1)
        ++m.casts;
        sfx(1.8, 0.5);
    }

    void combat::_belt_cycle()
    {
        magic_state& m = cd.m;

        if(m.cast_timer > 0)
        {
            return;
        }

        for(int k = 1; k <= belt_size; ++k)
        {
            int i = (m.belt_sel + k) % belt_size;

            if(m.belt[i] >= 0)
            {
                m.belt_sel = i;
                break;
            }
        }

        int entry = m.belt[m.belt_sel];
        bn::string<28> t;

        if(entry == belt_potion)
        {
            t.append("POTION  x");
            t.append(bn::to_string<4>(cd.p.potions));
        }
        else
        {
            const spell_stats& st = spell_table[entry - 1];
            t.append(st.name);
            t.append("  ");
            t.append(bn::to_string<4>(st.mp));
            t.append(" MP");
        }

        _hud.message(t.c_str(), 60);
        sfx(1.4, 0.3);
    }

    void combat::_cast_done()
    {
        magic_state& m = cd.m;
        player_stats& p = cd.p;

        switch(m.cast_spell)
        {
        case SPELL_MISSILE:
            m.missile_cell = _cell;
            m.missile_dir = _dir;   // the way the player faces now (MAGIA 1.3)
            m.missile_t = 0;
            m.missile_cells = 0;
            m.missile_hit = 0;
            sfx(2.2, 0.6);
            break;

        case SPELL_LIGHT:
            m.light = light_frames;
            m.light_fx = 8;
            _hud.message("LIGHT", 60);
            sfx(2.4, 0.5);
            break;

        case SPELL_HEAL:
        {
            int heal = bn::min((p.hp_max * 30 + 99) / 100, p.hp_max - p.hp);
            p.hp += heal;
            m.heal_fx = 20;
            bn::string<28> t("HEAL: +");
            t.append(bn::to_string<4>(heal));
            t.append(" HP");
            _hud.message(t.c_str(), 120);
            sfx(1.6, 0.6);
            break;
        }

        default:
            break;
        }

        m.cast_spell = -1;
    }

    void combat::_missile_update()
    {
        magic_state& m = cd.m;

        if(m.missile_hit > 0)
        {
            if(--m.missile_hit == 0)
            {
                m.missile_cell = -1;
            }

            return;
        }

        if(m.missile_cell < 0 || ++m.missile_t < missile_cell_f)
        {
            return;
        }

        m.missile_t = 0;

        if(m.missile_cells >= missile_range || ! _edge_pass(m.missile_cell, m.missile_dir, true))
        {
            m.missile_hit = 8;          // fizzles against the wall / at the end of its range
            return;
        }

        int next = neighbour(m.missile_cell, m.missile_dir);
        m.missile_cell = next;
        ++m.missile_cells;
        int mi = next >= 0 ? cd.mon_at[next] : -1;

        if(mi >= 0 && cd.mons[mi].state != MS_DYING)
        {
            monster& mon = cd.mons[mi];
            int damage = 6 + (cd.p.level - 1);     // ignores DEF (MAGIA 1.4)
            mon.hp = int16_t(mon.hp - damage);
            mon.hit = hit_frames;
            _post(EV_MON_HIT, mon.ent, damage, false);
            sfx(1.2, 0.8);

            if(mon.state == MS_IDLE)
            {
                mon.state = MS_CHASE;
            }

            if(mon.hp <= 0)
            {
                ++m.missile_kills;
                _kill(mi);
            }

            m.missile_hit = 8;
        }
    }

    void combat::_learn(int spell)
    {
        magic_state& m = cd.m;
        bool first = ! m.known;
        m.known |= 1 << spell;
        int slot = -1;

        for(int i = 0; i < belt_size && slot < 0; ++i)
        {
            if(m.belt[i] < 0)
            {
                slot = i;
            }
        }

        if(slot < 0)
        {
            slot = m.belt_sel != belt_potion ? m.belt_sel : belt_size - 1;
        }

        m.belt[slot] = int8_t(1 + spell);
        m.belt_sel = slot;

        if(first)
        {
            m.mp = mp_max();
            m.mp_timer = 0;
        }

        bn::string<28> t("LEARNED: ");
        t.append(spell_table[spell].name);
        _hud.message(t.c_str(), 120);

        if(! (_hints & HINT_SWITCH))
        {
            m.hint_timer = belt_hint_delay;
        }

        sfx(2.0, 0.7);
    }

    void combat::toggle_ring()
    {
        magic_state& m = cd.m;

        if(m.ring == 0)
        {
            return;
        }

        m.ring = m.ring == 1 ? 2 : 1;
        m.mp = bn::min(m.mp, mp_max());
        sfx(m.ring == 2 ? bn::fixed(1.8) : bn::fixed(1.0), 0.5);
    }

    // ---- items
    bool combat::_give(int item)
    {
        player_stats& p = cd.p;
        magic_state& m = cd.m;
        int hints_before = _hints;

        switch(item)
        {
        case ITEM_KEY:
            ++p.keys;
            _hud.message("IRON KEY", 120);
            break;

        case ITEM_BREAD:
            p.hp = bn::min(p.hp_max, p.hp + 10);
            _hud.message("BREAD: +10 HP", 120);
            break;

        case ITEM_POTION:
            if(p.potions >= 9)
            {
                _hud.message("BAG FULL", 90);
                return false;
            }

            ++p.potions;

            if(_hints & HINT_POTION)
            {
                _hud.message("HEALING POTION", 120);
            }
            else
            {
                hint(HINT_POTION, "POTION! B: DRINK IT");
            }
            break;

        case ITEM_SWORD:
            if(p.weapon == 0)
            {
                p.weapon = 1;
                p.reload = 0;
                _hud.message("SHORT SWORD! MORE DAMAGE", 150);
            }
            else
            {
                _hud.message("A SPARE SWORD. LEFT IT.", 90);
            }
            break;

        case ITEM_SCROLL_MISSILE:
        case ITEM_SCROLL_LIGHT:
        case ITEM_SCROLL_HEAL:
        {
            int s = item - ITEM_SCROLL_MISSILE;

            if(! (m.known & (1 << s)))
            {
                _learn(s);
                break;
            }

            if(m.scrolls[s] >= 9)
            {
                _hud.message("BAG FULL", 90);
                return false;
            }

            ++m.scrolls[s];
            bn::string<28> t("SCROLL: FREE ");
            t.append(spell_table[s].name);
            _hud.message(t.c_str(), 120);
            break;
        }

        case ITEM_RING_MANA:
            if(m.ring)
            {
                _hud.message("A SPARE RING. LEFT IT.", 90);
            }
            else
            {
                m.ring = 1;
                _hud.message("RING OF MANA! START: WEAR", 150);
            }
            break;

        default:
            break;
        }

        // G-42 (DISENO 10): the first thing kept in the bag teaches START, after the pickup line (and after the
        // pickup hint when there is one)
        bool to_bag = item == ITEM_KEY || item == ITEM_POTION || item == ITEM_RING_MANA ||
                ((item == ITEM_SCROLL_MISSILE || item == ITEM_SCROLL_LIGHT || item == ITEM_SCROLL_HEAL) &&
                 m.scrolls[item - ITEM_SCROLL_MISSILE] > 0);

        if(to_bag && ! (_hints & HINT_BAG) && ! _next_hint)
        {
            _next_hint = "START: BAG";
            _next_hint_timer = (_hints & ~hints_before) ? bag_hint_after_hint_delay : bag_hint_delay;

            if(item == ITEM_RING_MANA)
            {
                _next_hint_timer = bag_hint_after_hint_delay;   // its line already says START: WEAR
            }
        }

        return true;
    }

    // ---- chests (MAGIA 2.1: normal and locked; walking against one opens it)
    void combat::open_chest(int ent)
    {
        magic_state& m = cd.m;
        entity& e = _lv->ents[ent];
        int index = -1;

        for(int i = 0; i < m.nchests; ++i)
        {
            if(m.chest_ent[i] == ent)
            {
                index = i;
            }
        }

        if(index < 0 || (e.a & chest_opened))
        {
            return;
        }

        if(e.a & chest_locked)
        {
            if(cd.p.keys <= 0)
            {
                _hud.message("LOCKED. FIND A KEY.", 120);
                sfx(0.5, 0.5);
                return;
            }

            --cd.p.keys;
        }

        if(e.a & chest_trapped)
        {
            // M2-a (MAGIA 2.1): hiss and a shaking lid for 24 f, then needles on the cell in front; stepping away
            // (back or aside, 12 f) dodges them. The loot comes out after the needles either way
            if(m.trap_timer > 0)
            {
                return;
            }

            m.trap_timer = trap_warn_f;
            m.trap_ent = ent;
            m.trap_cell = _cell;
            m.trap_index = index;
            objects_dirty = true;
            _hud.message("HISS! STEP AWAY!", 60);
            sfx(2.2, 0.5);
            return;
        }

        if(! _give(m.chest_item[index]))
        {
            return;     // bag full: the chest stays shut
        }

        e.a = uint8_t(e.a | chest_opened);
        objects_dirty = true;
        _post(EV_PICKUP, ent, m.chest_item[index]);
        sfx(1.1, 0.7);
    }

    void combat::_trap_update()
    {
        magic_state& m = cd.m;

        if(m.trap_timer <= 0 || --m.trap_timer > 0)
        {
            return;
        }

        entity& e = _lv->ents[m.trap_ent];
        e.a = uint8_t(e.a & ~chest_trapped);    // fired: from now on a normal chest

        if(_cell == m.trap_cell && cd.p.hp > 0)
        {
            ++m.trap_hits;
            _hud.message("NEEDLES!", 60);
            trap_hit(6 + 2 * (_floor + 1));     // MAGIA 2.1: 6 + 2 x floor, ignores DEF
        }
        else
        {
            ++m.trap_dodges;
            _post(EV_MON_MISS, m.trap_ent, 0);
            sfx(1.6, 0.5);
        }

        if(cd.p.hp > 0 && _give(m.chest_item[m.trap_index]))
        {
            e.a = uint8_t(e.a | chest_opened);
            _post(EV_PICKUP, m.trap_ent, m.chest_item[m.trap_index]);
        }

        m.trap_ent = -1;
        objects_dirty = true;
    }

    void combat::_wake_mimic(monster& m)
    {
        // MAGIA 2.1: teeth and a white blink for 30 f before the first bite (R5); it never moves
        m.state = MS_CHASE;
        m.attack_cd = 0;
        _hud.message("A MIMIC!", 90);
        sfx(0.4, 0.8);
        objects_dirty = true;
    }

    void combat::touch(int cell)
    {
        int mi = cd.mon_at[cell];

        if(mi >= 0 && cd.mons[mi].kind == MON_MIMIC && cd.mons[mi].state == MS_IDLE)
        {
            _wake_mimic(cd.mons[mi]);
        }
    }

    // M2 (MAGIA 2.1): trapped chests from floor 2 (20 %, 25 % from 4) among the normal ones and one mimic from floor
    // 3 (15 %, 20 % from 4), never the first chest, a scroll chest or a dead end (always somewhere to step back)
    void combat::_setup_chest_kinds()
    {
        magic_state& m = cd.m;
        level& lv = *_lv;
        int trap_pct = _floor < 1 ? 0 : _floor < 3 ? 20 : 25;
        int mimic_pct = _floor < 2 ? 0 : _floor < 3 ? 15 : 20;
        auto scroll = [](int item) { return item >= ITEM_SCROLL_MISSILE && item <= ITEM_SCROLL_HEAL; };

        if(mimic_pct && _rng.get_int(100) < mimic_pct)
        {
            for(int i = m.nchests - 1; i >= 1; --i)
            {
                entity& e = lv.ents[m.chest_ent[i]];
                int c = e.y * W + e.x;
                int sides = 0;

                for(int d = 0; d < 4; ++d)
                {
                    sides += _edge_pass(c, d, true) ? 1 : 0;
                }

                if((e.a & chest_locked) || scroll(m.chest_item[i]) || sides < 2 || cd.nmons >= max_monsters ||
                   cd.mon_at[c] >= 0)
                {
                    continue;
                }

                // the chest entity becomes the monster (drawn with prop_mimic, frame 0 == a closed chest)
                _ent_of_cell[c] = -1;
                e.type = E_MONSTER;
                e.b = MON_MIMIC;
                cd.mons[cd.nmons] = monster{ m.chest_ent[i], int16_t(c), MON_MIMIC, MS_IDLE,
                                             int16_t(mon_table[MON_MIMIC].hp), 0, 0, 0, -1, 0, 0, 0, 0 };
                cd.mon_at[c] = int16_t(cd.nmons++);
                ++m.mimics;

                for(int k = i; k + 1 < m.nchests; ++k)
                {
                    m.chest_ent[k] = m.chest_ent[k + 1];
                    m.chest_item[k] = m.chest_item[k + 1];
                }

                --m.nchests;
                break;
            }
        }

        for(int i = 0; i < m.nchests && trap_pct; ++i)
        {
            entity& e = lv.ents[m.chest_ent[i]];

            if(! (e.a & chest_locked) && ! scroll(m.chest_item[i]) && _rng.get_int(100) < trap_pct)
            {
                e.a = uint8_t(e.a | chest_trapped);
            }
        }
    }

    void combat::_setup_chests(bool test_room)
    {
        magic_state& m = cd.m;
        level& lv = *_lv;
        m.nchests = 0;
        _bfs(lv.entry, true, 250);

        if(test_room)
        {
            // build_test_room order: normal chest (Heal scroll), locked chest (Ring of Mana), trapped chest (potion)
            static constexpr uint8_t contents[3] = { ITEM_SCROLL_HEAL, ITEM_RING_MANA, ITEM_POTION };

            for(int i = 0; i < lv.nents && m.nchests < 3; ++i)
            {
                if(lv.ents[i].type == E_FURN && lv.ents[i].c == FURN_CHEST)
                {
                    m.chest_ent[m.nchests] = int16_t(i);
                    m.chest_item[m.nchests] = contents[m.nchests];
                    ++m.nchests;
                }
            }

            cd.dist_from = -1;
            return;
        }

        // chests the generator placed, reachable, nearest first
        int want = _floor == 0 ? 2 : 3;

        // pass 2: still too few, so an item lying in a dead end (nothing behind it) or in the open middle of a room
        // (3+ open sides) becomes a chest: a blocking chest there never cuts a path
        auto open_sides = [&lv](int c)
        {
            int n = 0;

            for(int d = 0; d < 4; ++d)
            {
                n += graph_kind[kind_at(lv, c, d)] ? 1 : 0;
            }

            return n;
        };

        // G-42 (DISENO 10): a fallback chest never hides a crack (SECRET edge) at the end of a dead end
        auto secret_wall = [&lv](int c)
        {
            for(int d = 0; d < 4; ++d)
            {
                if(kind_at(lv, c, d) == K_SECRET)
                {
                    return true;
                }
            }

            return false;
        };

        for(int pass = 0; pass < 3; ++pass)
        {
            for(int i = 0; i < lv.nents && m.nchests < want; ++i)
            {
                entity& e = lv.ents[i];
                int c = e.y * W + e.x;

                if((e.type != E_FURN && ! (pass == 2 && e.type == E_ITEM)) || cell_dist(c) == 255 ||
                   _ent_of_cell[c] != i)
                {
                    continue;
                }

                bool taken = false;

                for(int k = 0; k < m.nchests; ++k)
                {
                    taken |= m.chest_ent[k] == i;
                }

                // pass 1: too few chests, so room furniture that blocks anyway becomes one (MAGIA 6 M1: 2-3 on floor 1)
                int sides = pass == 2 ? open_sides(c) : 0;
                bool ok = ! taken && (pass == 0 ? e.c == FURN_CHEST :
                          pass == 1 ? (e.c == FURN_CRATE || e.c == FURN_BARREL || e.c == FURN_STATUE) && lv.th[c] &&
                                      cell_dist(c) > 3 :
                          cell_dist(c) > 3 && ! secret_wall(c) && (sides == 1 || (lv.th[c] && sides >= 3)));

                if(ok)
                {
                    e.type = E_FURN;
                    e.c = FURN_CHEST;
                    e.a = uint8_t(e.a & 3);
                    m.chest_ent[m.nchests++] = int16_t(i);
                }
            }
        }

        // nearest first
        for(int i = 1; i < m.nchests; ++i)
        {
            for(int j = i; j > 0; --j)
            {
                const entity& a = lv.ents[m.chest_ent[j - 1]];
                const entity& b = lv.ents[m.chest_ent[j]];

                if(cell_dist(a.y * W + a.x) > cell_dist(b.y * W + b.x))
                {
                    int16_t t = m.chest_ent[j - 1];
                    m.chest_ent[j - 1] = m.chest_ent[j];
                    m.chest_ent[j] = t;
                }
            }
        }

        auto unknown_scroll = [this, &m]() -> int
        {
            int start = _rng.get_int(SPELL_COUNT);

            for(int k = 0; k < SPELL_COUNT; ++k)
            {
                int s = (start + k) % SPELL_COUNT;

                if(! (m.known & (1 << s)))
                {
                    return ITEM_SCROLL_MISSILE + s;
                }
            }

            return -1;
        };

        for(int i = 0; i < m.nchests; ++i)
        {
            int item;

            if(i == 0 && _floor == 0)
            {
                item = ITEM_SCROLL_MISSILE;     // MAGIA 1.1: the guaranteed scroll, in a normal chest, early
            }
            else if(i == 1 && _floor == 0)
            {
                item = _rng.get_int(2) ? ITEM_SCROLL_LIGHT : ITEM_SCROLL_HEAL;
            }
            else
            {
                int u = unknown_scroll();
                int r = _rng.get_int(100);
                item = u >= 0 && r < 50 ? u : r < 75 ? int(ITEM_POTION) : int(ITEM_BREAD);
            }

            m.chest_item[i] = uint8_t(item);
        }

        // the farthest chest is the locked one when its key can lie nearer the entry (MAGIA 2.1, R7)
        if(m.nchests >= 2)
        {
            int last = m.nchests - 1;
            entity& ce = lv.ents[m.chest_ent[last]];
            int chest_d = cell_dist(ce.y * W + ce.x);
            int key = -1;

            for(int i = 0; i < lv.nents; ++i)
            {
                const entity& e = lv.ents[i];
                int c = e.y * W + e.x;

                if(e.type == E_ITEM && e.b == ITEM_BREAD && cell_dist(c) < chest_d && cell_dist(c) > 2 &&
                   (key < 0 || cell_dist(c) > cell_dist(lv.ents[key].y * W + lv.ents[key].x)))
                {
                    key = i;
                }
            }

            if(key >= 0)
            {
                int other = m.chest_item[last];
                lv.ents[key].type = E_KEY;
                ce.a = uint8_t(ce.a | chest_locked);
                m.chest_item[last] = uint8_t(! m.ring ? int(ITEM_RING_MANA) : other);

                if(_floor == 0 && last == 1)
                {
                    // the Light / Heal scroll of chest 2 moves to the floor (below)
                    m.chest_item[0] = ITEM_SCROLL_MISSILE;
                }
            }
        }

        // floor 1: the Light / Heal scrolls the chests did not give lie on the floor (3 scrolls, MAGIA 6 M1)
        if(_floor == 0)
        {
            for(int s = SPELL_LIGHT; s < SPELL_COUNT; ++s)
            {
                bool given = false;

                for(int i = 0; i < m.nchests; ++i)
                {
                    given |= m.chest_item[i] == ITEM_SCROLL_MISSILE + s;
                }

                for(int i = lv.nents - 1; i >= 0 && ! given; --i)
                {
                    entity& e = lv.ents[i];

                    if(e.type == E_ITEM && e.b == ITEM_BREAD && cell_dist(e.y * W + e.x) != 255)
                    {
                        e.b = uint8_t(ITEM_SCROLL_MISSILE + s);
                        given = true;
                    }
                }
            }
        }

        _setup_chest_kinds();
        cd.dist_from = -1;
    }
}
