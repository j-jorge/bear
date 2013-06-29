/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A ceiling that is not horizontal.
 * \author Julien Jorge
 */
#ifndef __BEAR_DESCENDING_CEILING_HPP__
#define __BEAR_DESCENDING_CEILING_HPP__

#include "generic_items/base_ground.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A ceiling that is not horizontal.
   *
   * The parameters accepted by this item are:
   *  - \a steepness: \c real The difference beween the Y-coordinate of the
   *    ceiling's right and left edges,
   *  - \a opposite_side_is_active: \c boolean Turn on/off the alignement on the
   *    top side,
   *  - \a left_side_is_active: \c boolean Turn on/off the alignement on the
   *    left side,
   *  - \a right_side_is_active: \c boolean Turn on/off the alignement on the
   *    right side,
   *  - \a apply_angle: \c boolean Tell if the angle of the ceiling is applied
   *    to the colliding items,
   *  - any field supported by the parent class.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT descending_ceiling:
    public base_ground
  {
    DECLARE_BASE_ITEM(descending_ceiling);

  public:
    /** \brief The type of the parent class. */
    typedef base_ground super;

    /** \brief The type of the line describing the surface of the ceiling. */
    typedef claw::math::line_2d<universe::coordinate_type> line_type;

  public:
    descending_ceiling();

    bool set_real_field( const std::string& name, double value );
    bool set_bool_field( const std::string& name, bool value );

    bool is_valid() const;

    void build();

    universe::coordinate_type get_steepness() const;

  protected:
    void collision_as_ceiling
    ( engine::base_item& that, universe::collision_info& info );

  private:
    void collision
    ( engine::base_item& that, universe::collision_info& info );

    bool check_left_contact_as_ceiling
    ( engine::base_item& that, universe::collision_info& info ) const;
    bool check_right_contact_as_ceiling
    ( engine::base_item& that, universe::collision_info& info ) const;
    bool check_top_below_ceiling
    ( engine::base_item& that, universe::collision_info& info ) const;

    bool align_on_ceiling
    ( engine::base_item& that, universe::collision_info& info );
    bool align_nearest_edge
    ( engine::base_item& that, universe::collision_info& info );

    bool item_crossed_down_up
    ( engine::base_item& that, const universe::collision_info& info ) const;

    void apply_angle_to
    ( engine::base_item& that, const universe::collision_info& info ) const;

  private:
    /** \brief The side opposite to the descending_ceiling, is it active ? */
    bool m_opposite_side_is_active;

    /** \brief Tell if the left side of the block is active. */
    bool m_left_side_is_active;

    /** \brief Tell if the right side of the block is active. */
    bool m_right_side_is_active;

    /** \brief Tell if the angle of the ceiling is applied to the colliding
        items. */
    bool m_apply_angle;

    /** \brief The line of the ground. */
    line_type m_line;

  }; // class descending_ceiling
} // namespace bear

#endif // __BEAR_DESCENDING_CEILING_HPP__
