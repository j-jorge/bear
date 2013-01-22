/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::sprite_sequence class.
 * \author Julien Jorge
 */
#include "visual/sprite_sequence.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::visual::sprite_sequence::sprite_sequence()
  : m_index(0), m_loops(1), m_loop_back(false), m_forward(true),
    m_play_count(1), m_first_index(0), m_last_index(0)
{

} // sprite_sequence::sprite_sequence()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param images The sprites in the sequence.
 * \pre images.size() > 0
 * \post loops == 0, loop_back == false
 */
bear::visual::sprite_sequence::sprite_sequence
( const std::vector<sprite>& images )
  : m_sprites(images), m_index(0), m_loops(0), m_loop_back(false),
    m_forward(true), m_play_count(0), m_first_index(0),
    m_last_index(m_sprites.size()-1)
{
  CLAW_PRECOND(images.size() > 0);

  set_size(get_max_size());
} // sprite_sequence::sprite_sequence()

/*----------------------------------------------------------------------------*/
/**
 * \brief Come back to first frame.
 */
void bear::visual::sprite_sequence::reset()
{
  m_index = 0;
  m_forward = true;
  m_play_count = 0;
} // sprite_sequence::reset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the next frame.
 */
void bear::visual::sprite_sequence::next()
{
  if ( !is_finished() )
    {
      if (m_forward)
        next_forward();
      else
        next_backward();
    }
} // sprite_sequence::next()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the sequence is empty.
 */
bool bear::visual::sprite_sequence::empty() const
{
  return m_sprites.empty();
} // sprite_sequence::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current frame.
 */
bear::visual::sprite bear::visual::sprite_sequence::get_sprite() const
{
  sprite result;

  if ( m_index < m_sprites.size() )
    {
      result = m_sprites[m_index];
      result.combine(*this);

      result.set_size( result.width() * width() / get_max_size().x,
                       result.height() * height() / get_max_size().y );
    }

  return result;
} // sprite_sequence::get_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the biggest size of the frames.
 */
bear::visual::size_box_type bear::visual::sprite_sequence::get_max_size() const
{
  claw::math::coordinate_2d<unsigned int> result(0, 0);

  for (unsigned int i=0; i!=m_sprites.size(); ++i)
    {
      if ( m_sprites[i].get_size().x > result.x )
        result.x = m_sprites[i].get_size().x;

      if ( m_sprites[i].get_size().y > result.y )
        result.y = m_sprites[i].get_size().y;
    }

  return result;
} // sprite_sequence::get_max_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the highest index.
 */
unsigned int bear::visual::sprite_sequence::get_max_index() const
{
  return m_sprites.size() - 1;
} // sprite_sequence::get_max_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the current index.
 * \param index The new index.
 */
void bear::visual::sprite_sequence::set_current_index( unsigned int index )
{
  m_index = std::min(index, get_max_index());
} // sprite_sequence::set_current_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set loop back mode.
 */
void bear::visual::sprite_sequence::set_loop_back( bool loop_back )
{
  m_loop_back = loop_back;
} // sprite_sequence::set_loop_back()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the number of times the sprite_sequence should be read.
 */
void bear::visual::sprite_sequence::set_loops( unsigned int loops )
{
  m_play_count = 0;
  m_loops = loops;
} // sprite_sequence::set_loops()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the index of the first frame of the loops.
 * \param index The index.
 */
void bear::visual::sprite_sequence::set_first_index( unsigned int index )
{
  m_first_index = index;

  if (m_last_index < m_first_index)
    m_last_index = m_first_index;
} // sprite_sequence::set_first_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the index of the last frame of the loops.
 * \param index The index.
 */
void bear::visual::sprite_sequence::set_last_index( unsigned int index )
{
  if ( index >= m_sprites.size() )
    m_last_index = m_sprites.size() - 1;
  else
    {
      m_last_index = index;

      if (m_last_index < m_first_index)
        m_first_index = m_last_index;
    }
} // sprite_sequence::set_last_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we have reached the last frame.
 */
bool bear::visual::sprite_sequence::is_finished() const
{
  bool result = (m_play_count == m_loops) && m_loops;

  if ( result )
    {
      if ( m_loop_back )
        {
          if ( m_last_index + 1 == m_sprites.size() )
            result = (m_index == m_first_index);
          else
            result = (m_index + 1 == m_sprites.size());
        }
      else
        result = (m_index + 1 == m_sprites.size());
    }

  return result || m_sprites.empty();
} // sprite_sequence::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the sequence will end.
 */
bool bear::visual::sprite_sequence::is_finite() const
{
  return m_loops != 0;
} // sprite_sequence::is_finite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the animation is made with valid sprites.
 */
bool bear::visual::sprite_sequence::is_valid() const
{
  bool result = !m_sprites.empty();

  for (unsigned int i=0; result && (i!=m_sprites.size()); ++i)
    result = m_sprites[i].is_valid();

  return result;
} // sprite_sequence::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor. Create a sequence with a single sprite.
 * \param spr The single sprite.
 * \post loops == 0, loop_back == false
 */
bear::visual::sprite_sequence::sprite_sequence( const sprite& spr )
  : m_index(0), m_loops(1), m_loop_back(false), m_forward(true),
    m_play_count(1), m_first_index(0), m_last_index(0)
{
  m_sprites.push_back(spr);
  set_size(get_max_size());
} // sprite_sequence::sprite_sequence()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the index of the current frame.
 */
unsigned int bear::visual::sprite_sequence::get_current_index() const
{
  return m_index;
} // sprite_sequence::get_current_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the next frame, forward playing.
 */
void bear::visual::sprite_sequence::next_forward()
{
  CLAW_PRECOND( !is_finished() );

  if ( m_index == m_last_index ) // we're on the last frame of the loop interval
    {
      if ( m_loop_back ) // should we play reverse ?
        {
          m_forward = false;

          if (m_index > 0)
            --m_index;
        }
      else // come back directly to the first frame of the interval
        {
          ++m_play_count;

          // the last time we play the sequence, we'll go after the interval
          if ( m_play_count != m_loops )
            m_index = m_first_index;
          else if ( m_last_index + 1 != m_sprites.size() )
            m_index = m_last_index + 1;
        }
    }
  else
    ++m_index;
} // sprite_sequence::next_forward()

/*----------------------------------------------------------------------------*/
/**
 * \brief Go to the next frame, backward playing.
 */
void bear::visual::sprite_sequence::next_backward()
{
  if ( m_index == m_first_index ) // we're at the begining
    {
      ++m_play_count;

      if ( m_sprites.size() > 1 )
        {
          m_forward = true;

          if ( m_play_count != m_loops )
            {
              if (m_first_index != m_last_index)
                ++m_index;
            }
          else if ( m_last_index + 1 != m_sprites.size() )
            m_index = m_last_index + 1;
        }
    }
  else
    --m_index;
} // sprite_sequence::next_backward()
