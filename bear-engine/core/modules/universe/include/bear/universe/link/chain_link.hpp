/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class is a chain link between two items.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_CHAIN_LINK_HPP__
#define __UNIVERSE_CHAIN_LINK_HPP__

#include "bear/universe/link/base_link.hpp"

#include "bear/universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief This class is a chain chain_link between two items.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT chain_link:
      public base_link
    {
    public:
      chain_link( physical_item& first_item, physical_item& second_item,
                  universe::coordinate_type minimal_length,
                  universe::coordinate_type maximal_length );
      chain_link
        ( const reference_point& first_item, 
          const reference_point& second_item,
          universe::coordinate_type minimal_length,
          universe::coordinate_type maximal_length );
      virtual void adjust();

    private:
      /** \brief The minimum length of the link. */
      const universe::coordinate_type m_minimal_length;

      /** \brief The maximum length of the link. */
      const universe::coordinate_type m_maximal_length;

    }; // class chain_link
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_CHAIN_LINK_HPP__
