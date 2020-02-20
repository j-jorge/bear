if(WIN32)
  set(platform_files "time_win32.cpp")
else()
  set(platform_files "time_unix.cpp")
endif()

add_bear_engine_library(
  NAME
  time

  FILES
  "time.cpp"
  ${platform_files}
  )
