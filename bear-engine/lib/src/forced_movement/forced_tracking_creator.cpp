/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::forced_tracking_creator class.
 * \author Julien Jorge
 */
#include "bear/generic_items/forced_movement/forced_tracking_creator.hpp"

#include "bear/engine/export.hpp"

BASE_ITEM_EXPORT( forced_tracking_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::forced_tracking_creator::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "forced_tracking_creator.duration" )
    get_movement().set_total_time(value);
  else if ( name == "forced_tracking_creator.distance.x" )
    {
      universe::position_type s( get_movement().get_distance() );
      s.x = value;
      get_movement().set_distance(s);
    }
  else if ( name == "forced_tracking_creator.distance.y" )
    {
      universe::position_type s( get_movement().get_distance() );
      s.y = value;
      get_movement().set_distance(s);
    }
  else
    result = super::set_real_field(name, value);

  return result;
} // forced_tracking_creator::set_real_field()
