/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation the the bf::stream_conv class.
 * \author Julien Jorge
 */

#include "bf/arithmetic_parser.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the value from a stream.
 * \param is The stream in which we read.
 * \param v The value we have read.
 */
template<typename Type>
std::istream& bf::stream_conv<Type>::read( std::istream& is, value_type& v )
{
  return is;
} // read()




/*----------------------------------------------------------------------------*/
/**
 * \brief Read the value from a stream.
 * \param is The stream in which we read.
 * \param v The value we have read.
 */
template<typename T>
std::istream&
bf::stream_conv< bf::custom_type<T> >::read( std::istream& is, value_type& v )
{
  std::string expr;
  std::getline( is, expr );

  const arithmetic_parser parser;
  double value;

  if ( parser.evaluate( value, expr ) )
    v.set_value( (typename value_type::value_type)value );

  return is;
} // read()
