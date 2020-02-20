/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class representing a sound. This class uses the SDL_mixer library.
 * \author Julien Jorge
 */
#ifndef __AUDIO_SDL_SOUND_HPP__
#define __AUDIO_SDL_SOUND_HPP__

#include "bear/audio/sound.hpp"

#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <boost/thread.hpp>

#include "bear/audio/class_export.hpp"

namespace bear
{
  namespace audio
  {
    class sound_manager;

    /**
     * \brief A class representing a sound.
     */
    class AUDIO_EXPORT sdl_sound:
      public sound
    {
    public:
      sdl_sound
        ( std::istream& file, const std::string& name, sound_manager& owner );
      sdl_sound( const sdl_sound& that, sound_manager& owner );
      ~sdl_sound();

      sample* new_sample();

      int play( unsigned int loops ) const;

      static bool initialize();
      static void release();

      static unsigned int get_audio_format();

    private:
      void load_sound( char* buffer, std::size_t buffer_length );

      void ensure_loaded() const;

    private:
      /** \brief The sound allocated by SDL_mixer. */
      Mix_Chunk* m_sound;

      /** \brief The thread loading the sound. */
      boost::thread* m_loader;

      /** \brief The raw audio data passed to the SDL mixer functions,
          eventually. */
      Uint8* m_raw_audio;

      /** \brief Output audio rate. */
      static unsigned int s_audio_rate;

      /** \brief Output audio format. */
      static unsigned int s_audio_format;

      /** \brief Number of channels. */
      static unsigned int s_audio_channels;

      /** \brief Size of the buffer. */
      static unsigned int s_audio_buffers;

      /** \brief Count of channels for mixing. */
      static unsigned int s_audio_mix_channels;

    }; // class sdl_sound
  } // namespace audio
} // namespace bear

#endif // __AUDIO_SDL_SOUND_HPP__
