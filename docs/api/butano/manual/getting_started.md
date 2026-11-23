# Getting started

Downloading Butano and building their games and examples is easy and doesn't take too much time, pinky promise.

@tableofcontents

## GBA emulator

Before anything, it is convenient to have a GBA emulator at hand,
so you don't have to test each change you make to your project on real hardware.

For developing GBA games, [mGBA](https://mgba.io),
[NanoBoyAdvance](https://github.com/nba-emu/NanoBoyAdvance),
[Mesen](https://github.com/SourMesen/Mesen2) and the debug version of
[No$gba](https://problemkaputt.de/gba.htm) are recommended.

## Toolchain

Butano can be built on top of [devkitARM](https://devkitpro.org/)
or [Wonderful Toolchain](https://wonderful.asie.pl/). Both are compatible with the same operating systems
(Windows, macOS and Unix-like platforms), so the differences between them aren't that big:
* [No$gba](https://problemkaputt.de/gba.htm)'s exception system
  with `*.elf` files only works with [devkitARM](https://devkitpro.org/).
* [Maxmod](https://blocksds.skylyrac.net/maxmod/index.html)'s conversion tool provided by
  [BlocksDS](https://blocksds.skylyrac.net) is more robust, so using Butano with
  [Wonderful Toolchain](https://wonderful.asie.pl/) could improve the playback of your songs.
* [Wonderful Toolchain](https://wonderful.asie.pl/) offers better compatibility with some
  standard containers that use threads.

So:
* If you want to use [devkitARM](https://devkitpro.org/), go to getting_started_dka.
* If you want to use [Wonderful Toolchain](https://wonderful.asie.pl/), go to getting_started_wt.
