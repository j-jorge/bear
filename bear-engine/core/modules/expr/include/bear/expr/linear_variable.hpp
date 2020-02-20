/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A linear expression made of a linear variable.
 * \author Julien Jorge
 */
#ifndef __EXPR_LINEAR_VARIABLE_HPP__
#define __EXPR_LINEAR_VARIABLE_HPP__

#include "bear/expr/base_linear_expression.hpp"

namespace bear
{
  namespace expr
  {
    /**
     * \brief A linear expression made of a linear variable.
     * \author Julien Jorge
     */
    class EXPR_EXPORT linear_variable:
      public base_linear_expression
    {
    public:
      linear_variable( const double& v );

      base_linear_expression* clone() const;
      double evaluate() const;

    private:
      /** \brief The value of the variable. */
      const double& m_value;

    }; // class linear_variable

  } // namespace expr
} // namespace bear

#endif // __EXPR_LINEAR_VARIABLE_HPP__
