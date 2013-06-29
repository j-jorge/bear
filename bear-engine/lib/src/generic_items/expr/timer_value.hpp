/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A linear expression that returns the value of a timer.
 * \author Julien Jorge
 */
#ifndef __BEAR_TIMER_VALUE_HPP__
#define __BEAR_TIMER_VALUE_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/with_linear_expression_creation.hpp"

#include "universe/derived_item_handle.hpp"

#include "generic_items/class_export.hpp"
#include "engine/export.hpp"

namespace bear
{
  class timer;

  /**
   * \brief A linear expression that returns the value of a timer.
   *
   * The valid fields for this item are
   *  - initial (bool), tell if the evaluation is the initial time or the
   *    current time (default = false),
   *  - timer (timer_kill_players) [required], the timer whose value is taken.
   *  - any field supported by the parent classes.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT timer_value:
    public engine::base_item,
    public engine::with_linear_expression_creation
  {
    DECLARE_BASE_ITEM(timer_value);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  private:
    /** \brief The type of an handle on the timer. */
    typedef universe::const_derived_item_handle<timer> handle_type;

  public:
    timer_value();

    bool is_valid() const;

    bool set_bool_field( const std::string& name, bool value );
    bool set_item_field( const std::string& name, engine::base_item* value );

  private:
    virtual expr::linear_expression do_get_expression() const;

  private:
    /** \brief Tell if the evaluation returns the initial value of the timer or
        its current value. */
    bool m_initial;

    /** \brief The timer in which the value is taken. */
    handle_type m_timer;

  }; // class timer_value
} // namespace bear

#endif // __BEAR_TIMER_VALUE_HPP__
