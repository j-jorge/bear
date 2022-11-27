add_executable(
  10-stars
  ${BEAR_ROOT}/bear-engine/doc/examples/10-stars/main.cpp
)
target_link_libraries(
  10-stars
  bear_visual
)
