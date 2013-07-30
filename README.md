What is this?
====

This is the Bear engine, a set of C++ libraries and tools dedicated to
creating great 2D games. It has been used to create Plee the Bear,
Andy's Super Great Park and Tunnel.

The engine provides at least the following functionalities:

- multiple layers levels,
- character models with multiple actions, multiple parts, interpolated
  movement and event handling,
- multiple collision detection regions per character,
- complex animations with loops, pre loop and post loop sequences,
- music and sound playing, with distance tone down effects,
- understandable physics with blocks, curves and areas with different
  densities,
- predefined classical movements (rotating around something, tracking
  something, going back and forth two target positions, etc.),
- simple rendering procedure supporting sprites, polygons, lines,
  scaling, rotations, color variation,
- fragment shaders at the sprite, item or layer level,
- true type fonts and bitmap fonts,
- basic scripting for cut scenes,
- easy creation and restoration of save states 
- a set of widgets for dialog interfaces,
- keyboard, mouse and joystick support.

There is also a collection of classical 2D games items provided with
the engine, like:

- decorative items (sprites, animations, flows, stars),
- cameras tracking some objects,
- pushable blocks,
- fireworks,
- springs,
- rolling credits,
- elastic links,
- etc.

The engine comes with a set of tools, namely the Bear Factory,
intended to help creating resources for the game. These tools include
a level editor, a character/model editor and an animation editor.

On what system relies the Bear engine?
====

The engine uses the SDL and OpenGL as subsystems. There is no other
back-end yet but you are welcome to propose yours :)

What are the other build dependencies?
====

We use CMake as the build system (http://www.cmake.org/). You will
also need a C++ compiler and the following libraries in order to build
the engine and its tools:

- Boost: http://www.boost.org,
- SDL: http://www.libsdl.org,
- Claw: http://libclaw.sourceforge.net,
- Gettext: http://www.gnu.org/software/gettext/,
- FreeType: http://www.freetype.org/,
- wxWidgets (for the editors): http://www.wxwidgets.org/,

How to build the project?
====

1. Enter the source directory and type the following command:

  cmake .

2. If everything went well, go to 4.
3. Grumble about missing libraries, unreadable messages and
inaccurate documentation. Fix the problems and go to 1.
4. Type the following command and wait:
 
   make

Where to get help?
====

Issues should be reported on the projects page on GitHub:
https://github.com/j-jorge/bear

For general questions, send an email to julien.jorge@stuff-o-matic.com. 

Legal information
====

The software is licenced under the version 2 of the GNU GPL. All
multimedia resources are provided under the terms of the version 3 of
the Creative Commons license, with attribution and share-alike clauses
(CC-by-sa).

See the accompanying LICENSE file for details.
