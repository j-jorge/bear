/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::force_rectangle_creator class.
 * \author Sebastien Angibaud
 */
#include "bear/generic_items/force_rectangle_creator.hpp"

#include "bear/engine/layer/layer.hpp"
#include "bear/engine/world.hpp"

BASE_ITEM_EXPORT( force_rectangle_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::force_rectangle_creator::force_rectangle_creator()
  : m_force(0, 0)
{

} // force_rectangle_creator::force_rectangle_creator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::force_rectangle_creator::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "force_rectangle_creator.force.x" )
    m_force.x = value;
  else if ( name == "force_rectangle_creator.force.y" )
    m_force.y = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // force_rectangle_creator::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::force_rectangle_creator::build()
{
  if ( get_layer().has_world() )
    get_layer().get_world().add_force_rectangle(get_bounding_box(), m_force);

  kill();
} // force_rectangle_creator::build()
