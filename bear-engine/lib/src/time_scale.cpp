/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::time_scale class.
 * \author Julien Jorge
 */
#include "bear/generic_items/time_scale.hpp"

#include "bear/engine/game.hpp"

BASE_ITEM_EXPORT( time_scale, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::time_scale::time_scale()
  : m_scale(2)
{

} // time_scale::time_scale()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::time_scale::set_real_field( const std::string& name, double value )
{
  bool result(true);

  if ( name == "time_scale.scale" )
    m_scale = value;
  else
    result = super::set_real_field( name, value );

  return result;
} // time_scale::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is well initialised.
 */
bool bear::time_scale::is_valid() const
{
  return (m_scale >= 0) && super::is_valid();
} // time_scale::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the item when the toggle is on.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::time_scale::progress_on( universe::time_type elapsed_time )
{
  super::progress_on(elapsed_time);
  engine::game::get_instance().set_time_scale(m_scale);
} // time_scale::progress_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set scale value.
 * \param scale The new scale.
 */
void bear::time_scale::set_scale(double scale)
{
  m_scale = scale;
} // time_scale::set_scale()
