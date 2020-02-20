/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementaion of the bear::audio::sample class.
 * \author Julien Jorge
 */
#include "bear/audio/sample.hpp"

#include "bear/audio/sound_manager.hpp"
#include <cstdlib>

/*----------------------------------------------------------------------------*/
std::size_t bear::audio::sample::s_next_id(1);

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the sound resource.
 */
bear::audio::sample::sample( const std::string& name )
  : m_manager(NULL), m_id(s_next_id), m_name(name), m_is_playing(false)
{
  ++s_next_id;
} // sample::sample()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the sound resource.
 * \param owner The instance of sound_manager who manages the sound.
 */
bear::audio::sample::sample(  const std::string& name, sound_manager& owner )
  : m_manager(&owner), m_id(s_next_id), m_name(name), m_is_playing(false)
{
  ++s_next_id;
} // sample::sample()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::audio::sample::~sample()
{
  stop();

  if (m_manager != NULL)
    m_manager->sample_deleted(this);
} // sample::~sample()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this sample.
 */
bear::audio::sample* bear::audio::sample::clone() const
{
  sample* result = NULL;

  if ( m_manager != NULL )
    {
      result = m_manager->new_sample( get_sound_name() );
      result->set_effect(get_effect());
    }

  return result;
} // sample::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the identifier of the sample.
 * \remark identifiers are strictly greater than zero.
 */
std::size_t bear::audio::sample::get_id() const
{
  return m_id;
} // sample::get_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the sound resource.
 */
const std::string& bear::audio::sample::get_sound_name() const
{
  return m_name;
} // sample::get_sound_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start to play the sound.
 */
void bear::audio::sample::play()
{
  set_playing();
  stop();
} // sample::play()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start to play the sound, with an effect.
 * \param effect The effect applied to the sound.
 */
void bear::audio::sample::play( const sound_effect& effect )
{
  set_playing();
  stop();
} // sample::play()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pause the sound.
 */
void bear::audio::sample::pause()
{
  /* nothing. */
} // sample::pause()

/*----------------------------------------------------------------------------*/
/**
 * \brief Resume the sound.
 */
void bear::audio::sample::resume()
{
  /* nothing. */
} // sample::resume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop the sample.
 */
void bear::audio::sample::stop()
{
  sample_finished();
} // sample::stop()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop the sound after fading out.
 * \param d Fade out this duration (in seconds).
 */
void bear::audio::sample::stop( double d )
{
  sample_finished();
} // sample::stop()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the effect of the sample.
 */
bear::audio::sound_effect bear::audio::sample::get_effect() const
{
  return sound_effect();
} // sample::get_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the effect of the sample.
 * \param effect The new effect.
 */
void bear::audio::sample::set_effect( const sound_effect& effect )
{

} // sample::set_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the volume of the sample (for sound_manager only).
 * \param v The volume.
 */
void bear::audio::sample::set_volume( double v )
{

} // sample::set_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this sample is currently playing.
 */
bool bear::audio::sample::is_playing() const
{
  return m_is_playing;
} // sample::is_playing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the manager that this sample is finished.
 */
void bear::audio::sample::sample_finished()
{
  m_is_playing = false;

  if (m_manager != NULL)
    m_manager->sample_finished(this);
} // sample::sample_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Flag this sample as currently playing.
 */
void bear::audio::sample::set_playing()
{
  m_is_playing = true;
} // sample::sample_finished()
