find_package(Boost COMPONENTS unit_test_framework REQUIRED)

function( add_boost_test )
  set( oneValueArgs SOURCE )
  set( multiValueArgs INCLUDE LINK )
  cmake_parse_arguments( ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

  get_filename_component( TEST_EXECUTABLE_NAME ${ARG_SOURCE} NAME_WE )

  add_executable( ${TEST_EXECUTABLE_NAME} ${ARG_SOURCE} )
  target_link_libraries( ${TEST_EXECUTABLE_NAME} 
    ${ARG_LINK}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
  set_target_properties(
    ${TEST_EXECUTABLE_NAME}
    PROPERTIES
    INCLUDE_DIRECTORIES "${ARG_INCLUDE}"
    )
  
  file( READ "${ARG_SOURCE}" SOURCE_FILE_CONTENTS )
  string( REGEX MATCHALL "BOOST_AUTO_TEST_CASE\\( *([A-Za-z_0-9]+) *\\)" 
    FOUND_TESTS ${SOURCE_FILE_CONTENTS} )

  foreach( HIT ${FOUND_TESTS} )
    string( REGEX REPLACE ".*\\( *([A-Za-z_0-9]+) *\\).*" "\\1"
      TEST_NAME
      ${HIT}
      )

    add_test( NAME "${TEST_EXECUTABLE_NAME}.${TEST_NAME}" 
      COMMAND ${TEST_EXECUTABLE_NAME}
      --run_test=${TEST_NAME} --catch_system_error=yes
      )
  endforeach()
endfunction()
