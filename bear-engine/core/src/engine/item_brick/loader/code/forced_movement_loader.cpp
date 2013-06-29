/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::forced_movement_loader class.
 * \author Julien Jorge.
 */
#include "engine/item_brick/loader/forced_movement_loader.hpp"

#include "engine/base_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param prefix The prefix of the fields loaded by this loader.
 * \param movement The movement by this loader.
 */
bear::engine::forced_movement_loader::forced_movement_loader
( const std::string& prefix, universe::forced_movement& mvt )
  : item_loader_base( prefix ), m_forced_movement(mvt)
{

} // forced_movement_loader::forced_movement_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this object.
 */
bear::engine::forced_movement_loader*
bear::engine::forced_movement_loader::clone() const
{
  return new forced_movement_loader(*this);
} // forced_movement_loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::forced_movement_loader::set_field
( const std::string& name, bool value )
{
  bool result = true;

  if ( name == "auto_remove" )
    m_forced_movement.set_auto_remove( value );
  else if ( name == "auto_angle" )
    m_forced_movement.set_auto_angle( value );
  else
    result = false;

  return result;
} // forced_movement_loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::forced_movement_loader::set_field
( const std::string& name, double value )
{
  bool ok(true);

  if (name == "actor.ratio.x")
    m_forced_movement.set_moving_item_ratio
      ( universe::position_type
        ( value, m_forced_movement.get_moving_item_ratio().y ) );
  else if (name == "actor.ratio.y")
    m_forced_movement.set_moving_item_ratio
      ( universe::position_type
        ( m_forced_movement.get_moving_item_ratio().x, value ) );
  else if (name == "actor.gap.x")
    m_forced_movement.set_moving_item_gap
      ( universe::position_type
        ( value, m_forced_movement.get_moving_item_gap().y ) );
  else if (name == "actor.gap.y")
    m_forced_movement.set_moving_item_gap
      ( universe::position_type
        ( m_forced_movement.get_moving_item_gap().x, value ) );
  else
    ok = false;

  return ok;
} // forced_movement_loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "item".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::engine::forced_movement_loader::set_field
( const std::string& name, engine::base_item* value )
{
  bool result = true;

  if ( name == "movement" )
    m_forced_movement = value->get_forced_movement();
  else
    result = false;

  return result;
} // forced_movement_loader::set_field()
