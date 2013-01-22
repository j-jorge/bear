/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the visual::text_align class.
 * \author Julien Jorge
 */
#include "visual/text_align.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Interprets a string to extract a value of the horizontal_align enum.
 * \param s The string to interpret.
 * \param default_value The value to return if s could not be understood.
 */
bear::visual::text_align::horizontal_align
bear::visual::text_align::horizontal_align_from_string
( std::string s, horizontal_align default_value )
{
  if ( s == "align_left" )
    return align_left;
  else if ( s == "align_center" )
    return align_center;
  else if ( s == "align_right" )
    return align_right;
  else
    return default_value;
} // text_align::horizontal_align_from_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Interprets a string to extract a value of the vertical_align enum.
 * \param s The string to interpret.
 * \param default_value The value to return if s could not be understood.
 */
bear::visual::text_align::vertical_align
bear::visual::text_align::vertical_align_from_string
( std::string s, vertical_align default_value )
{
  if ( s == "align_top" )
    return align_top;
  else if ( s == "align_middle" )
    return align_middle;
  else if ( s == "align_bottom" )
    return align_bottom;
  else
    return default_value;
} // text_align::vertical_align_from_string()
