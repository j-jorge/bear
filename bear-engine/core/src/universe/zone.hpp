/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Watching a rectangle placed in a 2D space, we can consider nine zones,
 *        described here.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_ZONE_HPP__
#define __UNIVERSE_ZONE_HPP__

#include "universe/types.hpp"

#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief Watching a rectangle placed in a 2D space, we can consider nine
     *        zones, described here.
     */
    class UNIVERSE_EXPORT zone
    {
    public:
      enum position
        {
          top_left_zone = 0,
          top_zone,
          top_right_zone,
          middle_left_zone,
          middle_zone,
          middle_right_zone,
          bottom_left_zone,
          bottom_zone,
          bottom_right_zone
        }; // enum position

      /** \brief Number of zones considered. */
      static const unsigned int cardinality;

      static position find
      ( const rectangle_type& that_box, const rectangle_type& this_box );

      static position opposite_of( position p );

    }; // class zone
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_ZONE_HPP__
