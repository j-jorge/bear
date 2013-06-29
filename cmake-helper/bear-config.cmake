#-------------------------------------------------------------------------------
# Source directory and build properties
#

set(
  BEAR_ENGINE_INCLUDE_DIRECTORY
  "${BEAR_ROOT_DIRECTORY}/bear-engine/core/src"
  )
set(
  BEAR_ITEMS_INCLUDE_DIRECTORY
  "${BEAR_ROOT_DIRECTORY}/bear-engine/lib/src"
  )

if( NOT DEFINED BEAR_ENGINE_LIBRARY_DIRECTORY )

  #The path to the directory containing the libraries of the Bear engine.
  set(
    BEAR_ENGINE_LIBRARY_DIRECTORY
    "${BEAR_ROOT_DIRECTORY}/bear-engine/bin"
    )
endif()

#-------------------------------------------------------------------------------
# Install properties
#

# where the engine's programs are installed
if( NOT BEAR_ENGINE_INSTALL_EXECUTABLE_DIR )
  set( BEAR_ENGINE_INSTALL_EXECUTABLE_DIR bin )
endif( NOT BEAR_ENGINE_INSTALL_EXECUTABLE_DIR )

# where the engine's libraries are installed
if( NOT BEAR_ENGINE_INSTALL_LIBRARY_DIR )
  if( WIN32 )
    set( BEAR_ENGINE_INSTALL_LIBRARY_DIR
      "${BEAR_ENGINE_INSTALL_EXECUTABLE_DIR}"
      )
  else( WIN32 )
    set( BEAR_ENGINE_INSTALL_LIBRARY_DIR lib )
  endif( WIN32 )
endif( NOT BEAR_ENGINE_INSTALL_LIBRARY_DIR )

