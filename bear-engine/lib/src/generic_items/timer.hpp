/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A simple timer.
 * \author Julien Jorge
 */
#ifndef __BEAR_TIMER_HPP__
#define __BEAR_TIMER_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/item_with_toggle.hpp"

#include "universe/derived_item_handle.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief A simple timer.
   *
   * The custom fields of this class are :
   * - \a countdown (boolean): Indicates if the timer is a countdown
   *   (default = false),
   * - \a loop (boolean): tell if the timer restarts automatically when the
   *   time is over (default = false),
   * _ \a reset_when_reactivated : Indicates if the timer is initialized
   * when the timer is reactivated (default = true),
   * - \a time (real): the initial time, in seconds (default = 0),
   * - \a toggle (item): a toggle to toggle when the countdown is over
   *   (default = none),
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT timer:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(timer);

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_toggle<engine::base_item> super;

  private:
    /** \brief An handle on a toggle. */
    typedef universe::derived_item_handle<engine::with_toggle> toggle_handle;

  public:
    timer();
    timer(bear::universe::time_type initial_time, bool countdown);

    void progress_on( bear::universe::time_type elapsed_time );

    bool set_bool_field( const std::string& name, bool value );
    bool set_real_field( const std::string& name, double value );
    bool set_item_field( const std::string& name, engine::base_item* value );

    universe::time_type get_time() const;
    universe::time_type get_initial_time() const;
    universe::time_type get_elapsed_time() const;
    bool is_countdown() const;
    std::size_t get_loops() const;

    engine::base_item* get_toggle() const;
    void set_toggle( engine::base_item* t );

  private:
    void on_toggle_on( engine::base_item* activator );

  private:
    /** \brief Elapsed time since the creation of the item. */
    universe::time_type m_elapsed_time;

    /** \brief The initial time. */
    universe::time_type m_initial_time;

    /** \brief Indicates if the timer is a countdown. */
    bool m_countdown;

    /** \brief Tell if the timer restarts automatically on a timeout. */
    bool m_loop;

    /** \brief A toggle to be turned on when the timer is over. */
    toggle_handle m_toggle;

    /** \brief How many loops have we done. */
    std::size_t m_loops_count;

    /** \brief Indicates if the timer is initialized
     * when the timer is reactivated. */
    bool m_reset_when_reactivated;
  }; // class timer
} // namespace bear

#endif // __BEAR_TIMER_HPP__
