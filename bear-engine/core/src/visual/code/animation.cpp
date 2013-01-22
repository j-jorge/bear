/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::animation class.
 * \author Julien Jorge
 */
#include "visual/animation.hpp"
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::visual::animation::animation()
  : m_time(0), m_time_factor(1)
{

} // animation::animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor. Create an animation with a single frame from a sprite.
 * \param spr The single frame of the animation.
 */
bear::visual::animation::animation( const sprite& spr )
  : sprite_sequence(spr), m_duration(0), m_time(0), m_time_factor(1)
{

} // animation::animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param images The frames of the animation.
 * \param d The duration of the frames.
 * \pre images.size() > 0
 * \pre images.size() == d.size()
 * \post loops == 0, loop_back == false
 */
bear::visual::animation::animation
( const std::vector<sprite>& images, const std::vector<double>& d )
  : sprite_sequence(images), m_duration(d), m_time(0), m_time_factor(1)
{
  CLAW_PRECOND( images.size() == d.size() );
} // animation::animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the coefficient applied to the durations of the frames.
 * \param f The time factor.
 */
void bear::visual::animation::set_time_factor( double f )
{
  if ( f < 0 )
    f = 0;

  m_time_factor = f;
} // animation::set_time_factor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the coefficient applied to the durations of the frames.
 */
double bear::visual::animation::get_time_factor() const
{
  return m_time_factor;
} // animation::get_time_factor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Come back to first frame.
 */
void bear::visual::animation::reset()
{
  sprite_sequence::reset();
  m_time = 0;
} // animation::reset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go forward in the animation.
 * \param t Go this dutation forward.
 * \pre t >= 0
 */
void bear::visual::animation::next( double t )
{
  CLAW_PRECOND( t >= 0 );

  if ( !is_finished() )
    {
      m_time += m_time_factor * t;

      while ( ( m_time >= get_scaled_duration(get_current_index()) )
              && !sprite_sequence::is_finished() )
        {
          m_time -= get_scaled_duration(get_current_index());
          sprite_sequence::next();
        }
    }
} // animation::next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we have reached the last frame and if it is completely played.
 */
bool bear::visual::animation::is_finished() const
{
  if ( m_duration.empty() )
    return true;
  else
    return sprite_sequence::is_finished()
      && ( m_time >= get_scaled_duration(get_current_index()) );
} // animation::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the duration of a frame, according to the current time_factor.
 * \param i The index of the frame.
 */
double bear::visual::animation::get_scaled_duration( std::size_t i ) const
{
  CLAW_PRECOND( i < m_duration.size() );

  return m_duration[i];
} // animation::get_scaled_duration()
