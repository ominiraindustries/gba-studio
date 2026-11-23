// POP PANIC: shoot a harpoon upwards to split bouncing balls, on a world tour of capital cities.
// Block 1 (DISENO.md section 8): stages 1-5, DOUBLE and SHIELD items, PIGEON. Graphics are gb_* placeholders.

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_fixed.h"
#include "bn_random.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sound_items.h"
#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_vector.h"

#include "bn_regular_bg_items_gb_bg.h"
#include "bn_regular_bg_items_bg_london.h"
#include "bn_regular_bg_items_bg_madrid.h"
#include "bn_regular_bg_items_bg_paris.h"
#include "bn_sprite_items_ball_16.h"
#include "bn_sprite_items_ball_24.h"
#include "bn_sprite_items_ball_32.h"
#include "bn_sprite_items_ball_8.h"
#include "bn_sprite_items_gb_shield.h"
#include "bn_sprite_items_harpoon.h"
#include "bn_sprite_items_item_double.h"
#include "bn_sprite_items_item_shield.h"
#include "bn_sprite_items_pigeon.h"
#include "bn_sprite_items_player.h"
#include "bn_sprite_items_pop_fx.h"

#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

#include "pop_tuning.h"

// Mirrors of the game state for headless tests (gbarun.py --check test_x.32...).
volatile int test_state;
volatile int test_stage;
volatile int test_lives;
volatile int test_score;
volatile int test_balls;
volatile int test_pops;
volatile int test_player_x;
volatile int test_harpoons;
volatile int test_weapon_double;
volatile int test_shield;
volatile int test_item_type;
volatile int test_pigeon_state;

namespace
{
    using namespace pop;

    enum class game_state { title, card, ready, playing, paused, dying, clear, game_over, continued };

    // Sprite and background items in one place (art/LEEME has frames and palettes; gb_* = still placeholder).
    const bn::sprite_item* ball_items[size_count] = {
        &bn::sprite_items::ball_32, &bn::sprite_items::ball_24,
        &bn::sprite_items::ball_16, &bn::sprite_items::ball_8,
    };
    const bn::sprite_item& player_item = bn::sprite_items::player;
    const bn::sprite_item& harpoon_item = bn::sprite_items::harpoon;
    const bn::sprite_item& shield_item = bn::sprite_items::gb_shield;
    const bn::sprite_item& pigeon_item = bn::sprite_items::pigeon;
    const bn::sprite_item& pop_fx_item = bn::sprite_items::pop_fx;
    const bn::sprite_item* item_items[2] = { &bn::sprite_items::item_double, &bn::sprite_items::item_shield };

    // Player frames: 0-1 idle, 2-5 walk, 6 shoot, 7 hurt, 8 death.
    constexpr int player_frame_idle = 0;
    constexpr int player_frame_walk = 2;
    constexpr int player_frame_shoot = 6;
    constexpr int player_frame_hurt = 7;
    constexpr int player_frame_death = 8;
    constexpr int pop_fx_frames = 4;
    constexpr int pop_fx_frame_time = 4;

    [[nodiscard]] const bn::regular_bg_item& city_bg(int city)
    {
        switch(city)
        {

        case 0:
            return bn::regular_bg_items::bg_madrid;

        case 1:
            return bn::regular_bg_items::bg_paris;

        case 2:
            return bn::regular_bg_items::bg_london;

        default:
            return bn::regular_bg_items::gb_bg;
        }
    }

    struct pop_effect
    {
        bn::sprite_ptr sprite;
        int age;
    };

    constexpr bn::fixed to_sprite_x(bn::fixed x)
    {
        return x - screen_width / 2;
    }

    constexpr bn::fixed to_sprite_y(bn::fixed y)
    {
        return y - 80;
    }

    [[nodiscard]] bool circle_hits_rect(bn::fixed cx, bn::fixed cy, bn::fixed r,
                                        bn::fixed left, bn::fixed top, bn::fixed right, bn::fixed bottom)
    {
        bn::fixed nx = cx < left ? left : (cx > right ? right : cx);
        bn::fixed ny = cy < top ? top : (cy > bottom ? bottom : cy);
        bn::fixed dx = cx - nx;
        bn::fixed dy = cy - ny;
        return dx * dx + dy * dy < r * r;
    }

    [[nodiscard]] bool rects_overlap(bn::fixed l1, bn::fixed t1, bn::fixed r1, bn::fixed b1,
                                     bn::fixed l2, bn::fixed t2, bn::fixed r2, bn::fixed b2)
    {
        return l1 < r2 && l2 < r1 && t1 < b2 && t2 < b1;
    }

    struct ball
    {
        int size;
        bn::fixed x;
        bn::fixed y;
        bn::fixed vx;
        bn::fixed vy;
        bn::sprite_ptr sprite;

        ball(int s, bn::fixed bx, bn::fixed by, bn::fixed bvx, bn::fixed bvy) :
            size(s), x(bx), y(by), vx(bvx), vy(bvy),
            sprite(ball_items[s]->create_sprite(to_sprite_x(bx), to_sprite_y(by)))
        {
        }

        [[nodiscard]] bn::fixed radius() const
        {
            return ball_diameter[size] / 2;
        }

        void update()
        {
            bn::fixed r = radius();
            vy += ball_gravity;
            x += vx;
            y += vy;

            if(y + r >= floor_y)
            {
                y = floor_y - r;
                vy = -ball_bounce_speed[size];
            }
            else if(y - r < 0)
            {
                y = r;
                vy = 0;
            }

            if(x - r < 0)
            {
                x = r;
                vx = -vx;
            }
            else if(x + r > screen_width)
            {
                x = screen_width - r;
                vx = -vx;
            }

            sprite.set_position(to_sprite_x(x), to_sprite_y(y));
        }
    };

    constexpr int harpoon_segments = 10;

    struct harpoon
    {
        bool active = false;
        bn::fixed x;
        bn::fixed top;
        bn::vector<bn::sprite_ptr, harpoon_segments + 1> sprites;

        void init()
        {
            sprites.push_back(harpoon_item.create_sprite(0, 0, 0));

            for(int i = 0; i < harpoon_segments; ++i)
            {
                sprites.push_back(harpoon_item.create_sprite(0, 0, 1));
            }

            hide();
        }

        void hide()
        {
            for(bn::sprite_ptr& sprite : sprites)
            {
                sprite.set_visible(false);
            }
        }

        void draw()
        {
            if(! active)
            {
                hide();
                return;
            }

            bn::fixed sx = to_sprite_x(x);
            sprites[0].set_position(sx, to_sprite_y(top + 8));
            sprites[0].set_visible(true);

            for(int i = 0; i < harpoon_segments; ++i)
            {
                bn::fixed segment_top = top + 16 + i * 16;
                bn::sprite_ptr& sprite = sprites[i + 1];

                if(segment_top < floor_y)
                {
                    sprite.set_position(sx, to_sprite_y(segment_top + 8));
                    sprite.set_visible(true);
                }
                else
                {
                    sprite.set_visible(false);
                }
            }
        }
    };

    enum class pigeon_state { none, warning, flying };

    class game
    {

    public:
        game_state state = game_state::title;
        int stage = 0;
        int lives = start_lives;
        int score = 0;
        int timer = 0;
        int hitstop = 0;
        bool first_shot_done = false;
        int bonus_shown = 0;

        bn::fixed player_x = player_start_x;
        bool facing_left = false;
        int invulnerable = 0;
        bool weapon_double = false;
        bool shield = false;
        bn::sprite_ptr player_sprite = player_item.create_sprite(0, 0);
        bn::sprite_ptr shield_sprite = shield_item.create_sprite(0, 0);

        harpoon harpoons[2];
        bn::vector<ball, max_balls> balls;
        int deadly_ball = -1;

        bool stage_first_pop = true;
        int item = item_none;
        bn::fixed item_x;
        bn::fixed item_y;
        int item_timer = 0;
        bn::sprite_ptr item_sprite = item_items[0]->create_sprite(0, 0);

        pigeon_state pigeon = pigeon_state::none;
        bn::fixed pigeon_x;
        bn::fixed pigeon_y;
        int pigeon_dir = 1;
        int pigeon_timer = 0;
        bn::sprite_ptr pigeon_sprite = pigeon_item.create_sprite(0, 0);
        int stage_frames = 0;

        // Floating "+40" over the last pop.
        int popup_points = 0;
        bn::fixed popup_x;
        bn::fixed popup_y;
        int popup_timer = 0;

        bn::random random;

        game()
        {
            for(harpoon& h : harpoons)
            {
                h.init();
            }

            player_sprite.set_visible(false);
            shield_sprite.set_visible(false);
            item_sprite.set_visible(false);
            pigeon_sprite.set_visible(false);
        }

        void new_game()
        {
            lives = start_lives;
            score = 0;
            stage = 0;
            first_shot_done = false;
            weapon_double = false;
            shield = false;
            test_pops = 0;
            start_card(card_frames);
        }

        void retry()
        {
            // Game over retry: same stage, full lives, score back to 0 (DISENO.md section 4.6).
            lives = start_lives;
            score = 0;
            weapon_double = false;
            shield = false;
            start_card(card_frames);
        }

        void update()
        {
            ++random_ticks;

            switch(state)
            {

            case game_state::card:
                ++card_elapsed;

                // The START that begins the game must not skip the card: allow skipping after a moment.
                if(--timer <= 0 || (card_elapsed > 15 && (bn::keypad::a_pressed() || bn::keypad::start_pressed())))
                {
                    start_stage(0);
                }
                break;

            case game_state::ready:
                // Balls stay frozen; the player may move but not shoot.
                update_player();

                if(--timer <= 0)
                {
                    state = game_state::playing;
                    stage_frames = 0;
                }
                break;

            case game_state::playing:
                if(bn::keypad::start_pressed())
                {
                    state = game_state::paused;
                    break;
                }

                if(hitstop > 0)
                {
                    --hitstop;
                    break;
                }

                update_playing();
                break;

            case game_state::paused:
                if(bn::keypad::start_pressed())
                {
                    state = game_state::playing;
                }
                break;

            case game_state::dying:
                update_dying();
                break;

            case game_state::clear:
                if(--timer <= 0)
                {
                    if(stage + 1 >= last_stage_of_block)
                    {
                        clear_stage_objects();
                        player_sprite.set_visible(false);
                        state = game_state::continued;
                    }
                    else
                    {
                        bool same_city = stages[stage + 1].city == stages[stage].city;
                        ++stage;
                        start_card(same_city ? card_frames_same_city : card_frames);
                    }
                }
                break;

            case game_state::game_over:
                if(timer > 0)
                {
                    --timer;
                }
                else if(bn::keypad::a_pressed())
                {
                    retry();
                }
                else if(bn::keypad::start_pressed())
                {
                    state = game_state::title;
                }
                break;

            case game_state::continued:
                if(bn::keypad::a_pressed() || bn::keypad::start_pressed())
                {
                    state = game_state::title;
                }
                break;

            default:
                break;
            }

            if(popup_timer > 0)
            {
                --popup_timer;
                popup_y -= 0.5;
            }

            if(state == game_state::playing && hitstop == 0)
            {
                update_effects();
            }

            draw();
        }

        [[nodiscard]] int active_harpoons() const
        {
            return int(harpoons[0].active) + int(harpoons[1].active);
        }

    private:
        unsigned random_ticks = 0;
        int card_elapsed = 0;
        int shoot_timer = 0;
        int walk_timer = 0;
        bool walking = false;
        bn::vector<pop_effect, 6> effects;

        void clear_stage_objects()
        {
            balls.clear();

            for(harpoon& h : harpoons)
            {
                h.active = false;
                h.hide();
            }

            item = item_none;
            pigeon = pigeon_state::none;
            popup_timer = 0;
            deadly_ball = -1;
            effects.clear();
        }

        void start_card(int frames)
        {
            clear_stage_objects();
            player_sprite.set_visible(false);
            timer = frames;
            card_elapsed = 0;
            state = game_state::card;
        }

        void start_stage(int invulnerable_frames)
        {
            clear_stage_objects();
            const stage_info& info = stages[stage];

            for(int i = 0; i < info.count; ++i)
            {
                const ball_spawn& spawn = info.balls[i];
                bn::fixed vx = ball_speed_x[spawn.size] * info.speed_factor * spawn.dir;
                balls.emplace_back(spawn.size, spawn.x, ball_apex_y[spawn.size], vx, 0);
            }

            player_x = info.player_x;
            facing_left = false;
            invulnerable = invulnerable_frames;
            stage_first_pop = true;
            pigeon_timer = pigeon_first_delay;
            hitstop = 0;
            timer = ready_frames + go_frames;
            state = game_state::ready;
        }

        void update_player()
        {
            walking = bn::keypad::left_held() || bn::keypad::right_held();

            if(walking)
            {
                ++walk_timer;
            }

            if(shoot_timer > 0)
            {
                --shoot_timer;
            }

            if(bn::keypad::left_held())
            {
                player_x -= player_speed;
                facing_left = true;
            }
            else if(bn::keypad::right_held())
            {
                player_x += player_speed;
                facing_left = false;
            }

            bn::fixed half = 6;

            if(player_x < half)
            {
                player_x = half;
            }
            else if(player_x > screen_width - half)
            {
                player_x = screen_width - half;
            }
        }

        void update_playing()
        {
            ++stage_frames;
            update_player();

            if(invulnerable > 0)
            {
                --invulnerable;
            }

            // Fire: one harpoon, two with DOUBLE.
            int max_harpoons = weapon_double ? 2 : 1;

            if(bn::keypad::a_pressed() && active_harpoons() < max_harpoons)
            {
                for(harpoon& h : harpoons)
                {
                    if(! h.active)
                    {
                        h.active = true;
                        h.x = player_x;
                        h.top = harpoon_start_y;
                        first_shot_done = true;
                        shoot_timer = 8;
                        break;
                    }
                }
            }

            for(harpoon& h : harpoons)
            {
                if(h.active)
                {
                    h.top -= harpoon_speed;

                    if(h.top <= 0)
                    {
                        h.active = false;
                    }
                }
            }

            for(ball& b : balls)
            {
                b.update();
            }

            // Harpoon against balls: the whole rope hits.
            for(harpoon& h : harpoons)
            {
                if(! h.active)
                {
                    continue;
                }

                for(int i = 0, n = balls.size(); i < n; ++i)
                {
                    ball& b = balls[i];

                    if(circle_hits_rect(b.x, b.y, b.radius() + 1, h.x - harpoon_half_width, h.top,
                                        h.x + harpoon_half_width, floor_y))
                    {
                        h.active = false;
                        pop_ball(i);
                        break;
                    }
                }
            }

            update_item();
            update_pigeon();

            // Balls against the player.
            if(invulnerable == 0)
            {
                bn::fixed left = player_x - player_hit_width / 2;
                bn::fixed right = player_x + player_hit_width / 2;
                bn::fixed top = floor_y - player_hit_height;

                for(int i = 0, n = balls.size(); i < n; ++i)
                {
                    const ball& b = balls[i];

                    if(circle_hits_rect(b.x, b.y, b.radius() - ball_hurt_shrink[b.size], left, top, right, floor_y))
                    {
                        if(shield)
                        {
                            shield = false;
                            invulnerable = respawn_invulnerable_frames;
                        }
                        else
                        {
                            start_dying(i);
                            return;
                        }

                        break;
                    }
                }
            }

            if(balls.empty())
            {
                score += stage_bonus * (stage + 1);
                bonus_shown = stage_bonus * (stage + 1);
                timer = clear_frames;
                state = game_state::clear;

                for(harpoon& h : harpoons)
                {
                    h.active = false;
                }

                item = item_none;
                pigeon = pigeon_state::none;
            }
        }

        void start_dying(int ball_index)
        {
            --lives;
            weapon_double = false;
            deadly_ball = ball_index;
            timer = death_hitstop_frames + death_fall_frames;
            state = game_state::dying;

            for(harpoon& h : harpoons)
            {
                h.active = false;
            }

            bn::sound_items::blip.play();
        }

        void update_dying()
        {
            --timer;

            if(timer <= 0)
            {
                if(lives > 0)
                {
                    start_stage(respawn_invulnerable_frames);
                }
                else
                {
                    clear_stage_objects();
                    player_sprite.set_visible(false);
                    timer = game_over_frames;
                    state = game_state::game_over;
                }
            }
        }

        void pop_ball(int index)
        {
            ball& b = balls[index];
            int size = b.size;
            bn::fixed x = b.x;
            bn::fixed y = b.y;
            bn::fixed speed = ball_speed_x[size + 1 < size_count ? size + 1 : size] * stages[stage].speed_factor;
            score += ball_points[size];
            test_pops = test_pops + 1;
            hitstop = pop_hitstop[size];
            popup_points = ball_points[size];
            popup_x = x;
            popup_y = y;
            popup_timer = 30;
            bn::sound_items::blip.play();
            add_effect(x, y);
            balls.erase(balls.begin() + index);

            if(size + 1 < size_count)
            {
                balls.emplace_back(size + 1, x, y, -speed, -ball_split_hop);
                balls.emplace_back(size + 1, x, y, speed, -ball_split_hop);
                maybe_drop_item(x, y);
            }

            stage_first_pop = false;
        }

        void add_effect(bn::fixed x, bn::fixed y)
        {
            if(effects.full())
            {
                effects.erase(effects.begin());
            }

            effects.push_back(pop_effect{ pop_fx_item.create_sprite(to_sprite_x(x), to_sprite_y(y)), 0 });
        }

        void update_effects()
        {
            for(int i = 0; i < effects.size();)
            {
                pop_effect& e = effects[i];
                ++e.age;

                if(e.age >= pop_fx_frames * pop_fx_frame_time)
                {
                    effects.erase(effects.begin() + i);
                }
                else
                {
                    e.sprite.set_tiles(pop_fx_item.tiles_item(), e.age / pop_fx_frame_time);
                    ++i;
                }
            }
        }

        void maybe_drop_item(bn::fixed x, bn::fixed y)
        {
            if(item != item_none)
            {
                return;
            }

            int type = item_none;

            if(stage_first_pop && stages[stage].intro_item != item_none)
            {
                type = stages[stage].intro_item;
            }
            else if(stage >= 1 && random.get_int(item_drop_one_in) == 0)
            {
                int roll = random.get_int(item_double_weight + item_shield_weight);
                type = roll < item_double_weight ? item_double : item_shield;
            }

            if(type == item_none)
            {
                return;
            }

            item = type;
            item_x = x;
            item_y = y;
            item_timer = item_floor_frames;
            item_sprite = item_items[type]->create_sprite(0, 0);
        }

        void update_item()
        {
            if(item == item_none)
            {
                return;
            }

            if(item_y + 8 < floor_y)
            {
                item_y += item_fall_speed;

                if(item_y + 8 > floor_y)
                {
                    item_y = floor_y - 8;
                }
            }
            else if(--item_timer <= 0)
            {
                item = item_none;
                return;
            }

            bn::fixed reach = player_hit_width / 2 + item_pick_margin;

            if(rects_overlap(player_x - reach, floor_y - player_hit_height - item_pick_margin, player_x + reach, floor_y,
                             item_x - 6, item_y - 6, item_x + 6, item_y + 6))
            {
                if(item == item_double)
                {
                    weapon_double = true;
                }
                else
                {
                    shield = true;
                }

                score += 100;
                item = item_none;
                bn::sound_items::blip.play();
            }
        }

        void update_pigeon()
        {
            if(! stages[stage].pigeons)
            {
                return;
            }

            switch(pigeon)
            {

            case pigeon_state::none:
                if(--pigeon_timer <= 0 && ! balls.empty())
                {
                    pigeon_dir = random.get_bool() ? 1 : -1;
                    pigeon_x = pigeon_dir > 0 ? -8 : screen_width + 8;
                    pigeon_y = 24 + random.get_int(17);
                    pigeon_timer = pigeon_warning_frames;
                    pigeon = pigeon_state::warning;
                }
                break;

            case pigeon_state::warning:
                if(--pigeon_timer <= 0)
                {
                    pigeon = pigeon_state::flying;
                }
                break;

            case pigeon_state::flying:
            {
                pigeon_x += pigeon_speed * pigeon_dir;
                bn::fixed left = pigeon_x - 6;
                bn::fixed right = pigeon_x + 6;
                bn::fixed top = pigeon_y - 5;
                bn::fixed bottom = pigeon_y + 5;

                for(harpoon& h : harpoons)
                {
                    if(! h.active || ! rects_overlap(left, top, right, bottom, h.x - harpoon_half_width, h.top,
                                                     h.x + harpoon_half_width, floor_y))
                    {
                        continue;
                    }

                    h.active = false;

                    if(h.top >= top - 8)
                    {
                        // The tip reached the pigeon: shot down.
                        score += pigeon_points;
                        popup_points = pigeon_points;
                        popup_x = pigeon_x;
                        popup_y = pigeon_y;
                        popup_timer = 30;
                        bn::sound_items::blip.play();
                        pigeon = pigeon_state::none;
                        pigeon_timer = pigeon_repeat_delay;
                        return;
                    }

                    // Otherwise the pigeon flew into the rope and cut it.
                }

                if(pigeon_x < -16 || pigeon_x > screen_width + 16)
                {
                    pigeon = pigeon_state::none;
                    pigeon_timer = pigeon_repeat_delay;
                }
                break;
            }

            default:
                break;
            }
        }

        void draw()
        {
            bool in_stage = state == game_state::ready || state == game_state::playing || state == game_state::paused ||
                    state == game_state::dying || state == game_state::clear;

            // Player: blinks while invulnerable, falls through the floor at the end of a death.
            bn::fixed player_y = floor_y - 8;

            if(state == game_state::dying && timer < death_fall_frames)
            {
                player_y += (death_fall_frames - timer) / 3;
            }

            player_sprite.set_visible(in_stage && (invulnerable == 0 || (invulnerable / 4) % 2 == 0));
            player_sprite.set_position(to_sprite_x(player_x), to_sprite_y(player_y));
            player_sprite.set_horizontal_flip(facing_left);

            int frame = player_frame_idle + (stage_frames / 30) % 2;

            if(state == game_state::dying)
            {
                frame = timer < death_fall_frames ? player_frame_death : player_frame_hurt;
            }
            else if(shoot_timer > 0)
            {
                frame = player_frame_shoot;
            }
            else if(walking && (state == game_state::playing || state == game_state::ready))
            {
                frame = player_frame_walk + (walk_timer / 6) % 4;
            }

            player_sprite.set_tiles(player_item.tiles_item(), frame);

            shield_sprite.set_visible(in_stage && shield && (stage_frames / 8) % 4 != 0);
            shield_sprite.set_position(to_sprite_x(player_x), to_sprite_y(floor_y - 8));

            for(harpoon& h : harpoons)
            {
                h.draw();
            }

            // The ball that killed the player blinks during the death hitstop.
            for(int i = 0, n = balls.size(); i < n; ++i)
            {
                bool blink_off = state == game_state::dying && i == deadly_ball && (timer / 4) % 2 == 0;
                balls[i].sprite.set_visible(! blink_off);
            }

            if(item != item_none)
            {
                bool blink_off = item_timer < item_blink_frames && (item_timer / 4) % 2 == 0;
                item_sprite.set_visible(! blink_off);
                item_sprite.set_position(to_sprite_x(item_x), to_sprite_y(item_y));
            }
            else
            {
                item_sprite.set_visible(false);
            }

            pigeon_sprite.set_visible(pigeon == pigeon_state::flying);

            if(pigeon == pigeon_state::flying)
            {
                pigeon_sprite.set_position(to_sprite_x(pigeon_x), to_sprite_y(pigeon_y));
                pigeon_sprite.set_horizontal_flip(pigeon_dir < 0);
                pigeon_sprite.set_tiles(pigeon_item.tiles_item(), (stage_frames / 6) % 3);
            }
        }
    };
}

int main()
{
    bn::core::init();

    bn::regular_bg_ptr bg = city_bg(0).create_bg(8, 48);
    int shown_city = 0;
    bn::sprite_text_generator small_text(common::variable_8x8_sprite_font);
    bn::sprite_text_generator big_text(common::variable_8x16_sprite_font);
    small_text.set_center_alignment();
    big_text.set_center_alignment();

    bn::vector<bn::sprite_ptr, 32> hud_sprites;
    bn::vector<bn::sprite_ptr, 40> message_sprites;

    game g;
    int shown_score = -1;
    int shown_lives = -1;
    int shown_stage = -1;
    int shown_flags = -1;
    bool shown_hud = false;
    int blink = 0;

    while(true)
    {
        ++blink;

        if(g.state == game_state::title)
        {
            if(bn::keypad::start_pressed() || bn::keypad::a_pressed())
            {
                g.new_game();
            }
        }

        g.update();

        int city = g.state == game_state::title ? 0 : stages[g.stage].city;

        if(city != shown_city)
        {
            shown_city = city;
            bg.set_item(city_bg(city));
        }

        // HUD on the thin floor: lives, item flags, stage, score.
        bool hud = g.state != game_state::title && g.state != game_state::continued;
        int flags = int(g.weapon_double) + int(g.shield) * 2;

        if(hud != shown_hud || (hud && (g.score != shown_score || g.lives != shown_lives || g.stage != shown_stage ||
                                        flags != shown_flags)))
        {
            shown_hud = hud;
            shown_score = g.score;
            shown_lives = g.lives;
            shown_stage = g.stage;
            shown_flags = flags;
            hud_sprites.clear();

            if(hud)
            {
                bn::string<24> text("LIVES ");
                text += bn::to_string<4>(g.lives);

                if(g.weapon_double)
                {
                    text += "  D";
                }

                if(g.shield)
                {
                    text += "  S";
                }

                small_text.set_left_alignment();
                small_text.generate(-116, 76, text, hud_sprites);

                text = "ST ";
                text += bn::to_string<4>(g.stage + 1);
                small_text.set_center_alignment();
                small_text.generate(16, 76, text, hud_sprites);

                bn::string<8> digits = bn::to_string<8>(g.score);
                text.clear();

                for(int i = digits.size(); i < 6; ++i)
                {
                    text += '0';
                }

                text += digits;
                small_text.set_right_alignment();
                small_text.generate(116, 76, text, hud_sprites);
                small_text.set_center_alignment();
            }
        }

        // Centre messages, rebuilt each frame (a handful of sprites).
        message_sprites.clear();

        switch(g.state)
        {

        case game_state::title:
            big_text.generate(0, -50, "POP PANIC", message_sprites);
            small_text.generate(0, -30, "A WORLD TOUR OF POPPING", message_sprites);

            if((blink / 30) % 2 == 0)
            {
                small_text.generate(0, 20, "PRESS START", message_sprites);
            }
            break;

        case game_state::card:
        {
            bn::string<32> text("STAGE ");
            text += bn::to_string<4>(g.stage + 1);
            text += " - ";
            text += city_names[stages[g.stage].city];
            big_text.generate(0, -12, text, message_sprites);
            break;
        }

        case game_state::ready:
            big_text.generate(0, -20, g.timer > go_frames ? "READY" : "GO!", message_sprites);

            if(g.stage == 0 && ! g.first_shot_done)
            {
                small_text.generate(0, 0, "A: SHOOT", message_sprites);
            }
            break;

        case game_state::playing:
            if(g.stage == 0 && ! g.first_shot_done)
            {
                small_text.generate(0, 0, "A: SHOOT", message_sprites);
            }

            if(g.pigeon == pigeon_state::warning && (blink / 4) % 2 == 0)
            {
                small_text.generate(g.pigeon_dir > 0 ? -114 : 114, to_sprite_y(g.pigeon_y), "!", message_sprites);
            }
            break;

        case game_state::paused:
            big_text.generate(0, -20, "PAUSE", message_sprites);
            break;

        case game_state::clear:
        {
            big_text.generate(0, -20, "CLEAR!", message_sprites);
            bn::string<24> text("BONUS ");
            text += bn::to_string<8>(g.bonus_shown);
            small_text.generate(0, 0, text, message_sprites);
            break;
        }

        case game_state::game_over:
            big_text.generate(0, -20, "GAME OVER", message_sprites);

            if(g.timer == 0 && (blink / 30) % 2 == 0)
            {
                small_text.generate(0, 4, "PRESS A TO RETRY", message_sprites);
            }
            break;

        case game_state::continued:
        {
            big_text.generate(0, -30, "TO BE CONTINUED", message_sprites);
            bn::string<24> text("SCORE ");
            text += bn::to_string<8>(g.score);
            small_text.generate(0, -4, text, message_sprites);
            small_text.generate(0, 20, "PRESS A", message_sprites);
            break;
        }

        default:
            break;
        }

        if(g.popup_timer > 0 && g.state == game_state::playing)
        {
            bn::string<8> text("+");
            text += bn::to_string<6>(g.popup_points);
            small_text.generate(to_sprite_x(g.popup_x), to_sprite_y(g.popup_y), text, message_sprites);
        }

        test_state = int(g.state);
        test_stage = g.stage;
        test_lives = g.lives;
        test_score = g.score;
        test_balls = g.balls.size();
        test_player_x = g.player_x.right_shift_integer();
        test_harpoons = g.active_harpoons();
        test_weapon_double = g.weapon_double;
        test_shield = g.shield;
        test_item_type = g.item;
        test_pigeon_state = int(g.pigeon);
        bn::core::update();
    }
}
