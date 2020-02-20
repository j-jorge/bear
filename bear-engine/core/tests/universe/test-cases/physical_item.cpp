#include "bear/universe/physical_item.hpp"
#include "bear/universe/world.hpp"
#include "bear/universe/forced_movement/forced_rotation.hpp"
#include "bear/universe/link/link.hpp"

#define BOOST_TEST_MODULE bear::universe::physical_item
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( forced_movement )
{
  bear::universe::physical_item item;

  BOOST_CHECK( !item.has_forced_movement() );

  item.set_forced_movement( bear::universe::forced_rotation() );
  BOOST_CHECK( item.has_forced_movement() );

  item.clear_forced_movement();
  BOOST_CHECK( !item.has_forced_movement() );
}

BOOST_AUTO_TEST_CASE( movement_reference )
{
  bear::universe::physical_item item1;
  bear::universe::physical_item item2;

  BOOST_CHECK( item1.get_movement_reference() == nullptr );

  item1.set_movement_reference( &item2 );
  BOOST_CHECK_EQUAL( item1.get_movement_reference(), &item2 );

  item1.set_movement_reference( nullptr );
  BOOST_CHECK( item1.get_movement_reference() == nullptr );

  bear::universe::forced_rotation rotation;
  rotation.set_reference_point_on_center( item2 );
  item1.set_forced_movement( rotation );
  
  BOOST_CHECK_EQUAL( item1.get_movement_reference(), &item2 );
}

BOOST_AUTO_TEST_CASE( age )
{
  bear::universe::physical_item item;

  BOOST_CHECK_EQUAL( item.get_age(), 0 );

  item.time_step( 1 );
  BOOST_CHECK_EQUAL( item.get_age(), 1 );

  item.time_step( 0.5 );
  BOOST_CHECK_EQUAL( item.get_age(), 1.5 );
}

BOOST_AUTO_TEST_CASE( owner )
{
  bear::universe::physical_item item;
  BOOST_CHECK( !item.has_owner() );

  bear::universe::world world( bear::universe::size_box_type( 1000, 1000 ) );
  item.set_owner( world );
  BOOST_CHECK( item.has_owner() );
  BOOST_CHECK_EQUAL( &item.get_owner(), &world );

  item.set_forced_movement( bear::universe::forced_rotation() );
  bear::universe::item_handle handle( item );
  bear::universe::const_item_handle const_handle( item );

  bear::universe::physical_item item2;
  new bear::universe::link( item, item2, 10, 20, 34 );
  item.quit_owner();
  BOOST_CHECK( !item.has_owner() );
  BOOST_CHECK( !item.has_forced_movement() );
  BOOST_CHECK( handle.get() == nullptr );
  BOOST_CHECK( const_handle.get() == nullptr );
  BOOST_CHECK( item.links_begin() == item.links_end() );

  bear::universe::world world2( bear::universe::size_box_type( 1000, 1000 ) );
  item.set_owner( world2 );
  BOOST_CHECK( item.has_owner() );
  BOOST_CHECK_EQUAL( &item.get_owner(), &world2 );
}
