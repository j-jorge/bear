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
#ifndef __EXPR_BOOLEAN_FUNCTION_HPP__
#define __EXPR_BOOLEAN_FUNCTION_HPP__

#include "expr/base_boolean_expression.hpp"

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
    class boolean_function:
      public base_boolean_expression
    {
    public:
      boolean_function();
      boolean_function
      ( const boolean_function<FunctionType, PointerType>& that);
      boolean_function( const PointerType& inst, FunctionType m );

      base_boolean_expression* clone() const;
      bool evaluate() const;

      void set_function( FunctionType m );

    private:
      /** \brief The class on which we call the function. */
      const PointerType m_value;

      /** \brief The member function to call. */
      FunctionType m_function;

    }; // class boolean_function

    /**
     * \brief An expression returning the result of a call of a function.
     *
     * The evaluation of this expression is m(), where \a m is the function
     * passed to the constructor.
     *
     * \author Julien Jorge
     */
    template<typename FunctionType>
    class boolean_function<FunctionType, void>:
      public base_boolean_expression
    {
    public:
      boolean_function();
      boolean_function( FunctionType m );

      base_boolean_expression* clone() const;
      bool evaluate() const;

      void set_function( FunctionType m );

    private:
      /** \brief The member function to call. */
      FunctionType m_function;

    }; // class boolean_function

    template<typename FunctionType, typename PointerType>
    boolean_function<FunctionType, PointerType>
    boolean_function_maker( const PointerType& inst, FunctionType m);

    template<typename FunctionType>
    boolean_function<FunctionType, void>
    boolean_function_maker( FunctionType m);

  } // namespace expr
} // namespace bear

#include "expr/impl/boolean_function.tpp"

#endif // __EXPR_BOOLEAN_FUNCTION_HPP__
