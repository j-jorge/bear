/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief implementation of the bear::universe::alignment class.
 * \author Julien Jorge
 */
#include "universe/alignment/alignment.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::universe::alignment::~alignment()
{
  // nothing to do
} // alignment::~alignment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align one box with another.
 * \param this_box The box to which we will align the other.
 * \param that_old_pos The position from where comes the other box.
 * \param that_new_box (in/out) The box we will align.
 * \remark This method does nothing.
 */
void bear::universe::alignment::align
( const bear::universe::rectangle_type& this_box,
  const bear::universe::position_type& that_old_pos,
  bear::universe::rectangle_type& that_new_box ) const
{
  that_new_box.bottom_left( that_old_pos );
} // alignment::align()
