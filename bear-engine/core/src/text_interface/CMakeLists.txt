set( TEXT_INTERFACE_TARGET_NAME bear_text_interface )

#-------------------------------------------------------------------------------
set( TEXT_INTERFACE_SOURCE_FILES
  code/argument_converter.cpp
  code/auto_converter.cpp
  code/base_exportable.cpp
  code/converted_argument.cpp
  code/string_to_arg.cpp
  )

add_library(
  ${TEXT_INTERFACE_TARGET_NAME}
  ${BEAR_ENGINE_CORE_LINK_TYPE}
  ${TEXT_INTERFACE_SOURCE_FILES}
  )

set_target_properties(
  ${TEXT_INTERFACE_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_ENGINE_INSTALL_LIBRARY_DIR}"
  )

install(
  TARGETS ${TEXT_INTERFACE_TARGET_NAME}
  DESTINATION ${BEAR_ENGINE_INSTALL_LIBRARY_DIR}
  )

set(
  TEXT_INTERFACE_LINK_LIBRARIES
  ${CLAW_LOGGER_LIBRARIES}
)

target_link_libraries(
  ${TEXT_INTERFACE_TARGET_NAME}
  ${TEXT_INTERFACE_LINK_LIBRARIES}
  )
