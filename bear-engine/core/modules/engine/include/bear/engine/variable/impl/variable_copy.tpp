/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the template methods of the
 *        bear::engine::variable_copy class.
 * \author Sebastien Angibaud
 */

#include "bear/engine/variable/type_to_string.hpp"
#include "bear/engine/variable/variable.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Save a variable's value.
 * \param name The name of the variable.
 * \param value The value of the variable.
 */
template<typename T>
void bear::engine::variable_copy::operator()
( const std::string& name, const T& value ) const
{
  if ( boost::regex_match(name, m_pattern) )
    {
        bear::engine::variable<T> var( escape(name), escape(value) );
        var.assign_value_to(m_vars);
    }
} // variable_copy::operator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Escape value to avoid ambiguity.
 * \param value The value to escape.
 * \remark There is nothing to do by default.
 */
template<typename T>
const T& bear::engine::variable_copy::escape( const T& value ) const
{
  return value;
} // variable_copy::escape()
