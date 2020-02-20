/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The base class for all forced movements.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_BASE_FORCED_MOVEMENT_HPP__
#define __UNIVERSE_BASE_FORCED_MOVEMENT_HPP__

#include "bear/universe/item_handle.hpp"
#include "bear/universe/forced_movement/reference_point.hpp"

#include "bear/universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    class physical_item;

    /**
     * \brief The base class for all forced movements.
     *
     * If an item have a forced movement, the physic rules won't apply to him.
     *
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT base_forced_movement
    {
    public:
      base_forced_movement();
      virtual ~base_forced_movement();

      virtual base_forced_movement* clone() const = 0;

      void init();

      void clear_item();
      void set_item( physical_item& item );

      bool has_reference_point() const;
      void set_reference_point( const reference_point& r );
      position_type get_reference_position() const;

      void set_reference_point_on_center( physical_item& item );
      void set_ratio_reference_point
      ( physical_item& item, const position_type& ratio,
        const position_type& gap );
      bool has_reference_item() const;
      physical_item& get_reference_item() const;

      void set_auto_remove(bool b);
      void set_auto_angle(bool b);

      position_type get_moving_item_ratio() const;
      void set_moving_item_ratio(const position_type & ratio);

      position_type get_moving_item_gap() const;
      void set_moving_item_gap(const position_type & gap);

      time_type next_position( time_type elapsed_time );
      virtual bool is_finished() const = 0;

    protected:

      physical_item& get_item();
      const physical_item& get_item() const;
      const position_type get_moving_item_position() const;
      void set_moving_item_position(const position_type& pos);
      void set_moving_item_system_angle( double angle );
      double get_moving_item_system_angle() const;

    private:
      void update_angle( time_type elapsed_time ) const;
      void adjust_cinetic
        ( const position_type& initial_position, double initial_angle,
          time_type dt ) const;

      virtual void do_init() = 0;
      virtual time_type do_next_position( time_type elapsed_time ) = 0;

    private:
      /** \brief The item currently moving. */
      item_handle m_moving_item;

      /** \brief An computation of the reference point. */
      reference_point m_reference_point;

      /** \brief Tell if the movement removes himself from the moving item when
          finished. */
      bool m_auto_remove;

      /** \brief Tell if the angle of the moving item is updated according to
          its angular speed. */
      bool m_auto_angle;

      /* \brief The ratio on size applied to compute moving item's position. */
      universe::position_type m_moving_item_ratio;

       /* \brief The gap applied to compute moving item's position. */
      universe::position_type m_moving_item_gap;

    }; // class base_forced_movement
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_BASE_FORCED_MOVEMENT_HPP__
