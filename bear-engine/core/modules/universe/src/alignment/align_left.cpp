/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::align_left class.
 * \author Julien Jorge
 */
#include "bear/universe/alignment/align_left.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Align a rectangle on the left of an other.
 * \param this_box The box to which we will align the other.
 * \param that_old_pos The position from where comes the other box.
 * \param that_new_box (in/out) The box we will align.
 */
void bear::universe::align_left::align
( const rectangle_type& this_box, const position_type& that_old_pos,
  rectangle_type& that_new_box ) const
{
  that_new_box.right(this_box.left());
} // align_left::align()
