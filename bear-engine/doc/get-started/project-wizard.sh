#!/bin/bash

# Builds and install the Bear Engine.
# \param $1 The path to the engine's repository.
#
function build_engine()
{
    cd "$1"
    cmake . -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=/usr
    make
    sudo make install
} # build_engine()

# If the name of the project to create is not provided on the command line, we
# ask it to the user.
PROJECT_NAME="$1"

if [ -z "$PROJECT_NAME" ]
then
    read -p "Enter your project's name: " PROJECT_NAME
fi

# We are going to create a directory for the user's project. If we are in a
# subdirectory of a cloned repository of the bear engine, then we create the
# directory where we are. Otherwise we will create this directory alongside a
# clone of the repository in a directory created where we are.

BEAR_ROOT="$(git rev-parse --show-toplevel 2>/dev/null)"

if [ $? -eq 0 ]
then
    PROJECT_ROOT=$PWD
else
    PROJECT_ROOT="$PWD/$PROJECT_NAME-project"

    if [ ! -d "$PROJECT_ROOT" ]
    then
        mkdir --parents "$PROJECT_ROOT" || exit 1
    fi

    pushd "$PROJECT_ROOT" > /dev/null

    git clone https://github.com/j-jorge/bear.git
    BEAR_ROOT="$PWD/bear"

    build_engine "$BEAR_ROOT"

    popd
fi

# Whatever the project's root is, we go inside to create the its specific files.
pushd "$PROJECT_ROOT" > /dev/null

mkdir --parents "$PROJECT_NAME"

# The project is initialized with the files of the sample project provided with
# this script.
SCRIPT_ORIGIN_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cp --recursive --no-clobber "$SCRIPT_ORIGIN_DIR/project-sample/"* \
    "$PROJECT_NAME/"

# The path to the Bear Engine and the name of the project must be updated in
# the CMake file.
sed --in-place \
    "s:YourProjectName:$PROJECT_NAME:;s:path/to/bear/repository:$BEAR_ROOT:" \
    "$PROJECT_NAME/CMakeLists.txt"

pushd "$PROJECT_NAME" > /dev/null

cmake . -DCMAKE_BUILD_TYPE=debug
make
./"$PROJECT_NAME"