/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief implementation of the bear::universe::align_top_left class.
 * \author Julien Jorge
 */
#include "universe/alignment/align_top_left.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Align a box at the top or on the left, according to the first
 *        touched edge.
 * \param this_box The box to which we will align the other.
 * \param that_old_pos The position from where comes the other box.
 * \param that_new_box (in/out) The position of the box we will align.
 */
void bear::universe::align_top_left::align
( const rectangle_type& this_box, const position_type& that_old_pos,
  rectangle_type& that_new_box ) const
{
  claw::math::line_2d<coordinate_type> dir, ortho;
  position_type inter;

  dir.origin.x = that_old_pos.x + that_new_box.width();
  dir.origin.y = that_old_pos.y;
  dir.direction = dir.origin - that_new_box.bottom_right();

  ortho.origin = this_box.top_left();
  ortho.direction = dir.direction.get_orthonormal_anticlockwise();

  inter = dir.intersection( ortho );

  if ( inter.x < this_box.left() )
    align_left(this_box, that_new_box, dir);
  else if ( inter.x > this_box.left() )
    align_top(this_box, that_new_box, dir);
  else
    that_new_box.bottom_right( inter );
} // align_top_left::align()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align a box at the top.
 * \param this_box The box to which we will align the other.
 * \param that_new_box (in/out) The position of the box we will align.
 * \param dir A line representing the movement of the top right corner of the
 *        other box.
 */
void bear::universe::align_top_left::align_top
( const rectangle_type& this_box, rectangle_type& that_new_box,
  const claw::math::line_2d<coordinate_type>& dir ) const
{
  claw::math::line_2d<coordinate_type> edge;
  position_type inter;

  edge.origin = this_box.top_left();
  edge.direction.x = 1;
  edge.direction.y = 0;

  inter = dir.intersection( edge );

  that_new_box.bottom_right( inter );
} // align_top_left::align_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align a box on the left.
 * \param this_box The box to which we will align the other.
 * \param that_new_box (in/out) The position of the box we will align.
 * \param dir A line representing the movement of the top right corner of the
 *        other box.
 */
void bear::universe::align_top_left::align_left
( const rectangle_type& this_box, rectangle_type& that_new_box,
  const claw::math::line_2d<coordinate_type>& dir ) const
{
  claw::math::line_2d<coordinate_type> edge;
  position_type inter;

  edge.origin = this_box.top_left();
  edge.direction.x = 0;
  edge.direction.y = 1;

  inter = edge.intersection( dir );

  that_new_box.bottom_right( inter );
} // align_top_left::align_left()
