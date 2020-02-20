/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A temporary item containing tweeners on another item.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_TWEENER_ITEM_HPP__
#define __BEAR_TWEENER_ITEM_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/universe/derived_item_handle.hpp"

#include "bear/generic_items/class_export.hpp"
#include "bear/engine/export.hpp"

#include <claw/tween/tweener.hpp>

namespace bear
{
  /**
   * \brief A temporary item containing tweeners on another item.
   * \author Sebastien Angibaud
   *
   * This item has no custom fields.
   *
   */
  class GENERIC_ITEMS_EXPORT tweener_item:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(tweener_item);

  private:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

    /** \brief The type of the item handle. */
    typedef universe::derived_item_handle< engine::base_item > item_handle;

  public:
    tweener_item
    ( const claw::tween::tweener& t, engine::base_item* item = NULL,
      bool same_lifespan = false );

    void on_enters_layer();
    void progress( universe::time_type elapsed_time );

    void set_tweener
    ( const claw::tween::tweener& t, engine::base_item* item = NULL,
      bool same_lifespan = false );

  private:
    /** \brief The item to modify. */
    item_handle m_item;

    /** \brief Tell if m_item must be killed with *this. */
    bool m_same_lifespan;

    /** \brief Tell if *this must be killed when m_item dies. */
    bool m_follow_item;

    /** \brief The set of current tweeners. */
    claw::tween::tweener m_tweener;

  }; // class tweener_item
} // namespace bear

#endif // __BEAR_TWEENER_ITEM_HPP__
