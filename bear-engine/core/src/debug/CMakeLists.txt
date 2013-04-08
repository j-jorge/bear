set( DEBUG_TARGET_NAME bear_debug )

#-------------------------------------------------------------------------------
set( DEBUG_SOURCE_FILES
  code/scoped_time_measure.cpp
  code/timelog.cpp
  code/timing_log.cpp
)

add_library(
  ${DEBUG_TARGET_NAME}
  ${BEAR_ENGINE_CORE_LINK_TYPE}
  ${DEBUG_SOURCE_FILES}
  )

set_target_properties(
  ${DEBUG_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_ENGINE_INSTALL_LIBRARY_DIR}"
  )

install(
  TARGETS ${DEBUG_TARGET_NAME}
  DESTINATION ${BEAR_ENGINE_INSTALL_LIBRARY_DIR}
  )

target_link_libraries(
  ${DEBUG_TARGET_NAME}
  ${CLAW_LOGGER_LIBRARIES}
  bear_time
  )
