This document will guide you through the creation of your first project
using the Bear Engine.

Installing the dependencies
====

We use CMake as the build system (http://www.cmake.org/). You will
also need a C++ compiler and the following libraries in order to build
the engine and its tools:

- Boost: http://www.boost.org,
- SDL 2: http://www.libsdl.org,
- SDL Mixer 2: http://www.libsdl.org/projects/SDL_mixer/,
- Claw: http://libclaw.sourceforge.net,
- Gettext: http://www.gnu.org/software/gettext/,
- FreeType: http://www.freetype.org/,
- wxWidgets 2.8 (for the editors): http://www.wxwidgets.org/,

Creating your project using the wizard
====

When all the dependencies are installed, you can run the project
wizard script provided with this file in order to build a fully
functional project based on the Bear Engine:

    path/to/project-wizard.sh YourProjectName

Then jump to the section named "Building your project" to see how to
build your project!

Creating your project step by step
====

First of all, you need to get the source of the engine:

    git clone --branch sdl2-port https://github.com/j-jorge/bear.git

Ensure that you have all the dependencies installed, then enter the
loned repository in order to build and install the engine:

    cd bear
    cmake . -DCMAKE_BUILD_TYPE=release
    make
    make install # as root

Create a directory for your project then copy the content of the
`sample-project` directory provided with this document into your
project's root directory. Edit this file to set the path to the
directory of the cloned repository of the bear engine (line 6) and
optionally to set your program's executable name (line 42).

Building your project
====

You can use the file `YourProjectDirectory/launcher/main.cpp` as the base
for your project, as it provides a minimal game code. When you will need
more c++ files, add them anywhere and list them in the CMakeLists.txt, in
the `set( SOURCES … )` directive line 44.

Finally, enter your project's directory and run `cmake` and `make` to
configure and to build your program:

    cd your/project/directory
    cmake .
    make

