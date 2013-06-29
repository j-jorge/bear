/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::forced_movement_applicator_loader
 *        class.
 * \author Julien Jorge.
 */
#include "engine/item_brick/loader/forced_movement_applicator_loader.hpp"

#include "engine/item_brick/forced_movement_applicator.hpp"
#include "engine/base_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item loaded by this loader.
 */
bear::engine::forced_movement_applicator_loader::
forced_movement_applicator_loader
( forced_movement_applicator& item )
  : forced_movement_loader("forced_movement_applicator", item.m_movement),
    m_item(item)
{

} // forced_movement_applicator_loader::forced_movement_applicator_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this object.
 */
bear::engine::forced_movement_applicator_loader*
bear::engine::forced_movement_applicator_loader::clone() const
{
  return new forced_movement_applicator_loader(*this);
} // forced_movement_applicator_loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "list of item".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::engine::forced_movement_applicator_loader::set_field
( const std::string& name, const std::vector<engine::base_item*>& value )
{
  bool result = true;

  if ( name == "actor" )
    m_item.m_actor = value;
  else
    result = false;

  return result;
} // forced_movement_applicator_loader::set_field()
