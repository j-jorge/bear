set(tests_root "${BEAR_ROOT}/bear-engine/core/tests/universe")
set(source_root "${tests_root}/helpers/")

add_library(
  bear_test_universe
  ${source_root}/item_call_tracker.cpp
  ${source_root}/item_mockup.cpp
  )

target_include_directories(
  bear_test_universe
  PUBLIC
  ${tests_root}/include
  )

target_link_libraries(
  bear_test_universe
  PUBLIC
  bear_universe
  )

add_boost_test(
  SOURCE ${tests_root}/test-cases/item_selection.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE ${tests_root}/test-cases/physical_item.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE ${tests_root}/test-cases/physical_item_state.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE ${tests_root}/test-cases/world_active_region.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE ${tests_root}/test-cases/world_collisions.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE ${tests_root}/test-cases/world_environment.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE ${tests_root}/test-cases/world_density.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE ${tests_root}/test-cases/world_force.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE ${tests_root}/test-cases/world_friction.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE ${tests_root}/test-cases/world_items.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE ${tests_root}/test-cases/world_update.cpp
  LINK bear_test_universe bear_universe
  )
