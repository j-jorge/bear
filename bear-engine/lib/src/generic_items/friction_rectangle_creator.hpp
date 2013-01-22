/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class adds a universe::friction_rectangle in universe::world
 *         then dies.
 * \author Julien Jorge
 */
#ifndef __BEAR_FRICTION_RECTANGLE_CREATOR_HPP__
#define __BEAR_FRICTION_RECTANGLE_CREATOR_HPP__

#include "engine/base_item.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class adds a universe::friction_rectangle in universe::world
   *         then dies.
   *
   * The fields of this item are
   *  - \a friction: (real) \c the friction in the rectangle (default = 1).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT friction_rectangle_creator:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(friction_rectangle_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    friction_rectangle_creator();

    bool set_real_field( const std::string& name, double value );

    void build();

  private:
    /** \brief Friction applied to the items. */
    double m_friction;

  }; // class friction_rectangle_creator
} // namespace bear

#endif // __BEAR_FRICTION_RECTANGLE_CREATOR_HPP__
