cmake_minimum_required(VERSION 2.8)

#-------------------------------------------------------------------------------
# Find the root directory of the source of the Bear Engine. This directory
# contain the bear-engine and bear-factory project directories.
if( NOT DEFINED BEAR_ROOT_DIRECTORY
    OR NOT IS_DIRECTORY "${BEAR_ROOT_DIRECTORY}/bear-engine" )
  message(
    FATAL_ERROR
    "I can't find the Bear Engine. Please set BEAR_ROOT_DIRECTORY to the path of the engine's root:\n  cmake . -DBEAR_ROOT_DIRECTORY=/home/joe/dev/bear\nCurrent path is \"${BEAR_ROOT_DIRECTORY}\" and it doesn't contain a bear-engine directory."
    )
endif()

# The engine comes with some CMake scripts to ease its configuration and usage.
# These scripts are in the directory below and must be assigned to
# CMAKE_MODULE_PATH in order to be found by the upcoming include() instructions
set( CMAKE_MODULE_PATH "${BEAR_ROOT_DIRECTORY}/cmake-helper" )

# This will sets the variables of the source directories, required by the CMake
# package below.
include( "bear-config" )

#-------------------------------------------------------------------------------
# Include Bear Engine's CMake package to find the libraries, the link paths and
# the and include paths required by the engine.
find_package( bear )

include_directories( ${BEAR_ENGINE_INCLUDE_DIRECTORY} )

#-------------------------------------------------------------------------------
# Now we can describe our project.
set( TARGET_NAME ex-stars )
file( GLOB SOURCES *.cpp )

add_executable( ${TARGET_NAME} ${SOURCES} )
target_link_libraries( ${TARGET_NAME} ${BEAR_ENGINE_LIBRARIES} )
