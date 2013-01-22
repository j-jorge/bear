/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::variable_copy class.
 * \author Sebastien Angibaud
 */
#include "engine/variable/variable_copy.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param os The stream in which the result will be written.
 * \param pattern A regular expression that must be verified by the variable's
 *        name to be serialised.
 */
bear::engine::variable_copy::variable_copy
( var_map& vars, const boost::regex& pattern )
  : m_vars(vars), m_pattern(pattern)
{

} // variable_copy::variable_copy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Escape value to avoid ambiguity.
 * \param value The value to escape.
 */
std::string
bear::engine::variable_copy::escape( const std::string& value ) const
{
  std::string result;
  result.reserve(value.size());

  for (std::size_t i=0; i!=value.length(); ++i)
    switch( value[i] )
      {
      case '"':
      case '\\':
        result += '\\'; /* no break */
      default:
        result += value[i];
      }

  return result;
} // variable_copy::escape()
