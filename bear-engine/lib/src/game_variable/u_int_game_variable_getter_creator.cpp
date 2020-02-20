/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::u_int_game_variable_getter_creator class.
 * \author Julien Jorge
 */
#include "bear/generic_items/game_variable/u_int_game_variable_getter_creator.hpp"

BASE_ITEM_EXPORT( u_int_game_variable_getter_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
bear::u_int_game_variable_getter_creator::u_int_game_variable_getter_creator()
{
  m_expr.set_default_value(0);
} // u_int_game_variable_getter_creator::u_int_game_variable_getter_creator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::u_int_game_variable_getter_creator::build()
{
  kill();
} // u_int_game_variable_getter_creator::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is correctly initialized.
 */
bool bear::u_int_game_variable_getter_creator::is_valid() const
{
  return !m_expr.get_name().empty() && super::is_valid();
} // u_int_game_variable_getter_creator::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::u_int_game_variable_getter_creator::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "u_int_game_variable_getter_creator.name" )
    m_expr.set_name(value);
  else
    result = super::set_string_field(name, value);

  return result;
} // u_int_game_variable_getter_creator::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c unsigned integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::u_int_game_variable_getter_creator::set_u_integer_field
( const std::string& name, unsigned int value )
{
  bool result = true;

  if ( name == "u_int_game_variable_getter_creator.default_value" )
    m_expr.set_default_value(value);
  else
    result = super::set_u_integer_field(name, value);

  return result;
} // u_int_game_variable_getter_creator::set_u_int_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the expression created by this item.
 */
bear::expr::linear_expression
bear::u_int_game_variable_getter_creator::do_get_expression() const
{
  return m_expr;
} // u_int_game_variable_getter_creator::do_get_expression()
