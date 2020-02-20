add_bear_engine_library(
  NAME
  net

  FILES
  "client.cpp"
  "connection_task.cpp"
  "message_listener/base_message_listener.cpp"
  "message_listener/message_listener.cpp"
  "message_listener/message_listener_group.cpp"
  "message/message.cpp"
  "server.cpp"

  LINK
  Claw::net
  Boost::thread
  Boost::system
  )
