/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class pops the level after a given delay.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_LEVEL_POPPER_TOGGLE_HPP__
#define __BEAR_LEVEL_POPPER_TOGGLE_HPP__

#include "engine/item_brick/item_with_toggle.hpp"
#include "engine/base_item.hpp"
#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This item pops the level when activated after a given delay.
   *
   * The custom fields of this class are :
   * - \a fade_duration: real, the duration of the fading effect before starting
   *   the level (default = 1),
   * - \a transition_layer_name: string, the name of the transition layer
   *   receiving the fade effect (default = none).
   * - any field supported by the parent class.
   *
   * \author Sebastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT level_popper_toggle:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(level_popper_toggle);

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_toggle<engine::base_item> super;

  public:
    level_popper_toggle();

    void progress( bear::universe::time_type elapsed_time );
    bool set_string_field( const std::string& name, const std::string& value );
    bool set_real_field( const std::string& name, double value );

  private:
    void on_toggle_on( engine::base_item* activator );
    void start_fading();
    std::string get_string_from_vars( const std::string& name ) const;

  private:
    /** \brief The name of the transition layer receiving the fade effect. */
    std::string m_transition_layer_name;

    /** \brief The duration of the fading. */
    universe::time_type m_fade_duration;

    /** \brief Tell if the level has been popped. */
    bool m_applied;

    /** \brief The time elapsed since the creation of the item. */
    bear::universe::time_type m_time;

    /** \brief Tell if the level has been start the fade out. */
    bool m_start_fade_out;
  }; // class level_popper_toggle
} // namespace bear

#endif // __BEAR_LEVEL_POPPER_TOGGLE_HPP__
