/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An utility class to serve as a base class for items that can
 *        instanciate boolean expressions.
 * \author Julien Jorge
 */
#ifndef __ENGINE_WITH_BOOLEAN_EXPRESSION_CREATION_HPP__
#define __ENGINE_WITH_BOOLEAN_EXPRESSION_CREATION_HPP__

#include "bear/expr/boolean_expression.hpp"

#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief An utility class to serve as a base class for items that can
     *        instanciate boolean expressions.
     *
     * \author Julien Jorge
     */
    class ENGINE_EXPORT with_boolean_expression_creation
    {
    public:
      /** \brief The type of the expression created. */
      typedef expr::boolean_expression expression_type;

    public:
      virtual ~with_boolean_expression_creation();

      expr::boolean_expression get_expression() const;

    private:
      virtual expr::boolean_expression do_get_expression() const = 0;

    }; // class with_boolean_expression_creation

  } // namespace engine
} // namespace bear

#endif // __ENGINE_WITH_BOOLEAN_EXPRESSION_CREATION_HPP__
