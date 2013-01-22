/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An animation.
 * \author Julien Jorge
 */
#ifndef __VISUAL_ANIMATION_HPP__
#define __VISUAL_ANIMATION_HPP__

#include "visual/sprite_sequence.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief An animation.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT animation:
      public sprite_sequence
    {
    public:
      animation();
      animation( const sprite& spr );
      animation
      ( const std::vector<sprite>& images, const std::vector<double>& d );

      void set_time_factor( double f );
      double get_time_factor() const;

      void reset();
      void next( double t );
      bool is_finished() const;

    private:
      double get_scaled_duration( std::size_t i ) const;

    private:
      /** \brief The duration of the frames. */
      std::vector<double> m_duration;

      /** \brief Time spent on the current frame. */
      double m_time;

      /** \brief A factor applied to the duration of the frames. */
      double m_time_factor;

    }; // class animation

  } // namespace visual
} // namespace bear

#endif // __VISUAL_ANIMATION_HPP__
