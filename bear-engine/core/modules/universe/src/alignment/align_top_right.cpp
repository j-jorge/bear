/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief implementation of the bear::universe::align_top_right class.
 * \author Julien Jorge
 */
#include "bear/universe/alignment/align_top_right.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Align a box at the top or on the right, according to the first
 *        touched edge.
 * \param this_box The box to which we will align the other.
 * \param that_old_pos The position from where comes the other box.
 * \param that_new_box (in/out) The position of the box we will align.
 */
void bear::universe::align_top_right::align
( const bear::universe::rectangle_type& this_box,
  const bear::universe::position_type& that_old_pos,
  bear::universe::rectangle_type& that_new_box ) const
{
  claw::math::line_2d<coordinate_type> dir, ortho;
  bear::universe::position_type inter;

  dir.origin = that_old_pos;
  dir.direction = that_old_pos - that_new_box.bottom_left();

  ortho.origin = this_box.top_right();
  ortho.direction = dir.direction.get_orthonormal_anticlockwise();

  inter = dir.intersection( ortho );

  if ( inter.y < this_box.top() )
    align_right(this_box, that_new_box, dir);
  else if ( inter.y > this_box.top() )
    align_top(this_box, that_new_box, dir);
  else
    that_new_box.bottom_left( inter );
} // align_top_right::align()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align a box at the top.
 * \param this_box The box to which we will align the other.
 * \param that_new_box (in/out) The position of the box we will align.
 * \param dir A line representing the movement of the top right corner of the
 *        other box.
 */
void bear::universe::align_top_right::align_top
( const bear::universe::rectangle_type& this_box,
  bear::universe::rectangle_type& that_new_box,
  const claw::math::line_2d<coordinate_type>& dir ) const
{
  claw::math::line_2d<coordinate_type> edge;
  bear::universe::position_type inter;

  edge.origin = this_box.top_left();
  edge.direction.x = 1;
  edge.direction.y = 0;

  inter = dir.intersection( edge );

  that_new_box.bottom_left( inter );
} // align_top_right::align_top

/*----------------------------------------------------------------------------*/
/**
 * \brief Align a box on the right.
 * \param this_box The box to which we will align the other.
 * \param that_new_box (in/out) The position of the box we will align.
 * \param dir A line representing the movement of the top right corner of the
 *        other box.
 */
void bear::universe::align_top_right::align_right
( const bear::universe::rectangle_type& this_box,
  bear::universe::rectangle_type& that_new_box,
  const claw::math::line_2d<coordinate_type>& dir ) const
{
  claw::math::line_2d<coordinate_type> edge;
  bear::universe::position_type inter;

  edge.origin = this_box.top_right();
  edge.direction.x = 0;
  edge.direction.y = 1;

  inter = edge.intersection( dir );

  that_new_box.bottom_left( inter );
} // align_top_right::align_right
