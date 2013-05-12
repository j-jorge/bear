/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::item_with_restricted_z_collision
 *        class.
 * \author Julien Jorge
 */
#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
template<class Base>
bear::engine::item_with_restricted_z_collision<Base>::
item_with_restricted_z_collision()
  : m_min_z(std::numeric_limits<int>::min()),
    m_max_z(std::numeric_limits<int>::max())
{

} // item_with_restricted_z_collision::item_with_restricted_z_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type int.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::item_with_restricted_z_collision<Base>::set_integer_field
( const std::string& name, int value )
{
  bool ok(true);

  if (name == "item_with_restricted_z_collision.min_z_for_collision")
    m_min_z = value;
  else if (name == "item_with_restricted_z_collision.max_z_for_collision")
    m_max_z = value;
  else
    ok = super::set_integer_field(name, value);

  return ok;
} // item_with_restricted_z_collision::set_integer_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the minimum z-position to be aligned.
 * \param z The new bound.
 */
template<class Base>
void bear::engine::item_with_restricted_z_collision<Base>::
set_min_z_for_collision( int z )
{
  m_min_z = z;
} // item_with_restricted_z_collision::set_min_z_for_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the maximum z-position to be aligned.
 * \param z The new bound.
 */
template<class Base>
void bear::engine::item_with_restricted_z_collision<Base>::
set_max_z_for_collision( int z )
{
  m_max_z = z;
} // item_with_restricted_z_collision::set_max_z_for_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the other item satisfies the condition for the collision
 *        treatment to be applied.
 * \param that The item to check.
 */
template<class Base>
bool bear::engine::item_with_restricted_z_collision<Base>::
satisfy_collision_condition( const base_item& that ) const
{
  return (that.get_z_position() >= m_min_z)
    && (that.get_z_position() <= m_max_z);
} // item_with_restricted_z_collision::satisfy_collision_condition()
