add_bear_engine_library(
  NAME
  communication

  FILES
  "message.cpp"
  "messageable.cpp"
  "post_office.cpp"

  LINK
  bear_concept
  Claw::logger
  )
