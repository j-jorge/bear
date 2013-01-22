/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::forced_rotation_creator class.
 * \author Julien Jorge
 */
#include "generic_items/forced_movement/forced_rotation_creator.hpp"

#include "universe/forced_movement/forced_rotation.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( forced_rotation_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::forced_rotation_creator::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "forced_rotation_creator.duration" )
    get_movement().set_total_time(value);
  else if ( name == "forced_rotation_creator.acceleration_time" )
    get_movement().set_acceleration_time(value);
  else if ( name == "forced_rotation_creator.start_angle" )
    get_movement().set_start_angle(value);
  else if ( name == "forced_rotation_creator.end_angle" )
    get_movement().set_end_angle(value);
  else if ( name == "forced_rotation_creator.radius" )
    get_movement().set_radius(value);
  else
    result = super::set_real_field(name, value);

  return result;
} // forced_rotation_creator::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "bool".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::forced_rotation_creator::set_bool_field
( const std::string& name, bool value )
{
  bool result = true;

  if ( name == "forced_rotation_creator.loop_back" )
    get_movement().set_loop_back(value);
  else
    result = super::set_bool_field(name, value);

  return result;
} // forced_rotation_creator::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "string".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::forced_rotation_creator::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "forced_rotation_creator.apply_angle" )
    {
      if ( value == "add" )
        get_movement().set_angle_application
          ( universe::forced_rotation::apply_add );
      else if ( value == "force" )
        get_movement().set_angle_application
          (universe::forced_rotation::apply_force);
      else if ( value == "force_delta" )
        get_movement().set_angle_application
          (universe::forced_rotation::apply_force_delta);
      else if ( value == "force_remaining" )
        get_movement().set_angle_application
          (universe::forced_rotation::apply_force_remaining);
      else if ( value == "keep" )
        get_movement().set_angle_application
          ( universe::forced_rotation::apply_keep );
      else
        result = false;
    }
  else
    result = super::set_string_field(name, value);

  return result;
} // forced_rotation_creator::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "unsigned int".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::forced_rotation_creator::set_u_integer_field
( const std::string& name, unsigned int value )
{
  bool result = true;

  if ( name == "forced_rotation_creator.loops" )
    get_movement().set_loops(value);
  else
    result = super::set_u_integer_field(name, value);

  return result;
} // forced_rotation_creator::set_loops()
