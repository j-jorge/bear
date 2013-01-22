/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An interface to the forced movements.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_FORCED_MOVEMENT_HPP__
#define __UNIVERSE_FORCED_MOVEMENT_HPP__

#include "universe/forced_movement/reference_point.hpp"
#include "universe/types.hpp"

#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    class base_forced_movement;
    class physical_item;

    /**
     * \brief An interface to the forced movements.
     *
     * If an item has a forced movement, the physical rules won't apply to him.
     *
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT forced_movement
    {
    public:
      forced_movement();
      forced_movement( const forced_movement& that );
      forced_movement( const base_forced_movement& m );
      ~forced_movement();

      forced_movement& operator=( forced_movement that );

      bool is_null() const;
      void clear();

      void init();

      void clear_item();
      void set_item( physical_item& item );

      bool has_reference_point() const;
      void set_reference_point( const reference_point& r );
      position_type get_reference_position() const;

      void set_reference_point_on_center( physical_item& item );
      bool has_reference_item() const;
      physical_item& get_reference_item() const;

      void set_auto_remove(bool b);
      void set_auto_angle(bool b);

      position_type get_moving_item_ratio() const;
      void set_moving_item_ratio(const position_type & ratio);

      position_type get_moving_item_gap() const;
      void set_moving_item_gap(const position_type & gap);

      time_type next_position( time_type elapsed_time );
      bool is_finished() const;

    private:
      /** \brief The effective movement. */
      base_forced_movement* m_movement;

    }; // class forced_movement
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_FORCED_MOVEMENT_HPP__
