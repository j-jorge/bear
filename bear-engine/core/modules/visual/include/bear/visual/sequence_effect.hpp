/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An effect that can be applied to a sequence of elements.
 * \author Julien Jorge
 */
#ifndef __VISUAL_SEQUENCE_EFFECT_HPP__
#define __VISUAL_SEQUENCE_EFFECT_HPP__

#include "bear/visual/types.hpp"

#include "bear/visual/class_export.hpp"

#include <vector>

namespace bear
{
  namespace visual
  {
    /**
     * \brief An effect that can be applied to a sequence of elements.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT sequence_effect
    {
    public:
      sequence_effect();

      void add_color( color_type c );
      color_type get_color( std::size_t i ) const;

      void set_wave_length( std::size_t length );
      void set_wave_height( size_type h );
      void set_wave_speed( size_type s );
      void set_decrease_coefficient( size_type coefficient );

      position_type get_delta( std::size_t i ) const;

      void update( double t );

    private:
      /** \brief The current date of the effect. */
      double m_time;

      /** \brief The height to decrease per second. */
      size_type m_decrease_coefficient;

      /** \brief The colors among which we pick. */
      std::vector<color_type> m_colors;

      /** \brief The length of the wave, in term of elements in the sequence. */
      std::size_t m_wave_length;

      /** \brief The height of the wave. */
      size_type m_wave_height;

      /** \brief The speed of the wave. */
      size_type m_wave_speed;

    }; // class sequence_effect
  } // namespace visual
} // namespace bear

#endif // __VISUAL_SEQUENCE_EFFECT_HPP__
