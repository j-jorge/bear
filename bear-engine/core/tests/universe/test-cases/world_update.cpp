#include "bear/universe/world.hpp"
#include "bear/universe/forced_movement/forced_tracking.hpp"

#include "test/universe/item_call_tracker.hpp"

#define BOOST_TEST_MODULE bear::universe::world/update
#include <boost/test/included/unit_test.hpp>

#include <array>

namespace test
{
  static const bear::universe::size_box_type g_world_size( 1000, 1000 );
  static const bear::universe::world::region_type g_update_region =
    []() -> bear::universe::world::region_type
  {
    bear::universe::world::region_type region;
    region.push_back( bear::universe::rectangle_type( 0, 0, 1000, 1000 ) );
    return region;
  }();
}


BOOST_AUTO_TEST_CASE( progress_order_movement_reference )
{
  test::universe::item_function_call result;
  test::universe::item_call_tracker item1( result );
  test::universe::item_call_tracker item2( result );

  bear::universe::world world( test::g_world_size );
  
  world.register_item( &item1 );
  world.register_item( &item2 );

  item1.set_movement_reference( &item2 );
  world.progress_entities( test::g_update_region, 1 );

  test::universe::item_function_call expected;
  expected.time_step.push_back( &item2 );
  expected.time_step.push_back( &item1 );
  expected.move = expected.time_step;
  
  BOOST_CHECK_EQUAL_COLLECTIONS
    ( result.time_step.begin(), result.time_step.end(),
      expected.time_step.begin(), expected.time_step.end() );
  BOOST_CHECK_EQUAL_COLLECTIONS
    ( result.move.begin(), result.move.end(),
      expected.move.begin(), expected.move.end() );
}

BOOST_AUTO_TEST_CASE( progress_order_dependent_items )
{
  test::universe::item_function_call result;
  test::universe::item_call_tracker item1( result );
  test::universe::item_call_tracker item2( result );

  bear::universe::world world( test::g_world_size );
  
  world.register_item( &item1 );
  world.register_item( &item2 );

  item2.add_dependent_item( item1 );
  
  world.progress_entities( test::g_update_region, 1 );

  test::universe::item_function_call expected;
  expected.time_step.push_back( &item2 );
  expected.time_step.push_back( &item1 );
  expected.move = expected.time_step;
  
  BOOST_CHECK_EQUAL_COLLECTIONS
    ( result.time_step.begin(), result.time_step.end(),
      expected.time_step.begin(), expected.time_step.end() );
  BOOST_CHECK_EQUAL_COLLECTIONS
    ( result.move.begin(), result.move.end(),
      expected.move.begin(), expected.move.end() );
}

BOOST_AUTO_TEST_CASE( progress_order_forced_movement )
{
  test::universe::item_function_call result;
  test::universe::item_call_tracker item1( result );
  test::universe::item_call_tracker item2( result );

  bear::universe::world world( test::g_world_size );
  
  world.register_item( &item1 );
  world.register_item( &item2 );

  bear::universe::forced_tracking movement;
  movement.set_reference_point_on_center( item2 );
  
  item1.set_forced_movement( movement );
  world.progress_entities( test::g_update_region, 1 );

  test::universe::item_function_call expected;
  expected.time_step.push_back( &item2 );
  expected.time_step.push_back( &item1 );
  expected.move = expected.time_step;
  
  BOOST_CHECK_EQUAL_COLLECTIONS
    ( result.time_step.begin(), result.time_step.end(),
      expected.time_step.begin(), expected.time_step.end() );
  BOOST_CHECK_EQUAL_COLLECTIONS
    ( result.move.begin(), result.move.end(),
      expected.move.begin(), expected.move.end() );
}


BOOST_AUTO_TEST_CASE( progress_order )
{
  test::universe::item_function_call result;
  std::array< test::universe::item_call_tracker, 5 > items =
    {
      test::universe::item_call_tracker( result ),
      test::universe::item_call_tracker( result ),
      test::universe::item_call_tracker( result ),
      test::universe::item_call_tracker( result ),
      test::universe::item_call_tracker( result )
    };

  bear::universe::world world( test::g_world_size );

  for ( test::universe::item_call_tracker& item : items )
    world.register_item( &item );

  items[ 3 ].set_movement_reference( &items[ 0 ] );

  bear::universe::forced_tracking movement;
  movement.set_reference_point_on_center( items[ 3 ] );
  items[ 1 ].set_forced_movement( movement );

  items[ 1 ].add_dependent_item( items[ 2 ] );
  items[ 4 ].set_movement_reference( &items[ 2 ] );
  
  world.progress_entities( test::g_update_region, 1 );

  test::universe::item_function_call expected;
  expected.time_step.push_back( &items[ 0 ] );
  expected.time_step.push_back( &items[ 3 ] );
  expected.time_step.push_back( &items[ 1 ] );
  expected.time_step.push_back( &items[ 2 ] );
  expected.time_step.push_back( &items[ 4 ] );
  expected.move = expected.time_step;
  
  BOOST_CHECK_EQUAL_COLLECTIONS
    ( result.time_step.begin(), result.time_step.end(),
      expected.time_step.begin(), expected.time_step.end() );
  BOOST_CHECK_EQUAL_COLLECTIONS
    ( result.move.begin(), result.move.end(),
      expected.move.begin(), expected.move.end() );
}

