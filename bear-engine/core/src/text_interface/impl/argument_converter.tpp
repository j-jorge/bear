/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the template methods of the
 *        bear::text_interface::argument_converter class.
 * \author Julien Jorge.
 */

#include "text_interface/string_to_arg.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a string representation of a value into a value of a given
 *        type.
 * \param arg The argument to convert.
 */
template<typename T>
typename
bear::text_interface::argument_converter::conversion_result<T>::result_type
bear::text_interface::argument_converter::convert_argument
( const std::string& arg ) const
{
  return string_to_arg<T>::convert_argument(*this, arg);
} // argument_converter::convert_argument()
