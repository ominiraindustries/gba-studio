// 001_apila: falling-block puzzle with modern rules (7-bag, SRS kicks, hold, ghost, hard drop, lock delay,
// T-spins, combos, back-to-back). Greybox: the board is a dynamic regular BG, text uses Butano's generator.

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_memory.h"
#include "bn_bg_tiles.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_seed_random.h"
#include "bn_sound_items.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_vector.h"

#include "bn_bg_palette_items_cells_palette.h"
#include "bn_regular_bg_tiles_items_cells.h"

#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"

#include "apila_rules.h"

// Mirrors of the game state for headless tests (gbarun.py --check test_x.32...).
volatile int test_state;
volatile int test_score;
volatile int test_lines;
volatile int test_level;
volatile int test_pieces;
volatile int test_holds;
volatile int test_tspins;
volatile int test_piece_type;
volatile int test_piece_x;
volatile int test_piece_y;
volatile int test_piece_rot;
volatile int test_selftest;   // bitmask of rule self-tests passed at boot (15 = all)

namespace
{
    using namespace apila;

    // Tile indices in graphics/cells.bmp.
    constexpr int tile_empty = 0;
    constexpr int tile_well = 1;
    constexpr int tile_block = 2;
    constexpr int tile_ghost = 3;
    constexpr int tile_wall = 4;
    constexpr int tile_flash = 5;

    // Screen layout in 8x8 map cells (30x20 visible).
    constexpr int well_column = 10;
    constexpr int hold_box_column = 1;
    constexpr int hold_box_row = 2;
    constexpr int next_box_column = 22;
    constexpr int next_box_row = 2;
    constexpr int box_width = 7;

    enum class game_state { title, playing, clearing, paused, game_over };

    struct bg_map
    {
        static constexpr int columns = 32;
        static constexpr int rows = 32;

        alignas(int) bn::regular_bg_map_cell cells[columns * rows];
        bn::regular_bg_map_item map_item;

        bg_map() :
            map_item(cells[0], bn::size(columns, rows))
        {
            bn::memory::clear(cells);
        }

        void set(int x, int y, int tile, int palette)
        {
            bn::regular_bg_map_cell_info info;
            info.set_tile_index(tile);
            info.set_palette_id(palette);
            cells[map_item.cell_index(x, y)] = info.cell();
        }

        void clear()
        {
            bn::memory::clear(cells);
        }
    };

    bg_map map_data;

    point piece_cells[piece_count][4][4];

    void build_rotations()
    {
        for(int type = 0; type < piece_count; ++type)
        {
            int n = box_size[type];

            for(int i = 0; i < 4; ++i)
            {
                piece_cells[type][0][i] = spawn_cells[type][i];
            }

            for(int rot = 1; rot < 4; ++rot)
            {
                for(int i = 0; i < 4; ++i)
                {
                    point p = piece_cells[type][rot - 1][i];

                    if(type == piece_o)
                    {
                        piece_cells[type][rot][i] = p;
                    }
                    else
                    {
                        piece_cells[type][rot][i] = point{ n - 1 - p.y, p.x };
                    }
                }
            }
        }
    }

    struct active_piece
    {
        int type = 0;
        int rot = 0;
        int x = 0;
        int y = 0;
    };

    class game
    {

    public:
        game_state state = game_state::title;
        int board[board_rows][board_columns];
        active_piece piece;
        int hold_type = -1;
        bool can_hold = true;
        int queue[next_count + piece_count];
        int queue_size = 0;
        int bag[piece_count];
        int bag_left = 0;
        bn::seed_random random;

        int gravity_accum = 0;
        int lock_timer = 0;
        int lock_resets = 0;
        int lowest_y = 0;
        bool last_move_rotation = false;
        int last_kick = 0;

        int das_dir = 0;
        int das_timer = 0;

        int score = 0;
        int lines = 0;
        int level = 1;
        int combo = -1;
        bool back_to_back = false;

        bool clearing_rows[board_rows];
        int clear_timer = 0;

        // Text shown under the stats after a clear ("DOBLE", "T-GIRO", ...).
        const char* popup_1 = nullptr;
        const char* popup_2 = nullptr;
        int popup_combo = 0;
        int popup_timer = 0;

        void start(unsigned seed)
        {
            bn::memory::clear(board);
            random.set_seed(seed);
            queue_size = 0;
            bag_left = 0;
            hold_type = -1;
            can_hold = true;
            score = 0;
            lines = 0;
            level = 1;
            combo = -1;
            back_to_back = false;
            popup_timer = 0;
            das_dir = 0;
            das_timer = 0;
            test_pieces = 0;
            test_holds = 0;
            test_tspins = 0;

            for(bool& row : clearing_rows)
            {
                row = false;
            }

            fill_queue();
            state = game_state::playing;
            spawn(pop_queue());
        }

        [[nodiscard]] bool fits(int type, int rot, int x, int y) const
        {
            for(const point& cell : piece_cells[type][rot])
            {
                int cx = x + cell.x;
                int cy = y + cell.y;

                if(cx < 0 || cx >= board_columns || cy < 0 || cy >= board_rows || board[cy][cx])
                {
                    return false;
                }
            }

            return true;
        }

        [[nodiscard]] bool grounded() const
        {
            return ! fits(piece.type, piece.rot, piece.x, piece.y + 1);
        }

        [[nodiscard]] int ghost_y() const
        {
            int y = piece.y;

            while(fits(piece.type, piece.rot, piece.x, y + 1))
            {
                ++y;
            }

            return y;
        }

        void update()
        {
            switch(state)
            {

            case game_state::playing:
                if(bn::keypad::start_pressed())
                {
                    state = game_state::paused;
                    return;
                }

                update_playing();
                break;

            case game_state::clearing:
                update_das_only();

                if(--clear_timer <= 0)
                {
                    collapse_rows();
                    spawn(pop_queue());
                }
                break;

            case game_state::paused:
                if(bn::keypad::start_pressed())
                {
                    state = game_state::playing;
                }
                break;

            default:
                break;
            }

            if(popup_timer > 0)
            {
                --popup_timer;
            }
        }

        bool quiet = false;

        friend int run_selftest();

    private:
        void fill_queue()
        {
            while(queue_size < next_count + 1)
            {
                if(bag_left == 0)
                {
                    for(int i = 0; i < piece_count; ++i)
                    {
                        bag[i] = i;
                    }

                    for(int i = piece_count - 1; i > 0; --i)
                    {
                        int j = random.get_unbiased_int(i + 1);
                        int t = bag[i];
                        bag[i] = bag[j];
                        bag[j] = t;
                    }

                    bag_left = piece_count;
                }

                queue[queue_size] = bag[piece_count - bag_left];
                ++queue_size;
                --bag_left;
            }
        }

        int pop_queue()
        {
            int type = queue[0];

            for(int i = 1; i < queue_size; ++i)
            {
                queue[i - 1] = queue[i];
            }

            --queue_size;
            fill_queue();
            return type;
        }

        void spawn(int type)
        {
            piece.type = type;
            piece.rot = 0;
            piece.x = 3;
            piece.y = 3;   // after the spawn drop the piece is fully visible (design review: rule 5)
            gravity_accum = 0;
            lock_timer = 0;
            lock_resets = 0;
            last_move_rotation = false;
            last_kick = 0;
            test_pieces = test_pieces + 1;

            if(! fits(type, 0, piece.x, piece.y))
            {
                state = game_state::game_over;
                return;
            }

            // Pieces appear just above the visible field and drop one row at once if they can.
            if(fits(type, 0, piece.x, piece.y + 1))
            {
                ++piece.y;
            }

            lowest_y = piece.y;
            state = game_state::playing;
        }

        void on_moved()
        {
            // Move reset: moving or rotating on the ground restarts the lock delay, up to max_lock_resets times.
            if(lock_timer > 0 || grounded())
            {
                if(lock_resets < max_lock_resets)
                {
                    ++lock_resets;
                    lock_timer = 0;
                }
            }
        }

        bool try_shift(int dx)
        {
            if(fits(piece.type, piece.rot, piece.x + dx, piece.y))
            {
                piece.x += dx;
                last_move_rotation = false;
                on_moved();
                return true;
            }

            return false;
        }

        bool try_rotate(int dir)
        {
            if(piece.type == piece_o)
            {
                return false;
            }

            int from = piece.rot;
            int to = (from + (dir == 0 ? 1 : 3)) & 3;
            const point* kicks = piece.type == piece_i ? kicks_i[from][dir] : kicks_jlstz[from][dir];

            for(int test = 0; test < 5; ++test)
            {
                int nx = piece.x + kicks[test].x;
                int ny = piece.y - kicks[test].y;

                if(fits(piece.type, to, nx, ny))
                {
                    piece.x = nx;
                    piece.y = ny;
                    piece.rot = to;
                    last_move_rotation = true;
                    last_kick = test;

                    if(piece.y > lowest_y)
                    {
                        lowest_y = piece.y;
                        lock_resets = 0;
                    }

                    on_moved();
                    return true;
                }
            }

            return false;
        }

        void update_das(bool move_allowed)
        {
            if(bn::keypad::left_pressed())
            {
                das_dir = -1;
                das_timer = 0;

                if(move_allowed)
                {
                    try_shift(-1);
                }
                return;
            }

            if(bn::keypad::right_pressed())
            {
                das_dir = 1;
                das_timer = 0;

                if(move_allowed)
                {
                    try_shift(1);
                }
                return;
            }

            bool held = das_dir < 0 ? bn::keypad::left_held() : (das_dir > 0 ? bn::keypad::right_held() : false);

            if(! held)
            {
                das_dir = bn::keypad::left_held() ? -1 : (bn::keypad::right_held() ? 1 : 0);
                das_timer = 0;
                return;
            }

            ++das_timer;

            if(move_allowed && das_timer >= das_frames && (das_timer - das_frames) % arr_frames == 0)
            {
                try_shift(das_dir);
            }
        }

        void update_das_only()
        {
            // Keep the auto-shift charge while lines clear, so a held direction is ready for the next piece.
            update_das(false);

            if(das_timer > das_frames)
            {
                das_timer = das_frames;
            }
        }

        void update_playing()
        {
            if((bn::keypad::l_pressed() || bn::keypad::r_pressed()) && can_hold)
            {
                int current = piece.type;
                can_hold = false;
                test_holds = test_holds + 1;

                if(hold_type < 0)
                {
                    hold_type = current;
                    spawn(pop_queue());
                }
                else
                {
                    int next = hold_type;
                    hold_type = current;
                    spawn(next);
                }

                if(state != game_state::playing)
                {
                    return;
                }
            }

            if(bn::keypad::a_pressed())
            {
                try_rotate(0);
            }
            else if(bn::keypad::b_pressed())
            {
                try_rotate(1);
            }

            update_das(true);

            if(bn::keypad::up_pressed())
            {
                int target = ghost_y();
                score += (target - piece.y) * 2;

                if(target != piece.y)
                {
                    last_move_rotation = false;
                }

                piece.y = target;
                lock_piece();
                return;
            }

            bool soft = bn::keypad::down_held();
            int gravity = gravity_table[level - 1];

            if(soft)
            {
                gravity *= soft_drop_factor;
            }

            gravity_accum += gravity;

            while(gravity_accum >= 65536)
            {
                gravity_accum -= 65536;

                if(fits(piece.type, piece.rot, piece.x, piece.y + 1))
                {
                    ++piece.y;
                    lock_timer = 0;
                    last_move_rotation = false;

                    if(soft)
                    {
                        ++score;
                    }

                    if(piece.y > lowest_y)
                    {
                        lowest_y = piece.y;
                        lock_resets = 0;
                    }
                }
                else
                {
                    gravity_accum = 0;
                    break;
                }
            }

            if(grounded())
            {
                ++lock_timer;

                if(lock_timer >= lock_delay_frames || lock_resets >= max_lock_resets)
                {
                    lock_piece();
                }
            }
            else
            {
                lock_timer = 0;
            }
        }

        [[nodiscard]] bool occupied(int x, int y) const
        {
            return x < 0 || x >= board_columns || y < 0 || y >= board_rows || board[y][x];
        }

        // 0 = none, 1 = mini, 2 = full T-spin (3-corner rule; the last kick test always counts as full).
        [[nodiscard]] int tspin_kind() const
        {
            if(piece.type != piece_t || ! last_move_rotation)
            {
                return 0;
            }

            int corners = 0;
            corners += occupied(piece.x, piece.y);
            corners += occupied(piece.x + 2, piece.y);
            corners += occupied(piece.x, piece.y + 2);
            corners += occupied(piece.x + 2, piece.y + 2);

            if(corners < 3)
            {
                return 0;
            }

            const point* front = t_front_corners[piece.rot];
            bool front_full = occupied(piece.x + front[0].x, piece.y + front[0].y) &&
                    occupied(piece.x + front[1].x, piece.y + front[1].y);
            return front_full || last_kick == 4 ? 2 : 1;
        }

        void lock_piece()
        {
            int tspin = tspin_kind();
            bool all_hidden = true;

            for(const point& cell : piece_cells[piece.type][piece.rot])
            {
                int cy = piece.y + cell.y;
                board[cy][piece.x + cell.x] = piece.type + 1;

                if(cy >= hidden_rows)
                {
                    all_hidden = false;
                }
            }

            can_hold = true;

            if(all_hidden)
            {
                state = game_state::game_over;
                return;
            }

            int cleared = 0;
            bool perfect = true;

            for(int y = 0; y < board_rows; ++y)
            {
                bool full = true;
                bool empty = true;

                for(int x = 0; x < board_columns; ++x)
                {
                    if(board[y][x])
                    {
                        empty = false;
                    }
                    else
                    {
                        full = false;
                    }
                }

                clearing_rows[y] = full;

                if(full)
                {
                    ++cleared;
                }
                else if(! empty)
                {
                    perfect = false;
                }
            }

            score_clear(cleared, tspin, perfect && cleared > 0);

            if(cleared)
            {
                if(! quiet)
                {
                    bn::sound_items::blip.play();
                }
                clear_timer = clear_delay_frames;
                state = game_state::clearing;
            }
            else
            {
                spawn(pop_queue());
            }
        }

        void score_clear(int cleared, int tspin, bool perfect)
        {
            int points = 0;
            const char* name = nullptr;
            const char* extra = nullptr;

            if(tspin)
            {
                test_tspins = test_tspins + 1;
            }

            if(tspin == 2)
            {
                points = score_tspin[cleared];
                name = "T-GIRO";
            }
            else if(tspin == 1)
            {
                points = score_tspin_mini[cleared < 2 ? cleared : 2];
                name = "T-GIRO MINI";
            }
            else
            {
                points = score_plain[cleared];
                constexpr const char* names[5] = { nullptr, "SIMPLE", "DOBLE", "TRIPLE", "CUADRUPLE!" };
                name = names[cleared];
            }

            if(cleared > 0)
            {
                bool difficult = cleared == 4 || tspin;

                if(difficult && back_to_back)
                {
                    points = points * 3 / 2;
                    extra = "SEGUIDO x1.5";
                }

                back_to_back = difficult;
                ++combo;

                if(combo > 0)
                {
                    points += 50 * combo;
                }

                if(perfect)
                {
                    points += cleared == 4 && extra ? score_perfect_clear_b2b_four : score_perfect_clear[cleared];
                    extra = "TODO LIMPIO!";
                }
            }
            else
            {
                combo = -1;
            }

            score += points * level;

            if(name)
            {
                popup_1 = name;
                popup_2 = extra;
                popup_combo = combo;
                popup_timer = 90;
            }

            lines += cleared;
            int new_level = 1 + lines / lines_per_level;
            level = new_level > max_level ? max_level : new_level;
        }

        void collapse_rows()
        {
            int write = board_rows - 1;

            for(int read = board_rows - 1; read >= 0; --read)
            {
                if(! clearing_rows[read])
                {
                    if(write != read)
                    {
                        for(int x = 0; x < board_columns; ++x)
                        {
                            board[write][x] = board[read][x];
                        }
                    }

                    --write;
                }

                clearing_rows[read] = false;
            }

            for(; write >= 0; --write)
            {
                for(int x = 0; x < board_columns; ++x)
                {
                    board[write][x] = 0;
                }
            }
        }
    };

    game the_game;

    // Rule checks on a scratch game, run once at boot. Each passed check sets one bit of the result.
    int run_selftest()
    {
        static game t;
        t.quiet = true;
        int result = 0;

        // 1: the 7-bag deals every piece once per group of seven.
        t.start(12345);
        bool bag_ok = true;

        for(int group = 0; group < 3; ++group)
        {
            int seen = 0;

            for(int i = 0; i < piece_count; ++i)
            {
                seen |= 1 << (group == 0 && i == 0 ? t.piece.type : t.pop_queue());
            }

            bag_ok = bag_ok && seen == (1 << piece_count) - 1;
        }

        result |= bag_ok ? 1 : 0;

        // 2: SRS wall kick. Vertical I against the left wall rotated clockwise uses test 3 (+2, 0).
        t.start(1);
        t.piece = active_piece{ piece_i, 1, -2, 10 };

        if(t.try_rotate(0) && t.piece.x == 0 && t.piece.rot == 2 && t.last_kick == 2)
        {
            result |= 2;
        }

        // 3: four lines with a vertical I in a one-wide well, then a perfect clear.
        t.start(1);

        for(int y = board_rows - 4; y < board_rows; ++y)
        {
            for(int x = 1; x < board_columns; ++x)
            {
                t.board[y][x] = 1;
            }
        }

        t.piece = active_piece{ piece_i, 1, -2, board_rows - 4 };
        t.lock_piece();

        if(t.state == game_state::clearing && t.lines == 4 && t.back_to_back &&
                t.score == score_plain[4] + score_perfect_clear[4])
        {
            t.collapse_rows();
            bool empty = true;

            for(int y = 0; y < board_rows; ++y)
            {
                for(int x = 0; x < board_columns; ++x)
                {
                    empty = empty && ! t.board[y][x];
                }
            }

            result |= empty ? 4 : 0;
        }

        // 4: T-spin double: T pointing down into a slot under an overhang.
        t.start(1);

        for(int x = 0; x < board_columns; ++x)
        {
            t.board[board_rows - 1][x] = x == 4 ? 0 : 1;
            t.board[board_rows - 2][x] = x >= 3 && x <= 5 ? 0 : 1;
        }

        t.board[board_rows - 3][3] = 1;
        t.piece = active_piece{ piece_t, 2, 3, board_rows - 3 };
        t.last_move_rotation = true;
        t.last_kick = 0;
        t.lock_piece();

        if(t.lines == 2 && test_tspins == 1 && t.score == score_tspin[2])
        {
            result |= 8;
        }

        test_pieces = 0;
        test_holds = 0;
        test_tspins = 0;
        return result;
    }

    void draw_box(int column, int row, int width, int height)
    {
        for(int y = row; y < row + height; ++y)
        {
            for(int x = column; x < column + width; ++x)
            {
                map_data.set(x, y, tile_well, 0);
            }
        }
    }

    void draw_preview(int type, int column, int row, int palette)
    {
        // Centre 3-wide pieces in the 7-wide box; I and O use their 4x4 box one column to the left.
        int x0 = column + (box_size[type] == 4 ? 1 : 2);

        for(const point& cell : piece_cells[type][0])
        {
            map_data.set(x0 + cell.x, row + cell.y - (type == piece_i ? 1 : 0), tile_block, palette);
        }
    }

    void draw_game()
    {
        const game& g = the_game;
        map_data.clear();

        if(g.state == game_state::title)
        {
            return;
        }

        bool over = g.state == game_state::game_over;

        for(int y = 0; y < visible_rows; ++y)
        {
            map_data.set(well_column - 1, y, tile_wall, 0);
            map_data.set(well_column + board_columns, y, tile_wall, 0);

            for(int x = 0; x < board_columns; ++x)
            {
                int value = g.board[y + hidden_rows][x];

                if(g.state == game_state::clearing && g.clearing_rows[y + hidden_rows])
                {
                    bool on = (g.clear_timer / 4) % 2 == 0;
                    map_data.set(well_column + x, y, on ? tile_flash : tile_block, on ? 0 : value);
                }
                else if(over && y >= 7 && y <= 10)
                {
                    // Clear band behind the game over text.
                    map_data.set(well_column + x, y, tile_empty, 0);
                }
                else if(value)
                {
                    map_data.set(well_column + x, y, tile_block, over ? 0 : value);
                }
                else
                {
                    map_data.set(well_column + x, y, tile_well, 0);
                }
            }
        }

        if(g.state == game_state::playing || g.state == game_state::paused)
        {
            int ghost = g.ghost_y();

            for(const point& cell : piece_cells[g.piece.type][g.piece.rot])
            {
                int gy = ghost + cell.y - hidden_rows;

                if(gy >= 0)
                {
                    map_data.set(well_column + g.piece.x + cell.x, gy, tile_ghost, g.piece.type + 1);
                }
            }

            for(const point& cell : piece_cells[g.piece.type][g.piece.rot])
            {
                int py = g.piece.y + cell.y - hidden_rows;

                if(py >= 0)
                {
                    map_data.set(well_column + g.piece.x + cell.x, py, tile_block, g.piece.type + 1);
                }
            }
        }

        draw_box(hold_box_column, hold_box_row, box_width, 4);

        if(g.hold_type >= 0)
        {
            draw_preview(g.hold_type, hold_box_column, hold_box_row + 1, g.can_hold && ! over ? g.hold_type + 1 : 0);
        }

        draw_box(next_box_column, next_box_row, box_width, 3 * next_count + 1);

        for(int i = 0; i < next_count; ++i)
        {
            draw_preview(g.queue[i], next_box_column, next_box_row + 1 + i * 3, over ? 0 : g.queue[i] + 1);
        }
    }

    constexpr int row_y(int row)
    {
        return row * 8 + 4 - 80;
    }

    constexpr int left_x = -84;
    constexpr int right_x = 84;
}

int main()
{
    bn::core::init();
    build_rotations();
    test_selftest = run_selftest();

    bn::bg_tiles::set_allow_offset(false);
    bn::regular_bg_item bg_item(bn::regular_bg_tiles_items::cells, bn::bg_palette_items::cells_palette,
                                map_data.map_item);
    bn::regular_bg_ptr bg = bg_item.create_bg(8, 48);
    bn::regular_bg_map_ptr bg_map = bg.map();
    bn::bg_tiles::set_allow_offset(true);

    bn::sprite_text_generator small_text(common::variable_8x8_sprite_font);
    bn::sprite_text_generator big_text(common::variable_8x16_sprite_font);
    small_text.set_center_alignment();
    big_text.set_center_alignment();

    bn::vector<bn::sprite_ptr, 48> static_sprites;
    bn::vector<bn::sprite_ptr, 16> stat_sprites;
    bn::vector<bn::sprite_ptr, 16> popup_sprites;
    bn::vector<bn::sprite_ptr, 16> overlay_sprites;

    game& g = the_game;
    game_state shown_state = game_state::paused;
    int shown_score = -1;
    int shown_lines = -1;
    int shown_level = -1;
    int shown_popup = -1;
    unsigned frame = 0;
    unsigned over_frame = 0;

    while(true)
    {
        ++frame;

        if(g.state == game_state::title)
        {
            if(bn::keypad::start_pressed() || bn::keypad::a_pressed())
            {
                g.start(frame * 2654435761u + 1);
            }
        }
        else if(g.state == game_state::game_over)
        {
            if(bn::keypad::start_pressed())
            {
                g.start(frame * 2654435761u + 1);
            }
        }
        else
        {
            g.update();
        }

        // Static texts depend on the screen (title or game).
        bool in_title = g.state == game_state::title;

        if((shown_state == game_state::title) != in_title || shown_state == game_state::paused)
        {
            static_sprites.clear();
            shown_score = -1;
            shown_lines = -1;
            shown_level = -1;

            if(in_title)
            {
                big_text.generate(0, -60, "A P I L A", static_sprites);
                small_text.generate(0, -42, "COMPLETA FILAS", static_sprites);
                small_text.generate(0, -28, "IZQ / DER: MOVER", static_sprites);
                small_text.generate(0, -16, "A / B: GIRAR", static_sprites);
                small_text.generate(0, -4, "ABAJO: BAJAR   ARRIBA: SOLTAR", static_sprites);
                small_text.generate(0, 8, "L / R: RESERVAR", static_sprites);
                small_text.generate(0, 20, "START: PAUSA", static_sprites);
            }
            else
            {
                small_text.generate(left_x, row_y(1), "RESERVA", static_sprites);
                small_text.generate(right_x, row_y(1), "SIGUE", static_sprites);
                small_text.generate(left_x, row_y(7), "PUNTOS", static_sprites);
                small_text.generate(left_x, row_y(10), "LINEAS", static_sprites);
                small_text.generate(left_x, row_y(13), "NIVEL", static_sprites);
            }
        }

        if(in_title)
        {
            stat_sprites.clear();
            popup_sprites.clear();
            overlay_sprites.clear();

            if((frame / 30) % 2 == 0)
            {
                small_text.generate(0, 48, "PULSA START", overlay_sprites);
            }
        }
        else
        {
            if(g.score != shown_score || g.lines != shown_lines || g.level != shown_level)
            {
                shown_score = g.score;
                shown_lines = g.lines;
                shown_level = g.level;
                stat_sprites.clear();
                small_text.generate(left_x, row_y(8), bn::to_string<12>(g.score), stat_sprites);
                small_text.generate(left_x, row_y(11), bn::to_string<12>(g.lines), stat_sprites);
                small_text.generate(left_x, row_y(14), bn::to_string<12>(g.level), stat_sprites);
            }

            int popup_id = g.popup_timer > 0 ? g.popup_timer : 0;

            if((popup_id > 0) != (shown_popup > 0) || (popup_id == 89))
            {
                popup_sprites.clear();

                if(popup_id > 0)
                {
                    small_text.generate(left_x, row_y(16), g.popup_1, popup_sprites);

                    if(g.popup_2)
                    {
                        small_text.generate(left_x, row_y(17), g.popup_2, popup_sprites);
                    }

                    if(g.popup_combo > 0)
                    {
                        bn::string<16> combo_text("COMBO ");
                        combo_text += bn::to_string<4>(g.popup_combo);
                        small_text.generate(left_x, row_y(18), combo_text, popup_sprites);
                    }
                }
            }

            shown_popup = popup_id;
            overlay_sprites.clear();

            if(g.state == game_state::paused)
            {
                small_text.generate(0, row_y(9), "PAUSA", overlay_sprites);
            }
            else if(g.state == game_state::game_over)
            {
                small_text.generate(0, row_y(8), "FIN", overlay_sprites);

                if(shown_state != game_state::game_over)
                {
                    over_frame = frame;
                }

                if(((frame - over_frame) / 30) % 2 == 0)
                {
                    small_text.generate(0, row_y(10), "START: OTRA", overlay_sprites);
                }
            }
        }

        shown_state = g.state == game_state::paused ? game_state::playing : g.state;

        draw_game();
        bg_map.reload_cells_ref();

        test_state = int(g.state);
        test_score = g.score;
        test_lines = g.lines;
        test_level = g.level;
        test_piece_type = g.piece.type;
        test_piece_x = g.piece.x;
        test_piece_y = g.piece.y;
        test_piece_rot = g.piece.rot;
        bn::core::update();
    }
}
