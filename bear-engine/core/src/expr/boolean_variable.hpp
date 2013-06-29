/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A boolean expression made of a boolean variable.
 * \author Julien Jorge
 */
#ifndef __EXPR_BOOLEAN_VARIABLE_HPP__
#define __EXPR_BOOLEAN_VARIABLE_HPP__

#include "expr/base_boolean_expression.hpp"

namespace bear
{
  namespace expr
  {
    /**
     * \brief A boolean expression made of a boolean variable.
     * \author Julien Jorge
     */
    class EXPR_EXPORT boolean_variable:
      public base_boolean_expression
    {
    public:
      boolean_variable( const bool& v );

      base_boolean_expression* clone() const;
      bool evaluate() const;

    private:
      /** \brief The value of the variable. */
      const bool& m_value;

    }; // class boolean_variable

  } // namespace expr
} // namespace bear

#endif // __EXPR_BOOLEAN_VARIABLE_HPP__
