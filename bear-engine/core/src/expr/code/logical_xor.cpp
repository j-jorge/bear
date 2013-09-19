/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::expr::logical_xor class.
 * \author Julien Jorge.
 */
#include "expr/logical_xor.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 * \param left The left operand.
 * \param right The right operand.
 */
bear::expr::logical_xor::logical_xor
( const boolean_expression& left, const boolean_expression& right )
  : m_left(left), m_right(right)
{

} // logical_xor::logical_xor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
bear::expr::base_boolean_expression* bear::expr::logical_xor::clone() const
{
  return new logical_xor(*this);
} // logical_xor::logical_xor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
bool bear::expr::logical_xor::evaluate() const
{
  return m_left.evaluate() ^ m_right.evaluate();
} // logical_xor::evaluate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a formatted and human readable representation of this expression.
 */
std::string bear::expr::logical_xor::formatted_string()
  const
{
  return "xor( " + m_left.formatted_string() + ", " + m_right.formatted_string()
    + " )";
} // logical_xor::formatted_string()
