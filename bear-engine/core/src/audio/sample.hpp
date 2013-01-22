/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class representing a sound sample.
 * \author Julien Jorge
 */
#ifndef __AUDIO_SAMPLE_HPP__
#define __AUDIO_SAMPLE_HPP__

#include "audio/sound_effect.hpp"
#include "audio/class_export.hpp"

#include <claw/coordinate_2d.hpp>
#include <claw/non_copyable.hpp>
#include <string>

namespace bear
{
  namespace audio
  {
    class sound_manager;

    /**
     * \brief A class representing a sound.
     */
    class AUDIO_EXPORT sample:
      public claw::pattern::non_copyable
    {
    public:
      explicit sample( const std::string& name );
      sample( const std::string& name, sound_manager& owner );
      virtual ~sample();

      sample* clone() const;

      std::size_t get_id() const;
      const std::string& get_sound_name() const;

      virtual void play();
      virtual void play( const sound_effect& effect );
      virtual void pause();
      virtual void resume();
      virtual void stop();
      virtual void stop( double d );

      virtual sound_effect get_effect() const;
      virtual void set_effect( const sound_effect& effect );

      /* for sound_manager only. */
      virtual void set_volume( double v );

      bool is_playing() const;

    protected:
      void sample_finished();
      void set_playing();

    private:
      /** \brief The sound of which we are a sample. */
      sound_manager* m_manager;

      /** \brief The identifier of the sound. */
      const std::size_t m_id;

      /** \brief The name of the sound resource. */
      const std::string& m_name;

      /** \brief Tells if this sample is currently playing. */
      bool m_is_playing;

      /** \brief The next available identifier. */
      static std::size_t s_next_id;

    }; // class sample
  } // namespace audio
} // namespace bear

#endif // __AUDIO_SAMPLE_HPP__
