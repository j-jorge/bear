/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class representing a sound sample. This class uses the SDL_mixer
 *        library.
 * \author Julien Jorge
 */
#ifndef __AUDIO_SDL_SAMPLE_HPP__
#define __AUDIO_SDL_SAMPLE_HPP__

#include "audio/sample.hpp"

#include "audio/class_export.hpp"
#include <vector>

namespace bear
{
  namespace audio
  {
    class sdl_sound;

    /**
     * \brief A class representing a sound sample.
     */
    class AUDIO_EXPORT sdl_sample:
      public sample
    {
    private:
      /**
       * \brief Informations stored in the s_playing_channels vector.
       */
      class AUDIO_EXPORT channel_attribute
      {
      public:
        channel_attribute();
        ~channel_attribute();

        void set_sample( const sdl_sample& s );
        const sdl_sample& get_sample() const;

        void set_effect( const sound_effect& effect );
        const sound_effect& get_effect() const;

        void clear();
        bool is_empty() const;

      private:
        /** \brief The sample in this channel. */
        const sdl_sample* m_sample;

        /** \brief The effect applied to the sound. */
        sound_effect m_effect;

      }; // class channel_attribute

    public:
      sdl_sample( const sdl_sound& s, sound_manager& owner );
      ~sdl_sample();

      void play();
      void play( const sound_effect& effect );
      void pause();
      void resume();
      void stop();
      void stop( double d );

      sound_effect get_effect() const;
      void set_effect( const sound_effect& effect );

      /* for sound_manager only. */
      void set_volume( double v );

      static void channel_finished(int channel);

    private:
      static void distance_tone_down
      ( int channel, void *stream, int length, void *position );
      static void balance
      ( int channel, void *stream, int length, void *position );
      static void volume
      ( int channel, void *stream, int length, void *position );

      void inside_play();
      void stop_sample();

      void inside_set_effect();

      void global_add_channel();
      void finished();

    private:
      /** \brief The channel in which this sample is played. */
      int m_channel;

      /** \brief The sound of which we are a sample. */
      const sdl_sound* m_sound;

      /** \brief The effects applied to the sample, by default. */
      sound_effect m_effect;

      /** \brief Global vector giving, for a channel, the sample currently
          played. */
      static std::vector<channel_attribute*> s_playing_channels;

    }; // class sdl_sample
  } // namespace audio
} // namespace bear

#endif // __AUDIO_SDL_SAMPLE_HPP__
