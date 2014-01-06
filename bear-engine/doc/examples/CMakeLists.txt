cmake_minimum_required(VERSION 2.8)

set( BEAR_ROOT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../../" )

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}" )

subdirs(
  01-screen
  02-display
  03-sprite
  04-text
  05-keyboard
  06-mouse
  07-joystick
  08-music
  09-sound
  10-stars
  11-animation
)
