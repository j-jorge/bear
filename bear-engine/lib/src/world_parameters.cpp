/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::world_parameters class.
 * \author Julien Jorge
 */
#include "bear/generic_items/world_parameters.hpp"

#include "bear/engine/layer/layer.hpp"
#include "bear/engine/world.hpp"

BASE_ITEM_EXPORT( world_parameters, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::world_parameters::world_parameters()
  : m_gravity(0, -9.81), m_speed_epsilon(0.1, 0.1),
    m_angular_speed_epsilon(0.01), m_unit(3000),
    m_default_friction(1), m_default_density(0),
    m_default_environment(universe::air_environment)
{

} // world_parameters::world_parameters()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool
bear::world_parameters::set_real_field( const std::string& name, double value )
{
  bool result = true;

  if ( name == "world_parameters.gravity.x" )
    m_gravity.x = value;
  else if ( name == "world_parameters.gravity.y" )
    m_gravity.y = value;
  else if ( name == "world_parameters.speed_epsilon.x" )
    m_speed_epsilon.x = value;
  else if ( name == "world_parameters.speed_epsilon.y" )
    m_speed_epsilon.y = value;
  else if ( name == "world_parameters.angular_speed_epsilon" )
    m_angular_speed_epsilon = value;
  else if ( name == "world_parameters.unit" )
    m_unit = value;
  else if ( name == "world_parameters.default_friction" )
    m_default_friction = value;
  else if ( name == "world_parameters.default_density" )
    m_default_density = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // world_parameters::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "string".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::world_parameters::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "world_parameters.default_environment" )
    {
      if ( value == "air_environment" )
        m_default_environment = universe::air_environment;
      else if ( value == "fire_environment" )
        m_default_environment = universe::fire_environment;
      else if ( value == "ice_environment" )
        m_default_environment = universe::ice_environment;
      else if ( value == "water_environment" )
        m_default_environment = universe::water_environment;
      else
        result = false;
    }
  else
    result = super::set_string_field(name, value);

  return result;
} // world_parameters::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::world_parameters::build()
{
  if ( get_layer().has_world() )
    {
      get_layer().get_world().set_unit(m_unit);
      get_layer().get_world().set_scaled_gravity(m_gravity);
      get_layer().get_world().set_scaled_speed_epsilon(m_speed_epsilon);
      get_layer().get_world().set_angular_speed_epsilon
        (m_angular_speed_epsilon);
      get_layer().get_world().set_default_friction(m_default_friction);
      get_layer().get_world().set_default_density(m_default_density);
      get_layer().get_world().set_default_environment(m_default_environment);
    }

  kill();
} // world_parameters::build()
