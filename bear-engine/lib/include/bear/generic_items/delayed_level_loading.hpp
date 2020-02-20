/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class starts a level after a given delay.
 * \author Julien Jorge
 */
#ifndef __BEAR_DELAYED_LEVEL_LOADING_HPP__
#define __BEAR_DELAYED_LEVEL_LOADING_HPP__

#include "bear/engine/item_brick/item_with_input_listener.hpp"
#include "bear/engine/base_item.hpp"
#include "bear/engine/export.hpp"
#include "bear/generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This item starts a level after a given delay or when the user
   *        presses a key.
   *
   * The custom fields of this class are :
   * - \a delay: real, number of seconds of delay (default = infinity),
   * - \a fade_duration: real, the duration of the fading effect before starting
   *   the level (default = 1),
   * - \a level: string, required. The name of the game variable containing the
   *   path of the level to load or the name of the level itself,
   * - \a transition_layer_name: string, the name of the transition layer
   *   receiving the fade effect (default = none).
   * - \a load_on_input: boolean, tell if the level must starts before the end
   *   of the delay if the user presses a key (default = false),
   * - any field supported by the parent class.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT delayed_level_loading:
    public engine::item_with_input_listener<engine::base_item>
  {
    DECLARE_BASE_ITEM(delayed_level_loading);

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_input_listener<engine::base_item> super;

  public:
    delayed_level_loading();
    delayed_level_loading
    ( const std::string& level_name, universe::time_type delay,
      bool load_on_input = false, universe::time_type fade_duration = 1,
      const std::string& transition_layer_name = "" );

    void destroy();

    bool set_string_field( const std::string& name, const std::string& value );
    bool set_real_field( const std::string& name, double value );
    bool set_bool_field( const std::string& name, bool value );

    bool is_valid() const;

    void progress( universe::time_type elapsed_time );
    void set_push_mode(bool push_mode);

  private:
    void start_fading();

    bool key_pressed( const input::key_info& key );
    bool button_pressed
    ( input::joystick::joy_code button, unsigned int joy_index );
    bool mouse_pressed( input::mouse::mouse_code button,
                        const claw::math::coordinate_2d<unsigned int>& pos );
    bool finger_action( const input::finger_event& event );

    std::string get_string_from_vars( const std::string& name ) const;

  private:
    /** \brief The path of the level to load. */
    std::string m_level_path;

    /** \brief The name of the transition layer receiving the fade effect. */
    std::string m_transition_layer_name;

    /** \brief The time elapsed since the creation of the item. */
    universe::time_type m_time;

    /** \brief The delay before we start the next level. */
    universe::time_type m_delay;

    /** \brief The duration of the fading. */
    universe::time_type m_fade_duration;

    /** \brief Flag set when the item starts to fade before loading. */
    bool m_loading;

    /** \brief Tell if the level must starts before the end of the delay if the
        user presses a key. */
    bool m_load_on_input;

    /** \brief Tell if the level must be pushed. */
    bool m_push_mode;

    /** \brief The identifier of the fading effect. */
    std::size_t m_effect_id;

  }; // class delayed_level_loading
} // namespace bear

#endif // __BEAR_DELAYED_LEVEL_LOADING_HPP__
