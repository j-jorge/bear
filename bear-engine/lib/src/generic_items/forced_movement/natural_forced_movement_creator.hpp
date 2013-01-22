/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class creates a forced movement of type
 *        "natural_forced_movement", applies it to itself then die.
 * \author Julien Jorge
 */
#ifndef __BEAR_NATURAL_FORCED_MOVEMENT_CREATOR_HPP__
#define __BEAR_NATURAL_FORCED_MOVEMENT_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "universe/forced_movement/natural_forced_movement.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class creates a forced movement of type
   *        "natural_natural_forced_movement", applies it to itself then die.
   *
   * The fields of this item are
   *  - \a duration: (real) \c the total time of the movement (default = inf.).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT natural_forced_movement_creator:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(natural_forced_movement_creator);

  private:
    /**
     * \brief The item loader for the forced path creator.
     */
    class loader:
      public engine::item_loader_base
    {
    private:
      /** \brief The type of the parent class. */
      typedef engine::item_loader_base super;

    public:
      explicit loader( universe::natural_forced_movement& mvt );

      loader* clone() const;

      bool set_field( const std::string& name, double value );

    private:
      /** \brief The movement loaded by this loader. */
      universe::natural_forced_movement& m_movement;

    }; // class loader

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    void build();

  protected:
    void populate_loader_map( engine::item_loader_map& m );

  private:
    /** \brief The movement finally applied. */
    universe::natural_forced_movement m_movement;

  }; // class natural_forced_movement_creator
} // namespace bear

#endif // __BEAR_NATURAL_FORCED_MOVEMENT_CREATOR_HPP__
