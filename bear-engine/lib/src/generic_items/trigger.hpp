/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A minimal trigger.
 * \author Julien Jorge
 */
#ifndef __BEAR_TRIGGER_HPP__
#define __BEAR_TRIGGER_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/item_with_trigger.hpp"
#include "engine/item_brick/with_toggle.hpp"

#include "generic_items/class_export.hpp"
#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief A minimal trigger.
   * \author Julien Jorge
   *
   * The custom fields of this class are the fields of the parent classes.
   */
  class GENERIC_ITEMS_EXPORT trigger:
    public engine::item_with_trigger<engine::base_item>
  {
    DECLARE_BASE_ITEM(trigger);

    TEXT_INTERFACE_DECLARE_METHOD_LIST(super, init_exported_methods)

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_trigger<engine::base_item> super;

    /** \brief The different behaviours of the trigger. */
    enum mode
      {
        /** \brief The toggles are turned on if the condition is true, not
            changed otherwise. */
        trigger_one_way,

        /** \brief The state of the toggles change each time the condition is
            true. */
        trigger_switch,

        /** \brief The state of the toggles matches the value of the
            condition. */
        trigger_condition

      }; // enum mode

  private:
    /** \brief The type of an handle on a toggle. */
    typedef universe::derived_item_handle<engine::with_toggle> handle_type;

    /** \brief The type of a list of toggles. */
    typedef std::list<handle_type> handle_list;

  public:
    explicit trigger( mode m = trigger_one_way );

    bool set_string_field( const std::string& name, const std::string& value );
    bool set_item_list_field
      ( const std::string& name, const std::vector<engine::base_item*>& value );

    void get_visual( std::list<bear::engine::scene_visual>& visuals ) const;

    void activate();
    void deactivate();

    void add_toggle( base_item* t );

    virtual void get_dependent_items
      ( std::list<universe::physical_item*>& d ) const;

  private:
    void on_trigger_on( base_item* activator );
    void on_trigger_off( base_item* activator );

    void set_toggles( base_item* activator, bool b );
    void switch_toggles( base_item* activator );

    static void init_exported_methods();

  private:
    /** \brief The behaviour of the trigger. */
    mode m_mode;

    /** \brief The toggles to turn on when the condition is verified. */
    handle_list m_toggle;

  }; // class trigger
} // namespace bear

#endif // __BEAR_TRIGGER_HPP__
