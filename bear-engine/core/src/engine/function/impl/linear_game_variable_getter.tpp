/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::linear_game_variable_getter class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
template<typename T>
bear::engine::linear_game_variable_getter<T>::linear_game_variable_getter()
{
  this->set_default_value(0);
} // linear_game_variable_getter::linear_game_variable_getter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
template<typename T>
bear::engine::linear_game_variable_getter<T>*
bear::engine::linear_game_variable_getter<T>::clone() const
{
  return new linear_game_variable_getter(*this);
} // linear_game_variable_getter::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
template<typename T>
double bear::engine::linear_game_variable_getter<T>::evaluate() const
{
  return (double)(*this)();
} // linear_game_variable_getter::evaluate()
