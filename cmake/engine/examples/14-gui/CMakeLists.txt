add_executable(
  14-gui
  ${BEAR_ROOT}/bear-engine/doc/examples/14-gui/main.cpp
)
target_link_libraries(
  14-gui
  bear_gui
)
