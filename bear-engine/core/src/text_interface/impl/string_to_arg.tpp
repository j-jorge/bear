/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::text_interface::string_to_arg class.
 * \author Julien Jorge.
 */

#include "text_interface/argument_converter.hpp"

#include <sstream>
#include <stdexcept>

#include <claw/logger.hpp>
#include <claw/string_algorithm.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert an argument using the default conversion.
 * \param c (ignored) The argument converter.
 * \param arg The argument to convert.
 */
template<typename T>
typename bear::text_interface::string_to_arg_helper<T, true>::result_type
bear::text_interface::string_to_arg_helper<T, true>::convert_argument
( const argument_converter& c, const std::string& arg )
{
  result_type result;
  std::istringstream iss(arg);
  iss >> result;

  if ( !iss || (iss.rdbuf()->in_avail() != 0) )
    {
      claw::logger << claw::log_warning << "Can't convert '" << arg << "'"
                   << std::endl;
      throw std::invalid_argument("Can't convert '" + arg + "'");
    }

  return result;
} // string_to_arg_helper::convert_argument()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert an argument using the implementation of the sub classe of the
 *        argument converter.
 * \param c The argument converter.
 * \param arg The argument to convert.
 */
template<typename T>
typename bear::text_interface::string_to_arg_helper<T, false>::result_type
bear::text_interface::string_to_arg_helper<T, false>::convert_argument
( const argument_converter& c, const std::string& arg )
{
  return c.do_convert_argument(arg, typeid(T)).cast_to<T>();
} // string_to_arg_helper::convert_argument()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert an argument using the implementation of the sub classe of the
 *        argument converter.
 * \param c The argument converter.
 * \param arg The argument to convert.
 */
template<typename T>
typename bear::text_interface::string_to_arg_helper<T&, false>::result_type
bear::text_interface::string_to_arg_helper<T&, false>::convert_argument
( const argument_converter& c, const std::string& arg )
{
  converted_argument converted = c.do_convert_argument(arg, typeid(T*));

  T* result = converted.cast_to<T*>();

  if ( result == NULL )
    throw std::invalid_argument("Can't convert '" + arg + "'");

  return *result;
} // string_to_arg_helper::convert_argument()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert an argument using the implementation of the sub classe of the
 *        argument converter.
 * \param c The argument converter.
 * \param arg The argument to convert.
 */
template<typename T>
typename
bear::text_interface::string_to_arg_helper<const T&, false>::result_type
bear::text_interface::string_to_arg_helper<const T&, false>::convert_argument
( const argument_converter& c, const std::string& arg )
{
  converted_argument converted = c.do_convert_argument(arg, typeid(const T*));

  const T* result = converted.cast_to<T*>();

  if ( result == NULL )
    throw std::invalid_argument("Can't convert '" + arg + "'");

  return *result;
} // string_to_arg_helper::convert_argument()




/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a string to the equivalent sequence of value to pass as an
 *        argument for a character method.
 * \param c The context on which the conversion is done.
 * \param arg The string representation of the value.
 */
template<typename Sequence>
typename
bear::text_interface::string_to_sequence_arg<Sequence>::result_type
bear::text_interface::string_to_sequence_arg<Sequence>::convert_argument
( const argument_converter& c, const std::string& arg )
{
  result_type result;

  if ( !arg.empty() )
    claw::text::split( result, arg.begin()+1, arg.end(), arg[0] );

  return result;
} // string_to_sequence_arg::convert_argument()
