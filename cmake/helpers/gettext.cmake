# Replaces the gettext_create_translations() macro to allow to specify the
# target name. So we can have translations in several subdirectories.
#
# The macro creates an install rule that put the compiled files in the
# subdirectories of ${CMAKE_INSTALL_LOCALEDIR}.
#
# \param _targetName The name of the target that builds the translations.
# \param _potFile The .po template file from which the translations are built.
# \param _firstPoFile (and following arguments) The .po files to compile
#
macro(gettext_create_translations_custom _targetName _potFile _firstPoFile)

   set(_gmoFiles)
   get_filename_component(_potBasename ${_potFile} NAME_WE)
   get_filename_component(_absPotFile ${_potFile} ABSOLUTE)

   foreach(_currentPoFile ${_firstPoFile} ${ARGN})
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
       DESTINATION "${CMAKE_INSTALL_LOCALEDIR}/${_lang}/LC_MESSAGES"
       RENAME ${_potBasename}.mo
       )
      
     set(_gmoFiles ${_gmoFiles} ${_gmoFile})
   endforeach()

   add_custom_target(${_targetName} ALL DEPENDS ${_gmoFiles})
endmacro()
