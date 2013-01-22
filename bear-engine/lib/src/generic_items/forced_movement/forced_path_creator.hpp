/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class creates a forced movement that joins several items in a
 *        sequence.
 * \author Julien Jorge
 */
#ifndef __BEAR_FORCED_PATH_CREATOR_HPP__
#define __BEAR_FORCED_PATH_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "universe/forced_movement/forced_sequence.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class creates a forced movement that joins several items in a
   *        sequence, applies it to itself then die.
   *
   * The fields of this item are
   *  - \a path: (item list) \b [required] \c the items to reach in the path,
   *    must be non empty,
   *  - \a loops: (unsigned int) \c how many times the sequence will be
   *    done (default = no limits = 0),
   *  - \a speed: (real) The speed of the moving item (default = 1).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT forced_path_creator:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(forced_path_creator);

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
      explicit loader( universe::forced_sequence& mvt );

      loader* clone() const;

      bool set_field( const std::string& name, unsigned int value );
      bool set_field( const std::string& name, double value );
      bool set_field
      ( const std::string& name, const std::vector<engine::base_item*>& value );

    private:
      bool build_sequence( const std::vector<engine::base_item*>& items );
      void add_movement_in_sequence
      ( engine::base_item& item, universe::time_type duration );

    private:
      /** \brief The movement loaded by this loader. */
      universe::forced_sequence& m_movement;

      /** \brief The speed of the item between the vertices of the path. */
      double m_speed;

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
    universe::forced_sequence m_movement;

  }; // class forced_path_creator
} // namespace bear

#endif // __BEAR_FORCED_PATH_CREATOR_HPP__
