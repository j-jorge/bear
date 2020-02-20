/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::expr::boolean_variable class.
 * \author Julien Jorge.
 */
#include "bear/expr/boolean_variable.hpp"

#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param v The variable from which we take the value.
 * \remark \a v must live longer than \a this.
 */
bear::expr::boolean_variable::boolean_variable( const bool& v )
  : m_value(v)
{

} // boolean_variable::boolean_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
bear::expr::base_boolean_expression*
bear::expr::boolean_variable::clone() const
{
  return new boolean_variable(*this);
} // boolean_variable::boolean_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
bool bear::expr::boolean_variable::evaluate() const
{
  return m_value;
} // boolean_variable::evaluate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a formatted and human readable representation of this expression.
 */
std::string bear::expr::boolean_variable::formatted_string()
  const
{
  std::ostringstream result;

  result << "var( " << m_value << " )";

  return result.str();
} // boolean_variable::formatted_string()
