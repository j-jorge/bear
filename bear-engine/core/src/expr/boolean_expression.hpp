/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A boolean expression.
 * \author Julien Jorge
 */
#ifndef __EXPR_BOOLEAN_EXPRESSION_HPP__
#define __EXPR_BOOLEAN_EXPRESSION_HPP__

#include "expr/class_export.hpp"

namespace bear
{
  namespace expr
  {
    class base_boolean_expression;

    /**
     * \brief A boolean expression.
     * \author Julien Jorge
     */
    class EXPR_EXPORT boolean_expression
    {
    public:
      /** \brief The base class for boolean expressions. */
      typedef base_boolean_expression base_expression_type;

    public:
      boolean_expression();
      boolean_expression( const base_boolean_expression& e );
      boolean_expression( const boolean_expression& that );
      ~boolean_expression();

      bool evaluate() const;
      operator bool() const;

      boolean_expression& operator=( const boolean_expression& that );

      boolean_expression operator==( const boolean_expression& that ) const;
      boolean_expression operator!=( const boolean_expression& that ) const;
      boolean_expression operator&&( const boolean_expression& that ) const;
      boolean_expression operator||( const boolean_expression& that ) const;
      boolean_expression operator^( const boolean_expression& that ) const;

    private:
      /** \brief The implemented expression. */
      base_boolean_expression* m_expr;

    }; // class boolean_expression

  } // namespace expr
} // namespace bear

EXPR_EXPORT bear::expr::boolean_expression
operator!( const bear::expr::boolean_expression& that );

#endif // __EXPR_BOOLEAN_EXPRESSION_HPP__
