/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::real_game_variable_setter class.
 * \author Sebastien Angibaud
 */
#include "bear/generic_items/game_variable/real_game_variable_setter.hpp"

#include "bear/engine/game.hpp"
#include "bear/engine/variable/variable.hpp"

BASE_ITEM_EXPORT( real_game_variable_setter, bear )
BASE_ITEM_EXPORT( real_game_variable_setter_suicide, bear )
BASE_ITEM_EXPORT( real_game_variable_setter_toggle, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
bear::real_game_variable_setter::real_game_variable_setter()
  : m_value(0)
{

} // real_game_variable_setter::real_game_variable_setter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is correctly initialized.
 */
bool bear::real_game_variable_setter::is_valid() const
{
  return (!m_name.empty()) && super::is_valid();
} // real_game_variable_setter::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "double".
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::real_game_variable_setter::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "real_game_variable_setter.value" )
    m_value = value;
  else
    result = super::set_real_field( name, value );

  return result;
} // real_game_variable_setter::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::real_game_variable_setter::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "real_game_variable_setter.name" )
    m_name = value;
  else
    result = super::set_string_field(name, value);

  return result;
} // real_game_variable_setter::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assign the value to the game variable.
 */
void bear::real_game_variable_setter::assign_game_variable_value() const
{
  engine::game::get_instance().set_game_variable
    ( engine::variable<double>( m_name, m_value ) );
} // real_game_variable_setter::assign_game_variable_value()
