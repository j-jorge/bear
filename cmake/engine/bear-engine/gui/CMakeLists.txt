add_bear_engine_library(
  NAME
  gui

  FILES
  "button.cpp"
  "callback.cpp"
  "callback_group.cpp"
  "checkable.cpp"
  "checkbox.cpp"
  "frame.cpp"
  "horizontal_flow.cpp"
  "multi_page.cpp"
  "picture.cpp"
  "radio_button.cpp"
  "radio_group.cpp"
  "scene_element.cpp"
  "static_text.cpp"
  "text_input.cpp"
  "visual_component.cpp"
  
  LINK
  bear_input
  bear_time
  bear_visual
  )
