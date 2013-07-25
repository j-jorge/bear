/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class starts a level after a given delay.
 * \author Julien Jorge
 */
#ifndef __BEAR_LEVEL_LOADER_TOGGLE_HPP__
#define __BEAR_LEVEL_LOADER_TOGGLE_HPP__

#include "engine/item_brick/item_with_toggle.hpp"
#include "engine/base_item.hpp"
#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This item starts a level when activated.
   *
   * The custom fields of this class are :
   * - \a fade_duration: real, the duration of the fading effect before starting
   *   the level (default = 1),
   * - \a level: string, required. The name of the game variable containing the
   *   name of the level to load or the path of the level itself,
   * - \a transition_layer_name: string, the name of the transition layer
   *   receiving the fade effect (default = none).
   * - \a push_mode: bool, indicates if the level must be pushed
   * (default = false).
   * - any field supported by the parent class.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT level_loader_toggle:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(level_loader_toggle);

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_toggle<engine::base_item> super;

  public:
    level_loader_toggle();

    bool set_string_field( const std::string& name, const std::string& value );
    bool set_real_field( const std::string& name, double value );
    bool set_bool_field( const std::string& name, bool value );

    bool is_valid() const;

  private:
    void on_toggle_on( engine::base_item* activator );

    std::string get_string_from_vars( const std::string& name ) const;

  private:
    /** \brief The path of the level to load. */
    std::string m_level_path;

    /** \brief The name of the transition layer receiving the fade effect. */
    std::string m_transition_layer_name;

    /** \brief The duration of the fading. */
    universe::time_type m_fade_duration;

    /** \brief Indicates if the level must be pushed. */
    bool m_push_mode;

  }; // class level_loader_toggle
} // namespace bear

#endif // __BEAR_LEVEL_LOADER_TOGGLE_HPP__
