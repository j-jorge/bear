function(add_bear_engine_library)
  cmake_parse_arguments(ARG
    ""
    "NAME"
    "DEFINITIONS;FILES;LINK"
    ${ARGN}
    )

  set(target_name "bear_${ARG_NAME}")
  set(module_root "${BEAR_ROOT}/bear-engine/core/modules/${ARG_NAME}")
  set(include_root "${module_root}/include")
  set(source_root "${module_root}/src")

  prepend_string(files "${source_root}/" ${ARG_FILES})
  add_library(${target_name} ${files})

  target_include_directories(
    ${target_name} PUBLIC $<BUILD_INTERFACE:${include_root}>
    )

  install(
    DIRECTORY ${include_root}/bear
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

  install(
    TARGETS ${target_name}
    EXPORT ${BEAR_ENGINE_EXPORT}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

  target_link_libraries(${target_name} PUBLIC ${ARG_LINK})

  if(ARG_DEFINITIONS)
    target_compile_definitions(${target_name} PRIVATE ${ARG_DEFINITIONS})
  endif()
endfunction()

function(add_bear_engine_interface_library)
  cmake_parse_arguments(ARG
    ""
    "NAME"
    "LINK"
    ${ARGN}
    )

  set(target_name "bear_${ARG_NAME}")
  set(module_root "${BEAR_ROOT}/bear-engine/core/modules/${ARG_NAME}")
  set(include_root "${module_root}/include")

  add_library(${target_name} INTERFACE)

  target_include_directories(
    ${target_name} INTERFACE $<BUILD_INTERFACE:${include_root}>
    )

  install(
    DIRECTORY ${include_root}/bear
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

  install(
    TARGETS ${target_name}
    EXPORT ${BEAR_ENGINE_EXPORT}
    )
  
  if(ARG_LINK)
    target_link_libraries(${target_name} INTERFACE ${ARG_LINK})
  endif()
endfunction()

add_subdirectory(audio)
add_subdirectory(charset)
add_subdirectory(concept)
add_subdirectory(communication)
add_subdirectory(expr)
add_subdirectory(i18n)
add_subdirectory(input)
add_subdirectory(net)
add_subdirectory(text-interface)
add_subdirectory(time)
add_subdirectory(universe)
add_subdirectory(visual)

add_subdirectory(debug)
add_subdirectory(gui)

add_subdirectory(engine)
