if( NOT BEAR_ENGINE_CORE_LINK_TYPE )
  set( BEAR_ENGINE_CORE_LINK_TYPE "SHARED" )
endif( NOT BEAR_ENGINE_CORE_LINK_TYPE )

include_directories( ${BEAR_ENGINE_INCLUDE_DIRECTORY} )

subdirs(
  audio
  communication
  debug
  engine
  expr
  gui
  input
  net
  text_interface
  time
  universe
  visual )
