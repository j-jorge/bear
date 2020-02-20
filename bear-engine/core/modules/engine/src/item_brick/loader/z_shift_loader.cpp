/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::z_shift_loader class.
 * \author Julien Jorge.
 */
#include "bear/engine/item_brick/loader/z_shift_loader.hpp"

#include "bear/engine/item_brick/z_shift.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item loaded by this loader.
 */
bear::engine::z_shift_loader::z_shift_loader( z_shift& item )
  : item_loader_base("item_with_z_shift"), m_item(item)
{

} // z_shift_loader::z_shift_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this object.
 */
bear::engine::z_shift_loader* bear::engine::z_shift_loader::clone() const
{
  return new z_shift_loader(*this);
} // z_shift_loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::z_shift_loader::set_field
( const std::string& name, int value )
{
  bool ok(true);

  if (name == "z_shift")
    m_item.set_z_shift( value );
  else
    ok = false;

  return ok;
} // z_shift_loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::z_shift_loader::set_field
( const std::string& name, bool value )
{
  bool ok(true);

  if (name == "force_z_position")
    m_item.set_force_z_position( value );
  else
    ok = false;

  return ok;
} // z_shift_loader::set_field()
