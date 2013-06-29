/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::boolean_constant_creator class.
 * \author Sébastien Angibaud
 */
#include "generic_items/expr/boolean_constant_creator.hpp"

#include <claw/logger.hpp>

BASE_ITEM_EXPORT( boolean_constant_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "boolean".
 */
bear::boolean_constant_creator::boolean_constant_creator()
: m_expr(false)
{

} // boolean_constant_creator::boolean_constant_creator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "boolean".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::boolean_constant_creator::set_bool_field
( const std::string& name, bool value )
{
  bool result = true;

  if ( name == "boolean_constant_creator.value" )
    m_expr.set_value( value );
  else
    result = super::set_bool_field(name, value);

  return result;
} // boolean_constant_creator::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::boolean_constant_creator::build()
{
  kill();
} // boolean_constant_creator::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the expression created by this item.
 */
bear::expr::boolean_expression
bear::boolean_constant_creator::do_get_expression() const
{
  return m_expr;
} // boolean_constant_creator::do_get_expression()
