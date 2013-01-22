set( TIME_TARGET_NAME bear_time )

#-------------------------------------------------------------------------------
set( TIME_SOURCE_FILES
  code/time.cpp
)

add_library(
  ${TIME_TARGET_NAME}
  ${BEAR_ENGINE_CORE_LINK_TYPE}
  ${TIME_SOURCE_FILES}
  )

set_target_properties(
  ${TIME_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_ENGINE_INSTALL_LIBRARY_DIR}"
  )

install(
  TARGETS ${TIME_TARGET_NAME}
  DESTINATION ${BEAR_ENGINE_INSTALL_LIBRARY_DIR}
  )
