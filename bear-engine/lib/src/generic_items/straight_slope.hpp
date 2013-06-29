/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A piece of ground intended to be used next some slopes.
 * \author Julien Jorge
 */
#ifndef __BEAR_STRAIGHT_SLOPE_HPP__
#define __BEAR_STRAIGHT_SLOPE_HPP__

#include "generic_items/base_ground.hpp"

#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A piece of ground intended to be used next some slopes.
   *
   * This items works like a slope. That is, the other item is aligned only if
   * the X-coordinate of his center of mass is in the range covered by the
   * current item.
   *
   * The parameters accepted by this item are:
   *  - \a margin: \c real The tolerance between the position of the other
   *    item and the current item for alignments. Positive value means that the
   *    margin is inside the block (default = 10),
   *  - \a friction: real, the friction applied to the items in contact
   *    (default = 1),
   *  - \a top_side_is_solid: bool, tell if the top side is solid
   *    (default = true),
   *  - \a bottom_side_is_solid: bool, tell if the bottom side is solid
   *    (default = true).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT straight_slope :
    public base_ground
  {
    DECLARE_BASE_ITEM(straight_slope);

  public:
    /** \brief The type of the parent class. */
    typedef base_ground super;

  private:
    /** \brief The result of a collision. */
    struct collision_result
    {
      collision_result();
      explicit collision_result( bool a, double f = 1 );

      /** \brief Tell if the other item has been aligned. */
      bool align;

      /** \brief The friction of the side in contact with the item. */
      double friction;

    }; // struct collision_result

  public:
    straight_slope();

    bool set_bool_field( const std::string& name, bool value );
    bool set_real_field( const std::string& name, double value );

    void build();

  protected:
    void collision_check_center_included
    ( engine::base_item& that, universe::collision_info& info );

  private:
    collision_result collision_classic_ground
    ( engine::base_item& that, universe::collision_info& info );
    collision_result collision_classic_ceiling
    ( engine::base_item& that, universe::collision_info& info );
    collision_result collision_slope_ground
    ( engine::base_item& that, universe::collision_info& info );
    collision_result collision_slope_ceiling
    ( engine::base_item& that, universe::collision_info& info );

    void collision
    ( engine::base_item& that, universe::collision_info& info );

  private:
    /** \brief The tolerance of the distance between the position of the other
        item and the edge of the collision, to apply the alignment. */
    universe::coordinate_type m_margin;

    /** \brief Tell if the top side is active. */
    bool m_top_side_is_active;

    /** \brief Tell if the bottom side is active. */
    bool m_bottom_side_is_active;

  }; // class straight_slope
} // namespace bear

#endif // __BEAR_STRAIGHT_SLOPE_HPP__
