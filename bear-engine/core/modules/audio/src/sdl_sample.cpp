/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::audio::sdl_sample class.
 * \author Julien Jorge
 */
#include "bear/audio/sdl_sample.hpp"

#include "bear/audio/sdl_sound.hpp"
#include "bear/audio/sound_manager.hpp"

#include <SDL2/SDL.h>
#include <claw/assert.hpp>
#include <claw/exception.hpp>
#include <claw/logger/logger.hpp>
#include <claw/types.hpp>
#include <cmath>
#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \post is_empty() == true.
 */
bear::audio::sdl_sample::channel_attribute::channel_attribute()
  : m_sample(NULL)
{

} // sdl_sample::channel_attribute::channel_attribute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::audio::sdl_sample::channel_attribute::~channel_attribute()
{
  clear();
} // sdl_sample::channel_attribute::~channel_attribute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the sample attribute.
 * \param s The sample.
 * \post is_empty() = false.
 */
void
bear::audio::sdl_sample::channel_attribute::set_sample( const sdl_sample& s )
{
  m_sample = &s;
} // sdl_sample::channel_attribute::set_sample()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the stored sample.
 * \pre is_empty() = false.
 */
const bear::audio::sdl_sample&
bear::audio::sdl_sample::channel_attribute::get_sample() const
{
  CLAW_PRECOND( m_sample != NULL );

  return *m_sample;
} // sdl_sample::channel_attribute::get_sample()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the effect of the sound.
 * \param effect The effect.
 */
void bear::audio::sdl_sample::channel_attribute::set_effect
( const sound_effect& effect )
{

  m_effect = effect;
} // sdl_sample::channel_attribute::set_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the effect of the sound.
 */
const bear::audio::sound_effect&
bear::audio::sdl_sample::channel_attribute::get_effect() const
{
  return m_effect;
} // sdl_sample::channel_attribute::get_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the sound attribute.
 */
void bear::audio::sdl_sample::channel_attribute::clear()
{
  m_sample = NULL;
} // sdl_sample::channel_attribute::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there's a sound associated.
 */
bool bear::audio::sdl_sample::channel_attribute::is_empty() const
{
  return m_sample == NULL;
} // sdl_sample::channel_attribute::is_empty()




/*----------------------------------------------------------------------------*/
std::vector<bear::audio::sdl_sample::channel_attribute*>
bear::audio::sdl_sample::s_playing_channels;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param s The sound of which we are a sample.
 * \param owner The instance of sound_manager who manage the sound.
 */
bear::audio::sdl_sample::sdl_sample( const sdl_sound& s, sound_manager& owner )
  : sample(s.get_sound_name(), owner), m_channel(-1), m_sound(&s)
{

} // sdl_sample::sdl_sample()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::audio::sdl_sample::~sdl_sample()
{
  stop();
} // sdl_sample::~sdl_sample()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start to play the sound.
 */
void bear::audio::sdl_sample::play()
{
  inside_play();
} // sdl_sample::play()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start to play the sound, with an effect.
 * \param effect The effect applied to the sound.
 */
void bear::audio::sdl_sample::play( const sound_effect& effect )
{
  m_effect = effect;

  inside_play();
} // sdl_sample::play() [effect]

/*----------------------------------------------------------------------------*/
/**
 * \brief Pause the sound.
 */
void bear::audio::sdl_sample::pause()
{
  if ( m_channel != -1 )
    Mix_Pause( m_channel );
} // sdl_sample::pause()

/*----------------------------------------------------------------------------*/
/**
 * \brief Resume the sound.
 */
void bear::audio::sdl_sample::resume()
{
  if ( m_channel != -1 )
    Mix_Resume( m_channel );
} // sdl_sample::resume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop the sound now.
 */
void bear::audio::sdl_sample::stop()
{
  if ( m_channel != -1 )
    Mix_HaltChannel( m_channel );

  m_channel = -1;

  sample_finished();
} // sdl_sample::stop()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop the sound after fading out.
 * \param d Fade out this duration (in seconds).
 */
void bear::audio::sdl_sample::stop( double d )
{
  if ( m_channel != -1 )
    {
      const int ms( d * 1000 + 0.5 );

      if ( ms <= 0 )
        stop();
      else if ( Mix_FadeOutChannel( m_channel, ms ) != 1 )
        stop();
    }
} // sdl_sample::stop()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the effect applied to this sample.
 */
bear::audio::sound_effect bear::audio::sdl_sample::get_effect() const
{
  if ( m_channel != -1 )
    return s_playing_channels[m_channel]->get_effect();
  else
    return m_effect;
} // sdl_sample::get_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Change the effect of the sample.
 * \param effect The new effect.
 */
void bear::audio::sdl_sample::set_effect( const sound_effect& effect )
{
  m_effect = effect;

  if ( m_channel != -1 )
    {
      if ( !Mix_UnregisterAllEffects(m_channel) )
        claw::logger << claw::log_warning << "sdl_sample::set_effect(): "
                     << Mix_GetError() << std::endl;

      inside_set_effect();
    }
} // sdl_sample::change_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the volume of the sample (for sound_manager only).
 * \param v The volume.
 */
void bear::audio::sdl_sample::set_volume( double v )
{
  if ( m_channel != -1 )
    Mix_Volume( m_channel, (int)(v * MIX_MAX_VOLUME) );
} // sdl_sample::set_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Callback function called when a channel is finished.
 * \param channel The finished channel.
 */
void bear::audio::sdl_sample::channel_finished(int channel)
{
  /* This is the last use of s_playing_channels[channel] for this sound.
     sdl_sample::finished() can't be const for now because it needs to remove
     channel from the m_channels list. That's why we exceptionally remove the
     const attribute. */
  sdl_sample* s = const_cast<sdl_sample*>
    (&s_playing_channels[channel]->get_sample());

  s->finished();
} // sdl_sample::channel_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Callback function applying a distance tone down effect to a channel.
 * \param channel The channel receiving the effect.
 * \param stream (in/out) Sound data.
 * \param length The size of the stream.
 * \param attr (in) Channel attribute.
 * \pre attr != NULL.
 */
void bear::audio::sdl_sample::distance_tone_down
(int channel, void* stream, int length, void* attr)
{
  CLAW_PRECOND( attr != NULL );
  CLAW_PRECOND( length >= 0 );
  CLAW_PRECOND( length % 2 == 0 );
  CLAW_PRECOND( sdl_sound::get_audio_format() == AUDIO_S16 );

  claw::int_16* buffer = static_cast<claw::int_16*>(stream);
  length /= 2;
  const channel_attribute* attribute = static_cast<channel_attribute*>(attr);

  const sdl_sound* sound = attribute->get_sample().m_sound;

  const claw::math::coordinate_2d<double> ears =
    sound->get_manager().get_ears_position();

  const claw::math::coordinate_2d<double> pos =
    attribute->get_effect().get_position();

  const double distance = std::abs(ears.x - pos.x) + std::abs(ears.y - pos.y);
  const double tone_down =
    sound->get_manager().get_volume_for_distance( distance );

  if ( tone_down <= std::numeric_limits<double>::epsilon() )
    std::fill( buffer, buffer + (unsigned int)length, 0 );
  else if ( tone_down < 1.0 )
    for (unsigned int i=0; i!=(unsigned)length; ++i)
      buffer[i] = (claw::int_16)((double)buffer[i] * tone_down);
} // sdl_sample::distance_tone_down()

/*----------------------------------------------------------------------------*/
/**
 * \brief Callback function applying the balance effect to a channel.
 * \param channel The channel receiving the effect.
 * \param stream (in/out) Sound data.
 * \param length The size of the stream.
 * \param attr (in) Channel attribute.
 * \pre attr != NULL.
 */
void bear::audio::sdl_sample::balance
(int channel, void* stream, int length, void* attr)
{
  CLAW_PRECOND( attr != NULL );
  CLAW_PRECOND( length >= 0 );
  CLAW_PRECOND( length % 2 == 0 );
  CLAW_PRECOND( sdl_sound::get_audio_format() == AUDIO_S16 );

  const channel_attribute* attribute = static_cast<channel_attribute*>(attr);

  const sdl_sound* sound = attribute->get_sample().m_sound;

  const claw::math::coordinate_2d<double> ears =
    sound->get_manager().get_ears_position();

  const claw::math::coordinate_2d<double> pos =
    attribute->get_effect().get_position();

  const double distance = std::abs(ears.x - pos.x);
  const double tone_down =
    sound->get_manager().get_volume_for_distance( distance );

  double left_volume;
  double right_volume;

  if ( ears.x < pos.x )
    {
      right_volume = 1;
      left_volume = tone_down;
    }
  else
    {
      right_volume = tone_down;
      left_volume = 1;
    }

  claw::int_16* buffer = static_cast<claw::int_16*>(stream);
  length /= 2;

  CLAW_PRECOND( length % 2 == 0 );

  for ( int i=0; i!=length; i += 2 )
    {
      buffer[i] *= left_volume;
      buffer[i+1] *= right_volume;
    }
} // sdl_sample::balance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Callback function changing the volume of a channel.
 * \param channel The channel receiving the effect.
 * \param stream (in/out) Sound data.
 * \param length The size of the stream.
 * \param attr (in) Channel attribute.
 * \pre attr != NULL.
 * \remark This method adjust the volume of the sample, independently of the
 *         global volume, actually stored in Mix_Volume().
 */
void bear::audio::sdl_sample::volume
(int channel, void* stream, int length, void* attr)
{
  CLAW_PRECOND( attr != NULL );
  CLAW_PRECOND( length % 2 == 0 );
  CLAW_PRECOND( sdl_sound::get_audio_format() == AUDIO_S16 );

  claw::int_16* buffer = static_cast<claw::int_16*>(stream);
  length /= 2;
  const channel_attribute* attribute = static_cast<channel_attribute*>(attr);

  const double v = attribute->get_effect().get_volume();

  if ( v <= std::numeric_limits<double>::epsilon() )
    std::fill( buffer, buffer + (unsigned int)length, 0 );
  else
    for (unsigned int i=0; i!=(unsigned)length; ++i)
      buffer[i] = (claw::int_16)((double)buffer[i] * v);
} // sdl_sample::volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start to play the sample.
 */
void bear::audio::sdl_sample::inside_play()
{
  if ( m_channel != -1 )
    stop();

  if ( m_sound != NULL )
    m_channel = m_sound->play(m_effect.get_loops());

  if ( m_channel != -1 )
    {
      set_playing();

      global_add_channel();
      Mix_Volume
        ( m_channel,
          (int)(m_sound->get_manager().get_volume(this) * MIX_MAX_VOLUME) );

      inside_set_effect();
    }
} // sdl_sample::inside_play()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the effect associated to a channel.
 * \pre m_channel >= 0
 * \pre s_playing_channels[channel]->is_empty() == false.
 * \post s_playing_channels[channel]->get_effect().get_position() == position.
 */
void bear::audio::sdl_sample::inside_set_effect()
{
  CLAW_PRECOND( m_channel >= 0 );
  CLAW_PRECOND( s_playing_channels[m_channel]->is_empty() == false );

  s_playing_channels[m_channel]->set_effect( m_effect );

  if ( m_effect.has_a_position() )
    {
      int ok;
      ok = Mix_RegisterEffect
        ( m_channel, distance_tone_down, NULL, s_playing_channels[m_channel] );

      if (!ok)
        claw::logger << claw::log_warning << "position effect distance: "
                     << Mix_GetError() << std::endl;

      ok = Mix_RegisterEffect
        ( m_channel, balance, NULL, s_playing_channels[m_channel] );

      if (!ok)
        claw::logger << claw::log_warning << "position effect balance: "
                     << Mix_GetError() << std::endl;
    }

  if ( m_effect.get_volume() != 1 )
    {
      int ok;
      ok = Mix_RegisterEffect
        ( m_channel, volume, NULL, s_playing_channels[m_channel] );

      if (!ok)
        claw::logger << claw::log_warning << "volume effect: "
                     << Mix_GetError() << std::endl;
    }
} // sdl_sample::inside_set_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Associate the current instance to a channel in the common list.
 * \pre m_channel >= 0
 * \pre s_playing_channels[m_channel].is_empty().
 * \post &sdl_sample::s_playing_channels[m_channel].get_sound() == this.
 */
void bear::audio::sdl_sample::global_add_channel()
{
  CLAW_PRECOND( m_channel >= 0 );

  if ( (unsigned int)m_channel >= s_playing_channels.size() )
    s_playing_channels.resize(m_channel+1, NULL);
  else
    {
      CLAW_PRECOND( s_playing_channels[m_channel] == NULL );
    }

  s_playing_channels[m_channel] = new channel_attribute;
  s_playing_channels[m_channel]->set_sample(*this);
} // sdl_sample::global_add_channel()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a finished channel from the playing list.
 * \pre m_channel >= 0
 * \post sdl_sample::s_playing_channels[channel]->is_empty() = true.
 * \post m_channels.find( channel ) == m_channels.end().
 */
void bear::audio::sdl_sample::finished()
{
  CLAW_PRECOND( m_channel >= 0 );

  if ( !Mix_UnregisterAllEffects(m_channel) )
    claw::logger << claw::log_warning << "sdl_sample::finished(): "
                 << Mix_GetError() << std::endl;

  delete s_playing_channels[m_channel];
  s_playing_channels[m_channel] = NULL;

  m_channel = -1;

  sample_finished();
} // sdl_sample::finished()
