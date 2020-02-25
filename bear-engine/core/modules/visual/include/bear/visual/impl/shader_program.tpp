/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the template methods of
 *        bear::visual::shader_program.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the value of a variable of the program.
 * \param name The name of the variable.
 * \param value The value of the variable.
 */
template<typename T>
void bear::visual::shader_program::set_variable
( const std::string& name, const T& value )
{
  m_input_variable.set<T>( name, value );
} // shader_program::set_variable()
