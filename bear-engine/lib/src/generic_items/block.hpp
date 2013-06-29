/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class representing a block.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_BLOCK_HPP__
#define __BEAR_BLOCK_HPP__

#include "generic_items/base_ground.hpp"

#include "engine/item_brick/activable_sides.hpp"
#include "universe/zone.hpp"

#include "generic_items/class_export.hpp"

#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief A class representing an block.
   *
   * The custom fields of this class are:
   * - collision_threshold: unsigned integer, a threshold for the collisions in
   *   the corners of the item, to choose if the other item is aligned on the
   *   collided side or the nearest adjacent one (default = 10).
   *
   * \author Sebastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT block:
    public base_ground,
    public engine::activable_sides
  {
    DECLARE_BASE_ITEM(block);

    typedef base_ground super;

  public:
    block();

    void build();

    bool set_u_integer_field( const std::string& name, unsigned int value );

  protected:
    void populate_loader_map( engine::item_loader_map& m );

    bool collision_check_and_align
    ( engine::base_item& that, universe::collision_info& info );
    void collision
    ( engine::base_item& that, universe::collision_info& info );

  private:
    universe::zone::position choose_alignment_side
    ( const engine::base_item& that,
      const universe::collision_info& info ) const;
    bool align_bottom
      ( engine::base_item& that, universe::collision_info& info );
    bool align_top( engine::base_item& that, universe::collision_info& info );
    bool align_left( engine::base_item& that, universe::collision_info& info );
    bool align_right( engine::base_item& that, universe::collision_info& info );
    bool align_middle
      ( engine::base_item& that, universe::collision_info& info );

    void post_alignment( engine::base_item& that, double f );

  private:
    /** \brief A threshold for the collisions in the corners of the item, to
        choose if the other item is aligned on the collided side or the nearest
        adjacent one. */
    universe::coordinate_type m_collision_threshold;

  }; // class block
} // namespace bear

#endif // __BEAR_BLOCK_HPP__
