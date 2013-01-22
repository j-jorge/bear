/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A forced movement that will make the current item rotating around
 *        another item.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_FORCED_STAY_AROUND_HPP__
#define __UNIVERSE_FORCED_STAY_AROUND_HPP__

#include "universe/forced_movement/base_forced_movement.hpp"

#include "universe/class_export.hpp"

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
    class UNIVERSE_EXPORT forced_stay_around:
      public base_forced_movement
    {
    public:
      forced_stay_around
      ( time_type length = std::numeric_limits<time_type>::infinity() );

      base_forced_movement* clone() const;

      void set_max_angle( double a );
      void set_speed( double s );
      void set_max_distance( coordinate_type d );
      void set_apply_angle( bool b );
      void set_total_time( time_type length );

      bool is_finished() const;

    private:
      void do_init();
      time_type do_next_position( time_type elapsed_time );

      time_type compute_remaining_time( time_type& elapsed_time );
      double compute_direction
        ( vector_type& dir, vector_type current_dir ) const;

    private:
      /** \brief Maximum angle when the direction changes. */
      double m_max_angle;

      /** \brief The constant speed of the moving item. */
      double m_speed;

      /** \brief The maximum distance to the center. */
      coordinate_type m_max_distance;

      /** \brief Total time of the movement. */
      time_type m_total_time;

      /** \brief Remaining time. */
      time_type m_remaining_time;

      /** \brief Tell if we apply the angle of the movement to the item. */
      bool m_apply_angle;

      /** \brief The previous position of the moving item, relatively to the
          reference. */
      position_type m_position_delta;

      /** \brief The previous position of the reference item. */
      position_type m_previous_reference_position;

    }; // class forced_stay_around
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_FORCED_STAY_AROUND_HPP__
