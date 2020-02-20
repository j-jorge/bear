add_library(bear_engine_common INTERFACE)

set(include_root ${BEAR_ROOT}/bear-engine/common/include)

target_include_directories(
  bear_engine_common
  INTERFACE
  $<BUILD_INTERFACE:${include_root}>
  )

install(DIRECTORY ${include_root}/bear DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  TARGETS bear_engine_common
  EXPORT ${BEAR_ENGINE_EXPORT}
  )
