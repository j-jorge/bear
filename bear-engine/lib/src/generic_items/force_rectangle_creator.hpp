/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class adds a universe::force_rectangle in universe::world
 *         then dies.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_FORCE_RECTANGLE_CREATOR_HPP__
#define __BEAR_FORCE_RECTANGLE_CREATOR_HPP__

#include "engine/base_item.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class adds a universe::force_rectangle in universe::world
   *         then dies.
   *
   * The fields of this item are
   *  - \a force_x: (double)
   * \c the force in x axis in the rectangle (default = 0).
   *  - \a force_y: (double)
   * \c the force in y axis in the rectangle (default = 0).
   *
   * \author Sebastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT force_rectangle_creator:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(force_rectangle_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    force_rectangle_creator();

    bool set_real_field( const std::string& name, double value );

    void build();

  private:
    /** \brief Force applied to the items. */
    universe::force_type m_force;

  }; // class force_rectangle_creator
} // namespace bear

#endif // __BEAR_FORCE_RECTANGLE_CREATOR_HPP__
