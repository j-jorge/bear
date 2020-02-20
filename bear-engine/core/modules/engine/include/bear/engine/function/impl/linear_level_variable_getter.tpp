/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::linear_level_variable_getter class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
template<typename T>
bear::engine::linear_level_variable_getter<T>::linear_level_variable_getter()
{
  this->set_default_value(0);
} // linear_level_variable_getter::linear_level_variable_getter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
template<typename T>
bear::engine::linear_level_variable_getter<T>::linear_level_variable_getter
( const level* lvl, const std::string& var_name )
{
  this->set_level(lvl);
  this->set_name(var_name);
  this->set_default_value(false);
} // linear_level_variable_getter::linear_level_variable_getter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
template<typename T>
bear::engine::linear_level_variable_getter<T>*
bear::engine::linear_level_variable_getter<T>::clone() const
{
  return new linear_level_variable_getter(*this);
} // linear_level_variable_getter::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
template<typename T>
double bear::engine::linear_level_variable_getter<T>::evaluate() const
{
  return (double)(*this)();
} // linear_level_variable_getter::evaluate()
