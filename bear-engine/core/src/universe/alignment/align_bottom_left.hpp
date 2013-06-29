/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Align a box at the bottom or on the left, according to the first
 *        touched edge.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_ALIGN_BOTTOM_LEFT_HPP__
#define __UNIVERSE_ALIGN_BOTTOM_LEFT_HPP__

#include "universe/alignment/alignment.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief Align a box at the bottom or on the left, according to the first
     *        touched edge.
     */
    class UNIVERSE_EXPORT align_bottom_left:
      public alignment
    {
    public:
      virtual void align
      ( const rectangle_type& this_box, const position_type& that_old_pos,
        rectangle_type& that_new_box ) const;

    private:
      void align_bottom
      ( const rectangle_type& this_box, rectangle_type& that_new_box,
        const claw::math::line_2d<coordinate_type>& dir ) const;

      void align_left
      ( const rectangle_type& this_box, rectangle_type& that_new_box,
        const claw::math::line_2d<coordinate_type>& dir ) const;

    }; // class align_bottom_left
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_ALIGN_BOTTOM_LEFT_HPP__
