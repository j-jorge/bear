/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class stores the needs for repairing a collision.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_COLLISION_REPAIR_HPP__
#define __UNIVERSE_COLLISION_REPAIR_HPP__

#include "bear/universe/types.hpp"

#include "bear/universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    class physical_item_state;

    /**
     * \brief This class stores the needs for repairing a collision.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT collision_repair
    {
    public:
      collision_repair
        ( physical_item_state& first_item, physical_item_state& second_item );

      void
      set_contact_normal
        ( const physical_item_state& ref, const vector_type& normal );

      void apply();

    private:
      void apply_force_transfert();

    private:
      /** \brief The first item in the collision. */
      physical_item_state& m_first_item;

      /** \brief The second item in the collision. */
      physical_item_state& m_second_item;

      /** \brief The vector normal of the contact. */
      vector_type m_contact_normal;

      /** \brief The item to use as the reference for the contact normal. */
      physical_item_state* m_contact_reference;

    }; // class collision_repair
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_COLLISION_REPAIR_HPP__
