#include "bear/universe/world.hpp"

#include "test/universe/item_call_tracker.hpp"

#define BOOST_TEST_MODULE bear::universe::world
#include <boost/test/included/unit_test.hpp>

namespace test
{
  static const bear::universe::size_box_type g_world_size( 1000, 1000 );

  class active_region_item:
    public bear::universe::physical_item
  {
  public:
    void enters_active_region() override
    {
      enters_called = true;
    }

    void leaves_active_region() override
    {
      leaves_called = true;
    }

  public:
    bool enters_called = false;
    bool leaves_called = false;
  };
}

BOOST_AUTO_TEST_CASE( enters_active_region )
{
  test::active_region_item item;
  bear::universe::world world( test::g_world_size );

  item.set_center_of_mass( 10, 10 );
  world.register_item( &item );

  {
    bear::universe::world::region_type region;
    region.push_back( bear::universe::rectangle_type( 200, 200, 100, 100 ) );

    world.progress_entities( region, 1 );
  
    BOOST_CHECK( !item.enters_called );
    BOOST_CHECK( !item.leaves_called );
  }

  {
    bear::universe::world::region_type region;
    region.push_back( bear::universe::rectangle_type( 0, 0, 100, 100 ) );

    world.progress_entities( region, 1 );
  
    BOOST_CHECK( item.enters_called );
    BOOST_CHECK( !item.leaves_called );
  }

  item.enters_called = false;
  
  {
    bear::universe::world::region_type region;
    region.push_back( bear::universe::rectangle_type( 0, 0, 100, 100 ) );

    world.progress_entities( region, 1 );
  
    BOOST_CHECK( !item.enters_called );
    BOOST_CHECK( !item.leaves_called );
  }
}
