#include "bear/universe/friction_rectangle.hpp"
#include "bear/universe/world.hpp"

#define BOOST_TEST_MODULE bear::universe::world
#include <boost/test/included/unit_test.hpp>

namespace test
{
  static const bear::universe::size_box_type g_world_size( 1000, 1000 );
}

BOOST_AUTO_TEST_CASE( default_friction )
{
  bear::universe::world world( test::g_world_size );
  world.set_default_friction( 16 );
  
  BOOST_CHECK_EQUAL
    ( world.get_average_friction
      ( bear::universe::rectangle_type( 10, 10, 20, 20 ) ),
        16 );
}

BOOST_AUTO_TEST_CASE( in_one_friction_rectangle )
{
  bear::universe::world world( test::g_world_size );
  world.set_default_friction( 1 );
  
  world.add_friction_rectangle
    ( bear::universe::rectangle_type( 10, 10, 20, 20 ), 2 );
      
  BOOST_CHECK_EQUAL
    ( world.get_average_friction
      ( bear::universe::rectangle_type( 10, 10, 20, 20 ) ),
        2 );
}

BOOST_AUTO_TEST_CASE( in_two_friction_rectangles )
{
  bear::universe::world world( test::g_world_size );
  world.set_default_friction( 1 );
  
  world.add_friction_rectangle
    ( bear::universe::rectangle_type( 10, 10, 20, 20 ), 2 );
  world.add_friction_rectangle
    ( bear::universe::rectangle_type( 20, 10, 30, 20 ), 4 );
      
  BOOST_CHECK_EQUAL
    ( world.get_average_friction
      ( bear::universe::rectangle_type( 15, 12, 25, 17 ) ),
        3 );
}

BOOST_AUTO_TEST_CASE( partially_in_one_friction_rectangle )
{
  bear::universe::world world( test::g_world_size );
  world.set_default_friction( 1 );
  
  world.add_friction_rectangle
    ( bear::universe::rectangle_type( 10, 10, 20, 20 ), 2 );
      
  BOOST_CHECK_EQUAL
    ( world.get_average_friction
      ( bear::universe::rectangle_type( 5, 10, 15, 20 ) ),
        1.5 );
}

BOOST_AUTO_TEST_CASE( changing_friction_rectangle )
{
  bear::universe::world world( test::g_world_size );
  world.set_default_friction( 1 );
  
  bear::universe::friction_rectangle* const r
    ( world.add_friction_rectangle
      ( bear::universe::rectangle_type( 10, 10, 20, 20 ), 2 ) );

  r->rectangle = bear::universe::rectangle_type( 5, 10, 15, 20 );
  r->friction = 12;
  
  BOOST_CHECK_EQUAL
    ( world.get_average_friction
      ( bear::universe::rectangle_type( 5, 10, 15, 20 ) ),
        12 );
}

