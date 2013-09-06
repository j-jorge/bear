/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An object that activates a toggle according to the mouse.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_MOUSE_DETECTOR_HPP__
#define __BEAR_MOUSE_DETECTOR_HPP__

#include "engine/export.hpp"
#include "engine/item_brick/item_with_input_listener.hpp"
#include "engine/item_brick/with_toggle.hpp"
#include "generic_items/class_export.hpp"
#include "universe/derived_item_handle.hpp"

namespace bear
{
  /**
   * \brief An object that activates a toggle according to the mouse.
   * \author Sebastien Angibaud
   *
   */
  class GENERIC_ITEMS_EXPORT mouse_detector :
    public engine::item_with_input_listener
  < engine::base_item >
  {
    DECLARE_BASE_ITEM(mouse_detector);

    /** \brief The type of the parent class. */
    typedef engine::item_with_input_listener
      < engine::base_item > super;

    /** \brief The type of an handle on a toggle. */
    typedef
    bear::universe::derived_item_handle<bear::engine::with_toggle> handle_type;

  public:
    mouse_detector();

    void progress( universe::time_type elapsed_time );
    bool set_bool_field( const std::string& name, bool value);
    bool set_item_field
      ( const std::string& name, bear::engine::base_item* value);

  protected:
    bool mouse_released
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );
    bool check_mouse_position
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );

    bool finger_action( const input::finger_event& event );

  private:
    void get_dependent_items
      ( universe::physical_item::item_list& d ) const;

  private:
    /** \brief The toggle to activate. */
    handle_type m_toggle;

    /** \brief Indicates if the toggle must be activated on right press. */
    bool m_right_button;

    /** \brief Indicates if the toggle must be activated on left press. */
    bool m_left_button;

    /** \brief Indicates if the toggle must be activated on middle press. */
    bool m_middle_button;

    /** \brief Indicates if the toggle must be activated on wheel up. */
    bool m_wheel_up;

    /** \brief Indicates if the toggle must be activated on wheel down. */
    bool m_wheel_down;

    /** \brief Indicates if the toggle must be activated on finger events. */
    bool m_finger;

  }; // class mouse_detector
} // namespace bear

#endif // __BEAR_MOUSE_DETECTOR_HPP__
