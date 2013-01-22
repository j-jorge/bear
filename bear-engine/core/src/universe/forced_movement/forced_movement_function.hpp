/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A forced movement that will assign the result of a function to the
 *        center of mass on an item.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_FORCED_MOVEMENT_FUNCTION_HPP__
#define __UNIVERSE_FORCED_MOVEMENT_FUNCTION_HPP__

#include "universe/forced_movement/base_forced_movement.hpp"

#include <limits>
#include <boost/function.hpp>

namespace bear
{
  namespace universe
  {
    /**
     * \brief A forced movement that will assign the result of a function to the
     *        center of mass on an item.
     *
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT forced_movement_function:
      public base_forced_movement
    {
    public:
      /** \brief The type of the function called to move the item. */
      typedef boost::function<position_type ()> function_type;

    public:
      forced_movement_function
        ( const function_type& f,
          time_type length = std::numeric_limits<time_type>::infinity() );

      base_forced_movement* clone() const;

      void set_total_time( time_type length );

      bool is_finished() const;

    private:
      void do_init();
      time_type do_next_position( time_type elapsed_time );

    private:
      /** \brief Total time of the movement. */
      time_type m_total_time;

      /** \brief Remaining time. */
      time_type m_remaining_time;

      /** \brief The function called to move the item. */
      function_type m_function;

    }; // class forced_movement_function
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_FORCED_MOVEMENT_FUNCTION_HPP__
