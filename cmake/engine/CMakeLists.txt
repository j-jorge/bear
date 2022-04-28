set(BEAR_ENGINE_EXPORT bear-engine)

add_subdirectory(bear-engine-common)
add_subdirectory(bear-engine)
add_subdirectory(generic-items)
add_subdirectory(desktop)

if(RUNNING_BEAR_ENABLED)
  add_subdirectory(running-bear)
endif()

add_subdirectory(package)

if(BEAR_TESTING_ENABLED)
  include(BoostTestHelpers)
  add_subdirectory(bear-engine-tests)
endif()

if(BEAR_EXAMPLES_ENABLED)
  add_subdirectory(examples)
endif()
