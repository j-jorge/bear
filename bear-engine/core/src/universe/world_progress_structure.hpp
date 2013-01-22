/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class stores some informations about the progression of an item
 *        by the world.
 * \author Julien Jorge.
 */
#ifndef __UNIVERSE_WORLD_PROGRESS_STRUCTURE_HPP__
#define __UNIVERSE_WORLD_PROGRESS_STRUCTURE_HPP__

#include "universe/class_export.hpp"

#include <claw/non_copyable.hpp>
#include <list>

namespace bear
{
  namespace universe
  {
    class physical_item;
    class physical_item_state;

    /**
     * \brief This class stores some informations about the progression of an
     *        item by the world.
     * \author Julien Jorge.
     */
    class UNIVERSE_EXPORT world_progress_structure:
      public claw::pattern::non_copyable
    {
    public:
      /** \brief A list of items, the same than universe::world. */
      typedef std::list<physical_item*> item_list;

      /** \brief A list of constant items. */
      typedef std::list<const physical_item*> const_item_list;

    private:
      /** \brief Comparator to sort the neighbours in a collision by increasing
          mass and increasing intersection. */
      class lt_collision
      {
      public:
        lt_collision( const physical_item& item );

        bool operator()( const physical_item* a, const physical_item* b ) const;

      private:
        /** \brief The item with which we compute the intersection. */
        const physical_item& m_item;

      }; // classe lt_collision

    public:
      world_progress_structure( physical_item& item );

      void init();
      void deinit();

      void select();
      void unselect();
      bool is_selected() const;
      bool was_selected() const;

      void set_waiting_for_collision();
      void unset_waiting_for_collision();
      bool is_waiting_for_collision() const;

      void set_move_done();
      bool move_is_done() const;

      const physical_item_state& get_initial_state() const;

      void set_collision_neighborhood( item_list& n, double mass, double area );
      void swap_collision_neighborhood( item_list& n );
      double get_collision_mass() const;
      double get_collision_area() const;

      void meet( physical_item* item );
      bool has_met( const physical_item* item ) const;

      physical_item* pick_next_neighbor();

      bool update_collision_penetration();

    private:
      /** \brief The item that can be selected. */
      physical_item& m_item;

      /** \brief The initial state of the item. */
      physical_item_state* m_initial_state;

      /** \brief Tell if the item is selected. */
      bool m_is_selected;

      /** \brief Tell if the item was previously selected. */
      bool m_was_selected;

      /** \brief Tell if the move of the item has been done. */
      bool m_move_is_done;

      /** \brief Tell if the item is in the queue for collision detection. */
      bool m_is_waiting_for_collision;

      /** \brief The list of items colliding with this one. */
      item_list m_collision_neighborhood;

      /** \brief The largest mass of the items in
          \a m_collision_neighborhood. */
      double m_collision_mass;

      /** \brief The largest area of the items in
          \a m_collision_neighborhood with the same mass
          \a m_collision_mass. */
      double m_collision_area;

      /** \brief The set of items with which the collision has already been
          processed. */
      const_item_list m_already_met;

    }; // class world_progress_structure
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_WORLD_PROGRESS_STRUCTURE_HPP__
