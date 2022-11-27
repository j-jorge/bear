add_executable(
  02-display
  ${BEAR_ROOT}/bear-engine/doc/examples/02-display/main.cpp
)
target_link_libraries(02-display bear_visual)
