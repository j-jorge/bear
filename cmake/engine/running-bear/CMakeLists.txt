set(target_name running-bear)

set(target_root ${BEAR_ROOT}/bear-engine/running_bear)
set(source_root ${target_root}/src)

add_executable(
  ${target_name}
  ${source_root}/main.cpp
  ${source_root}/running_bear.cpp
  )

target_include_directories(
  ${target_name}
  PRIVATE
  ${target_root}/include
  )

if(NOT WIN32)
  target_compile_definitions(
    ${target_name}
    PRIVATE
    "BEAR_TEXT_DOMAIN_PATH=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALEDIR}"
    )
endif()

target_link_libraries(
  ${target_name}
  PRIVATE
  bear_engine
  )

install(
  TARGETS ${target_name}
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
