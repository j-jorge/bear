/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A boolean expression that returns true if any input is pressed.
 * \author Julien Jorge
 */
#ifndef __BEAR_ANY_INPUT_PRESSED_HPP__
#define __BEAR_ANY_INPUT_PRESSED_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/engine/item_brick/item_with_input_listener.hpp"
#include "bear/engine/item_brick/with_boolean_expression_creation.hpp"

#include "bear/generic_items/class_export.hpp"
#include "bear/engine/export.hpp"

namespace bear
{
  class timer;

  /**
   * \brief A boolean expression that returns true if any input is pressed.
   *
   * This item has no custom fields.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT any_input_pressed:
    public engine::item_with_input_listener<engine::base_item>,
    public engine::with_boolean_expression_creation
  {
    DECLARE_BASE_ITEM(any_input_pressed);

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_input_listener<engine::base_item> super;

  public:
    any_input_pressed();

    virtual void progress( universe::time_type elapsed_time );

  private:
    virtual expr::boolean_expression do_get_expression() const;

    bool key_pressed( const input::key_info& key );
    bool button_pressed
    ( input::joystick::joy_code button, unsigned int joy_index );
    bool mouse_pressed
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );

    bool key_maintained( const input::key_info& key );
    bool button_maintained
    ( input::joystick::joy_code button, unsigned int joy_index );
    bool mouse_maintained
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );
    bool finger_action( const input::finger_event& event );

  private:
    /** \brief Tell if a key or a button is pressed. */
    bool m_pressed;

  }; // class any_input_pressed
} // namespace bear

#endif // __BEAR_ANY_INPUT_PRESSED_HPP__
