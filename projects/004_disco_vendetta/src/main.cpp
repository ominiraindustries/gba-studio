// DISCO VENDETTA: top-down one-hit-kill action in a 70s mafia setting. Block 1 greybox (DISENO.md section 7).
// Floor 1 "LOBBY" on a big BG map, doors you kick open, windows and bars, knockouts and finishers, aim assist,
// camera lead, noise, gunmen that warn before every shot, exit, hints, score and combo, pause, death and retry,
// blood particles and floor splats on a second big BG.

#include "bn_bg_palettes.h"
#include "bn_bg_palette_ptr.h"
#include "bn_regular_bg_tiles_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_color.h"
#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_keypad.h"
#include "bn_math.h"
#include "bn_memory.h"
#include "bn_optional.h"
#include "bn_random.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sound_items.h"
#include "bn_sprite_palettes.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_vector.h"

#include "bn_bg_palette_items_gb_tiles_palette.h"
#include "bn_regular_bg_tiles_items_gb_tiles.h"
#include "bn_sprite_items_gb_arrow.h"
#include "bn_sprite_items_gb_bar.h"
#include "bn_sprite_items_gb_blood.h"
#include "bn_sprite_items_gb_bullet.h"
#include "bn_sprite_items_gb_dot.h"
#include "bn_sprite_items_gb_gunman.h"
#include "bn_sprite_items_gb_pickups.h"
#include "bn_sprite_items_gb_player.h"
#include "bn_sprite_items_gb_thug.h"

#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

#include "dv_tuning.h"

// Mirrors of the game state for headless tests (gbarun.py --check test_x.32...).
volatile int test_state;            // 0 card, 1 playing, 2 paused, 3 dying, 4 retry fade, 5 clear
volatile int test_player_x;
volatile int test_player_y;
volatile int test_aim;
volatile int test_enemies_alive;
volatile int test_knocked;
volatile int test_score;
volatile int test_combo;
volatile int test_ammo;
volatile int test_held;
volatile int test_deaths;
volatile int test_enemy_shots;
volatile int test_unwarned_shots;   // enemy shots without 10+ frames of "!" on screen first
volatile int test_retry_frames;     // death to playing again
volatile int test_doors_open;
volatile int test_door_knocks;
volatile int test_throw_knocks;
volatile int test_finishes;
volatile int test_hint;
volatile int test_hints_done;
volatile int test_floor_frames;
volatile int test_exit_open;
volatile int test_cpu;
volatile int test_cpu_max;
volatile int test_cpu_max_frame;
volatile int test_cpu_over;      // frames over budget          // per mille of a frame, while playing
volatile int test_particles;
volatile int test_splats;
volatile int test_pointer;
volatile int test_menu;
volatile int test_ex[12];          // per enemy: x, y, state (es)
volatile int test_ey[12];
volatile int test_es[12];

namespace
{
    using namespace dv;

    // -----------------------------------------------------------------------------------------------------
    // Angles: degrees in [0, 360), clockwise from east.

    bn::fixed wrap_angle(bn::fixed a)
    {
        while(a >= 360)
        {
            a -= 360;
        }

        while(a < 0)
        {
            a += 360;
        }

        return a;
    }

    bn::fixed angle_diff(bn::fixed a, bn::fixed b)
    {
        bn::fixed d = a - b;

        while(d > 180)
        {
            d -= 360;
        }

        while(d <= -180)
        {
            d += 360;
        }

        return d;
    }

    bn::fixed abs_fixed(bn::fixed v)
    {
        return v < 0 ? -v : v;
    }

    bn::fixed angle_of(bn::fixed dx, bn::fixed dy)
    {
        return wrap_angle(bn::degrees_atan2((dy * 16).right_shift_integer(), (dx * 16).right_shift_integer()));
    }

    bn::fixed cos_of(bn::fixed a)
    {
        return bn::degrees_lut_cos(a);
    }

    bn::fixed sin_of(bn::fixed a)
    {
        return bn::degrees_lut_sin(a);
    }

    // Hardware rotation is counter-clockwise; our angles are clockwise on screen.
    bn::fixed sprite_rotation(bn::fixed a)
    {
        return a == 0 ? bn::fixed(0) : wrap_angle(360 - a);
    }

    int dist2(bn::fixed ax, bn::fixed ay, bn::fixed bx, bn::fixed by)
    {
        int dx = (ax - bx).right_shift_integer();
        int dy = (ay - by).right_shift_integer();
        return dx * dx + dy * dy;
    }

    // World coordinates start at the map's top-left corner; the 512x512 BGs are centred on the view origin and
    // wrap, so world column c is drawn in BG column c & 63 (see level::set_window).
    constexpr bn::fixed view_x(bn::fixed world)
    {
        return world - 256;
    }

    constexpr bn::fixed view_y(bn::fixed world)
    {
        return world - 256;
    }

    // -----------------------------------------------------------------------------------------------------
    // Map.

    enum class cell : uint8_t { floor, wall, door, door_open, window, bar, exit };

    // Distance fields use a grid padded with one unwalkable tile on every side: no bounds checks in the flood.
    constexpr int pad_w = map_columns + 2;
    constexpr int pad_count = pad_w * (map_rows + 2);

    constexpr int pad_index(int tx, int ty)
    {
        return (ty + 1) * pad_w + tx + 1;
    }

    // BG tile indices (tools/gen_greybox.py).
    constexpr int t_floor = 1;
    constexpr int t_wall = 3;
    constexpr int t_wall_top = 4;
    constexpr int t_wall_left = 5;
    constexpr int t_wall_corner = 6;
    constexpr int t_door_v = 7;
    constexpr int t_door_h = 8;
    constexpr int t_window_h = 9;
    constexpr int t_window_v = 10;
    constexpr int t_bar = 11;
    constexpr int t_exit_closed = 13;
    constexpr int t_exit_open = 14;
    constexpr int t_splat = 15;
    constexpr int t_puddle = 21;

    // Large buffers live in EWRAM (.sbss, zeroed at boot): IWRAM is only 32 KB.
    #define DV_EWRAM __attribute__((section(".sbss")))

    // Both layers (floor and blood splats) are kept at world resolution here and mirrored into 64x64 VRAM maps.
    // The map is 72 cells wide: the VRAM window shows world columns [base, base + 64), moved with the camera.
    // (A Butano big map would do this for us, but reload_cells_ref() on a big map costs over a frame.)
    constexpr int world_cells_x = map_columns * 2;
    constexpr int world_cells_y = map_rows * 2;
    DV_EWRAM bn::regular_bg_map_cell floor_world[world_cells_x * world_cells_y];
    DV_EWRAM bn::regular_bg_map_cell splat_world[world_cells_x * world_cells_y];
    bn::regular_bg_map_cell* floor_vram = nullptr;
    bn::regular_bg_map_cell* splat_vram = nullptr;
    int vram_tiles_offset = 0;          // where the shared tile set landed in VRAM
    int vram_palette_bank = 0;

    constexpr int vram_index(int x, int y)
    {
        return (((y >> 5) * 2 + (x >> 5)) << 10) + ((y & 31) << 5) + (x & 31);
    }

    DV_EWRAM uint8_t player_field[pad_count];
    DV_EWRAM uint8_t enemy_fields[12][pad_count];
    DV_EWRAM int16_t bfs_queue[pad_count];
    DV_EWRAM uint8_t walkable[pad_count];

    struct level
    {
        cell cells[map_rows][map_columns];
        uint8_t plank[map_rows][map_columns];   // open door leaf drawn here: 1 west edge, 2 north edge
        int base = 0;
        int splats = 0;

        [[nodiscard]] cell at(int tx, int ty) const
        {
            if(tx < 0 || ty < 0 || tx >= map_columns || ty >= map_rows)
            {
                return cell::wall;
            }

            return cells[ty][tx];
        }

        [[nodiscard]] static int tile(bn::fixed v)
        {
            return v.right_shift_integer() >> 4;
        }

        [[nodiscard]] bool blocks_walk(int tx, int ty) const
        {
            cell c = at(tx, ty);
            return c == cell::wall || c == cell::window || c == cell::bar;
        }

        [[nodiscard]] bool blocks_bullet(bn::fixed x, bn::fixed y) const
        {
            cell c = at(tile(x), tile(y));
            return c == cell::wall || c == cell::window || c == cell::door;
        }

        [[nodiscard]] bool blocks_sight(bn::fixed x, bn::fixed y) const
        {
            cell c = at(tile(x), tile(y));
            return c == cell::wall || c == cell::door;
        }

        [[nodiscard]] bool passable(int tx, int ty) const
        {
            return ! blocks_walk(tx, ty);
        }

        // A door or window lies in a horizontal wall when its left or right neighbour is wall-like.
        [[nodiscard]] bool in_horizontal_wall(int tx, int ty) const
        {
            cell l = at(tx - 1, ty);
            cell r = at(tx + 1, ty);
            return l == cell::wall || l == cell::window || r == cell::wall || r == cell::window;
        }

        [[nodiscard]] bool sight_clear(bn::fixed x0, bn::fixed y0, bn::fixed x1, bn::fixed y1) const
        {
            bn::fixed dx = x1 - x0;
            bn::fixed dy = y1 - y0;
            int adx = abs_fixed(dx).right_shift_integer();
            int ady = abs_fixed(dy).right_shift_integer();
            int steps = (adx > ady ? adx : ady) / 4 + 1;
            bn::fixed sx = dx / steps;
            bn::fixed sy = dy / steps;
            bn::fixed x = x0;
            bn::fixed y = y0;

            for(int i = 1; i < steps; ++i)
            {
                x += sx;
                y += sy;

                if(blocks_sight(x, y))
                {
                    return false;
                }
            }

            return true;
        }

        void load()
        {
            for(int ty = 0; ty < map_rows; ++ty)
            {
                for(int tx = 0; tx < map_columns; ++tx)
                {
                    char c = floor_1[ty][tx];
                    cell v = cell::floor;

                    switch(c)
                    {

                    case '#':
                        v = cell::wall;
                        break;

                    case 'D':
                        v = cell::door;
                        break;

                    case 'w':
                        v = cell::window;
                        break;

                    case '=':
                        v = cell::bar;
                        break;

                    case 'X':
                        v = cell::exit;
                        break;

                    default:
                        break;
                    }

                    cells[ty][tx] = v;
                    plank[ty][tx] = 0;
                }
            }

            bn::memory::clear(floor_world);
            bn::memory::clear(splat_world);
            splats = 0;

            for(int ty = 0; ty < map_rows; ++ty)
            {
                for(int tx = 0; tx < map_columns; ++tx)
                {
                    draw_tile(tx, ty);
                }
            }

            build_walkable();

            for(int cx = 0; cx < 64; ++cx)
            {
                copy_column(cx);
            }
        }

        // World column shown in VRAM column cx (0..63) for the current window base.
        [[nodiscard]] int world_column(int cx) const
        {
            return cx < base ? cx + 64 : cx;
        }

        void copy_column(int cx)
        {
            int wx = world_column(cx);

            for(int cy = 0; cy < world_cells_y; ++cy)
            {
                floor_vram[vram_index(cx, cy)] = floor_world[cy * world_cells_x + wx];
                splat_vram[vram_index(cx, cy)] = splat_world[cy * world_cells_x + wx];
            }
        }

        // Keep the VRAM window over the camera: left visible column minus a margin, clamped to the map.
        void set_window(int left_column)
        {
            int new_base = left_column - 16;
            new_base = new_base < 0 ? 0 : (new_base > world_cells_x - 64 ? world_cells_x - 64 : new_base);

            if(new_base != base)
            {
                int from = new_base < base ? new_base : base;
                int to = new_base < base ? base : new_base;
                base = new_base;

                for(int cx = from; cx < to; ++cx)
                {
                    copy_column(cx);
                }
            }
        }

        void put(bool splat, int cx, int cy, int tile_index, bool hflip, bool vflip)
        {
            bn::regular_bg_map_cell_info info;
            info.set_tile_index(tile_index + vram_tiles_offset);
            info.set_palette_id(vram_palette_bank);
            info.set_horizontal_flip(hflip);
            info.set_vertical_flip(vflip);
            bn::regular_bg_map_cell value = info.cell();
            (splat ? splat_world : floor_world)[cy * world_cells_x + cx] = value;
            int vx = cx & 63;

            if(world_column(vx) == cx)
            {
                (splat ? splat_vram : floor_vram)[vram_index(vx, cy)] = value;
            }
        }

        // One logical tile = 2x2 cells. Strict top-down: flat blocks, 1 px outline where a wall meets non-wall.
        void draw_tile(int tx, int ty)
        {
            cell c = cells[ty][tx];

            for(int qy = 0; qy < 2; ++qy)
            {
                for(int qx = 0; qx < 2; ++qx)
                {
                    int cx = tx * 2 + qx;
                    int cy = ty * 2 + qy;
                    int tile_index = t_floor;
                    bool hflip = qx == 1;
                    bool vflip = qy == 1;

                    switch(c)
                    {

                    case cell::wall:
                        {
                            bool edge_y = at(tx, ty + (qy ? 1 : -1)) != cell::wall;
                            bool edge_x = at(tx + (qx ? 1 : -1), ty) != cell::wall;
                            tile_index = edge_x && edge_y ? t_wall_corner :
                                                            (edge_y ? t_wall_top : (edge_x ? t_wall_left : t_wall));
                        }
                        break;

                    case cell::door:
                        tile_index = in_horizontal_wall(tx, ty) ? t_door_h : t_door_v;
                        break;

                    case cell::window:
                        tile_index = in_horizontal_wall(tx, ty) ? t_window_h : t_window_v;
                        break;

                    case cell::bar:
                        tile_index = t_bar;
                        hflip = false;
                        vflip = false;
                        break;

                    case cell::exit:
                        tile_index = t_exit_closed;
                        hflip = false;
                        vflip = false;
                        break;

                    default:
                        tile_index = t_floor;
                        hflip = false;
                        vflip = false;

                        // Open door leaf lying along the hinge edge of the swing tile.
                        if(plank[ty][tx] == 1 && qx == 0)
                        {
                            tile_index = t_door_v;
                            hflip = true;
                        }
                        else if(plank[ty][tx] == 2 && qy == 0)
                        {
                            tile_index = t_door_h;
                            vflip = true;
                        }
                        break;
                    }

                    put(false, cx, cy, tile_index, hflip, vflip);
                }
            }

        }

        void set_exit_open()
        {
            for(int ty = 0; ty < map_rows; ++ty)
            {
                for(int tx = 0; tx < map_columns; ++tx)
                {
                    if(cells[ty][tx] == cell::exit)
                    {
                        for(int q = 0; q < 4; ++q)
                        {
                            put(false, tx * 2 + (q & 1), ty * 2 + (q >> 1), t_exit_open, false, false);
                        }
                    }
                }
            }

        }

        void stamp_splat(bn::fixed x, bn::fixed y, bn::random& rng)
        {
            int px = x.right_shift_integer();
            int py = y.right_shift_integer();

            if(px < 0 || py < 0 || at(px >> 4, py >> 4) == cell::wall || at(px >> 4, py >> 4) == cell::bar)
            {
                return;
            }

            put(true, px >> 3, py >> 3, t_splat + rng.get_int(6), rng.get_int(2), rng.get_int(2));
            ++splats;
        }

        void stamp_puddle(bn::fixed x, bn::fixed y)
        {
            int cx = ((x.right_shift_integer() + 4) >> 3) - 1;
            int cy = ((y.right_shift_integer() + 4) >> 3) - 1;

            for(int q = 0; q < 4; ++q)
            {
                int qx = q & 1;
                int qy = q >> 1;
                int x8 = cx + qx;
                int y8 = cy + qy;

                if(x8 >= 0 && y8 >= 0 && x8 < map_columns * 2 && y8 < map_rows * 2 &&
                        at(x8 >> 1, y8 >> 1) != cell::wall)
                {
                    put(true, x8, y8, t_puddle, qx == 1, qy == 1);
                }
            }

            ++splats;
        }

        void build_walkable()
        {
            bn::memory::clear(walkable);

            for(int ty = 0; ty < map_rows; ++ty)
            {
                for(int tx = 0; tx < map_columns; ++tx)
                {
                    walkable[pad_index(tx, ty)] = passable(tx, ty);
                }
            }
        }

        // Breadth-first distances (in tiles) to a target tile; 255 = unreachable. Closed doors are passable.
        static void flood(uint8_t* field, int target_tx, int target_ty)
        {
            bn::memory::set_bytes(255, pad_count, field);
            int start = pad_index(target_tx, target_ty);

            if(target_tx < 0 || target_ty < 0 || target_tx >= map_columns || target_ty >= map_rows ||
                    ! walkable[start])
            {
                return;
            }

            int head = 0;
            int tail = 0;
            field[start] = 0;
            bfs_queue[tail++] = int16_t(start);

            while(head < tail)
            {
                int index = bfs_queue[head++];
                int next = field[index] + 1;

                if(next > 254)
                {
                    continue;
                }

                const int neighbours[4] = { index + 1, index - 1, index + pad_w, index - pad_w };

                for(int ni : neighbours)
                {
                    if(field[ni] == 255 && walkable[ni])
                    {
                        field[ni] = uint8_t(next);
                        bfs_queue[tail++] = int16_t(ni);
                    }
                }
            }
        }
    };

    level lvl;

    // -----------------------------------------------------------------------------------------------------
    // Actors.

    enum class es : uint8_t { idle, patrol, investigate, alert, chase, windup, recover, aim, gap, knocked, dead };

    enum hint_id { hint_move, hint_punch, hint_finish, hint_pick, hint_shoot, hint_throw, hint_count };

    constexpr const char* hint_text[hint_count] = {
        "D-PAD MOVE  L/R AIM", "A: PUNCH", "A: FINISH", "B: PICK UP", "A: SHOOT", "B: THROW",
    };

    bool hint_done[hint_count];         // once per power-on, kept through retries

    struct enemy
    {
        enemy(bool is_gunman, bn::fixed sx, bn::fixed sy, bn::fixed facing, bn::sprite_ptr&& spr) :
            gunman(is_gunman), x(sx), y(sy), face(facing), face_target(facing), sprite(bn::move(spr))
        {
        }

        bool gunman;
        es state = es::idle;
        bn::fixed x;
        bn::fixed y;
        bn::fixed face;
        bn::fixed face_target;
        int timer = 0;
        int look_timer = 0;
        bool looking = false;
        patrol_axis axis = patrol_axis::none;
        int from = 0;
        int to = 0;
        int patrol_dir = 1;
        bn::fixed goal_x;
        bn::fixed goal_y;
        int warned = 0;                 // frames of "!" seen on screen during this aim
        int flash = 0;                  // white hit flash frames
        int trail = 0;
        bool field_pending = false;     // distance field to goal not computed yet (one flood per frame)
        bn::fixed hit_angle;
        bn::sprite_ptr sprite;
        bn::vector<bn::sprite_ptr, 1> mark;
        bn::fixed mark_dx;
        bn::optional<bn::sprite_ptr> muzzle;

        [[nodiscard]] const bn::sprite_item& item() const
        {
            return gunman ? bn::sprite_items::gb_gunman : bn::sprite_items::gb_thug;
        }

        [[nodiscard]] bool alive() const
        {
            return state != es::dead;
        }

        [[nodiscard]] bool standing() const
        {
            return state != es::dead && state != es::knocked;
        }

        [[nodiscard]] bool unaware() const
        {
            return state == es::idle || state == es::patrol || state == es::investigate;
        }
    };

    struct bullet
    {
        bn::fixed x;
        bn::fixed y;
        bn::fixed vx;
        bn::fixed vy;
        bool from_player;
        bn::sprite_ptr sprite;
        bn::sprite_ptr trail;
    };

    struct pickup
    {
        bn::fixed x;
        bn::fixed y;
        int ammo;
        bool flying;
        bn::fixed vx;
        bn::fixed vy;
        int timer;
        bn::sprite_ptr sprite;
    };

    struct particle
    {
        bn::fixed x;
        bn::fixed y;
        bn::fixed vx;
        bn::fixed vy;
        int life;
        bn::sprite_ptr sprite;
    };

    enum class game_state { card, playing, paused, dying, fading, clear };

    class game
    {

    public:
        game_state state = game_state::card;
        bn::camera_ptr camera = bn::camera_ptr::create(0, 0);
        bn::fixed cam_x;
        bn::fixed cam_y;
        bn::fixed px;
        bn::fixed py;
        bn::fixed aim;
        int aim_hold = 0;
        bool held = false;
        int ammo = 0;
        int cooldown = 0;
        int finish_timer = 0;
        int finish_target = -1;
        int score = 0;
        int combo = 0;
        int combo_timer = 0;
        int frames = 0;
        int since_kill = 0;
        int shots_fired = 0;
        int kills = 0;
        int hitstop = 0;
        int shake_frames = 0;
        int shake_amp = 0;
        int player_tile = -1;
        bool exit_open = false;
        bool moved = false;
        bool turned = false;
        int death_frames = 0;
        int final_bonus = 0;
        bn::random rng;
        bn::optional<bn::sprite_ptr> player_sprite;
        bn::vector<bn::sprite_ptr, 3> dots;
        bn::optional<bn::sprite_ptr> arrow;
        bn::vector<enemy, 12> enemies;
        bn::vector<bullet, 16> bullets;
        bn::vector<pickup, 12> pickups;
        bn::vector<particle, blood_particles> particles;
        bn::sprite_text_generator* text = nullptr;

        void load()
        {
            enemies.clear();
            bullets.clear();
            pickups.clear();
            particles.clear();
            dots.clear();
            arrow.reset();
            player_sprite.reset();
            lvl.load();
            rng = bn::random();

            for(int ty = 0; ty < map_rows; ++ty)
            {
                for(int tx = 0; tx < map_columns; ++tx)
                {
                    char c = floor_1[ty][tx];
                    bn::fixed wx = tx * tile_size + tile_size / 2;
                    bn::fixed wy = ty * tile_size + tile_size / 2;

                    if(c == 'P')
                    {
                        px = wx;
                        py = wy;
                    }
                    else if(c == 'p')
                    {
                        add_pickup(wx, wy, floor_pistol_ammo);
                    }
                    else if(c >= '1' && c <= '9')
                    {
                        for(const enemy_spawn& spawn : floor_1_enemies)
                        {
                            if(spawn.id == c && ! enemies.full())
                            {
                                spawn_enemy(spawn, wx, wy);
                            }
                        }
                    }
                }
            }

            aim = 0;
            aim_hold = 0;
            held = false;
            ammo = 0;
            cooldown = 0;
            finish_timer = 0;
            finish_target = -1;
            score = 0;
            combo = 0;
            combo_timer = 0;
            frames = 0;
            since_kill = 0;
            shots_fired = 0;
            kills = 0;
            hitstop = 0;
            shake_frames = 0;
            player_tile = -1;
            exit_open = false;
            moved = false;
            turned = false;
            final_bonus = 0;
            player_sprite = bn::sprite_items::gb_player.create_sprite(0, 0, 0);
            player_sprite->set_camera(camera);
            player_sprite->set_z_order(-1);
            player_sprite->set_rotation_angle(0);

            for(int i = 0; i < 3; ++i)
            {
                bn::sprite_ptr dot = bn::sprite_items::gb_dot.create_sprite(0, 0, 0);
                dot.set_camera(camera);
                dot.set_z_order(-2);
                dots.push_back(bn::move(dot));
            }

            arrow = bn::sprite_items::gb_arrow.create_sprite(0, 0);
            arrow->set_z_order(-4);
            arrow->set_visible(false);
            arrow->set_rotation_angle(0);
            sync_player();
            update_camera(true);
        }

        // Hide or show every world sprite (floor card).
        void set_world_visible(bool visible)
        {
            if(player_sprite)
            {
                player_sprite->set_visible(visible);
            }

            for(enemy& e : enemies)
            {
                e.sprite.set_visible(visible);

                for(bn::sprite_ptr& s : e.mark)
                {
                    s.set_visible(visible);
                }
            }

            for(pickup& p : pickups)
            {
                p.sprite.set_visible(visible);
            }

            for(bn::sprite_ptr& d : dots)
            {
                d.set_visible(visible);
            }
        }

        void update()
        {
            if(hitstop > 0)
            {
                --hitstop;
                return;
            }

            ++frames;
            ++since_kill;

            if(combo_timer > 0 && --combo_timer == 0)
            {
                combo = 0;
            }

            update_player();
            update_player_field();
            update_enemies();
            update_bullets();
            update_pickups();
            update_particles();

            if(! exit_open && alive_enemies() == 0)
            {
                exit_open = true;
                lvl.set_exit_open();
            }

            if(exit_open && state == game_state::playing &&
                    lvl.at(level::tile(px), level::tile(py)) == cell::exit)
            {
                finish_floor();
            }

            update_camera(false);
            update_arrow();
        }

        // Dying: slow motion for the first frames, then only particles.
        void update_dying()
        {
            ++death_frames;

            if(death_frames <= death_slowmo_frames && death_frames % 3 == 0)
            {
                update_enemies();
                update_bullets();
                update_pickups();
            }

            update_particles();
            update_camera(false);
        }

        [[nodiscard]] int alive_enemies() const
        {
            int alive = 0;

            for(const enemy& e : enemies)
            {
                alive += e.alive();
            }

            return alive;
        }

        [[nodiscard]] int knocked_enemies() const
        {
            int knocked = 0;

            for(const enemy& e : enemies)
            {
                knocked += e.state == es::knocked;
            }

            return knocked;
        }

        // Which hint fits right now (first one not done whose condition holds), or -1.
        [[nodiscard]] int wanted_hint() const
        {
            if(state != game_state::playing)
            {
                return -1;
            }

            if(! hint_done[hint_finish])
            {
                for(const enemy& e : enemies)
                {
                    if(e.state == es::knocked && dist2(e.x, e.y, px, py) <= 40 * 40)
                    {
                        return hint_finish;
                    }
                }
            }

            if(! hint_done[hint_throw] && held && ammo == 0)
            {
                return hint_throw;
            }

            if(! hint_done[hint_shoot] && held && ammo > 0)
            {
                return hint_shoot;
            }

            if(! hint_done[hint_pick] && ! held)
            {
                for(const pickup& p : pickups)
                {
                    if(! p.flying && dist2(p.x, p.y, px, py) <= 20 * 20)
                    {
                        return hint_pick;
                    }
                }
            }

            if(! hint_done[hint_punch])
            {
                for(const enemy& e : enemies)
                {
                    if(e.unaware() && dist2(e.x, e.y, px, py) <= 32 * 32)
                    {
                        return hint_punch;
                    }
                }
            }

            if(! hint_done[hint_move])
            {
                return hint_move;
            }

            return -1;
        }

    private:
        void spawn_enemy(const enemy_spawn& spawn, bn::fixed wx, bn::fixed wy)
        {
            const bn::sprite_item& item = spawn.gunman ? bn::sprite_items::gb_gunman : bn::sprite_items::gb_thug;
            bn::sprite_ptr sprite = item.create_sprite(view_x(wx), view_y(wy), 0);
            sprite.set_camera(camera);
            sprite.set_rotation_angle(sprite_rotation(spawn.facing));
            enemy e(spawn.gunman, wx, wy, spawn.facing, bn::move(sprite));
            e.axis = spawn.axis;
            e.from = spawn.from;
            e.to = spawn.to;
            e.state = spawn.axis == patrol_axis::none ? es::idle : es::patrol;
            enemies.push_back(bn::move(e));
        }

        void add_pickup(bn::fixed x, bn::fixed y, int pickup_ammo)
        {
            if(pickups.full())
            {
                return;
            }

            bn::sprite_ptr sprite = bn::sprite_items::gb_pickups.create_sprite(view_x(x), view_y(y),
                                                                               pickup_ammo > 0 ? 0 : 2);
            sprite.set_camera(camera);
            sprite.set_z_order(1);
            pickups.push_back(pickup{ x, y, pickup_ammo, false, 0, 0, 0, bn::move(sprite) });
        }

        void set_mark(enemy& e, const char* mark)
        {
            e.mark.clear();
            text->generate(view_x(e.x), view_y(e.y - 14), mark, e.mark);

            for(bn::sprite_ptr& s : e.mark)
            {
                s.set_camera(camera);
                s.set_z_order(-3);
                e.mark_dx = s.x() - view_x(e.x);
            }
        }

        void shake(int amp, int length)
        {
            if(amp >= shake_amp || shake_frames == 0)
            {
                shake_amp = amp;
                shake_frames = length;
            }
        }

        void update_camera(bool snap)
        {
            bn::fixed tx = view_x(px) + cos_of(aim) * camera_lead_x;
            bn::fixed ty = view_y(py) + sin_of(aim) * camera_lead_y;

            if(snap)
            {
                cam_x = tx;
                cam_y = ty;
            }
            else
            {
                cam_x += (tx - cam_x) / 8;
                cam_y += (ty - cam_y) / 8;
            }

            bn::fixed min_x = view_x(120);
            bn::fixed max_x = view_x(map_columns * tile_size - 120);
            bn::fixed min_y = view_y(80);
            bn::fixed max_y = view_y(map_rows * tile_size - 80);
            cam_x = cam_x < min_x ? min_x : (cam_x > max_x ? max_x : cam_x);
            cam_y = cam_y < min_y ? min_y : (cam_y > max_y ? max_y : cam_y);
            bn::fixed ox = 0;
            bn::fixed oy = 0;

            if(shake_frames > 0)
            {
                --shake_frames;
                ox = (shake_frames & 1) ? shake_amp : -shake_amp;
                oy = (shake_frames & 2) ? shake_amp : -shake_amp;

                if(shake_frames == 0)
                {
                    shake_amp = 0;
                }
            }

            camera.set_position(cam_x.right_shift_integer() + ox, cam_y.right_shift_integer() + oy);
            lvl.set_window((cam_x.right_shift_integer() + 256 - 120) >> 3);
        }

        [[nodiscard]] bool on_screen(bn::fixed x, bn::fixed y, int margin) const
        {
            bn::fixed dx = abs_fixed(view_x(x) - cam_x);
            bn::fixed dy = abs_fixed(view_y(y) - cam_y);
            return dx <= 120 - margin && dy <= 80 - margin;
        }

        // Box movement with wall sliding, corner correction and doors that open when walked into.
        bool box_blocked(bn::fixed x, bn::fixed y, int half) const
        {
            int x0 = level::tile(x - half);
            int x1 = level::tile(x + half);
            int y0 = level::tile(y - half);
            int y1 = level::tile(y + half);
            return lvl.blocks_walk(x0, y0) || lvl.blocks_walk(x1, y0) || lvl.blocks_walk(x0, y1) ||
                    lvl.blocks_walk(x1, y1);
        }

        void open_doors_in_box(bn::fixed x, bn::fixed y, int half, bool by_player)
        {
            int x0 = level::tile(x - half);
            int x1 = level::tile(x + half);
            int y0 = level::tile(y - half);
            int y1 = level::tile(y + half);

            for(int ty = y0; ty <= y1; ++ty)
            {
                for(int tx = x0; tx <= x1; ++tx)
                {
                    if(lvl.at(tx, ty) == cell::door)
                    {
                        open_door(tx, ty, x, y, by_player);
                    }
                }
            }
        }

        void open_door(int tx, int ty, bn::fixed from_x, bn::fixed from_y, bool by_player)
        {
            bool horizontal = lvl.in_horizontal_wall(tx, ty);
            bn::fixed door_x = tx * tile_size + 8;
            bn::fixed door_y = ty * tile_size + 8;
            int sx = 0;
            int sy = 0;

            if(horizontal)
            {
                sy = from_y < door_y ? 1 : -1;
            }
            else
            {
                sx = from_x < door_x ? 1 : -1;
            }

            lvl.cells[ty][tx] = cell::door_open;
            lvl.draw_tile(tx, ty);
            int swing_tx = tx + sx;
            int swing_ty = ty + sy;

            if(lvl.at(swing_tx, swing_ty) == cell::floor)
            {
                lvl.plank[swing_ty][swing_tx] = uint8_t(horizontal ? 1 : 2);
                lvl.draw_tile(swing_tx, swing_ty);
            }

            test_doors_open = test_doors_open + 1;

            if(! by_player)
            {
                return;
            }

            // The door sweeps the 16x16 tile behind it: a free weapon.
            bn::fixed swing_x = swing_tx * tile_size + 8;
            bn::fixed swing_y = swing_ty * tile_size + 8;

            for(enemy& e : enemies)
            {
                if(e.standing() && abs_fixed(e.x - swing_x) < 8 + enemy_hit_half &&
                        abs_fixed(e.y - swing_y) < 8 + enemy_hit_half)
                {
                    knock(e, knock_door, style_door, angle_of(e.x - door_x, e.y - door_y));
                    test_door_knocks = test_door_knocks + 1;
                }
            }

            make_noise(door_x, door_y, noise_door);
            bn::sound_items::blip.play();
        }

        void move_box(bn::fixed& x, bn::fixed& y, bn::fixed dx, bn::fixed dy, int half, bool by_player)
        {
            if(dx != 0)
            {
                if(! box_blocked(x + dx, y, half))
                {
                    x += dx;
                }
                else if(dy == 0)
                {
                    for(int o = 1; o <= corner_correction; ++o)
                    {
                        if(! box_blocked(x + dx, y + o, half) && ! box_blocked(x, y + 1, half))
                        {
                            y += 1;
                            break;
                        }

                        if(! box_blocked(x + dx, y - o, half) && ! box_blocked(x, y - 1, half))
                        {
                            y -= 1;
                            break;
                        }
                    }
                }
            }

            if(dy != 0)
            {
                if(! box_blocked(x, y + dy, half))
                {
                    y += dy;
                }
                else if(dx == 0)
                {
                    for(int o = 1; o <= corner_correction; ++o)
                    {
                        if(! box_blocked(x + o, y + dy, half) && ! box_blocked(x + 1, y, half))
                        {
                            x += 1;
                            break;
                        }

                        if(! box_blocked(x - o, y + dy, half) && ! box_blocked(x - 1, y, half))
                        {
                            x -= 1;
                            break;
                        }
                    }
                }
            }

            open_doors_in_box(x, y, half, by_player);
        }

        void sync_player()
        {
            player_sprite->set_position(view_x(px), view_y(py));
            player_sprite->set_rotation_angle(sprite_rotation(aim));
        }

        // Nearest standing-or-knocked enemy within +-22.5 degrees and 120 px with a clear line, or -1.
        [[nodiscard]] int assist_target() const
        {
            int best = -1;
            bn::fixed best_gap = assist_half_angle + 1;

            for(int i = 0; i < enemies.size(); ++i)
            {
                const enemy& e = enemies[i];

                if(! e.alive() || dist2(e.x, e.y, px, py) > assist_range * assist_range)
                {
                    continue;
                }

                bn::fixed gap = abs_fixed(angle_diff(angle_of(e.x - px, e.y - py), aim));

                if(gap < best_gap && lvl.sight_clear(px, py, e.x, e.y))
                {
                    best_gap = gap;
                    best = i;
                }
            }

            return best;
        }

        [[nodiscard]] int punch_target() const
        {
            for(int i = 0; i < enemies.size(); ++i)
            {
                const enemy& e = enemies[i];

                if(e.standing() && dist2(e.x, e.y, px, py) <= punch_reach * punch_reach &&
                        abs_fixed(angle_diff(angle_of(e.x - px, e.y - py), aim)) <= punch_half_arc)
                {
                    return i;
                }
            }

            return -1;
        }

        [[nodiscard]] int finish_candidate() const
        {
            for(int i = 0; i < enemies.size(); ++i)
            {
                const enemy& e = enemies[i];

                if(e.state == es::knocked && dist2(e.x, e.y, px, py) <= (finish_range + 4) * (finish_range + 4))
                {
                    return i;
                }
            }

            return -1;
        }

        void update_player()
        {
            // Finishing: standing still for finish_frames.
            if(finish_timer > 0)
            {
                if(finish_target < 0 || enemies[finish_target].state != es::knocked)
                {
                    finish_timer = 0;
                }
                else if(--finish_timer == 0)
                {
                    enemy& e = enemies[finish_target];
                    kill(e, style_finish, angle_of(e.x - px, e.y - py));
                    hint_done[hint_finish] = true;
                    test_finishes = test_finishes + 1;
                }

                sync_player();
                update_dots();
                return;
            }

            // Movement: 8 directions, diagonals normalised.
            bn::fixed mx = int(bn::keypad::right_held()) - int(bn::keypad::left_held());
            bn::fixed my = int(bn::keypad::down_held()) - int(bn::keypad::up_held());

            if(mx != 0 && my != 0)
            {
                mx *= bn::fixed(0.7071);
                my *= bn::fixed(0.7071);
            }

            if(mx != 0 || my != 0)
            {
                moved = true;
                move_box(px, py, mx * player_speed, my * player_speed, player_wall_half, true);
            }

            // Aim: continuous angle. A press turns one small step at once; holding turns 6 degrees per frame.
            int turn = int(bn::keypad::r_held()) - int(bn::keypad::l_held());

            if(bn::keypad::l_pressed() || bn::keypad::r_pressed())
            {
                aim = wrap_angle(aim + aim_tap_step * turn);
                aim_hold = 0;
                turned = true;
            }
            else if(turn != 0)
            {
                if(++aim_hold >= aim_hold_delay)
                {
                    aim = wrap_angle(aim + aim_hold_speed * turn);
                }
            }

            if(moved && turned)
            {
                hint_done[hint_move] = true;
            }

            sync_player();

            if(cooldown > 0)
            {
                --cooldown;
            }

            if(bn::keypad::a_pressed())
            {
                attack();
            }

            if(bn::keypad::b_pressed())
            {
                throw_or_pick();
            }

            update_dots();
        }

        void update_dots()
        {
            bool locked = held && ammo > 0 ? assist_target() >= 0 : punch_target() >= 0;
            int frame = locked ? 2 : (held ? 1 : 0);
            constexpr int pistol_dots[3] = { 16, 28, 40 };
            bn::fixed c = cos_of(aim);
            bn::fixed s = sin_of(aim);

            for(int i = 0; i < 3; ++i)
            {
                bn::sprite_ptr& d = dots[i];
                bool visible = held || i == 0;
                d.set_visible(visible);

                if(visible)
                {
                    int r = held ? pistol_dots[i] : 14;
                    d.set_position(view_x(px + c * r), view_y(py + s * r));
                    d.set_tiles(bn::sprite_items::gb_dot.tiles_item(), frame);
                }
            }
        }

        void attack()
        {
            int finish = finish_candidate();

            if(finish >= 0)
            {
                finish_timer = finish_frames;
                finish_target = finish;
                return;
            }

            if(cooldown > 0)
            {
                return;
            }

            if(held && ammo > 0)
            {
                --ammo;
                ++shots_fired;
                cooldown = pistol_cooldown;
                bn::fixed vx = cos_of(aim) * player_bullet_speed;
                bn::fixed vy = sin_of(aim) * player_bullet_speed;
                int target = assist_target();

                if(target >= 0)
                {
                    const enemy& e = enemies[target];
                    bn::fixed dx = e.x - px;
                    bn::fixed dy = e.y - py;
                    int d = bn::sqrt(dist2(e.x, e.y, px, py));

                    if(d > 0)
                    {
                        vx = dx * player_bullet_speed / d;
                        vy = dy * player_bullet_speed / d;
                    }
                }

                spawn_bullet(px + cos_of(aim) * 8, py + sin_of(aim) * 8, vx, vy, true);
                make_noise(px, py, noise_pistol);
                shake(2, 6);
                hint_done[hint_shoot] = true;
                bn::sound_items::blip.play();
                return;
            }

            // Punch (also with an empty pistol): 90 degree arc, knocks out.
            cooldown = punch_cooldown;
            hint_done[hint_punch] = hint_done[hint_punch] || punch_target() >= 0;
            int target = punch_target();

            if(target >= 0)
            {
                enemy& e = enemies[target];
                knock(e, knock_punch, 0, angle_of(e.x - px, e.y - py));
            }
        }

        void throw_or_pick()
        {
            if(held)
            {
                if(! pickups.full())
                {
                    bn::sprite_ptr sprite = bn::sprite_items::gb_pickups.create_sprite(view_x(px), view_y(py),
                                                                                       ammo > 0 ? 0 : 2);
                    sprite.set_camera(camera);
                    sprite.set_z_order(-1);
                    pickups.push_back(pickup{ px, py, ammo, true, cos_of(aim) * throw_speed,
                                              sin_of(aim) * throw_speed, throw_frames, bn::move(sprite) });
                }

                held = false;
                ammo = 0;
                hint_done[hint_throw] = true;
                return;
            }

            for(int i = 0; i < pickups.size(); ++i)
            {
                pickup& p = pickups[i];

                if(! p.flying && dist2(p.x, p.y, px, py) <= 16 * 16)
                {
                    held = true;
                    ammo = p.ammo;
                    pickups.erase(pickups.begin() + i);
                    hint_done[hint_pick] = true;
                    return;
                }
            }
        }

        void spawn_bullet(bn::fixed x, bn::fixed y, bn::fixed vx, bn::fixed vy, bool from_player)
        {
            if(bullets.full())
            {
                return;
            }

            bn::sprite_ptr sprite = bn::sprite_items::gb_bullet.create_sprite(view_x(x), view_y(y),
                                                                              from_player ? 0 : 1);
            sprite.set_camera(camera);
            sprite.set_z_order(-2);
            bn::sprite_ptr trail = bn::sprite_items::gb_bullet.create_sprite(view_x(x), view_y(y),
                                                                             from_player ? 3 : 4);
            trail.set_camera(camera);
            trail.set_z_order(-2);
            bullets.push_back(bullet{ x, y, vx, vy, from_player, bn::move(sprite), bn::move(trail) });
        }

        void make_noise(bn::fixed x, bn::fixed y, int radius)
        {
            for(int i = 0; i < enemies.size(); ++i)
            {
                enemy& e = enemies[i];

                if(e.unaware() && dist2(e.x, e.y, x, y) < radius * radius)
                {
                    investigate(e, i, x, y);
                }
            }
        }

        void investigate(enemy& e, int index, bn::fixed x, bn::fixed y)
        {
            e.state = es::investigate;
            e.goal_x = x;
            e.goal_y = y;
            e.looking = false;
            e.field_pending = true;
            static_cast<void>(index);
            set_mark(e, "?");
        }

        void spawn_blood(bn::fixed x, bn::fixed y, bn::fixed angle, int count)
        {
            for(int i = 0; i < count && ! particles.full(); ++i)
            {
                bn::fixed a = wrap_angle(angle - 35 + rng.get_fixed(70));
                bn::fixed speed = bn::fixed(1.5) + rng.get_fixed(2);
                bn::sprite_ptr sprite = bn::sprite_items::gb_blood.create_sprite(view_x(x), view_y(y),
                                                                                 rng.get_int(3));
                sprite.set_camera(camera);
                sprite.set_z_order(1);
                particles.push_back(particle{ x, y, cos_of(a) * speed, sin_of(a) * speed, 20 + rng.get_int(11),
                                              bn::move(sprite) });
            }
        }

        void add_score(int points)
        {
            score += points * (combo > 0 ? combo : 1);
        }

        void knock(enemy& e, int frames_out, int style, bn::fixed angle)
        {
            e.state = es::knocked;
            e.timer = frames_out;
            e.trail = knock_trail_interval;
            e.mark.clear();
            e.muzzle.reset();
            e.flash = hit_flash_frames;
            e.sprite.set_tiles(e.item().tiles_item(), 1);
            spawn_blood(e.x, e.y, angle, 4);
            add_score(style);
        }

        void kill(enemy& e, int style, bn::fixed angle)
        {
            e.state = es::dead;
            e.mark.clear();
            e.muzzle.reset();
            e.flash = hit_flash_frames;
            e.hit_angle = angle;
            e.sprite.set_z_order(2);

            if(combo > 0 && combo_timer > 0)
            {
                combo = combo < combo_max ? combo + 1 : combo_max;
            }
            else
            {
                combo = 1;
            }

            combo_timer = combo_window;
            add_score((e.gunman ? gunman_points : goon_points) + style);
            since_kill = 0;
            ++kills;
            hitstop = hitstop_frames;
            spawn_blood(e.x, e.y, angle, 10 + rng.get_int(7));
            bn::sound_items::blip.play();

            if(e.gunman)
            {
                add_pickup(e.x + 6, e.y + 4, dropped_pistol_ammo);
            }
        }

        void kill_player(bn::fixed angle)
        {
            state = game_state::dying;
            death_frames = 0;
            test_deaths = test_deaths + 1;
            player_sprite->set_tiles(bn::sprite_items::gb_player.tiles_item(), 3);
            player_sprite->remove_affine_mat();
            player_sprite->set_z_order(2);
            spawn_blood(px, py, angle, 14);
            lvl.stamp_puddle(px, py);
            shake(4, 10);

            for(bn::sprite_ptr& d : dots)
            {
                d.set_visible(false);
            }

            bn::sound_items::blip.play();
        }

        void finish_floor()
        {
            int seconds = frames / 60;
            final_bonus = 0;

            if(seconds < par_seconds)
            {
                final_bonus += (par_seconds - seconds) * time_bonus_per_second;
            }

            if(shots_fired == 0)
            {
                final_bonus += no_shot_bonus;
            }

            score += final_bonus;
            state = game_state::clear;
            test_floor_frames = frames;
        }

        [[nodiscard]] bool sees_player(const enemy& e, int range) const
        {
            int d2 = dist2(e.x, e.y, px, py);

            if(d2 > range * range)
            {
                return false;
            }

            if(d2 > vision_near * vision_near &&
                    abs_fixed(angle_diff(angle_of(px - e.x, py - e.y), e.face)) > vision_half_angle)
            {
                return false;
            }

            return lvl.sight_clear(e.x, e.y, px, py);
        }

        void update_player_field()
        {
            int tile = level::tile(py) * map_columns + level::tile(px);

            // At most one flood per frame: a pending enemy goal first, then the player's field.
            for(int i = 0; i < enemies.size(); ++i)
            {
                enemy& e = enemies[i];

                if(e.field_pending)
                {
                    e.field_pending = false;
                    level::flood(enemy_fields[i], level::tile(e.goal_x), level::tile(e.goal_y));
                    return;
                }
            }

            if(tile != player_tile)
            {
                player_tile = tile;
                level::flood(player_field, level::tile(px), level::tile(py));
            }
        }

        // Walk along a distance field towards a goal point.
        void navigate(enemy& e, const uint8_t* field, bn::fixed goal_x, bn::fixed goal_y, bn::fixed speed)
        {
            int tx = level::tile(e.x);
            int ty = level::tile(e.y);
            int d = field[pad_index(tx, ty)];
            bn::fixed gx = goal_x;
            bn::fixed gy = goal_y;

            if(d == 255)
            {
                return;
            }

            if(d > 1)
            {
                constexpr int ox[4] = { 1, -1, 0, 0 };
                constexpr int oy[4] = { 0, 0, 1, -1 };
                int best = d;

                for(int n = 0; n < 4; ++n)
                {
                    int nx = tx + ox[n];
                    int ny = ty + oy[n];

                    int nd = field[pad_index(nx, ny)];

                    if(nd < best)
                    {
                        best = nd;
                        gx = nx * tile_size + 8;
                        gy = ny * tile_size + 8;
                    }
                }
            }

            step_toward(e, gx, gy, speed);
        }

        bool step_toward(enemy& e, bn::fixed gx, bn::fixed gy, bn::fixed speed)
        {
            bn::fixed dx = gx - e.x;
            bn::fixed dy = gy - e.y;
            int d = bn::sqrt(dist2(gx, gy, e.x, e.y));

            if(d <= speed)
            {
                move_box(e.x, e.y, dx, dy, enemy_wall_half, false);
                return true;
            }

            e.face_target = angle_of(dx, dy);
            move_box(e.x, e.y, dx * speed / d, dy * speed / d, enemy_wall_half, false);
            return false;
        }

        void spot(enemy& e)
        {
            e.face_target = angle_of(px - e.x, py - e.y);

            if(e.gunman)
            {
                e.state = es::aim;
                e.timer = gunman_aim_frames;
                e.warned = 0;
            }
            else
            {
                e.state = es::alert;
                e.timer = goon_alert_frames;
            }

            set_mark(e, "!");
        }

        void update_enemies()
        {
            bool player_alive = state == game_state::playing;

            for(int i = 0; i < enemies.size(); ++i)
            {
                enemy& e = enemies[i];

                switch(e.state)
                {

                case es::idle:
                    if(e.looking && ++e.look_timer >= 45)
                    {
                        e.look_timer = 0;
                        e.face_target = wrap_angle(e.face_target + 90);
                    }
                    break;

                case es::patrol:
                    {
                        bn::fixed speed = e.gunman ? gunman_patrol_speed : goon_patrol_speed;
                        bool horizontal = e.axis == patrol_axis::horizontal;
                        int end = e.patrol_dir > 0 ? e.to : e.from;
                        bn::fixed goal = end * tile_size + 8;
                        bn::fixed& pos = horizontal ? e.x : e.y;
                        bn::fixed old = pos;
                        bn::fixed delta = goal - pos;
                        bn::fixed step = abs_fixed(delta) < speed ? abs_fixed(delta) : speed;

                        if(horizontal)
                        {
                            move_box(e.x, e.y, delta > 0 ? step : -step, 0, enemy_wall_half, false);
                            e.face_target = delta > 0 ? 0 : 180;
                        }
                        else
                        {
                            move_box(e.x, e.y, 0, delta > 0 ? step : -step, enemy_wall_half, false);
                            e.face_target = delta > 0 ? 90 : 270;
                        }

                        if(abs_fixed(delta) <= speed || pos == old)
                        {
                            e.patrol_dir = -e.patrol_dir;
                        }
                    }
                    break;

                case es::investigate:
                    if(dist2(e.x, e.y, e.goal_x, e.goal_y) <= 6 * 6)
                    {
                        e.state = es::idle;
                        e.looking = true;
                        e.look_timer = 0;
                        e.mark.clear();
                    }
                    else if(! e.field_pending)
                    {
                        navigate(e, enemy_fields[i], e.goal_x, e.goal_y, investigate_speed);
                    }
                    break;

                case es::alert:
                    e.face_target = angle_of(px - e.x, py - e.y);

                    if(--e.timer <= 0)
                    {
                        e.mark.clear();
                        e.state = es::chase;
                    }
                    break;

                case es::chase:
                    if(player_alive)
                    {
                        int d2 = dist2(e.x, e.y, px, py);

                        if(d2 <= goon_windup_distance * goon_windup_distance && lvl.sight_clear(e.x, e.y, px, py))
                        {
                            e.state = es::windup;
                            e.timer = goon_windup_frames;
                            e.face_target = angle_of(px - e.x, py - e.y);
                        }
                        else
                        {
                            navigate(e, player_field, px, py, goon_chase_speed);
                        }
                    }
                    break;

                case es::windup:
                    if(--e.timer <= 0)
                    {
                        bool hit = player_alive && dist2(e.x, e.y, px, py) <= goon_hit_reach * goon_hit_reach &&
                                abs_fixed(angle_diff(angle_of(px - e.x, py - e.y), e.face)) <= 45;

                        if(hit)
                        {
                            kill_player(angle_of(px - e.x, py - e.y));
                        }

                        e.state = es::recover;
                        e.timer = goon_recover_frames;
                    }
                    break;

                case es::recover:
                    if(--e.timer <= 0)
                    {
                        e.state = es::chase;
                    }
                    break;

                case es::aim:
                    update_aim(e, i, player_alive);
                    break;

                case es::gap:
                    e.face_target = angle_of(px - e.x, py - e.y);

                    if(--e.timer <= 0 && player_alive)
                    {
                        if(sees_player(e, vision_range + 48))
                        {
                            e.state = es::aim;
                            e.timer = gunman_aim_frames;
                            e.warned = 0;
                            set_mark(e, "!");
                        }
                        else
                        {
                            investigate(e, i, px, py);
                        }
                    }
                    break;

                case es::knocked:
                    if(--e.trail <= 0)
                    {
                        e.trail = knock_trail_interval;
                        lvl.stamp_splat(e.x, e.y, rng);
                    }

                    if(--e.timer <= 0)
                    {
                        e.sprite.set_tiles(e.item().tiles_item(), 0);
                        spot(e);
                    }
                    break;

                default:
                    break;
                }

                if(player_alive && e.unaware() && sees_player(e, vision_range))
                {
                    spot(e);
                }

                sync_enemy(e);

                if(state != game_state::playing && player_alive)
                {
                    player_alive = false;
                }
            }
        }

        void update_aim(enemy& e, int index, bool player_alive)
        {
            e.face_target = angle_of(px - e.x, py - e.y);

            if(! player_alive)
            {
                return;
            }

            if(! sees_player(e, vision_range + 48))
            {
                e.muzzle.reset();
                investigate(e, index, px, py);
                return;
            }

            bool visible = on_screen(e.x, e.y, offscreen_margin);
            e.warned += visible;

            // The last frames (muzzle flash) only run while the gunman is on screen: nothing fires unseen.
            if(e.timer > gunman_flash_frames || visible)
            {
                --e.timer;
            }

            if(e.timer <= gunman_flash_frames && e.timer > 0)
            {
                if(! e.muzzle)
                {
                    e.muzzle = bn::sprite_items::gb_bullet.create_sprite(0, 0, 2);
                    e.muzzle->set_camera(camera);
                    e.muzzle->set_z_order(-2);
                }

                e.muzzle->set_visible((e.timer & 2) != 0);
                e.muzzle->set_position(view_x(e.x + cos_of(e.face) * 10), view_y(e.y + sin_of(e.face) * 10));
            }

            if(e.timer <= 0)
            {
                e.muzzle.reset();
                e.mark.clear();
                test_enemy_shots = test_enemy_shots + 1;

                if(e.warned < gunman_flash_frames)
                {
                    test_unwarned_shots = test_unwarned_shots + 1;
                }

                int d = bn::sqrt(dist2(px, py, e.x, e.y));
                bn::fixed vx = cos_of(e.face) * enemy_bullet_speed;
                bn::fixed vy = sin_of(e.face) * enemy_bullet_speed;

                if(d > 0)
                {
                    vx = (px - e.x) * enemy_bullet_speed / d;
                    vy = (py - e.y) * enemy_bullet_speed / d;
                }

                spawn_bullet(e.x + cos_of(e.face) * 8, e.y + sin_of(e.face) * 8, vx, vy, false);
                e.state = es::gap;
                e.timer = gunman_shot_gap;
                bn::sound_items::blip.play();
            }
        }

        void sync_enemy(enemy& e)
        {
            if(e.state == es::dead)
            {
                if(e.flash > 0 && --e.flash == 0)
                {
                    e.sprite.set_tiles(e.item().tiles_item(), 3);
                    e.sprite.remove_affine_mat();
                    lvl.stamp_puddle(e.x, e.y);
                }
                else if(e.flash > 0)
                {
                    e.sprite.set_tiles(e.item().tiles_item(), 2);
                }

                e.sprite.set_position(view_x(e.x), view_y(e.y));
                return;
            }

            bn::fixed d = angle_diff(e.face_target, e.face);
            d = d > enemy_turn_speed ? enemy_turn_speed : (d < -enemy_turn_speed ? -enemy_turn_speed : d);
            e.face = wrap_angle(e.face + d);
            bn::fixed sx = view_x(e.x);

            if(e.state == es::knocked && e.timer <= knock_shake)
            {
                sx += (e.timer & 2) ? 1 : -1;
            }

            int frame = 0;

            if(e.flash > 0)
            {
                --e.flash;
                frame = 2;
            }
            else if(e.state == es::knocked)
            {
                frame = 1;
            }
            else if(e.state == es::windup)
            {
                frame = (e.timer / 3) & 1 ? 2 : 0;
            }

            e.sprite.set_tiles(e.item().tiles_item(), frame);
            e.sprite.set_position(sx, view_y(e.y));
            e.sprite.set_rotation_angle(sprite_rotation(e.face));

            for(bn::sprite_ptr& s : e.mark)
            {
                s.set_position(view_x(e.x) + e.mark_dx, view_y(e.y - 14));
            }
        }

        void update_bullets()
        {
            for(int i = 0; i < bullets.size();)
            {
                bullet& b = bullets[i];
                bool gone = false;
                b.trail.set_position(view_x(b.x), view_y(b.y));

                // Two sub-steps so fast bullets do not skip thin walls or targets.
                for(int step = 0; step < 2 && ! gone; ++step)
                {
                    b.x += b.vx / 2;
                    b.y += b.vy / 2;

                    if(lvl.blocks_bullet(b.x, b.y))
                    {
                        gone = true;
                        break;
                    }

                    if(b.from_player)
                    {
                        for(enemy& e : enemies)
                        {
                            if(e.alive() && abs_fixed(e.x - b.x) < enemy_hit_half + 2 &&
                                    abs_fixed(e.y - b.y) < enemy_hit_half + 2)
                            {
                                kill(e, 0, angle_of(b.vx, b.vy));
                                gone = true;
                                break;
                            }
                        }
                    }
                    else if(state == game_state::playing)
                    {
                        if(abs_fixed(px - b.x) < player_hit_half + 2 && abs_fixed(py - b.y) < player_hit_half + 2)
                        {
                            kill_player(angle_of(b.vx, b.vy));
                            gone = true;
                        }
                    }
                }

                if(gone)
                {
                    bullets.erase(bullets.begin() + i);
                }
                else
                {
                    b.sprite.set_position(view_x(b.x), view_y(b.y));
                    ++i;
                }
            }
        }

        void update_pickups()
        {
            for(pickup& p : pickups)
            {
                if(! p.flying)
                {
                    continue;
                }

                bn::fixed nx = p.x + p.vx;
                bn::fixed ny = p.y + p.vy;

                if(--p.timer <= 0 || lvl.blocks_bullet(nx, ny))
                {
                    p.flying = false;
                    p.sprite.set_z_order(1);
                }
                else
                {
                    p.x = nx;
                    p.y = ny;
                }

                for(enemy& e : enemies)
                {
                    if(p.flying && e.standing() && abs_fixed(e.x - p.x) < enemy_hit_half + 4 &&
                            abs_fixed(e.y - p.y) < enemy_hit_half + 4)
                    {
                        knock(e, knock_throw, style_throw, angle_of(p.vx, p.vy));
                        test_throw_knocks = test_throw_knocks + 1;
                        p.flying = false;
                        p.sprite.set_z_order(1);
                    }
                }

                p.sprite.set_position(view_x(p.x), view_y(p.y));
            }
        }

        void update_particles()
        {
            for(int i = 0; i < particles.size();)
            {
                particle& p = particles[i];
                p.x += p.vx;
                p.y += p.vy;
                p.vx *= bn::fixed(0.85);
                p.vy *= bn::fixed(0.85);

                if(--p.life <= 0)
                {
                    lvl.stamp_splat(p.x, p.y, rng);
                    particles.erase(particles.begin() + i);
                }
                else
                {
                    p.sprite.set_position(view_x(p.x), view_y(p.y));
                    ++i;
                }
            }
        }

        // After 10 s without a kill, an arrow on the screen edge points at the nearest living enemy; once the
        // floor is clear it points at the exit straight away (rule 9).
        void update_arrow()
        {
            bn::fixed tx;
            bn::fixed ty;
            bool show = false;

            if(exit_open)
            {
                tx = (map_columns - 2) * tile_size + 8;
                ty = 1 * tile_size + 8;
                show = true;
            }
            else if(since_kill >= pointer_delay)
            {
                int best = -1;

                for(const enemy& e : enemies)
                {
                    int d2 = dist2(e.x, e.y, px, py);

                    if(e.alive() && (best < 0 || d2 < best))
                    {
                        best = d2;
                        tx = e.x;
                        ty = e.y;
                        show = true;
                    }
                }
            }

            if(show && on_screen(tx, ty, 8))
            {
                show = false;
            }

            arrow->set_visible(show);
            test_pointer = show;

            if(show)
            {
                bn::fixed rx = view_x(tx) - cam_x;
                bn::fixed ry = view_y(ty) - cam_y;
                bn::fixed ax = rx < -108 ? bn::fixed(-108) : (rx > 108 ? bn::fixed(108) : rx);
                bn::fixed ay = ry < -60 ? bn::fixed(-60) : (ry > 52 ? bn::fixed(52) : ry);
                arrow->set_position(ax, ay);
                arrow->set_rotation_angle(sprite_rotation(angle_of(rx, ry)));
            }
        }
    };
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator small_text(common::variable_8x8_sprite_font);
    bn::sprite_text_generator big_text(common::variable_8x16_sprite_font);
    small_text.set_center_alignment();
    big_text.set_center_alignment();

    // Two 64x64 BGs share one tile set: the floor and, on top of it, blood splats. Cells are written straight
    // into their VRAM (allocated maps), so a door or a splat costs a few stores, not a map reload.
    bn::regular_bg_tiles_ptr bg_tiles = bn::regular_bg_tiles_items::gb_tiles.create_tiles();
    bn::bg_palette_ptr bg_palette = bn::bg_palette_items::gb_tiles_palette.create_palette();
    bn::regular_bg_map_ptr floor_map = bn::regular_bg_map_ptr::allocate(bn::size(64, 64), bg_tiles, bg_palette);
    bn::regular_bg_map_ptr splat_map = bn::regular_bg_map_ptr::allocate(bn::size(64, 64), bg_tiles, bg_palette);
    vram_tiles_offset = floor_map.tiles_offset();
    vram_palette_bank = floor_map.palette_banks_offset();
    floor_vram = floor_map.vram()->data();
    splat_vram = splat_map.vram()->data();
    bn::memory::clear(64 * 64, *floor_vram);
    bn::memory::clear(64 * 64, *splat_vram);
    bn::regular_bg_ptr floor_bg = bn::regular_bg_ptr::create(floor_map);
    bn::regular_bg_ptr splat_bg = bn::regular_bg_ptr::create(splat_map);
    splat_bg.set_z_order(-1);
    lvl.load();

    game g;
    g.text = &small_text;
    floor_bg.set_camera(g.camera);
    splat_bg.set_camera(g.camera);

    bn::vector<bn::sprite_ptr, 16> hud;
    bn::vector<bn::sprite_ptr, 32> message;
    bn::vector<bn::sprite_ptr, 8> hint_sprites;
    bn::optional<bn::sprite_ptr> combo_bar;
    int shown_score = -1;
    int shown_ammo = -1;
    int shown_held = -1;
    int shown_combo = -1;
    int shown_hint = -2;
    int shown_menu = -1;
    game_state shown_state = game_state::playing;
    int state_frames = 0;
    int menu = 0;
    int retry_clock = -1;
    bool hud_visible = false;

    auto start_card = [&]()
    {
        g.load();
        g.state = game_state::card;
        g.set_world_visible(false);
        bn::bg_palettes::set_fade(bn::color(0, 0, 0), 1);
    };

    auto restart = [&]()
    {
        g.load();
        g.state = game_state::playing;
        bn::bg_palettes::set_fade(bn::color(0, 0, 0), 0);
        bn::sprite_palettes::set_fade(bn::color(0, 0, 0), 0);
    };

    start_card();

    while(true)
    {
        ++state_frames;

        switch(g.state)
        {

        case game_state::card:
            if(state_frames >= card_frames || (state_frames > 1 && bn::keypad::a_pressed()))
            {
                g.state = game_state::playing;
                g.set_world_visible(true);
                bn::bg_palettes::set_fade(bn::color(0, 0, 0), 0);
            }
            break;

        case game_state::playing:
            if(bn::keypad::start_pressed())
            {
                g.state = game_state::paused;
                menu = 0;
                shown_menu = -1;
            }
            else
            {
                g.update();

                if(g.state == game_state::dying)
                {
                    retry_clock = 0;
                    bn::bg_palettes::set_fade(bn::color(31, 0, 0), 0.4);
                    bn::sprite_palettes::set_fade(bn::color(31, 0, 0), 0.3);
                }
            }
            break;

        case game_state::paused:
            if(bn::keypad::up_pressed())
            {
                menu = (menu + 2) % 3;
            }
            else if(bn::keypad::down_pressed())
            {
                menu = (menu + 1) % 3;
            }

            if(bn::keypad::start_pressed() || (bn::keypad::a_pressed() && menu == 0))
            {
                g.state = game_state::playing;
            }
            else if(bn::keypad::a_pressed() && menu == 1)
            {
                restart();
            }
            else if(bn::keypad::a_pressed() && menu == 2)
            {
                start_card();
            }
            break;

        case game_state::dying:
            g.update_dying();

            if(state_frames >= death_retry_frame && bn::keypad::a_pressed())
            {
                g.state = game_state::fading;
            }
            break;

        case game_state::fading:
            {
                bn::fixed f = bn::fixed(state_frames) / retry_fade_frames;
                f = f > 1 ? bn::fixed(1) : f;
                bn::bg_palettes::set_fade(bn::color(0, 0, 0), f);
                bn::sprite_palettes::set_fade(bn::color(0, 0, 0), f);

                if(state_frames >= retry_fade_frames)
                {
                    restart();
                }
            }
            break;

        case game_state::clear:
            if(state_frames > 30 && bn::keypad::a_pressed())
            {
                start_card();
            }
            break;

        default:
            break;
        }

        if(g.state == game_state::playing && retry_clock >= 0)
        {
            test_retry_frames = retry_clock;
            retry_clock = -1;
        }
        else if(retry_clock >= 0)
        {
            ++retry_clock;
        }


        // HUD: score top-left, combo top-right with its window bar, weapon bottom-left.
        bool want_hud = g.state != game_state::card;

        if(want_hud != hud_visible)
        {
            hud_visible = want_hud;
            shown_score = -1;
            hud.clear();
            combo_bar.reset();
        }

        int held = g.held;

        if(hud_visible && (g.score != shown_score || g.ammo != shown_ammo || held != shown_held ||
                           g.combo != shown_combo))
        {
            shown_score = g.score;
            shown_ammo = g.ammo;
            shown_held = held;
            shown_combo = g.combo;
            hud.clear();
            bn::string<24> line(g.held ? "PISTOL " : "FISTS");

            if(g.held)
            {
                line += bn::to_string<4>(g.ammo);
            }

            small_text.set_left_alignment();
            small_text.generate(-116, 72, line, hud);
            line = bn::to_string<8>(g.score);
            small_text.generate(-116, -72, line, hud);
            small_text.set_right_alignment();

            if(g.combo > 1)
            {
                line = "x";
                line += bn::to_string<4>(g.combo);
                small_text.generate(116, -72, line, hud);
            }

            small_text.set_center_alignment();

            for(bn::sprite_ptr& s : hud)
            {
                s.set_z_order(-5);
            }
        }

        if(hud_visible && g.combo > 0 && g.combo_timer > 0)
        {
            if(! combo_bar)
            {
                combo_bar = bn::sprite_items::gb_bar.create_sprite(100, -62, 0);
                combo_bar->set_z_order(-5);
            }

            combo_bar->set_tiles(bn::sprite_items::gb_bar.tiles_item(), g.combo_timer * 16 / combo_window);
        }
        else
        {
            combo_bar.reset();
        }

        // Hint line at the bottom, one at a time.
        int hint = g.wanted_hint();

        if(hint != shown_hint)
        {
            shown_hint = hint;
            hint_sprites.clear();

            if(hint >= 0)
            {
                small_text.generate(0, 58, hint_text[hint], hint_sprites);
            }
        }

        // Full-screen messages.
        if(g.state != shown_state || (g.state == game_state::paused && menu != shown_menu))
        {
            if(g.state != shown_state)
            {
                state_frames = 0;
            }

            shown_state = g.state;
            shown_menu = menu;
            message.clear();

            switch(g.state)
            {

            case game_state::card:
                big_text.generate(0, -8, "FLOOR 1 - LOBBY", message);
                small_text.generate(0, 12, "A: START", message);
                break;

            case game_state::paused:
                big_text.generate(0, -28, "PAUSED", message);
                small_text.generate(0, -4, menu == 0 ? "> RESUME <" : "RESUME", message);
                small_text.generate(0, 8, menu == 1 ? "> RETRY <" : "RETRY", message);
                small_text.generate(0, 20, menu == 2 ? "> QUIT <" : "QUIT", message);
                break;

            case game_state::dying:
                big_text.generate(0, -12, "YOU'RE DEAD", message);
                small_text.generate(0, 8, "A: RETRY", message);
                break;

            case game_state::clear:
                {
                    big_text.generate(0, -32, "FLOOR CLEAR", message);
                    bn::string<32> line("SCORE ");
                    line += bn::to_string<8>(g.score);
                    small_text.generate(0, -10, line, message);
                    line = "TIME ";
                    line += bn::to_string<4>(g.frames / 60);
                    line += " SEC  BONUS ";
                    line += bn::to_string<6>(g.final_bonus);
                    small_text.generate(0, 2, line, message);
                    small_text.generate(0, 20, "A: PLAY AGAIN", message);
                }
                break;

            default:
                break;
            }

            for(bn::sprite_ptr& s : message)
            {
                s.set_z_order(-6);
            }
        }

        if(g.state == game_state::playing && g.frames > 2)
        {
            int cpu = (bn::core::last_cpu_usage() * 1000).right_shift_integer();

            if(cpu > test_cpu_max)
            {
                test_cpu_max = cpu;
                test_cpu_max_frame = g.frames;
            }

            test_cpu_over += cpu > 1000;
            test_cpu = cpu;
        }

        int hints_mask = 0;

        for(int i = 0; i < hint_count; ++i)
        {
            hints_mask |= int(hint_done[i]) << i;
        }

        test_state = int(g.state);
        test_player_x = g.px.right_shift_integer();
        test_player_y = g.py.right_shift_integer();
        test_aim = g.aim.right_shift_integer();
        test_enemies_alive = g.alive_enemies();
        test_knocked = g.knocked_enemies();
        test_score = g.score;
        test_combo = g.combo;
        test_ammo = g.ammo;
        test_held = g.held;
        test_hint = hint;
        test_hints_done = hints_mask;
        test_exit_open = g.exit_open;
        test_particles = g.particles.size();
        test_splats = lvl.splats;
        test_menu = menu;

        for(int i = 0; i < g.enemies.size(); ++i)
        {
            test_ex[i] = g.enemies[i].x.right_shift_integer();
            test_ey[i] = g.enemies[i].y.right_shift_integer();
            test_es[i] = int(g.enemies[i].state);
        }

        bn::core::update();
    }
}
