// DELVE floor generator. Port of Desktop/SNES/tools/scripts/dungeon_model.py v2 (class Gen, phases layout, join,
// cull, entry, door, stairs, lock, entity, decor, validate; the Rng class), followed by the game hooks phase
// (DISENO.md 6b) that only uses the second PRNG stream (the v2's reserved dunRngGame) so the v2 output is unchanged.
// Comments "v2:" point at the Python method each block mirrors. Python lists become fixed arrays in the same order.

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
    constexpr int NE_MAX = 2 * N;

    constexpr int pop4(int m)
    {
        return (m & 1) + ((m >> 1) & 1) + ((m >> 2) & 1) + ((m >> 3) & 1);
    }

    constexpr int nth4(int m, int k)
    {
        for(int d = 0; d < 4; ++d)
        {
            if(m >> d & 1)
            {
                if(k == 0)
                {
                    return d;
                }

                --k;
            }
        }

        return 0;
    }

    int edge_dir(int idx)
    {
        return idx < N ? 1 : 2;
    }

    int floor_div(int a, int b)
    {
        int q = a / b;
        return (a % b != 0 && ((a < 0) != (b < 0))) ? q - 1 : q;
    }

    void* fail_jump[5];    // a failed phase aborts the attempt (v2: raise Fail); GCC builtin, no libc / exceptions

    // The generator state (v2: Gen.__init__). Lives in EWRAM.
    struct gen_state
    {
        level* lv;
        const band* bd;
        const tables* tb;
        int level_index;
        bool fb;
        v2_rng r;
        bool failed;

        uint8_t res[N];
        uint8_t deg[N];
        uint8_t occ[N];
        uint8_t car[N];
        uint8_t allow[N];
        uint8_t back[N];
        uint8_t lvl[N];
        uint8_t reg[N];
        uint16_t dist[N];
        int16_t uf[N];
        int16_t par[N];
        uint16_t q[N];
        int nq;
        uint16_t cells[N];
        int ncells;
        uint16_t list[NE_MAX];
        uint16_t list2[NE_MAX];
        int16_t disc[N];
        int16_t low[N];
        int16_t sz[N];
        int16_t pidx[N];
        uint8_t nx[N];
        uint8_t seen[N];
        int16_t lock_child[max_locks];

        int zones[4][4];
        int nzones;
        int ncar;
        int room_cells;
        int layout;
        int open_lo, open_hi, dead_lo, dead_hi, straight, target;
        int entry, entry_dir, stairs, maxd;
        int doors, tunnels, regions;
        int cap_hits;
        int rolled[6];
        int placed[6];
        int lvl_max;
        unsigned nib;
        int nibleft;
    };

    DL_EWRAM gen_state g;

    [[noreturn]] void raise()
    {
        __builtin_longjmp(fail_jump, 1);
    }

    // ---- edge helpers (v2: open_edge, close_edge, kind_at, is_room, clean, nibble)
    void open_edge(int idx, int kind = K_OPEN)
    {
        g.lv->ed[idx] = uint8_t(kind);
        ++g.deg[edge_a(idx)];
        ++g.deg[edge_b(idx)];
    }

    void close_edge(int idx)
    {
        g.lv->ed[idx] = K_WALL;
        --g.deg[edge_a(idx)];
        --g.deg[edge_b(idx)];
    }

    int kat(int c, int d)
    {
        return kind_at(*g.lv, c, d);
    }

    bool is_room_id(int r)
    {
        return r >= 1 && r <= 15;
    }

    bool clean(int idx)
    {
        int k = g.lv->ed[idx] & 15;

        if(k != K_OPEN && k != K_DOOR && k != K_ARCH)
        {
            return false;
        }

        int a = edge_a(idx);
        int b = edge_b(idx);
        int ra = g.res[a];
        int rb = g.res[b];
        bool ia = is_room_id(ra);
        bool ib = is_room_id(rb);

        if(ia && ib && ra == rb)
        {
            return false;
        }

        int d = edge_dir(idx);
        int l1 = (d + 1) & 3;
        int l2 = (d + 3) & 3;

        if(! ia && (graph_kind[kat(a, l1)] || graph_kind[kat(a, l2)]))
        {
            return false;
        }

        if(! ib && (graph_kind[kat(b, l1)] || graph_kind[kat(b, l2)]))
        {
            return false;
        }

        return true;
    }

    unsigned nibble()
    {
        if(g.nibleft == 0)
        {
            g.nib = g.r.next16();
            g.nibleft = 4;
        }

        unsigned v = g.nib & 15;
        g.nib >>= 4;
        --g.nibleft;
        return v;
    }

    // ---- v2: place_room, set_zone, box_zone
    bool place_room(int x, int y, int w, int h)
    {
        level& lv = *g.lv;

        if(lv.nrooms >= 15)
        {
            return false;
        }

        for(int yy = y; yy < y + h; ++yy)
        {
            for(int xx = x; xx < x + w; ++xx)
            {
                int c = yy * W + xx;

                if(g.car[c] || ! g.allow[c])
                {
                    return false;
                }
            }
        }

        int rid = lv.nrooms + 1;
        lv.rooms[lv.nrooms++] = room{ uint8_t(x), uint8_t(y), uint8_t(w), uint8_t(h), SP_NONE };

        for(int yy = y; yy < y + h; ++yy)
        {
            for(int xx = x; xx < x + w; ++xx)
            {
                int c = yy * W + xx;
                g.res[c] = uint8_t(rid);
                g.car[c] = 1;
                ++g.ncar;

                if(xx < x + w - 1)
                {
                    open_edge(c);
                }

                if(yy < y + h - 1)
                {
                    open_edge(N + c);
                }
            }
        }

        g.room_cells += w * h;
        return true;
    }

    void set_zone(int x0, int y0, int w, int h)
    {
        for(int yy = y0; yy < y0 + h; ++yy)
        {
            for(int xx = x0; xx < x0 + w; ++xx)
            {
                g.allow[yy * W + xx] = 1;
            }
        }

        int* z = g.zones[g.nzones++];
        z[0] = x0;
        z[1] = y0;
        z[2] = w;
        z[3] = h;
    }

    void box_zone(int bx, int by, int bw, int bh, int wlo, int whi, int hlo, int hhi)
    {
        int w = wlo + g.r.rnd(whi - wlo + 1);
        w = bw < w ? bw : w;
        int h = hlo + g.r.rnd(hhi - hlo + 1);
        h = bh < h ? bh : h;
        int x0 = bx + g.r.rnd(bw - w + 1);
        int y0 = by + g.r.rnd(bh - h + 1);
        set_zone(x0, y0, w, h);
    }

    // v2: grow (DFS carve without stack: arrival direction per cell)
    void grow(int c0, int limit)
    {
        int c = c0;
        g.car[c] = 1;
        ++g.ncar;
        g.back[c] = 4;

        while(g.ncar < limit)
        {
            int m = 0;

            for(int d = 0; d < 4; ++d)
            {
                int nb = neighbour(c, d);

                if(nb >= 0 && ! g.car[nb] && g.allow[nb])
                {
                    m |= 1 << d;
                }
            }

            if(m)
            {
                int ad = (g.back[c] + 2) & 3;
                int d;

                if(g.back[c] != 4 && (m >> ad & 1) && g.r.rnd(100) < g.straight)
                {
                    d = ad;
                }
                else
                {
                    d = nth4(m, g.r.rnd(pop4(m)));
                }

                open_edge(edge_index(c, d));
                int nb = neighbour(c, d);
                g.car[nb] = 1;
                ++g.ncar;
                g.back[nb] = uint8_t((d + 2) & 3);
                c = nb;
            }
            else if(g.back[c] == 4)
            {
                break;
            }
            else
            {
                c = neighbour(c, g.back[c]);
            }
        }
    }

    // v2: layout_phase
    void layout_phase()
    {
        const band& bd = *g.bd;
        level& lv = *g.lv;
        memset(g.car, 0, N);
        memset(g.allow, 0, N);
        memset(g.back, 0, N);
        g.nzones = 0;
        g.ncar = 0;
        g.room_cells = 0;

        if(g.fb)
        {
            g.layout = L_MAZE;
        }
        else if(g.r.rnd(100) < bd.maze_pct)
        {
            g.layout = L_MAZE;
        }
        else
        {
            int bits[3];
            int nb = 0;

            for(int i = 1; i <= 3; ++i)
            {
                if(bd.layout_mask >> i & 1)
                {
                    bits[nb++] = i;
                }
            }

            g.layout = bits[g.r.rnd(nb)];
        }

        int lay = g.layout;
        int pct;

        if(lay == L_MAZE)
        {
            pct = g.fb ? 100 : maze_open;
            g.open_lo = g.fb ? 0 : maze_lo;
            g.open_hi = g.fb ? 100 : maze_hi;
            g.dead_lo = g.fb ? 0 : maze_dead_lo;
            g.dead_hi = g.fb ? N : maze_dead_hi;
            g.straight = g.fb ? 0 : maze_straight;
            set_zone(0, 0, W, H);
        }
        else
        {
            pct = bd.open_pct;
            g.open_lo = bd.open_lo;
            g.open_hi = bd.open_hi;
            g.dead_lo = bd.dead_lo;
            g.dead_hi = bd.dead_hi;
            g.straight = bd.straight;
        }

        g.target = g.fb ? N : pct * 11;

        if(lay != L_MAZE)
        {
            int nclu = lay == L_ROOMS ? 1 + g.r.rnd(bd.nclu_max) : 1;

            if(nclu == 1)
            {
                if(lay != L_ROOMS)
                {
                    box_zone(0, 0, W, H, W, W, H, H);
                }
                else
                {
                    box_zone(0, 0, W, H, 20, 30, 18, 28);
                }
            }
            else if(nclu == 2)
            {
                if(g.r.rnd(2))
                {
                    box_zone(0, 0, 16, H, 13, 16, 18, 28);
                    box_zone(16, 0, 16, H, 13, 16, 18, 28);
                }
                else
                {
                    box_zone(0, 0, W, 16, 18, 28, 13, 16);
                    box_zone(0, 16, W, 16, 18, 28, 13, 16);
                }
            }
            else
            {
                int skip = g.r.rnd(4);

                for(int qd = 0; qd < 4; ++qd)
                {
                    if(qd != skip)
                    {
                        box_zone((qd & 1) * 16, (qd >> 1) * 16, 16, 16, 12, 16, 12, 16);
                    }
                }
            }
        }

        if(lay == L_CROSS)
        {
            int hy = 3 + g.r.rnd(H - 6);
            int vx = 3 + g.r.rnd(W - 6);

            for(int x = 0; x < W; ++x)
            {
                int c = hy * W + x;
                g.res[c] = 0xFE;
                g.car[c] = 1;
                ++g.ncar;

                if(x < W - 1)
                {
                    open_edge(c);
                }
            }

            for(int y = 0; y < H; ++y)
            {
                int c = y * W + vx;

                if(! g.car[c])
                {
                    g.car[c] = 1;
                    ++g.ncar;
                }

                g.res[c] = 0xFE;
            }

            for(int y = 0; y < H - 1; ++y)
            {
                int c = y * W + vx;

                if(lv.ed[N + c] == K_WALL)
                {
                    open_edge(N + c);
                }
            }
        }

        if(lay == L_ARENA)
        {
            int side = 6 + g.r.rnd(5);
            int x = ((W - side) >> 1) + g.r.rnd(5) - 2;
            int y = ((H - side) >> 1) + g.r.rnd(5) - 2;
            uint8_t* saved = g.reg;     // scratch copy of allow
            memcpy(saved, g.allow, N);
            memset(g.allow, 1, N);
            place_room(x, y, side, side);
            memcpy(g.allow, saved, N);
        }

        if(lay != L_MAZE)
        {
            int room_target = (g.target * bd.room_pct * 41) >> 12;
            int made = lv.nrooms;

            for(int it = 0; it < 10 * bd.rooms_max; ++it)
            {
                if(made >= bd.rooms_max || (made >= bd.rooms_min && g.room_cells >= room_target))
                {
                    break;
                }

                int w, h;

                if(g.r.rnd(10) < 3)
                {
                    if(g.r.rnd(2))
                    {
                        w = 2;
                        h = 2;
                    }
                    else
                    {
                        w = 3;
                        h = 2;
                    }

                    if(g.r.rnd(2))
                    {
                        int t = w;
                        w = h;
                        h = t;
                    }
                }
                else
                {
                    w = bd.side_min + g.r.rnd(bd.side_max - bd.side_min + 1);
                    h = bd.side_min + g.r.rnd(bd.side_max - bd.side_min + 1);
                }

                int x = g.r.rnd(W - w + 1);
                int y = g.r.rnd(H - h + 1);

                if(place_room(x, y, w, h))
                {
                    ++made;
                }
            }
        }

        int nz = g.nzones;
        int budget = g.target - g.ncar;

        for(int zi = 0; zi < nz; ++zi)
        {
            int zx = g.zones[zi][0], zy = g.zones[zi][1], zw = g.zones[zi][2], zh = g.zones[zi][3];
            int limit = zi == nz - 1 ? g.target : g.ncar + floor_div(budget, nz);

            for(int t = 0; t < 12; ++t)
            {
                if(g.ncar >= limit)
                {
                    break;
                }

                int yy = zy + g.r.rnd(zh);
                int c0 = yy * W + zx + g.r.rnd(zw);

                if(! g.car[c0])
                {
                    grow(c0, limit);
                }
            }
        }
    }

    int find(int a)
    {
        while(g.uf[a] != a)
        {
            g.uf[a] = g.uf[g.uf[a]];
            a = g.uf[a];
        }

        return a;
    }

    // every stored edge in the v2 enumeration order: per cell, E then S
    template<typename F>
    void for_edges(F f)
    {
        for(int c = 0; c < N; ++c)
        {
            if((c & (W - 1)) < W - 1)
            {
                f(c);
            }

            if((c >> 5) < H - 1)
            {
                f(N + c);
            }
        }
    }

    int edge_at(int k)     // EDGES[k]
    {
        // per row y < H-1: 32 cells give E (x<31) and S: 63 edges; last row: 31 E edges
        int y = k / (2 * W - 1);
        int r = k - y * (2 * W - 1);

        if(y >= H - 1)
        {
            return (H - 1) * W + r;    // E edges of the last row
        }

        if(r == 2 * W - 2)
        {
            return N + y * W + W - 1;  // S edge of the last cell of the row
        }

        int x = r >> 1;
        return (r & 1) ? N + y * W + x : y * W + x;
    }

    constexpr int NE = (W - 1) * H + W * (H - 1);

    // v2: join_phase
    void join_phase()
    {
        level& lv = *g.lv;

        for(int i = 0; i < N; ++i)
        {
            g.uf[i] = int16_t(i);
        }

        for_edges([&](int idx) {
            if(lv.ed[idx] != K_WALL)
            {
                int ra = find(edge_a(idx));
                int rb = find(edge_b(idx));

                if(ra != rb)
                {
                    g.uf[ra] = int16_t(rb);
                }
            }
        });

        int n = 0;
        for_edges([&](int idx) {
            if(lv.ed[idx] == K_WALL && g.car[edge_a(idx)] && g.car[edge_b(idx)])
            {
                g.list[n++] = uint16_t(idx);
            }
        });

        for(int i = n - 1; i > 0; --i)
        {
            int j = g.r.rnd(i + 1);
            uint16_t t = g.list[i];
            g.list[i] = g.list[j];
            g.list[j] = t;
        }

        for(int k = 0; k < n; ++k)
        {
            int idx = g.list[k];
            int ra = find(edge_a(idx));
            int rb = find(edge_b(idx));

            if(ra != rb)
            {
                open_edge(idx);
                g.uf[ra] = int16_t(rb);
            }
        }

        g.tunnels = 0;
        int first = -1;

        for(int guard = 0; guard < 16; ++guard)
        {
            first = -1;

            for(int c = 0; c < N; ++c)
            {
                if(g.car[c])
                {
                    first = c;
                    break;
                }
            }

            int root = find(first);
            bool all = true;

            for(int c = 0; c < N; ++c)
            {
                if(g.car[c] && find(c) != root)
                {
                    all = false;
                    break;
                }
            }

            if(all)
            {
                break;
            }

            int nq = 0;

            for(int c = 0; c < N; ++c)
            {
                g.par[c] = -2;
            }

            for(int c = 0; c < N; ++c)
            {
                if(g.car[c] && find(c) == root)
                {
                    g.par[c] = -1;
                    g.q[nq++] = uint16_t(c);
                }
            }

            int i = 0;
            int hit = -1;

            while(i < nq && hit < 0)
            {
                int c = g.q[i++];

                for(int d = 0; d < 4; ++d)
                {
                    int nb = neighbour(c, d);

                    if(nb < 0 || g.par[nb] != -2)
                    {
                        continue;
                    }

                    g.par[nb] = int16_t(c);

                    if(g.car[nb])
                    {
                        hit = nb;
                        break;
                    }

                    g.q[nq++] = uint16_t(nb);
                }
            }

            if(hit < 0)
            {
                raise();
            }

            g.uf[find(hit)] = int16_t(root);
            int c = hit;

            while(g.par[c] >= 0)
            {
                int p = g.par[c];
                int d = 0;

                while(neighbour(p, d) != c)
                {
                    ++d;
                }

                open_edge(edge_index(p, d));

                if(! g.car[c])
                {
                    g.car[c] = 1;
                    ++g.ncar;
                    g.uf[c] = int16_t(root);
                }

                c = p;
            }

            ++g.tunnels;
        }

        int root = find(first);

        for(int c = 0; c < N; ++c)
        {
            if(g.car[c] && find(c) != root)
            {
                raise();
            }
        }
    }

    // v2: cull_phase
    void cull_phase()
    {
        if(g.fb)
        {
            return;
        }

        level& lv = *g.lv;
        int dead = 0;

        for(int c = 0; c < N; ++c)
        {
            dead += g.deg[c] == 1;
        }

        int tgt = (g.dead_lo + g.dead_hi) >> 1;
        int join_pct = g.bd->join_pct;

        for(int pass = 0; pass < 2; ++pass)
        {
            if(dead <= tgt)
            {
                break;
            }

            int off = g.r.rnd(N);

            for(int k = 0; k < N; ++k)
            {
                if(dead <= tgt)
                {
                    break;
                }

                int c = (off + k) & (N - 1);

                if(g.deg[c] != 1 || g.res[c] != 0)
                {
                    continue;
                }

                int cand[4];
                int nc = 0;

                for(int d = 0; d < 4; ++d)
                {
                    int nb = neighbour(c, d);

                    if(nb >= 0 && g.car[nb] && lv.ed[edge_index(c, d)] == K_WALL)
                    {
                        cand[nc++] = d;
                    }
                }

                if(nc && g.r.rnd(100) < join_pct)
                {
                    int d = cand[g.r.rnd(nc)];

                    if(g.deg[neighbour(c, d)] == 1)
                    {
                        --dead;
                    }

                    open_edge(edge_index(c, d));
                    --dead;
                    continue;
                }

                int cur = c;

                while(true)
                {
                    int d = 0;

                    for(; d < 4; ++d)
                    {
                        int ei = edge_index(cur, d);

                        if(ei >= 0 && lv.ed[ei] != K_WALL)
                        {
                            break;
                        }
                    }

                    if(d == 4)
                    {
                        raise();    // unreachable: deg == 1 guarantees an open side
                    }

                    int nb = neighbour(cur, d);
                    close_edge(edge_index(cur, d));
                    g.car[cur] = 0;
                    --g.ncar;
                    --dead;
                    cur = nb;

                    if(g.deg[cur] == 1 && g.res[cur] == 0)
                    {
                        ++dead;
                    }
                    else
                    {
                        if(g.deg[cur] == 1)
                        {
                            ++dead;
                        }

                        break;
                    }
                }
            }
        }
    }

    void collect_cells()
    {
        g.ncells = 0;

        for(int c = 0; c < N; ++c)
        {
            if(g.car[c])
            {
                g.cells[g.ncells++] = uint16_t(c);
            }
        }
    }

    bool add_raw(const entity& e)
    {
        level& lv = *g.lv;

        if(lv.nents >= max_ents)
        {
            raise();
        }

        lv.ents[lv.nents++] = e;
        return true;
    }

    // v2: entry_phase
    void entry_phase()
    {
        int n = 0;

        for(int c = 0; c < N; ++c)
        {
            if(g.deg[c] == 1)
            {
                g.list[n++] = uint16_t(c);
            }
        }

        int c;

        if(n)
        {
            c = g.list[g.r.rnd(n)];
        }
        else
        {
            collect_cells();
            c = g.cells[g.r.rnd(g.ncar)];
        }

        g.entry = c;
        int d0 = 0;

        for(int d = 0; d < 4; ++d)
        {
            if(graph_kind[kat(c, d)])
            {
                d0 = d;
                break;
            }
        }

        g.entry_dir = d0;
        g.lv->cl[c] |= C_ENTRY | C_ENT;
        g.occ[c] = 1;
        add_raw(entity{ E_ENTRY, uint8_t(c % W), uint8_t(c / W), uint8_t(d0), 0, 0 });
    }

    // v2: door_phase
    void door_phase()
    {
        level& lv = *g.lv;
        int door_pct = g.fb ? 0 : g.bd->door_pct;

        for(int rid = 1; rid <= lv.nrooms; ++rid)
        {
            if(! (door_pct && g.r.rnd(100) < door_pct))
            {
                continue;
            }

            const room& rm = lv.rooms[rid - 1];
            int n = 0;

            for(int yy = rm.y; yy < rm.y + rm.h; ++yy)
            {
                for(int xx = rm.x; xx < rm.x + rm.w; ++xx)
                {
                    int c = yy * W + xx;

                    for(int d = 0; d < 4; ++d)
                    {
                        int nb = neighbour(c, d);

                        if(nb >= 0 && g.res[nb] != rid && nb != g.entry && c != g.entry)
                        {
                            int i = edge_index(c, d);

                            if((lv.ed[i] & 15) == K_OPEN && clean(i))
                            {
                                g.list[n++] = uint16_t(i);
                            }
                        }
                    }
                }
            }

            if(n)
            {
                int i = g.list[g.r.rnd(n)];
                lv.ed[i] = K_DOOR;
                ++g.doors;
            }
        }

        for_edges([&](int idx) {
            if(lv.ed[idx] == K_OPEN)
            {
                int ra = g.res[edge_a(idx)];
                int rb = g.res[edge_b(idx)];

                if(ra != rb && (is_room_id(ra) || is_room_id(rb)))
                {
                    lv.ed[idx] = K_ARCH;
                }
            }
        });

        int maxd = door_pct >> 2;
        int got = 0;

        for(int it = 0; it < 2 * door_pct; ++it)
        {
            if(got >= maxd)
            {
                break;
            }

            int idx = edge_at(g.r.rnd(NE));

            if((lv.ed[idx] & 15) == K_OPEN && clean(idx) && edge_a(idx) != g.entry && edge_b(idx) != g.entry)
            {
                lv.ed[idx] = K_DOOR;
                ++got;
                ++g.doors;
            }
        }
    }

    // v2: bfs (GRAPH edges); fills g.dist and g.q / g.nq
    void bfs(int start)
    {
        level& lv = *g.lv;

        for(int c = 0; c < N; ++c)
        {
            g.dist[c] = 0xFFFF;
        }

        g.dist[start] = 0;
        g.q[0] = uint16_t(start);
        g.nq = 1;
        int i = 0;

        while(i < g.nq)
        {
            int c = g.q[i++];
            int dc = g.dist[c] + 1;

            for(int d = 0; d < 4; ++d)
            {
                int ei = edge_index(c, d);

                if(ei >= 0 && graph_kind[lv.ed[ei] & 15])
                {
                    int nb = neighbour(c, d);

                    if(g.dist[nb] == 0xFFFF)
                    {
                        g.dist[nb] = uint16_t(dc);
                        g.q[g.nq++] = uint16_t(nb);
                    }
                }
            }
        }
    }

    // v2: stairs_phase
    void stairs_phase()
    {
        level& lv = *g.lv;
        bfs(g.entry);

        if(g.nq != g.ncar)
        {
            raise();
        }

        g.maxd = g.dist[g.q[g.nq - 1]];
        int smin = g.fb ? 24 : g.bd->stairs_min;
        int n = 0;
        int mx = -1;

        for(int c = 0; c < N; ++c)
        {
            if(g.deg[c] == 1 && c != g.entry && g.dist[c] >= smin)
            {
                g.list[n++] = uint16_t(c);
                mx = g.dist[c] > mx ? g.dist[c] : mx;
            }
        }

        int s;

        if(n)
        {
            int nt = 0;

            for(int k = 0; k < n; ++k)
            {
                if(g.dist[g.list[k]] * 4 >= mx * 3)
                {
                    g.list2[nt++] = g.list[k];
                }
            }

            s = g.list2[g.r.rnd(nt)];
        }
        else
        {
            s = g.q[g.nq - 1];
        }

        if(g.dist[s] < smin || s == g.entry)
        {
            raise();
        }

        g.stairs = s;
        lv.cl[s] |= C_STAIRS | C_ENT;
        g.occ[s] = 1;
        int sd = -1;
        int op = -1;

        for(int d = 0; d < 4; ++d)
        {
            if(graph_kind[kat(s, d)])
            {
                op = d;
                break;
            }
        }

        int order[5];
        int no = 0;

        if(op >= 0)
        {
            order[no++] = (op + 2) & 3;
        }

        for(int d = 0; d < 4; ++d)
        {
            order[no++] = d;
        }

        for(int k = 0; k < no; ++k)
        {
            int d = order[k];
            int i = edge_index(s, d);

            if(i >= 0 && (lv.ed[i] & 15) == K_WALL)
            {
                lv.ed[i] = K_STAIRS;
                sd = d;
                break;
            }
        }

        add_raw(entity{ E_STAIRS, uint8_t(s % W), uint8_t(s / W), uint8_t(sd >= 0 ? sd : 0), 0, 0 });

        // regions (stat only)
        memset(g.reg, 0, N);
        int nreg = 0;

        for(int c0 = 0; c0 < N; ++c0)
        {
            if(g.reg[c0] || ! g.car[c0])
            {
                continue;
            }

            ++nreg;
            g.reg[c0] = uint8_t(nreg < 255 ? nreg : 255);
            int sp = 0;
            g.q[sp++] = uint16_t(c0);

            while(sp)
            {
                int c = g.q[--sp];

                for(int d = 0; d < 4; ++d)
                {
                    int ei = edge_index(c, d);

                    if(ei >= 0 && ((lv.ed[ei] & 15) == K_OPEN || (lv.ed[ei] & 15) == K_ARCH))
                    {
                        int nb = neighbour(c, d);

                        if(! g.reg[nb])
                        {
                            g.reg[nb] = g.reg[c0];
                            g.q[sp++] = uint16_t(nb);
                        }
                    }
                }
            }
        }

        g.regions = nreg < 255 ? nreg : 255;
    }

    // flood from the entry through FLOOD kinds -> g.seen, g.q / g.nq
    void flood_entry()
    {
        level& lv = *g.lv;
        memset(g.seen, 0, N);
        g.seen[g.entry] = 1;
        g.q[0] = uint16_t(g.entry);
        g.nq = 1;
        int i = 0;

        while(i < g.nq)
        {
            int c = g.q[i++];

            for(int d = 0; d < 4; ++d)
            {
                int ei = edge_index(c, d);

                if(ei >= 0 && flood_kind[lv.ed[ei] & 15])
                {
                    int nb = neighbour(c, d);

                    if(! g.seen[nb])
                    {
                        g.seen[nb] = 1;
                        g.q[g.nq++] = uint16_t(nb);
                    }
                }
            }
        }
    }

    bool in_list(const uint16_t* l, int n, int v, int* pos)
    {
        for(int k = 0; k < n; ++k)
        {
            if(l[k] == v)
            {
                *pos = k;
                return true;
            }
        }

        return false;
    }

    void erase_at(uint16_t* l, int& n, int k)
    {
        for(int i = k; i < n - 1; ++i)
        {
            l[i] = l[i + 1];
        }

        --n;
    }

    // v2: lock_phase
    void lock_phase()
    {
        level& lv = *g.lv;
        memset(g.lvl, 0xFF, N);
        g.lvl_max = 0;
        int nwant = (! g.fb && g.level_index >= g.tb->lock_free) ? g.bd->n_locks : 0;
        int entry = g.entry;
        int stairs = g.stairs;
        lv.nlocks = 0;

        if(nwant)
        {
            memset(g.disc, 0, sizeof(g.disc));
            memset(g.low, 0, sizeof(g.low));
            memset(g.sz, 0, sizeof(g.sz));
            memset(g.nx, 0, N);

            for(int c = 0; c < N; ++c)
            {
                g.par[c] = -1;
                g.pidx[c] = -1;
            }

            int timer = 1;
            g.disc[entry] = g.low[entry] = int16_t(timer);
            g.sz[entry] = 1;
            int sp = 0;
            g.q[sp++] = uint16_t(entry);
            int nc = 0;     // cands in g.list

            while(sp)
            {
                int c = g.q[sp - 1];
                int d = g.nx[c];

                if(d < 4)
                {
                    g.nx[c] = uint8_t(d + 1);
                    int ei = edge_index(c, d);

                    if(ei < 0 || ! graph_kind[lv.ed[ei] & 15])
                    {
                        continue;
                    }

                    int nb = neighbour(c, d);

                    if(g.disc[nb] == 0)
                    {
                        g.par[nb] = int16_t(c);
                        g.pidx[nb] = int16_t(ei);
                        ++timer;
                        g.disc[nb] = g.low[nb] = int16_t(timer);
                        g.sz[nb] = 1;
                        g.q[sp++] = uint16_t(nb);
                    }
                    else if(nb != g.par[c] && g.disc[nb] < g.low[c])
                    {
                        g.low[c] = g.disc[nb];
                    }
                }
                else
                {
                    --sp;
                    int p = g.par[c];

                    if(p >= 0)
                    {
                        g.sz[p] = int16_t(g.sz[p] + g.sz[c]);

                        if(g.low[c] < g.low[p])
                        {
                            g.low[p] = g.low[c];
                        }

                        if(g.low[c] > g.disc[p] && g.sz[c] >= lock_far_min && g.ncar - g.sz[c] >= lock_near_min &&
                                p != entry && c != stairs && p != stairs && clean(g.pidx[c]))
                        {
                            g.list[nc++] = uint16_t(c);
                        }
                    }
                }
            }

            int nm = 0;     // main in g.list2

            for(int k = 0; k < nc; ++k)
            {
                int c = g.list[k];

                if(g.disc[c] <= g.disc[stairs] && g.disc[stairs] < g.disc[c] + g.sz[c])
                {
                    g.list2[nm++] = uint16_t(c);
                }
            }

            int kinds_n = g.bd->key_kinds;

            for(int i = 0; i < nwant; ++i)
            {
                for(int tr = 0; tr < 8; ++tr)
                {
                    if(! nc)
                    {
                        break;
                    }

                    bool use_main = i == 0 && nm && g.r.rnd(100) < g.bd->main_pct;
                    const uint16_t* pool = use_main ? g.list2 : g.list;
                    int pn = use_main ? nm : nc;
                    int j = g.r.rnd(pn);
                    int c = pool[j];
                    int pos;

                    if(in_list(g.list, nc, c, &pos))
                    {
                        erase_at(g.list, nc, pos);
                    }

                    if(in_list(g.list2, nm, c, &pos))
                    {
                        erase_at(g.list2, nm, pos);
                    }

                    int r0 = g.r.rnd(kinds_n);
                    int kind = -1;

                    for(int t = 0; t < kinds_n; ++t)
                    {
                        int k = (r0 + t) % kinds_n;
                        bool ok = true;

                        for(int li = 0; li < lv.nlocks; ++li)
                        {
                            if(lv.locks[li].kind == k)
                            {
                                int c2 = g.lock_child[li];
                                bool nested = (g.disc[c] <= g.disc[c2] && g.disc[c2] < g.disc[c] + g.sz[c]) ||
                                        (g.disc[c2] <= g.disc[c] && g.disc[c] < g.disc[c2] + g.sz[c2]);

                                if(! nested)
                                {
                                    ok = false;
                                    break;
                                }
                            }
                        }

                        if(ok)
                        {
                            kind = k;
                            break;
                        }
                    }

                    if(kind < 0)
                    {
                        continue;
                    }

                    g.lock_child[lv.nlocks] = int16_t(c);
                    lv.locks[lv.nlocks++] = lock{ g.pidx[c], uint8_t(kind) };
                    break;
                }
            }
        }

        int nl = lv.nlocks;
        bool opened[max_locks] = {};

        for(int i = 0; i < nl; ++i)
        {
            lv.ed[lv.locks[i].edge] = uint8_t(K_LOCKED | (lv.locks[i].kind << 4));
        }

        flood_entry();

        for(int k = 0; k < g.nq; ++k)
        {
            g.lvl[g.q[k]] = 0;
        }

        for(int step = 1; step <= nl; ++step)
        {
            int frontier[max_locks];
            int nf = 0;

            for(int i = 0; i < nl; ++i)
            {
                int e = lv.locks[i].edge;

                if(! opened[i] && (g.seen[edge_a(e)] || g.seen[edge_b(e)]))
                {
                    frontier[nf++] = i;
                }
            }

            if(! nf)
            {
                raise();
            }

            int li = frontier[g.r.rnd(nf)];
            int nfree = 0;
            int npref = 0;

            for(int k = 0; k < g.nq; ++k)
            {
                int c = g.q[k];

                if(! g.occ[c])
                {
                    g.list[nfree++] = uint16_t(c);

                    if(g.deg[c] == 1 || is_room_id(g.res[c]))
                    {
                        g.list2[npref++] = uint16_t(c);
                    }
                }
            }

            bool use_pref = npref && g.r.rnd(100) < 70;
            const uint16_t* pool = use_pref ? g.list2 : g.list;
            int pn = use_pref ? npref : nfree;

            if(! pn)
            {
                raise();
            }

            int kc = pool[g.r.rnd(pn)];
            int kind = lv.locks[li].kind;
            g.occ[kc] = 1;
            lv.cl[kc] |= C_ENT;
            add_raw(entity{ E_KEY, uint8_t(kc % W), uint8_t(kc / W), 0, uint8_t(kind), uint8_t(li) });
            opened[li] = true;
            lv.ed[lv.locks[li].edge] = K_OPEN;
            flood_entry();

            for(int k = 0; k < g.nq; ++k)
            {
                if(g.lvl[g.q[k]] == 0xFF)
                {
                    g.lvl[g.q[k]] = uint8_t(step);
                }
            }

            g.lvl_max = step;
        }

        for(int i = 0; i < nl; ++i)
        {
            lv.ed[lv.locks[i].edge] = uint8_t(K_LOCKED | (lv.locks[i].kind << 4));
        }
    }

    // v2: roll, add_ent
    int roll(const count& cnt)
    {
        if(cnt.pct && g.r.rnd(100) < cnt.pct)
        {
            int t = cnt.bonus;

            for(int i = 0; i < cnt.dice_n; ++i)
            {
                t += g.r.rnd(cnt.dice_sides) + 1;
            }

            return t;
        }

        return 0;
    }

    bool add_ent(const entity& e, int c, int cat)
    {
        level& lv = *g.lv;

        if(lv.nents >= g.tb->max_ent)
        {
            ++g.cap_hits;
            return false;
        }

        add_raw(e);
        g.occ[c] = 1;
        lv.cl[c] |= C_ENT;

        if(cat >= 0)
        {
            ++g.placed[cat];
        }

        return true;
    }

    entity ent_at(int type, int c, int a, int b, int cc)
    {
        return entity{ uint8_t(type), uint8_t(c % W), uint8_t(c / W), uint8_t(a), uint8_t(b), uint8_t(cc) };
    }

    int count_max(const count& c)
    {
        return c.pct ? c.dice_n * c.dice_sides + c.bonus : 0;
    }

    int cell_in_room(const room& r)
    {
        int yy = r.y + g.r.rnd(r.h);
        return yy * W + r.x + g.r.rnd(r.w);
    }

    // v2: entity_phase
    void entity_phase()
    {
        const band& bd = *g.bd;
        level& lv = *g.lv;
        int nroom = lv.nrooms;
        int ncar = g.ncar;
        int lair_room = 0, shop_room = 0, treasure_room = 0;

        if(! g.fb && nroom)
        {
            const int sps[3] = { SP_TREASURE, SP_LAIR, SP_SHOP };
            const int pcts[3] = { bd.treasure_pct, bd.lair_pct, bd.shop_pct };

            for(int k = 0; k < 3; ++k)
            {
                int sp = sps[k];
                int pct = pcts[k];

                if(pct && g.r.rnd(100) < pct)
                {
                    int fr[max_rooms];
                    int nf = 0;

                    for(int i = 0; i < nroom; ++i)
                    {
                        if(lv.rooms[i].special == SP_NONE)
                        {
                            fr[nf++] = i;
                        }
                    }

                    if(nf)
                    {
                        int i = fr[g.r.rnd(nf)];
                        room& r = lv.rooms[i];
                        r.special = uint8_t(sp);
                        add_raw(entity{ E_ROOM, r.x, r.y, r.w, r.h, uint8_t((sp << 4) | (i + 1)) });

                        if(sp == SP_TREASURE)
                        {
                            treasure_room = i + 1;
                        }
                        else if(sp == SP_LAIR)
                        {
                            lair_room = i + 1;
                        }
                        else
                        {
                            shop_room = i + 1;
                        }
                    }
                }
            }
        }

        for(int i = 0; i < nroom; ++i)
        {
            const room& r = lv.rooms[i];

            for(int yy = r.y; yy < r.y + r.h; ++yy)
            {
                for(int xx = r.x; xx < r.x + r.w; ++xx)
                {
                    lv.th[yy * W + xx] = uint8_t((r.special << 4) | (i + 1));
                }
            }
        }

        if(g.fb)
        {
            return;
        }

        int scale = 15360 / (g.maxd ? g.maxd : 1);
        const int spawn_clear = g.tb->spawn_clear;

        // monsters
        int cnt = roll(bd.monsters);
        g.rolled[0] = cnt;
        int left = cnt;

        for(int it = 0; it < 6 * cnt + 16; ++it)
        {
            if(left <= 0)
            {
                break;
            }

            int c = g.cells[g.r.rnd(ncar)];

            if(g.occ[c] || g.dist[c] < spawn_clear || (shop_room && g.res[c] == shop_room))
            {
                continue;
            }

            if(! is_room_id(g.res[c]) && g.r.rnd(100) >= 45)
            {
                continue;
            }

            if(g.r.rnd(100) >= 40 + ((g.dist[c] * scale) >> 8))
            {
                continue;
            }

            int tier = bd.tier_min + g.r.rnd(bd.tier_max - bd.tier_min + 1);
            int dir = g.r.rnd(4);

            if(add_ent(ent_at(E_MONSTER, c, dir, tier, 0), c, 0))
            {
                --left;
            }
        }

        g.cap_hits += left > 0 ? left : 0;

        if(lair_room)
        {
            const room& r = lv.rooms[lair_room - 1];
            int extra_mon = 2 + g.r.rnd(3);
            int got = 0;

            for(int it = 0; it < 16; ++it)
            {
                if(got >= extra_mon)
                {
                    break;
                }

                int c = cell_in_room(r);

                if(g.occ[c] || g.dist[c] < spawn_clear)
                {
                    continue;
                }

                int dir = g.r.rnd(4);

                if(add_ent(ent_at(E_MONSTER, c, dir, bd.tier_max, 1), c, 0))
                {
                    ++got;
                }
            }

            g.rolled[0] += extra_mon;
        }

        // items
        cnt = roll(bd.items);
        int extra_item = 0;

        if(treasure_room)
        {
            extra_item = 3 + g.r.rnd(2);
            cnt += extra_item;
        }

        g.rolled[1] = cnt;
        left = cnt;

        if(treasure_room)
        {
            const room& r = lv.rooms[treasure_room - 1];
            int got = 0;

            for(int it = 0; it < 16; ++it)
            {
                if(got >= extra_item)
                {
                    break;
                }

                int c = cell_in_room(r);

                if(g.occ[c])
                {
                    continue;
                }

                int t = g.r.rnd(8);

                if(add_ent(ent_at(E_ITEM, c, 0, t, 1), c, 1))
                {
                    ++got;
                }
            }

            left -= got;
        }

        for(int it = 0, n = 6 * left + 16; it < n; ++it)
        {
            if(left <= 0)
            {
                break;
            }

            int c = g.cells[g.r.rnd(ncar)];

            if(g.occ[c])
            {
                continue;
            }

            if(! (g.deg[c] == 1 || is_room_id(g.res[c])) && g.r.rnd(100) >= 40)
            {
                continue;
            }

            int t = g.r.rnd(8);

            if(add_ent(ent_at(E_ITEM, c, 0, t, 0), c, 1))
            {
                --left;
            }
        }

        g.cap_hits += left > 0 ? left : 0;

        // traps
        cnt = roll(bd.traps);
        g.rolled[2] = cnt;
        left = cnt;

        for(int it = 0; it < 6 * cnt + 16; ++it)
        {
            if(left <= 0)
            {
                break;
            }

            int c = g.cells[g.r.rnd(ncar)];

            if(g.occ[c])
            {
                continue;
            }

            if(g.deg[c] != 2 || is_room_id(g.res[c]) || g.dist[c] < 4)
            {
                continue;
            }

            int t = g.r.rnd(3);

            if(add_ent(ent_at(E_TRAP, c, 0, t, 0), c, 2))
            {
                lv.cl[c] = uint8_t((lv.cl[c] & ~7) | (1 + t));
                --left;
            }
        }

        g.cap_hits += left > 0 ? left : 0;

        // teleport pairs
        cnt = roll(bd.teleports);
        g.rolled[3] = cnt;
        left = cnt;

        for(int it = 0; it < 6 * cnt + 16; ++it)
        {
            if(left <= 0)
            {
                break;
            }

            int c1 = g.cells[g.r.rnd(ncar)];
            int c2 = g.cells[g.r.rnd(ncar)];

            if(c1 == c2 || g.occ[c1] || g.occ[c2] || g.lvl[c1] != g.lvl[c2])
            {
                continue;
            }

            int mdx = c1 % W - c2 % W;
            int mdy = c1 / W - c2 / W;

            if((mdx < 0 ? -mdx : mdx) + (mdy < 0 ? -mdy : mdy) < 8)
            {
                continue;
            }

            if(lv.nents + 2 > g.tb->max_ent)
            {
                ++g.cap_hits;
                break;
            }

            add_ent(entity{ E_TELE, uint8_t(c1 % W), uint8_t(c1 / W), 0, uint8_t(c2 % W), uint8_t(c2 / W) }, c1, 3);
            add_ent(entity{ E_TELE, uint8_t(c2 % W), uint8_t(c2 / W), 0, uint8_t(c1 % W), uint8_t(c1 / W) }, c2, -1);
            lv.cl[c1] = uint8_t((lv.cl[c1] & ~7) | 4);
            lv.cl[c2] = uint8_t((lv.cl[c2] & ~7) | 4);
            --left;
        }

        g.cap_hits += left > 0 ? left : 0;

        // illusory walls
        cnt = roll(bd.secrets);
        g.rolled[4] = cnt;
        left = cnt;

        for(int it = 0; it < 12 * cnt + 32; ++it)
        {
            if(left <= 0)
            {
                break;
            }

            int cc = g.cells[g.r.rnd(ncar)];
            int idx = edge_index(cc, g.r.rnd(4));

            if(idx < 0 || lv.ed[idx] != K_WALL)
            {
                continue;
            }

            int da = g.dist[edge_a(idx)];
            int db = g.dist[edge_b(idx)];

            if(da == 0xFFFF || db == 0xFFFF)
            {
                continue;
            }

            if((da > db ? da - db : db - da) < 8)
            {
                continue;
            }

            lv.ed[idx] = uint8_t(K_SECRET | (g.r.rnd(16) << 4));
            ++g.placed[4];
            --left;
        }

        g.cap_hits += left > 0 ? left : 0;

        // furniture
        cnt = roll(bd.furniture);
        g.rolled[5] = cnt;
        left = cnt;

        for(int it = 0; it < 6 * cnt + 16; ++it)
        {
            if(left <= 0)
            {
                break;
            }

            int c = g.cells[g.r.rnd(ncar)];

            if(g.occ[c])
            {
                continue;
            }

            if(! is_room_id(g.res[c]) && g.r.rnd(100) >= (g.deg[c] == 1 ? 40 : 8))
            {
                continue;
            }

            int a = g.r.rnd(4);
            int b = g.r.rnd(8);

            if(add_ent(ent_at(E_FURN, c, a, b, 0), c, 5))
            {
                --left;
            }
        }

        g.cap_hits += left > 0 ? left : 0;
    }

    // v2: decor_phase
    void decor_phase()
    {
        level& lv = *g.lv;

        for_edges([&](int idx) {
            int k = lv.ed[idx];

            if(k == K_WALL && ! g.car[edge_a(idx)] && ! g.car[edge_b(idx)])
            {
                return;
            }

            if(k == K_WALL || k == K_DOOR || k == K_ARCH)
            {
                lv.ed[idx] = uint8_t(k | (nibble() << 4));
            }
        });

        for(int k = 0; k < g.ncells; ++k)
        {
            lv.cl[g.cells[k]] |= C_CARVED;
        }
    }

    // v2: validate
    bool validate()
    {
        const band& bd = *g.bd;
        level& lv = *g.lv;

        if(g.entry == g.stairs)
        {
            return false;
        }

        if(g.dist[g.stairs] < (g.fb ? 24 : bd.stairs_min))
        {
            return false;
        }

        if(g.regions < 1 || lv.nents > g.tb->max_ent)
        {
            return false;
        }

        int pct = (g.ncar * 25) >> 8;

        if(pct < g.open_lo || pct > g.open_hi)
        {
            return false;
        }

        int dead = 0;

        for(int c = 0; c < N; ++c)
        {
            dead += g.deg[c] == 1;
        }

        if(dead < g.dead_lo || dead > g.dead_hi)
        {
            return false;
        }

        if(! g.fb)
        {
            int nl_want = g.level_index >= g.tb->lock_free ? bd.n_locks : 0;

            if(nl_want && ! lv.nlocks)
            {
                return false;
            }

            if(lv.nlocks)
            {
                int nl = lv.nlocks;
                bool opened[max_locks] = {};
                bool progress = true;

                while(progress)
                {
                    progress = false;
                    memset(g.seen, 0, N);
                    g.seen[g.entry] = 1;
                    g.q[0] = uint16_t(g.entry);
                    int nq = 1;
                    int i = 0;

                    while(i < nq)
                    {
                        int c = g.q[i++];

                        for(int d = 0; d < 4; ++d)
                        {
                            int ei = edge_index(c, d);

                            if(ei < 0)
                            {
                                continue;
                            }

                            int kd = lv.ed[ei] & 15;
                            int nb = neighbour(c, d);

                            if(g.seen[nb])
                            {
                                continue;
                            }

                            bool pass = flood_kind[kd];

                            if(! pass && kd == K_LOCKED)
                            {
                                for(int j = 0; j < nl; ++j)
                                {
                                    if(opened[j] && lv.locks[j].edge == ei)
                                    {
                                        pass = true;
                                        break;
                                    }
                                }
                            }

                            if(pass)
                            {
                                g.seen[nb] = 1;
                                g.q[nq++] = uint16_t(nb);
                            }
                        }
                    }

                    int have[16] = {};

                    for(int k = 0; k < lv.nents; ++k)
                    {
                        const entity& e = lv.ents[k];

                        if(e.type == E_KEY && g.seen[e.y * W + e.x])
                        {
                            ++have[e.b & 15];
                        }
                    }

                    for(int j = 0; j < nl; ++j)
                    {
                        if(opened[j])
                        {
                            --have[lv.locks[j].kind];
                        }
                    }

                    for(int j = 0; j < nl; ++j)
                    {
                        int e = lv.locks[j].edge;

                        if(! opened[j] && have[lv.locks[j].kind] > 0 && (g.seen[edge_a(e)] || g.seen[edge_b(e)]))
                        {
                            opened[j] = true;
                            --have[lv.locks[j].kind];
                            progress = true;
                            break;
                        }
                    }
                }

                for(int j = 0; j < nl; ++j)
                {
                    if(! opened[j])
                    {
                        return false;
                    }
                }
            }

            const count* cs[6] = { &bd.monsters, &bd.items, &bd.traps, &bd.teleports, &bd.secrets, &bd.furniture };

            for(int cat = 0; cat < 6; ++cat)
            {
                if(g.rolled[cat] > count_max(*cs[cat]) + 4)
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool run_attempt(unsigned seed32, int level_index, int attempt, const tables& t, level& out)
    {
        memset(&out, 0, sizeof(out));
        memset(g.res, 0, N);
        memset(g.deg, 0, N);
        memset(g.occ, 0, N);
        memset(g.lvl, 0xFF, N);
        g.lv = &out;
        g.tb = &t;
        int b = level_index / t.band_levels;
        g.bd = &t.bands[b < t.band_count ? b : t.band_count - 1];
        g.level_index = level_index;
        g.fb = attempt == max_attempts;
        g.r.seed(seed32, level_index, attempt);
        g.cap_hits = 0;
        g.doors = 0;
        g.tunnels = 0;
        g.nib = 0;
        g.nibleft = 0;
        g.ncells = 0;

        for(int k = 0; k < 6; ++k)
        {
            g.rolled[k] = 0;
            g.placed[k] = 0;
        }

        volatile bool ok = false;

        if(__builtin_setjmp(fail_jump) == 0)
        {
            layout_phase();
            join_phase();
            cull_phase();
            collect_cells();
            entry_phase();
            door_phase();
            stairs_phase();
            lock_phase();
            entity_phase();
            decor_phase();
            ok = validate();
        }

        out.layout = g.layout;
        out.entry = g.entry;
        out.entry_dir = g.entry_dir;
        out.stairs = g.stairs;
        out.attempts = attempt + 1;
        out.fallback = g.fb;
        out.tunnels = g.tunnels;
        out.ncar = g.ncar;
        out.regions = g.regions;
        int dead = 0;

        for(int c = 0; c < N; ++c)
        {
            dead += g.deg[c] == 1;
        }

        out.dead_ends = dead;
        return ok;
    }
}

}


namespace dl::gen
{
    bool generate_v2(unsigned seed32, int level_index, const tables& t, level& out)
    {
        for(int attempt = 0; attempt <= max_attempts; ++attempt)
        {
            if(run_attempt(seed32, level_index, attempt, t, out))
            {
                return true;
            }
        }

        return false;
    }

    int level_bytes(const level& lv, uint8_t* out, int capacity)
    {
        int n = 2 * N + 2 * N + 1 + 6 * lv.nents;

        if(n > capacity)
        {
            return -1;
        }

        memcpy(out, lv.ed, 2 * N);
        memcpy(out + 2 * N, lv.cl, N);
        memcpy(out + 3 * N, lv.th, N);
        out[4 * N] = uint8_t(lv.nents);
        memcpy(out + 4 * N + 1, lv.ents, size_t(6 * lv.nents));
        return n;
    }

    unsigned crc16(const uint8_t* data, int size)
    {
        unsigned crc = 0xFFFF;

        for(int i = 0; i < size; ++i)
        {
            crc ^= unsigned(data[i]) << 8;

            for(int k = 0; k < 8; ++k)
            {
                crc = (crc & 0x8000) ? ((crc << 1) ^ 0x1021) & 0xFFFF : (crc << 1) & 0xFFFF;
            }
        }

        return crc;
    }
}
