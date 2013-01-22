/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::u_int_game_variable_setter class.
 * \author Sebastien Angibaud
 */
#include "generic_items/game_variable/u_int_game_variable_setter.hpp"

#include "engine/game.hpp"
#include "engine/variable/variable.hpp"

BASE_ITEM_EXPORT( u_int_game_variable_setter, bear )
BASE_ITEM_EXPORT( u_int_game_variable_setter_suicide, bear )
BASE_ITEM_EXPORT( u_int_game_variable_setter_toggle, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
bear::u_int_game_variable_setter::u_int_game_variable_setter()
  : m_value(0)
{

} // u_int_game_variable_setter::u_int_game_variable_setter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is correctly initialized.
 */
bool bear::u_int_game_variable_setter::is_valid() const
{
  return (!m_name.empty()) && super::is_valid();
} // u_int_game_variable_setter::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "unsigned int".
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::u_int_game_variable_setter::set_u_integer_field
( const std::string& name, unsigned int value )
{
  bool result = true;

  if ( name == "u_int_game_variable_setter.value" )
    m_value = value;
  else
    result = super::set_u_integer_field( name, value );

  return result;
} // u_int_game_variable_setter::set_u_integer_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::u_int_game_variable_setter::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "u_int_game_variable_setter.name" )
    m_name = value;
  else
    result = super::set_string_field(name, value);

  return result;
} // u_int_game_variable_setter::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assign the value to the game variable.
 */
void bear::u_int_game_variable_setter::assign_game_variable_value() const
{
  engine::game::get_instance().set_game_variable
    ( engine::variable<unsigned int>( m_name, m_value ) );
} // u_int_game_variable_setter::assign_game_variable_value()
