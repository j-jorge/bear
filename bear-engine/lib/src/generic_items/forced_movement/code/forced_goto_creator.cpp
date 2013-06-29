/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::forced_goto_creator class.
 * \author Julien Jorge
 */
#include "generic_items/forced_movement/forced_goto_creator.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( forced_goto_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::forced_goto_creator::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "forced_goto_creator.duration" )
    m_movement.set_total_time(value);
  else if ( name == "forced_goto_creator.acceleration_time" )
    m_movement.set_acceleration_time(value);
  else if ( name == "forced_goto_creator.length.x" )
    m_movement.set_x_length(value);
  else if ( name == "forced_goto_creator.length.y" )
    m_movement.set_y_length(value);
  else
    result = super::set_real_field(name, value);

  return result;
} // forced_goto_creator::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::forced_goto_creator::build()
{
  set_forced_movement(m_movement);

  kill();
} // forced_goto_creator::build()
