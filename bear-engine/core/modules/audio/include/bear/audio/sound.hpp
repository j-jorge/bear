/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class representing a sound.
 * \author Julien Jorge
 */
#ifndef __AUDIO_SOUND_HPP__
#define __AUDIO_SOUND_HPP__

#include "bear/audio/sound_manager.hpp"
#include "bear/audio/class_export.hpp"

namespace bear
{
  namespace audio
  {
    class sample;
    class sound_manager;

    /**
     * \brief A class representing a sound.
     */
    class AUDIO_EXPORT sound
    {
    public:
      sound( const std::string& name, sound_manager& owner );
      virtual ~sound();

      virtual sample* new_sample();

      sound_manager& get_manager();
      const sound_manager& get_manager() const;

      const std::string& get_sound_name() const;

    private:
      /** \brief The sound_manager who stores me. */
      sound_manager& m_owner;

      /** \brief The name of the sound resource. */
      const std::string m_name;

    }; // class sound
  } // namespace audio
} // namespace bear

#endif // __AUDIO_SOUND_HPP__
