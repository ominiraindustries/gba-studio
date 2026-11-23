# Getting started with Wonderful Toolchain

@tableofcontents

## Supported platforms

[Wonderful Toolchain](https://wonderful.asie.pl/) supports Windows, macOS and Unix-like platforms.

## Wonderful Toolchain

The first step is to download and install [Wonderful Toolchain](https://wonderful.asie.pl/)
using [this tutorial](https://wonderful.asie.pl/docs/getting-started/).

From now on, this guide assumes a Windows environment, but the steps to take for Unix-like platforms
should be similar.

@image html wt_installer.png

Once installed, open the Wonderful Toolchain Shell and run the following commands,
accepting the default options when prompted:

```cpp
pacman -S make
wf-pacman -Syu wf-tools
wf-pacman -Syu wf-tools
wf-config repo enable blocksds
wf-pacman -Syu
wf-pacman -S target-gba
wf-pacman -S blocksds-toolchain
```

@image html wt_pacman.png

## Python

To execute some of the tools needed to include image and audio files in your project,
you are going to need [Python](https://www.python.org/).

If you are going to install Python on Windows,
remember to add it to the `PATH` and to close all system console windows before installing it:

@image html python_windows.png

Both Python 2 and Python 3 are supported for now, but Python 2 is not going to be supported for too long.
Make sure you can execute Python from the Wonderful Toolchain Shell:

@image html wt_python.png

If you have an Ubuntu/Debian based system with `python3` instead of `python`, you can:
* Specify `python3` as the Python interpreter path in the `Makefile` of the project you are trying to build.
* Install `python-is-python3`: `sudo apt-get install python-is-python3`.

## Butano

Download or clone the latest Butano release from [GitHub](https://github.com/GValiente/butano)
and put it in a path without spaces or anything weird, please.

## Butano examples

Butano contains multiple examples of most aspects of the engine.

Make sure that you are able to build and run some of them:
* They are located in `/path/to/butano/examples/`.
* To build a Butano example, open the Wonderful Toolchain Shell, `cd` to the folder of the example
  you want to build and type `make -j8` if your CPU has 8 cores, `make -j16` if it has 16 and so on.

@image html wt_example_1.png

When trying to build the `sprites` example, if everything went as expected,
a `sprites.gba` file should have been generated in the `sprites` folder,
and the system console should show something like this:

@image html wt_example_2.png

## Make your own GBA game!

If you were able to build and run some examples, congrats! Now you can make your own GBA game with Butano.
To do that:
* Copy the template project located in `/path/to/butano/template/` to a path without spaces or weird characters.
* Change in the `Makefile` the path in which Butano library folder is located.
  For example, if Butano is located in `/path/to/butano/`,
  `LIBBUTANO` variable should be set to `/path/to/butano/butano`.
* If you want, you can also change ROM title, ROM code and more in the `Makefile`.
* `cd` to the new folder.
* Type `make -j#cpu_cores#` again.

If everything went as expected, a `*.gba` file should have been generated in the project folder!

Now learn how to import your assets in the game and
also take a look at the [modules](modules.html) page.
It's always a good idea to play around with the examples as they cover most of what Butano offers.
The frequently asked questions (FAQ) are worth a look too, as they answer most of the initial questions.
