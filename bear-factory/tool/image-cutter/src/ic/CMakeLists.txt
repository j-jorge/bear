cmake_minimum_required(VERSION 2.6)
project(image-cutter)

include_directories(.)

#-------------------------------------------------------------------------------
set( IC_SOURCE_FILES
  code/application.cpp
  code/main.cpp )

add_executable(
  image-cutter
  ${IC_SOURCE_FILES}
  )
target_link_libraries(
  image-cutter
  ${CLAW_APPLICATION_LIBRARIES}
  ${CLAW_GRAPHIC_LIBRARIES}
  )
