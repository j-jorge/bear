/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::audio::sdl_sound class.
 * \author Julien Jorge
 */
#include "bear/audio/sdl_sound.hpp"
#include "bear/audio/sdl_sample.hpp"
#include "bear/audio/sound_manager.hpp"

#include <SDL2/SDL.h>
#include <claw/assert.hpp>
#include <claw/exception.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
unsigned int bear::audio::sdl_sound::s_audio_rate = 44100;
unsigned int bear::audio::sdl_sound::s_audio_format = AUDIO_S16;
unsigned int bear::audio::sdl_sound::s_audio_channels = 2;
unsigned int bear::audio::sdl_sound::s_audio_buffers = 1024;
unsigned int bear::audio::sdl_sound::s_audio_mix_channels = 256;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param file The stream containing the wav file.
 * \param name The name of the sound resource.
 * \param owner The instance of sound_manager who stores me.
 */
bear::audio::sdl_sound::sdl_sound
( std::istream& file, const std::string& name, sound_manager& owner )
  : sound(name, owner), m_sound(NULL), m_raw_audio(NULL)
{
  file.seekg( 0, std::ios::end );
  std::streamoff file_size = file.tellg();
  file.seekg( 0, std::ios::beg );

  char* buffer = new char[file_size];
  file.read( buffer, file_size );

  m_loader =
    new boost::thread
    ( boost::bind( &sdl_sound::load_sound, this, buffer, file_size ) );
} // sdl_sound::sdl_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 * \param owner The instance of sound_manager who stores me.
 */
bear::audio::sdl_sound::sdl_sound
( const sdl_sound& that, sound_manager& owner )
  : sound(that.get_sound_name(), owner), m_sound(NULL), m_loader(NULL)
{
  const Uint32 buffer_length( that.m_sound->alen );
  m_raw_audio = new Uint8[buffer_length];
  
  std::copy
    ( that.m_sound->abuf, that.m_sound->abuf + buffer_length, m_raw_audio );
  
  m_sound = Mix_QuickLoad_RAW( m_raw_audio, buffer_length );

  if (!m_sound)
    {
      delete[] m_raw_audio;
      throw claw::exception( Mix_GetError() );
    }
} // sdl_sound::sdl_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::audio::sdl_sound::~sdl_sound()
{
  ensure_loaded();

  delete m_loader;

  Mix_FreeChunk( m_sound );

  delete[] m_raw_audio;
} // sdl_sound::~sdl_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a new sample of this sound.
 */
bear::audio::sample* bear::audio::sdl_sound::new_sample()
{
  return new sdl_sample( *this, get_manager() );
} // sdl_sound::new_sample()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start to play the sound.
 * \param loops Number of loops, 0 for infinite.
 * \return The channel in which the sound is played.
 */
int bear::audio::sdl_sound::play( unsigned int loops ) const
{
  ensure_loaded();

  const int sdl_loops((int)loops - 1);
  const int channel = Mix_PlayChannel(-1, m_sound, sdl_loops);

  if (channel == -1)
    claw::logger << claw::log_warning << "sdl_sound::play(): "
                 << Mix_GetError() << std::endl;

  return channel;
} // sdl_sound::play()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the SDL.
 */
bool bear::audio::sdl_sound::initialize()
{
  bool result = false;

  if ( SDL_InitSubSystem(SDL_INIT_AUDIO) != 0 )
    claw::logger << claw::log_error << SDL_GetError() << std::endl;
  else if ( Mix_OpenAudio(s_audio_rate, s_audio_format, s_audio_channels,
                          s_audio_buffers) != 0 )
    claw::logger << claw::log_error << Mix_GetError() << std::endl;
  else
    {
      result = true;
      Mix_AllocateChannels(s_audio_mix_channels);
      Mix_ChannelFinished(sdl_sample::channel_finished);
    }

  return result;
} // sdl_sound::initialize()

/*----------------------------------------------------------------------------*/
/**
 * \brief Close the SDL.
 */
void bear::audio::sdl_sound::release()
{
  SDL_QuitSubSystem(SDL_INIT_AUDIO);
} // sdl_sound::release()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the format of the output audio stream.
 */
unsigned int bear::audio::sdl_sound::get_audio_format()
{
  return s_audio_format;
} // sdl_sound::get_audio_format()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads the sound from a given data.
 * \param buffer The buffer from which the sound is read. It will be deleted by
 *        the end of the call.
 * \param buffer_length The length of buffer.
 */
void bear::audio::sdl_sound::load_sound
( char* buffer, std::size_t buffer_length )
{
  SDL_RWops* rw( SDL_RWFromMem(buffer, buffer_length) );

  if (rw)
    m_sound = Mix_LoadWAV_RW( rw, 1 );

  delete[] buffer;

  if (!m_sound)
    {
      claw::logger << claw::log_error << Mix_GetError() << std::endl;
      throw claw::exception( Mix_GetError() );
    }
} // sdl_sound::load_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Ensures the sound has been fully loaded. The function will force the
 *        loading to finish if the sound has not been loaded yet.
 */
void bear::audio::sdl_sound::ensure_loaded() const
{
  if ( m_loader == NULL )
    return;

  m_loader->join();
} // sdl_sound::ensure_loaded()
