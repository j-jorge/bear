/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::fade_effect class.
 * \author Julien Jorge
 */
#include "engine/transition_effect/fade_effect.hpp"

#include "engine/level.hpp"
#include "visual/scene_rectangle.hpp"

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::fade_effect::fade_effect()
  : m_fade_in_duration(1), m_full_duration(0), m_fade_out_duration(0),
    m_elapsed_time(0), m_opacity(1)
{
  m_color.components.red = 0;
  m_color.components.green = 0;
  m_color.components.blue = 0;
  m_color.components.alpha =
    std::numeric_limits<visual::color_type::component_type>::max();
} // fade_effect::fade_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the effect is finished.
 */
bool bear::engine::fade_effect::is_finished() const
{
  return m_elapsed_time
    >= m_fade_in_duration + m_full_duration + m_fade_out_duration;
} // fade_effect::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the components of the effect.
 * \param elapsed_time Elapsed time since the last call.
 */
bear::universe::time_type
bear::engine::fade_effect::progress( universe::time_type elapsed_time )
{
  universe::time_type result(0);

  if ( !get_layer().get_level().is_paused() )
    {
      const universe::time_type total_time
        ( m_fade_in_duration + m_full_duration + m_fade_out_duration );

      if ( m_elapsed_time + elapsed_time >= total_time )
        {
          if ( m_elapsed_time < total_time )
            result = m_elapsed_time + elapsed_time - total_time;
          else
            result = elapsed_time;
        }

      m_elapsed_time += elapsed_time;

      if ( result != elapsed_time )
        adjust_opacity();
    }

  return result;
} // fade_effect::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the components of the effect.
 * \param e (out) The scene elements.
 */
void bear::engine::fade_effect::render( scene_element_list& e ) const
{
  e.push_back
    ( visual::scene_rectangle
      ( 0, 0, m_color,
        visual::rectangle_type
        (0, 0, get_layer().get_size().x, get_layer().get_size().y) ) );
} // fade_effect::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the durations of the effect.
 * \param in How long the fade in is.
 * \param full How long the full intensity is kept.
 * \param out How long the fade out is.
 */
void bear::engine::fade_effect::set_duration
( universe::time_type in, universe::time_type full, universe::time_type out )
{
  m_fade_in_duration = in;
  m_full_duration = full;
  m_fade_out_duration = out;
} // fade_effect::set_duration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the color of the effect.
 * \param r Intensity of the red component in [0, 1].
 * \param g Intensity of the green component in [0, 1].
 * \param b Intensity of the blue component in [0, 1].
 */
void bear::engine::fade_effect::set_color( double r, double g, double b )
{
  const visual::color_type::component_type comp_max
    ( std::numeric_limits<visual::color_type::component_type>::max() );

  m_color.components.red = r * comp_max;
  m_color.components.green = g * comp_max;
  m_color.components.blue = b * comp_max;
} // fade_effect::set_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the maximum intensity of the effect.
 * \param o The maximum opacity [0, 1].
 */
void bear::engine::fade_effect::set_opacity( double o )
{
  if ( o < 0 )
    o = 0;
  else if ( o > 1 )
    o = 1;

  m_opacity = o;
} // fade_effect::set_opacity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the opacity of the sprite.
 */
void bear::engine::fade_effect::adjust_opacity()
{
  double opacity;

  if ( m_elapsed_time > m_fade_in_duration )
    {
      if ( m_elapsed_time > m_fade_in_duration + m_full_duration )
        {
          const universe::time_type t
            (m_elapsed_time - m_fade_in_duration - m_full_duration);

          if ( t >= m_fade_out_duration )
            opacity = 0;
          else
            opacity =
              1 - (t) / m_fade_out_duration;
        }
      else
        opacity = 1;
    }
  else
    opacity = m_elapsed_time / m_fade_in_duration;

  m_color.components.alpha =
    opacity * m_opacity
    * std::numeric_limits<visual::color_type::component_type>::max();
} // fade_effect::adjust_opacity()

