#include "universe/environment_rectangle.hpp"
#include "universe/world.hpp"

#define BOOST_TEST_MODULE bear::universe::world
#include <boost/test/included/unit_test.hpp>

namespace test
{
  static const bear::universe::size_box_type g_world_size( 1000, 1000 );
}

BOOST_AUTO_TEST_CASE( default_environment )
{
  bear::universe::world world( test::g_world_size );
  world.set_default_environment
    ( bear::universe::environment_type::ice_environment );

  std::set< bear::universe::environment_type > environments;
  world.get_environments
    ( bear::universe::rectangle_type( 10, 10, 20, 20 ),
      environments );
  
  BOOST_CHECK
    ( environments.find( bear::universe::environment_type::ice_environment )
      != environments.end() );
}

BOOST_AUTO_TEST_CASE( in_one_environment_rectangle )
{
  bear::universe::world world( test::g_world_size );
  world.set_default_environment
    ( bear::universe::environment_type::ice_environment );

  world.add_environment_rectangle
    ( bear::universe::rectangle_type( 10, 10, 20, 20 ),
      bear::universe::environment_type::fire_environment );

  std::set< bear::universe::environment_type > environments;
  world.get_environments
    ( bear::universe::rectangle_type( 10, 10, 20, 20 ),
      environments );
  
  BOOST_CHECK
    ( environments.find( bear::universe::environment_type::fire_environment )
      != environments.end() );
  BOOST_CHECK
    ( environments.find( bear::universe::environment_type::ice_environment )
      == environments.end() );
}

BOOST_AUTO_TEST_CASE( in_two_environment_rectangles )
{
  bear::universe::world world( test::g_world_size );
  world.set_default_environment
    ( bear::universe::environment_type::ice_environment );

  world.add_environment_rectangle
    ( bear::universe::rectangle_type( 10, 10, 20, 20 ),
      bear::universe::environment_type::fire_environment );
  world.add_environment_rectangle
    ( bear::universe::rectangle_type( 20, 10, 30, 20 ),
      bear::universe::environment_type::water_environment );

  std::set< bear::universe::environment_type > environments;
  world.get_environments
    ( bear::universe::rectangle_type( 15, 12, 25, 17 ),
      environments );
  
  BOOST_CHECK
    ( environments.find( bear::universe::environment_type::fire_environment )
      != environments.end() );
  BOOST_CHECK
    ( environments.find( bear::universe::environment_type::water_environment )
      != environments.end() );
  BOOST_CHECK
    ( environments.find( bear::universe::environment_type::ice_environment )
      == environments.end() );
}

BOOST_AUTO_TEST_CASE( partially_in_one_environment_rectangle )
{
  bear::universe::world world( test::g_world_size );
  world.set_default_environment
    ( bear::universe::environment_type::ice_environment );

  world.add_environment_rectangle
    ( bear::universe::rectangle_type( 10, 10, 20, 20 ),
      bear::universe::environment_type::fire_environment );

  std::set< bear::universe::environment_type > environments;
  world.get_environments
    ( bear::universe::rectangle_type( 5, 10, 15, 20 ),
      environments );
  
  BOOST_CHECK
    ( environments.find( bear::universe::environment_type::fire_environment )
      != environments.end() );
  BOOST_CHECK
    ( environments.find( bear::universe::environment_type::ice_environment )
      != environments.end() );
}

BOOST_AUTO_TEST_CASE( changing_environment_rectangle )
{
  bear::universe::world world( test::g_world_size );
  world.set_default_environment
    ( bear::universe::environment_type::ice_environment );

  bear::universe::environment_rectangle* const r
    ( world.add_environment_rectangle
      ( bear::universe::rectangle_type( 10, 10, 20, 20 ),
        bear::universe::environment_type::fire_environment ) );

  r->rectangle = bear::universe::rectangle_type( 5, 10, 15, 20 );
  r->environment = bear::universe::environment_type::water_environment;
  
  std::set< bear::universe::environment_type > environments;
  world.get_environments
    ( bear::universe::rectangle_type( 5, 10, 15, 20 ),
      environments );

  BOOST_CHECK
    ( environments.find( bear::universe::environment_type::water_environment )
      != environments.end() );
  BOOST_CHECK_EQUAL( environments.size(), 1 );
}

BOOST_AUTO_TEST_CASE( is_in_environment )
{
  bear::universe::world world( test::g_world_size );
  world.set_default_environment
    ( bear::universe::environment_type::ice_environment );

  world.add_environment_rectangle
    ( bear::universe::rectangle_type( 10, 10, 20, 20 ),
      bear::universe::environment_type::fire_environment );
  world.add_environment_rectangle
    ( bear::universe::rectangle_type( 0, 0, 40, 40 ),
      bear::universe::environment_type::water_environment );

  BOOST_CHECK
    ( world.is_in_environment
      ( bear::universe::position_type( 15, 15 ),
        bear::universe::environment_type::fire_environment ) );
  BOOST_CHECK
    ( world.is_in_environment
      ( bear::universe::position_type( 15, 15 ),
        bear::universe::environment_type::water_environment ) );
  BOOST_CHECK
    ( !world.is_in_environment
      ( bear::universe::position_type( 15, 15 ),
        bear::universe::environment_type::ice_environment ) );
}
