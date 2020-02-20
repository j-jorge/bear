/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the gui::slider class.
 * \author Julien Jorge
 */
#include <claw/assert.hpp>

#include "bear/visual/scene_sprite.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param bar The sprite of the bar on which the slider slides..
 * \param slider The sprite of the slide.
 * \param min The minimum value.
 * \param max The maximum value.
 * \param value The initial value.
 * \pre min <= max
 */
template<typename T>
bear::gui::slider<T>::slider
( const visual::sprite& bar, const visual::sprite& slider, T min, T max,
  T value )
  : visual_component(), m_bar(bar), m_slider(slider), m_min(min),
    m_max(max), m_delta(0)
{
  CLAW_PRECOND(min <= max);

  set_value(value);
} // slider::slider()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param bar The sprite of the bar on which the slider slides..
 * \param slider The sprite of the slide.
 * \param min The minimum value.
 * \param max The maximum value.
 * \param value The initial value.
 * \param value_changed The callback called when the valued has changed.
 * \pre min <= max
 */
template<typename T>
bear::gui::slider<T>::slider
( const visual::sprite& bar, const visual::sprite& slider, T min, T max,
  T value, const callback& value_changed )
  : visual_component(), m_bar(bar), m_slider(slider), m_min(min),
    m_max(max), m_delta(0)
{
  CLAW_PRECOND(min <= max);

  set_value(value);

  m_value_changed_callback.add(value_changed);
} // slider::slider()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the delta applied when moving the slider.
 * \param v The new delta.
 */
template<typename T>
void bear::gui::slider<T>::set_delta( T v )
{
  m_delta = v;
} // slider::set_delta()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of the slider.
 * \param v The new value.
 */
template<typename T>
void bear::gui::slider<T>::set_value( T v )
{
  const T old(m_value);

  if ( v < m_min )
    m_value = m_min;
  else if ( v > m_max )
    m_value = m_max;
  else
    m_value = v;

  if ( old != m_value )
    m_value_changed_callback.execute();
} // slider::set_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the slider.
 * \param text The new text.
 */
template<typename T>
T bear::gui::slider<T>::get_value() const
{
  return m_value;
} // slider::get_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the scene elements of the component.
 * \param e (out) The scene elements.
 */
template<typename T>
void bear::gui::slider<T>::display
( std::list<visual::scene_element>& e ) const
{
  position_type p(bottom_left());

  if ( height() > m_bar.height() )
    p.y += (height() - m_bar.height()) / 2;

  e.push_back( visual::scene_sprite(p.x, p.y, m_bar) );

  p = bottom_left();

  if ( height() > m_slider.height() )
    p.y += (height() - m_slider.height()) / 2;

  if (m_min != m_max)
    p.x += (m_slider.width() / 2)
      + (int)( ((m_value - m_min)
                * (width() - 2 * m_slider.width())) / (m_max - m_min) );

  e.push_back( visual::scene_sprite(p.x, p.y, m_slider) );
} // slider::display()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the slider's x-position at a given coordinate, adjust the value
 *        accordingly.
 * \param x The new x position of the center of the slider.
 */
template<typename T>
void bear::gui::slider<T>::set_slider_at( unsigned int x )
{
  if ( x <= (m_slider.width() / 2) )
    set_value(m_min);
  else if ( x >= (right() - m_slider.width() / 2) )
    set_value(m_max);
  else
    set_value( m_min + (T)( (x - m_slider.width()) * (m_max - m_min))
               / (T)(width() - 2 * m_slider.width()) );
} // slider::set_slider_at()

/*----------------------------------------------------------------------------*/
/**
 * \brief The control was resized.
 */
template<typename T>
void bear::gui::slider<T>::on_resized()
{
  m_bar.set_width( width() );
} // slider::on_resized()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that a key has been pressed.
 * \param key The code of the key.
 */
template<typename T>
bool bear::gui::slider<T>::on_key_press( const bear::input::key_info& key )
{
  bool result = true;

  if ( key.is_left() )
    set_value( m_value - m_delta );
  else if ( key.is_right() )
    set_value( m_value + m_delta );
  else
    result = false;

  return result;
} // slider::on_key_press()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that a joystick button has been pressed.
 * \param button The code of the button.
 * \param joy_index The index of the joytick.
 */
template<typename T>
bool bear::gui::slider<T>::on_button_press
( bear::input::joystick::joy_code button, unsigned int joy_index )
{
  bool result = true;

  switch( button )
    {
    case bear::input::joystick::jc_axis_left:
      set_value( m_value - m_delta );
      break;
    case bear::input::joystick::jc_axis_right:
      set_value( m_value + m_delta );
      break;
    default:
      result = false;
    }

  return result;
} // slider::on_button_press()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the component that a mouse button had been pressed.
 * \param key The value of the pressed button.
 * \param pos The current position of the cursor.
 */
template<typename T>
bool bear::gui::slider<T>::on_mouse_press
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  set_slider_at(pos.x);

  return true;
} // slider::on_mouse_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the component that a mouse button had been released.
 * \param key The value of the released button.
 * \param pos The current position of the cursor.
 */
template<typename T>
bool bear::gui::slider<T>::on_mouse_released
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  set_slider_at(pos.x);

  return true;
} // slider::on_mouse_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the component that a mouse button had been released.
 * \param key The value of the released button.
 * \param pos The current position of the cursor.
 */
template<typename T>
bool bear::gui::slider<T>::on_mouse_maintained
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  set_slider_at(pos.x);

  return true;
} // slider::on_mouse_maintained()

