## Core (`core`)

It coordinates all of the other modules.

It is needed to start the engine and to update it each frame, among other things.

## Display (`display`)

That square window of 240x160 pixels capable of displaying 32768 colors (15 bits).

## Tiles (`tile`)

On the GBA, the pixel data of sprites and backgrounds is made of pixel tiles: 8x8 pixel matrices.

(dentro de `display`)

## Colors (`color`)

GBA displays 15 bits colors: 5 bits for the blue component, 5 for the green one and 5 for the red one.

(dentro de `display`)

## Color palettes (`palette`)

On the GBA, both sprites and backgrounds have 256 colors organized in blocks of multiples of 16 colors
(512 colors in total). Each one of these blocks is called color palette.

There are 8 bits per pixel palettes (which can refer to all colors)
and 4 bits per pixel palettes (which can access only to 16 consecutive colors).

Butano color palettes system allows to manage individual 4 bits per pixel palettes for sprites and backgrounds,
but it only can be one 8 bits per pixel backgrounds palette and one 8 bits per pixel sprites palette:
all 8 bits per pixel backgrounds share the same 8 bits per pixel backgrounds palette
and all 8 bits per pixel sprites share the same 8 bits per pixel sprites palette.

(dentro de `display`)

## Affine transformation matrices (`affine_mat`)

Used to apply two-dimensional geometric transformations like rotating and/or scaling to sprites and backgrounds.

(dentro de `display`)

## Backgrounds (`bg`)

Big graphics layers.

Depending of their type, the GBA offers up to 4 of them.

(dentro de `display`)

## Background maps (`bg_map`)

Also called tile-maps, backgrounds use them to refer to the tile to render in each 8x8 pixel block.

(dentro de `bg`)

## Regular backgrounds (`regular_bg`)

Tiled backgrounds which can't perform affine transformations.

Also known as text backgrounds.

(dentro de `bg`)

## Affine backgrounds (`affine_bg`)

Tiled backgrounds which can perform affine transformations.

Also known as rotation backgrounds.

(dentro de `bg`)

## Bitmap backgrounds (`bitmap_bg`)

Backgrounds without tiles, with which video memory works like a width x height bitmap.

(dentro de `bg`)

## Palette bitmap backgrounds (`palette_bitmap_bg`)

Dual page bitmap backgrounds in which pixels are palette indexes.

(dentro de `bitmap_bg`)

## Direct bitmap backgrounds (`direct_bitmap_bg`)

Bitmap backgrounds in which pixels are colors.

(dentro de `bitmap_bg`)

## Single page direct bitmap backgrounds (`sp_direct_bitmap_bg`)

Direct bitmap backgrounds with a single page.

(dentro de `direct_bitmap_bg`)

## Dual page direct bitmap backgrounds (`dp_direct_bitmap_bg`)

Direct bitmap backgrounds with two pages.

(dentro de `direct_bitmap_bg`)

## Sprites (`sprite`)

Small animated objects that can move freely from the background(s).

In GBA's jargon they are called OBJs.

(dentro de `display`)

## Backdrop (`backdrop`)

Single color layer displayed behind all backgrounds and sprites.

(dentro de `display`)

## Mosaic (`mosaic`)

It makes sprites and backgrounds look blocky.

It divides a sprite or background into square blocks of pixels.
The top-left pixel of each block is used to fill the rest of that block, which makes it blocky.

(dentro de `display`)

## Blending (`blending`)

Allows transparency and fade effects for sprites and backgrounds.

(dentro de `display`)

## Windows (`window`)

They allow to divide the screen into regions.

For each one of these regions, they can show or hide sprites, backgrounds and blending between them.

(dentro de `display`)

## Inside windows (`inside_window`)

Windows which region can be covered by other windows.

(dentro de `window`)

## Rectangle windows (`rect_window`)

Rectangular windows.

(dentro de `window`)

## Audio (`audio`)

Music for your ears, using both Direct Sound and DMG channels.

## Music (`music`)

Module files (files with `*.mod`, `*.xm`, `*.s3m` and `*.it` extensions) played with Direct Sound channels
thanks to [Maxmod](https://blocksds.skylyrac.net/maxmod/index.html).

(dentro de `audio`)

## DMG music (`dmg_music`)

Module files (files with `*.mod` and `*.s3m` extensions) played with DMG channels
thanks to [GBT Player](https://github.com/AntonioND/gbt-player)
and VGM audio files (files with `*.vgm` extension) played with DMG channels
thanks to [advgm](https://github.com/copyrat90/advgm).

(dentro de `audio`)

## Sound effects (`sound`)

Waveform audio files (files with `*.wav` extension) sound effects played with Direct Sound channels
thanks to [Maxmod](https://blocksds.skylyrac.net/maxmod/index.html).

(dentro de `audio`)

## Keypad (`keypad`)

GBA keys handling.

Keypad logging can be enabled or disabled by overloading the definition of BN_CFG_KEYPAD_LOG_ENABLED.

Recorded key presses can be replayed later by passing the log to bn::core::init.

## Text (`text`)

Butano text system, based on sprites.

Currently, it supports 4 bits per pixel (16 colors) fixed width AND variable width characters.

Text can be printed in one sprite per character or multiple characters per sprite.

Also, UTF-8 characters are supported.

(dentro de `display`)

## Green swap (`green_swap`)

When it is enabled, green intensity of each two screen pixels are exchanged.

It produces an interesting dirt effect.

(dentro de `display`)

## H-Blank effects (`hblank_effect`)

They allow to change how things are drawn in each screen horizontal line,
since they transfer data to the GBA during each horizontal blanking (H-Blank) period.

For example, with a H-Blank effect you can change the transparent color every screen horizontal line,
which is how games like Chrono Trigger fills their menus with a color gradient.

They differ from HDMA in that only a register can be written by a H-Blank effect in each screen horizontal line,
but since multiple H-Blank effects can be displayed at the same time,
non consecutive registers can be written with them.

They are also higher level than HDMA, so they should be your first option.

(dentro de `display`)

## H-Blank direct memory access (`hdma`)

It allows to change how things are drawn in each screen horizontal line,
since they transfer data to the GBA during each horizontal blanking (H-Blank) period.

For example, with HDMA you can change the transparent color every screen horizontal line,
which is how games like Chrono Trigger fills their menus with a color gradient.

It differs from H-Blank effects in that multiple registers can be written by HDMA in each screen horizontal line,
but since there's only one DMA channel available, written registers must be consecutive.

It is also lower level than H-Blank effects, so you should try with H-Blank effects first.

(dentro de `display`)

## Link communication (`link`)

Multiplayer communication through a link cable,
provided by [gba-link-connection](https://github.com/rodri042/gba-link-connection).

Keep in mind that some messages will be lost between players.

## Timers (`timer`)

They allow to measure elapsed times with high precision.

## Game Pak (`game_pak`)

Each GBA game or application is stored on a plastic cartridge, officially called "Game Pak".

## Game Pak SRAM (`sram`)

Allows game or application data to be saved when the GBA is turned off.

(dentro de `game_pak`)

## Date and time (`date_time`)

Utilities for the real time clock (RTC), if the cart has one.

## Rumble (`rumble`)

It allows your GBA to vibrate, if the cart supports it.

## Cameras (`camera`)

They allow to set the position of sprites, backgrounds and rectangle windows relative to another position.

(dentro de `display`)

## Memory (`memory`)

Memory management utilities.

## Asserts (`assert`)

Butano assert system.

It can be enabled or disabled by overloading the definition of BN_CFG_ASSERT_ENABLED.

Note that these asserts can be used in `constexpr` contexts (`if consteval` returns `true`).

## Logging (`log`)

Butano logging system.

It allows to print information on the console of multiple emulators.

It can be enabled or disabled by overloading the definition of BN_CFG_LOG_ENABLED.

It supports printing on only one emulator at once.
The supported emulator can be changed by overloading the definition of BN_CFG_LOG_BACKEND.

## Profiler (`profiler`)

Butano profiling system.

It allows to measure elapsed time between code blocks defined by the user.

It can be enabled or disabled by overloading the definition of BN_CFG_PROFILER_ENABLED.

## Standard library (`std`)

Butano standard library replacement.

Main differences against C++'s standard library is the usage of asserts instead of exceptions
and that it avoids heap usage when possible.

## Containers (`container`)

STL like containers with the capacity defined at compile time.

They don't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `std`)

## Vector (`vector`)

`std::vector` like container with the capacity defined at compile time.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `container`)

## Deque (`deque`)

`std::deque` like container with the capacity defined at compile time.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `container`)

## List (`list`)

`std::list` like container with the capacity defined at compile time.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `container`)

## Intrusive list (`intrusive_list`)

`std::list` like container that doesn't contain values, it just references them.

To be part of an intrusive list, values must inherit bn::intrusive_list_node_type class.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `container`)

## Forward list (`forward_list`)

`std::forward_list` like container with the capacity defined at compile time.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `container`)

## Intrusive forward list (`intrusive_forward_list`)

`std::forward_list` like container that doesn't contain values, it just references them.

To be part of an intrusive forward list, values must inherit bn::intrusive_forward_list_node_type class.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `container`)

## Unordered map (`unordered_map`)

`std::unordered_map` like container with the capacity defined at compile time.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `container`)

## Unordered set (`unordered_set`)

`std::unordered_set` like container with the capacity defined at compile time.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `container`)

## Strings (`string`)

Strings describe objects that can refer to a constant contiguous sequence of char values.

(dentro de `container`)

## Array (`array`)

`std::array` like container.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `container`)

## Bitset (`bitset`)

`std::bitset` like container.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `container`)

## Span (`span`)

`std::span` like container.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `container`)

## Optional (`optional`)

`std::optional` like container.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `container`)

## Any (`any`)

`std::any` like container with the capacity defined at compile time.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `container`)

## Unique pointer (`unique_ptr`)

`std::unique_ptr` like container.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

Unlike most Butano containers, it uses the heap instead of the stack.

(dentro de `container`)

## Pool (`pool`)

A fixed capacity object pool, where allocation and release are O(1) operations.

A pool doesn't destroy its elements in its destructor, they must be destroyed manually.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `container`)

## Allocator (`allocator`)

Generic allocators where allocation and release are not O(1) operations.

An allocator doesn't destroy its elements in its destructor, they must be destroyed manually.

It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.

(dentro de `container`)

## Utilities (`utility`)

Standard library utilities.

(dentro de `std`)

## Functional (`functional`)

Part of the standard function objects library.

It provides the standard hash function.

(dentro de `std`)

## Actions (`action`)

Actions allow the transformation of properties in time.

Some of Butano classes and namespaces can have actions performed on it.

For example, you can move a sprite from its current position to another and do it over a span of time.

## Action templates (`template_action`)

Templates used to build Butano actions.

You can use them to build your own actions too.

(dentro de `action`)

## Tools (`tool`)

Stuff generated by the assets conversion tools to use your assets with Butano.

## Math (`math`)

Math related stuff.

## Random (`random`)

Random number generators.

## Other (`other`)

Stuff which doesn't fit anywhere else.
