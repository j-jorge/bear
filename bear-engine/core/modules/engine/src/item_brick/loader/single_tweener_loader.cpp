/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::single_tweener_loader class.
 * \author Julien Jorge.
 */
#include "bear/engine/item_brick/loader/single_tweener_loader.hpp"

#include "bear/engine/item_brick/z_shift.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param prefix The prefix of the field names read by this loader.
 * \param tweener The tweener loaded by this loader.
 */
bear::engine::single_tweener_loader::single_tweener_loader
( std::string prefix, claw::tween::single_tweener& tweener )
  : item_loader_base(prefix), m_tweener(tweener)
{

} // single_tweener_loader::single_tweener_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciates a copy of this object.
 */
bear::engine::single_tweener_loader*
bear::engine::single_tweener_loader::clone() const
{
  return new single_tweener_loader(*this);
} // single_tweener_loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::single_tweener_loader::set_field
( const std::string& name, double value )
{
  bool ok(true);

  if (name == "origin")
    m_tweener.set_init( value );
  else if (name == "target")
    m_tweener.set_end( value );
  else if (name == "duration")
    m_tweener.set_duration( value );
  else
    ok = item_loader_base::set_field( name, value );

  return ok;
} // single_tweener_loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c easing function.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::single_tweener_loader::set_field
( const std::string& name, easing_function value )
{
  bool ok(true);

  if (name == "easing")
    m_tweener.set_easing( value );
  else
    ok = item_loader_base::set_field( name, value );

  return ok;
} // single_tweener_loader::set_field()
