cmake_minimum_required(VERSION 2.6)

set( BEAR_ROOT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" )

set(
  BEAR_ENGINE_COMMON_INCLUDE_DIRECTORY
  "${CMAKE_CURRENT_SOURCE_DIR}/bear-engine/common/include"
  )

set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-helper" )

include( bear-config )
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
  cmake-module
  )

if( BEAR_EDITORS_ENABLED )
subdirs(
  bear-factory
  )
endif()
