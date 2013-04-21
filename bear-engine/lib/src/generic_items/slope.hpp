/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A ground that is not horizontal.
 * \author Julien Jorge
 */
#ifndef __BEAR_SLOPE_HPP__
#define __BEAR_SLOPE_HPP__

#include "generic_items/base_ground.hpp"
#include "generic_items/class_export.hpp"

#include "engine/export.hpp"

#include <claw/curve.hpp>

namespace bear
{
  /**
   * \brief A ground that is not horizontal.
   *
   * The parameters accepted by this item are:
   *  - \a steepness [required]: \c real The difference beween the Y-coordinate
   *    of the slope's right and left edges,
   *  - \a control_point.left.x: \c real The X-coordinate of the left control
   *       point.
   *  - \a control_point.left.y: \c real The Y-coordinate of the left control
   *       point.
   *  - \a control_point.right.x: \c real The X-coordinate of the right control
   *       point.
   *  - \a control_point.right.y: \c real The Y-coordinate of the right control
   *       point.
   *  - \a margin: \c real An extra margin above the slope (default = 0),
   *  - \a tangent_friction: \c real The friction applied to the items on the
   *     slope (default = 0.8),
   *  - \a opposite_side_is_active: \c boolean Turn on/off the alignement on the
   *    side opposite to the slope (default = false),
   *  - \a left_side_is_active: \c boolean Turn on/off the alignement on the
   *    left side (default = false),
   *  - \a right_side_is_active: \c boolean Turn on/off the alignement on the
   *    right side (default = false),
   *  - \a apply_angle: \c boolean Tell if the angle of the slope is applied to
   *    the colliding items (default = true),
   *  - any field supported by the parent class.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT slope:
    public base_ground
  {
    DECLARE_BASE_ITEM(slope);

  public:
    /** \brief The type of the parent class. */
    typedef base_ground super;

    /** \brief The type of the line describing the surface of the slope. */
    typedef claw::math::line_2d<universe::coordinate_type> line_type;

    /** \brief The type of the curve describing the surface of the slope. */
    typedef claw::math::curve<universe::position_type> curve_type;

  public:
    slope();

    bool set_real_field( const std::string& name, double value );
    bool set_bool_field( const std::string& name, bool value );

    bool is_valid() const;

    void build();

    universe::coordinate_type get_steepness() const;
    universe::coordinate_type get_margin() const;

    const curve_type& get_curve() const;

    universe::coordinate_type get_y_at_x( universe::coordinate_type x ) const;

  protected:
    void collision_as_slope
    ( engine::base_item& that, universe::collision_info& info );

  private:
    void collision
    ( engine::base_item& that, universe::collision_info& info );

    bool check_left_contact_as_slope
    ( engine::base_item& that, universe::collision_info& info ) const;
    bool check_right_contact_as_slope
    ( engine::base_item& that, universe::collision_info& info ) const;
    bool check_bottom_above_ground
    ( engine::base_item& that, universe::collision_info& info ) const;

    bool align_on_ground
    ( engine::base_item& that, universe::collision_info& info );
    bool align_nearest_edge
    ( engine::base_item& that, universe::collision_info& info );

    bool item_crossed_up_down
    ( engine::base_item& that, const universe::collision_info& info ) const;

    void apply_angle_to
    ( engine::base_item& that, const universe::collision_info& info ) const;

  private:
    /** \brief The coefficient for tangent friction. */
    double m_tangent_friction;

    /** \brief The side opposite to the slope, is it active ? */
    bool m_opposite_side_is_active;

    /** \brief Tell if the left side of the block is active. */
    bool m_left_side_is_active;

    /** \brief Tell if the right side of the block is active. */
    bool m_right_side_is_active;

    /** \brief Tell if the angle of the slope is applied to the colliding
        items. */
    bool m_apply_angle;

    /** \brief The steepness of the slope. */
    double m_steepness;

    /** \brief The margin of the ground. */
    universe::coordinate_type m_margin;

    /** \brief The vector of left control point. */
    universe::vector_type m_left_control_point;
    
    /** \brief The vector of right control point. */
    universe::vector_type m_right_control_point;
    
    /** \brief The bezier curve on the ground. */
    curve_type m_curve;    

    /** \brief The width of the surface of the slope. */
    static const universe::coordinate_type s_line_width;

  }; // class slope
} // namespace bear

#endif // __BEAR_SLOPE_HPP__
