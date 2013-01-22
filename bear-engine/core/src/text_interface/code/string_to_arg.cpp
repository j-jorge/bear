/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::text_interface::string_to_arg class.
 * \author Julien Jorge.
 */
#include "text_interface/string_to_arg.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a string to the equivalent value to pass as an argument for a
 *        character method.
 * \param c The context on which the conversion is done.
 * \param arg The string representation of the value.
 */
std::string bear::text_interface::string_to_arg<std::string>::convert_argument
( const argument_converter& c, const std::string& arg )
{
  return arg;
} // string_to_arg::convert_argument()
