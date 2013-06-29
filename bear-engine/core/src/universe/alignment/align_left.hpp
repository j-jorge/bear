/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Align a rectangle on the left of an other.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_ALIGN_LEFT_HPP__
#define __UNIVERSE_ALIGN_LEFT_HPP__

#include "universe/alignment/alignment.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief Align a rectangle on the left of an other.
     */
    class UNIVERSE_EXPORT align_left:
      public alignment
    {
    public:
      virtual void align
      ( const rectangle_type& this_box, const position_type& that_old_pos,
        rectangle_type& that_new_box ) const;

    }; // class align_left
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_ALIGN_LEFT_HPP__
