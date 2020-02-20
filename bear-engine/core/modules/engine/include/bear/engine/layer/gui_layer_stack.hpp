/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The stack of the layers for the graphical user interface.
 * \author Julien Jorge
 */
#ifndef __BEAR_GUI_LAYER_STACK_HPP__
#define __BEAR_GUI_LAYER_STACK_HPP__

#include "bear/input/input_listener.hpp"
#include "bear/input/input_status.hpp"
#include "bear/visual/scene_element.hpp"
#include "bear/universe/types.hpp"

#include "bear/engine/class_export.hpp"

#include <vector>

namespace bear
{
  namespace engine
  {
    class gui_layer;

    /**
     * \brief The stack of the layers for the graphical user interface.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT gui_layer_stack:
      public input::input_listener
    {
    public:
      /** \brief The type of the container in which we store the visuals when
          rendering. */
      typedef std::list<visual::scene_element> scene_element_list;

    public:
      ~gui_layer_stack();

      void push_layer( gui_layer* the_layer );

      void progress( universe::time_type elapsed_time );
      void render( scene_element_list& e ) const;

      void clear();

    private:
      bool key_pressed( const input::key_info& key );
      bool key_maintained( const input::key_info& key );
      bool key_released( const input::key_info& key );
      bool char_pressed( const input::key_info& key );
      bool button_pressed
      ( input::joystick::joy_code button, unsigned int joy_index );
      bool button_maintained
      ( input::joystick::joy_code button, unsigned int joy_index );
      bool button_released
      ( input::joystick::joy_code button, unsigned int joy_index );
      bool mouse_pressed( input::mouse::mouse_code key,
                          const claw::math::coordinate_2d<unsigned int>& pos );
      bool mouse_released( input::mouse::mouse_code button,
                           const claw::math::coordinate_2d<unsigned int>& pos );
      bool mouse_maintained
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );
      bool mouse_move( const claw::math::coordinate_2d<unsigned int>& pos );
      bool finger_action( const input::finger_event& event );

    private:
      /** \brief The sub layers */
      std::vector<gui_layer*> m_sub_layers;

      /** \brief The status of the input controllers. */
      input::input_status m_input_status;

    }; // class gui_layer_stack
  } // namespace engine
} // namespace bear

#endif // __BEAR_GUI_LAYER_STACK_HPP__
