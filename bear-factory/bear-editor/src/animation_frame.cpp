/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::animation_frame class.
 * \author Julien Jorge
 */
#include "bf/animation_frame.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bf::animation_frame::animation_frame()
  : m_sprite(), m_duration(1)
{

} // animation_frame::animation_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the sprite.
 * \param spr The new sprite.
 */
void bf::animation_frame::set_sprite(const sprite& spr)
{
  m_sprite = spr;
} // animation_frame::set_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the duration.
 * \param duration The new duration.
 */
void bf::animation_frame::set_duration(double duration)
{
  m_duration = duration;
} // animation_frame::set_duration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprite.
 */
const bf::sprite& bf::animation_frame::get_sprite() const
{
  return m_sprite;
} // animation_frame::get_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the duration.
 */
double bf::animation_frame::get_duration() const
{
  return m_duration;
} // animation_frame::get_duration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two frames are the same.
 * \param that The other frame.
 */
bool bf::animation_frame::operator==( const animation_frame& that ) const
{
  return (m_sprite == that.m_sprite) && (m_duration == that.m_duration);
} // animation_frame::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare with an other animation frame in lexicographic order of their
 *        attributes.
 * \param that The other animation frame.
 */
bool bf::animation_frame::operator<(const animation_frame& that) const
{
  if ( m_sprite != that.m_sprite )
    return m_sprite < that.m_sprite;

  return m_duration < that.m_duration;
} // animation_frame::operator<()
