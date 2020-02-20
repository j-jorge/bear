/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A forced movement that will bring an item to a given position.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_FORCED_GOTO_HPP__
#define __UNIVERSE_FORCED_GOTO_HPP__

#include "bear/universe/forced_movement/base_forced_movement.hpp"
#include "bear/universe/forced_movement/sinus_speed_generator.hpp"

#include "bear/universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief A forced movement that will bring an item to a given position.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT forced_goto:
      public base_forced_movement
    {
    public:
      forced_goto( time_type length = 1 );

      base_forced_movement* clone() const;

      void set_total_time( time_type length );
      void set_acceleration_time( time_type length );
      void set_length( const vector_type& v );
      void set_x_length( coordinate_type v );
      void set_y_length( coordinate_type v );

      bool is_finished() const;

    private:
      void do_init();
      time_type do_next_position( time_type elapsed_time );

      coordinate_type get_speed( time_type t ) const;

    private:
      /** \brief Total time to reach the target. */
      time_type m_total_time;

      /** \brief Time elapsed since the begining of the movement. */
      time_type m_elapsed_time;

      /** \brief The position to reach. */
      position_type m_target_position;

      /** \brief The length of the journey. */
      vector_type m_length;

      /** \brief The speed generator used to move the item. */
      sinus_speed_generator m_speed_generator;

    }; // class forced_goto
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_FORCED_GOTO_HPP__
