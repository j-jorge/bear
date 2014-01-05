cmake_minimum_required(VERSION 2.8)

set( BEAR_ROOT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../../" )

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}" )

subdirs(
  1-screen
  2-display
  3-sprite
  4-text
  5-keyboard
  6-mouse
  7-joystick
  8-music
  9-sound
  10-stars
)
