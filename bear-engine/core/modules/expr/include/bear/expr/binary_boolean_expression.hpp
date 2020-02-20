/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A logical and of two boolean expressions.
 * \author Julien Jorge
 */
#ifndef __EXPR_BINARY_BOOLEAN_EXPRESSION_HPP__
#define __EXPR_BINARY_BOOLEAN_EXPRESSION_HPP__

#include "bear/expr/base_boolean_expression.hpp"
#include "bear/expr/binary_expression.hpp"
#include "bear/expr/boolean_expression.hpp"

#include <functional>

namespace bear
{
  namespace expr
  {
    /**
     * \brief Utility class to create a binary_expression whose operands are
     *        boolean_expressions.
     * \author Julien Jorge
     */
    template<typename Function>
    class logical_expression_maker
    {
    public:
      typedef
      binary_expression<base_boolean_expression, boolean_expression, Function>
      type;
    }; // class logical_expression_maker

    /** Tell if two booleans are equal. */
    typedef
    logical_expression_maker< std::equal_to<bool> >::type boolean_equality;

    /** Tell if two booleans are not equal. */
    typedef
    logical_expression_maker< std::not_equal_to<bool> >::type
    boolean_disequality;

    /** Compute the logical and value of the evaluation of two boolean
        expressions. */
    typedef
    logical_expression_maker< std::logical_and<bool> >::type logical_and;

    /** Compute the logical or value of the evaluation of two boolean
        expressions. */
    typedef logical_expression_maker< std::logical_or<bool> >::type logical_or;

  } // namespace expr
} // namespace bear

#endif // __EXPR_BINARY_BOOLEAN_EXPRESSION_HPP__
