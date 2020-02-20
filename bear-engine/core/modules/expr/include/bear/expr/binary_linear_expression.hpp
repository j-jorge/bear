/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A logical and of two linear expressions.
 * \author Julien Jorge
 */
#ifndef __EXPR_BINARY_LINEAR_EXPRESSION_HPP__
#define __EXPR_BINARY_LINEAR_EXPRESSION_HPP__

#include "bear/expr/base_boolean_expression.hpp"
#include "bear/expr/base_linear_expression.hpp"

#include "bear/expr/binary_expression.hpp"
#include "bear/expr/linear_expression.hpp"

#include <functional>

namespace bear
{
  namespace expr
  {
    /**
     * \brief Utility class to create a binary_expression whose operands are
     *        linear_expressions.
     * \author Julien Jorge
     */
    template<typename Function, typename Base>
    class binary_linear_expression_maker
    {
    public:
      typedef binary_expression<Base, linear_expression, Function> type;
    }; // class binary_linear_expression_maker

    /**
     * \brief Utility class to create a binary_expression whose operands are
     *        linear_expressions and the result a linear_expression too.
     * \author Julien Jorge
     */
    template<typename Function>
    class linear_operation_maker:
      public binary_linear_expression_maker<Function, base_linear_expression>
    {
      // nothing
    }; // class linear_operation_maker

    /**
     * \brief Utility class to create a binary_expression whose operands are
     *        linear_expressions and the result a boolean_expression too.
     * \author Julien Jorge
     */
    template<typename Function>
    class linear_comparison_maker:
      public binary_linear_expression_maker<Function, base_boolean_expression>
    {
      // nothing
    }; // class linear_comparison_maker

    /** Add the evaluation of two expressions. */
    typedef linear_operation_maker< std::plus<double> >::type linear_plus;

    /** Subtract the evaluation of an expression to the evaluation of an
        other. */
    typedef linear_operation_maker< std::minus<double> >::type linear_minus;

    /** Multiply the evaluation of two expressions. */
    typedef
    linear_operation_maker< std::multiplies<double> >::type linear_multiplies;

    /** Divide the evaluation of an expression by the evaluation of an other. */
    typedef linear_operation_maker< std::divides<double> >::type linear_divides;

    /** Tell if the evaluation of two expressions are equal. */
    typedef
    linear_comparison_maker< std::equal_to<double> >::type linear_equality;

    /** Tell if the evaluation of two expressions are not equal. */
    typedef
    linear_comparison_maker< std::not_equal_to<double> >::type
    linear_disequality;

    /** Tell if the evaluation of an expression is lower than the evaluation of
        an other. */
    typedef linear_comparison_maker< std::less<double> >::type linear_less;

    /** Tell if the evaluation of an expression is lower or equal to the
        evaluation of an other. */
    typedef
    linear_comparison_maker< std::less_equal<double> >::type linear_less_equal;

    /** Tell if the evaluation of an expression is greater than the evaluation
        of an other. */
    typedef
    linear_comparison_maker< std::greater<double> >::type linear_greater;

    /** Tell if the evaluation of an expression is greater or equal to the
        evaluation of an other. */
    typedef
    linear_comparison_maker< std::greater_equal<double> >::type
    linear_greater_equal;

  } // namespace expr
} // namespace bear

#endif // __EXPR_BINARY_LINEAR_EXPRESSION_HPP__
