/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item that add an actor at a script director.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_ADD_SCRIPT_ACTOR_HPP__
#define __BEAR_ADD_SCRIPT_ACTOR_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/universe/derived_item_handle.hpp"
#include "bear/generic_items/script/script_director.hpp"
#include "bear/generic_items/class_export.hpp"

#include "bear/engine/export.hpp"

namespace bear
{
  /**
   * \brief An item that add an actor at a script director.
   * \author Sebastien Angibaud
   *
   * The custom fields of this class are:
   * - \a script_director (reference_item) [required], the script director,
   * - \a actor (reference_item) [required], the new actor,
   * - \a actor_name (string) [required], the name of the actor,
   * - any field supported by the parent class.
   */
  class GENERIC_ITEMS_EXPORT add_script_actor:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(add_script_actor);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  private:
    /** \brief The type of handle on actor. */
    typedef universe::derived_item_handle<engine::base_item> handle_type;

  public:
    add_script_actor();

    void set_actor(script_director& director);
    bool set_item_field( const std::string& name, base_item* value );
    bool set_string_field( const std::string& name, const std::string& value );

    bool is_valid() const;

  private:
    /** \brief The new actor. */
    handle_type m_actor;

    /** \brief The name of the actor. */
    std::string m_actor_name;
  }; // class add_script_actor
} // namespace bear

#endif // __BEAR_ADD_SCRIPT_ACTOR_HPP__
