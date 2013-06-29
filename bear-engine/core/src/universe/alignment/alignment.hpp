/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Base class for rectangle alignment.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_ALIGNMENT_HPP__
#define __UNIVERSE_ALIGNMENT_HPP__

#include <claw/math.hpp>
#include "universe/types.hpp"
#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief Base class for rectangle alignment.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT alignment
    {
    public:
      virtual ~alignment();

      virtual void align
      ( const rectangle_type& this_box, const position_type& that_old_pos,
        rectangle_type& that_new_box ) const;

    }; // class alignment
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_ALIGNMENT_HPP__
