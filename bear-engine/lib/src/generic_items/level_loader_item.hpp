/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This item loads a level.
 * \author Julien Jorge
 */
#ifndef __BEAR_LEVEL_LOADER_ITEM_HPP__
#define __BEAR_LEVEL_LOADER_ITEM_HPP__

#include "engine/base_item.hpp"
#include "engine/level_loader.hpp"

#include "generic_items/class_export.hpp"

#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief This item loads a level.
   *
   * The custom fields of this class are :
   * - \a ratio: real, ratio of the game's time window to spend in the loading
   *   of the level (default = 0.5),
   * - \a level: string, required. The name of the game variable containing the
   *   path of the level to load or the name of the level itself.
   *
   * \author Julien Jorge.
   */
  class GENERIC_ITEMS_EXPORT level_loader_item:
    public bear::engine::base_item
  {
    DECLARE_BASE_ITEM(level_loader_item);

    typedef bear::engine::base_item super;

    /** \brief The type of a method to call in the progress() method. */
    typedef void (level_loader_item::*progress_function_type)
      (bear::universe::time_type elapsed_time );

  public:
    level_loader_item();
    level_loader_item( const level_loader_item& that );
    ~level_loader_item();

    void build();
    void progress( bear::universe::time_type elapsed_time );

    bool set_string_field( const std::string& name, const std::string& value );
    bool set_real_field( const std::string& name, double value );

    bool is_valid() const;

    unsigned int get_item_index() const;
    unsigned int get_items_count() const;

    bool level_is_loaded() const;
    void start_level();

  private:
    void progress_preload( bear::universe::time_type elapsed_time );
    void progress_loading();

    void clear_loading_data();

    level_loader_item& operator=( const level_loader_item& that );

  private:
    /** \brief The loader with wich we load the level. */
    engine::level_loader* m_level_loader;

    /** \brief The file given to the level_loader. */
    engine::compiled_file* m_level_file;

    /** \brief The stream passed to the compiled file. */
    std::stringstream* m_level_stream;

    /** \brief The level, once we have loaded it. */
    engine::level* m_level;

    /** \brief The path of the level to load. */
    std::string m_level_path;

    /** \brief The ratio of the game's time window to spend in the loading of
        the level. */
    double m_ratio;

    /** \brief The index of the currently loaded item. */
    unsigned int m_item_index;

    /** \brief The total number of items to load. */
    unsigned int m_items_count;

  }; // class level_loader_item
} // namespace bear

#endif // __BEAR_LEVEL_LOADER_ITEM_HPP__
