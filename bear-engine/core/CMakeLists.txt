set( BEAR_ENGINE_INCLUDE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/" )

subdirs( src )

option(
  TESTING_ENABLED
  "Tells to compile the unit tests."
  TRUE )

if( TESTING_ENABLED )
  subdirs( test )
endif()
