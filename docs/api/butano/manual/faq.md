# Frequently asked questions (FAQ)

@tableofcontents

## General

### How do I-

The best way to get started is to read the
guide to download, install and start using Butano.

After that, there are various examples explaining most aspects of the engine.

At last, check how to import your own assets in a game and
take a look at the [modules](modules.html) page.

This page is worth a look too.

### Would I be able to sell my game made with Butano for money?

Sure!

If you comply with [the Butano license](https://github.com/GValiente/butano/blob/master/LICENSE)
and [the licenses of the third party libraries](https://github.com/GValiente/butano/blob/master/licenses)
used by Butano, you can sell your game without issues.

If you use assets such as the sprite font from the examples or from the `common` folder,
you should also comply with their license. The licenses for these assets can be found in
[the credits folder](https://github.com/GValiente/butano/blob/master/credits).

### Has Butano been made with the help of AI?

Butano itself doesn't contain any AI-generated code. I'm not 100% sure about third party libraries,
but I think they're AI-free too. If you think a third party library contains AI code, please let me know.

### Is it possible to use Butano to create multiboot games?

It is possible, but not recommended as Butano footprint is too big for multiboot stuff,
unless you want to make something *very* simple.

Multiboot ROMs *should be* generated when they have the suffix `_mb`:

`TARGET := $(notdir $(CURDIR))_mb`

### Is there any way to donate to Butano?

Yes, you can make games with it and share them with your friends :)

You also can:
* Make good bug reports.
* Help newbies.
* Show cool splash screens in your games to say they were made with Butano.

## Programming

### Can I use Butano without knowing C++?

While Butano is easy to use if you have some C++ skills,
it is going to be hard to work with it if you don't know nothing about C++,
so you should learn a bit of C++ before diving into Butano.

After that, maybe you can learn both C++ and Butano at the same time while you make a game,
but if you don't want to have a hard time with Butano you should know the basics of C++ before messing with it.

### Is there any good free IDE for Butano projects out there?

Take a look at the qt_creator.

### Why do I get an incomplete type error when trying to use a Butano class?

If you get an error message like these:

```
error: variable 'bn::regular_bg_ptr bg' has initializer but incomplete type
error: invalid use of incomplete type 'class bn::regular_bg_ptr'
error: aggregate 'bn::fixed' has incomplete type and cannot be defined
error: static assertion failed: template argument must be a complete class or an unbounded array
```

It is almost always because of lack of included headers.

You can find the header of each Butano class in its documentation page.

### Why can't I use some standard functions and classes?

You need to link the standard system libraries to use standard functions like `strlen`
and standard classes like `std::shared_ptr`.

To do that, the `DEFAULTLIBS` variable of the `Makefile` of your project must not be empty. For example:

`DEFAULTLIBS := true`

Remember to rebuild your project from scratch after modifying a `Makefile` (`make clean` before `make`).

Since you are now linking the standard system libraries, you must comply with their license.
Also, don't expect all standard functions and classes to work after linking the standard system libraries.

### Why can't I use the content of the common folder in a new project?

The `common` folder contains code and assets used by most Butano examples and tests.

It is not part of Butano (the library), so it is not included by default in new projects.

As the content of the `common` folder could change in future Butano versions,
you should copy its content to your project if you want to use it:
* Copy `common` `*.cpp` files to the `src` folder of your project.
* Copy `common` `*.h` files to the `include` folder of your project.
* Do the same with the rest of the files in the `common` folder (`*.bmp`, `*.json`, etc).

### Can I use subfolders for code and assets?

Yes, but you need to add them to the `Makefile` of your project.

For example:

`SOURCES := src src/subfolder`

### What function or method call is the source of an error message?

You can enable stack trace logging in the `Makefile` of your project, so that when an error message is displayed,
the current stack trace will be logged.

To enable stack trace logging, the `STACKTRACE` variable must not be empty. For example:

`STACKTRACE := true`

Remember to rebuild your project from scratch after modifying a `Makefile` (`make clean` before `make`).

If everything went well, the error message should indicate that the stack trace has been logged.
In that case, open the logs window of your emulator and the mangled stack trace should be there.
To demangle it, you can use [demangler.com](http://demangler.com/) for example.

### Why do I get undefined reference errors after updating Butano?

If after updating Butano you get error messages like this one:

`error: undefined reference to 'bn::reciprocal_lut_16'`

They usually go away with a full rebuild of your project (`make clean` before `make`).

Also make sure you update [devkitARM](https://devkitpro.org/)
or [Wonderful Toolchain](https://wonderful.asie.pl/) when you update Butano, since some Butano features
don't work with older releases.

### How to destroy sprites and backgrounds?

bn::sprite_ptr, bn::regular_bg_ptr and all Butano classes that end with the `_ptr` suffix are
`std::shared_ptr` like smart pointers that retains shared ownership of a hardware resource.

In the case of bn::sprite_ptr, several bn::sprite_ptr objects may own the same sprite.
The sprite is released when the last remaining bn::sprite_ptr owning it is destroyed.

If you want to learn more about `std::shared_ptr`, you can read:
* [A basic introduction of standard smart pointers](https://www.geeksforgeeks.org/auto_ptr-unique_ptr-shared_ptr-weak_ptr-2).
* [A std::shared_ptr usage guide](https://docs.microsoft.com/en-us/cpp/cpp/how-to-create-and-use-shared-ptr-instances?view=msvc-160).

### That's cool, but how can I destroy and reload sprites and backgrounds as easy as possible?

bn::optional helps to unload and reload hardware resources easily:

```cpp
// bg_optional is empty:
bn::optional<bn::regular_bg_ptr> bg_optional;

// bg_optional now contains the background specified by bn::regular_bg_items::bg1:
bg_optional = bn::regular_bg_items::bg1.create_bg(0, 0);

// bg_optional is empty again:
bg_optional.reset();

// bg_optional now contains the background specified by bn::regular_bg_items::bg2:
bg_optional = bn::regular_bg_items::bg2.create_bg(0, 0);
```

### Can I use float or double when coding for the GBA?

Modern CPUs have dedicated hardware for processing `float`s and `double`s.
The GBA doesn't have it, so you're left with two choices:

* Using `float`s and `double`s emulated in software (*very* slow).
* Using fixed point arithmetic, which is just integer arithmetic with a few extra steps.

Butano provides bn::fixed, which tries to be as close as possible to a `float` with fixed point arithmetic.
Use it when you want to work with numbers with decimal points.

### How do I print a number in a string?

*Don't* use `sprintf`, `std::ostringstream` or anything else from the standard library, because:

* Butano functions for printing numbers in strings can be more than 5 times faster than the standard functions.
* Standard functions can take more than 4KB of IWRAM.
* Standard functions sometimes can't be used because they throw link errors.

If you want to print a number in a string with Butano, bn::to_string provides the easiest way to do it:

```cpp
bn::string<16> string = bn::to_string<16>(number);
```

If you want to compose a string with multiple numbers, you can use bn::ostringstream too:

```cpp
bn::string<64> string;
bn::ostringstream string_stream(string);
string_stream << "Text: ";
string_stream << 123;
string_stream << " - ";
string_stream << number;
```

### How can I set the seed of a bn::random?

If you want to do that, use bn::seed_random instead.

### Does bn::rect work with odd dimensions?

Since it takes a center point as position instead of a top-left point, bn::rect has problems with odd dimensions.

If you need to handle odd dimensions, use bn::top_left_rect instead.

### How can I get the delta time?

You can usually assume than your game is running at 60FPS, so to wait one second
you should call bn::core::update 60 times. In this case, the delta time (the time elapsed
between two bn::core::update calls) is always one frame, or `1 / 60` seconds.

However, if your game is so choppy that it has variable frame rate (it shouldn't), you can get
the number of frames that were missed in the last bn::core::update call with bn::core::last_missed_frames.
The delta time is the number of missed frames plus one:

```cpp
int elapsed_frames = bn::core::last_missed_frames() + 1;
```

### Is there a way to stop running my code for a certain amount of time?

Since you can usually assume than your game is running at 60FPS,
for example you can wait one second with this code:

```cpp
for(int index = 0; index < 60; ++index)
{
    bn::core::update();
}
```

### How can I know what code makes my game crash?

[No$gba](https://problemkaputt.de/gba.htm) exception system allows to catch common programming errors.
When an exception is triggered, No$gba can stop the execution of the ROM and show the code that has triggered the exception.
Please check the nocashgba_exception if you want to try it.

As always, you also can remove code until the crash goes away.

### Can I directly manipulate screen pixels?

You can directly manipulate the pixels of bitmap backgrounds, but avoid them if you can,
as they're much slower than tiled backgrounds.

### Are there some more general notes on GBA programming out there?

[I'm glad you asked](https://gbadev.net/tonc/first.html#sec-notes).

## Memory

### Why are there std like containers included with Butano?

Butano containers differ from the standard library ones in two important points:
* They don't use the heap, their content is always on the stack.
* They don't throw exceptions. assert are used instead.

Since avoiding heap usage and exceptions is usually good for GBA development,
use Butano containers whenever possible.

Keep in mind that unlike most Butano containers, bn::unique_ptr uses the heap instead of the stack.

### So I shouldn't use the heap?

Since heap usage is slow and the heap allocator included with Butano is not very fast,
avoid heap usage whenever possible.

Also, remember to call bn::core::init before using the heap.

### Why do I run out of memory so often?

Besides VRAM and such, the GBA provides two memory banks:
* IWRAM: 32KB fast RAM.
* EWRAM: 256KB slow RAM.

Data is allocated in IWRAM by default, so it is common to run out of memory if you don't use EWRAM.

To place data in EWRAM, you can:
* Allocate memory in the heap, since it is placed in EWRAM.
* Declare static data with the `BN_DATA_EWRAM` macro:
```cpp
BN_DATA_EWRAM static_data data;
```

However, if the data is read only, you can avoid wasting RAM by placing it in ROM with the `constexpr` qualifier:
```cpp
constexpr const_data data;
```

bn::memory provides functions to query available and used RAM, like bn::memory::used_stack_iwram
and bn::memory::used_static_iwram.

To avoid running out of IWRAM, Butano Fighter and Varooom 3D place all scenes in EWRAM.
Check their `main.cpp` files to see how it works.

### Does Butano allow to declare bn::sprite_ptr or bn::regular_bg_ptr objects globally?

In general, you should not do anything with Butano before calling bn::core::init,
including creating global Butano objects and allocating memory in the heap.

If you want to declare global Butano objects, you can do something like this instead:

```cpp
struct global_data
{
    bn::sprite_ptr sprite;
    bn::regular_bg_ptr bg;
};

global_data* global_ptr;

int main()
{
    bn::core::init();

    global_data global_instance = {
        bn::sprite_items::sprite_item.create_sprite(0, 0),
        bn::regular_bg_items::bg_item.create_bg(0, 0)
    };

    global_ptr = &global_instance;

    // ...
}
```

With that, you can access global Butano objects from anywhere in your project with this code:

```cpp
global_ptr->sprite.set_position(50, 50);
```

### How can I generate ARM code in IWRAM?

By default, functions and methods are compiled to Thumb code and placed in ROM.
If you want to increase the performance of a function/method, a good way is to compile it to ARM code
and place it in IWRAM.

To do it, you have to:
* Place the `BN_CODE_IWRAM` macro before the function/method declaration to indicate its section.
For example:
```cpp
BN_CODE_IWRAM void my_function(int arg);
```
* Place the function/method definition in a file with extension `.bn_iwram.cpp`.

For example, the `world_map` example generates ARM code in IWRAM for the `load_attributes` function.

Keep in mind that IWRAM is small, so you shouldn't place too much code in it.

## Images

### Why do I get an "invalid header size" error when I try to import a *.bmp file?

If you get that error when importing a `*.bmp` file, it probably means that it contains unsupported features, such as:
* Compression.
* Color space information.
* Non-indexed colors.

Please check the import_image import guide to learn more about how to generate supported `*.bmp` files.

## Colors

### Which color is the transparent one?

Butano supports 16 or 256 color images only, so they must have a color palette.

The transparent color is the first one in the color palette,
so in order to change it you should use a bitmap editor with color palette manipulation tools,
like [Usenti](https://github.com/gb-archive/usenti):

@image html import_usenti.png

### How can I set the backdrop color?

The transparent or the backdrop color (displayed color when nothing else is)
is the first one in the backgrounds palette.

You can override its default value with bn::bg_palettes::set_transparent_color.

### How to share the same color palette between sprites or backgrounds?

If two sprites or backgrounds have the same colors, by default they share the same color palette.

Keep in mind that unused colors are also taken into account when deciding if two color palettes are equal or not.

### Why changing the color palette of an 8BPP sprite or background doesn't work?

Since the GBA has only 256 colors for sprites, if you use two sprites with more than 16 colors at the same time,
Butano assumes that they have the same color palette (same colors in the same order).

So if you change the palette of an 8BPP sprite with a new palette item, Butano assumes that the palette
created with the palette item is the same as the old one and doesn't update the colors of the sprite:

```cpp
sprite.set_palette(palette_item);
```

If what you want to do is to update the colors of the sprite, you have to retrieve the palette of the sprite
and update its colors:

```cpp
bn::sprite_palette_ptr sprite_palette = sprite.palette();
sprite_palette.set_colors(palette_item);
```

The same happens with 8BPP backgrounds.

## Sprites

### Why are sprite coordinates relative to the center of the screen instead of the top-left corner?

There's no way to make sprite coordinates relative to the top-left corner of the screen by default.

However, if you don't like it, you can always use the `top_left` methods to specify coordinates relative
to the top-left corner of the screen. They're a bit slower than the regular ones, though.

### Why everything looks weird when I show two or more sprites with more than 16 colors?

Since the GBA has only 256 colors for sprites, if you use two sprites with more than 16 colors at the same time,
Butano assumes that they have the same color palette (same colors in the same order).

So if you are going to show multiple sprites with more than 16 colors at the same time, use the same color palette
with all of them (in the same scene of course, sprites shown in different scenes can have different color palettes).

### Is there a way to get the bn::sprite_item used to create a bn::sprite_ptr?

No :)

### Is 2x the maximum scale for sprites?

Yes. Well, the size of a sprite's canvas can only be doubled, so for example if you need to scale
a sprite of 16x16 pixels more than 2x, you need to resize the image used to create the sprite to
at least 32x32 pixels.

It's a hardware limitation, Butano can't avoid it.

### Why whenever I have too many sprites on screen, some of them get cut off?

If you show too many sprites on screen (especially if they're rotated or scaled),
they won't be rendered because the GBA has a
[maximum number of sprite pixels per scanline](https://problemkaputt.de/gbatek-lcd-obj-overview.htm).

You can try to get around this limitation by showing sprites without rotation or scaling, or even better,
using backgrounds instead of sprites.

### Does Butano support metasprites?

Metasprites (groups of regular sprites that together represent one single bigger sprite) are not supported by Butano.

### How can I draw on top of sprite tiles?

Butano doesn't provide an easy way to do it, so you have to allocate tiles in VRAM with
bn::sprite_tiles_ptr::allocate and draw on the the allocated tiles returned by bn::sprite_tiles_ptr::vram.
Remember that you can't write to VRAM in byte-sized chunks.

### Does hiding a sprite make it count towards the total number of allowed sprites?

Hidden sprites are not committed to the GBA, but they still take resources like color palettes and VRAM.

The easiest way to increase the maximum number of hidden sprites allowed for a particular project is
to set it in the `USERFLAGS` of its `Makefile`.

For example, to set the maximum number of hidden sprites to 256:

`USERFLAGS := -DBN_CFG_SPRITES_MAX_ITEMS=256`

Remember to rebuild your project from scratch after modifying a `Makefile` (`make clean` before `make`).

### Why do I get "no more sprite sort layers available" errors?

Sprites are grouped in layers depending of their background priority and z order, so to reduce memory usage
and improve performance, please use as few unique z orders as possible.

If you still need to increase the maximum number of sprite sort layers available for a particular project,
the easiest way to do it is to set it in the `USERFLAGS` of its `Makefile`.

For example, to set the maximum number of sprite sort layers to 32:

`USERFLAGS := -DBN_CFG_SPRITES_MAX_SORT_LAYERS=32`

Remember to rebuild your project from scratch after modifying a `Makefile` (`make clean` before `make`).

### How can I print UTF-8 characters like Japanese or Chinese ones?

bn::sprite_text_generator already supports UTF-8 characters rendering,
but the bn::sprite_font instances used in the examples don't provide Japanese or Chinese characters,
so you will have to make a new one with them.

## Backgrounds

### Why are background coordinates relative to the center of the screen instead of the top-left corner?

There's no way to make background coordinates relative to the top-left corner of the screen by default.

If you don't like it, you can always use the `top_left` methods to specify coordinates relative
to the top-left corner of the screen. They're a bit slower than the regular ones, though.

### Why everything looks weird when I show two or more backgrounds with more than 16 colors?

Since the GBA has only 256 colors for tiled backgrounds, if you use two 8BPP backgrounds,
Butano assumes that they have the same color palette (same colors in the same order).

So if you are going to show multiple 8BPP backgrounds, you can:
* Use the same color palette with all of them (in the same scene of course, backgrounds shown in different scenes
  can have different color palettes).
* Change their BPP mode to 4BPP, so each background can have its own color palette with more than 16 colors.

### What's a big background?

The GBA only supports some fixed sizes for background maps.

However, Butano allows to manage background maps with any size up to 16384 pixels and multiple of 256 pixels.
These special background maps and the backgrounds that display them are called big maps/backgrounds.

Try to avoid big backgrounds whenever possible, because they are slower CPU wise.

### Why are there two types of backgrounds (regular and affine)?

It seems it is always better to use affine backgrounds, since they can be rotated, scaled, etc.
and its size can be up to 1024x1024 pixels without becoming big backgrounds.

However, compared to regular backgrounds, affine backgrounds have these limitations:
* Only two of them can be displayed at the same time, instead of four.
* They don't support 16 color tiles, only 256 color ones.
* They only support up to 256 different tiles, instead of 1024.
* They don't support flipped tiles.

Because of these limitations, you should avoid affine backgrounds whenever possible.

### How can I show a background with animated tiles?

There are multiple ways, from easy to powerful:
* If you only want to show some basic tile animation, such the water or the grass tiles in top-down RPGs,
  you can use bn::regular_bg_tiles_ptr::overwrite_tile and bn::affine_bg_tiles_ptr::overwrite_tile.
  You also can use one of these actions: bn::regular_bg_animate_action, bn::regular_bg_cached_animate_action,
  bn::affine_bg_animate_action and bn::affine_bg_cached_animate_action.
  The `regular_bgs` and `affine_bgs` examples show how to use the overwrite tile methods and some of these actions.
* If you want to show something more advanced, like destructible tiles, you should first think if it's possible
  to handle those tiles with sprites, because it's going to be easier than the following approaches.
* If animation actions and extra sprites are not enough, you need to manage the background map by yourself.
  The `dynamic_regular_bg` and `dynamic_affine_bg` examples show how to handle a dynamic map.
* If you need to squeeze the GBA CPU as much as possible, you always can handle *everything* by yourself
  by calling bn::regular_bg_tiles_ptr::allocate and bn::regular_bg_map_ptr::allocate
  (or bn::affine_bg_tiles_ptr::allocate and bn::affine_bg_map_ptr::allocate).

Some dynamic map performance tips:
* You can improve animated tiles performance if you only update the tiles to animate in the visible area
  of your "virtual" map.
* bn::regular_bg_map_ptr::reload_cells_ref() and bn::affine_bg_map_ptr::reload_cells_ref() only copy
  the visible map cells to VRAM if the map is big, so reload performance isn't affected by the map size.
* bn::regular_bg_tiles_ptr::reload_tiles_ref() and bn::affine_bg_tiles_ptr::reload_tiles_ref() copy *all* tiles
  to VRAM even if the map is big, so you should avoid calling them if the map has enough unique tiles.

### How can I draw on top of background tiles?

Butano doesn't provide an easy way to do it, so you have to allocate tiles in VRAM with
bn::regular_bg_tiles_ptr::allocate or bn::affine_bg_tiles_ptr::allocate and draw on the the allocated tiles
returned by bn::regular_bg_tiles_ptr::vram or bn::affine_bg_tiles_ptr::vram. Remember that you can't
write to VRAM in byte-sized chunks.

### Why my dynamic map sometimes doesn't work as expected?

If you don't want to have extra headaches when dealing with a map by yourself, remember to disable
background tiles offset before creating the tiles used by the map with bn::bg_tiles::set_allow_offset.

You can re-enable it after creating the tiles to improve VRAM usage again.

### How can I disable wrapping in a regular background?

You can't, it's a hardware limitation (the GBA doesn't allow it).

There are several workarounds, including:
* Using a larger image with a transparent border around the background to keep the wrapping off-screen.
* Using a window to mask away the background outside of a certain rectangular area on the screen.
* Using an affine background, which allows to disable wrapping.

### Why can't I import a regular background with 1024 or less tiles?

If you get this error when trying to import a regular background with 1024 or less tiles:

`error: Regular BGs with more than 1024 tiles not supported: 1025`

Or you get this error when importing an affine background with 256 or less tiles:

`error: Affine BGs with more than 256 tiles not supported: 257`

Your image is fine, but [grit](https://github.com/devkitPro/grit)
(the tool used by Butano to import images) is generating unneeded extra tiles.

The only workaround that I know of is reducing detail in your input image until the tiles count of
the generated background is valid.

However, if you are working with unique tiles to generate a dynamic background,
importing background tiles only instead of a background with map should work.
Check import_regular_bg_tiles, import_affine_bg_tiles, the `dynamic_regular_bg` example and
the `dynamic_affine_bg` example for that.

### Does Butano support metatiles?

Metatiles (groups of regular tiles that together represent one single bigger tile) are not supported by Butano.

### Does hiding a background make it count towards the total number of allowed backgrounds?

Hidden backgrounds are not committed to the GBA, but they still take resources like color palettes and VRAM.

The easiest way to increase the maximum number of hidden backgrounds allowed for a particular project is
to set it in the `USERFLAGS` of its `Makefile`.

For example, to set the maximum number of hidden backgrounds to 8:

`USERFLAGS := -DBN_CFG_BGS_MAX_ITEMS=8`

Remember to rebuild your project from scratch after modifying a `Makefile` (`make clean` before `make`).

## Audio

### Why the game crashes when some Direct Sound songs are played?

By default, Butano uses the excellent [Maxmod](https://blocksds.skylyrac.net/maxmod/index.html)
library for Direct Sound audio support.

It provides impressive performance and support for lots of module music formats,
but unfortunately it crashes with some songs.

Sometimes it helps to change song's file format (for example, from `*.xm` to `*.it`).
You can use [OpenMPT](https://openmpt.org/) to do that.

Keep in mind that the Maxmod's conversion tool provided by [BlocksDS](https://blocksds.skylyrac.net)
is more robust than the one provided by [devkitARM](https://devkitpro.org/), so using Butano with
[Wonderful Toolchain](https://wonderful.asie.pl/) could make your song work.

If your song still crashes after trying [Wonderful Toolchain](https://wonderful.asie.pl/),
you can try to create a new issue in the [Maxmod's GitHub issues](https://github.com/blocksds/sdk/issues)
page.

### Why can't I use a long *.wav file as music?

[Maxmod](https://blocksds.skylyrac.net/maxmod/index.html) doesn't allow to play
long `*.wav` files as music, unfortunately.

The maximum length of sound samples is also small, so if you try to play a long `*.wav` file as a sound effect,
it should stop after a few seconds.

What you can do is to split your long `*.wav` file in small pieces and put them in a module file
(files with `*.mod`, `*.xm`, `*.s3m` and `*.it` extensions).
You can use [SoX](https://sox.sourceforge.net/) to split the long `*.wav` file and
[OpenMPT](https://openmpt.org/) to create the final module file.

You also can use [GBA Wav to S3m Converter](https://github.com/Fralacticus/gba-wav-to-s3m-converter),
a tool that generates a `*.s3m` file from a long `*.wav` automatically.

### Why are there missing notes when playing some Direct Sound songs?

If a song doesn't have more channels than the maximum number of active Direct Sound music channels
specified by BN_CFG_AUDIO_MAX_MUSIC_CHANNELS, as before,
sometimes it helps to change its file format (for example, from `*.xm` to `*.it`).
You can use [OpenMPT](https://openmpt.org/) to do that.

### If two songs share a sample, is it stored twice in ROM?

[Maxmod](https://blocksds.skylyrac.net/maxmod/index.html) should be able to deduplicate the same samples
across different songs.

### How can I improve Direct Sound audio quality?

If you have some free CPU left, you can increase Direct Sound audio mixing rate to improve its quality.

The easiest way to specify Direct Sound audio mixing rate for a particular project is
to set it in the `USERFLAGS` of its `Makefile`.

For example, to set Direct Sound audio mixing rate to 21KHz:

`USERFLAGS := -DBN_CFG_AUDIO_MIXING_RATE=BN_AUDIO_MIXING_RATE_21_KHZ`

Remember to rebuild your project from scratch after modifying a `Makefile` (`make clean` before `make`).

Available Direct Sound mixing rates are here.

## Flash carts

### Why my game runs fine on emulators but doesn't work on a real GBA with a flash cart?

Emulators usually initialize SRAM with zeros, while some flash carts don't.
You should check if SRAM is formatted and format it if it isn't. The `sram` example shows how to do it.

Also, some flash carts allow to improve commercial games with patches like `saver patch`, `enable restart`,
`enable real time save`, etc. These patches can break homebrew games, so try to disable some or all of them
if you run into any issues.

### Why SRAM works on emulators but doesn't work with this old flash cart?

While SRAM works out-of-the-box with most modern flash carts, it can fail with some older ones.

To fix it you can try to:
* Set save type as SRAM.
* Disable or enable save patches.
* Update the firmware of the flash cart.
