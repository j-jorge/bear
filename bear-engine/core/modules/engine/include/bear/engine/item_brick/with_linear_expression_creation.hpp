/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An utility class to serve as a base class for items that can
 *        instanciate linear expressions.
 * \author Julien Jorge
 */
#ifndef __ENGINE_WITH_LINEAR_EXPRESSION_CREATION_HPP__
#define __ENGINE_WITH_LINEAR_EXPRESSION_CREATION_HPP__

#include "bear/expr/linear_expression.hpp"

#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief An utility class to serve as a base class for items that can
     *        instanciate linear expressions.
     *
     * \author Julien Jorge
     */
    class ENGINE_EXPORT with_linear_expression_creation
    {
    public:
      /** \brief The type of the expression created. */
      typedef expr::linear_expression expression_type;

    public:
      virtual ~with_linear_expression_creation();

      expr::linear_expression get_expression() const;

    private:
      virtual expr::linear_expression do_get_expression() const = 0;

    }; // class with_linear_expression_creation

  } // namespace engine
} // namespace bear

#endif // __ENGINE_WITH_LINEAR_EXPRESSION_CREATION_HPP__
