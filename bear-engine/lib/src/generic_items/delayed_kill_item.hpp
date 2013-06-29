/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item that kill items after a given duration.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_DELAYED_KILL_ITEM_HPP__
#define __BEAR_DELAYED_KILL_ITEM_HPP__

#include "engine/base_item.hpp"
#include "universe/derived_item_handle.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A item that kills items after a given duration.
   *
   * The valid fields for this item are
   *  - \a duration: (real) duration before kill items (default = 0),
   *  - \a killing_items: \c (list of items) The items to kill.
   *  - any field supported by the parent classes.
   *
   * \author Sebastien Angiabud
   */
  class GENERIC_ITEMS_EXPORT delayed_kill_item:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(delayed_kill_item);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  private:
    /** \brief The type of the handles on the items to kill. */
    typedef universe::derived_item_handle<engine::base_item> handle_type;

  public:
    delayed_kill_item();

    void progress( universe::time_type elapsed_time );

    virtual bool set_item_list_field
      ( const std::string& name, const std::vector<base_item*>& value );
    virtual bool set_real_field( const std::string& name, double value );
    void set_duration(double value);
    void add_item( base_item* item );

  private:
    /** \brief The list of items to kill. */
    std::vector<handle_type> m_items;

    /** \brief The duration before kill items. */
    double m_duration;

    /** \brief The duration until the start. */
    double m_start_duration;

  }; // class delayed_kill_item
} // namespace bear

#endif // __BEAR_DELAYED_KILL_ITEM_HPP__
