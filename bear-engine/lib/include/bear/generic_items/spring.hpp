/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A spring gives a force to the items colliding with him.
 * \author Julien Jorge
 */
#ifndef __BEAR_SPRING_HPP__
#define __BEAR_SPRING_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/engine/item_brick/basic_renderable_item.hpp"
#include "bear/engine/item_brick/item_with_decoration.hpp"
#include "bear/engine/item_brick/activable_sides.hpp"

#include "bear/generic_items/class_export.hpp"

#include "bear/engine/export.hpp"

namespace bear
{
  /**
   * \brief A spring gives a force to the items colliding with him.
   *
   * The valid fields for this item are
   *  - \a applied_force_x: (real) the force applied on the x-axis
   *    (default = 0),
   *  - \a applied_force_y: (real) the force applied on the y-axis
   *    (default = 0),
   *  - any field supported by the parent classes.
   *
   * When the spring is not active, the last frame of the animation is
   * displayed.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT spring:
    public engine::item_with_decoration
  < engine::basic_renderable_item<engine::base_item> >,
    public engine::activable_sides
  {
    DECLARE_BASE_ITEM(spring);

  public:
    /** \brief The type of the parent class. */
    typedef
    engine::item_with_decoration
    < engine::basic_renderable_item<engine::base_item> > super;

  public:
    spring();
    ~spring();

    bool set_real_field( const std::string& name, double value );

    void build();

    void set_bounce_sound( audio::sample* s );

  protected:
    void populate_loader_map( engine::item_loader_map& m );

    void collision_check_and_bounce
    ( engine::base_item& that, universe::collision_info& info );

  private:
    void collision
    ( engine::base_item& that, universe::collision_info& info );

    void play_sound() const;

  private:
    /** \brief The force applied to the item. */
    universe::force_type m_applied_force;

    /** \brief The sound played when an item bounces on the spring. */
    audio::sample* m_sample;

  }; // class spring
} // namespace bear

#endif // __BEAR_SPRING_HPP__
