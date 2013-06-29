/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A linear expression.
 * \author Julien Jorge
 */
#ifndef __EXPR_LINEAR_EXPRESSION_HPP__
#define __EXPR_LINEAR_EXPRESSION_HPP__

#include "expr/boolean_expression.hpp"

namespace bear
{
  namespace expr
  {
    class base_linear_expression;

    /**
     * \brief A linear expression.
     * \author Julien Jorge
     */
    class EXPR_EXPORT linear_expression
    {
    public:
      /** \brief The base class for linear expressions. */
      typedef base_linear_expression base_expression_type;

    public:
      linear_expression( double v = 0 );
      linear_expression( const base_linear_expression& e );
      linear_expression( const linear_expression& that );
      ~linear_expression();

      double evaluate() const;

      linear_expression& operator=( const linear_expression& that );

      boolean_expression operator==( const linear_expression& that ) const;
      boolean_expression operator!=( const linear_expression& that ) const;

      boolean_expression operator<( const linear_expression& that ) const;
      boolean_expression operator<=( const linear_expression& that ) const;
      boolean_expression operator>( const linear_expression& that ) const;
      boolean_expression operator>=( const linear_expression& that ) const;

      linear_expression& operator+=( const linear_expression& that );
      linear_expression& operator-=( const linear_expression& that );
      linear_expression& operator*=( const linear_expression& that );
      linear_expression& operator/=( const linear_expression& that );

      linear_expression operator+( const linear_expression& that ) const;
      linear_expression operator-( const linear_expression& that ) const;
      linear_expression operator*( const linear_expression& that ) const;
      linear_expression operator/( const linear_expression& that ) const;

    private:
      /** \brief The implemented expression. */
      base_linear_expression* m_expr;

    }; // class linear_expression

  } // namespace expr
} // namespace bear

EXPR_EXPORT bear::expr::boolean_expression
operator==
( const bear::expr::base_linear_expression& a,
  const bear::expr::base_linear_expression& b );
EXPR_EXPORT bear::expr::boolean_expression
operator!=
( const bear::expr::base_linear_expression& a,
  const bear::expr::base_linear_expression& b );

EXPR_EXPORT bear::expr::boolean_expression
operator==( double v, const bear::expr::base_linear_expression& e );
EXPR_EXPORT bear::expr::boolean_expression
operator!=( double v, const bear::expr::base_linear_expression& e );
EXPR_EXPORT bear::expr::boolean_expression
operator<( double v, const bear::expr::base_linear_expression& e );
EXPR_EXPORT bear::expr::boolean_expression
operator<=( double v, const bear::expr::base_linear_expression& e );
EXPR_EXPORT bear::expr::boolean_expression
operator>( double v, const bear::expr::base_linear_expression& e );
EXPR_EXPORT bear::expr::boolean_expression
operator>=( double v, const bear::expr::base_linear_expression& e );

EXPR_EXPORT bear::expr::linear_expression
operator+( double v, const bear::expr::base_linear_expression& e );
EXPR_EXPORT bear::expr::linear_expression
operator-( double v, const bear::expr::base_linear_expression& e );
EXPR_EXPORT bear::expr::linear_expression
operator*( double v, const bear::expr::base_linear_expression& e );
EXPR_EXPORT bear::expr::linear_expression
operator/( double v, const bear::expr::base_linear_expression& e );

EXPR_EXPORT bear::expr::boolean_expression
operator==( const bear::expr::base_linear_expression& e, double v );
EXPR_EXPORT bear::expr::boolean_expression
operator!=( const bear::expr::base_linear_expression& e, double v );
EXPR_EXPORT bear::expr::boolean_expression
operator<( const bear::expr::base_linear_expression& e, double v );
EXPR_EXPORT bear::expr::boolean_expression
operator<=( const bear::expr::base_linear_expression& e, double v );
EXPR_EXPORT bear::expr::boolean_expression
operator>( const bear::expr::base_linear_expression& e, double v );
EXPR_EXPORT bear::expr::boolean_expression
operator>=( const bear::expr::base_linear_expression& e, double v );

EXPR_EXPORT bear::expr::linear_expression
operator+( const bear::expr::base_linear_expression& e, double v );
EXPR_EXPORT bear::expr::linear_expression
operator-( const bear::expr::base_linear_expression& e, double v );
EXPR_EXPORT bear::expr::linear_expression
operator*( const bear::expr::base_linear_expression& e, double v );
EXPR_EXPORT bear::expr::linear_expression
operator/( const bear::expr::base_linear_expression& e, double v );

#endif // __EXPR_LINEAR_EXPRESSION_HPP__
