cmake_minimum_required(VERSION 2.6)
project(bear-factory)

# where the game programs are installed
if( NOT BEAR_FACTORY_INSTALL_EXECUTABLE_DIR )
  set( BEAR_FACTORY_INSTALL_EXECUTABLE_DIR bin )
endif( NOT BEAR_FACTORY_INSTALL_EXECUTABLE_DIR )

# where the engine libraries are installed
if( NOT BEAR_FACTORY_INSTALL_LIBRARY_DIR )
  if( WIN32 )
    set(
      BEAR_FACTORY_INSTALL_LIBRARY_DIR
      "${BEAR_FACTORY_INSTALL_EXECUTABLE_DIR}"
      )
  else( WIN32 )
    set( BEAR_FACTORY_INSTALL_LIBRARY_DIR lib )
  endif( WIN32 )
endif( NOT BEAR_FACTORY_INSTALL_LIBRARY_DIR )

# Directories where the executable and libraries are built
set( EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/bin" )
set( LIBRARY_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}" )

#-------------------------------------------------------------------------------
# various libraries

#-------------------------------------------------------------------------------
# Claw
FIND_SVN_CLAW()

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

#-------------------------------------------------------------------------------
# Include directories for Boost
include_directories(
  ${Boost_INCLUDE_DIR}
  )

find_package( Boost 1.35 REQUIRED COMPONENTS system filesystem )

if( NOT Boost_FOUND )
  message( FATAL_ERROR 
    "You must have boost::system and boost::filesystem libraries installed (at least 1.35)" )
endif( NOT Boost_FOUND )
add_definitions(-D BOOST_FILESYSTEM_VERSION=2)

#-------------------------------------------------------------------------------
# Link directories for Boost
link_directories(
  ${Boost_LIBRARY_DIRS}
  )

#-------------------------------------------------------------------------------
# Link directories for the game
link_directories(
  ${LIBRARY_OUTPUT_PATH}
  )

#-------------------------------------------------------------------------------
if( BEAR_ENGINE_COMMON_INCLUDE_DIRECTORY )
  include_directories( "${BEAR_ENGINE_COMMON_INCLUDE_DIRECTORY}" )
else( BEAR_ENGINE_COMMON_INCLUDE_DIRECTORY )
  include_directories( ../bear-engine/common/include )
endif( BEAR_ENGINE_COMMON_INCLUDE_DIRECTORY )

set(
  BEAR_FACTORY_EDITOR_INCLUDE_DIRECTORY
  "${CMAKE_CURRENT_SOURCE_DIR}/bear-editor/src"
  )

#-------------------------------------------------------------------------------
# check wxWidgets
if( WIN32 )
  set( wxWidgets_USE_LIBS mono )
  add_definitions( "-DwxUSE_UNICODE" )
else( WIN32 )
  set( wxWidgets_USE_LIBS xml core adv )
endif( WIN32 )

include(FindwxWidgets)

if( NOT wxWidgets_FOUND )
  message( "wxWidgets not found" )
endif( NOT wxWidgets_FOUND )

#-------------------------------------------------------------------------------
# path to the images used in the interface
if( NOT BF_IMAGES_PATH )
  set( BF_IMAGES_PATH "share/bear-factory/images" )
endif( NOT BF_IMAGES_PATH )

add_definitions( '-DBF_IMAGES_PATH="${CMAKE_INSTALL_PREFIX}/${BF_IMAGES_PATH}"' )

#-------------------------------------------------------------------------------
subdirs(
  desktop
  bear-editor
  animation-editor
  level-editor
  model-editor
  tool
  )

install(
  DIRECTORY "item-description"
  DESTINATION "share/bear-factory"
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE
  GROUP_READ
  WORLD_READ
  DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
  GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE
  PATTERN ".svn" EXCLUDE
  PATTERN "*~" EXCLUDE
)
