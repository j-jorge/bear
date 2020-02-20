/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::environment_rectangle_creator class.
 * \author Sebastien Angibaud
 */
#include "bear/generic_items/environment_rectangle_creator.hpp"

#include "bear/engine/layer/layer.hpp"
#include "bear/engine/world.hpp"

BASE_ITEM_EXPORT( environment_rectangle_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::environment_rectangle_creator::environment_rectangle_creator()
  : m_environment(bear::universe::air_environment)
{
  set_can_move_items(false);
} // environment_rectangle_creator::environment_rectangle_creator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "string".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::environment_rectangle_creator::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "environment_rectangle_creator.environment" )
    {
      if ( value == "air_environment" )
        m_environment = universe::air_environment;
      else if ( value == "fire_environment" )
        m_environment = universe::fire_environment;
      else if ( value == "ice_environment" )
        m_environment = universe::ice_environment;
      else if ( value == "water_environment" )
        m_environment = universe::water_environment;
      else
        result = false;
    }
  else
    result = super::set_string_field(name, value);

  return result;
} // environment_rectangle_creator::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::environment_rectangle_creator::build()
{
  if ( get_layer().has_world() )
    get_layer().get_world().add_environment_rectangle
      (get_bounding_box(), m_environment);

  kill();
} // environment_rectangle_creator::build()
