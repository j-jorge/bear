/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A forced movement that will force the current item to be at a precise
 *        distance of an other item.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_FORCED_TRACKING_HPP__
#define __UNIVERSE_FORCED_TRACKING_HPP__

#include "bear/universe/forced_movement/base_forced_movement.hpp"

#include "bear/universe/class_export.hpp"

#include <limits>

namespace bear
{
  namespace universe
  {
    /**
     * \brief A forced movement that will force the current item to be at a
     *        precise distance of an other item.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT forced_tracking:
      public base_forced_movement
    {
    public:
      forced_tracking
      ( time_type length = std::numeric_limits<time_type>::infinity() );
      forced_tracking
      ( const position_type& distance,
        time_type length = std::numeric_limits<time_type>::infinity() );

      base_forced_movement* clone() const;

      void set_distance( const position_type& distance );
      void set_total_time( time_type length );

      const position_type& get_distance() const;

      bool is_finished() const;

    private:
      void do_init();
      time_type do_next_position( time_type elapsed_time );

    private:
      /** \brief Stay at this distance of the reference. */
      position_type m_distance;

      /** \brief Total time of the movement. */
      time_type m_total_time;

      /** \brief Remaining time. */
      time_type m_remaining_time;

    }; // class forced_tracking
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_FORCED_TRACKING_HPP__

