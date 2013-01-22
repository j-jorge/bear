/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::text_interface::auto_converter class.
 * \author Julien Jorge.
 */
#include "text_interface/auto_converter.hpp"

#include <algorithm>
#include <claw/functional.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the names of the arguments.
 */
std::vector<std::string>
bear::text_interface::auto_converter::get_arguments() const
{
  std::vector<std::string> result( m_args.size() );
  std::transform
    ( m_args.begin(), m_args.end(), result.begin(),
      claw::const_pair_first<arg_map_type::value_type>() );
  return result;
} // auto_converter::get_arguments()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert an argument into its value.
 * \param arg The string representation of the argument.
 * \param type The expected result type.
 */
bear::text_interface::converted_argument
bear::text_interface::auto_converter::do_convert_argument
( const std::string& arg, const std::type_info& type ) const
{
  arg_map_type::const_iterator it = m_args.find(arg);
  if ( it != m_args.end() )
    return it->second;
  else
    throw std::invalid_argument("Can't convert '" + arg + "'");;
} // auto_converter::do_convert_argument()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an argument value at the end of the argument list.
 * \param arg The value of the argument.
 */
void bear::text_interface::auto_converter::push( converted_argument argv )
{
  std::ostringstream oss;
  oss << "arg" << m_args.size();
  m_args[oss.str()] = argv;
} // auto_converter::push()
