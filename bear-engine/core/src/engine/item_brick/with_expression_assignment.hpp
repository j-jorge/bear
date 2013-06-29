/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An utility class to determine the item brick to use as the
 *        assignment class for a given expression type.
 * \author Julien Jorge
 */
#ifndef __ENGINE_WITH_EXPRESSION_ASSIGNMENT_HPP__
#define __ENGINE_WITH_EXPRESSION_ASSIGNMENT_HPP__

#include "engine/item_brick/with_boolean_expression_assignment.hpp"
#include "engine/item_brick/with_linear_expression_assignment.hpp"

#include <claw/meta/conditional.hpp>
#include <claw/meta/is_base_of.hpp>
#include <claw/meta/no_type.hpp>

namespace bear
{
  namespace engine
  {
    /**
     * \brief An utility class to determine the item brick to use as the
     *        assignment class for a given expression type.
     *
     * \author Julien Jorge
     */
    template<typename Expression>
    class with_expression_assignment
    {
    public:
      /** \brief The type of the class to use to instanciate Expression. */
      typedef typename
      claw::meta::if_then_else
      <
        claw::meta::is_base_of         /* check if it's a boolean expression. */
        <
          expr::base_boolean_expression,
          Expression
        >::result,
        with_boolean_expression_assignment,
        typename claw::meta::if_then_else /* maybe a linear expression then? */
        <
          claw::meta::is_base_of
          <
            expr::base_linear_expression,
            Expression
          >::result,
          with_linear_expression_assignment,
          claw::meta::no_type            /* I can't determine the expression. */
        >::result
      >::result assignment_class_type;

    }; // class with_expression_assignment

  } // namespace engine
} // namespace bear

#endif // __ENGINE_WITH_EXPRESSION_ASSIGNMENT_HPP__
