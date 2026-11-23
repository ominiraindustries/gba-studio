// DISCO VENDETTA: top-down one-hit-kill action in a 70s mafia setting.
// Greybox core: tile map with camera, 8-way movement, L/R aim in 16 steps, punch and pistol, thrown weapons,
// thugs and gunmen that patrol, spot the player (with a "!" warning), chase and attack. Instant restart.

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_bg_tiles.h"
#include "bn_camera_ptr.h"
#include "bn_fixed.h"
#include "bn_memory.h"
#include "bn_optional.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sound_items.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_vector.h"

#include "bn_bg_palette_items_gb_tiles_palette.h"
#include "bn_regular_bg_tiles_items_gb_tiles.h"
#include "bn_sprite_items_gb_bullet.h"
#include "bn_sprite_items_gb_gunman.h"
#include "bn_sprite_items_gb_pickups.h"
#include "bn_sprite_items_gb_player.h"
#include "bn_sprite_items_gb_thug.h"

#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

#include "dv_tuning.h"

// Mirrors of the game state for headless tests (gbarun.py --check test_x.32...).
volatile int test_state;
volatile int test_player_x;
volatile int test_player_y;
volatile int test_aim;
volatile int test_enemies_alive;
volatile int test_score;
volatile int test_ammo;
volatile int test_deaths;

namespace
{
    using namespace dv;

    // 16 aim directions, clockwise from east (screen y grows down).
    constexpr bn::fixed dir_x[16] = {
        1, 0.9239, 0.7071, 0.3827, 0, -0.3827, -0.7071, -0.9239,
        -1, -0.9239, -0.7071, -0.3827, 0, 0.3827, 0.7071, 0.9239,
    };
    constexpr bn::fixed dir_y[16] = {
        0, 0.3827, 0.7071, 0.9239, 1, 0.9239, 0.7071, 0.3827,
        0, -0.3827, -0.7071, -0.9239, -1, -0.9239, -0.7071, -0.3827,
    };
    constexpr int dead_frame = 16;

    // Nearest of the 16 directions to a vector.
    int direction_to(bn::fixed dx, bn::fixed dy)
    {
        int best = 0;
        bn::fixed best_dot = -1000;

        for(int d = 0; d < 16; ++d)
        {
            bn::fixed dot = dx * dir_x[d] + dy * dir_y[d];

            if(dot > best_dot)
            {
                best_dot = dot;
                best = d;
            }
        }

        return best;
    }

    int direction_gap(int a, int b)
    {
        int gap = (a - b) & 15;
        return gap > 8 ? 16 - gap : gap;
    }

    // World coordinates start at the map's top-left corner; Butano's origin is the 512x512 BG centre.
    constexpr bn::fixed to_view(bn::fixed world)
    {
        return world - 256;
    }

    // ---------------------------------------------------------------------------------------------------------
    // Map.

    struct tile_map
    {
        static constexpr int cells = 64;
        alignas(int) bn::regular_bg_map_cell map_cells[cells * cells];
        bn::regular_bg_map_item map_item;
        bool walls[map_rows][map_columns] = {};

        tile_map() :
            map_item(map_cells[0], bn::size(cells, cells))
        {
            bn::memory::clear(map_cells);
        }

        void load(const char* const* rows)
        {
            bn::memory::clear(map_cells);

            for(int ty = 0; ty < map_rows; ++ty)
            {
                for(int tx = 0; tx < map_columns; ++tx)
                {
                    bool wall = rows[ty][tx] == '#';
                    walls[ty][tx] = wall;
                    bool wall_below = ty + 1 < map_rows && rows[ty + 1][tx] == '#';
                    int tile = wall ? 3 : 1;   // flat floor in the greybox: characters must read first

                    for(int cy = 0; cy < 2; ++cy)
                    {
                        for(int cx = 0; cx < 2; ++cx)
                        {
                            // Walls facing the floor below get a lighter top edge to read depth.
                            int cell_tile = wall && ! wall_below && cy == 1 ? 4 : tile;
                            bn::regular_bg_map_cell_info info;
                            info.set_tile_index(cell_tile);
                            map_cells[map_item.cell_index(tx * 2 + cx, ty * 2 + cy)] = info.cell();
                        }
                    }
                }
            }
        }

        [[nodiscard]] bool solid(bn::fixed x, bn::fixed y) const
        {
            int tx = x.right_shift_integer() / tile_size;
            int ty = y.right_shift_integer() / tile_size;

            if(x < 0 || y < 0 || tx >= map_columns || ty >= map_rows)
            {
                return true;
            }

            return walls[ty][tx];
        }

        [[nodiscard]] bool box_free(bn::fixed x, bn::fixed y, int r) const
        {
            return ! solid(x - r, y - r) && ! solid(x + r, y - r) && ! solid(x - r, y + r) && ! solid(x + r, y + r);
        }

        // Line of sight in 4 px steps.
        [[nodiscard]] bool clear_line(bn::fixed x0, bn::fixed y0, bn::fixed x1, bn::fixed y1) const
        {
            bn::fixed dx = x1 - x0;
            bn::fixed dy = y1 - y0;
            int adx = (dx < 0 ? -dx : dx).right_shift_integer();
            int ady = (dy < 0 ? -dy : dy).right_shift_integer();
            int steps = (adx > ady ? adx : ady) / 4 + 1;

            for(int i = 1; i < steps; ++i)
            {
                bn::fixed t = bn::fixed(i) / steps;

                if(solid(x0 + dx * t, y0 + dy * t))
                {
                    return false;
                }
            }

            return true;
        }
    };

    tile_map level_map;

    // ---------------------------------------------------------------------------------------------------------
    // Actors.

    enum class weapon { none, pistol };
    enum class enemy_kind { thug, gunman };
    enum class enemy_state { patrol, reacting, attacking, stunned, dead };
    enum class game_state { playing, dead, clear };

    struct enemy
    {
        enemy_kind kind;
        enemy_state state = enemy_state::patrol;
        bn::fixed x;
        bn::fixed y;
        int dir = 0;
        int timer = 0;
        int patrol_dir = 1;
        bn::sprite_ptr sprite;
        bn::vector<bn::sprite_ptr, 1> alert;
    };

    struct bullet
    {
        bn::fixed x;
        bn::fixed y;
        bn::fixed vx;
        bn::fixed vy;
        bool from_player;
        bn::sprite_ptr sprite;
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

    class game
    {

    public:
        game_state state = game_state::playing;
        bn::camera_ptr camera = bn::camera_ptr::create(0, 0);
        bn::fixed px;
        bn::fixed py;
        int aim = 0;
        int aim_hold = 0;
        weapon held = weapon::none;
        int ammo = 0;
        int cooldown = 0;
        int score = 0;
        int combo = 0;
        int combo_timer = 0;
        int frames = 0;
        bn::optional<bn::sprite_ptr> player_sprite;
        bn::vector<enemy, 16> enemies;
        bn::vector<bullet, 24> bullets;
        bn::vector<pickup, 16> pickups;
        bn::sprite_text_generator* text = nullptr;

        void load()
        {
            enemies.clear();
            bullets.clear();
            pickups.clear();
            level_map.load(floor_1);

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
                    else if(c == 'T' || c == 'G')
                    {
                        enemy_kind kind = c == 'T' ? enemy_kind::thug : enemy_kind::gunman;
                        const bn::sprite_item& item = kind == enemy_kind::thug ?
                                    bn::sprite_items::gb_thug : bn::sprite_items::gb_gunman;
                        int dir = (enemies.size() * 5) & 15;
                        bn::sprite_ptr sprite = item.create_sprite(to_view(wx), to_view(wy), dir);
                        sprite.set_camera(camera);
                        enemies.push_back(enemy{ kind, enemy_state::patrol, wx, wy, dir, 0, 1, bn::move(sprite), {} });
                    }
                    else if(c == 'g')
                    {
                        add_pickup(wx, wy, pistol_ammo);
                    }
                }
            }

            aim = 0;
            held = weapon::none;
            ammo = 0;
            cooldown = 0;
            score = 0;
            combo = 0;
            combo_timer = 0;
            frames = 0;
            player_sprite = bn::sprite_items::gb_player.create_sprite(0, 0, 0);
            player_sprite->set_camera(camera);
            player_sprite->set_z_order(-1);
            state = game_state::playing;
            update_camera();
        }

        void update()
        {
            if(state != game_state::playing)
            {
                update_bullets();
                return;
            }

            ++frames;

            if(combo_timer > 0 && --combo_timer == 0)
            {
                combo = 0;
            }

            update_player();
            update_enemies();
            update_bullets();
            update_pickups();

            if(state == game_state::playing && alive_enemies() == 0)
            {
                state = game_state::clear;
            }

            update_camera();
        }

        [[nodiscard]] int alive_enemies() const
        {
            int alive = 0;

            for(const enemy& e : enemies)
            {
                alive += e.state != enemy_state::dead;
            }

            return alive;
        }

    private:
        void add_pickup(bn::fixed x, bn::fixed y, int pickup_ammo)
        {
            if(pickups.full())
            {
                return;
            }

            bn::sprite_ptr sprite = bn::sprite_items::gb_pickups.create_sprite(to_view(x), to_view(y), 0);
            sprite.set_camera(camera);
            sprite.set_z_order(1);
            pickups.push_back(pickup{ x, y, pickup_ammo, false, 0, 0, 0, bn::move(sprite) });
        }

        void update_camera()
        {
            bn::fixed cx = to_view(px);
            bn::fixed cy = to_view(py);
            bn::fixed min_x = -256 + 120;
            bn::fixed max_x = to_view(map_columns * tile_size) - 120;
            bn::fixed min_y = -256 + 80;
            bn::fixed max_y = to_view(map_rows * tile_size) - 80;
            cx = cx < min_x ? min_x : (cx > max_x ? max_x : cx);
            cy = cy < min_y ? min_y : (cy > max_y ? max_y : cy);
            camera.set_position(cx, cy);
        }

        void move_actor(bn::fixed& x, bn::fixed& y, bn::fixed dx, bn::fixed dy, int r)
        {
            if(level_map.box_free(x + dx, y, r))
            {
                x += dx;
            }

            if(level_map.box_free(x, y + dy, r))
            {
                y += dy;
            }
        }

        void update_player()
        {
            // Movement: 8 directions, diagonals normalised.
            bn::fixed mx = int(bn::keypad::right_held()) - int(bn::keypad::left_held());
            bn::fixed my = int(bn::keypad::down_held()) - int(bn::keypad::up_held());

            if(mx != 0 && my != 0)
            {
                mx *= 0.7071;
                my *= 0.7071;
            }

            move_actor(px, py, mx * player_speed, my * player_speed, player_radius);

            // Aim: L counter-clockwise, R clockwise; tap = one step, hold = continuous.
            int turn = int(bn::keypad::r_held()) - int(bn::keypad::l_held());

            if(bn::keypad::l_pressed() || bn::keypad::r_pressed())
            {
                aim = (aim + turn) & 15;
                aim_hold = 0;
            }
            else if(turn != 0)
            {
                ++aim_hold;

                if(aim_hold >= aim_repeat_delay && (aim_hold - aim_repeat_delay) % aim_repeat_rate == 0)
                {
                    aim = (aim + turn) & 15;
                }
            }

            player_sprite->set_position(to_view(px), to_view(py));
            player_sprite->set_tiles(bn::sprite_items::gb_player.tiles_item(), aim);

            if(cooldown > 0)
            {
                --cooldown;
            }

            if(bn::keypad::a_pressed() && cooldown == 0)
            {
                attack();
            }

            if(bn::keypad::b_pressed())
            {
                throw_or_pick();
            }
        }

        void attack()
        {
            if(held == weapon::pistol && ammo > 0)
            {
                --ammo;
                cooldown = pistol_cooldown;
                spawn_bullet(px + dir_x[aim] * 8, py + dir_y[aim] * 8, dir_x[aim] * player_bullet_speed,
                             dir_y[aim] * player_bullet_speed, true);
                make_noise(px, py);
                return;
            }

            // Punch (also with an empty pistol): front arc.
            cooldown = punch_cooldown;

            for(enemy& e : enemies)
            {
                if(e.state == enemy_state::dead)
                {
                    continue;
                }

                bn::fixed dx = e.x - px;
                bn::fixed dy = e.y - py;

                if(dx * dx + dy * dy <= punch_range * punch_range && direction_gap(direction_to(dx, dy), aim) <= 2)
                {
                    kill(e);
                    break;
                }
            }
        }

        void throw_or_pick()
        {
            if(held == weapon::pistol)
            {
                // Throw: the weapon flies forward and stuns the first enemy it touches.
                bn::sprite_ptr sprite = bn::sprite_items::gb_pickups.create_sprite(to_view(px), to_view(py), 0);
                sprite.set_camera(camera);

                if(! pickups.full())
                {
                    pickups.push_back(pickup{ px, py, ammo, true, dir_x[aim] * throw_speed, dir_y[aim] * throw_speed,
                                              throw_frames, bn::move(sprite) });
                }

                held = weapon::none;
                ammo = 0;
                return;
            }

            for(int i = 0; i < pickups.size(); ++i)
            {
                pickup& p = pickups[i];
                bn::fixed dx = p.x - px;
                bn::fixed dy = p.y - py;

                if(! p.flying && dx * dx + dy * dy <= 14 * 14)
                {
                    held = weapon::pistol;
                    ammo = p.ammo;
                    pickups.erase(pickups.begin() + i);
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

            bn::sprite_ptr sprite = bn::sprite_items::gb_bullet.create_sprite(to_view(x), to_view(y),
                                                                              from_player ? 0 : 1);
            sprite.set_camera(camera);
            sprite.set_z_order(-2);
            bullets.push_back(bullet{ x, y, vx, vy, from_player, bn::move(sprite) });
        }

        void make_noise(bn::fixed x, bn::fixed y)
        {
            for(enemy& e : enemies)
            {
                bn::fixed dx = e.x - x;
                bn::fixed dy = e.y - y;

                if(e.state == enemy_state::patrol &&
                        dx * dx + dy * dy <= gunshot_noise_radius * gunshot_noise_radius)
                {
                    start_reacting(e);
                }
            }
        }

        void start_reacting(enemy& e)
        {
            e.state = enemy_state::reacting;
            e.timer = reaction_frames;
            e.dir = direction_to(px - e.x, py - e.y);
            e.alert.clear();
            text->generate(to_view(e.x), to_view(e.y - 12), "!", e.alert);

            for(bn::sprite_ptr& s : e.alert)
            {
                s.set_camera(camera);
            }
        }

        void kill(enemy& e)
        {
            e.state = enemy_state::dead;
            e.alert.clear();
            e.sprite.set_tiles((e.kind == enemy_kind::thug ? bn::sprite_items::gb_thug : bn::sprite_items::gb_gunman)
                               .tiles_item(), dead_frame);
            e.sprite.set_z_order(2);
            ++combo;
            combo_timer = combo_window;
            score += kill_points * combo;
            bn::sound_items::blip.play();

            if(e.kind == enemy_kind::gunman)
            {
                add_pickup(e.x + 6, e.y + 4, pistol_ammo);
            }
        }

        void kill_player()
        {
            state = game_state::dead;
            test_deaths = test_deaths + 1;
            player_sprite->set_tiles(bn::sprite_items::gb_player.tiles_item(), dead_frame);
            bn::sound_items::blip.play();
        }

        [[nodiscard]] bool sees_player(const enemy& e) const
        {
            bn::fixed dx = px - e.x;
            bn::fixed dy = py - e.y;

            if(dx * dx + dy * dy > vision_range * vision_range)
            {
                return false;
            }

            return direction_gap(direction_to(dx, dy), e.dir) <= 2 && level_map.clear_line(e.x, e.y, px, py);
        }

        void update_enemies()
        {
            for(enemy& e : enemies)
            {
                switch(e.state)
                {

                case enemy_state::patrol:
                    if(e.kind == enemy_kind::thug)
                    {
                        // Walk east/west, turn at walls.
                        e.dir = e.patrol_dir > 0 ? 0 : 8;
                        bn::fixed old_x = e.x;
                        move_actor(e.x, e.y, patrol_speed * e.patrol_dir, 0, player_radius);

                        if(e.x == old_x)
                        {
                            e.patrol_dir = -e.patrol_dir;
                        }
                    }
                    else if(++e.timer >= 90)
                    {
                        // Gunmen look around in 45 degree turns.
                        e.timer = 0;
                        e.dir = (e.dir + 2 * e.patrol_dir) & 15;

                        if((e.dir & 7) == 0)
                        {
                            e.patrol_dir = -e.patrol_dir;
                        }
                    }

                    if(sees_player(e))
                    {
                        start_reacting(e);
                    }
                    break;

                case enemy_state::reacting:
                    e.dir = direction_to(px - e.x, py - e.y);

                    if(--e.timer <= 0)
                    {
                        e.alert.clear();
                        e.state = enemy_state::attacking;
                        e.timer = e.kind == enemy_kind::gunman ? 0 : thug_windup;
                    }
                    break;

                case enemy_state::attacking:
                    update_attacker(e);
                    break;

                case enemy_state::stunned:
                    if(--e.timer <= 0)
                    {
                        e.state = enemy_state::attacking;
                        e.timer = thug_windup;
                    }
                    break;

                default:
                    break;
                }

                if(e.state != enemy_state::dead)
                {
                    e.sprite.set_position(to_view(e.x), to_view(e.y));
                    e.sprite.set_tiles((e.kind == enemy_kind::thug ? bn::sprite_items::gb_thug :
                                                                     bn::sprite_items::gb_gunman).tiles_item(), e.dir);
                }

                for(bn::sprite_ptr& s : e.alert)
                {
                    s.set_position(to_view(e.x), to_view(e.y - 12));
                }

                if(state != game_state::playing)
                {
                    return;
                }
            }
        }

        void update_attacker(enemy& e)
        {
            bn::fixed dx = px - e.x;
            bn::fixed dy = py - e.y;
            e.dir = direction_to(dx, dy);

            if(e.kind == enemy_kind::thug)
            {
                bn::fixed dist2 = dx * dx + dy * dy;

                if(dist2 <= thug_reach * thug_reach)
                {
                    // Wind up, then hit if the player is still in reach.
                    if(--e.timer <= 0)
                    {
                        e.timer = thug_windup;

                        if(dist2 <= (thug_reach + 2) * (thug_reach + 2))
                        {
                            kill_player();
                        }
                    }
                }
                else
                {
                    e.timer = thug_windup;
                    move_actor(e.x, e.y, dir_x[e.dir] * thug_speed, dir_y[e.dir] * thug_speed, player_radius);
                }

                return;
            }

            // Gunman: shoot while the player is in sight, otherwise walk towards them.
            if(level_map.clear_line(e.x, e.y, px, py))
            {
                if(--e.timer <= 0)
                {
                    e.timer = gunman_fire_interval;
                    spawn_bullet(e.x + dir_x[e.dir] * 8, e.y + dir_y[e.dir] * 8, dir_x[e.dir] * enemy_bullet_speed,
                                 dir_y[e.dir] * enemy_bullet_speed, false);
                }
            }
            else
            {
                e.timer = reaction_frames;
                move_actor(e.x, e.y, dir_x[e.dir] * patrol_speed * 2, dir_y[e.dir] * patrol_speed * 2,
                           player_radius);
            }
        }

        void update_bullets()
        {
            for(int i = 0; i < bullets.size();)
            {
                bullet& b = bullets[i];
                bool gone = false;

                // Two sub-steps so fast bullets do not skip thin walls or targets.
                for(int step = 0; step < 2 && ! gone; ++step)
                {
                    b.x += b.vx / 2;
                    b.y += b.vy / 2;

                    if(level_map.solid(b.x, b.y))
                    {
                        gone = true;
                        break;
                    }

                    if(b.from_player)
                    {
                        for(enemy& e : enemies)
                        {
                            bn::fixed dx = e.x - b.x;
                            bn::fixed dy = e.y - b.y;

                            if(e.state != enemy_state::dead && dx * dx + dy * dy <= 7 * 7)
                            {
                                kill(e);
                                gone = true;
                                break;
                            }
                        }
                    }
                    else if(state == game_state::playing)
                    {
                        bn::fixed dx = px - b.x;
                        bn::fixed dy = py - b.y;

                        if(dx * dx + dy * dy <= player_radius * player_radius)
                        {
                            kill_player();
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
                    b.sprite.set_position(to_view(b.x), to_view(b.y));
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

                if(--p.timer <= 0 || level_map.solid(nx, ny))
                {
                    p.flying = false;
                }
                else
                {
                    p.x = nx;
                    p.y = ny;
                }

                for(enemy& e : enemies)
                {
                    bn::fixed dx = e.x - p.x;
                    bn::fixed dy = e.y - p.y;

                    if(p.flying && (e.state == enemy_state::patrol || e.state == enemy_state::reacting ||
                                    e.state == enemy_state::attacking) && dx * dx + dy * dy <= 9 * 9)
                    {
                        e.state = enemy_state::stunned;
                        e.timer = stun_frames;
                        e.alert.clear();
                        p.flying = false;
                    }
                }

                p.sprite.set_position(to_view(p.x), to_view(p.y));
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

    bn::bg_tiles::set_allow_offset(false);
    bn::regular_bg_item bg_item(bn::regular_bg_tiles_items::gb_tiles, bn::bg_palette_items::gb_tiles_palette,
                                level_map.map_item);
    level_map.load(dv::floor_1);
    bn::regular_bg_ptr bg = bg_item.create_bg(0, 0);
    bn::regular_bg_map_ptr bg_map = bg.map();
    bn::bg_tiles::set_allow_offset(true);

    game g;
    g.text = &small_text;
    g.load();
    bg.set_camera(g.camera);
    bg_map.reload_cells_ref();

    bn::vector<bn::sprite_ptr, 24> hud;
    bn::vector<bn::sprite_ptr, 24> message;
    int shown_score = -1;
    int shown_ammo = -1;
    int shown_held = -1;
    game_state shown_state = game_state::clear;

    while(true)
    {
        if(g.state != game_state::playing && bn::keypad::a_pressed())
        {
            g.load();
            shown_score = -1;
        }

        g.update();

        int held = int(g.held);

        if(g.score != shown_score || g.ammo != shown_ammo || held != shown_held)
        {
            shown_score = g.score;
            shown_ammo = g.ammo;
            shown_held = held;
            hud.clear();
            bn::string<24> line(g.held == weapon::pistol ? "PISTOL " : "FISTS");

            if(g.held == weapon::pistol)
            {
                line += bn::to_string<4>(g.ammo);
            }

            small_text.set_left_alignment();
            small_text.generate(-116, 72, line, hud);
            small_text.set_right_alignment();
            line = bn::to_string<8>(g.score);
            line += " PTS";
            small_text.generate(116, -72, line, hud);
            small_text.set_center_alignment();
        }

        if(g.state != shown_state)
        {
            shown_state = g.state;
            message.clear();

            if(g.state == game_state::dead)
            {
                big_text.generate(0, -12, "YOU'RE DEAD", message);
                small_text.generate(0, 8, "A: TRY AGAIN", message);
            }
            else if(g.state == game_state::clear)
            {
                big_text.generate(0, -12, "FLOOR CLEAR!", message);
                small_text.generate(0, 8, "A: PLAY AGAIN", message);
            }
        }

        test_state = int(g.state);
        test_player_x = g.px.right_shift_integer();
        test_player_y = g.py.right_shift_integer();
        test_aim = g.aim;
        test_enemies_alive = g.alive_enemies();
        test_score = g.score;
        test_ammo = g.ammo;
        bn::core::update();
    }
}
