set( NET_TARGET_NAME bear_net )

#-------------------------------------------------------------------------------
set( NET_SOURCE_FILES
  code/client.cpp
  code/connection_task.cpp
  code/server.cpp

  message/code/message.cpp

  message_listener/code/base_message_listener.cpp
  message_listener/code/message_listener.cpp
  message_listener/code/message_listener_group.cpp
)

add_library(
  ${NET_TARGET_NAME}
  ${BEAR_ENGINE_CORE_LINK_TYPE}
  ${NET_SOURCE_FILES}
  )

set_target_properties(
  ${NET_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_ENGINE_INSTALL_LIBRARY_DIR}"
  )

install(
  TARGETS ${NET_TARGET_NAME}
  DESTINATION ${BEAR_ENGINE_INSTALL_LIBRARY_DIR}
  )

target_link_libraries(
  ${NET_TARGET_NAME}
  ${CLAW_NET_LIBRARIES}
  ${Boost_THREAD_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)
