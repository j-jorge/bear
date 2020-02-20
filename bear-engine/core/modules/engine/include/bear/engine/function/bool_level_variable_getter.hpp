/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A boolean expression that returns the value of a level variable.
 * \author Sébastien Angibaud
 */
#ifndef __BEAR_BOOL_LEVEL_VARIABLE_GETTER_HPP__
#define __BEAR_BOOL_LEVEL_VARIABLE_GETTER_HPP__

#include "bear/engine/function/level_variable_getter.hpp"
#include "bear/expr/boolean_function.hpp"
#include "bear/expr/base_boolean_expression.hpp"

#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief A boolean expression that returns the value of a level variable.
     *
     * The valid fields for this item are
     *  - name The name of the variable to get,
     *  - default_value The default value of the variable, if not set,
     *  - any field supported by the parent classes.
     *
     * \author Sébastien Angibaud
     */
    class ENGINE_EXPORT bool_level_variable_getter:
      public expr::base_boolean_expression,
      public level_variable_getter<bool>
    {
    public:
      /** \brief The type of the parent class. */
      typedef expr::base_boolean_expression super;

    public:
      bool_level_variable_getter();
      bool_level_variable_getter
      (const level* lvl, const std::string& var_name);

      bool_level_variable_getter* clone() const;
      bool evaluate() const;

      std::string formatted_string() const;

    }; // class bool_level_variable_getter
  } // namespace engine
} // namespace bear

#endif // __BEAR_BOOL_LEVEL_VARIABLE_GETTER_HPP__
