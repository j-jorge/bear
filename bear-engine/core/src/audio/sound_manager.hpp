/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class to manage sound resources.
 * \author Julien Jorge
 */
#ifndef __AUDIO_SOUND_MANAGER_HPP__
#define __AUDIO_SOUND_MANAGER_HPP__

#include <claw/coordinate_2d.hpp>
#include <iostream>
#include <map>
#include <list>
#include <string>

#include "audio/class_export.hpp"

namespace bear
{
  namespace audio
  {
    class sample;
    class sound;
    class sound_effect;

    /**
     * \brief A class to manage sound resources.
     * \author Julien Jorge
     */
    class AUDIO_EXPORT sound_manager
    {
    private:
      /** \brief The information stored when muting a music. */
      typedef std::pair<sample*, sound_effect> muted_music_data;

      /** \brief The list of musics muted by the current music. */
      typedef std::list<muted_music_data> muted_music_list;

    public:
      sound_manager();
      ~sound_manager();

      void clear();
      void load_sound( const std::string& name, std::istream& file );

      void play_sound( const std::string& name );
      void play_sound( const std::string& name, const sound_effect& effect );

      sample* new_sample( const std::string& name );
      sample* new_sample( const sample& s );
      std::size_t play_music( const std::string& name, unsigned int loops );
      void stop_music( std::size_t id, double fadeout = 0 );
      void stop_all_musics( double fadeout = 0 );

      void
      set_ears_position( const claw::math::coordinate_2d<double>& position );
      const claw::math::coordinate_2d<double>& get_ears_position() const;

      void set_sound_volume( double v );
      void set_music_volume( double v );

      double get_sound_volume() const;
      double get_music_volume() const;
      double get_volume( const sample* s ) const;

      bool sound_exists( const std::string& name ) const;

      void sample_finished( sample* s );
      void sample_deleted( sample* s );

      void pause_all();
      void resume_all();
      void stop_all();

      void set_full_volume_distance( double d );
      double get_full_volume_distance() const;

      void set_silence_distance( double d );
      double get_silence_distance() const;

      void set_distance_unit( double d );
      double get_distance_unit() const;

      double get_volume_for_distance( double d ) const;

      static void initialize();
      static void release();

    private:
      void remove_muted_music( sample* m );
      bool is_music( const sample* m ) const;

    private:
      /** \brief All sounds. */
      std::map<std::string, sound*> m_sounds;

      /** \brief Ears position. */
      claw::math::coordinate_2d<double> m_ears_position;

      /** \brief The samples currently played and the boolean telling if they
          are managed or not. */
      std::map<sample*, bool> m_samples;

      /** \brief The music currently played. */
      sample* m_current_music;

      /** \brief The musics muted by the current music. */
      muted_music_list m_muted_musics;

      /** \brief The volume of the sounds, in [0, 1]. */
      double m_sound_volume;

      /** \brief The volume of the music, in [0, 1]. */
      double m_music_volume;

      /** \brief Distance from which we can't hear a sound. */
      double m_silence_distance;

      /** \brief Distance under which sounds are at maximum volume. */
      double m_full_volume_distance;

      /** \brief The unit of the measures applied in the computation of the
          distances for the sound effects. */
      double m_distance_unit;

      /** \brief Tell if the sound system is initialized. */
      static bool s_initialized;

    }; // class sound_manager
  } // namespace audio
} // namespace bear

#endif // __AUDIO_SOUND_MANAGER_HPP__
