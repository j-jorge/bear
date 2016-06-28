#include "universe/collision_info.hpp"
#include "universe/world.hpp"

#include "test/universe/item_mockup.hpp"

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

BOOST_AUTO_TEST_CASE( double_collision )
{
  bear::universe::world world( test::g_world_size );
  world.set_gravity( bear::universe::force_type( 0, 0 ) );
  
  test::universe::item_mockup itemA;
  itemA.set_size( 10, 10 );
  itemA.set_center_of_mass( 100, 100 );
  itemA.set_mass( 50 );
  world.register_item( &itemA );
  
  test::universe::item_mockup itemB;
  itemB.set_size( 10, 10 );
  itemB.set_center_of_mass( 90, 90 );
  itemB.set_mass( 50 );
  world.register_item( &itemB );

  test::universe::item_mockup itemC;
  itemC.set_size( 10, 10 );
  itemC.set_center_of_mass( 110, 110 );
  itemC.set_mass( 50 );
  world.register_item( &itemC );

  itemB.time_step_impl =
    [ &itemB ]( bear::universe::time_type ) -> void
    {
      itemB.set_center_of_mass( 100, 100 );
    };

  itemC.time_step_impl =
    [ &itemC ]( bear::universe::time_type ) -> void
    {
      itemC.set_center_of_mass( 100, 100 );
    };

  itemA.collision_impl =
    []( bear::universe::collision_info& info ) -> void
    {
      info.other_item().set_center_of_mass
      ( info.other_previous_state().get_center_of_mass() );
    };

  bool collisionBA( false );
  bool collisionBC( false );
  bool collisionCA( false );

  itemB.collision_impl =
    [ & ]( bear::universe::collision_info& info ) -> void
    {
      if ( &info.other_item() == &itemA )
        collisionBA = true;
      else if ( &info.other_item() == &itemC )
        collisionBC = true;
    };
    
  itemC.collision_impl =
    [ & ]( bear::universe::collision_info& info ) -> void
    {
      if ( &info.other_item() == &itemA )
        collisionCA = true;
      else if ( &info.other_item() == &itemB )
        collisionBC = true;
    };
    
  world.progress_entities( test::g_update_region, 1 );

  BOOST_CHECK( collisionBA );
  BOOST_CHECK( collisionCA );
  BOOST_CHECK( !collisionBC );
}

BOOST_AUTO_TEST_CASE( chain_of_four )
{
  bear::universe::world world( test::g_world_size );

  test::universe::item_mockup itemA;
  itemA.set_size( 10, 10 );
  itemA.set_center_of_mass( 100, 50 );
  itemA.set_mass( 50 );
  world.register_item( &itemA );
  
  test::universe::item_mockup itemB;
  itemB.set_size( 10, 10 );
  itemB.set_center_of_mass( 110, 50 );
  itemB.set_mass( 50 );
  world.register_item( &itemB );

  test::universe::item_mockup itemC;
  itemC.set_size( 10, 10 );
  itemC.set_center_of_mass( 120, 50 );
  itemC.set_mass( 50 );
  world.register_item( &itemC );

  test::universe::item_mockup itemD;
  itemD.set_size( 10, 10 );
  itemD.set_center_of_mass( 130, 50 );
  itemD.set_mass( 50 );
  world.register_item( &itemD );

  itemA.time_step_impl =
    [ &itemA ]( bear::universe::time_type ) -> void
    {
      itemA.set_center_of_mass( 105, 100 );
    };

  itemB.time_step_impl =
    [ &itemB ]( bear::universe::time_type ) -> void
    {
      itemB.set_center_of_mass( 113, 100 );
    };

  itemC.time_step_impl =
    [ &itemC ]( bear::universe::time_type ) -> void
    {
      itemC.set_center_of_mass( 121, 100 );
    };

  itemD.time_step_impl =
    [ &itemD ]( bear::universe::time_type ) -> void
    {
      itemD.set_center_of_mass( 129, 100 );
    };

  bool collisionAB( false );
  bool collisionBC( false );
  bool collisionCD( false );

  itemA.collision_impl =
    [ &itemB, &collisionAB ]( bear::universe::collision_info& info ) -> void
    {
      info.other_item().set_center_of_mass
      ( info.other_previous_state().get_center_of_mass() );

      if ( &info.other_item() == &itemB )
        collisionAB = true;
    };
    
  itemB.collision_impl =
    [ &itemC, &collisionBC ]( bear::universe::collision_info& info ) -> void
    {
      if ( &info.other_item() == &itemC )
        collisionBC = true;
    };
    
  itemC.collision_impl =
    [ &itemB, &collisionBC ]( bear::universe::collision_info& info ) -> void
    {
      if ( &info.other_item() == &itemB )
        collisionBC = true;
    };
    
  itemD.collision_impl =
    [ &itemC, &collisionCD ]( bear::universe::collision_info& info ) -> void
    {
      info.other_item().set_center_of_mass
      ( info.other_previous_state().get_center_of_mass() );

      if ( &info.other_item() == &itemC )
        collisionCD = true;
    };
    
  world.progress_entities( test::g_update_region, 1 );

  BOOST_CHECK( collisionAB );
  BOOST_CHECK( collisionCD );
  BOOST_CHECK( !collisionBC );
}

BOOST_AUTO_TEST_CASE( successive_collisions )
{
  bear::universe::world world( test::g_world_size );
  world.set_gravity( bear::universe::force_type( 0, 0 ) );
  
  test::universe::item_mockup itemA;
  itemA.set_size( 10, 10 );
  itemA.set_center_of_mass( 100, 100 );
  itemA.set_mass( 50 );
  world.register_item( &itemA );
  
  test::universe::item_mockup itemB;
  itemB.set_size( 10, 10 );
  itemB.set_center_of_mass( 90, 90 );
  itemB.set_mass( 50 );
  world.register_item( &itemB );

  test::universe::item_mockup itemC;
  itemC.set_size( 10, 10 );
  itemC.set_center_of_mass( 110, 110 );
  itemC.set_mass( 50 );
  world.register_item( &itemC );

  itemB.time_step_impl =
    [ &itemB ]( bear::universe::time_type ) -> void
    {
      itemB.set_center_of_mass( 100, 100 );
    };

  itemA.collision_impl =
    []( bear::universe::collision_info& info ) -> void
    {
      info.other_item().set_center_of_mass( 110, 110 );
    };

  bool collisionBA( false );
  bool collisionBC( false );
  bool collisionCA( false );

  itemB.collision_impl =
    [ & ]( bear::universe::collision_info& info ) -> void
    {
      if ( &info.other_item() == &itemA )
        collisionBA = true;
      else if ( &info.other_item() == &itemC )
        collisionBC = true;
    };
    
  itemC.collision_impl =
    [ & ]( bear::universe::collision_info& info ) -> void
    {
      if ( &info.other_item() == &itemA )
        collisionCA = true;
      else if ( &info.other_item() == &itemB )
        collisionBC = true;
    };
    
  world.progress_entities( test::g_update_region, 1 );

  BOOST_CHECK( collisionBA );
  BOOST_CHECK( !collisionCA );
  BOOST_CHECK( collisionBC );
}
