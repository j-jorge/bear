/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::string_game_variable_setter class.
 * \author Sebastien Angibaud
 */
#include "generic_items/game_variable/string_game_variable_setter.hpp"

#include "engine/game.hpp"
#include "engine/variable/variable.hpp"

BASE_ITEM_EXPORT( string_game_variable_setter, bear )
BASE_ITEM_EXPORT( string_game_variable_setter_suicide, bear )
BASE_ITEM_EXPORT( string_game_variable_setter_toggle, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
bear::string_game_variable_setter::string_game_variable_setter()
  : m_name(""), m_value("")
{
} // string_game_variable_setter::string_game_variable_setter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is correctly initialized.
 */
bool bear::string_game_variable_setter::is_valid() const
{
  return (!m_name.empty()) && super::is_valid();
} // string_game_variable_setter::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::string_game_variable_setter::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "string_game_variable_setter.name" )
    m_name = value;
  else if ( name == "string_game_variable_setter.value" )
    m_value = value;
  else
    result = super::set_string_field(name, value);

  return result;
} // string_game_variable_setter::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assign the value to the game variable.
 */
void bear::string_game_variable_setter::assign_game_variable_value() const
{
  engine::game::get_instance().set_game_variable
    ( engine::variable<std::string>( m_name, m_value ) );
} // string_game_variable_setter::assign_game_variable_value()
