#include "universe/force_rectangle.hpp"
#include "universe/world.hpp"

#define BOOST_TEST_MODULE bear::universe::world
#include <boost/test/included/unit_test.hpp>

namespace test
{
  static const bear::universe::size_box_type g_world_size( 1000, 1000 );
}

BOOST_AUTO_TEST_CASE( in_one_force_rectangle )
{
  bear::universe::world world( test::g_world_size );
  
  world.add_force_rectangle
    ( bear::universe::rectangle_type( 10, 10, 20, 20 ),
      bear::universe::force_type( 3, 2 ) );
      
  const bear::universe::force_type f
    ( world.get_average_force
      ( bear::universe::rectangle_type( 10, 10, 20, 20 ) ) );

  BOOST_CHECK_EQUAL( f.x, 3 );
  BOOST_CHECK_EQUAL( f.y, 2 );
}

BOOST_AUTO_TEST_CASE( in_two_force_rectangles )
{
  bear::universe::world world( test::g_world_size );
  
  world.add_force_rectangle
    ( bear::universe::rectangle_type( 10, 10, 20, 20 ),
      bear::universe::force_type( 3, 2 ) );
  world.add_force_rectangle
    ( bear::universe::rectangle_type( 20, 10, 30, 20 ),
      bear::universe::force_type( 4, 3 ) );
      
  const bear::universe::force_type f
    ( world.get_average_force
      ( bear::universe::rectangle_type( 15, 12, 25, 17 ) ) );
    
  BOOST_CHECK_EQUAL( f.x, 3.5 );
  BOOST_CHECK_EQUAL( f.y, 2.5 );
}

BOOST_AUTO_TEST_CASE( partially_in_one_force_rectangle )
{
  bear::universe::world world( test::g_world_size );
  
  world.add_force_rectangle
    ( bear::universe::rectangle_type( 10, 10, 20, 20 ),
      bear::universe::force_type( 3, 2 ) );
      
  const bear::universe::force_type f
    ( world.get_average_force
      ( bear::universe::rectangle_type( 5, 10, 15, 20 ) ) );

  BOOST_CHECK_EQUAL( f.x, 1.5 );
  BOOST_CHECK_EQUAL( f.y, 1 );
}

BOOST_AUTO_TEST_CASE( changing_force_rectangle )
{
  bear::universe::world world( test::g_world_size );
  
  bear::universe::force_rectangle* const r
    ( world.add_force_rectangle
      ( bear::universe::rectangle_type( 10, 10, 20, 20 ),
        bear::universe::force_type( 3, 2 ) ) );

  r->rectangle = bear::universe::rectangle_type( 5, 10, 15, 20 );
  r->force = bear::universe::force_type( 6, 8 );
  
  const bear::universe::force_type f
    ( world.get_average_force
      ( bear::universe::rectangle_type( 5, 10, 15, 20 ) ) );

  BOOST_CHECK_EQUAL( f.x, 6 );
  BOOST_CHECK_EQUAL( f.y, 8 );
}

