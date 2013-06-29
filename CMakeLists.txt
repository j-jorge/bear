cmake_minimum_required(VERSION 2.6)

set(
  BEAR_ENGINE_COMMON_INCLUDE_DIRECTORY
  "${CMAKE_CURRENT_SOURCE_DIR}/bear-engine/common/include"
  )
set(
  BEAR_ENGINE_INCLUDE_DIRECTORY
  "${CMAKE_CURRENT_SOURCE_DIR}/bear-engine/core/src"
  )
set(
  BEAR_ITEMS_INCLUDE_DIRECTORY
  "${CMAKE_CURRENT_SOURCE_DIR}/bear-engine/lib/src"
  )

set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-helper" )

include( gettext )
include( compiler-defaults )
include( docbook-to-man )

if( BEAR_NO_MANPAGES )
  set(BEAR_ENGINE_NO_MANPAGES true)
  set(BF_NO_MANPAGES true)
endif()

option(
  BEAR_EDITORS_ENABLED
  "Tells to compile the Bear Factory, the editors of the Bear engine"
  TRUE )

subdirs(
  bear-engine
  )

if( BEAR_EDITORS_ENABLED )
subdirs(
  bear-factory
  )
endif()
