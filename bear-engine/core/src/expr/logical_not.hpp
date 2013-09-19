/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A logical not of a boolean expression.
 * \author Julien Jorge
 */
#ifndef __EXPR_LOGICAL_NOT_HPP__
#define __EXPR_LOGICAL_NOT_HPP__

#include "expr/base_boolean_expression.hpp"
#include "expr/boolean_expression.hpp"

namespace bear
{
  namespace expr
  {
    /**
     * \brief A logical not of a boolean expression.
     * \author Julien Jorge
     */
    class EXPR_EXPORT logical_not:
      public base_boolean_expression
    {
    public:
      logical_not();
      logical_not( const boolean_expression& op );

      const boolean_expression& get_operand() const;
      void set_operand( const boolean_expression& op );

      base_boolean_expression* clone() const;
      bool evaluate() const;

      std::string formatted_string() const;

    private:
      /** \brief The operand. */
      boolean_expression m_operand;

    }; // class logical_not

  } // namespace expr
} // namespace bear

#endif // __EXPR_LOGICAL_NOT_HPP__
