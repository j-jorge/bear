/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::linear_constant_creator class.
 * \author Sébastien Angibaud
 */
#include "generic_items/expr/linear_constant_creator.hpp"

#include <claw/logger.hpp>

BASE_ITEM_EXPORT( linear_constant_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "linear".
 */
bear::linear_constant_creator::linear_constant_creator()
: m_expr(0)
{

} // linear_constant_creator::linear_constant_creator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "double".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::linear_constant_creator::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "linear_constant_creator.value" )
    m_expr.set_value( value );
  else
    result = super::set_real_field(name, value);

  return result;
} // linear_constant_creator::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::linear_constant_creator::build()
{
  kill();
} // linear_constant_creator::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the expression created by this item.
 */
bear::expr::linear_expression
bear::linear_constant_creator::do_get_expression() const
{
  return m_expr;
} // linear_constant_creator::do_get_expression()
