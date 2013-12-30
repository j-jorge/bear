This document will guide yout into the creation of your first project
using the Bear Engine.

Creating your project using the wizard
====

So you don't want to go through all the details of the configuration,
and you are sure to have all the required dependencies installed? Then
you can run the project wizard script provided with this file in order
to build a fully functional project based on the Bear Engine:

    path/to/project-wizard.sh YourProjectName

Then jump to the section named "Building your project" to see how to
build your project!

Creating your project step by step
====

First of all, you need to get the source of the engine:

    git clone https://github.com/j-jorge/bear.git

Ensure that you have all the dependencies listed in section "What are
the other build dependencies?" of the README.md file located at the
root of the repository. Install any missing dependency.

Then enter the cloned repository in order to build and install the
engine:

    cd bear
    cmake . -DCMAKE_BUILD_TYPE=release
    make
    make install

Create a directory for your project then copy the file CMakeLists.txt
provided with this document into your project's root directoryEdit
this file to replace set the path to the directory of the cloned
repository of the bear engine (line 6) and optionally to set your
program's name (line 41).

Building your project
====

To finish, put your source files into your project's directory, enter
this directory and run CMake and make to configure and to build your
program:

    cd my/project/directory
    cmake .
    make

