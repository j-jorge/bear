/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Inherit from the engine::gui_layer class to create a layer in the
 *        graphical user interface.
 * \author Julien Jorge
 */
#ifndef __ENGINE_GUI_LAYER_HPP__
#define __ENGINE_GUI_LAYER_HPP__

#include "engine/level_object.hpp"
#include "gui/visual_component.hpp"
#include "input/input_listener.hpp"
#include "visual/screen.hpp"
#include "universe/types.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief Inherit from this class to create a layer in the graphical user
     *        interface.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT gui_layer:
      public input::input_listener,
      virtual public level_object
    {
    public:
      typedef std::list<visual::scene_element> scene_element_list;

    public:
      gui_layer();

      virtual void pre_cache();
      virtual void build();
      virtual void progress( universe::time_type elapsed_time );
      virtual void render( scene_element_list& e ) const;

      const claw::math::coordinate_2d<unsigned int>& get_size() const;
      claw::math::coordinate_2d<unsigned int>
        adjust_screen_position
        ( const claw::math::coordinate_2d<unsigned int>& pos ) const;

      void set_root_component( gui::visual_component* c );

      bool key_pressed( const input::key_info& key );
      bool char_pressed( const input::key_info& key );
      bool button_pressed
        ( input::joystick::joy_code button, unsigned int joy_index );
      bool mouse_pressed
        ( input::mouse::mouse_code key,
          const claw::math::coordinate_2d<unsigned int>& pos );
      bool mouse_released
        ( input::mouse::mouse_code key,
          const claw::math::coordinate_2d<unsigned int>& pos );
      bool mouse_maintained
        ( input::mouse::mouse_code key,
          const claw::math::coordinate_2d<unsigned int>& pos );
      bool mouse_move( const claw::math::coordinate_2d<unsigned int>& pos );
      bool finger_action( const bear::input::finger_event& event );

    private:
      /** \brief The size of the layer. */
      const claw::math::coordinate_2d<unsigned int> m_size;

      /** \brief The component that receives the inputs by default. */
      gui::visual_component* m_root;

    }; // class gui_layer
  } // namespace engine
} // namespace bear

#endif // __ENGINE_GUI_LAYER_HPP__
