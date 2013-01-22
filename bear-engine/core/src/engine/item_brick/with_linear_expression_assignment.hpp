/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An utility class to serve as a base class for items that can
 *        receive linear expressions.
 * \author Julien Jorge
 */
#ifndef __ENGINE_WITH_LINEAR_EXPRESSION_ASSIGNMENT_HPP__
#define __ENGINE_WITH_LINEAR_EXPRESSION_ASSIGNMENT_HPP__

#include "expr/linear_expression.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief An utility class to serve as a base class for items that can
     *        receive linear expressions.
     *
     * \author Julien Jorge
     */
    class ENGINE_EXPORT with_linear_expression_assignment
    {
    public:
      /** \brief The type of the expression created. */
      typedef expr::linear_expression expression_type;

    public:
      virtual ~with_linear_expression_assignment();

      void set_expression( const expr::linear_expression& e );

    private:
      virtual void do_set_expression( const expr::linear_expression& e ) = 0;

    }; // class with_linear_expression_assignment

  } // namespace engine
} // namespace bear

#endif // __ENGINE_WITH_LINEAR_EXPRESSION_ASSIGNMENT_HPP__
