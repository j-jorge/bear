/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Basic class for basic not progressive sounds effects.
 * \author Julien Jorge
 */
#ifndef __AUDIO_SOUND_EFFECT_HPP__
#define __AUDIO_SOUND_EFFECT_HPP__

#include <claw/coordinate_2d.hpp>
#include "audio/class_export.hpp"

namespace bear
{
  namespace audio
  {
    /**
     * \brief Basic class for basic not progressive sounds effects.
     */
    class AUDIO_EXPORT sound_effect
    {
    public:
      sound_effect();
      explicit sound_effect( double volume );
      explicit sound_effect( unsigned int loops, double volume = 1 );
      explicit sound_effect( const claw::math::coordinate_2d<double>& pos );
      sound_effect( const sound_effect& that );
      ~sound_effect();

      sound_effect& operator=( const sound_effect& that );

      void set_volume( double volume );
      double get_volume() const;

      void set_loops( unsigned int loops );
      int get_loops() const;

      bool has_a_position() const;
      void set_position( const claw::math::coordinate_2d<double>& pos );
      const claw::math::coordinate_2d<double>& get_position() const;

    private:
      /** \brief Sound volume, in [0, 1]. */
      double m_volume;

      /** \brief Number of loops (added to a first default play). */
      int m_loops;

      /** \brief The position, if any. */
      claw::math::coordinate_2d<double>* m_position;

    }; // class sound_effect
  } // namespace audio
} // namespace bear

#endif // __AUDIO_SOUND_EFFECT_HPP__
