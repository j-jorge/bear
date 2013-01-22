cmake_minimum_required(VERSION 2.6)
project(running-bear)

set( RUNNING_BEAR_TARGET_NAME running-bear )

include_directories( . ${BEAR_ENGINE_INCLUDE_DIRECTORY} )

#-------------------------------------------------------------------------------
set( RUNNING_BEAR_SOURCE_FILES
  code/running_bear.cpp
  code/main.cpp
  )

add_executable( ${RUNNING_BEAR_TARGET_NAME} ${RUNNING_BEAR_SOURCE_FILES} )

set_target_properties(
  ${RUNNING_BEAR_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_ENGINE_INSTALL_LIBRARY_DIR}"
  )

if( NOT WIN32 )
  set_target_properties(
    ${RUNNING_BEAR_TARGET_NAME}
    PROPERTIES
    COMPILE_DEFINITIONS "BEAR_TEXT_DOMAIN_PATH=${CMAKE_INSTALL_PREFIX}/${TRANSLATIONS_INSTALL_DIR}"
    )
endif( NOT WIN32 )

install(
  TARGETS ${RUNNING_BEAR_TARGET_NAME}
  DESTINATION ${BEAR_ENGINE_INSTALL_EXECUTABLE_DIR}
  )

set(
  RUNNING_BEAR_LINK_LIBRARIES
  bear_engine
  ${CLAW_APPLICATION_LIBRARIES}
  )

if(WIN32)
  set(
    RUNNING_BEAR_LINK_LIBRARIES
    ${RUNNING_BEAR_LINK_LIBRARIES}
    ${SDL_LIBRARY}
    intl )
endif(WIN32)

# FIXME: it would be even better to use "sdl-config"
if(APPLE)
  set(
    RUNNING_BEAR_LINK_LIBRARIES
    ${RUNNING_BEAR_LINK_LIBRARIES}
    ${SDL_LIBRARY}
    SDLmain
    intl )
endif(APPLE)

target_link_libraries(
  ${RUNNING_BEAR_TARGET_NAME}
  ${RUNNING_BEAR_LINK_LIBRARIES}
  )
