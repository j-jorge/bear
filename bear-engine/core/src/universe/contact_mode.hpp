/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Defines how the contacts are set in a collision.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_CONTACT_MODE_HPP__
#define __UNIVERSE_CONTACT_MODE_HPP__

#include "universe/class_export.hpp"

#include <string>

namespace bear
{
  namespace universe
  {
    /**
     * \brief The contact_mode class defines how the contacts are set.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT contact_mode
    {
    public:
      /**
       * The various ways the contacts are set when aligning the items.
       */
      enum value_type
      {
        /** The contact is set on the whole side of the item. */
        full_contact,

        /** The contact is set on the range of the collision. */
        range_contact,

        /** The contact is not set at all. */
        no_contact

      }; // enum contact_mode

    public:
      
      static value_type from_string( const std::string& v );
      
    }; // class contact_mode
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_CONTACT_MODE_HPP__
