/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An expression returning the result of a call of a function on any
 *        class instance.
 * \author Julien Jorge
 */
#ifndef __EXPR_LINEAR_FUNCTION_HPP__
#define __EXPR_LINEAR_FUNCTION_HPP__

#include "expr/base_linear_expression.hpp"

namespace bear
{
  namespace expr
  {
    /**
     * \brief An expression returning the result of a call of a function on any
     *        class instance.
     *
     * The evaluation of this expression is m(*inst), where \a m is the function
     * passed to the constructor, and \a inst the pointer passed to the
     * constructor.
     *
     * \author Julien Jorge
     */
    template<typename FunctionType, typename PointerType>
    class linear_function:
      public base_linear_expression
    {
    public:
      linear_function( const PointerType& inst, FunctionType m );

      base_linear_expression* clone() const;
      double evaluate() const;

    private:
      /** \brief The class on which we call the function. */
      PointerType m_value;

      /** \brief The member function to call. */
      FunctionType m_function;

    }; // class linear_function

    template<typename FunctionType, typename PointerType>
    linear_function<FunctionType, PointerType>
    linear_function_maker( const PointerType& inst, FunctionType m );

  } // namespace expr
} // namespace bear

#include "expr/impl/linear_function.tpp"

#endif // __EXPR_LINEAR_FUNCTION_HPP__
