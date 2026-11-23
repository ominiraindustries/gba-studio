// POP PANIC: shoot a harpoon upwards to split bouncing balls, on a world tour of capital cities.
// Block 1 greybox: player, harpoon, balls in 4 sizes, stages, lives, score. Graphics are gb_* placeholders.

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_fixed.h"
#include "bn_optional.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sound_items.h"
#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_vector.h"

#include "bn_regular_bg_items_gb_bg.h"
#include "bn_sprite_items_gb_ball_16.h"
#include "bn_sprite_items_gb_ball_32.h"
#include "bn_sprite_items_gb_ball_48.h"
#include "bn_sprite_items_gb_ball_8.h"
#include "bn_sprite_items_gb_harpoon.h"
#include "bn_sprite_items_gb_player.h"

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
volatile int test_harpoon_active;

namespace
{
    using namespace pop;

    enum class game_state { title, ready, playing, dying, clear, game_over, paused, win };

    const bn::sprite_item* ball_items[size_count] = {
        &bn::sprite_items::gb_ball_48, &bn::sprite_items::gb_ball_32,
        &bn::sprite_items::gb_ball_16, &bn::sprite_items::gb_ball_8,
    };

    constexpr bn::fixed to_sprite_x(bn::fixed x)
    {
        return x - screen_width / 2;
    }

    constexpr bn::fixed to_sprite_y(bn::fixed y)
    {
        return y - 80;
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
                vy = -vy;
            }

            if(x - r < 0)
            {
                x = r;
                vx = ball_speed_x;
            }
            else if(x + r > screen_width)
            {
                x = screen_width - r;
                vx = -ball_speed_x;
            }

            sprite.set_position(to_sprite_x(x), to_sprite_y(y));
        }
    };

    // Squared distance test between a circle and an axis aligned rectangle [left, right] x [top, bottom].
    [[nodiscard]] bool circle_hits_rect(bn::fixed cx, bn::fixed cy, bn::fixed r,
                                        bn::fixed left, bn::fixed top, bn::fixed right, bn::fixed bottom)
    {
        bn::fixed nx = cx < left ? left : (cx > right ? right : cx);
        bn::fixed ny = cy < top ? top : (cy > bottom ? bottom : cy);
        bn::fixed dx = cx - nx;
        bn::fixed dy = cy - ny;
        return dx * dx + dy * dy < r * r;
    }

    constexpr int harpoon_segments = 9;

    class game
    {

    public:
        game_state state = game_state::title;
        int stage = 0;
        int lives = start_lives;
        int score = 0;
        int timer = 0;

        bn::fixed player_x = screen_width / 2;
        bool facing_left = false;
        bn::sprite_ptr player_sprite = bn::sprite_items::gb_player.create_sprite(0, to_sprite_y(floor_y - 16));

        bool harpoon_active = false;
        bn::fixed harpoon_x;
        bn::fixed harpoon_top;
        bn::vector<bn::sprite_ptr, harpoon_segments + 1> harpoon_sprites;

        bn::vector<ball, 48> balls;

        game()
        {
            harpoon_sprites.push_back(bn::sprite_items::gb_harpoon.create_sprite(0, 0, 0));

            for(int i = 0; i < harpoon_segments; ++i)
            {
                harpoon_sprites.push_back(bn::sprite_items::gb_harpoon.create_sprite(0, 0, 1));
            }

            hide_harpoon();
            player_sprite.set_visible(false);
        }

        void new_game()
        {
            lives = start_lives;
            score = 0;
            stage = 0;
            test_pops = 0;
            start_stage();
        }

        void start_stage()
        {
            balls.clear();
            const stage_info& info = stages[stage];

            for(int i = 0; i < info.count; ++i)
            {
                const ball_spawn& spawn = info.balls[i];
                balls.emplace_back(spawn.size, spawn.x, spawn.y, ball_speed_x * spawn.dir, 0);
            }

            player_x = screen_width / 2;
            facing_left = false;
            harpoon_active = false;
            hide_harpoon();
            player_sprite.set_visible(true);
            place_player();
            timer = ready_frames;
            state = game_state::ready;
        }

        void update()
        {
            switch(state)
            {

            case game_state::ready:
                // Balls stay frozen during "READY" so nothing can hit the player before the stage starts.
                if(--timer <= 0)
                {
                    state = game_state::playing;
                }
                break;

            case game_state::playing:
                if(bn::keypad::start_pressed())
                {
                    state = game_state::paused;
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
                if(--timer <= 0)
                {
                    if(lives > 0)
                    {
                        start_stage();
                    }
                    else
                    {
                        state = game_state::game_over;
                        balls.clear();
                        player_sprite.set_visible(false);
                    }
                }
                break;

            case game_state::clear:
                if(--timer <= 0)
                {
                    if(stage + 1 < stage_count)
                    {
                        ++stage;
                        start_stage();
                    }
                    else
                    {
                        state = game_state::win;
                        player_sprite.set_visible(false);
                    }
                }
                break;

            default:
                break;
            }
        }

    private:
        void place_player()
        {
            player_sprite.set_position(to_sprite_x(player_x), to_sprite_y(floor_y - 16));
            player_sprite.set_horizontal_flip(facing_left);
        }

        void hide_harpoon()
        {
            for(bn::sprite_ptr& sprite : harpoon_sprites)
            {
                sprite.set_visible(false);
            }
        }

        void draw_harpoon()
        {
            bn::fixed sx = to_sprite_x(harpoon_x);
            harpoon_sprites[0].set_position(sx, to_sprite_y(harpoon_top + 8));
            harpoon_sprites[0].set_visible(true);

            for(int i = 0; i < harpoon_segments; ++i)
            {
                bn::fixed top = harpoon_top + 16 + i * 16;
                bn::sprite_ptr& sprite = harpoon_sprites[i + 1];

                if(top < floor_y)
                {
                    sprite.set_position(sx, to_sprite_y(top + 8));
                    sprite.set_visible(true);
                }
                else
                {
                    sprite.set_visible(false);
                }
            }
        }

        void update_playing()
        {
            // Player.
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

            bn::fixed half = 8;

            if(player_x < half)
            {
                player_x = half;
            }
            else if(player_x > screen_width - half)
            {
                player_x = screen_width - half;
            }

            place_player();

            // Harpoon: one at a time, rises from the player's feet to the ceiling.
            if(bn::keypad::a_pressed() && ! harpoon_active)
            {
                harpoon_active = true;
                harpoon_x = player_x;
                harpoon_top = floor_y - 16;
            }

            if(harpoon_active)
            {
                harpoon_top -= harpoon_speed;

                if(harpoon_top <= 0)
                {
                    harpoon_active = false;
                }
            }

            // Balls.
            for(ball& b : balls)
            {
                b.update();
            }

            if(harpoon_active)
            {
                for(int i = 0, n = balls.size(); i < n; ++i)
                {
                    ball& b = balls[i];

                    if(circle_hits_rect(b.x, b.y, b.radius(), harpoon_x - 2, harpoon_top, harpoon_x + 2, floor_y))
                    {
                        harpoon_active = false;
                        pop_ball(i);
                        break;
                    }
                }
            }

            if(harpoon_active)
            {
                draw_harpoon();
            }
            else
            {
                hide_harpoon();
            }

            // Player hit.
            bn::fixed left = player_x - player_hit_width / 2;
            bn::fixed right = player_x + player_hit_width / 2;
            bn::fixed top = floor_y - player_hit_height;

            for(const ball& b : balls)
            {
                if(circle_hits_rect(b.x, b.y, b.radius(), left, top, right, floor_y))
                {
                    --lives;
                    timer = dying_frames;
                    state = game_state::dying;
                    harpoon_active = false;
                    hide_harpoon();
                    return;
                }
            }

            if(balls.empty())
            {
                timer = clear_frames;
                state = game_state::clear;
                hide_harpoon();
            }
        }

        void pop_ball(int index)
        {
            ball& b = balls[index];
            int size = b.size;
            bn::fixed x = b.x;
            bn::fixed y = b.y;
            score += ball_points[size];
            test_pops = test_pops + 1;
            bn::sound_items::blip.play();
            balls.erase(balls.begin() + index);

            if(size + 1 < size_count)
            {
                balls.emplace_back(size + 1, x, y, -ball_speed_x, -ball_split_hop);
                balls.emplace_back(size + 1, x, y, ball_speed_x, -ball_split_hop);
            }
        }
    };
}

int main()
{
    bn::core::init();

    bn::regular_bg_ptr bg = bn::regular_bg_items::gb_bg.create_bg(8, 48);
    bn::sprite_text_generator small_text(common::variable_8x8_sprite_font);
    bn::sprite_text_generator big_text(common::variable_8x16_sprite_font);
    small_text.set_center_alignment();
    big_text.set_center_alignment();

    bn::vector<bn::sprite_ptr, 40> title_sprites;
    bn::vector<bn::sprite_ptr, 24> hud_sprites;
    bn::vector<bn::sprite_ptr, 24> message_sprites;

    game g;
    game_state shown_state = game_state::paused;
    int shown_score = -1;
    int shown_lives = -1;
    int shown_stage = -1;
    int blink = 0;

    while(true)
    {
        ++blink;

        if(g.state == game_state::title || g.state == game_state::game_over || g.state == game_state::win)
        {
            if(bn::keypad::start_pressed())
            {
                g.new_game();
            }
        }
        else
        {
            g.update();
        }

        bool title = g.state == game_state::title;

        if(title != (shown_state == game_state::title))
        {
            title_sprites.clear();

            if(title)
            {
                big_text.generate(0, -50, "POP PANIC", title_sprites);
                small_text.generate(0, -24, "LEFT / RIGHT: MOVE", title_sprites);
                small_text.generate(0, -12, "A: SHOOT", title_sprites);
                small_text.generate(0, 0, "START: PAUSE", title_sprites);
            }
        }

        if(title)
        {
            hud_sprites.clear();
            shown_score = -1;
        }
        else if(g.score != shown_score || g.lives != shown_lives || g.stage != shown_stage)
        {
            shown_score = g.score;
            shown_lives = g.lives;
            shown_stage = g.stage;
            hud_sprites.clear();

            bn::string<24> text("LIVES ");
            text += bn::to_string<4>(g.lives);
            small_text.set_left_alignment();
            small_text.generate(-116, 72, text, hud_sprites);

            text = "SCORE ";
            text += bn::to_string<8>(g.score);
            small_text.set_center_alignment();
            small_text.generate(0, 72, text, hud_sprites);

            text = "STAGE ";
            text += bn::to_string<4>(g.stage + 1);
            small_text.set_right_alignment();
            small_text.generate(116, 72, text, hud_sprites);
            small_text.set_center_alignment();
        }

        // Centre messages: rebuilt every frame they can blink; cheap with a handful of sprites.
        message_sprites.clear();

        switch(g.state)
        {

        case game_state::title:
            if((blink / 30) % 2 == 0)
            {
                small_text.generate(0, 30, "PRESS START", message_sprites);
            }
            break;

        case game_state::ready:
        {
            bn::string<32> text("STAGE ");
            text += bn::to_string<4>(g.stage + 1);
            text += " - ";
            text += city_names[stages[g.stage].city];
            small_text.generate(0, -30, text, message_sprites);
            big_text.generate(0, -12, g.timer > ready_frames / 2 ? "READY" : "GO!", message_sprites);
            break;
        }

        case game_state::paused:
            big_text.generate(0, -12, "PAUSE", message_sprites);
            break;

        case game_state::clear:
            big_text.generate(0, -12, "STAGE CLEAR!", message_sprites);
            break;

        case game_state::game_over:
            big_text.generate(0, -20, "GAME OVER", message_sprites);

            if((blink / 30) % 2 == 0)
            {
                small_text.generate(0, 4, "PRESS START", message_sprites);
            }
            break;

        case game_state::win:
            big_text.generate(0, -20, "WORLD TOUR COMPLETE!", message_sprites);
            small_text.generate(0, 4, "PRESS START", message_sprites);
            break;

        default:
            break;
        }

        shown_state = g.state;

        test_state = int(g.state);
        test_stage = g.stage;
        test_lives = g.lives;
        test_score = g.score;
        test_balls = g.balls.size();
        test_player_x = g.player_x.right_shift_integer();
        test_harpoon_active = g.harpoon_active;
        bn::core::update();
    }
}
