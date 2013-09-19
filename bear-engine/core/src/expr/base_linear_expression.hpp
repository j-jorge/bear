/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The base class for a linear expression.
 * \author Julien Jorge
 */
#ifndef __EXPR_BASE_LINEAR_EXPRESSION_HPP__
#define __EXPR_BASE_LINEAR_EXPRESSION_HPP__

#include <string>

#include "expr/class_export.hpp"

namespace bear
{
  namespace expr
  {
    /**
     * \brief The base class for a linear expression.
     * \author Julien Jorge
     */
    class EXPR_EXPORT base_linear_expression
    {
    public:
      /** \brief The type of the result of the evaluation. */
      typedef double result_type;

    public:
      virtual ~base_linear_expression() {};

      virtual base_linear_expression* clone() const = 0;
      virtual result_type evaluate() const = 0;

      std::string formatted_string() const { return ""; }

    }; // class base_linear_expression

  } // namespace expr
} // namespace bear

#endif // __EXPR_BASE_LINEAR_EXPRESSION_HPP__
