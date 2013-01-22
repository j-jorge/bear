set( GUI_TARGET_NAME bear_gui )

#-------------------------------------------------------------------------------
set( GUI_SOURCE_FILES
  code/button.cpp
  code/callback.cpp
  code/callback_group.cpp
  code/checkable.cpp
  code/checkbox.cpp
  code/frame.cpp
  code/horizontal_flow.cpp
  code/multi_page.cpp
  code/picture.cpp
  code/radio_button.cpp
  code/radio_group.cpp
  code/scene_element.cpp
  code/static_text.cpp
  code/text_input.cpp
  code/visual_component.cpp
)

add_library(
  ${GUI_TARGET_NAME}
  ${BEAR_ENGINE_CORE_LINK_TYPE}
  ${GUI_SOURCE_FILES}
  )

set_target_properties(
  ${GUI_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_ENGINE_INSTALL_LIBRARY_DIR}"
  )

install(
  TARGETS ${GUI_TARGET_NAME}
  DESTINATION ${BEAR_ENGINE_INSTALL_LIBRARY_DIR}
  )

target_link_libraries(
  ${GUI_TARGET_NAME}
  bear_input
  bear_visual
)
