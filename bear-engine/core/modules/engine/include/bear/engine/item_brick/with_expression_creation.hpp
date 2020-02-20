/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An utility class to determine the item brick to use as the
 *        creation class for a given expression type.
 * \author Julien Jorge
 */
#ifndef __ENGINE_WITH_EXPRESSION_CREATION_HPP__
#define __ENGINE_WITH_EXPRESSION_CREATION_HPP__

#include "bear/engine/item_brick/with_boolean_expression_creation.hpp"
#include "bear/engine/item_brick/with_linear_expression_creation.hpp"

#include <claw/meta/conditional.hpp>
#include <claw/meta/is_base_of.hpp>
#include <claw/meta/same_type.hpp>
#include <claw/meta/no_type.hpp>

namespace bear
{
  namespace engine
  {
    /**
     * \brief An utility class to determine the item brick to use as the
     *        creation class for a given expression type.
     *
     * \author Julien Jorge
     */
    template<typename Expression>
    class with_expression_creation
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
        with_boolean_expression_creation,

        typename claw::meta::if_then_else /* maybe a linear expression then? */
        <
          claw::meta::is_base_of
          <
            expr::base_linear_expression,
            Expression
          >::result,
          with_linear_expression_creation,

          typename claw::meta::if_then_else
          <
            claw::meta::same_type             /* general boolean expression. */
            <
              expr::boolean_expression,
              Expression
            >::result,
            with_boolean_expression_creation,

            typename claw::meta::if_then_else   /* general linear expression */
            <
              claw::meta::same_type
              <
                expr::linear_expression,
                Expression
              >::result,
              with_linear_expression_creation,
              claw::meta::no_type       /* I can't determine the expression. */

            >::result /* linear_expression */
          >::result /* boolean expression */
        >::result /* base_linear_expression */
      >::result /* base_boolean_expression */
      creation_class_type;

    }; // class with_expression_creation

  } // namespace engine
} // namespace bear

#endif // __ENGINE_WITH_EXPRESSION_CREATION_HPP__
