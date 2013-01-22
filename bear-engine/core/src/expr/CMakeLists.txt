set( EXPR_TARGET_NAME bear_expr )

#-------------------------------------------------------------------------------
set( EXPR_SOURCE_FILES
  code/boolean_constant.cpp
  code/boolean_expression.cpp
  code/boolean_variable.cpp
  code/linear_constant.cpp
  code/linear_expression.cpp
  code/linear_variable.cpp
  code/logical_not.cpp
  code/logical_xor.cpp
  )

add_library(
  ${EXPR_TARGET_NAME}
  ${BEAR_ENGINE_CORE_LINK_TYPE}
  ${EXPR_SOURCE_FILES}
  )

set_target_properties(
  ${EXPR_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_ENGINE_INSTALL_LIBRARY_DIR}"
  )

install(
  TARGETS ${EXPR_TARGET_NAME}
  DESTINATION ${BEAR_ENGINE_INSTALL_LIBRARY_DIR} )
