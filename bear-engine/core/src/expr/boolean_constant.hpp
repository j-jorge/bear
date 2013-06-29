/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A boolean expression made of a boolean constant.
 * \author Julien Jorge
 */
#ifndef __EXPR_BOOLEAN_CONSTANT_HPP__
#define __EXPR_BOOLEAN_CONSTANT_HPP__

#include "expr/base_boolean_expression.hpp"

namespace bear
{
  namespace expr
  {
    /**
     * \brief A boolean expression made of a boolean constant.
     * \author Julien Jorge
     */
    class EXPR_EXPORT boolean_constant:
      public base_boolean_expression
    {
    public:
      boolean_constant( bool v );

      base_boolean_expression* clone() const;
      bool evaluate() const;

      void set_value( bool b );

    private:
      /** \brief The value of the constant. */
      bool m_value;

    }; // class boolean_constant

  } // namespace expr
} // namespace bear

#endif // __EXPR_BOOLEAN_CONSTANT_HPP__
