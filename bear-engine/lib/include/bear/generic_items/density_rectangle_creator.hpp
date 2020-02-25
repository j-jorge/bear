/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class adds a universe::density_rectangle in universe::world
 *         then dies.
 * \author S�bastien Angibaud
 */
#ifndef __BEAR_DENSITY_RECTANGLE_CREATOR_HPP__
#define __BEAR_DENSITY_RECTANGLE_CREATOR_HPP__

#include "bear/engine/base_item.hpp"

#include "bear/engine/export.hpp"
#include "bear/generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class adds a universe::density_rectangle in universe::world
   *         then dies.
   *
   * The fields of this item are
   *  - \a zone_density: (real) \c the density in the rectangle (default = 0).
   *
   * \author S�bastien angibaud
   */
  class GENERIC_ITEMS_EXPORT density_rectangle_creator:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(density_rectangle_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    density_rectangle_creator();

    bool set_real_field( const std::string& name, double value );

    void build();

  private:
    /** \brief Density applied to the items. */
    double m_density;

  }; // class density_rectangle_creator
} // namespace bear

#endif // __BEAR_DENSITY_RECTANGLE_CREATOR_HPP__
