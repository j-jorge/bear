/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the template methods of the
 *        bear::engine::item_loader class.
 * \author Julien Jorge.
 */

#include "engine/loader/item_loader.hpp"

#include "engine/loader/item_loader_base.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of the item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<typename T>
bool bear::engine::item_loader::set_field
( const std::string& name, T value )
{
  return m_impl->set_field( name, value );
} // item_loader::set_field()
