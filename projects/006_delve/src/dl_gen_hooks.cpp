// DELVE game hooks on top of the v2 floor (DISENO.md 6 and 6b), the game validation and the independent solver.
// Only the second PRNG stream is used (the v2 reserves dunRngGame for this), so the v2 arrays are never re-rolled:
// hooks reinterpret (trap / furniture type), add (P1 lever + bars) or remove / change what breaks a hard rule.

#include "dl_gen.h"

#include <string.h>

#ifdef DL_HOST
    #define DL_EWRAM
#else
    #define DL_EWRAM __attribute__((section(".ewram")))
#endif

namespace dl::gen
{
namespace
{
    struct hook_state
    {
        uint16_t dist[N];
        uint16_t dist2[N];
        uint16_t q[N];
        uint8_t blocked[N];
        uint8_t seen[N];
        uint8_t hooked[N];      // 1 = lever cell, 2 = bars cell
    };

    DL_EWRAM hook_state hs;

    bool carved(const level& lv, int c)
    {
        return lv.cl[c] & C_CARVED;
    }

    // BFS through GRAPH edges, not entering blocked cells, up to depth limit. Returns the number of cells reached.
    int bfs(const level& lv, int start, uint16_t* dist, int limit, const uint8_t* blocked)
    {
        for(int c = 0; c < N; ++c)
        {
            dist[c] = 0xFFFF;
        }

        dist[start] = 0;
        hs.q[0] = uint16_t(start);
        int nq = 1;
        int i = 0;

        while(i < nq)
        {
            int c = hs.q[i++];

            if(dist[c] >= limit)
            {
                continue;
            }

            for(int d = 0; d < 4; ++d)
            {
                int ei = edge_index(c, d);

                if(ei >= 0 && graph_kind[lv.ed[ei] & 15])
                {
                    int nb = neighbour(c, d);

                    if(dist[nb] == 0xFFFF && ! (blocked && blocked[nb]))
                    {
                        dist[nb] = uint16_t(dist[c] + 1);
                        hs.q[nq++] = uint16_t(nb);
                    }
                }
            }
        }

        return nq;
    }

    int carved_count(const level& lv)
    {
        int n = 0;

        for(int c = 0; c < N; ++c)
        {
            n += carved(lv, c);
        }

        return n;
    }

    // a lever / bars cell or a cell next to a door, lock, bars or stairs edge: nothing may cover it
    bool use_cell(const level& lv, int c)
    {
        if(hs.hooked[c])
        {
            return true;
        }

        for(int d = 0; d < 4; ++d)
        {
            int k = kind_at(lv, c, d);

            if(k == K_DOOR || k == K_LOCKED || k == K_BARS || k == K_STAIRS)
            {
                return true;
            }
        }

        return false;
    }

    // a room cell with a passable side to a cell outside its room
    bool room_opening(const level& lv, int c)
    {
        if(! lv.th[c])
        {
            return false;
        }

        for(int d = 0; d < 4; ++d)
        {
            int nb = neighbour(c, d);

            if(nb >= 0 && graph_kind[kind_at(lv, c, d)] && (lv.th[nb] & 15) != (lv.th[c] & 15))
            {
                return true;
            }
        }

        return false;
    }

    void remove_entity(level& lv, entity& e)
    {
        int c = e.y * W + e.x;
        e.type = E_NONE;
        lv.cl[c] = uint8_t(lv.cl[c] & ~(C_ENT | 7));
    }

    // P1 shortcut: a WALL between carved a and b (>= p1_min_steps apart by path) becomes BARS; the lever goes on a
    // plain wall of b (the far one) perpendicular to the bars, so the bars are the side wall of the lever cell.
    void place_p1(level& lv, v2_rng& rg, const floor_table& ft, int maxd)
    {
        for(int n = 0; n < ft.p1_count && lv.nhooks < max_hooks; ++n)
        {
            for(int tr = 0; tr < 512; ++tr)
            {
                int c = rg.rnd(N);
                int d = rg.rnd(4);
                int idx = edge_index(c, d);

                if(idx < 0 || (lv.ed[idx] & 15) != K_WALL)
                {
                    continue;
                }

                int a = edge_a(idx);
                int b = edge_b(idx);

                if(! carved(lv, a) || ! carved(lv, b) || hs.hooked[a] || hs.hooked[b])
                {
                    continue;
                }

                if(hs.dist[a] > hs.dist[b])
                {
                    int t = a;
                    a = b;
                    b = t;
                }

                if(hs.dist[a] < 6 || b == lv.stairs || a == lv.stairs || a == lv.entry || b == lv.entry)
                {
                    continue;
                }

                if(hs.dist[b] * 3 < maxd)
                {
                    continue;   // lever in the middle or far third
                }

                bfs(lv, a, hs.dist2, 0xFFFF, nullptr);

                if(hs.dist2[b] < ft.p1_min_steps)
                {
                    continue;
                }

                int dba = 0;

                while(neighbour(b, dba) != a)
                {
                    ++dba;
                }

                int sides[2] = { (dba + 1) & 3, (dba + 3) & 3 };
                int first = rg.rnd(2);
                int side = -1;

                for(int k = 0; k < 2; ++k)
                {
                    int s = sides[(first + k) & 1];
                    int ei = edge_index(b, s);

                    if(ei >= 0 && (lv.ed[ei] & 15) == K_WALL)
                    {
                        side = s;
                        break;
                    }
                }

                if(side < 0)
                {
                    continue;
                }

                lv.ed[idx] = K_BARS;
                lv.hooks[lv.nhooks++] = hook{ H_LEVER, uint8_t(b % W), uint8_t(b / W), uint8_t(side), int16_t(idx),
                                              int16_t(edge_index(b, side)) };
                hs.hooked[b] = 1;
                hs.hooked[a] = 2;
                break;
            }
        }
    }

    bool connected_without(const level& lv, int total)
    {
        int nb = 0;

        for(int c = 0; c < N; ++c)
        {
            nb += hs.blocked[c];
        }

        return bfs(lv, lv.entry, hs.dist2, 0xFFFF, hs.blocked) == total - nb;
    }

    // cells reached from the entry through FLOOD kinds, LOCKED edges passable only for lock index `open_lock`
    int flood_locks(const level& lv, int open_lock)
    {
        memset(hs.seen, 0, N);
        hs.seen[lv.entry] = 1;
        hs.q[0] = uint16_t(lv.entry);
        int nq = 1;
        int i = 0;

        while(i < nq)
        {
            int c = hs.q[i++];

            for(int d = 0; d < 4; ++d)
            {
                int ei = edge_index(c, d);

                if(ei < 0)
                {
                    continue;
                }

                int nb = neighbour(c, d);
                bool pass = flood_kind[lv.ed[ei] & 15] || (open_lock >= 0 && lv.locks[open_lock].edge == ei);

                if(pass && ! hs.seen[nb])
                {
                    hs.seen[nb] = 1;
                    hs.q[nq++] = uint16_t(nb);
                }
            }
        }

        return nq;
    }

    // DISENO.md 6 (R9, R12): the entry -> stairs path never crosses a locked door. A lock that cuts it becomes a
    // plain DOOR and its key is removed (hooks change instead of re-rolling).
    void fix_locks(level& lv)
    {
        for(int guard = 0; guard < max_locks; ++guard)
        {
            int base = flood_locks(lv, -1);

            if(hs.seen[lv.stairs])
            {
                return;
            }

            int best = -1;

            for(int j = 0; j < lv.nlocks && best < 0; ++j)
            {
                if((lv.ed[lv.locks[j].edge] & 15) == K_LOCKED && flood_locks(lv, j) > base)
                {
                    best = j;
                }
            }

            if(best < 0)
            {
                return;
            }

            lv.ed[lv.locks[best].edge] = K_DOOR;

            for(int i = 0; i < lv.nents; ++i)
            {
                entity& e = lv.ents[i];

                if(e.type == E_KEY && e.c == best)
                {
                    remove_entity(lv, e);
                }
            }

            ++lv.hook_removed_locks;
        }
    }

    void fix_traps(level& lv, const floor_table& ft, int level_index)
    {
        int kept[16];
        int nk = 0;
        int total = carved_count(lv);

        for(int i = 0; i < lv.nents; ++i)
        {
            entity& e = lv.ents[i];

            if(e.type != E_TRAP)
            {
                continue;
            }

            int c = e.y * W + e.x;
            int type = ft.trap_of_t[e.b < 3 ? e.b : 0];
            e.c = uint8_t(type);
            bool ok = nk < ft.max_traps && nk < 16;
            ok = ok && hs.dist[c] >= ft.trap_min_entry && c != lv.entry && c != lv.stairs && ! use_cell(lv, c);

            if(ok)      // never in a bend: the two open sides are opposite
            {
                bool ns = graph_kind[kind_at(lv, c, 0)] && graph_kind[kind_at(lv, c, 2)];
                bool ew = graph_kind[kind_at(lv, c, 1)] && graph_kind[kind_at(lv, c, 3)];
                ok = ns || ew;
            }

            if(ok && ! trap_dodgeable[type])    // never the only passage
            {
                memset(hs.blocked, 0, N);
                hs.blocked[c] = 1;
                ok = connected_without(lv, total);
            }

            if(ok)
            {
                bfs(lv, c, hs.dist2, 4, nullptr);

                for(int k = 0; k < nk && ok; ++k)
                {
                    ok = hs.dist2[kept[k]] >= ft.trap_spacing;
                }

                if(ok && ft.monster_trap_gap && level_index < 2)
                {
                    for(int k = 0; k < lv.nents && ok; ++k)
                    {
                        const entity& m = lv.ents[k];
                        ok = ! (m.type == E_MONSTER && hs.dist2[m.y * W + m.x] < ft.monster_trap_gap);
                    }
                }
            }

            if(ok)
            {
                static constexpr uint8_t floor_of_trap[TRAP_COUNT] = { 1, 2, 3, 5 };
                kept[nk++] = c;
                lv.cl[c] = uint8_t((lv.cl[c] & ~7) | floor_of_trap[type]);
            }
            else
            {
                remove_entity(lv, e);
                ++lv.hook_removed_traps;
            }
        }
    }

    void fix_furniture(level& lv, const floor_table& ft)
    {
        int total = carved_count(lv);
        memset(hs.blocked, 0, N);

        for(int i = 0; i < lv.nents; ++i)
        {
            entity& e = lv.ents[i];

            if(e.type != E_FURN)
            {
                continue;
            }

            int c = e.y * W + e.x;
            e.c = ft.furn_of_b[e.b & 7];

            if(c == lv.entry || use_cell(lv, c))
            {
                remove_entity(lv, e);
                ++lv.hook_changed_furn;
                continue;
            }

            if(! furn_blocks[e.c])
            {
                continue;
            }

            bool dead_end_barrel = e.c == FURN_BARREL && ! lv.th[c];
            bool ok = (lv.th[c] && ! room_opening(lv, c)) || dead_end_barrel;

            if(ok)
            {
                hs.blocked[c] = 1;
                ok = connected_without(lv, total);

                if(! ok)
                {
                    hs.blocked[c] = 0;
                }
            }

            if(! ok)
            {
                e.c = FURN_BONES;
                ++lv.hook_changed_furn;
            }
        }
    }
}

void apply_hooks(unsigned seed32, int level_index, level& lv)
{
    v2_rng rg;
    rg.seed(seed32 ^ 0x6A4Du, level_index, 7);     // the game stream (dunRngGame), never the v2 one
    const int ti = 0;                               // block 1: floor 1 table on every floor (DISENO.md 6b)
    const floor_table& ft = floor_tables[ti];
    lv.floor_table = ti;
    lv.nhooks = 0;
    lv.hook_removed_traps = 0;
    lv.hook_changed_furn = 0;
    lv.hook_removed_locks = 0;
    memset(hs.hooked, 0, N);
    bfs(lv, lv.entry, hs.dist, 0xFFFF, nullptr);
    int maxd = 0;

    for(int c = 0; c < N; ++c)
    {
        if(hs.dist[c] != 0xFFFF && hs.dist[c] > maxd)
        {
            maxd = hs.dist[c];
        }
    }

    fix_locks(lv);
    place_p1(lv, rg, ft, maxd);
    fix_traps(lv, ft, level_index);
    fix_furniture(lv, ft);
}

bool solvable(const level& lv, bool stairs_without_keys)
{
    uint8_t* blocked = hs.blocked;
    memset(blocked, 0, N);

    for(int i = 0; i < lv.nents; ++i)
    {
        const entity& e = lv.ents[i];

        if(e.type == E_FURN && furn_blocks[e.c < FURN_COUNT ? e.c : 0])
        {
            blocked[e.y * W + e.x] = 1;
        }
    }

    int nl = lv.nlocks;

    for(int pass = stairs_without_keys ? 0 : 1; pass < 2; ++pass)
    {
        bool use_keys = pass == 1;
        bool lock_open[max_locks] = {};

        for(int j = 0; j < nl; ++j)
        {
            lock_open[j] = (lv.ed[lv.locks[j].edge] & 15) != K_LOCKED;     // turned into a door by fix_locks
        }

        bool lever_on[max_hooks] = {};
        bool progress = true;

        while(progress)
        {
            progress = false;
            memset(hs.seen, 0, N);
            hs.seen[lv.entry] = 1;
            hs.q[0] = uint16_t(lv.entry);
            int nq = 1;
            int i = 0;

            while(i < nq)
            {
                int c = hs.q[i++];

                for(int d = 0; d < 4; ++d)
                {
                    int ei = edge_index(c, d);

                    if(ei < 0)
                    {
                        continue;
                    }

                    int k = lv.ed[ei] & 15;
                    int nb = neighbour(c, d);

                    if(hs.seen[nb] || blocked[nb])
                    {
                        continue;
                    }

                    bool pass_edge = flood_kind[k];

                    for(int j = 0; j < nl && ! pass_edge && k == K_LOCKED; ++j)
                    {
                        pass_edge = lock_open[j] && lv.locks[j].edge == ei;
                    }

                    for(int j = 0; j < lv.nhooks && ! pass_edge && k == K_BARS; ++j)
                    {
                        pass_edge = lever_on[j] && lv.hooks[j].target == ei;
                    }

                    if(pass_edge)
                    {
                        hs.seen[nb] = 1;
                        hs.q[nq++] = uint16_t(nb);
                    }
                }
            }

            for(int j = 0; j < lv.nhooks; ++j)
            {
                if(! lever_on[j] && hs.seen[lv.hooks[j].cell_y * W + lv.hooks[j].cell_x])
                {
                    lever_on[j] = true;
                    progress = true;
                }
            }

            if(use_keys && ! progress)
            {
                int have[16] = {};

                for(int k = 0; k < lv.nents; ++k)
                {
                    const entity& e = lv.ents[k];

                    if(e.type == E_KEY && hs.seen[e.y * W + e.x])
                    {
                        ++have[e.b & 15];
                    }
                }

                for(int j = 0; j < nl; ++j)
                {
                    if(lock_open[j])
                    {
                        --have[lv.locks[j].kind];
                    }
                }

                for(int j = 0; j < nl; ++j)
                {
                    int e = lv.locks[j].edge;

                    if(! lock_open[j] && have[lv.locks[j].kind] > 0 && (hs.seen[edge_a(e)] || hs.seen[edge_b(e)]))
                    {
                        lock_open[j] = true;
                        progress = true;
                        break;
                    }
                }
            }
        }

        if(! hs.seen[lv.stairs])
        {
            return false;
        }

        if(use_keys)
        {
            for(int j = 0; j < nl; ++j)
            {
                if(! lock_open[j])
                {
                    return false;
                }
            }

            for(int j = 0; j < lv.nhooks; ++j)
            {
                if(! lever_on[j])
                {
                    return false;
                }
            }
        }
    }

    return true;
}

unsigned generate_floor(unsigned seed32, int level_index, level& out)
{
    unsigned s = seed32;

    for(int k = 0; k < 8; ++k)
    {
        s = seed32 + unsigned(k);
        generate_v2(s, level_index, game_tables, out);
        apply_hooks(s, level_index, out);

        if(solvable(out, true))
        {
            break;
        }
    }

    return s;
}
}
