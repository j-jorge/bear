/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A slider.
 * \author Julien Jorge
 */
#ifndef __GUI_SLIDER_HPP__
#define __GUI_SLIDER_HPP__

#include "gui/callback_group.hpp"
#include "gui/visual_component.hpp"

#include "visual/sprite.hpp"
#include "input/key_info.hpp"

namespace bear
{
  namespace gui
  {
    /**
     * \brief A slider.
     * \author Julien Jorge
     */
    template<typename T>
    class slider:
      public visual_component
    {
    public:
      slider( const visual::sprite& bar, const visual::sprite& slider,
              T min, T max, T value );
      slider( const visual::sprite& bar, const visual::sprite& slider,
              T min, T max, T value, const callback& value_changed );

      void set_delta( T v );
      void set_value( T v );
      T get_value() const;

    private:
      void display( std::list<visual::scene_element>& e ) const;

      void set_slider_at( unsigned int x );

      void on_resized();
      bool on_key_press( const input::key_info& key );
      bool on_button_press
        ( input::joystick::joy_code button, unsigned int joy_index );
      bool on_mouse_press
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );
      bool on_mouse_released
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );
      bool on_mouse_maintained
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );

    private:
      /** \brief The bar on which the slider slides. */
      visual::sprite m_bar;

      /** \brief The slider. */
      visual::sprite m_slider;

      /** \brief The minimum value. */
      const T m_min;

      /** \brief The maximum value. */
      const T m_max;

      /** \brief The current value. */
      T m_value;

      /** \brief The delta applied to the value. */
      T m_delta;

      /** \brief Callback executed when the value changed. */
      callback_group m_value_changed_callback;

    }; // class slider
  } // namespace gui
} // namespace bear

#include "gui/impl/slider.tpp"

#endif // __GUI_SLIDER_HPP__
