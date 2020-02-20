/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Check, in a collision context, if the bottom of the "that" item was
 *        lower than a given value, relative to the top of the "self" item.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_BOTTOM_CONTACT_IS_LOWER_HPP__
#define __UNIVERSE_BOTTOM_CONTACT_IS_LOWER_HPP__

#include "bear/universe/collision_info.hpp"
#include "bear/universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief Check, in a collision context, if the bottom of the "that" item
     *        was lower than a given value, relative to the top of the "self"
     *        item.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT bottom_contact_is_lower
    {
    public:
      /** \brief The type of the line that separate the block into two
          regions. */
      typedef claw::math::line_2d<coordinate_type> line_type;

    public:
      bottom_contact_is_lower( const line_type& line );
      bottom_contact_is_lower( coordinate_type val );

      bool operator()( const collision_info& info, physical_item& self,
                       physical_item& that ) const;

    private:
      /** \brief We will check if the bottom of the other item was in the part
          at the top of this line. */
      const line_type m_line;

    }; // class bottom_contact_is_lower
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_BOTTOM_CONTACT_IS_LOWER_HPP__
