cmake_minimum_required(VERSION 2.6)
project(bear-engine)

# Directories where the executable and libraries are built
set( BEAR_ENGINE_EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/bin" )
set( BEAR_ENGINE_LIBRARY_OUTPUT_PATH "${BEAR_ENGINE_EXECUTABLE_OUTPUT_PATH}" )

set( EXECUTABLE_OUTPUT_PATH "${BEAR_ENGINE_EXECUTABLE_OUTPUT_PATH}" )
set( LIBRARY_OUTPUT_PATH "${BEAR_ENGINE_LIBRARY_OUTPUT_PATH}" )

# common flags
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )

#-------------------------------------------------------------------------------
# various libraries

#-------------------------------------------------------------------------------
# Claw
find_package( libclaw 1.7.4 )

#-------------------------------------------------------------------------------
# Include directories for Claw
include_directories(
  ${CLAW_INCLUDE_DIRECTORY}
  ${CLAW_GRAPHIC_INCLUDE_DIRECTORY}
  )

#-------------------------------------------------------------------------------
# Link directories for Claw
link_directories(
  ${CLAW_LINK_DIRECTORY}
  ${CLAW_GRAPHIC_LINK_DIRECTORY}
  )

#-------------------------------------------------------------------------------
# Boost
include(FindBoost)

find_package(
  Boost 1.42 REQUIRED COMPONENTS filesystem regex signals system thread
  )
if( NOT Boost_FOUND )
  message( FATAL_ERROR 
    "You must have boost::filesystem, boost::signals, boost::thread and boost::regex libraries installed (at least 1.42)" )
endif( NOT Boost_FOUND )

#-------------------------------------------------------------------------------
# Include directories for Boost
include_directories(
  ${Boost_INCLUDE_DIR}
  )

#-------------------------------------------------------------------------------
# Link directories for Boost
link_directories(
  ${Boost_LIBRARY_DIRS}
  )

#-------------------------------------------------------------------------------
# check OpenGL
if( NOT BUILD_PLATFORM STREQUAL "android" )
  include( FindOpenGL )
  
  if( NOT OPENGL_FOUND )
    message( FATAL_ERROR "OpenGL must be installed." )
  endif()
endif()

#-------------------------------------------------------------------------------
# check SDL2
include( FindSDL2 )

if( NOT SDL2_FOUND )
  message( FATAL_ERROR "SDL2 library must be installed." )
else( NOT SDL2_FOUND )
  #-----------------------------------------------------------------------------
  # Include directories for SDL
  include_directories(
    ${SDL2_INCLUDE_DIR}
    )
endif( NOT SDL2_FOUND )

#-------------------------------------------------------------------------------
# check SDL2_mixer
include( FindSDL2_mixer )

if( NOT SDL2MIXER_FOUND )
  message( FATAL_ERROR "SDL2_mixer lbrary must be installed." )
else( NOT SDL2MIXER_FOUND )
  #-----------------------------------------------------------------------------
  # Include directories for all these libraries
  include_directories(
    ${SDL2MIXER_INCLUDE_DIR}
    )
endif( NOT SDL2MIXER_FOUND )

#-------------------------------------------------------------------------------
# Link directories for the game
link_directories(
  ${LIBRARY_OUTPUT_PATH}
  )

#-------------------------------------------------------------------------------
include_directories( common/include )

set( BEAR_ENGINE_INCLUDE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/core/src" )

#-------------------------------------------------------------------------------
subdirs(
  core
  lib
  desktop
  )

option(
  RUNNING_BEAR_ENABLED
  "Tells to compile the main program of the engine."
  FALSE )

if( RUNNING_BEAR_ENABLED )
subdirs(
  running_bear
  )
endif()
