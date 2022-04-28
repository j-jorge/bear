add_bear_engine_library(
  NAME
  debug

  FILES
  "timing_log.cpp"
  "timelog.cpp"
  "scoped_time_measure.cpp"

  LINK
  bear_time
  claw_logger
  )
