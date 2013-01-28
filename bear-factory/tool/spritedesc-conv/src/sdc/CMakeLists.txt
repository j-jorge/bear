cmake_minimum_required(VERSION 2.6)
project(spritedesc-conv)

include_directories(.)

#-------------------------------------------------------------------------------
set( SDC_EXECUTABLE_NAME spritedesc-conv )

#-------------------------------------------------------------------------------
set( SDC_SOURCE_FILES
  code/application.cpp
  code/image_generator.cpp
  code/main.cpp
  code/makefile_generator.cpp
  code/parser.cpp
  code/spritedesc.cpp
  code/xcf_map.cpp
  code/working_directory.cpp

  node_parser/code/node_parser.cpp
  node_parser/code/node_parser_file.cpp
  node_parser/code/node_parser_layer_to_sprite.cpp
  node_parser/code/node_parser_sprite_declaration.cpp
  node_parser/code/node_parser_sprite_description_base.cpp
  node_parser/code/node_parser_sprite_sheet.cpp
  node_parser/code/node_parser_xcf_entry.cpp
  )

add_executable(
  ${SDC_EXECUTABLE_NAME}
  ${SDC_SOURCE_FILES}
  )
target_link_libraries(
  ${SDC_EXECUTABLE_NAME}
  ${CLAW_APPLICATION_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  )

install(
  FILES ${SDC_EXECUTABLE_NAME}
  DESTINATION ${BEAR_FACTORY_INSTALL_EXECUTABLE_DIR}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
  GROUP_READ GROUP_EXECUTE
  WORLD_READ WORLD_EXECUTE )
