/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Find some informations on how the scene was when a collision occured.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_COLLISION_INFO_HPP__
#define __UNIVERSE_COLLISION_INFO_HPP__

#include "bear/universe/zone.hpp"
#include "bear/universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    class alignment;
    class collision_repair;
    class physical_item_state;

    /**
     * \brief Find some informations on how the scene was when a collision
     *        occured.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT collision_info
    {
    public:
      collision_info( const physical_item_state& previous_self,
                      const physical_item_state& previous_that,
                      physical_item_state& self, physical_item_state& that,
                      collision_repair& repair );

      zone::position get_collision_side() const;
      const position_type& get_bottom_left_on_contact() const;

      const physical_item_state& other_previous_state() const;
      const physical_item_state& reference_previous_state() const;

      physical_item_state& other_item() const;

      collision_repair& get_collision_repair() const;

    private:
      alignment* find_alignment() const;

      void apply_alignment
        ( const alignment& align, const physical_item_state& self );

    private:
      /** \brief The previous state of the item considered as reference in the
          collision. */
      const physical_item_state& m_previous_self;

      /** \brief The previous state of the other item in the collision. */
      const physical_item_state& m_previous_other;

      /** \brief The other item in the collision. */
      physical_item_state& m_other;

      /** \brief Position of the bottom-left corner of the other item when the
          collision occured. */
      position_type m_position_on_contact;

      /** \brief Side of the reference item hit first when the collision
          occured. */
      zone::position m_side;

      /** \brief Informations for repairing the collision. */
      collision_repair& m_repair;

    }; // class collision_info
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_COLLISION_INFO_HPP__
