set( UNIVERSE_TEST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )

add_library(
  bear_test_universe
  STATIC
  helpers/item_call_tracker.cpp
  helpers/item_mockup.cpp
  )
target_include_directories(
  bear_test_universe
  PUBLIC "${UNIVERSE_TEST_INCLUDE_DIR}" "${BEAR_ENGINE_INCLUDE_DIRECTORY}"
  )

include(BoostTestHelpers)

add_boost_test(
  SOURCE test-cases/item_selection.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE test-cases/physical_item.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE test-cases/physical_item_state.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE test-cases/world_collisions.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE test-cases/world_environment.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE test-cases/world_density.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE test-cases/world_force.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE test-cases/world_friction.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE test-cases/world_items.cpp
  LINK bear_test_universe bear_universe
  )

add_boost_test(
  SOURCE test-cases/world_update.cpp
  LINK bear_test_universe bear_universe
  )
