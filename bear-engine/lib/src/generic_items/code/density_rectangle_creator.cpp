/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::density_rectangle_creator class.
 * \author Sebastien Angibaud
 */
#include "generic_items/density_rectangle_creator.hpp"

#include "engine/layer/layer.hpp"
#include "engine/world.hpp"

BASE_ITEM_EXPORT( density_rectangle_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::density_rectangle_creator::density_rectangle_creator()
  : m_density(0)
{
  set_can_move_items(false);
} // density_rectangle_creator::density_rectangle_creator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::density_rectangle_creator::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "density_rectangle_creator.zone_density" )
    m_density = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // density_rectangle_creator::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::density_rectangle_creator::build()
{
  if ( get_layer().has_world() )
    get_layer().get_world().add_density_rectangle
      (get_bounding_box(), m_density);

  kill();
} // density_rectangle_creator::build()
