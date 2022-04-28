add_bear_engine_library(
  NAME
  audio

  FILES
  "sample.cpp"
  "sdl_sample.cpp"
  "sdl_sound.cpp"
  "sound.cpp"
  "sound_effect.cpp"
  "sound_manager.cpp"

  LINK
  SDL2::mixer
  SDL2
  claw_logger
  Boost::thread
  Boost::system
  )
