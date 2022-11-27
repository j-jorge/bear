add_executable(
  12-shader
  ${BEAR_ROOT}/bear-engine/doc/examples/12-shader/main.cpp
)
target_link_libraries(
  12-shader
  bear_visual
)
