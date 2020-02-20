/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The classes related to all grounds and walls.
 * \author Julien Jorge
 */
#ifndef __BEAR_BASE_GROUND_HPP__
#define __BEAR_BASE_GROUND_HPP__

#include "bear/engine/item_brick/basic_renderable_item.hpp"
#include "bear/engine/item_brick/item_with_decoration.hpp"
#include "bear/engine/item_brick/item_with_friction.hpp"
#include "bear/engine/item_brick/item_with_restricted_z_collision.hpp"
#include "bear/engine/item_brick/z_shift.hpp"
#include "bear/universe/contact_mode.hpp"

#include "bear/generic_items/class_export.hpp"

#include "bear/engine/export.hpp"

namespace bear
{
  /**
   * \brief The base class for the grounds, ceiling and walls.
   *
   * The custom fields of this class are:
   * - top_left_contact_mode: string, how the contact is set on the collisions
   *   on the top left corner, among "full_contact", "range_contact"
   *   (default = "no_contact").
   *
   * - top_right_contact_mode: string, how the contact is set on the collisions
   *   on the top right corner, among "full_contact", "range_contact"
   *   (default = "no_contact").
   *
   * - bottom_left_contact_mode: string, how the contact is set on the
   *   collisions on the bottom left corner, among "full_contact",
   *   "range_contact" (default = "no_contact").
   *
   * - bottom_right_contact_mode: string, how the contact is set on the
   *   collisions on the bottom side, among "full_contact", "range_contact"
   *   (default = "no_contact").
   *
   * - top_sets_contact: boolean, tell if a collision on the top side sets the
   *   contact flag of the colliding item (default = true).
   *
   * - bottom_sets_contact: boolean, tell if a collision on the bottom side sets
   *   the contact flag of the colliding item (default = true).
   *
   * - left_sets_contact: boolean, tell if a collision on the left side sets the
   *   contact flag of the colliding item (default = true).
   *
   * - right_sets_contact: boolean, tell if a collision on the right side sets
   *   the contact flag of the colliding item (default = true).
   *
   * - middle_sets_contact: boolean, tell if a collision on the middle sets the
   *   contact flag of the colliding item (default = true).
   *
   *  - any field supported by the parent class.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT base_ground:
    public engine::item_with_friction
    < engine::item_with_restricted_z_collision
      < engine::item_with_decoration
        < engine::basic_renderable_item<engine::base_item>
        >
      >
    >,
    public engine::z_shift
  {
  private:
    typedef engine::item_with_friction
    < engine::item_with_restricted_z_collision
      < engine::item_with_decoration
        < engine::basic_renderable_item<engine::base_item>
        >
      >
    > super;

  public:
    base_ground();

    bool set_bool_field( const std::string& name, bool value );
    bool set_string_field( const std::string& name, const std::string& value );

    void init_default_contact_mode
      ( bool top, bool bottom, bool left, bool right );

    universe::contact_mode::value_type
      get_left_contact_mode( const universe::collision_info& info ) const;
    universe::contact_mode::value_type
      get_left_contact_mode
      ( const universe::collision_info& info,
        const universe::position_type& pos ) const;

    universe::contact_mode::value_type
      get_right_contact_mode( const universe::collision_info& info ) const;
    universe::contact_mode::value_type
      get_right_contact_mode
      ( const universe::collision_info& info,
        const universe::position_type& pos ) const;

    universe::contact_mode::value_type
      get_top_contact_mode( const universe::collision_info& info ) const;
    universe::contact_mode::value_type
      get_top_contact_mode
      ( const universe::collision_info& info,
        const universe::position_type& pos ) const;

    universe::contact_mode::value_type
      get_bottom_contact_mode( const universe::collision_info& info ) const;
    universe::contact_mode::value_type
      get_bottom_contact_mode
      ( const universe::collision_info& info,
        const universe::position_type& pos ) const;

    universe::contact_mode::value_type
      get_middle_contact_mode( const universe::collision_info& info ) const;

    void do_z_shift( engine::base_item& that ) const;

  protected:
    void populate_loader_map( engine::item_loader_map& m );

  private:
    universe::contact_mode::value_type
      compute_horizontal_contact_mode
      ( const universe::collision_info& info,
        const universe::position_type& pos,
        universe::contact_mode::value_type left_mode,
        universe::contact_mode::value_type right_mode ) const;

    universe::contact_mode::value_type
      compute_vertical_contact_mode
      ( const universe::collision_info& info,
        const universe::position_type& pos,
        universe::contact_mode::value_type bottom_mode,
        universe::contact_mode::value_type top_mode ) const;

    void set_default_contact_mode
      ( universe::contact_mode::value_type& c, bool one_side, bool other_side );

  private:
    /** \brief Tell if the top side sets the contact. */
    bool m_top_sets_contact;

    /** \brief Tell if the bottom side sets the contact. */
    bool m_bottom_sets_contact;

    /** \brief Tell if the right side sets the contact. */
    bool m_right_sets_contact;

    /** \brief Tell if the left side sets the contact. */
    bool m_left_sets_contact;

    /** \brief Tell if the middle sets the contact. */
    bool m_middle_sets_contact;

    /** \brief Tell how the contact is set on a collision on the top left
        corner. */
    universe::contact_mode::value_type m_top_left_contact_mode;

    /** \brief Tell how the contact is set on a collision on the top right
        corner. */
    universe::contact_mode::value_type m_top_right_contact_mode;

    /** \brief Tell how the contact is set on a collision on the bottom left
        corner. */
    universe::contact_mode::value_type m_bottom_left_contact_mode;

    /** \brief Tell how the contact is set on a collision on the bottom right
        corner. */
    universe::contact_mode::value_type m_bottom_right_contact_mode;

  }; // class base_ground
} // namespace bear

#endif // __BEAR_BASE_GROUND_HPP__
