cmake_minimum_required(VERSION 2.6)
project(bend-image)

include_directories(.)

#-------------------------------------------------------------------------------
set( SDC_SOURCE_FILES
  code/bezier-proj.cpp
  )

add_executable(
  bend-image
  ${SDC_SOURCE_FILES}
  )
target_link_libraries(
  bend-image
  ${CLAW_GRAPHIC_LIBRARIES}
  )
