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
set(
  TRANSLATIONS_INSTALL_DIR "share/locale"
)

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

# This macro checks if a version number is greater or equal to an other
# version number.
macro(VERSION_GREATER_EQUAL LEFT RIGHT RESULT)
  set(${RESULT} 0)

  if( ("${LEFT}" VERSION_EQUAL "${RIGHT}")
      OR ("${LEFT}" VERSION_GREATER "${RIGHT}") )
    set(${RESULT} 1)
  endif( ("${LEFT}" VERSION_EQUAL "${RIGHT}")
    OR ("${LEFT}" VERSION_GREATER "${RIGHT}") )
endmacro(VERSION_GREATER_EQUAL)

# replace the GETTEXT_CREATE_TRANSLATIONS macro to allow to specify the target
# name. So we can have translations in several subdirectories.
macro(GETTEXT_CREATE_TRANSLATIONS_CUSTOM _targetName _potFile _firstPoFile)

   set(_gmoFiles)
   get_filename_component(_potBasename ${_potFile} NAME_WE)
   get_filename_component(_absPotFile ${_potFile} ABSOLUTE)

   FOREACH (_currentPoFile ${_firstPoFile} ${ARGN})
     get_filename_component(_poFile ${_currentPoFile} ABSOLUTE)
     get_filename_component(_abs_PATH ${_poFile} PATH)
     get_filename_component(_lang ${_poFile} NAME_WE)
     set(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_lang}.gmo)

     # Generate the .gmo file if needed.
     add_custom_command( 
       OUTPUT ${_gmoFile}
       COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${_gmoFile} ${_poFile}
       DEPENDS ${_poFile}
       )

     install(
       FILES ${_gmoFile}
       DESTINATION "${TRANSLATIONS_INSTALL_DIR}/${_lang}/LC_MESSAGES"
       RENAME ${_potBasename}.mo
       )
      
     set(_gmoFiles ${_gmoFiles} ${_gmoFile})
   ENDFOREACH (_currentPoFile )

   add_custom_target(${_targetName} ALL DEPENDS ${_gmoFiles})

endmacro(GETTEXT_CREATE_TRANSLATIONS_CUSTOM)

# This macro search the most recent svn version of libclaw
macro(FIND_SVN_CLAW)
  # Not really optional, but we want to display a custom message if we do not
  # find the module
  find_package(libclaw)

  set( EXPECTED_CLAW_SVN_REVISION 259 )

  if( CLAW_FOUND )
    VERSION_GREATER_EQUAL( ${CLAW_VERSION} "1.7.0" CLAW_VERSION_OK )

    if( NOT CLAW_VERSION_OK )
      message( FATAL_ERROR
	"It seems that libclaw is installed on your system in a too old "
	"version."
	"Check http://libclaw.sourceforge.net/ to download the latest release."
	)
    endif( NOT CLAW_VERSION_OK )
  else( CLAW_FOUND )
    message( FATAL_ERROR
      "It seems that libclaw is not installed on your system. If your "
      "distribution does not give you an easy installation procedure, check "
      "http://libclaw.sourceforge.net/ to download it."
      )
  endif( CLAW_FOUND )
endmacro(FIND_SVN_CLAW)

if( BEAR_NO_MANPAGES )
  set(BEAR_ENGINE_NO_MANPAGES true)
  set(BF_NO_MANPAGES true)
  set(PTB_NO_MANPAGES true)
else( BEAR_NO_MANPAGES )
  # Search docbook-to-man
  find_program(DOCBOOK_TO_MAN docbook-to-man ENV)

  if(DOCBOOK_TO_MAN)
    set(DOCBOOK_TO_MAN_FOUND 1)
  endif(DOCBOOK_TO_MAN)

  # Use this macro to create man pages with docbook-to-man
  macro(DOCBOOK_CREATE_MAN _targetName _volume _first_sgml_file)
    foreach (_file ${_first_sgml_file} ${ARGN})
      get_filename_component(_absFile ${_file} ABSOLUTE)
      get_filename_component(_abs_PATH ${_absFile} PATH)
      get_filename_component(_prog ${_absFile} NAME_WE)
      set(_man_file ${CMAKE_CURRENT_BINARY_DIR}/${_prog}.${_volume})

      add_custom_command(
        OUTPUT ${_man_file}
        COMMAND ${DOCBOOK_TO_MAN} ${_absFile} > ${_man_file}
        DEPENDS ${_absFile}
        )

      install(
        FILES ${_man_file}
        DESTINATION share/man/man${_volume}
        )
      
      set(_man_files ${_man_files} ${_man_file})
    endforeach(_file)

    add_custom_target(${_targetName} ALL DEPENDS ${_man_files})
  endmacro(DOCBOOK_CREATE_MAN)
endif( BEAR_NO_MANPAGES )

if( NOT DEFINED BEAR_EDITORS_ENABLED )
  set( BEAR_EDITORS_ENABLED 1 )
endif()

subdirs(
  bear-engine
  )

if( BEAR_EDITORS_ENABLED )
subdirs(
  bear-factory
  )
endif()
