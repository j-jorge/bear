find_path( BEAR_ENGINE_INCLUDE_DIRECTORY "engine/base_item.hpp" )
find_path( BEAR_ITEMS_INCLUDE_DIRECTORY "generic_items/ambient_sound.hpp" )

set( BEAR_ENGINE_INCLUDE
  ${BEAR_ENGINE_INCLUDE_DIRECTORY}
  ${BEAR_ITEMS_INCLUDE_DIRECTORY}
)

set( BEAR_ENGINE_LIBRARIES
  bear_generic_items
  bear_engine
  bear_audio
  bear_communication
  bear_expr
  bear_gui
  bear_input
  bear_net
  bear_text_interface
  bear_visual
  bear_time
  bear_universe
  bear_debug
)

#-------------------------------------------------------------------------------
# The Bear Engine requires several libraries. We will search them below.

#-------------------------------------------------------------------------------
# check SDL2_mixer
include( FindSDL2_mixer )

if( NOT SDL2MIXER_FOUND )
  message( FATAL_ERROR "The Bear Engine needs SDL2-mixer." )
else()
  set(
    BEAR_ENGINE_INCLUDE_DIRECTORY
    ${BEAR_ENGINE_INCLUDE_DIRECTORY}
    ${SDL2MIXER_INCLUDE_DIR}
    )
  set(
    BEAR_ENGINE_LIBRARIES
    ${BEAR_ENGINE_LIBRARIES}
    ${SDL2MIXER_LIBRARY}
  )
endif( NOT SDL2MIXER_FOUND )

#-------------------------------------------------------------------------------
# The SDL2 is used for the display and the inputs.
include( FindSDL2 )

if( NOT SDL2_FOUND )
  message( FATAL_ERROR "The Bear Engine needs the SDL2." )
else()
  set(
    BEAR_ENGINE_INCLUDE_DIRECTORY
    ${BEAR_ENGINE_INCLUDE_DIRECTORY}
    ${SDL2_INCLUDE_DIR}
    )
  set(
    BEAR_ENGINE_LIBRARIES
    ${BEAR_ENGINE_LIBRARIES}
    ${SDL2_LIBRARY}
  )
endif()

#-------------------------------------------------------------------------------
# check OpenGL
include( FindOpenGL )
  
if( NOT OPENGL_FOUND )
  message( FATAL_ERROR "The Bear Engine requires OpenGL development files." )
else()
  set(
    BEAR_ENGINE_LIBRARIES
    ${BEAR_ENGINE_LIBRARIES}
    ${OPENGL_LIBRARIES}
  )
endif()

#-------------------------------------------------------------------------------
# libclaw is used in a lot of places in the Bear Engine
find_package( libclaw 1.7.4 )

if( NOT CLAW_FOUND )
  message( FATAL_ERROR "The Bear Engine needs libclaw." )
else()
  set(
    BEAR_ENGINE_INCLUDE_DIRECTORY
    ${BEAR_ENGINE_INCLUDE_DIRECTORY}
    ${CLAW_INCLUDE_DIRECTORY}
    ${CLAW_GRAPHIC_INCLUDE_DIRECTORY}
    )
  set(
    BEAR_ENGINE_LIBRARIES
    ${BEAR_ENGINE_LIBRARIES}
    ${CLAW_APPLICATION_LIBRARIES}
    ${CLAW_DYNAMIC_LIBRARY_LIBRARIES}
    ${CLAW_GRAPHIC_LIBRARIES}
    ${CLAW_NET_LIBRARIES}
    ${CLAW_TWEEN_LIBRARIES}
  )
endif()

#-------------------------------------------------------------------------------
# Boost is used in a lot of places in the Bear Engine
find_package(
  Boost 1.42 REQUIRED COMPONENTS filesystem regex system thread
  )

if( NOT Boost_FOUND )
  message(
    FATAL_ERROR
    "The Bear Engine needs Boost.FileSystem, Boost.thread and Boost.Regex."
    )
else()
  set(
    BEAR_ENGINE_INCLUDE_DIRECTORY
    ${BEAR_ENGINE_INCLUDE_DIRECTORY}
    ${BOOST_INCLUDE_DIR}
    )
  set(
    BEAR_ENGINE_LIBRARIES
    ${BEAR_ENGINE_LIBRARIES}
    ${Boost_REGEX_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
  )
endif()

#-------------------------------------------------------------------------------
# The fonts are loaded with FreeType 
include(FindFreetype)

if( NOT FREETYPE_FOUND )
  message(
    FATAL_ERROR
    "The Bear Engine needs FreeType."
    )
else()
  set(
    BEAR_ENGINE_LIBRARIES
    ${BEAR_ENGINE_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    )
endif()

#-------------------------------------------------------------------------------
# Gettext is used for the translations
if(WIN32 OR APPLE)

  find_library( GETTEXT_LIBRARY intl )

  if( NOT GETTEXT_LIBRARY )
    message(
      FATAL_ERROR "The Bear Engine needs Gettext (libintl)."
      )
  else()
    set(
      BEAR_ENGINE_LIBRARIES
      ${BEAR_ENGINE_LIBRARIES}
      intl
      )
  endif()

endif()
