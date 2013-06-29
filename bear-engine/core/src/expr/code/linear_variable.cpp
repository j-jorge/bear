/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::expr::linear_variable class.
 * \author Julien Jorge.
 */
#include "expr/linear_variable.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param v The variable from which we take the value.
 * \remark \a v must live longer than \a this.
 */
bear::expr::linear_variable::linear_variable( const double& v )
  : m_value(v)
{

} // linear_variable::linear_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
bear::expr::base_linear_expression*
bear::expr::linear_variable::clone() const
{
  return new linear_variable(*this);
} // linear_variable::linear_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
double bear::expr::linear_variable::evaluate() const
{
  return m_value;
} // linear_variable::evaluate()
