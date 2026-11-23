# Qt Creator setup guide

@tableofcontents

Qt Creator is a cross-platform integrated development environment (IDE) built for the maximum developer experience.
Qt Creator runs on Windows, Linux, and macOS desktop operating systems,
and allows developers to create applications across desktop, mobile, and embedded platforms.

Since it has lots of C++ advanced features for a free IDE, it is a good fit for Butano projects.

This is a small tutorial for setting up Butano template project in Qt Creator 4.14.
Although the guide has been made in Windows, the steps for other operating systems are similar if not the same.

## Getting started

Before messing with Qt Creator, make sure that you are able to build and run Butano projects from the system console.
The best way to achieve it is to read the
guide to download, install and start using Butano.

## Install Qt Creator

If you don't have Qt Creator installed yet, you can get it from [here](https://www.qt.io/offline-installers).

@image html qt_creator/qt_0.png

## Compilers

Once you have Qt Creator installed, add [devkitARM](https://devkitpro.org/)
or [Wonderful Toolchain](https://wonderful.asie.pl/) GCC and G++ compilers
from `Tools -> Options -> Kits -> Compilers`:

@image html qt_creator/qt_1.png

@image html qt_creator/qt_2.png

## Kit

Add a new kit with the previous [devkitARM](https://devkitpro.org/)
or [Wonderful Toolchain](https://wonderful.asie.pl/) compilers from `Tools -> Options -> Kits -> Kits`:

@image html qt_creator/qt_3.png

You can ignore the yellow triangle: it is a warning, not an error.

## Import template project

Now that you have the kit setup, import Butano template project from `File -> New File or Project...`:

@image html qt_creator/qt_4.png

@image html qt_creator/qt_5.png

@image html qt_creator/qt_6.png

@image html qt_creator/qt_7.png

## C++23

Enable C++23 support in the `template.cxxflags` file with the following line:

```cpp
-std=c++23
```

## Include paths

Add the following paths to the `template.includes` file:

```cpp
include
build
../butano/include
```

## Build settings

Disable `all` build target from `Projects -> Build Settings -> Build Steps`:

@image html qt_creator/qt_8.png

## Run settings

Add your favorite GBA emulator from `Projects -> Run Settings -> Run`:

@image html qt_creator/qt_9.png

## Build and run

Finally, build and run the template project by clicking at the bottom left arrow (the one without the roach please).

If everything went as expected, your GBA emulator of choice should show a nice black screen. Congrats!
