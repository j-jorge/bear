cmake_minimum_required(VERSION 3.13)

project(bear-engine)

enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/helpers")
set(BEAR_ROOT "${CMAKE_CURRENT_LIST_DIR}/../")

include(GNUInstallDirs)

include(bear-config)
include(compiler-defaults)
include(dependencies)
include(docbook-to-man)
include(gettext)
include(options)
include(prepend-string)

add_subdirectory(engine)

if(BEAR_EDITORS_ENABLED)
  add_subdirectory(editors)
endif()
