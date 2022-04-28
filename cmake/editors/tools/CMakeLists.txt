function(add_tool_application)
  cmake_parse_arguments(ARG
    ""
    "NAME"
    "FILES;LINK"
    ${ARGN}
    )

  set(target_name "${ARG_NAME}")
  set(source_root "${BEAR_ROOT}/bear-factory/tool/${ARG_NAME}/src")

  prepend_string(files "${source_root}/" ${ARG_FILES})
  add_executable(${target_name} ${files})

  target_include_directories(
    ${target_name} PUBLIC $<BUILD_INTERFACE:${source_root}>
    )

  install(
    TARGETS ${target_name}
    EXPORT ${BEAR_FACTORY_EXPORT}
    )

  target_link_libraries(${target_name} PUBLIC ${ARG_LINK})
endfunction()

add_tool_application(
  NAME
  image-cutter

  FILES
  "application.cpp"
  "main.cpp"

  LINK
  claw_application
  claw_graphic
  )

add_tool_application(
  NAME
  bend-image

  FILES
  "bezier-proj.cpp"

  LINK
  claw_graphic
  )
