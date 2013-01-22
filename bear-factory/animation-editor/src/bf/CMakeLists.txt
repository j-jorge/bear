cmake_minimum_required(VERSION 2.6)
project(bf-animation-editor)

set( BF_AE_TARGET_NAME bf-animation-editor )

include_directories( .. ${BEAR_FACTORY_EDITOR_INCLUDE_DIRECTORY})

#-------------------------------------------------------------------------------
set( BF_AE_SOURCE_FILES
  code/about_dialog.cpp
  code/animation_editor.cpp
  code/configuration.cpp
  code/frame_duration_dialog.cpp
  code/main.cpp
  code/main_frame.cpp
)

include(${wxWidgets_USE_FILE})

add_executable( ${BF_AE_TARGET_NAME} ${BF_AE_SOURCE_FILES} )

set_target_properties(
  ${BF_AE_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_FACTORY_INSTALL_LIBRARY_DIR}"
  )

target_link_libraries(
  ${BF_AE_TARGET_NAME}
  bear-editor
  ${wxWidgets_LIBRARIES}
  ${CLAW_CONFIGURATION_FILE_LIBRARIES}
  ${CLAW_LOGGER_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  )

install(
  TARGETS ${BF_AE_TARGET_NAME}
  DESTINATION "${BEAR_FACTORY_INSTALL_EXECUTABLE_DIR}"
  )
