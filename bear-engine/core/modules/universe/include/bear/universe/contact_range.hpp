/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Stores the informations related to the length of a contact.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_CONTACT_RANGE_HPP__
#define __UNIVERSE_CONTACT_RANGE_HPP__

#include "bear/universe/types.hpp"

#include "bear/universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief The contact_range class describes the length of a contact.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT contact_range
    {
    public:
      contact_range( coordinate_type min = 0, coordinate_type max = 0 );

      coordinate_type get_min() const;
      coordinate_type get_max() const;

      void clear();

      bool empty() const;

    private:
      /** \brief The coordinate where the contact starts. */
      coordinate_type m_min;

      /** \brief The coordinate where the contact ends. */
      coordinate_type m_max;

    }; // class contact_range
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_CONTACT_RANGE_HPP__
