/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class stores the needs for repairing a collision.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_COLLISION_REPAIR_HPP__
#define __UNIVERSE_COLLISION_REPAIR_HPP__

#include "universe/types.hpp"

#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    class physical_item;

    /**
     * \brief This class stores the needs for repairing a collision.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT collision_repair
    {
    public:
      collision_repair( physical_item& first_item, physical_item& second_item );

      void
      set_contact_normal( const physical_item& ref, const vector_type& normal );

      void apply();

    private:
      void apply_force_transfert();

    private:
      /** \brief The first item in the collision. */
      physical_item& m_first_item;

      /** \brief The second item in the collision. */
      physical_item& m_second_item;

      /** \brief The vector normal of the contact. */
      vector_type m_contact_normal;

      /** \brief The item to use as the reference for the contact normal. */
      physical_item* m_contact_reference;

    }; // class collision_repair
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_COLLISION_REPAIR_HPP__
