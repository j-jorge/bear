/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::forced_join_creator class.
 * \author Julien Jorge
 */
#include "generic_items/forced_movement/forced_join_creator.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( forced_join_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::forced_join_creator::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "forced_join_creator.duration" )
    get_movement().set_total_time(value);
  else
    result = super::set_real_field(name, value);

  return result;
} // forced_join_creator::set_real_field()

