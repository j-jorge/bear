/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the trinary_logic class.
 * \author Sebastien Angibaud
 */
#include "bf/trinary_logic.hpp"
#include <cstdlib>

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert an action into its string representation.
 * \param value The boolean to convert.
 */
std::string bf::trinary_logic::to_string( value_type value )
{
  std::string result("boolean_random");

  switch(value)
    {
    case boolean_true:   result = "boolean_true" ; break;
    case boolean_false:  result = "boolean_false" ; break;
    }

  return result;
} // trinary_logic::to_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a string into an action.
 * \param s The boolean to convert.
 */
bf::trinary_logic::value_type
bf::trinary_logic::from_string( const std::string& s )
{
  value_type b(boolean_random);

  if ( s == "boolean_true" )
    b = boolean_true;
  else if ( s == "boolean_false" )
    b = boolean_false;

  return b;
} // trinary_logic::from_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a trinary boolean into boolean type.
 * Boolean random is converted into false.
 * \param value The trinary boolean to convert.
 */
bool bf::trinary_logic::to_bool( value_type value )
{
  return (value == boolean_true);
} // trinary_logic::to_bool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a boolean into trinary boolean.
 * \param value The trinary boolean to convert.
 */
bf::trinary_logic::value_type bf::trinary_logic::from_bool( bool value )
{
  if ( value )
    return boolean_true;
  else
    return boolean_false;
} // trinary_logic::from_bool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert an action into boolean type. Boolean random is converted
 * by randomize.
 * \param value The boolean to convert.
 */
bool bf::trinary_logic::to_random_bool( value_type value )
{
  bool result(false);

  switch(value)
    {
    case boolean_true:   result = true ; break;
    case boolean_random:
      return ( (2.0 * (double)rand()) / RAND_MAX ) < 1;
    }

  return result;
} // trinary_logic::to_random_bool()
