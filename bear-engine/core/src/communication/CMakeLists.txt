set( COMMUNICATION_TARGET_NAME bear_communication )

#-------------------------------------------------------------------------------
set( COMMUNICATION_SOURCE_FILES
  code/post_office.cpp
  code/message.cpp
  code/messageable.cpp
)

add_library(
  ${COMMUNICATION_TARGET_NAME}
  ${BEAR_ENGINE_CORE_LINK_TYPE}
  ${COMMUNICATION_SOURCE_FILES}
  )

set_target_properties(
  ${COMMUNICATION_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_ENGINE_INSTALL_LIBRARY_DIR}"
  )

install(
  TARGETS ${COMMUNICATION_TARGET_NAME}
  DESTINATION ${BEAR_ENGINE_INSTALL_LIBRARY_DIR}
  )

target_link_libraries(
  ${COMMUNICATION_TARGET_NAME}
  ${CLAW_LOGGER_LIBRARIES}
  )
