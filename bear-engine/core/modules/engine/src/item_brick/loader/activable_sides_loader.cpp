/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::activable_sides_loader class.
 * \author Julien Jorge.
 */
#include "bear/engine/item_brick/loader/activable_sides_loader.hpp"

#include "bear/engine/item_brick/activable_sides.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item loaded by this loader.
 */
bear::engine::activable_sides_loader::activable_sides_loader
( activable_sides& item )
  : item_loader_base("item_with_activable_sides"), m_item(item)
{

} // activable_sides_loader::activable_sides_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this object.
 */
bear::engine::activable_sides_loader*
bear::engine::activable_sides_loader::clone() const
{
  return new activable_sides_loader(*this);
} // activable_sides_loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::activable_sides_loader::set_field
( const std::string& name, bool value )
{
  bool ok(true);

  if (name == "left_side_is_active")
    m_item.set_left_side_activation( value );
  else if (name == "right_side_is_active")
    m_item.set_right_side_activation( value );
  else if (name == "top_side_is_active")
    m_item.set_top_side_activation( value );
  else if (name == "bottom_side_is_active")
    m_item.set_bottom_side_activation( value );
  else
    ok = false;

  return ok;
} // activable_sides_loader::set_field()
