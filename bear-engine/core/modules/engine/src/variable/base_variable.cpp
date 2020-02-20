/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::base_variable class.
 * \author Julien Jorge
 */
#include "bear/engine/variable/base_variable.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the variable.
 */
bear::engine::base_variable::base_variable( const std::string& name )
  : m_name(name)
{

} // base_variable::base_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the variable.
 */
const std::string& bear::engine::base_variable::get_name() const
{
  return m_name;
} // base_variable::get_name()
