set( AUDIO_TARGET_NAME bear_audio )

add_definitions( "-DUSE_RWOPS" )

#-------------------------------------------------------------------------------
set( AUDIO_SOURCE_FILES
  code/sample.cpp
  code/sdl_sample.cpp
  code/sdl_sound.cpp
  code/sound.cpp
  code/sound_effect.cpp
  code/sound_manager.cpp
)

add_library(
  ${AUDIO_TARGET_NAME}
  ${BEAR_ENGINE_CORE_LINK_TYPE}
  ${AUDIO_SOURCE_FILES}
  )

set_target_properties(
  ${AUDIO_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_ENGINE_INSTALL_LIBRARY_DIR}"
  )

install(
  TARGETS ${AUDIO_TARGET_NAME}
  DESTINATION ${BEAR_ENGINE_INSTALL_LIBRARY_DIR}
  )

target_link_libraries(
  ${AUDIO_TARGET_NAME}
  ${SDLMIXER_LIBRARY}
  ${SDL_LIBRARY}
  ${CLAW_LOGGER_LIBRARIES}
  ${Boost_THREAD_LIBRARY}
  )
