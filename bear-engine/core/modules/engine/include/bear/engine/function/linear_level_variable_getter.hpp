/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A linear expression that returns the value of a level variable.
 * \author Julien Jorge
 */
#ifndef __BEAR_LINEAR_LEVEL_VARIABLE_GETTER_HPP__
#define __BEAR_LINEAR_LEVEL_VARIABLE_GETTER_HPP__

#include "bear/engine/function/level_variable_getter.hpp"
#include "bear/expr/linear_function.hpp"
#include "bear/expr/base_linear_expression.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief A linear expression that returns the value of a level variable.
     *
     * The valid fields for this item are
     *  - name The name of the variable to get,
     *  - default_value The default value of the variable, if not set,
     *  - any field supported by the parent classes.
     *
     * \author Julien Jorge
     */
    template<typename T>
    class linear_level_variable_getter:
      public expr::base_linear_expression,
      public level_variable_getter<T>
    {
    public:
      /** \brief The type of the parent class. */
      typedef expr::base_linear_expression super;

    public:
      linear_level_variable_getter();
      linear_level_variable_getter
      (const level* lvl, const std::string& var_name);

      linear_level_variable_getter<T>* clone() const;
      double evaluate() const;

    }; // class linear_level_variable_getter
  } // namespace engine
} // namespace bear

#include "bear/engine/function/impl/linear_level_variable_getter.tpp"

#endif // __BEAR_LINEAR_LEVEL_VARIABLE_GETTER_HPP__
