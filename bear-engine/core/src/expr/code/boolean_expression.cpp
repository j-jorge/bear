/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::expr::boolean_expression class.
 * \author Julien Jorge.
 */
#include "expr/boolean_expression.hpp"

#include "expr/binary_boolean_expression.hpp"
#include "expr/boolean_constant.hpp"
#include "expr/logical_not.hpp"
#include "expr/logical_xor.hpp"

#include <algorithm>

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
bear::expr::boolean_expression::boolean_expression()
  : m_expr( new boolean_constant(false) )
{

} // boolean_expression::boolean_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 * \param e An expression to init from.
 */
bear::expr::boolean_expression::boolean_expression
( const base_boolean_expression& e )
  : m_expr(e.clone())
{

} // boolean_expression::boolean_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy contructor.
 * \param that The instance to copy from.
 */
bear::expr::boolean_expression::boolean_expression
( const boolean_expression& that )
  : m_expr( that.m_expr->clone() )
{

} // boolean_expression::boolean_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::expr::boolean_expression::~boolean_expression()
{
  delete m_expr;
} // boolean_expression::~boolean_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
bool bear::expr::boolean_expression::evaluate() const
{
  return m_expr->evaluate();
} // boolean_expression::evaluate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
bear::expr::boolean_expression::operator bool() const
{
  return evaluate();
} // boolean_expression::operator bool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment.
 * \param that The instance to copy from.
 */
bear::expr::boolean_expression&
bear::expr::boolean_expression::operator=( const boolean_expression& that )
{
  boolean_expression tmp(that);

  std::swap(tmp.m_expr, m_expr);

  return *this;
} // boolean_expression::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Equality.
 * \param that The expression to compare to.
 */
bear::expr::boolean_expression bear::expr::boolean_expression::operator==
( const boolean_expression& that ) const
{
  return boolean_equality( *this, that );
} // boolean_expression::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disequality.
 * \param that The expression to compare to.
 */
bear::expr::boolean_expression bear::expr::boolean_expression::operator!=
( const boolean_expression& that ) const
{
  return boolean_disequality( *this, that );
} // boolean_expression::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Logical and.
 * \param that The expression to compare to.
 */
bear::expr::boolean_expression bear::expr::boolean_expression::operator&&
( const boolean_expression& that ) const
{
  return logical_and( *this, that );
} // boolean_expression::operator&&()

/*----------------------------------------------------------------------------*/
/**
 * \brief Logical or.
 * \param that The expression to compare to.
 */
bear::expr::boolean_expression bear::expr::boolean_expression::operator||
( const boolean_expression& that ) const
{
  return logical_or( *this, that );
} // boolean_expression::operator||()

/*----------------------------------------------------------------------------*/
/**
 * \brief Logical xor.
 * \param that The expression to compare to.
 */
bear::expr::boolean_expression bear::expr::boolean_expression::operator^
( const boolean_expression& that ) const
{
  return logical_xor( *this, that );
} // boolean_expression::operator^()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a formatted and human readable representation of this expression.
 */
std::string bear::expr::boolean_expression::formatted_string() const
{
  return m_expr->formatted_string();
} // boolean_expression::formatted_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Logical not.
 * \param e The expression to negate.
 */
bear::expr::boolean_expression
operator!( const bear::expr::boolean_expression& e )
{
  return bear::expr::logical_not(e);
} // operator!()
