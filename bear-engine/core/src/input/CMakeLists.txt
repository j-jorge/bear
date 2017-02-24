set( INPUT_TARGET_NAME bear_input )

#-------------------------------------------------------------------------------
set( INPUT_SOURCE_FILES
  code/controller_button.cpp
  code/display_projection.cpp
  code/finger.cpp
  code/finger_event.cpp
  code/finger_status.cpp
  code/input_listener.cpp
  code/input_status.cpp
  code/joystick.cpp
  code/joystick_button.cpp
  code/joystick_status.cpp
  code/key_event.cpp
  code/key_info.cpp
  code/keyboard.cpp
  code/keyboard_status.cpp
  code/mouse.cpp
  code/mouse_status.cpp
  code/system.cpp
)

add_library(
  ${INPUT_TARGET_NAME}
  ${BEAR_ENGINE_CORE_LINK_TYPE}
  ${INPUT_SOURCE_FILES}
  )

set_target_properties(
  ${INPUT_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_ENGINE_INSTALL_LIBRARY_DIR}"
  )

install(
  TARGETS ${INPUT_TARGET_NAME}
  DESTINATION ${BEAR_ENGINE_INSTALL_LIBRARY_DIR}
  )

set(
  INPUT_LINK_LIBRARIES
  ${SDL2_LIBRARY}
  ${CLAW_LOGGER_LIBRARIES}
)

if(WIN32 OR APPLE)
  set(
    INPUT_LINK_LIBRARIES
    ${INPUT_LINK_LIBRARIES}
    intl
    )
endif(WIN32 OR APPLE)

target_link_libraries(
  ${INPUT_TARGET_NAME}
  ${INPUT_LINK_LIBRARIES}
  )
