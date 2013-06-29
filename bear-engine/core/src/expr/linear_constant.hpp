/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A linear expression made of a single constant.
 * \author Julien Jorge
 */
#ifndef __EXPR_LINEAR_CONSTANT_HPP__
#define __EXPR_LINEAR_CONSTANT_HPP__

#include "expr/base_linear_expression.hpp"

namespace bear
{
  namespace expr
  {
    /**
     * \brief A linear expression made of a single constant.
     * \author Julien Jorge
     */
    class EXPR_EXPORT linear_constant:
      public base_linear_expression
    {
    public:
      linear_constant( double v );

      base_linear_expression* clone() const;
      double evaluate() const;
      void set_value( double b );

    private:
      /** \brief The value of the constant. */
      double m_value;

    }; // class linear_constant

  } // namespace expr
} // namespace bear

#endif // __EXPR_LINEAR_CONSTANT_HPP__
