/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::forced_translation_creator class.
 * \author Julien Jorge
 */
#include "generic_items/forced_movement/forced_translation_creator.hpp"

#include "universe/forced_movement/forced_translation.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( forced_translation_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::forced_translation_creator::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "forced_translation_creator.duration" )
    m_movement.set_total_time(value);
  else if ( name == "forced_translation_creator.slant" )
    m_movement.set_angle(value);
  else if ( name == "forced_translation_creator.speed.x" )
    {
      universe::speed_type s( m_movement.get_speed() );
      s.x = value;
      m_movement.set_speed(s);
    }
  else if ( name == "forced_translation_creator.speed.y" )
    {
      universe::speed_type s( m_movement.get_speed() );
      s.y = value;
      m_movement.set_speed(s);
    }
  else
    result = super::set_real_field(name, value);

  return result;
} // forced_translation_creator::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "bool".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::forced_translation_creator::set_bool_field
( const std::string& name, bool value )
{
  bool result = true;

  if ( name == "forced_translation_creator.force_angle" )
    m_movement.set_force_angle(value);
  else
    result = super::set_bool_field(name, value);

  return result;
} // forced_translation_creator::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::forced_translation_creator::build()
{
  set_forced_movement(m_movement);
  kill();
} // forced_translation_creator::build()
