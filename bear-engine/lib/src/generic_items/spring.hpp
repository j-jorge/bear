/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A spring gives a force to the items colliding with him.
 * \author Julien Jorge
 */
#ifndef __BEAR_SPRING_HPP__
#define __BEAR_SPRING_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/basic_renderable_item.hpp"
#include "engine/item_brick/item_with_decoration.hpp"
#include "engine/item_brick/activable_sides.hpp"

#include "engine/export.hpp"

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
  class spring:
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

    bool set_real_field( const std::string& name, double value );

    void build();

  protected:
    void populate_loader_map( engine::item_loader_map& m );

    void collision_check_and_bounce
    ( engine::base_item& that, universe::collision_info& info );

  private:
    void collision
    ( engine::base_item& that, universe::collision_info& info );

  private:
    /** \brief The force applied to the item. */
    universe::force_type m_applied_force;

  }; // class spring
} // namespace bear

#endif // __BEAR_SPRING_HPP__
