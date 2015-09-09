cmake_minimum_required(VERSION 2.8)

set( BEAR_ROOT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../../" )
set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -fdiagnostics-color=always")

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
set( TARGET_NAME entity-count )
file( GLOB SOURCES *.cpp )

add_executable( ${TARGET_NAME} ${SOURCES} )
target_link_libraries( ${TARGET_NAME} ${BEAR_ENGINE_LIBRARIES} )
