/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A forced movement that will try to bring an item to an other item.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_FORCED_JOIN_HPP__
#define __UNIVERSE_FORCED_JOIN_HPP__

#include "bear/universe/forced_movement/base_forced_movement.hpp"

#include "bear/universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief A forced movement that will try to bring an item to an other item.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT forced_join:
      public base_forced_movement
    {
    public:
      forced_join( time_type length = 0 );

      base_forced_movement* clone() const;

      void set_total_time( time_type length );

      bool is_finished() const;

    private:
      void do_init();
      time_type do_next_position( time_type elapsed_time );

    private:
      /** \brief Total time to reach the target. */
      time_type m_total_time;

      /** \brief Remaining time to reach the target. */
      time_type m_remaining_time;

      /** \brief The initial position of moving item. */
      bear::universe::position_type m_initial_item_position;

      /** \brief The initial position of target item. */
      bear::universe::position_type m_initial_target_position;      
    }; // class forced_join
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_FORCED_JOIN_HPP__
