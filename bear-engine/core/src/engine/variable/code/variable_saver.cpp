/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::variable_saver class.
 * \author Julien Jorge
 */
#include "engine/variable/variable_saver.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param os The stream in which the result will be written.
 * \param pattern A regular expression that must be verified by the variable's
 *        name to be serialised.
 */
bear::engine::variable_saver::variable_saver
( std::ostream& os, boost::regex pattern )
  : m_output(os), m_pattern(pattern)
{

} // variable_saver::variable_saver()

/*----------------------------------------------------------------------------*/
/**
 * \brief Escape value to avoid ambiguity.
 * \param value The value to escape.
 */
std::string
bear::engine::variable_saver::escape( const std::string& value ) const
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
} // variable_saver::escape()
