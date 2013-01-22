/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::friction_rectangle_creator class.
 * \author Julien Jorge
 */
#include "generic_items/friction_rectangle_creator.hpp"

#include "engine/layer/layer.hpp"
#include "engine/world.hpp"

BASE_ITEM_EXPORT( friction_rectangle_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::friction_rectangle_creator::friction_rectangle_creator()
  : m_friction(1)
{
  set_can_move_items(false);
} // friction_rectangle_creator::friction_rectangle_creator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::friction_rectangle_creator::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "friction_rectangle_creator.friction" )
    m_friction = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // friction_rectangle_creator::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::friction_rectangle_creator::build()
{
  if ( get_layer().has_world() )
    get_layer().get_world().add_friction_rectangle
      (get_bounding_box(), m_friction);

  kill();
} // friction_rectangle_creator::build()
