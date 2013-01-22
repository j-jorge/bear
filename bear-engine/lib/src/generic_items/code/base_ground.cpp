/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::base_ground class.
 * \author Julien Jorge
 */
#include "generic_items/base_ground.hpp"

#include "universe/collision_info.hpp"

#include "engine/item_brick/loader/z_shift_loader.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::base_ground::base_ground()
  : m_top_sets_contact(true), m_bottom_sets_contact(true),
    m_right_sets_contact(true), m_left_sets_contact(true),
    m_top_left_contact_mode( universe::contact_mode::no_contact ),
    m_top_right_contact_mode( universe::contact_mode::no_contact ),
    m_bottom_left_contact_mode( universe::contact_mode::no_contact ),
    m_bottom_right_contact_mode( universe::contact_mode::no_contact )
{

} // base_ground::base_ground()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c boolean
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::base_ground::set_bool_field( const std::string& name, bool value )
{
  bool result(true);

  if ( name == "base_ground.top_sets_contact" )
    m_top_sets_contact = value;
  else if ( name == "base_ground.bottom_sets_contact" )
    m_bottom_sets_contact = value;
  else if ( name == "base_ground.left_sets_contact" )
    m_left_sets_contact = value;
  else if ( name == "base_ground.right_sets_contact" )
    m_right_sets_contact = value;
  else
    result = super::set_bool_field(name, value);

  return result;
} // base_ground::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::base_ground::set_string_field
( const std::string& name, const std::string& value )
{
  bool result(true);

  if ( name == "base_ground.top_left_contact_mode" )
    m_top_left_contact_mode = universe::contact_mode::from_string( value );
  else if ( name == "base_ground.bottom_left_contact_mode" )
    m_bottom_left_contact_mode = universe::contact_mode::from_string( value );
  else if ( name == "base_ground.top_right_contact_mode" )
    m_top_right_contact_mode = universe::contact_mode::from_string( value );
  else if ( name == "base_ground.bottom_right_contact_mode" )
    m_bottom_right_contact_mode = universe::contact_mode::from_string( value );
  else
    result = super::set_string_field(name, value);

  return result;
} // base_ground::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the contact modes.
 * \param top Tell if the top side is considered.
 * \param bottom Tell if the bottom side is considered.
 * \param left Tell if the left side is considered.
 * \param right Tell if the right side is considered.
 *
 * If two adjacent sides are considered, and if the corresponding corner is set
 * to universe::contact_mode::no_contact, then the contact mode becomes
 * universe::contact_mode::range_contact. Otherwise if only one adjacent edge is
 * considered, the contact mode becomes universe::contact_mode::full_contact.
 */
void bear::base_ground::init_default_contact_mode
( bool top, bool bottom, bool left, bool right )
{
  set_default_contact_mode( m_top_left_contact_mode, top, left );
  set_default_contact_mode( m_top_right_contact_mode, top, right );
  set_default_contact_mode( m_bottom_left_contact_mode, bottom, left );
  set_default_contact_mode( m_bottom_right_contact_mode, bottom, right );
} // base_ground::init_default_contact_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the contact mode on the left side.
 * \param info The description of the collision.
 */
bear::universe::contact_mode::value_type
bear::base_ground::get_left_contact_mode
( const universe::collision_info& info ) const
{
  return get_left_contact_mode( info, info.get_bottom_left_on_contact() );
} // base_ground::get_left_contact_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the contact mode on the left side.
 * \param info The description of the collision.
 * \param pos The bottom left position where the other item is considered.
 */
bear::universe::contact_mode::value_type
bear::base_ground::get_left_contact_mode
( const universe::collision_info& info,
  const universe::position_type& pos ) const
{
  if ( !m_left_sets_contact )
    return universe::contact_mode::no_contact;
  else
    return compute_vertical_contact_mode
      ( info, pos, m_bottom_left_contact_mode, m_top_left_contact_mode );
} // base_ground::get_left_contact_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the contact mode on the right side.
 * \param info The description of the collision.
 */
bear::universe::contact_mode::value_type
bear::base_ground::get_right_contact_mode
( const universe::collision_info& info ) const
{
  return get_right_contact_mode( info, info.get_bottom_left_on_contact() );
} // base_ground::get_right_contact_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the contact mode on the right side.
 * \param info The description of the collision.
 * \param pos The bottom left position where the other item is considered.
 */
bear::universe::contact_mode::value_type
bear::base_ground::get_right_contact_mode
( const universe::collision_info& info,
  const universe::position_type& pos ) const
{
  if ( !m_right_sets_contact )
    return universe::contact_mode::no_contact;
  else
    return compute_vertical_contact_mode
      ( info, pos, m_bottom_right_contact_mode, m_top_right_contact_mode );
} // base_ground::get_right_contact_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the contact mode on the top side.
 * \param info The description of the collision.
 */
bear::universe::contact_mode::value_type
bear::base_ground::get_top_contact_mode
( const universe::collision_info& info ) const
{
  return get_top_contact_mode( info, info.get_bottom_left_on_contact() );
} // base_ground::get_top_contact_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the contact mode on the top side.
 * \param info The description of the collision.
 * \param pos The bottom left position where the other item is considered.
 */
bear::universe::contact_mode::value_type
bear::base_ground::get_top_contact_mode
( const universe::collision_info& info,
  const universe::position_type& pos ) const
{
  if ( !m_top_sets_contact )
    return universe::contact_mode::no_contact;
  else
    return compute_horizontal_contact_mode
      ( info, pos, m_top_left_contact_mode, m_top_right_contact_mode );
} // base_ground::get_top_contact_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the contact mode on the bottom side.
 * \param info The description of the collision.
 */
bear::universe::contact_mode::value_type
bear::base_ground::get_bottom_contact_mode
( const universe::collision_info& info ) const
{
  return get_bottom_contact_mode( info, info.get_bottom_left_on_contact() );
} // base_ground::get_bottom_contact_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the contact mode on the bottom side.
 * \param info The description of the collision.
 * \param pos The bottom left position where the other item is considered.
 */
bear::universe::contact_mode::value_type
bear::base_ground::get_bottom_contact_mode
( const universe::collision_info& info,
  const universe::position_type& pos ) const
{
  if ( !m_bottom_sets_contact )
    return universe::contact_mode::no_contact;
  else
    return compute_horizontal_contact_mode
      ( info, pos, m_bottom_left_contact_mode, m_bottom_right_contact_mode );
} // base_ground::get_bottom_contact_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the contact mode on the middle side.
 * \param info The description of the collision.
 */
bear::universe::contact_mode::value_type
bear::base_ground::get_middle_contact_mode
( const universe::collision_info& info ) const
{
  if ( m_middle_sets_contact )
    return universe::contact_mode::full_contact;
  else
    return universe::contact_mode::no_contact;
} // base_ground::get_middle_contact_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the z position of an other item according to this item.
 * \param that The item whose z position must be set.
 */
void bear::base_ground::do_z_shift( engine::base_item& that ) const
{
  z_shift::do_z_shift( that, *this );
} // base_ground::do_z_shift()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::base_ground::populate_loader_map( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( engine::z_shift_loader(*this) );
} // base_ground::populate_loader_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find how the contact mode must be set for a collision by the top or
 *        the bottom.
 * \param info The description of the collision.
 * \param pos The bottom left position where the other item is considered.
 * \param left_mode The mode defined for the corner on the left of the edge.
 * \param right_mode The mode defined for the corner on the right of the edge.
 */
bear::universe::contact_mode::value_type
bear::base_ground::compute_horizontal_contact_mode
( const universe::collision_info& info, const universe::position_type& pos,
  universe::contact_mode::value_type left_mode,
  universe::contact_mode::value_type right_mode ) const
{
  const universe::coordinate_type left = pos.x;
  const universe::coordinate_type right = left + info.other_item().get_width();

 if ( left < get_left() )
    {
      if ( (left_mode != universe::contact_mode::full_contact)
           && (right > get_right()) )
        return right_mode;
      else
        return left_mode;
    }
  else if ( right > get_right() )
    return right_mode;
  else
    return universe::contact_mode::full_contact;
} // base_ground::compute_horizontal_contact_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find how the contact mode must be set for a collision by the left or
 *        the right.
 * \param info The description of the collision.
 * \param pos The bottom left position where the other item is considered.
 * \param bottom_mode The mode defined for the corner on the bottom of the edge.
 * \param top_mode The mode defined for the corner on the top of the edge.
 */
bear::universe::contact_mode::value_type
bear::base_ground::compute_vertical_contact_mode
( const universe::collision_info& info, const universe::position_type& pos,
  universe::contact_mode::value_type bottom_mode,
  universe::contact_mode::value_type top_mode ) const
{
  const universe::coordinate_type bottom = pos.y;
  const universe::coordinate_type top = bottom + info.other_item().get_height();

 if ( bottom < get_bottom() )
    {
      if ( (bottom_mode != universe::contact_mode::full_contact)
           && (top > get_top()) )
        return top_mode;
      else
        return bottom_mode;
    }
  else if ( top > get_top() )
    return top_mode;
  else
    return universe::contact_mode::full_contact;
} // base_ground::compute_vertical_contact_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize a contact modes.
 * \param c The contact to initialize.
 * \param one_side Tell if the this adjacent edge is considered.
 * \param other_side Tell if the other adjacent edge is considered.
 *
 * If both adjacent sides are considered, and if \a c is set to
 * universe::contact_mode::no_contact, then \a c becomes
 * universe::contact_mode::range_contact. Otherwise if only one adjacent edge is
 * considered, \a c becomes universe::contact_mode::full_contact.
 */
void bear::base_ground::set_default_contact_mode
( universe::contact_mode::value_type& c, bool one_side, bool other_side )
{
  if ( c != universe::contact_mode::no_contact )
    return;

  if ( one_side && other_side )
    c = universe::contact_mode::range_contact;
  else if ( one_side ^ other_side )
    c = universe::contact_mode::full_contact;
} // base_ground::set_default_contact_mode()
