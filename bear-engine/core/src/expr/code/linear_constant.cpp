/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::expr::linear_constant class.
 * \author Julien Jorge.
 */
#include "expr/linear_constant.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param v The constant from which we take the value.
 */
bear::expr::linear_constant::linear_constant( double v )
  : m_value(v)
{

} // linear_constant::linear_constant()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
bear::expr::base_linear_expression*
bear::expr::linear_constant::clone() const
{
  return new linear_constant(*this);
} // linear_constant::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
double bear::expr::linear_constant::evaluate() const
{
  return m_value;
} // linear_constant::evaluate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of the constant.
 * \param b The new value.
 */
void bear::expr::linear_constant::set_value( double b )
{
  m_value = b;
} // linear_constant::set_value()
