/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class is an elastic link between two items.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_LINK_HPP__
#define __UNIVERSE_LINK_HPP__

#include "universe/link/base_link.hpp"

#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief This class is an elastic link between two items.
     *
     * A link makes hard to move two items away from each other.
     *
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT link:
      public base_link
    {
    public:
      link( physical_item& first_item, physical_item& second_item,
            double strength,
            universe::coordinate_type minimal_length,
            universe::coordinate_type maximal_length );
      link( const reference_point& first_item, 
            const reference_point& second_item,
            double strength, universe::coordinate_type minimal_length,
            universe::coordinate_type maximal_length );

      virtual void adjust();

    private:
      /** \brief The strength of the elastic. */
      const double m_strength;

      /** \brief The minimum length of the link. */
      const universe::coordinate_type m_minimal_length;

      /** \brief The maximum length of the link. */
      const universe::coordinate_type m_maximal_length;
    }; // class link
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_LINK_HPP__
