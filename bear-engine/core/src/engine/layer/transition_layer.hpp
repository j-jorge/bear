/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This layer displays transition effects.
 * \author Julien Jorge
 */
#ifndef __BEAR_ENGINE_TRANSITION_LAYER_HPP__
#define __BEAR_ENGINE_TRANSITION_LAYER_HPP__

#include "communication/messageable.hpp"
#include "engine/layer/gui_layer.hpp"

#include "engine/class_export.hpp"

#include <map>

namespace bear
{
  namespace engine
  {
    class transition_effect;

    /**
     * \brief This layer displays transition effects.
     * \author Julien Jorge
     *
     * The progress() method does nothing if the level is paused. Effect coders
     * can prevent the user from setting the pause by returning true in the
     * methods transition_effect::key_pressed() and
     * transition_effect::button_pressed().
     */
    class ENGINE_EXPORT transition_layer:
      public gui_layer,
      public communication::messageable
    {
    public:
      /** \brief The type of a list of scene elements retrieved from the
          layer. */
      typedef gui_layer::scene_element_list scene_element_list;

    private:
      /** \brief The data associated with the effects. */
      struct effect_entry
      {
        effect_entry( transition_effect* e, std::size_t id );

        /** \brief The displayed effect. */
        transition_effect* effect;

        /** \brief The identifier of the effect. */
        std::size_t id;

      }; // struct effect_entry

      /** \brief The type of the map in which are stored the active effects. */
      typedef std::multimap<int, effect_entry> effect_map_type;

    public:
      transition_layer( const std::string& name );
      ~transition_layer();

      void build();
      void progress( universe::time_type elapsed_time );
      void render( scene_element_list& e ) const;

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
      bool mouse_pressed
      ( input::mouse::mouse_code key,
        const claw::math::coordinate_2d<unsigned int>& pos );
      bool mouse_released
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );
      bool mouse_maintained
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );
      bool mouse_move( const claw::math::coordinate_2d<unsigned int>& pos );
      bool finger_action( const bear::input::finger_event& event );

      void erase_effect( std::size_t id );
      std::size_t push_effect( transition_effect* e, int p = 0 );
      std::size_t set_effect( transition_effect* e, int p = 0 );

    private:
      void clear();

      template<typename F>
      bool diffuse_call( F f ) const;

    public:
      /** \brief An invalid value for the identifiers of the effects. */
      static const std::size_t not_an_id;

    private:
      /** \brief The current effect displayed. */
      effect_map_type m_effect;

      /** \brief The next available id for the effects. */
      static std::size_t s_next_id;

    }; // class transition_layer
  } // namespace engine
} // namespace bear

#endif // __BEAR_ENGINE_TRANSITION_LAYER_HPP__
