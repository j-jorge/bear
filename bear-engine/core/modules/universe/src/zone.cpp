/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::zone class.
 * \author Julien Jorge
 */
#include "bear/universe/zone.hpp"

#include <claw/exception.hpp>

/*----------------------------------------------------------------------------*/
const unsigned int bear::universe::zone::cardinality = 9;

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the zone where is a box.
 * \param that_box The box to locate.
 * \param this_box The considered box (the box who split the space).
 */
bear::universe::zone::position
bear::universe::zone::find
( const rectangle_type& that_box, const rectangle_type& this_box )
{
  position result;

  // he's on my left
  if ( that_box.right() <= this_box.left() )
    {
      if ( that_box.bottom() >= this_box.top() )
        result = top_left_zone;
      else if ( that_box.top() <= this_box.bottom() )
        result = bottom_left_zone;
      else
        result = middle_left_zone;
    }
  // on my right
  else if ( that_box.left() >= this_box.right() )
    {
      if ( that_box.bottom() >= this_box.top() )
        result = top_right_zone;
      else if ( that_box.top() <= this_box.bottom() )
        result = bottom_right_zone;
      else
        result = middle_right_zone;
    }
  else // in the middle
    {
      if ( that_box.bottom() >= this_box.top() )
        result = top_zone;
      else if ( that_box.top() <= this_box.bottom() )
        result = bottom_zone;
      else
        result = middle_zone;
    }

  return result;
} // zone::find()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the opposite side of a given side.
 * \param side The side from which we want the opposite.
 */
bear::universe::zone::position
bear::universe::zone::opposite_of( position side )
{
  switch(side)
  {
  case top_left_zone: return bottom_right_zone;
  case top_zone: return bottom_zone;
  case top_right_zone: return bottom_left_zone;
  case middle_left_zone: return middle_right_zone;
  case middle_zone: return middle_zone;
  case middle_right_zone: return middle_left_zone;
  case bottom_left_zone: return top_right_zone;
  case bottom_zone: return top_zone;
  case bottom_right_zone: return top_left_zone;
  }

  // this should never happen but it turns a warning off
  throw new claw::exception
    ( "bear::universe::zone::opposite_of(): Invalid position size" );
} // zone::opposite_of()
