/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A forced movement that applies a natural movement to the moving
 *        item.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_NATURAL_FORCED_MOVEMENT_HPP__
#define __UNIVERSE_NATURAL_FORCED_MOVEMENT_HPP__

#include <climits>
#include <limits>

#include "universe/forced_movement/base_forced_movement.hpp"

#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief A forced movement that applies a natural movement to the moving
     *        item.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT natural_forced_movement:
      public base_forced_movement
    {
    public:
      natural_forced_movement
      ( time_type duration = std::numeric_limits<time_type>::infinity() );

      base_forced_movement* clone() const;

      void set_total_time( time_type t );

      bool is_finished() const;

    private:
      void do_init();
      time_type do_next_position( time_type elapsed_time );

      void update_position( time_type elapsed_time );

    private:
      /** \brief Total time of the movement. */
      time_type m_duration;

      /** \brief Remaining time. */
      time_type m_remaining_time;

    }; // class natural_forced_movement

  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_NATURAL_FORCED_MOVEMENT_HPP__
