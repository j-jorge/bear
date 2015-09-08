#include "universe/world.hpp"

#include "test/universe/item_call_tracker.hpp"

#define BOOST_TEST_MODULE bear::universe::world
#include <boost/test/included/unit_test.hpp>

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

BOOST_AUTO_TEST_CASE( insert_static_is_fixed )
{
  bear::universe::physical_item item;
  bear::universe::world world( test::g_world_size );

  world.add_static( &item );
  BOOST_CHECK( item.is_fixed() );
}

BOOST_AUTO_TEST_CASE( insert_static_not_moved )
{
  bear::universe::world world( test::g_world_size );

  test::universe::item_function_call result;
  test::universe::item_call_tracker item( result );
  world.add_static( &item );

  world.progress_entities( test::g_update_region, 1 );

  BOOST_CHECK( result.move.empty() );
}
