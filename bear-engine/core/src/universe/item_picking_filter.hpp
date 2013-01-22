/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A filter passed to bear::universe::world::pick_items to get only
 *        interesting items.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_ITEM_PICKING_FILTER_HPP__
#define __UNIVERSE_ITEM_PICKING_FILTER_HPP__

#include "universe/class_export.hpp"
#include "universe/types.hpp"

namespace bear
{
  namespace universe
  {
    class physical_item;

    /**
     * \brief A filter passed to bear::universe::world::pick_items to get only
     *        interesting items.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT item_picking_filter
    {
    public:
      item_picking_filter();
      virtual ~item_picking_filter();

      bool satisfies_condition( const physical_item& item ) const;

      void set_artificial_value( bool v );
      void set_phantom_value( bool v );
      void set_can_move_items_value( bool v );
      void set_fixed_value( bool v );
      void set_forbidden_position( const position_type& pos );

    protected:
      virtual bool do_satisfies_condition( const physical_item& item ) const;

    private:
      /** \brief Tell if the "artificial" flag must be checked. */
      bool m_check_artificial;

      /** \brief The value of the "artificial" flag, if checked. */
      bool m_artificial_value;

      /** \brief Tell if the "phantom" flag must be checked. */
      bool m_check_phantom;

      /** \brief The value of the "phantom" flag, if checked. */
      bool m_phantom_value;

      /** \brief Tell if the "can_move_items" flag must be checked. */
      bool m_check_can_move_items;

      /** \brief The value of the "can_move_items" flag, if checked. */
      bool m_can_move_items_value;

      /** \brief Tell if the "fixed" flag must be checked. */
      bool m_check_fixed;

      /** \brief The value of the "fixed" flag, if checked. */
      bool m_fixed_value;

      /** \brief Tell if we check that the item is not at the forbidden
          position. */
      bool m_check_forbidden_position;

      /** \brief A position where the item must not be. */
      position_type m_forbidden_position;

    }; // class item_picking_filter

  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_ITEM_PICKING_FILTER_HPP__
