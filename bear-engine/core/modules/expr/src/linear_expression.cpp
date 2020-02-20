/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::expr::linear_expression class.
 * \author Julien Jorge.
 */
#include "bear/expr/linear_expression.hpp"

#include "bear/expr/linear_constant.hpp"
#include "bear/expr/binary_linear_expression.hpp"

#include <algorithm>

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 * \param v The initial value.
 */
bear::expr::linear_expression::linear_expression( double v )
  : m_expr( new linear_constant(v) )
{

} // linear_expression::linear_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 * \param e An expression to init from.
 */
bear::expr::linear_expression::linear_expression
( const base_linear_expression& e )
  : m_expr(e.clone())
{

} // linear_expression::linear_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy contructor.
 * \param that The instance to copy from.
 */
bear::expr::linear_expression::linear_expression
( const linear_expression& that )
  : m_expr( that.m_expr->clone() )
{

} // linear_expression::linear_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::expr::linear_expression::~linear_expression()
{
  delete m_expr;
} // linear_expression::~linear_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
double bear::expr::linear_expression::evaluate() const
{
  return m_expr->evaluate();
} // linear_expression::evaluate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment.
 * \param that The instance to copy from.
 */
bear::expr::linear_expression&
bear::expr::linear_expression::operator=( const linear_expression& that )
{
  linear_expression tmp(that);

  std::swap(tmp.m_expr, m_expr);

  return *this;
} // linear_expression::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param that The expression to compare to.
 */
bear::expr::boolean_expression bear::expr::linear_expression::operator==
( const linear_expression& that ) const
{
  return linear_equality( *this, that );
} // linear_expression::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disequality.
 * \param that The expression to compare to.
 */
bear::expr::boolean_expression bear::expr::linear_expression::operator!=
( const linear_expression& that ) const
{
  return linear_disequality( *this, that );
} // linear_expression::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Less than.
 * \param that The expression to compare to.
 */
bear::expr::boolean_expression bear::expr::linear_expression::operator<
( const linear_expression& that ) const
{
  return linear_less( *this, that );
} // linear_expression::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Less equal.
 * \param that The expression to compare to.
 */
bear::expr::boolean_expression bear::expr::linear_expression::operator<=
( const linear_expression& that ) const
{
  return linear_less_equal( *this, that );
} // linear_expression::operator<=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Greater than.
 * \param that The expression to compare to.
 */
bear::expr::boolean_expression bear::expr::linear_expression::operator>
( const linear_expression& that ) const
{
  return linear_greater( *this, that );
} // linear_expression::operator>()

/*----------------------------------------------------------------------------*/
/**
 * \brief Greater equal.
 * \param that The expression to compare to.
 */
bear::expr::boolean_expression bear::expr::linear_expression::operator>=
( const linear_expression& that ) const
{
  return linear_greater_equal( *this, that );
} // linear_expression::operator>=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Plus.
 * \param that The right operand.
 */
bear::expr::linear_expression& bear::expr::linear_expression::operator+=
( const linear_expression& that )
{
  linear_plus result( *this, that );
  *this = result;
  return *this;
} // linear_expression::operator+=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Minus.
 * \param that The right operand.
 */
bear::expr::linear_expression& bear::expr::linear_expression::operator-=
( const linear_expression& that )
{
  linear_minus result( *this, that );
  *this = result;
  return *this;
} // linear_expression::operator-=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Multiply.
 * \param that The right operand.
 */
bear::expr::linear_expression& bear::expr::linear_expression::operator*=
( const linear_expression& that )
{
  linear_multiplies result( *this, that );
  *this = result;
  return *this;
} // linear_expression::operator*=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Divide
 * \param that The right operand.
 */
bear::expr::linear_expression& bear::expr::linear_expression::operator/=
( const linear_expression& that )
{
  linear_divides result( *this, that );
  *this = result;
  return *this;
} // linear_expression::operator/=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Plus.
 * \param that The right operand.
 */
bear::expr::linear_expression bear::expr::linear_expression::operator+
( const linear_expression& that ) const
{
  return linear_plus( *this, that );
} // linear_expression::operator+()

/*----------------------------------------------------------------------------*/
/**
 * \brief Minus.
 * \param that The right operand.
 */
bear::expr::linear_expression bear::expr::linear_expression::operator-
( const linear_expression& that ) const
{
  return linear_minus( *this, that );
} // linear_expression::operator-()

/*----------------------------------------------------------------------------*/
/**
 * \brief Multiply.
 * \param that The right operand.
 */
bear::expr::linear_expression bear::expr::linear_expression::operator*
( const linear_expression& that ) const
{
  return linear_multiplies( *this, that );
} // linear_expression::operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Divide
 * \param that The right operand.
 */
bear::expr::linear_expression bear::expr::linear_expression::operator/
( const linear_expression& that ) const
{
  return linear_divides( *this, that );
} // linear_expression::operator/()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a formatted and human readable representation of this expression.
 */
std::string bear::expr::linear_expression::formatted_string() const
{
  return m_expr->formatted_string();
} // linear_expression::formatted_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression checking if the evaluation of two linear
 *        expressions are equal.
 * \param a The left operand.
 * \param b The right operand.
 */
bear::expr::boolean_expression
operator==
( const bear::expr::base_linear_expression& a,
  const bear::expr::base_linear_expression& b )
{
  return bear::expr::linear_equality( a, b );
} // operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression checking if the evaluation of two linear
 *        expressions are not equal.
 * \param a The left operand.
 * \param b The right operand.
 */
bear::expr::boolean_expression
operator!=
( const bear::expr::base_linear_expression& a,
  const bear::expr::base_linear_expression& b )
{
  return bear::expr::linear_disequality( a, b );
} // operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression checking if the evaluation of two linear
 *        expressions are equal.
 * \param v The value.
 * \param e The expression.
 */
bear::expr::boolean_expression
operator==( double v, const bear::expr::base_linear_expression& e )
{
  return bear::expr::linear_equality( bear::expr::linear_constant(v), e );
} // operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression checking if a value is equal to the evaluation of
 *        an expression.
 * \param v The value.
 * \param e The expression.
 */
bear::expr::boolean_expression
operator!=( double v, const bear::expr::base_linear_expression& e )
{
  return bear::expr::linear_disequality( bear::expr::linear_constant(v), e );
} // operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression checking if a value is not equal to the
 *        evaluation of an expression.
 * \param v The value.
 * \param e The expression.
 */
bear::expr::boolean_expression
operator<( double v, const bear::expr::base_linear_expression& e )
{
  return bear::expr::linear_less( bear::expr::linear_constant(v), e );
} // operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression to check if a value is lower or equal to the
 *        evaluation of an expression.
 * \param v The value.
 * \param e The expression.
 */
bear::expr::boolean_expression
operator<=( double v, const bear::expr::base_linear_expression& e )
{
  return bear::expr::linear_less_equal( bear::expr::linear_constant(v), e );
} // operator<=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression to check if a value is greater than the
 *        evaluation of an expression.
 * \param v The value.
 * \param e The expression.
 */
bear::expr::boolean_expression
operator>( double v, const bear::expr::base_linear_expression& e )
{
  return bear::expr::linear_greater( bear::expr::linear_constant(v), e );
} // operator>()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression to check if a value is greater or equal to the
 *        evaluation of an expression.
 * \param v The value.
 * \param e The expression.
 */
bear::expr::boolean_expression
operator>=( double v, const bear::expr::base_linear_expression& e )
{
  return bear::expr::linear_greater_equal( bear::expr::linear_constant(v), e );
} // operator>=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression that compute the sum of a value and the
 *        evaluation of an expression.
 * \param v The value.
 * \param e The expression.
 */
bear::expr::linear_expression
operator+( double v, const bear::expr::base_linear_expression& e )
{
  return bear::expr::linear_plus( bear::expr::linear_constant(v), e );
} // operator+()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression that compute the difference of a value and the
 *        evaluation of an expression.
 * \param v The value.
 * \param e The expression.
 */
bear::expr::linear_expression
operator-( double v, const bear::expr::base_linear_expression& e )
{
  return bear::expr::linear_minus( bear::expr::linear_constant(v), e );
} // operator-()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression that multiplies a value with the evaluation of
 *        an expression.
 * \param v The value.
 * \param e The expression.
 */
bear::expr::linear_expression
operator*( double v, const bear::expr::base_linear_expression& e )
{
  return bear::expr::linear_multiplies( bear::expr::linear_constant(v), e );
} // operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression that divides a value by the evaluation of an
 *         expression.
 * \param v The value.
 * \param e The expression.
 */
bear::expr::linear_expression
operator/( double v, const bear::expr::base_linear_expression& e )
{
  return bear::expr::linear_divides( bear::expr::linear_constant(v), e );
} // operator/()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression checking if the evaluation of two linear
 *        expressions are equal.
 * \param e The expression.
 * \param v The value.
 */
bear::expr::boolean_expression
operator==( const bear::expr::base_linear_expression& e, double v )
{
  return bear::expr::linear_equality( e, bear::expr::linear_constant(v) );
} // operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression checking if a value is equal to the evaluation of
 *        an expression.
 * \param e The expression.
 * \param v The value.
 */
bear::expr::boolean_expression
operator!=( const bear::expr::base_linear_expression& e, double v )
{
  return bear::expr::linear_disequality( e, bear::expr::linear_constant(v) );
} // operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression checking if a value is not equal to the
 *        evaluation of an expression.
 * \param e The expression.
 * \param v The value.
 */
bear::expr::boolean_expression
operator<( const bear::expr::base_linear_expression& e, double v )
{
  return bear::expr::linear_less( e, bear::expr::linear_constant(v) );
} // operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression to check if a value is lower or equal to the
 *        evaluation of an expression.
 * \param e The expression.
 * \param v The value.
 */
bear::expr::boolean_expression
operator<=( const bear::expr::base_linear_expression& e, double v )
{
  return bear::expr::linear_less_equal( e, bear::expr::linear_constant(v) );
} // operator<=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression to check if a value is greater than the
 *        evaluation of an expression.
 * \param e The expression.
 * \param v The value.
 */
bear::expr::boolean_expression
operator>( const bear::expr::base_linear_expression& e, double v )
{
  return bear::expr::linear_greater( e, bear::expr::linear_constant(v) );
} // operator>()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression to check if a value is greater or equal to the
 *        evaluation of an expression.
 * \param e The expression.
 * \param v The value.
 */
bear::expr::boolean_expression
operator>=( const bear::expr::base_linear_expression& e, double v )
{
  return bear::expr::linear_greater_equal( e, bear::expr::linear_constant(v) );
} // operator>=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression that compute the sum of a value and the
 *        evaluation of an expression.
 * \param e The expression.
 * \param v The value.
 */
bear::expr::linear_expression
operator+( const bear::expr::base_linear_expression& e, double v )
{
  return bear::expr::linear_plus( e, bear::expr::linear_constant(v) );
} // operator+()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression that compute the difference of a value and the
 *        evaluation of an expression.
 * \param e The expression.
 * \param v The value.
 */
bear::expr::linear_expression
operator-( const bear::expr::base_linear_expression& e, double v )
{
  return bear::expr::linear_minus( e, bear::expr::linear_constant(v) );
} // operator-()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression that multiplies a value with the evaluation of
 *        an expression.
 * \param e The expression.
 * \param v The value.
 */
bear::expr::linear_expression
operator*( const bear::expr::base_linear_expression& e, double v )
{
  return bear::expr::linear_multiplies( e, bear::expr::linear_constant(v) );
} // operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an expression that divides a value by the evaluation of an
 *         expression.
 * \param e The expression.
 * \param v The value.
 */
bear::expr::linear_expression
operator/( const bear::expr::base_linear_expression& e, double v )
{
  return bear::expr::linear_divides( e, bear::expr::linear_constant(v) );
} // operator/()
