/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An item that plays a script.
 * \author Julien Jorge
 */
#ifndef __BEAR_SCRIPT_DIRECTOR_HPP__
#define __BEAR_SCRIPT_DIRECTOR_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/item_with_toggle.hpp"
#include "engine/script/script_runner.hpp"

#include "generic_items/time_scale_on_input_toggle.hpp"
#include "generic_items/time_scale.hpp"
#include "generic_items/class_export.hpp"

#include "engine/export.hpp"

namespace bear
{
  class add_script_actor;

  /**
   * \brief An item that plays a script.
   * \author Julien Jorge
   *
   * The custom fields of this class are:
   * - \a script (string) [required], the script to play,
   * - \a scale: real, the time scale when the script is accelerated
   * (default = 2),
   * - \a can_be_accelerated: boolean, indicates if the script can be
   * accelerated (default = false),
   * - any field supported by the parent class.
   */
  class GENERIC_ITEMS_EXPORT script_director:
    public engine::item_with_toggle<engine::base_item>,
    public engine::script_runner
  {
    DECLARE_BASE_ITEM(script_director);

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_toggle<engine::base_item> super;

  private:
    /** \brief The type of the handles on the items to kill. */
    typedef universe::derived_item_handle<add_script_actor> handle_type;

  public:
    script_director();

    bool set_string_field( const std::string& name, const std::string& value );
    bool set_item_list_field
    ( const std::string& name, const std::vector<engine::base_item*>& value );
    bool set_real_field( const std::string& name, double value );
    bool set_bool_field( const std::string& name, bool value );

    void get_dependent_items( std::list<universe::physical_item*>& d ) const;

  protected:
    void on_script_started();
    void on_script_stopped();

  private:
    void on_toggle_on(engine::base_item *activator);
    void progress_on(universe::time_type elapsed_time);
    void create_time_scale_items();
    void remove_time_scale_items();

  private:
    /** \brief The list of items that add an actor. */
    std::vector<handle_type> m_actors;

    /** \brief Toggle that checks input. */
    universe::derived_item_handle<time_scale_on_input_toggle>
    m_time_scale_toggle;

    /** \brief Item that increases time scale. */
    universe::derived_item_handle<time_scale> m_time_scale;

    /** \brief Indicates if time can be accelerated. */
    bool m_can_be_accelerated;

    /** \brief The scale of the time. */
    double m_scale;
  }; // class script_director
} // namespace bear

#endif // __BEAR_SCRIPT_DIRECTOR_HPP__
