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
#ifndef __UNIVERSE_FORCED_ROTATION_HPP__
#define __UNIVERSE_FORCED_ROTATION_HPP__

#include "bear/universe/forced_movement/base_forced_movement.hpp"
#include "bear/universe/forced_movement/sinus_speed_generator.hpp"

#include "bear/universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief A forced movement that will make the current item rotating around
     *        another item.
     *
     * The positions considered for the items are their center of mass.
     *
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT forced_rotation:
      public base_forced_movement
    {
    public:
      /** \brief How the angle of the rotation is applied to the item. */
      enum angle_application
        {
          /** \brief The angle is added to the angle of the item. */
          apply_add,

          /** \brief The angle is given to the item, regardless of its previous
              value. */
          apply_force,

          /** \brief The angle of the item is not changed. */
          apply_keep,

          /** \forte The angle of the item is the difference between the current
              angle and the initial one. */
          apply_force_delta,

          /** \forte The angle of the item is the difference between the final
              angle and the current one. */
          apply_force_remaining

        }; // enum angle_application

    public:
      forced_rotation();

      base_forced_movement* clone() const;

      void set_radius( double radius );
      void set_start_angle( double start_angle );
      void set_end_angle( double end_angle );
      void set_angle_application( angle_application a );
      void set_total_time( time_type length );
      void set_acceleration_time( time_type length );
      void set_loops( unsigned int loops );
      void set_loop_back( bool loop_back );

      bool is_finished() const;

    private:
      void do_init();
      time_type do_next_position( time_type elapsed_time );
      time_type update_angle( const time_type elapsed_time );
      void end_reached();
      void start_reached();
      void set_item_position();

    private:
      /** \brief Initial angle. */
      double m_start_angle;

      /** \brief End angle. */
      double m_end_angle;

      /** \brief Current angle. */
      double m_angle;

      /** \brief Distance of the moving item from the center of the rotation. */
      double m_radius;

      /** \brief Total time of the movement. */
      time_type m_total_time;

      /** \brief Elapsed time in the movement. */
      time_type m_elapsed_time;

      /** \brief Must we play the rotation backward when the end is reached or
          just come back to the start angle ? */
      bool m_loop_back;

      /** \brief How many times the rotation will be done. */
      unsigned int m_loops;

      /** \brief How many full loops have we done ? */
      unsigned int m_loop_count;

      /** \brief How the angle of the rotation is applied to the item. */
      angle_application m_angle_application;

      /** \brief The speed generator used to move the item. */
      sinus_speed_generator m_speed_generator;

      /** \brief Tells if we are moving the item from the start angle toward the
          end angle. */
      bool m_is_playing_forward;

    }; // class forced_rotation
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_FORCED_ROTATION_HPP__
