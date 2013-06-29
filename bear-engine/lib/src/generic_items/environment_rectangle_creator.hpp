/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class adds a universe::environment_rectangle in universe::world
 *         then dies.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_ENVIRONMENT_RECTANGLE_CREATOR_HPP__
#define __BEAR_ENVIRONMENT_RECTANGLE_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "universe/environment_type.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class adds a universe::environment_rectangle in
   * universe::world then dies.
   *
   * The fields of this item are
   *  - \a environment: (string)
   * \c the environment in the rectangle: (default = "air_environment").
   * \author Sebastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT environment_rectangle_creator:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(environment_rectangle_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    environment_rectangle_creator();

    bool set_string_field( const std::string& name, const std::string& value );

    void build();

  private:
    /** \brief Environment applied to the items. */
    bear::universe::environment_type m_environment;

  }; // class environment_rectangle_creator
} // namespace bear

#endif // __BEAR_ENVIRONMENT_RECTANGLE_CREATOR_HPP__
