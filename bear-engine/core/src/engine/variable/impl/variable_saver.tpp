/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the template methods of the
 *        bear::engine::variable_saver class.
 * \author Julien Jorge
 */

#include "engine/variable/type_to_string.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Save a variable's value.
 * \param name The name of the variable.
 * \param value The value of the variable.
 */
template<typename T>
void bear::engine::variable_saver::operator()
( const std::string& name, const T& value ) const
{
  if ( boost::regex_match(name, m_pattern) )
    m_output << type_to_string<T>::value << " \"" << escape(name) << "\" = \""
             << escape(value) << "\";" << std::endl;
} // variable_saver::operator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Escape value to avoid ambiguity.
 * \param value The value to escape.
 * \remark There is nothing to do by default.
 */
template<typename T>
const T& bear::engine::variable_saver::escape( const T& value ) const
{
  return value;
} // variable_saver::escape()
