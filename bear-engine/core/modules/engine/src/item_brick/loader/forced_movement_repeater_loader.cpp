/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::forced_movement_repeater_loader
 *        class.
 * \author Julien Jorge.
 */
#include "bear/engine/item_brick/loader/forced_movement_repeater_loader.hpp"

#include "bear/engine/item_brick/forced_movement_repeater.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item loaded by this loader.
 */
bear::engine::forced_movement_repeater_loader::forced_movement_repeater_loader
( forced_movement_repeater& item )
  : forced_movement_loader("forced_movement_repeater", item.m_movement),
    m_item(item)
{

} // forced_movement_repeater_loader::forced_movement_repeater_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this object.
 */
bear::engine::forced_movement_repeater_loader*
bear::engine::forced_movement_repeater_loader::clone() const
{
  return new forced_movement_repeater_loader(*this);
} // forced_movement_repeater_loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c unsigned int.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::forced_movement_repeater_loader::set_field
( const std::string& name, unsigned int value )
{
  bool result = true;

  if ( name == "count" )
    m_item.m_count = value;
  else
    result = false;

  return result;
} // forced_movement_repeater_loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::forced_movement_repeater_loader::set_field
( const std::string& name, double value )
{
  bool ok(true);

  if (name == "delay")
    m_item.m_delay = value;
  else
    ok = forced_movement_loader::set_field( name, value );

  return ok;
} // forced_movement_repeater_loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "item".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::engine::forced_movement_repeater_loader::set_field
( const std::string& name, engine::base_item* value )
{
  bool result = true;

  if ( name == "actor" )
    m_item.m_actor = value;
  else
    result = forced_movement_loader::set_field( name, value );

  return result;
} // forced_movement_repeater_loader::set_field()
