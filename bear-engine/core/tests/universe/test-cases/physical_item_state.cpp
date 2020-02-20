#include "bear/universe/physical_item_state.hpp"
#include "bear/universe/shape/curved_box.hpp"
#include "bear/universe/shape/rectangle.hpp"
#include "bear/universe/shape/shape_traits.hpp"

#define BOOST_TEST_MODULE bear::universe::physical_item_state
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( size )
{
  bear::universe::physical_item_state item;

  const bear::universe::rectangle_type bounding_box( 10, 5, 20, 20 );
  item.set_bounding_box( bounding_box );

  BOOST_CHECK( item.get_bounding_box() == bounding_box );
  BOOST_CHECK_EQUAL( item.get_width(), bounding_box.width() );
  BOOST_CHECK_EQUAL( item.get_height(), bounding_box.height() );
  BOOST_CHECK( item.get_size()
               == bear::universe::size_box_type
               ( bounding_box.width(), bounding_box.height() ) );
}

BOOST_AUTO_TEST_CASE( acceleration )
{
  bear::universe::physical_item_state item;

  item.set_acceleration( bear::universe::force_type( 24, 12 ) );
  BOOST_CHECK_EQUAL( item.get_acceleration().x, 24 );
  BOOST_CHECK_EQUAL( item.get_acceleration().y, 12 );
}

BOOST_AUTO_TEST_CASE( internal_force )
{
  bear::universe::physical_item_state item;

  item.set_internal_force( bear::universe::force_type( 24, 12 ) );
  BOOST_CHECK_EQUAL( item.get_internal_force().x, 24 );
  BOOST_CHECK_EQUAL( item.get_internal_force().y, 12 );

  item.add_internal_force( bear::universe::force_type( 2, 3 ) );
  BOOST_CHECK_EQUAL( item.get_internal_force().x, 26 );
  BOOST_CHECK_EQUAL( item.get_internal_force().y, 15 );
}

BOOST_AUTO_TEST_CASE( external_force )
{
  bear::universe::physical_item_state item;

  item.set_external_force( bear::universe::force_type( 24, 12 ) );
  BOOST_CHECK_EQUAL( item.get_external_force().x, 24 );
  BOOST_CHECK_EQUAL( item.get_external_force().y, 12 );

  item.add_external_force( bear::universe::force_type( 2, 3 ) );
  BOOST_CHECK_EQUAL( item.get_external_force().x, 26 );
  BOOST_CHECK_EQUAL( item.get_external_force().y, 15 );
}

BOOST_AUTO_TEST_CASE( force )
{
  bear::universe::physical_item_state item;

  item.set_external_force( bear::universe::force_type( 24, 12 ) );
  item.set_internal_force( bear::universe::force_type( 1, 2 ) );
  item.set_system_angle( 3.14159 / 2 );

  BOOST_CHECK_CLOSE( item.get_force().x, 22, 0.001 );
  BOOST_CHECK_CLOSE( item.get_force().y, 13, 0.001 );
}

BOOST_AUTO_TEST_CASE( mass )
{
  bear::universe::physical_item_state item;

  item.set_mass( 1 );
  BOOST_CHECK_EQUAL( item.get_mass(), 1 );
}

BOOST_AUTO_TEST_CASE( density )
{
  bear::universe::physical_item_state item;

  item.set_density( 23 );
  BOOST_CHECK_EQUAL( item.get_density(), 23 );
}

BOOST_AUTO_TEST_CASE( speed )
{
  bear::universe::physical_item_state item;

  item.set_speed( 3, 6 );
  BOOST_CHECK_EQUAL( item.get_speed().x, 3 );
  BOOST_CHECK_EQUAL( item.get_speed().y, 6 );

  item.set_speed( bear::universe::speed_type( 34, 16 ) );
  BOOST_CHECK_EQUAL( item.get_speed().x, 34 );
  BOOST_CHECK_EQUAL( item.get_speed().y, 16 );
}

BOOST_AUTO_TEST_CASE( angular_speed )
{
  bear::universe::physical_item_state item;

  item.set_angular_speed( 23 );
  BOOST_CHECK_EQUAL( item.get_angular_speed(), 23 );
  item.add_angular_speed( -5 );
  BOOST_CHECK_EQUAL( item.get_angular_speed(), 18 );
}

BOOST_AUTO_TEST_CASE( friction )
{
  bear::universe::physical_item_state item;

  item.set_friction( 23 );
  BOOST_CHECK_EQUAL( item.get_friction(), 23 );
}

BOOST_AUTO_TEST_CASE( contact_friction )
{
  bear::universe::physical_item_state item;

  item.set_contact_friction( 23 );
  BOOST_CHECK_EQUAL( item.get_contact_friction(), 23 );
}

BOOST_AUTO_TEST_CASE( elasticity )
{
  bear::universe::physical_item_state item;

  item.set_elasticity( 23 );
  BOOST_CHECK_EQUAL( item.get_elasticity(), 23 );
}

BOOST_AUTO_TEST_CASE( hardness )
{
  bear::universe::physical_item_state item;

  item.set_hardness( 23 );
  BOOST_CHECK_EQUAL( item.get_hardness(), 23 );
}

BOOST_AUTO_TEST_CASE( system_angle )
{
  bear::universe::physical_item_state item;

  BOOST_CHECK_EQUAL( item.get_x_axis().x, 1 );
  BOOST_CHECK_EQUAL( item.get_x_axis().y, 0 );

  item.set_system_angle( -3.14159 / 2 );
  BOOST_CHECK( std::abs( item.get_x_axis().x - 0 ) <= 0.001 );
  BOOST_CHECK( std::abs( item.get_x_axis().y - -1) <= 0.001 );
  
  BOOST_CHECK( !item.has_free_system() );
  
  item.set_system_angle( 24 );
  BOOST_CHECK_EQUAL( item.get_system_angle(), 24 );
  
  item.set_contact_angle( 3 );
  BOOST_CHECK_EQUAL( item.get_system_angle(), 3 );

  item.set_free_system( true );
  
  item.set_system_angle( 24 );
  BOOST_CHECK_EQUAL( item.get_system_angle(), 24 );
  
  item.set_contact_angle( 3 );
  BOOST_CHECK_EQUAL( item.get_system_angle(), 24 );
}

BOOST_AUTO_TEST_CASE( fix )
{
  bear::universe::physical_item_state item;

  BOOST_CHECK( !item.is_fixed() );
  item.fix();
  BOOST_CHECK( item.is_fixed() );
}

BOOST_AUTO_TEST_CASE( can_move_items )
{
  bear::universe::physical_item_state item;

  item.set_can_move_items( true );
  BOOST_CHECK( item.can_move_items() );
  item.set_can_move_items( false );
  BOOST_CHECK( !item.can_move_items() );
}

BOOST_AUTO_TEST_CASE( global )
{
  bear::universe::physical_item_state item;

  item.set_global( true );
  BOOST_CHECK( item.is_global() );
  item.set_global( false );
  BOOST_CHECK( !item.is_global() );
}

BOOST_AUTO_TEST_CASE( phantom )
{
  bear::universe::physical_item_state item;

  item.set_phantom( true );
  BOOST_CHECK( item.is_phantom() );
  item.set_phantom( false );
  BOOST_CHECK( !item.is_phantom() );
}

BOOST_AUTO_TEST_CASE( artificial )
{
  bear::universe::physical_item_state item;

  item.set_artificial( true );
  BOOST_CHECK( item.is_artificial() );
  item.set_artificial( false );
  BOOST_CHECK( !item.is_artificial() );
}

BOOST_AUTO_TEST_CASE( weak_collisions )
{
  bear::universe::physical_item_state item;

  item.set_weak_collisions( true );
  BOOST_CHECK( item.has_weak_collisions() );
  item.set_weak_collisions( false );
  BOOST_CHECK( !item.has_weak_collisions() );
}

BOOST_AUTO_TEST_CASE( shape )
{
  bear::universe::physical_item_state item;

  {
    using rectangle_traits =
      bear::universe::shape_traits< bear::universe::rectangle >;
  
    bear::universe::rectangle r;
    rectangle_traits::set_bounding_box
      ( r, bear::universe::rectangle_type( 3, 6, 9, 12 ) );
    item.set_shape( r );

    const bear::universe::rectangle* const r_clone
      ( dynamic_cast< bear::universe::rectangle* >
        ( item.get_shape().clone_impl() ) );
  
    BOOST_CHECK( r_clone != nullptr );
    BOOST_CHECK( rectangle_traits::get_bounding_box( *r_clone )
                 == rectangle_traits::get_bounding_box( r ) );
    delete r_clone;
  }

  {
    using curved_box_traits =
      bear::universe::shape_traits< bear::universe::curved_box >;
  
    bear::universe::curved_box curved_box;
    curved_box_traits::set_bounding_box
      ( curved_box, bear::universe::rectangle_type( 8, 9, 3, 6 ) );
    item.set_shape( curved_box );
    
    const bear::universe::curved_box* const box_clone
      ( dynamic_cast< bear::universe::curved_box* >
        ( item.get_shape().clone_impl() ) );
  
    BOOST_CHECK( box_clone != nullptr );
    BOOST_CHECK( curved_box_traits::get_bounding_box( *box_clone )
                 == curved_box_traits::get_bounding_box( curved_box ) );
    BOOST_CHECK( box_clone->get_steepness() == curved_box.get_steepness() );
    BOOST_CHECK( box_clone->get_margin() == curved_box.get_margin() );
    BOOST_CHECK( box_clone->get_left_control_point()
                 == curved_box.get_left_control_point() );
    BOOST_CHECK( box_clone->get_right_control_point()
                 == curved_box.get_right_control_point() );

    delete box_clone;
  }
}

BOOST_AUTO_TEST_CASE( bounding_box_applies_to_shape )
{
  using rectangle_traits =
    bear::universe::shape_traits< bear::universe::rectangle >;
  
  const bear::universe::rectangle_type box1( 3, 6, 9, 12 );
  const bear::universe::rectangle_type box2( 6, 3, 4, 9 );

  bear::universe::physical_item_state item;
  bear::universe::rectangle r;
  rectangle_traits::set_bounding_box( r, box1 );
  item.set_shape( r );

  BOOST_CHECK( item.get_bounding_box() == box1 );

  item.set_bounding_box( box2 );
  
  BOOST_CHECK( bear::universe::shape_traits
               <
                 bear::universe::shape
               >
               ::get_bounding_box( item.get_shape() )
               == box2 );
}

BOOST_AUTO_TEST_CASE( left_contact )
{
  bear::universe::physical_item_state item;

  item.set_bounding_box( bear::universe::rectangle_type( 10, 0, 20, 40 ) );

  item.set_left_contact( true );
  BOOST_CHECK( item.has_left_contact() );
  BOOST_CHECK( item.has_side_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_left_contact().get_min(), 0 );
  BOOST_CHECK_EQUAL( item.get_left_contact().get_max(), 1 );
  
  item.clear_contacts();
  BOOST_CHECK( !item.has_left_contact() );
  BOOST_CHECK( !item.has_side_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( !item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_left_contact().get_min(), 0 );
  BOOST_CHECK_EQUAL( item.get_left_contact().get_max(), 0 );

  item.set_left_contact( 10, 20 );
  BOOST_CHECK( item.has_left_contact() );
  BOOST_CHECK( item.has_side_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_left_contact().get_min(), 0.25 );
  BOOST_CHECK_EQUAL( item.get_left_contact().get_max(), 0.5 );

  item.set_left_contact( -1, 45 );
  BOOST_CHECK( item.has_left_contact() );
  BOOST_CHECK( item.has_side_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_left_contact().get_min(), 0 );
  BOOST_CHECK_EQUAL( item.get_left_contact().get_max(), 1 );
}

BOOST_AUTO_TEST_CASE( right_contact )
{
  bear::universe::physical_item_state item;

  item.set_bounding_box( bear::universe::rectangle_type( 10, 0, 20, 40 ) );

  item.set_right_contact( true );
  BOOST_CHECK( item.has_right_contact() );
  BOOST_CHECK( item.has_side_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_right_contact().get_min(), 0 );
  BOOST_CHECK_EQUAL( item.get_right_contact().get_max(), 1 );
  
  item.clear_contacts();
  BOOST_CHECK( !item.has_right_contact() );
  BOOST_CHECK( !item.has_side_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( !item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_right_contact().get_min(), 0 );
  BOOST_CHECK_EQUAL( item.get_right_contact().get_max(), 0 );

  item.set_right_contact( 10, 20 );
  BOOST_CHECK( item.has_side_contact() );
  BOOST_CHECK( item.has_right_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_right_contact().get_min(), 0.25 );
  BOOST_CHECK_EQUAL( item.get_right_contact().get_max(), 0.5 );

  item.set_right_contact( -1, 45 );
  BOOST_CHECK( item.has_side_contact() );
  BOOST_CHECK( item.has_right_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_right_contact().get_min(), 0 );
  BOOST_CHECK_EQUAL( item.get_right_contact().get_max(), 1 );
}

BOOST_AUTO_TEST_CASE( bottom_contact )
{
  bear::universe::physical_item_state item;

  item.set_bounding_box( bear::universe::rectangle_type( 10, 0, 20, 40 ) );

  item.set_bottom_contact( true );
  BOOST_CHECK( item.has_bottom_contact() );
  BOOST_CHECK( item.has_side_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_bottom_contact().get_min(), 0 );
  BOOST_CHECK_EQUAL( item.get_bottom_contact().get_max(), 1 );
  
  item.clear_contacts();
  BOOST_CHECK( !item.has_bottom_contact() );
  BOOST_CHECK( !item.has_side_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( !item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_bottom_contact().get_min(), 0 );
  BOOST_CHECK_EQUAL( item.get_bottom_contact().get_max(), 0 );

  item.set_bottom_contact( 15, 18 );
  BOOST_CHECK( item.has_bottom_contact() );
  BOOST_CHECK( item.has_side_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_bottom_contact().get_min(), 0.5 );
  BOOST_CHECK_EQUAL( item.get_bottom_contact().get_max(), 0.8 );

  item.set_bottom_contact( 3, 24 );
  BOOST_CHECK( item.has_bottom_contact() );
  BOOST_CHECK( item.has_side_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_bottom_contact().get_min(), 0 );
  BOOST_CHECK_EQUAL( item.get_bottom_contact().get_max(), 1 );
}

BOOST_AUTO_TEST_CASE( top_contact )
{
  bear::universe::physical_item_state item;

  item.set_bounding_box( bear::universe::rectangle_type( 10, 0, 20, 40 ) );

  item.set_top_contact( true );
  BOOST_CHECK( item.has_top_contact() );
  BOOST_CHECK( item.has_side_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_top_contact().get_min(), 0 );
  BOOST_CHECK_EQUAL( item.get_top_contact().get_max(), 1 );
  
  item.clear_contacts();
  BOOST_CHECK( !item.has_top_contact() );
  BOOST_CHECK( !item.has_side_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( !item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_top_contact().get_min(), 0 );
  BOOST_CHECK_EQUAL( item.get_top_contact().get_max(), 0 );

  item.set_top_contact( 15, 18 );
  BOOST_CHECK( item.has_top_contact() );
  BOOST_CHECK( item.has_side_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_top_contact().get_min(), 0.5 );
  BOOST_CHECK_EQUAL( item.get_top_contact().get_max(), 0.8 );

  item.set_top_contact( 3, 24 );
  BOOST_CHECK( item.has_top_contact() );
  BOOST_CHECK( item.has_side_contact() );
  BOOST_CHECK( !item.has_middle_contact() );
  BOOST_CHECK( item.has_contact() );
  BOOST_CHECK_EQUAL( item.get_top_contact().get_min(), 0 );
  BOOST_CHECK_EQUAL( item.get_top_contact().get_max(), 1 );
}

BOOST_AUTO_TEST_CASE( middle_contact )
{
  bear::universe::physical_item_state item;

  item.set_middle_contact( true );
  BOOST_CHECK( item.has_middle_contact() );
  BOOST_CHECK( item.has_contact() );
  BOOST_CHECK( !item.has_side_contact() );
}

static void check_position
( const bear::universe::physical_item_state& item,
  double left, double right, double bottom, double top )
{
  BOOST_CHECK_EQUAL( item.get_width(), right - left );
  BOOST_CHECK_EQUAL( item.get_height(), top - bottom );
  
  BOOST_CHECK_EQUAL( item.get_left(), left );
  BOOST_CHECK_EQUAL( item.get_right(), right );
  BOOST_CHECK_EQUAL( item.get_bottom(), bottom );
  BOOST_CHECK_EQUAL( item.get_top(), top );
  BOOST_CHECK_EQUAL( item.get_horizontal_middle(), ( left + right ) / 2 );
  BOOST_CHECK_EQUAL( item.get_vertical_middle(), ( bottom + top ) / 2 );

  BOOST_CHECK( item.get_top_left()
               == bear::universe::position_type
               ( item.get_left(), item.get_top() ) );
  BOOST_CHECK( item.get_bottom_left()
               == bear::universe::position_type
               ( item.get_left(), item.get_bottom() ) );
  BOOST_CHECK( item.get_top_right()
               == bear::universe::position_type
               ( item.get_right(), item.get_top() ) );
  BOOST_CHECK( item.get_bottom_right()
               == bear::universe::position_type
               ( item.get_right(), item.get_bottom() ) );
  BOOST_CHECK( item.get_center_of_mass()
               == bear::universe::position_type
               ( item.get_horizontal_middle(), item.get_vertical_middle() ) );
}

BOOST_AUTO_TEST_CASE( position )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  check_position( item, left, left + width, bottom, bottom + height );
}

BOOST_AUTO_TEST_CASE( position_left )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_left( 5 );
  check_position( item, 5, 5 + width, bottom, bottom + height );
}

BOOST_AUTO_TEST_CASE( position_right )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_right( 5 );
  check_position( item, 5 - width, 5, bottom, bottom + height );
}

BOOST_AUTO_TEST_CASE( position_bottom )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_bottom( 5 );
  check_position( item, left, left + width, 5, 5 + height );
}

BOOST_AUTO_TEST_CASE( position_top )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_top( 5 );
  check_position( item, left, left + width, 5 - height, 5 );
}

BOOST_AUTO_TEST_CASE( position_horizontal_middle )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_horizontal_middle( 5 );
  check_position( item, 5 - width / 2, 5 + width / 2, bottom, bottom + height );
}

BOOST_AUTO_TEST_CASE( position_vertical_middle )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_vertical_middle( 5 );
  check_position( item, left, left + width, 5 - height / 2, 5 + height / 2 );
}

BOOST_AUTO_TEST_CASE( position_2_parameters_bottom_left )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_bottom_left( 5, 10 );
  check_position( item, 5, 5 + width, 10, 10 + height );
}

BOOST_AUTO_TEST_CASE( position_2_parameters_bottom_right )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_bottom_right( 5, 10 );
  check_position( item, 5 - width, 5, 10, 10 + height );
}

BOOST_AUTO_TEST_CASE( position_2_parameters_top_left )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_top_left( 5, 10 );
  check_position( item, 5, 5 + width, 10 - height, 10 );
}

BOOST_AUTO_TEST_CASE( position_2_parameters_top_right )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_top_right( 5, 10 );
  check_position( item, 5 - width, 5, 10 - height, 10 );
}

BOOST_AUTO_TEST_CASE( position_2_parameters_top_middle )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_top_middle( 5, 10 );
  check_position( item, 5 - width / 2, 5 + width / 2, 10 - height, 10 );
}

BOOST_AUTO_TEST_CASE( position_2_parameters_bottom_middle )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_bottom_middle( 5, 10 );
  check_position( item, 5 - width / 2, 5 + width / 2, 10, 10 + height );
}

BOOST_AUTO_TEST_CASE( position_2_parameters_left_middle )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_left_middle( 5, 10 );
  check_position( item, 5, 5 + width, 10 - height / 2, 10 + height / 2 );
}

BOOST_AUTO_TEST_CASE( position_2_parameters_right_middle )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_right_middle( 5, 10 );
  check_position( item, 5 - width, 5, 10 - height / 2, 10 + height / 2 );
}

BOOST_AUTO_TEST_CASE( position_2_parameters_center )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_center_of_mass( 5, 10 );
  check_position
    ( item, 5 - width / 2, 5 + width / 2, 10 - height / 2, 10 + height / 2 );
}

BOOST_AUTO_TEST_CASE( position_1_parameter_bottom_left )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_bottom_left( bear::universe::position_type( 5, 10 ) );
  check_position( item, 5, 5 + width, 10, 10 + height );
}

BOOST_AUTO_TEST_CASE( position_1_parameter_bottom_right )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_bottom_right( bear::universe::position_type( 5, 10 ) );
  check_position( item, 5 - width, 5, 10, 10 + height );
}

BOOST_AUTO_TEST_CASE( position_1_parameter_top_left )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_top_left( bear::universe::position_type( 5, 10 ) );
  check_position( item, 5, 5 + width, 10 - height, 10 );
}

BOOST_AUTO_TEST_CASE( position_1_parameter_top_right )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_top_right( bear::universe::position_type( 5, 10 ) );
  check_position( item, 5 - width, 5, 10 - height, 10 );
}

BOOST_AUTO_TEST_CASE( position_1_parameter_top_middle )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_top_middle( bear::universe::position_type( 5, 10 ) );
  check_position( item, 5 - width / 2, 5 + width / 2, 10 - height, 10 );
}

BOOST_AUTO_TEST_CASE( position_1_parameter_bottom_middle )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_bottom_middle( bear::universe::position_type( 5, 10 ) );
  check_position( item, 5 - width / 2, 5 + width / 2, 10, 10 + height );
}

BOOST_AUTO_TEST_CASE( position_1_parameter_left_middle )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_left_middle( bear::universe::position_type( 5, 10 ) );
  check_position( item, 5, 5 + width, 10 - height / 2, 10 + height / 2 );
}

BOOST_AUTO_TEST_CASE( position_1_parameter_right_middle )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_right_middle( bear::universe::position_type( 5, 10 ) );
  check_position( item, 5 - width, 5, 10 - height / 2, 10 + height / 2 );
}

BOOST_AUTO_TEST_CASE( position_1_parameter_center )
{
  const float left( 10 );
  const float bottom( 0 );
  const float width( 10 );
  const float height( 40 );

  const bear::universe::rectangle_type box
      ( left, bottom, left + width, bottom + height );
  
  bear::universe::physical_item_state item;
  item.set_bounding_box( box );
  item.set_center_of_mass( bear::universe::position_type( 5, 10 ) );
  check_position
    ( item, 5 - width / 2, 5 + width / 2, 10 - height / 2, 10 + height / 2 );
}

BOOST_AUTO_TEST_CASE( position_center_on_item )
{
  bear::universe::physical_item_state item;
  item.set_bounding_box( bear::universe::rectangle_type( 10, 0, 20, 40 ) );

  bear::universe::physical_item_state other;
  other.set_bounding_box( bear::universe::rectangle_type( 3, 6, 9, 12 ) );

  item.set_center_on( other );
  BOOST_CHECK( item.get_center_of_mass() == other.get_center_of_mass() );
}

static void check_fixed_x( bear::universe::physical_item_state& item )
{
  const bear::universe::position_type center( item.get_center_of_mass() );
  const bear::universe::size_box_type size( item.get_size() );
  const bear::universe::speed_type speed( item.get_speed() );
  const bear::universe::force_type acceleration( item.get_acceleration() );
  const bear::universe::force_type internal_force( item.get_internal_force() );
  const bear::universe::force_type external_force( item.get_external_force() );
  
  item.set_center_of_mass( 0, 8 );
  BOOST_CHECK_EQUAL( item.get_horizontal_middle(), center.x );
  BOOST_CHECK_EQUAL( item.get_vertical_middle(), 8 );

  item.set_size( 0, 8 );
  BOOST_CHECK_EQUAL( item.get_width(), size.x );
  BOOST_CHECK_EQUAL( item.get_height(), 8 );

  item.set_acceleration( bear::universe::force_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_acceleration().x, acceleration.x );
  BOOST_CHECK_EQUAL( item.get_acceleration().y, 8 );

  item.set_internal_force( bear::universe::force_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_internal_force().x, internal_force.x );
  BOOST_CHECK_EQUAL( item.get_internal_force().y, 8 );

  item.add_internal_force( bear::universe::force_type( 7, 8 ) );
  BOOST_CHECK_EQUAL( item.get_internal_force().x, internal_force.x );
  BOOST_CHECK_EQUAL( item.get_internal_force().y, 16 );

  item.set_external_force( bear::universe::force_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_external_force().x, external_force.x );
  BOOST_CHECK_EQUAL( item.get_external_force().y, 8 );

  item.add_external_force( bear::universe::force_type( 7, 8 ) );
  BOOST_CHECK_EQUAL( item.get_external_force().x, external_force.x );
  BOOST_CHECK_EQUAL( item.get_external_force().y, 16 );

  item.set_speed( bear::universe::speed_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_speed().x, speed.x );
  BOOST_CHECK_EQUAL( item.get_speed().y, 8 );

  item.set_speed( 0, 16 );
  BOOST_CHECK_EQUAL( item.get_speed().x, speed.x );
  BOOST_CHECK_EQUAL( item.get_speed().y, 16 );

  using rectangle_traits =
    bear::universe::shape_traits< bear::universe::rectangle >;
  
  bear::universe::rectangle r;
  rectangle_traits::set_bounding_box
    ( r, bear::universe::rectangle_type( 6, 3, 4, 9 ) );
  item.set_shape( r );

  BOOST_CHECK_EQUAL( item.get_horizontal_middle(), center.x );
  BOOST_CHECK_EQUAL( item.get_width(), size.x );
  BOOST_CHECK_EQUAL( item.get_vertical_middle(), 6 );
  BOOST_CHECK_EQUAL( item.get_height(), 6 );
}

static void check_not_fixed( bear::universe::physical_item_state& item )
{
  const bear::universe::position_type center( item.get_center_of_mass() );
  const bear::universe::size_box_type size( item.get_size() );
  const bear::universe::speed_type speed( item.get_speed() );
  const bear::universe::force_type acceleration( item.get_acceleration() );
  const bear::universe::force_type internal_force( item.get_internal_force() );
  const bear::universe::force_type external_force( item.get_external_force() );
  
  item.set_center_of_mass( 0, 8 );
  BOOST_CHECK_EQUAL( item.get_horizontal_middle(), 0 );
  BOOST_CHECK_EQUAL( item.get_vertical_middle(), 8 );

  item.set_size( 0, 8 );
  BOOST_CHECK_EQUAL( item.get_width(), 0 );
  BOOST_CHECK_EQUAL( item.get_height(), 8 );

  item.set_acceleration( bear::universe::force_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_acceleration().x, 0 );
  BOOST_CHECK_EQUAL( item.get_acceleration().y, 8 );

  item.set_internal_force( bear::universe::force_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_internal_force().x, 0 );
  BOOST_CHECK_EQUAL( item.get_internal_force().y, 8 );

  item.add_internal_force( bear::universe::force_type( 7, 8 ) );
  BOOST_CHECK_EQUAL( item.get_internal_force().x, 7 );
  BOOST_CHECK_EQUAL( item.get_internal_force().y, 16 );

  item.set_external_force( bear::universe::force_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_external_force().x, 0 );
  BOOST_CHECK_EQUAL( item.get_external_force().y, 8 );

  item.add_external_force( bear::universe::force_type( 7, 8 ) );
  BOOST_CHECK_EQUAL( item.get_external_force().x, 7 );
  BOOST_CHECK_EQUAL( item.get_external_force().y, 16 );

  item.set_speed( bear::universe::speed_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_speed().x, 0 );
  BOOST_CHECK_EQUAL( item.get_speed().y, 8 );

  item.set_speed( 0, 16 );
  BOOST_CHECK_EQUAL( item.get_speed().x, 0 );
  BOOST_CHECK_EQUAL( item.get_speed().y, 16 );

  using rectangle_traits =
    bear::universe::shape_traits< bear::universe::rectangle >;
  
  bear::universe::rectangle r;
  rectangle_traits::set_bounding_box
    ( r, bear::universe::rectangle_type( 6, 3, 4, 9 ) );
  item.set_shape( r );

  BOOST_CHECK_EQUAL( item.get_horizontal_middle(), 5 );
  BOOST_CHECK_EQUAL( item.get_width(), 2 );
  BOOST_CHECK_EQUAL( item.get_vertical_middle(), 6 );
  BOOST_CHECK_EQUAL( item.get_height(), 6 );
}

BOOST_AUTO_TEST_CASE( position_constraint_x_cumulative )
{
  bear::universe::physical_item_state item;
  item.set_size( 10, 20 );
  item.set_center_of_mass( 3, 24 );
  item.set_acceleration( bear::universe::force_type( 2, 4 ) );
  item.set_internal_force( bear::universe::force_type( 4, 9 ) );
  item.set_external_force( bear::universe::force_type( 9, 18 ) );
  item.set_speed( bear::universe::speed_type( 1, -9 ) );
  
  item.add_position_constraint_x();
  check_fixed_x( item );
  
  item.add_position_constraint_x();
  item.remove_position_constraint_x();
  check_fixed_x( item );

  item.remove_position_constraint_x();
  check_not_fixed( item );
}

static void check_fixed_y( bear::universe::physical_item_state& item )
{
  const bear::universe::position_type center( item.get_center_of_mass() );
  const bear::universe::size_box_type size( item.get_size() );
  const bear::universe::speed_type speed( item.get_speed() );
  const bear::universe::force_type acceleration( item.get_acceleration() );
  const bear::universe::force_type internal_force( item.get_internal_force() );
  const bear::universe::force_type external_force( item.get_external_force() );
  
  item.set_center_of_mass( 0, 8 );
  BOOST_CHECK_EQUAL( item.get_horizontal_middle(), 0 );
  BOOST_CHECK_EQUAL( item.get_vertical_middle(), center.y );

  item.set_size( 0, 8 );
  BOOST_CHECK_EQUAL( item.get_width(), 0 );
  BOOST_CHECK_EQUAL( item.get_height(), size.y );

  item.set_acceleration( bear::universe::force_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_acceleration().x, 0 );
  BOOST_CHECK_EQUAL( item.get_acceleration().y, acceleration.y );

  item.set_internal_force( bear::universe::force_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_internal_force().x, 0 );
  BOOST_CHECK_EQUAL( item.get_internal_force().y, internal_force.y );

  item.add_internal_force( bear::universe::force_type( 7, 8 ) );
  BOOST_CHECK_EQUAL( item.get_internal_force().x, 7 );
  BOOST_CHECK_EQUAL( item.get_internal_force().y, internal_force.y );

  item.set_external_force( bear::universe::force_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_external_force().x, 0 );
  BOOST_CHECK_EQUAL( item.get_external_force().y, external_force.y );

  item.add_external_force( bear::universe::force_type( 7, 8 ) );
  BOOST_CHECK_EQUAL( item.get_external_force().x, 7 );
  BOOST_CHECK_EQUAL( item.get_external_force().y, external_force.y );

  item.set_speed( bear::universe::speed_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_speed().x, 0 );
  BOOST_CHECK_EQUAL( item.get_speed().y, speed.y );

  item.set_speed( 0, 16 );
  BOOST_CHECK_EQUAL( item.get_speed().x, 0 );
  BOOST_CHECK_EQUAL( item.get_speed().y, speed.y );

  using rectangle_traits =
    bear::universe::shape_traits< bear::universe::rectangle >;
  
  bear::universe::rectangle r;
  rectangle_traits::set_bounding_box
    ( r, bear::universe::rectangle_type( 6, 3, 4, 9 ) );
  item.set_shape( r );

  BOOST_CHECK_EQUAL( item.get_horizontal_middle(), 5 );
  BOOST_CHECK_EQUAL( item.get_width(), 2 );
  BOOST_CHECK_EQUAL( item.get_vertical_middle(), center.y );
  BOOST_CHECK_EQUAL( item.get_height(), size.y );
}

BOOST_AUTO_TEST_CASE( position_constraint_y_cumulative )
{
  bear::universe::physical_item_state item;
  item.set_size( 10, 20 );
  item.set_center_of_mass( 3, 24 );
  item.set_acceleration( bear::universe::force_type( 2, 4 ) );
  item.set_internal_force( bear::universe::force_type( 4, 9 ) );
  item.set_external_force( bear::universe::force_type( 9, 18 ) );
  item.set_speed( bear::universe::speed_type( 1, -9 ) );
  
  item.add_position_constraint_y();
  check_fixed_y( item );

  item.add_position_constraint_y();
  item.remove_position_constraint_y();
  check_fixed_y( item );

  item.remove_position_constraint_y();
  check_not_fixed( item );
}

static void check_fixed_both( bear::universe::physical_item_state& item )
{
  const bear::universe::position_type center( item.get_center_of_mass() );
  const bear::universe::size_box_type size( item.get_size() );
  const bear::universe::speed_type speed( item.get_speed() );
  const bear::universe::force_type acceleration( item.get_acceleration() );
  const bear::universe::force_type internal_force( item.get_internal_force() );
  const bear::universe::force_type external_force( item.get_external_force() );
  
  item.set_center_of_mass( 0, 8 );
  BOOST_CHECK_EQUAL( item.get_horizontal_middle(), center.x );
  BOOST_CHECK_EQUAL( item.get_vertical_middle(), center.y );

  item.set_size( 0, 8 );
  BOOST_CHECK_EQUAL( item.get_width(), size.x );
  BOOST_CHECK_EQUAL( item.get_height(), size.y );

  item.set_acceleration( bear::universe::force_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_acceleration().x, acceleration.x );
  BOOST_CHECK_EQUAL( item.get_acceleration().y, acceleration.y );

  item.set_internal_force( bear::universe::force_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_internal_force().x, internal_force.x );
  BOOST_CHECK_EQUAL( item.get_internal_force().y, internal_force.y );

  item.add_internal_force( bear::universe::force_type( 7, 8 ) );
  BOOST_CHECK_EQUAL( item.get_internal_force().x, internal_force.x );
  BOOST_CHECK_EQUAL( item.get_internal_force().y, internal_force.y );

  item.set_external_force( bear::universe::force_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_external_force().x, external_force.x );
  BOOST_CHECK_EQUAL( item.get_external_force().y, external_force.y );

  item.add_external_force( bear::universe::force_type( 7, 8 ) );
  BOOST_CHECK_EQUAL( item.get_external_force().x, external_force.x );
  BOOST_CHECK_EQUAL( item.get_external_force().y, external_force.y );

  item.set_speed( bear::universe::speed_type( 0, 8 ) );
  BOOST_CHECK_EQUAL( item.get_speed().x, speed.x );
  BOOST_CHECK_EQUAL( item.get_speed().y, speed.y );

  item.set_speed( 0, 16 );
  BOOST_CHECK_EQUAL( item.get_speed().x, speed.x );
  BOOST_CHECK_EQUAL( item.get_speed().y, speed.y );

  using rectangle_traits =
    bear::universe::shape_traits< bear::universe::rectangle >;
  
  bear::universe::rectangle r;
  rectangle_traits::set_bounding_box
    ( r, bear::universe::rectangle_type( 6, 3, 4, 9 ) );
  item.set_shape( r );

  BOOST_CHECK_EQUAL( item.get_horizontal_middle(), center.x );
  BOOST_CHECK_EQUAL( item.get_width(), size.x );
  BOOST_CHECK_EQUAL( item.get_vertical_middle(), center.y );
  BOOST_CHECK_EQUAL( item.get_height(), size.y );
}

BOOST_AUTO_TEST_CASE( position_constraint_both )
{
  bear::universe::physical_item_state item;
  item.set_size( 10, 20 );
  item.set_center_of_mass( 3, 24 );
  item.set_acceleration( bear::universe::force_type( 2, 4 ) );
  item.set_internal_force( bear::universe::force_type( 4, 9 ) );
  item.set_external_force( bear::universe::force_type( 9, 18 ) );
  item.set_speed( bear::universe::speed_type( 1, -9 ) );
  
  item.add_position_constraints();
  check_fixed_both( item );

  item.add_position_constraints();
  item.remove_position_constraints();
  check_fixed_both( item );

  item.remove_position_constraints();
  check_not_fixed( item );
}

BOOST_AUTO_TEST_CASE( copy )
{
  bear::universe::physical_item_state item;
  item.set_size( 10, 20 );
  item.set_center_of_mass( 3, 24 );
  item.set_acceleration( bear::universe::force_type( 2, 4 ) );
  item.set_internal_force( bear::universe::force_type( 4, 9 ) );
  item.set_external_force( bear::universe::force_type( 9, 18 ) );
  item.set_speed( bear::universe::speed_type( 1, -9 ) );
  item.fix();
  
  const bear::universe::physical_item_state other( item );

  BOOST_CHECK( other.get_size() == item.get_size() );
  BOOST_CHECK( other.get_center_of_mass() == item.get_center_of_mass() );
  BOOST_CHECK( other.get_acceleration() == item.get_acceleration() );
  BOOST_CHECK( other.get_internal_force() == item.get_internal_force() );
  BOOST_CHECK( other.get_external_force() == item.get_external_force() );
  BOOST_CHECK( other.get_speed() == item.get_speed() );
  BOOST_CHECK( !other.is_fixed() );
}

BOOST_AUTO_TEST_CASE( set_physical_state )
{
  bear::universe::physical_item_state item;
  item.set_size( 10, 20 );
  item.set_center_of_mass( 3, 24 );
  item.set_acceleration( bear::universe::force_type( 2, 4 ) );
  item.set_internal_force( bear::universe::force_type( 4, 9 ) );
  item.set_external_force( bear::universe::force_type( 9, 18 ) );
  item.set_speed( bear::universe::speed_type( 1, -9 ) );
  
  bear::universe::physical_item_state other;
  other.set_physical_state( item );

  BOOST_CHECK( other.get_size() == item.get_size() );
  BOOST_CHECK( other.get_center_of_mass() == item.get_center_of_mass() );
  BOOST_CHECK( other.get_acceleration() == item.get_acceleration() );
  BOOST_CHECK( other.get_internal_force() == item.get_internal_force() );
  BOOST_CHECK( other.get_external_force() == item.get_external_force() );
  BOOST_CHECK( other.get_speed() == item.get_speed() );
}

BOOST_AUTO_TEST_CASE( set_physical_state_fixed )
{
  bear::universe::physical_item_state item;
  item.set_size( 10, 20 );
  item.set_center_of_mass( 3, 24 );
  item.set_acceleration( bear::universe::force_type( 2, 4 ) );
  item.set_internal_force( bear::universe::force_type( 4, 9 ) );
  item.set_external_force( bear::universe::force_type( 9, 18 ) );
  item.set_speed( bear::universe::speed_type( 1, -9 ) );
  
  bear::universe::physical_item_state other;
  other.set_size( 15, 24 );
  other.set_center_of_mass( 9, 4 );
  other.set_acceleration( bear::universe::force_type( 4, 2 ) );
  other.set_internal_force( bear::universe::force_type( 9, 4 ) );
  other.set_external_force( bear::universe::force_type( 33, 1 ) );
  other.set_speed( bear::universe::speed_type( 93, 1934 ) );
  other.fix();
  
  const bear::universe::physical_item_state ref( other );
  other.set_physical_state( item );

  BOOST_CHECK( other.get_size() == ref.get_size() );
  BOOST_CHECK( other.get_center_of_mass() == ref.get_center_of_mass() );
  BOOST_CHECK( other.get_acceleration() == ref.get_acceleration() );
  BOOST_CHECK( other.get_internal_force() == ref.get_internal_force() );
  BOOST_CHECK( other.get_external_force() == ref.get_external_force() );
  BOOST_CHECK( other.get_speed() == ref.get_speed() );
}
