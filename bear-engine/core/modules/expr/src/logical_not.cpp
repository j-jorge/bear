/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::expr::logical_not class.
 * \author Julien Jorge.
 */
#include "bear/expr/logical_not.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
bear::expr::logical_not::logical_not()
{

} // logical_not::logical_not()

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 * \param op The operand.
 */
bear::expr::logical_not::logical_not( const boolean_expression& op )
  : m_operand(op)
{

} // logical_not::logical_not()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the operand.
 */
const bear::expr::boolean_expression&
bear::expr::logical_not::get_operand() const
{
  return m_operand;
} // logical_not::get_operand()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the operand.
 * \param op The new operand.
 */
void bear::expr::logical_not::set_operand( const boolean_expression& op )
{
  m_operand = op;
} // logical_not::set_operand()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
bear::expr::base_boolean_expression* bear::expr::logical_not::clone() const
{
  return new logical_not(*this);
} // logical_not::logical_not()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
bool bear::expr::logical_not::evaluate() const
{
  return !m_operand.evaluate();
} // logical_not::evaluate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a formatted and human readable representation of this expression.
 */
std::string bear::expr::logical_not::formatted_string()
  const
{
  return "not( " + m_operand.formatted_string() + " )";
} // logical_not::formatted_string()
