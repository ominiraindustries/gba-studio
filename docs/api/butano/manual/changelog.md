# Changelog

@tableofcontents

## 21.10.1

[Maxmod](https://blocksds.skylyrac.net/maxmod/index.html) pan/pitch envelopes fixed.

## 21.10.0

* bn::sound_item::play_optional and bn::sound_item::play_with_priority_optional added.
* [Maxmod](https://blocksds.skylyrac.net/maxmod/index.html) updated to 1.25.0. If you're using
  [Wonderful Toolchain](https://wonderful.asie.pl/), remember to update it and rebuild your project.
* Sprites GFX mode setup fixed (thanks JK0JK!).
* Butano Fighter sound loops removed.
* [Wonderful Toolchain](https://wonderful.asie.pl/) documentation improved.
* faq_ai and faq_audio_duplicated_samples questions added to the faq page.
* [A Betty and Claude Cartoon](https://drkylstein.itch.io/a-betty-and-claude-cartoon),
  [Ounfò Tan Sispann](https://fralacticus.itch.io/ounfo-tan-sispann),
  [Dare to Doku](https://daretosquare.itch.io/dare-to-doku),
  [Pinball Galaxy](https://saraitakemoto.itch.io/pinball-galaxy-demo-ver),
  [Witch×Bullet](https://coffeepasta.itch.io/witchxbullet),
  [xo89](https://nostabyte.itch.io/xo89),
  [MORDOBOY](https://lipucka.itch.io/mordoboy-gba),
  [Knock Time](https://stuck-pixel-studio.itch.io/knock-time),
  [Doom Escaping](https://ticolol.itch.io/doom-escaping) and
  [Aurora Mission](https://rodrigocard.itch.io/aurora-mission) added to `README.md`.
* changelog fixed.

## 21.9.0

* bn::sprite_animate_action::set_update_forever and bn::sprite_cached_animate_action::set_update_forever added.
* bn::sprite_animate_action and bn::sprite_cached_animate_action support a single graphics index.
* Background animate actions have the same features as sprite animate actions.
* bn::fixed, bn::fixed_point, bn::fixed_size and bn::fixed_rect multiplication operators fixed.

## 21.8.0

* bn::sprite_animate_action::set_current_index and bn::sprite_cached_animate_action::set_current_index
  allow to update their sprite on the next tiles change.
* bn::sprite_animate_action::current_graphics_index and bn::sprite_cached_animate_action::current_tiles
  asserts improved.
* bn::sprite_animate_action::current_index, bn::sprite_animate_action::current_graphics_index,
  bn::sprite_cached_animate_action::current_index and bn::sprite_cached_animate_action::current_tiles
  documentation improved.
* [Dare to Square](https://dare-to-square.com) added to `README.md`.

## 21.7.1

Standard containers swap maximum size check fixed (thanks yeon!).

## 21.7.0

* bn::istring::unsafe_resize added.
* bn::ostringstream::append with numbers optimized.
* Logging and assert messages building optimized.
* bn::memory::clear C++26 build warning fixed.

## 21.6.0

* bn::sound::play_optional and bn::sound::play_with_priority_optional added.
* bn::sound_handle::set_speed and bn::sound_handle::set_panning don't crash with inactive sounds.
* [advgm](https://github.com/copyrat90/advgm) updated to 0.3.0. Remember to rebuild your project
  after updating.
* bn::best_fit_allocator::realloc alignment fixed.
* bn::best_fit_allocator::calloc overflow fixed.
* bn::best_fit_allocator::realloc documentation improved.

## 21.5.1

Wait for V-Blank fixed (thanks yeon and AntonioND!).

## 21.5.0

* Multiplications, divisions and memory operations like `memcpy` and `memset` optimized thanks to
  [stdgba](https://github.com/felixjones/stdgba).
* [Maxmod](https://blocksds.skylyrac.net/maxmod/index.html) updated to 1.19.1.
* [advgm](https://github.com/copyrat90/advgm) updated to 0.2.0.
* Header files generated from tools are not written if their content has not changed.
* Heap test fixed.

## 21.4.0

* [VGM player](https://github.com/copyrat90/gbadev-ja-test) replaced with
  [advgm](https://github.com/copyrat90/advgm).
* The import_dmg_music import guide explains how to import `*.vgm` files made with
  [Furnace](https://github.com/tildearrow/furnace).
* faq_sprites_draw_tiles and faq_backgrounds_draw_tiles questions added to the faq page.
* [Modulo bias](https://web.archive.org/web/20201205020502/https://research.kudelskisecurity.com/2020/07/28/the-definitive-guide-to-modulo-bias-and-how-to-avoid-it/)
  links fixed.

## 21.3.0

* bn::regular_bg_tiles_ptr::overwrite_tile, bn::affine_bg_tiles_ptr::overwrite_tile and
  BN_CFG_BG_BLOCKS_MAX_OVERWRITE_TILES added.
* bn::regular_bg_tiles_ptr::bpp added.
* bn::regular_bg_tiles_ptr::offset and bn::affine_bg_tiles_ptr::offset added.
* BN_CFG_AUDIO_MAX_SOUND_CHANNELS and BN_CFG_SPRITE_TILES_MAX_ITEMS no longer have to be a power of two.
* VGM music wave sample fixed (thanks yeon!).
* Animated tiles scene added to `regular_bgs` and `affine_bgs` examples.
* bn::core callbacks documentation improved.
* import_generic_data section added to the import guide.
* faq_backgrounds_animated question improved.
* [Wonderful Toolchain](https://wonderful.asie.pl/) added to `README.md`.

## 21.2.0

* bn::regular_bg_tiles_item can contain more than 1024 tiles.
* bn::affine_bg_tiles_item can contain more than 256 tiles.
* Tiles offset can now be specified when creating bn::regular_bg_tiles_ptr and bn::affine_bg_tiles_ptr objects.
* bn::affine_bg_tiles_item::valid_tiles_count fixed.
* faq_backgrounds_tiles_offset question added to the faq page.

## 21.1.0

* bn::span<const bn::color> overloads for bn::bg_palette_ptr::set_colors and
  bn::sprite_palette_ptr::set_colors added.
* bn::palette_bitmap_item overloads for bn::palette_bitmap_bg_painter::unsafe_blit,
  bn::palette_bitmap_bg_painter::blit, bn::palette_bitmap_bg_painter::unsafe_draw and
  bn::palette_bitmap_bg_painter::draw added.
* faq_backgrounds_regular_wrapping and faq_backgrounds_metatiles questions added to the faq page.
* [Bloxorz](https://nostabyte.itch.io/bloxorzgba) added to `README.md`.
* [Maxmod](https://blocksds.skylyrac.net/maxmod/index.html) links updated.

## 21.0.0

* Palette bitmap backgrounds support. Check the `palette_bitmap_bgs` example
  and the import_image import guide to see how to use them.
* Single page direct bitmap backgrounds support. Check the `sp_direct_bitmap_bgs` example
  and the import_image import guide to see how to use them.
* Dual page direct bitmap backgrounds support. Check the `dp_direct_bitmap_bgs` example
  and the import_image import guide to see how to use them.
* bn::sprites::bg_sorting_enabled and bn::sprites::set_bg_sorting_enabled added.
  Check the `sprites` example to see how to create sprite masks with them.
* bn::memory::vram_safe_copy added.
* bn::clip_line added.
* `const` `vram` methods added for bn::regular_bg_tiles_ptr, bn::regular_bg_map_ptr,
  bn::affine_bg_tiles_ptr, bn::affine_bg_map_ptr and bn::sprite_tiles_ptr.
* Sprite tiles are allocated at the end of VRAM.
* Affine backgrounds setup optimized.
* faq_sprites_max_scale question added to the faq page.
* faq_backgrounds_animated question improved.
* [Maxmod](https://blocksds.skylyrac.net/maxmod/index.html) links updated.

## 20.3.2

BN_CFG_SPRITE_TILES_LOG_ENABLED and BN_CFG_BG_BLOCKS_LOG_ENABLED fixed.

## 20.3.1

* [AAS](https://github.com/stuij/apex-audio-system) processed audio size log removed since it was broken.
* `audio_player` example shows the audio backend.
* [RR Project](https://gvaliente.itch.io/rr-project) added to `README.md`.

## 20.3.0

* bn::unique_ptr constructors with a deleter added.
* bn::unique_ptr doesn't call the deleter if it doesn't contain a managed object.
* bn::unordered_map and bn::unordered_set documentation improved.
* faq_donate question added to the faq page.

## 20.2.0

* Custom palette effects added. Check the `palettes` example to see how to apply them.
* bn::log_level and BN_LOG_LEVEL added.
* bn::pool alignment fixed.
* [Heartwrench Advance](https://michaelchase.itch.io/heartwrench-advance) added to `README.md`.

## 20.1.0

* bn::fixed::shift_integer, bn::fixed::shift_multiplication, bn::fixed::unsafe_shift_multiplication,
  bn::fixed::unsafe_shift_multiplication, bn::fixed::shift_division and bn::fixed::unsafe_shift_division added.
* bn::fixed::left_shift and bn::fixed::right_shift added.
* [Maxmod](https://blocksds.skylyrac.net/maxmod/index.html) updated to 1.14.0.
* bn::best_fit_allocator alignment increased from 4 to 8 bytes.
* ROM usage reduced (thanks yeon!).
* bn::bg_maps and bn::bg_tiles documentation improved.
* Assets tool typo fixed (thanks alope107!).
* faq_sprites_top_left, faq_backgrounds_top_left and faq_audio_music_crash questions improved.
* [butano-ldtk](https://github.com/copyrat90/butano-ldtk) added to `README.md`.

## 20.0.0

* Butano now uses [BlocksDS's](https://blocksds.skylyrac.net)
  [Maxmod fork](https://blocksds.skylyrac.net/maxmod/index.html) for Direct Sound audio playback:
  it provides new features, bug fixes and reduced IWRAM usage.
* Audio events support added. Check
  [Maxmod documentation](https://blocksds.skylyrac.net/maxmod/md_documentation_2using__song__events.html)
  and the `audio` example to see how to insert and read audio events.
* bn::audio_mixing_rate, bn::audio::available_mixing_rates, bn::audio::mixing_rate and bn::audio::set_mixing_rate added.
* bn::jingle::pause, bn::jingle::resume and bn::jingle::stop added.
* bn::core::update_callback and bn::core::set_update_callback added.
* bn::memory::copy and bn::memory::clear array overloads added.
* bn::regular_bg_builder and bn::affine_bg_builder top-left coordinates fixed.
* Mixing rate can be changed in the `audio_player` example.

## 19.5.0

* bn::istring::from_char_array added.
* bn::istring_base public constructor documentation improved.
* faq_screen_pixels question added to the faq page.

## 19.4.1

Files in audio folders with unknown extensions are ignored.

## 19.4.0

* [Tonclib](https://www.coranac.com/man/tonclib) updated to 1.4.5.
* [Maxmod](https://maxmod.org) updated to 2.1.0.
* bn::safe_degrees_angle fixed.

## 19.3.0

* bn::hdma::high_priority_interrupt_handler, bn::hdma::set_high_priority_interrupt_handler and
  BN_CFG_HDMA_HIGH_PRIORITY_IRQ_ENABLED added.
* GCC 15 build fixed (thanks asie!).
* Huffman decompression is implemented by the BIOS to avoid relying on decompiled code.
* bn::time::set_minute and bn::time::set_second documentation fixed.

## 19.2.0

* bn::ivector::insert, bn::ilist::insert and bn::iforward_list::insert_after overloads added.
* BN_CFG_SPRITES_USE_IWRAM added.
* Import tool now checks if all input audio files have been imported or not.
* Release sounds from [AAS](https://github.com/stuij/apex-audio-system) queue fixed.
* faq_sprites_sort_layers question added to the faq page.

## 19.1.0

* bn::sprite_animate_action::set_current_index and
  bn::sprite_cached_animate_action::set_current_index added.
* bn::sprite_animate_action::set_next_change_updates and
  bn::sprite_cached_animate_action::set_next_change_updates added.
* bn::hdma::start and bn::hdma::high_priority_start check if the DMA channel is available.
* Link-time optimization builds fixed.

## 19.0.0

* `aas` ([Apex Audio System](https://github.com/stuij/apex-audio-system)) and `null` audio backends added.
  Audio backends can be specified with the `AUDIOBACKEND` makefile variable.
* BN_CFG_AUDIO_STEREO and BN_CFG_AUDIO_DYNAMIC_MIXING added.
* `null` DMG audio backend added. DMG audio backends can be specified with the `DMGAUDIOBACKEND` makefile variable.
* EWRAM overclocking removed, as some GBA SPs crash after a few frames when enabled.
* Sounds with higher priority are not discarded on the [Maxmod](https://maxmod.org) queue
  when playing a new one.
* Erase sounds from [Maxmod](https://maxmod.org) queue fixed.

## 18.9.0

* bn::fixed::modulo fixed.
* Binary data folders documentation improved.
* Some 3rd party games have been forked to work with this release.

## 18.8.0

* bn::best_fit_allocator::check_empty_on_destructor and bn::best_fit_allocator::set_check_empty_on_destructor added.
* BN_CFG_ASSERT_SHOW_GENERAL_INFO added.
* BN_CFG_ASSERT_TAG added.
* Import tool now allows to specify the width of sprites and sprite tiles.
  See the import_image import guide to learn how to specify it.
* Show the sum of the ticks of all entries in the profiler results.
* CPU usage reduced in most resource managers.
* getting_started guide improved.

## 18.7.1

* Placement `new` calls with user-provided `operator new` overloads fixed.
* bn::unordered_map and bn::unordered_set clangd warnings fixed.

## 18.7.0

* bn::green_swap_mode added (thanks Sono!). Check the `green_swap` example to see how to set
  how a background must be displayed when green swap is enabled.
* bn::unique_ptr `operator<<` added.
* bn::ibitset `operator<<` added.
* Hash support for bn::ibitset added.
* More bn::istring_base comparison operators added.
* bn::utf8_character validation improved.
* Green swap disabled when the display is stopped.
* `libstdc++` is now linked by default (thanks asie!).
* faq_delta_time, faq_sprites_metasprites and faq_backgrounds_animated questions
  added to the faq page.
* faq_standard and faq_memory_heap questions improved.
* getting_started_wt guide updated.
* qt_creator improved.
* Some spelling mistakes fixed.

## 18.6.0

* Big maps maximum size is limited to 16384 pixels.
* `create_new` map methods are deprecated.
* Small ROM usage reduction.
* bn::sprite_ptr::create_optional fixed.
* faq_subfolder question added to the faq page.

## 18.5.0

* bn::sprite_text_generator uses one sprite per character if the space between characters is too big.
* bn::sprite_ptr::horizontal_shear and bn::sprite_ptr::vertical_shear fixed.
* bn::sprite_font space between characters validation fixed.
* faq_memory_arm_iwram question added to the faq page.

## 18.4.0

* bn::random::get_bool and bn::seed_random::get_bool added.
* bn::bitset_ref move constructor added.
* BN_CFG_BG_BLOCKS_SANITY_CHECK_ENABLED and BN_CFG_SPRITE_TILES_SANITY_CHECK_ENABLED added.
* bn::random::get_int and bn::seed_random::get_int optimized.
* BG blocks manager allocation fixed.
* bn::sprite_tiles::log_status items count fixed.

## 18.3.0

* bn::sprite_text_generator::generate_top_left and bn::sprite_text_generator::generate_top_left_optional added.
* bn::sprite_text_generator allows to setup the mosaic, the blending, the visibility and the camera
  of the output sprites.
* bn::sprite_builder::release_affine_mat, bn::sprite_builder::release_camera,
  bn::regular_bg_builder::release_camera and bn::affine_bg_builder::release_camera fixed.
* bn::sprite_builder, bn::regular_bg_builder and bn::affine_bg_builder documentation improved.

## 18.2.1

* bn::forward_list and bn::intrusive_forward_list forward iterator requirements fixed.
* bn::list, bn::intrusive_list, bn::unordered_map and bn::unordered_set bidirectional iterator requirements fixed.
* bn::deque iterator random access requirements fixed.
* bn::deque, bn::list, bn::intrusive_list, bn::forward_list, bn::intrusive_forward_list, bn::unordered_map
  and bn::unordered_set iterator type aliases fixed.

## 18.2.0

* bn::regular_bg_ptr, bn::regular_bg_builder, bn::affine_bg_ptr, bn::affine_bg_builder,
  bn::affine_bg_mat_attributes, bn::sprite_ptr and bn::sprite_builder
  support top-left coordinates for their position.
* bn::regular_bg_top_left_move_to_action, bn::regular_bg_top_left_move_loop_action,
  bn::regular_bg_top_left_move_toggle_action, bn::affine_bg_top_left_move_to_action,
  bn::affine_bg_top_left_move_loop_action, bn::affine_bg_top_left_move_toggle_action,
  bn::sprite_top_left_move_to_action, bn::sprite_top_left_move_loop_action
  and bn::sprite_top_left_move_toggle_action added.
* bn::safe_degrees_angle, bn::degrees_lut_sin_safe, bn::degrees_lut_cos_safe
  and bn::degrees_lut_sin_and_cos_safe added.
* bn::affine_mat_attributes::set_rotation_angle_safe, bn::affine_bg_builder::set_rotation_angle_safe,
  bn::affine_bg_mat_attributes::set_rotation_angle_safe, bn::affine_bg_ptr::set_rotation_angle_safe,
  bn::sprite_affine_mat_ptr::set_rotation_angle_safe, bn::sprite_builder::set_rotation_angle_safe
  and bn::sprite_ptr::set_rotation_angle_safe added.
* Position in `create` methods is now optional.
* bn::intrusive_list and bn::intrusive_forward_list iterator constructors are now public.
* Real time clock (RTC) string added.
* SRAM string with link-time optimization fixed.
* faq_backgrounds_top_left and faq_sprites_top_left questions added to the faq page.
* faq_flash_carts_sram question improved.

## 18.1.0

* bn::blending::set_transparency_and_intensity_alpha added.
* bn::timer::elapsed_ticks_with_restart added.
* bn::timers::ticks_per_second added.
* BN_CFG_TIMER_FREQUENCY added.
* bn::timer overflow fixed.
* bn::degrees_lut_sin, bn::degrees_lut_cos and bn::degrees_lut_sin_and_cos optimized.
* Color palettes, sprite tiles and audio management ROM usage reduced.
* `timer` example added.
* faq_sprites_item_from_ptr question added to the faq page.
* [GBA pseudo-3D demo](https://github.com/chrislewisdev/gba-pseudo-3d) added to `README.md`.

## 18.0.1

Tools no longer crash on Windows with more than 60 logical CPU cores.

## 18.0.0

* Language standard updated to C++23.
* Old bn::hdma::start and bn::hdma::high_priority_start API deprecated.
* `BN_UNREACHABLE` deprecated.
* Construction of bn::string and bn::string_view from `nullptr` is disallowed (thanks C++23).
* bn::string::starts_with, bn::string::ends_with, bn::string_view::starts_with and bn::string_view::ends_with
  a `nullptr` char array is disallowed.
* bn::unique_ptr is now `constexpr`.
* bn::bitset and bn::string aren't `constexpr` anymore (it didn't work before).
* More overloads added to bn::string construction, assignment, bn::string::assign and bn::string::append.
* bn::string::find and bn::string_view::find added.
* bn::string::contains and bn::string_view::contains added.
* bn::string::substr added.
* bn::string_view::ends_with a char array added.
* `nullptr` constructor and method overloads added to bn::unique_ptr.
* Non-throwing `new` and `delete` supported.
* bn::string_view::remove_prefix and bn::string_view::remove_suffix assert fixed.
* `bn::is_constant_evaluated` replaced with `consteval`.
* Unnecessary `operator!=` implementations removed.
* bn::regular_bg_builder::release_map, bn::affine_bg_builder::release_map,
  bn::sprite_builder::release_tiles and bn::sprite_builder::release_palette ROM usage reduced.
* bn::deque iterators order operators optimized.

## 17.9.0

* bn::hdma::start and bn::hdma::high_priority_start API improved.
* Some assert messages improved.
* Missing header includes added.
* `hdma_palette` example added.
* Dead links updated.

(Recortado a las 400 líneas más recientes; el completo está en `engine/butano/butano/include/documentation/bn_documentation_g_changelog.h`.)
