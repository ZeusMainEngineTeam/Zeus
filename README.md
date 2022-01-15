![Zeus Game Engine Logo](/resource/Zeus_Game_Engine_Logo.jpg)
<h3 align="center">Zeus Game Engine</h3>
<p align="center">A cross-platform game engine written in modern C++17 from <u>scratch</u>.</p>
<p align="center">
  <img src="/resource/microsoft-48.png" alt="Microsoft Windows Logo"/>
  <img src="/resource/linux-48.png" alt="Linux Logo"/>
</p>

# Summary

Welcome to the Zeus game engine!  The Zeus game engine strives to provide a modern approach to writing game engines in C++ using modern techniques. It is important to state a project's general design goals and I think below is perfect for this project.

General Decision Rationale:
  1. User friendly (well-documented and intuitive)
  2. Keep It Simple Stupid (KISS)
  3. Speed & Efficiency
  4. Extensibility

# Another Game Engine?
Yes! It has always been a dream of mine to write a cross-platform 3D game engine in C++ from scratch!  One of the main goals of this project is to create the most well-documented game engine in history.  This includes the architecture, general design decisions and of course the internal frameworks and APIs.  It is *extremely* important that everything is documented.  It should be super easy for someone to contribute/understand this project.  One of the main goals is to not close doors, but to open them as much as possible.  Nothing is a secret.

## What Do You Consider From "Scratch?"
My definition of from "scratch" is _only_ using low-level libraries provided by the OS for help.  For example X11 for creating/managing windows in Linux.  I do not mean doing everything in assembly.  I am not that crazy...probably not.

# How To Build

## Overview

Below contains everything you need to know in order to build the project.  The project is mainly developed on Linux.  That is why a lot of the examples involve CMake and makefiles.  This project's main compiler is Clang.  Of course, the project will be tested on other compilers also.

## Prerequisites

If you want to build the project in its entirely and use all of its features you will need the following:
- [CMake](https://cmake.org/) (at least version 3.22) - Cross-platform build system
- [GoogleTest](https://github.com/google/googletest) - For unit and integration testing
- [Doxygen](https://www.doxygen.nl/index.html) - To build the documentation
- [PowerShell](https://docs.microsoft.com/en-us/powershell/scripting/overview?view=powershell-7.2) - To run the build scripts
- [Valgrind](https://valgrind.org/) - Memory leak detection
- [A compiler that supports C++17](https://en.cppreference.com/w/cpp/17)

### Platform Specific

There are specific packages that need to be downloaded in order to build the project.

<details>
<summary>Windows</summary>
</details>
<details>
<summary>Linux</summary>
</details>

## Building The Project

### Overview

There are two ways to setup/configure the project.  There are scripts created with predefined settings to get the project up and running quickly. Then there is the manual way which is using CMake's GUI or changing the settings on the main `CMakeLists.txt` file located in the project's root directory.

### Using Scripts

There are many [PowerShell](https://docs.microsoft.com/en-us/powershell/scripting/overview?view=powershell-7.2) scripts created to build the project with [CMake](https://cmake.org/) using different settings located in the `build_scripts` folder.
- `release_build.ps1` - Builds the project in **release mode** with optimizations turned on
  - This will also turn off all of the logging except for INFO, WARNING and FATAL ERROR
  - This will also turn off building examples and tests.
- `debug_build.ps1` - Builds the project in **debug mode** with optimizations turned off
- `hybrid_build.ps1` - Builds the project in **hybrid mode** with optimizations turned on for certain parts of the engine

Run the `build_scripts/release_build.ps1` file to build the project with predefined settings.

```bash
# From the root directory of the project
# 'pwsh' is PowerShell
pwsh build_scripts/release_build.ps1
```

### Using CMake

#### Configuring Settings

<details>
<summary>CMake GUI</summary>

![Zeus Game Engine Logo](/resource/cmake_gui.png)
</details>
<details>
<summary>CMakeLists.txt</summary>
</details>

#### Running CMake

```bash
# From the root directory

# Create a folder called "build" to build the project in and then change to
# that directory
mkdir build && cd build

# Call CMake using CMake's default settings
cmake ..
```

</details>

### Triggering Builds

#### Overview

Now the project should be setup with your favorite build system.  Below is how to build the targets with CMake using your build system.

<details>
<summary>Makefile</summary>

```bash
# Run `make help` for targets that can be built if you are using makefiles (common on Linux)
make help

# Run 'make' in the build folder to trigger the building
make
```
</details>

### Testing

Testing is done using [GoogleTest](https://github.com/google/googletest).  There are unit tests and integration tests for the project.  The tests can be found in the `tests` folder in the root directory.

### Documentation

The documentation is inside the `docs` folder in the main directory.  The documentation is generated using [Doxygen](https://www.doxygen.nl/index.html).

#### Building

<details>
<summary>Makefile</summary>

```bash
# Build the documentation using make
make Doxygen
```
</details>

### Examples

Examples are provided to show how to use the engine located in the `examples` folder in the root directory.  The examples are dependent on the project.  Any changes to the main project will require the examples to be rebuilt.

# Platform Support

One of the *big* goals of Zeus is to support as many platforms as possible.  Currently the main focus will be on Windows & Linux.  macOS will be added soon after along with support for consoles and for mobile devices (in that order).

Supported Platforms:
- Windows
- Linux

Planned Platforms:
- macOS
- Xbox
- Playstation
- Switch
- Android
- iPhone

# Contributing

Zeus is not accepting contributors at this time.  The project needs to have a foundation before accepting contributors.  But please come back often to check to see if the engine is accepting contributors!  If you want an idea on *how someone would contribute* please review the [CONTRIBUTING](dev/CONTRIBUTING.md) before making any contributions.  This document will explain the project's directory, coding style and how to contribute. Big contributors will be added to the [CONTRIBUTORS](CONTRIBUTORS.md) document.

# Authors

* **Tristan F.** - *Main engine architect & developer* - [programmersunited](https://github.com/programmersunited)

# License

This project currently uses the GPLv3 license.  Please look at the [LICENSE](LICENSE) for more details.  This is subject to change in the future.  It could become more restrictive or more relaxed.  If the engine becomes more restrictive then a repository will be created to keep the code that was created while the project was under the GPLv3 license.

# Project Status

The project currently is in its early stages.  Changes make take a while.  Please be patient :)
