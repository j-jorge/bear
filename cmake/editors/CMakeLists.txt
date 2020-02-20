set(BEAR_FACTORY_EXPORT bear-factory)

if(NOT BF_IMAGES_PATH)
  set(BF_IMAGES_PATH "${CMAKE_INSTALL_DATADIR}/bear-factory/images")
endif()

function(add_bear_factory_application)
  cmake_parse_arguments(ARG
    ""
    "NAME"
    "FILES;LINK"
    ${ARGN}
    )

  set(target_name "bf-${ARG_NAME}")
  set(module_root "${BEAR_ROOT}/bear-factory/${ARG_NAME}")
  set(include_root "${module_root}/include")
  set(source_root "${module_root}/src")

  prepend_string(files "${source_root}/" ${ARG_FILES})
  add_executable(${target_name} ${files})

  target_include_directories(
    ${target_name} PRIVATE $<BUILD_INTERFACE:${include_root}>
    )

  install(
    TARGETS ${target_name}
    EXPORT ${BEAR_FACTORY_EXPORT}
    )

  target_link_libraries(${target_name} PUBLIC ${ARG_LINK})
endfunction()

add_subdirectory(bear-editor)
add_subdirectory(animation-editor)
add_subdirectory(level-editor)
add_subdirectory(model-editor)

add_subdirectory(tools)
add_subdirectory(desktop)

add_subdirectory(package)
