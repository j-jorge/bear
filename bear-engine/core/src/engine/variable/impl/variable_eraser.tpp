/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the template methods of the
 *        bear::engine::variable_eraser class.
 * \author Julien Jorge
 */

#include "engine/variable/type_to_string.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Erase a variable if the pattern match her name.
 * \param name The name of the variable.
 * \param value The value of the variable.
 */
template<typename T>
void bear::engine::variable_eraser::operator()
( const std::string& name, const T& value ) const
{
  if ( boost::regex_match(name, m_pattern) )
    m_map.erase<T>(name);
} // variable_eraser::operator()
