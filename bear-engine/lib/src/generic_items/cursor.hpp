/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A simple decorative cursor.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_CURSOR_HPP__
#define __BEAR_CURSOR_HPP__

#include "engine/item_brick/item_with_input_listener.hpp"
#include "generic_items/decorative_item.hpp"
#include "generic_items/class_export.hpp"

#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief A simple decorative cursor.
   * \author Sebastien Angibaud
   *
   */
  class GENERIC_ITEMS_EXPORT cursor:
    public engine::item_with_input_listener< decorative_item >
  {
    DECLARE_BASE_ITEM(cursor);

    /** \brief The type of the parent class. */
    typedef engine::item_with_input_listener< decorative_item > super;

  public:
    cursor();

    void progress( universe::time_type elapsed_time );
    bool set_real_field( const std::string& name, double value );
    void get_visual( std::list<bear::engine::scene_visual>& visuals ) const;

  private:
    bool mouse_move( const claw::math::coordinate_2d<unsigned int>& pos );

  private:
    /* \brief The position of the mouse on the screen. We must store its
       coordinates in order to update the position of the cursor in the level
       at each iteration. */
    claw::math::coordinate_2d<unsigned int> m_screen_position;

    /* \brief The duration before hide the cursor. */
    bear::universe::time_type m_visibility_duration;

    /* \brief The inactive duration. */
    bear::universe::time_type m_inactive_duration;

  }; // class cursor
} // namespace bear

#endif // __BEAR_CURSOR_HPP__
