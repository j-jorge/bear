/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A forced movement that will make the current item rotating around
 *        another item.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_FORCED_AIMING_HPP__
#define __UNIVERSE_FORCED_AIMING_HPP__

#include "bear/universe/forced_movement/base_forced_movement.hpp"

#include "bear/universe/class_export.hpp"

#include <limits>

namespace bear
{
  namespace universe
  {
    /**
     * \brief A forced movement that will make the current item going in the
     *        direction of an other item.
     *
     * The positions considered for the items are their center of mass.
     *
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT forced_aiming:
      public base_forced_movement
    {
    public:
      forced_aiming
      ( time_type length = std::numeric_limits<time_type>::infinity() );

      base_forced_movement* clone() const;

      void set_max_angle( double a );
      void set_max_speed( double s );
      void set_acceleration( double a );
      void set_apply_angle( bool b );
      void set_total_time( time_type length );

      bool is_finished() const;

    private:
      void do_init();
      time_type do_next_position( time_type elapsed_time );

      time_type compute_remaining_time( time_type& elapsed_time );
      double compute_speed( time_type elapsed_time ) const;
      double compute_direction( vector_type& dir ) const;

    private:
      /** \brief Maximum angle when the direction changes. */
      double m_max_angle;

      /** \brief The maximum speed of the moving item. */
      double m_max_speed;

      /** \brief The acceleration of the moving item. */
      double m_acceleration;

      /** \brief Total time of the movement. */
      time_type m_total_time;

      /** \brief Remaining time. */
      time_type m_remaining_time;

      /** \brief Tell if we apply the angle of the movement to the item. */
      bool m_apply_angle;

    }; // class forced_aiming
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_FORCED_AIMING_HPP__
