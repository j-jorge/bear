/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::forced_stay_around_creator class.
 * \author Julien Jorge
 */
#include "generic_items/forced_movement/forced_stay_around_creator.hpp"

#include "universe/forced_movement/forced_stay_around.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( forced_stay_around_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::forced_stay_around_creator::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "forced_stay_around_creator.duration" )
    get_movement().set_total_time(value);
  else if ( name == "forced_stay_around_creator.max_angle" )
    get_movement().set_max_angle(value);
  else if ( name == "forced_stay_around_creator.speed" )
    get_movement().set_speed(value);
  else if ( name == "forced_stay_around_creator.max_distance" )
    get_movement().set_max_distance(value);
  else
    result = super::set_real_field(name, value);

  return result;
} // forced_stay_around_creator::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "bool".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::forced_stay_around_creator::set_bool_field
( const std::string& name, bool value )
{
  bool result = true;

  if ( name == "forced_stay_around_creator.apply_angle" )
    get_movement().set_apply_angle(value);
  else
    result = super::set_bool_field(name, value);

  return result;
} // forced_stay_around_creator::set_bool_field()
