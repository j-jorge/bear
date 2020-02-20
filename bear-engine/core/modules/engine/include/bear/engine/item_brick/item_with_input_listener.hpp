/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Inherit from this class to allow your item to read the player's
 *        inputs.
 * \author Julien Jorge
 */
#ifndef __ENGINE_ITEM_WITH_INPUT_LISTENER_HPP__
#define __ENGINE_ITEM_WITH_INPUT_LISTENER_HPP__

#include "bear/input/input_listener.hpp"
#include "bear/input/input_status.hpp"
#include "bear/universe/types.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief Inherit from this class to allow your item to read the controller
     *        inputs.
     *
     * \b template \b parameters :
     * - \a Base : the base class for this item. Must inherit from
     *    engine::base_item.
     *
     * There is no custom field of this class.
     *
     * The processing of the inputs is not done automatically. You must call the
     * method progress_input_reader() when you are ready to process them.
     *
     * \author Julien Jorge
     */
    template<class Base>
    class item_with_input_listener:
      public Base,
      public input::input_listener
    {
    private:
      typedef Base super;

    protected:
      void progress_input_reader( universe::time_type elapsed_time );

      virtual bool key_maintained
      ( universe::time_type elapsed_time, const input::key_info& key );

      virtual bool button_maintained
      ( universe::time_type elapsed_time, input::joystick::joy_code button,
        unsigned int joy_index );

      virtual bool mouse_maintained
      ( universe::time_type elapsed_time, input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );

      virtual bool mouse_pressed_local
      ( input::mouse::mouse_code button, const universe::position_type& pos );

      virtual bool mouse_maintained_local
      ( universe::time_type elapsed_time, input::mouse::mouse_code button,
        const universe::position_type& pos );

      virtual bool mouse_released_local
      ( input::mouse::mouse_code button, const universe::position_type& pos );

      virtual bool mouse_move_local( const universe::position_type& pos );

      virtual bool finger_action_local( const input::finger_event& event );

    private:
      bool key_maintained( const input::key_info& key );

      bool button_maintained
      ( input::joystick::joy_code button, unsigned int joy_index );

      bool mouse_pressed
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );

      bool mouse_maintained
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );

      bool mouse_released
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );

      bool mouse_move( const claw::math::coordinate_2d<unsigned int>& pos );

      bool finger_action( const input::finger_event& event );

    private:
      /** \brief The status of the controllers. */
      input::input_status m_input_status;

      /** \brief Last value of \a elapsed_time passed to
          progress_input_reader(). */
      universe::time_type m_elapsed_time;

    }; // class item_with_input_listener
  } // namespace engine
} // namespace bear

#include "bear/engine/item_brick/impl/item_with_input_listener.tpp"

#endif // __ENGINE_ITEM_WITH_INPUT_LISTENER_HPP__
