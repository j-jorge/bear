/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A speed generator which, for a given time-window and a distance,
 *        applies a sinusoïdal speed curve during the acceleration time and the
 *        deceleration time.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_SINUS_SPEED_GENERATOR_HPP__
#define __UNIVERSE_SINUS_SPEED_GENERATOR_HPP__

#include "universe/types.hpp"

#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief A speed generator which, for a given time-window and a distance,
     *        applies a sinusoïdal speed curve during the acceleration time and
     *        the deceleration time.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT sinus_speed_generator
    {
    public:
      sinus_speed_generator();

      void set_total_time( time_type length );
      time_type get_total_time() const;

      void set_acceleration_time( time_type length );
      time_type get_acceleration_time() const;

      void set_distance( coordinate_type d );
      coordinate_type get_distance() const;

      coordinate_type get_speed( time_type t ) const;

    private:
      /** \brief Total time to reach the target. */
      time_type m_total_time;

      /** \brief The duration of the acceleration and deceleration. */
      time_type m_acceleration_time;

      /** \brief The total distance of the journey. */
      coordinate_type m_distance;

    }; // class sinus_speed_generator
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_SINUS_SPEED_GENERATOR_HPP__
