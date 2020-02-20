/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item to set the parameters of the current level.
 * \author Julien Jorge
 */
#ifndef __BEAR_LEVEL_SETTINGS_HPP__
#define __BEAR_LEVEL_SETTINGS_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/engine/export.hpp"

#include "bear/generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief An item to set the parameters of the current level.
   *
   * The valid fields for this item are
   *  - \c full_volume_distance (real) The distance under which sounds are at
   *    maximum volume (default = 200),
   *  - \c silence_distance (real) The distance from which we can't hear a sound
   *    (default = 1200),
   *  - any field supported by the parent classes.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT level_settings:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(level_settings);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  private:
    /**
     * \brief The item loader for the level settings.
     */
    class loader:
      public engine::item_loader_base
    {
    private:
      /** \brief The type of the parent class. */
      typedef engine::item_loader_base super;

    public:
      explicit loader( engine::level_globals& settings );

      loader* clone() const;

      bool set_field( const std::string& name, double value );

    private:
      /** \brief The instance through which we set the parameters. */
      engine::level_globals& m_settings;

    }; // class loader

  public:
    void build();

  protected:
    void populate_loader_map( engine::item_loader_map& m );

  }; // class level_settings
} // namespace bear

#endif // __BEAR_LEVEL_SETTINGS_HPP__
