add_executable(
  01-screen
  ${BEAR_ROOT}/bear-engine/doc/examples/01-screen/main.cpp
)
target_link_libraries(01-screen bear_visual)
