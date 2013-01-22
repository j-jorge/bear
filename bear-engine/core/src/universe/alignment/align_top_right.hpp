/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Align a box at the top or on the right, according to the first
 *        touched edge.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_ALIGN_TOP_RIGHT_HPP__
#define __UNIVERSE_ALIGN_TOP_RIGHT_HPP__

#include "universe/alignment/alignment.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief Align a box at the top or on the right, according to the first
     *        touched edge.
     */
    class UNIVERSE_EXPORT align_top_right:
      public alignment
    {
    public:
      virtual void align
      ( const rectangle_type& this_box, const position_type& that_old_pos,
        rectangle_type& that_new_box ) const;

    private:
      void align_top
      ( const rectangle_type& this_box, rectangle_type& that_new_box,
        const claw::math::line_2d<coordinate_type>& dir ) const;

      void align_right
      ( const rectangle_type& this_box, rectangle_type& that_new_box,
        const claw::math::line_2d<coordinate_type>& dir ) const;

    }; // class align_top_right
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_ALIGN_TOP_RIGHT_HPP__
