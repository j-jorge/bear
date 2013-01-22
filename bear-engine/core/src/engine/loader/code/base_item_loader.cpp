/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::base_item_loader class.
 * \author Julien Jorge.
 */
#include "engine/loader/base_item_loader.hpp"

#include "engine/base_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item loaded by this loader.
 */
bear::engine::base_item_loader::base_item_loader( base_item& item )
  : item_loader_base("base_item"), m_item(item)
{

} // base_item_loader::base_item_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this object.
 */
bear::engine::base_item_loader* bear::engine::base_item_loader::clone() const
{
  return new base_item_loader(*this);
} // base_item_loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item_loader::set_field
( const std::string& name, int value )
{
  bool ok = true;

  if (name == "position.depth")
    m_item.set_z_position( value );
  else
    ok = false;

  return ok;
} // base_item_loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item_loader::set_field
( const std::string& name, double value )
{
  bool ok = true;

  if (name == "position.left")
    m_item.set_left(value);
  else if (name == "position.bottom")
    m_item.set_bottom(value);
  else if (name == "size.height")
    m_item.set_height(value);
  else if (name == "size.width")
    m_item.set_width(value);
  else if (name == "mass")
    m_item.set_mass(value);
  else if (name == "density")
    m_item.set_density(value);
  else if (name == "elasticity")
    m_item.set_elasticity(value);
  else if (name == "hardness")
    m_item.set_hardness(value);
  else if (name == "system_angle")
    m_item.set_system_angle(value);
  else if (name == "speed.x")
    m_item.set_speed( universe::speed_type(value, m_item.get_speed().y) );
  else if (name == "speed.y")
    m_item.set_speed( universe::speed_type(m_item.get_speed().x, value) );
  else if (name == "angular_speed")
    m_item.set_angular_speed( value );
  else
    ok = false;

  return ok;
} // base_item_loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item_loader::set_field
( const std::string& name, bool value )
{
  bool ok = true;

  if (name == "artificial")
    m_item.set_artificial(value);
  else if (name == "can_move_items")
    m_item.set_can_move_items(value);
  else if (name == "global")
    m_item.set_global(value);
  else if (name == "phantom")
    m_item.set_phantom(value);
  else if (name == "fixed.x")
    {
      if ( value )
        m_item.add_position_constraint_x();
    }
  else if (name == "fixed.y")
    {
      if ( value )
        m_item.add_position_constraint_y();
    }
  else if (name == "fixed.z")
    m_item.set_z_fixed(value);
  else if (name == "free_system")
    m_item.set_free_system(value);
  else
    ok = false;

  return ok;
} // base_item_loader::set_field()
