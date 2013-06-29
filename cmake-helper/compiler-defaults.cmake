if(CMAKE_COMPILER_IS_GNUCXX)
  if(NOT WIN32 AND NOT APPLE)
    add_definitions(
      -Wall
      -ansi
      -pedantic
      -Wmissing-field-initializers
      -Wuninitialized
      )
  endif(NOT WIN32 AND NOT APPLE)
endif(CMAKE_COMPILER_IS_GNUCXX)

IF( BEAR_PROFILE )
  add_definitions( "-DBEAR_PROFILE" )
ENDIF( BEAR_PROFILE )

IF( CLAW_SOFT_ASSERT )
  add_definitions( "-DCLAW_SOFT_ASSERT" )
ENDIF( CLAW_SOFT_ASSERT )

IF( CMAKE_BUILD_TYPE STREQUAL "release" )
  add_definitions( "-DNDEBUG" )
ENDIF()
