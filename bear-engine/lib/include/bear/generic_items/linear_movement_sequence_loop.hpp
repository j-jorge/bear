/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief  A linear_movement_sequence_loop is an item that creates some items
 * and sets their movement as a linear movement
 * \author Sébastien Angibaud
 */
#ifndef __BEAR_LINEAR_MOVEMENT_SEQUENCE_LOOP_HPP__
#define __BEAR_LINEAR_MOVEMENT_SEQUENCE_LOOP_HPP__

#include "bear/engine/base_item.hpp"

#include "bear/engine/export.hpp"
#include "bear/generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A linear_movement_sequence_loop is an item that creates some items
   * and sets their movement as a linear movement.
   *
   * The custom fields of this class are:
   * - duration (real) The duration of the movement (default = 1)
   * - length.x (real) The length of the movement on x-axis (default = 0)
   * - length.y (real) The length of the movement on y-axis (default = 0)
   * - positions (list of real) The item positions according the duration
   * - item (item_reference) The item that is copied.
   * - \a auto_remove: (bool) Tell if the angle of moving items are
   *   updated according to their angular speed (default = false).
   * - any field supported by the parent class.
   *
   * \author Sébastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT linear_movement_sequence_loop:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(linear_movement_sequence_loop);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    linear_movement_sequence_loop();

    void build();

    bool set_item_field( const std::string& name, base_item* value );
    bool set_real_field( const std::string& name, double value );
    bool set_real_list_field
    ( const std::string& name, const std::vector<double>& value );
    bool set_bool_field( const std::string& name, bool value );
    bool is_valid() const;

  private:
    void create_items();

  private:
    /** \brief The item that is copied. */
    engine::base_item* m_item;

    /** \brief The duration of the linear_movement_sequence_loop. */
    universe::time_type m_duration;

    /** \brief The item position at the beginning. */
    std::vector<double> m_positions;

    /** \brief The length of the movement on x-axis. */
    universe::coordinate_type m_x_length;

    /** \brief The length of the movement on y-axis. */
    universe::coordinate_type m_y_length;

    /** \brief Tell if the angle of the moving item is updated according to
        its angular speed. */
    bool m_auto_angle;
  }; // class linear_movement_sequence_loop
} // namespace bear

#endif // __BEAR_LINEAR_MOVEMENT_SEQUENCE_LOOP_HPP__
