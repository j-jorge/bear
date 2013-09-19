/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A logical xor of two boolean expressions.
 * \author Julien Jorge
 */
#ifndef __EXPR_LOGICAL_XOR_HPP__
#define __EXPR_LOGICAL_XOR_HPP__

#include "expr/base_boolean_expression.hpp"
#include "expr/boolean_expression.hpp"

namespace bear
{
  namespace expr
  {
    /**
     * \brief A logical xor of two boolean expressions.
     * \author Julien Jorge
     */
    class EXPR_EXPORT logical_xor:
      public base_boolean_expression
    {
    public:
      logical_xor
      ( const boolean_expression& left, const boolean_expression& right );

      base_boolean_expression* clone() const;
      bool evaluate() const;

      std::string formatted_string() const;

    private:
      /** \brief The left operand. */
      boolean_expression m_left;

      /** \brief The right operand. */
      boolean_expression m_right;

    }; // class logical_xor

  } // namespace expr
} // namespace bear

#endif // __EXPR_LOGICAL_XOR_HPP__
