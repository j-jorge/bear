/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::method_call class.
 * \author Julien Jorge
 */
#include "bear/engine/script/method_call.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the actor on which the method is called.
 * \param name The name.
 */
void bear::engine::method_call::set_actor_name( const std::string& name )
{
  m_actor_name = name;
} // method_call::set_actor_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the actor on which the method is called.
 */
const std::string& bear::engine::method_call::get_actor_name() const
{
  return m_actor_name;
} // method_call::get_actor_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the method to call.
 * \param name The name.
 */
void bear::engine::method_call::set_method_name( const std::string& name )
{
  m_method_name = name;
} // method_call::set_method_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the method to call.
 */
const std::string& bear::engine::method_call::get_method_name() const
{
  return m_method_name;
} // method_call::get_method_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the argument values passed to the called method.
 * \param val The values of the arguments.
 */
void bear::engine::method_call::set_arguments
( const std::vector<std::string>& val )
{
  m_arguments = val;
} // method_call::set_arguments()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the values passed as arguments to the called method.
 */
const std::vector<std::string>& bear::engine::method_call::get_arguments() const
{
  return m_arguments;
} // method_call::get_arguments()
