/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::animation_player class.
 * \author Julien Jorge
 */
#include "bf/animation_player.hpp"

#include <claw/assert.hpp>
#include <claw/real_number.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param anim The animation to play.
 */
bf::animation_player::animation_player( const animation& anim )
  : m_animation(anim), m_index(0), m_forward(true), m_play_count(0), m_time(0)
{

} // animation_player::animation_player()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the animation.
 * \param anim The animation to play.
 */
void bf::animation_player::set_animation( const animation& anim )
{
  m_animation = anim;
  reset();
} // animation_player::set_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the animation.
 */
const bf::animation& bf::animation_player::get_animation() const
{
  return m_animation;
} // animation_player::get_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Come back to first frame.
 */
void bf::animation_player::reset()
{
  m_index = 0;
  m_forward = true;
  m_play_count = 0;
  m_time = 0;
} // animation_player::reset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play the current frame and go to the next one.
 */
void bf::animation_player::next()
{
  if ( !is_finished() )
    next( m_animation.get_frame(m_index).get_duration() - m_time );
} // animation_player::next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play the animation during a given duration.
 * \param d The duration.
 */
void bf::animation_player::next( double d )
{
  CLAW_PRECOND( d >= 0 );

  if ( !is_finished() )
    {
      m_time += d;

      while ( claw::real_number<double>(m_time)
              >= m_animation.get_frame(m_index).get_duration() )
        {
          m_time -= m_animation.get_frame(m_index).get_duration();
          next_index();
        }

    }
} // animation_player::next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the duration until the next frame to be displayed.
 */
double bf::animation_player::get_duration_until_next() const
{
  if ( is_finished() )
    return std::numeric_limits<double>::infinity();
  else
    return m_animation.get_frame(m_index).get_duration() - m_time;
} // animation_player::get_duration_until_next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current playing time.
 */
double bf::animation_player::get_time() const
{
  return m_time;
} // animation_player::get_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current frame.
 */
bf::sprite bf::animation_player::get_sprite() const
{
  sprite result;

  if ( !m_animation.empty() )
    {
      result = m_animation.get_frame(m_index).get_sprite();

      result.flip( trinary_logic::to_bool(m_animation.get_flipped_status()) ^
                   trinary_logic::to_bool(result.get_flipped_status()) );
      result.mirror
        ( trinary_logic::to_bool(m_animation.get_mirrored_status()) ^
          trinary_logic::to_bool(result.get_mirrored_status()) );
      result.set_angle( m_animation.get_angle() + result.get_angle() );
      result.get_color().set_opacity
        ( m_animation.get_color().get_opacity() * 
          result.get_color().get_opacity() );
      result.get_color().set_intensity
        ( m_animation.get_color().get_red_intensity() * 
          result.get_color().get_red_intensity(),
          m_animation.get_color().get_green_intensity() * 
          result.get_color().get_green_intensity(),
          m_animation.get_color().get_blue_intensity() * 
          result.get_color().get_blue_intensity() );
      result.set_width( m_animation.width() );
      result.set_height( m_animation.height() );
    }

  return result;
} // animation_player::get_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move on a given frame (and reset everything else).
 * \param index The new index.
 */
void bf::animation_player::set_current_index( std::size_t index )
{
  reset();

  if ( m_animation.empty() )
    m_index = 0;
  else
    m_index = std::min(index, m_animation.frames_count() - 1);
} // animation_player::set_current_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the index of the current frame.
 */
std::size_t bf::animation_player::get_current_index() const
{
  return m_index;
} // animation_player::get_current_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we have reached the last frame and if this frame has been
 *        displayed during its whole duration.
 */
bool bf::animation_player::is_finished() const
{
  return sequence_is_finished()
    && ( m_time >= m_animation.get_frame(m_index).get_duration() );
} // animation_player::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we have reached the last frame.
 */
bool bf::animation_player::sequence_is_finished() const
{
  bool result = (m_play_count == m_animation.get_loops())
    && (m_animation.get_loops() != 0);

  if ( result )
    {
      if ( m_animation.get_loop_back() )
        {
          if ( m_animation.get_last_index() + 1 == m_animation.frames_count() )
            result = (m_index == m_animation.get_first_index());
          else
            result = (m_index + 1 == m_animation.frames_count());
        }
      else
        result = (m_index + 1 == m_animation.frames_count());
    }

  return result;
} // animation_player::sequence_is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the next frame.
 */
void bf::animation_player::next_index()
{
  if ( !sequence_is_finished() )
    {
      if (m_forward)
        next_forward();
      else
        next_backward();
    }
} // animation_player::next_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the next frame, forward playing.
 */
void bf::animation_player::next_forward()
{
  CLAW_PRECOND( !is_finished() );

  // we're on the last frame of the loop interval
  if ( m_index == m_animation.get_last_index() )
    {
      if ( m_animation.get_loop_back() ) // should we play reverse ?
        {
          m_forward = false;

          if (m_index > 0)
            --m_index;
        }
      else // come back directly to the first frame of the interval
        {
          ++m_play_count;

          // the last time we play the sequence, we'll go after the interval
          if ( m_play_count != m_animation.get_loops() )
            m_index = m_animation.get_first_index();
          else if ( m_animation.get_last_index() + 1
                    != m_animation.frames_count() )
            m_index = m_animation.get_last_index() + 1;
        }
    }
  else
    ++m_index;
} // animation_player::next_forward()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the next frame, backward playing.
 */
void bf::animation_player::next_backward()
{
  if ( m_index == m_animation.get_first_index() ) // we're at the begining
    {
      ++m_play_count;

      if ( m_animation.frames_count() > 1 )
        {
          m_forward = true;

          if ( m_play_count != m_animation.get_loops() )
            {
              if (m_animation.get_first_index() != m_animation.get_last_index())
                ++m_index;
            }
          else if ( m_animation.get_last_index() + 1
                    != m_animation.frames_count() )
            m_index = m_animation.get_last_index() + 1;
        }
    }
  else
    --m_index;
} // animation_player::next_backward()
