add_bear_engine_library(
  NAME
  text_interface

  FILES
  "argument_converter.cpp"
  "auto_converter.cpp"
  "base_exportable.cpp"
  "converted_argument.cpp"
  "string_to_arg.cpp"

  LINK
  claw_logger
  )
