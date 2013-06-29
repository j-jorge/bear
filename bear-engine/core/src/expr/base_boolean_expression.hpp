/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The base class for a boolean expression.
 * \author Julien Jorge
 */
#ifndef __EXPR_BASE_BOOLEAN_EXPRESSION_HPP__
#define __EXPR_BASE_BOOLEAN_EXPRESSION_HPP__

#include "expr/class_export.hpp"

namespace bear
{
  namespace expr
  {
    /**
     * \brief The base class for a boolean expression.
     * \author Julien Jorge
     */
    class EXPR_EXPORT base_boolean_expression
    {
    public:
      /** \brief The type of the result of the evaluation. */
      typedef bool result_type;

    public:
      virtual ~base_boolean_expression() {};

      virtual base_boolean_expression* clone() const = 0;
      virtual result_type evaluate() const = 0;

    }; // class base_boolean_expression

  } // namespace expr
} // namespace bear

#endif // __EXPR_BASE_BOOLEAN_EXPRESSION_HPP__
