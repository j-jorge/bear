/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::audio::sound_effect class.
 * \author Julien Jorge
 */
#include "bear/audio/sound_effect.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 *
 * Defaults values are :
 * - volume = 1
 * - loops = 1
 * - no position
 */
bear::audio::sound_effect::sound_effect()
  : m_volume(1), m_loops(1), m_position(NULL)
{

} // sound_effect::sound_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param volume The volume of the sound.
 */
bear::audio::sound_effect::sound_effect( double volume )
  : m_volume(volume), m_loops(1), m_position(NULL)
{
  if ( m_volume < 0 )
    m_volume = 0;
  else if ( m_volume > 1 )
    m_volume = 1;
} // sound_effect::sound_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param loops Set the number of loops. 0 means infinite.
 * \param volume The volume of the sound. Will be croped in [0, 1].
 */
bear::audio::sound_effect::sound_effect( unsigned int loops, double volume )
  : m_volume(volume), m_loops(loops), m_position(NULL)
{
  if ( m_volume < 0 )
    m_volume = 0;
  else if ( m_volume > 1 )
    m_volume = 1;
} // sound_effect::sound_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param pos The position from which the sound is emitted.
 */
bear::audio::sound_effect::sound_effect
( const claw::math::coordinate_2d<double>& pos )
  : m_volume(1), m_loops(1),
    m_position( new claw::math::coordinate_2d<double>(pos) )
{

} // sound_effect::sound_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bear::audio::sound_effect::sound_effect( const sound_effect& that )
  : m_volume(that.m_volume), m_loops(that.m_loops), m_position(NULL)
{
  if ( that.has_a_position() )
    m_position = new claw::math::coordinate_2d<double>( that.get_position() );
} // sound_effect::sound_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::audio::sound_effect::~sound_effect()
{
  delete m_position;
} // sound_effect::~sound_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment.
 * \param that The instance to copy from.
 */
bear::audio::sound_effect&
bear::audio::sound_effect::operator=( const sound_effect& that )
{
  if ( this != &that )
    {
      delete m_position;
      m_position = NULL;

      m_volume = that.m_volume;
      m_loops = that.m_loops;

      if ( that.has_a_position() )
        m_position = new claw::math::coordinate_2d<double>
          ( that.get_position() );
    }

  return *this;
} // sound_effect::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the volume value.
 * \param volume The volume. Will be croped in [0, 1].
 */
void bear::audio::sound_effect::set_volume( double volume )
{
  m_volume = volume;

  if ( m_volume < 0 )
    m_volume = 0;
  else if ( m_volume > 1 )
    m_volume = 1;
} // sound_effect::set_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get volume value.
 */
double bear::audio::sound_effect::get_volume() const
{
  return m_volume;
} // sound_effect::get_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the number of loops.
 * \param loops The number of loops. A value of 0 means infinite.
 */
void bear::audio::sound_effect::set_loops( unsigned int loops )
{
  m_loops = loops;
} // sound_effect::set_loops()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of loops.
 * \remark See set_loops for a detailed explanation of the meaning of the
 *         values.
 */
int bear::audio::sound_effect::get_loops() const
{
  return m_loops;
} // sound_effect::get_loops()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the effect has a position.
 */
bool bear::audio::sound_effect::has_a_position() const
{
  return m_position != NULL;
} // sound_effect::has_a_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position.
 * \param pos The new position.
 */
void bear::audio::sound_effect::set_position
( const claw::math::coordinate_2d<double>& pos )
{
  if (m_position != NULL)
    *m_position = pos;
  else
    m_position = new claw::math::coordinate_2d<double>(pos);
} // sound_effect::set_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position.
 * \pre has_a_position() == true.
 */
const claw::math::coordinate_2d<double>&
bear::audio::sound_effect::get_position() const
{
  CLAW_PRECOND( has_a_position() );
  return *m_position;
} // sound_effect::get_position()
