/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::with_toggle class.
 * \author Julien Jorge
 */
#include "bear/engine/item_brick/with_toggle.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Change the state of the toggle.
 * \param activator The item that activates the toggle, if any.
 */
void bear::engine::with_toggle::toggle( base_item* activator )
{
  toggle( !is_on(), activator );
} // with_toggle::toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Change the state of the toggle.
 * \param b The new state.
 * \param activator The item that activates the toggle, if any.
 */
void bear::engine::with_toggle::toggle( bool b, base_item* activator )
{
  if ( b )
    {
      if ( !is_on() )
        toggle_on( activator );
    }
  else if ( is_on() )
    toggle_off( activator );
} // with_toggle::toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the delay before the toggle goes automatically off after being
 *        turned on.
 */
bear::universe::time_type bear::engine::with_toggle::get_delay() const
{
  return 0;
} // get_delay()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn the toggle on.
 * \param activator The item that activates the toggle, if any.
 */
void bear::engine::with_toggle::toggle_on( base_item* activator )
{
  // nothing to do
} // with_toggle::toggle_on()

/*----------------------------------------------------------------------------*/
/*
 * \brief Turn the toggle on.
 * \param activator The item that activates the toggle, if any.
 */
void bear::engine::with_toggle::toggle_off( base_item* activator )
{
  // nothing to do
} // with_toggle::toggle_off()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the toggle is turned on.
 */
bool bear::engine::with_toggle::is_on() const
{
  return false;
} // with_toggle::is_on()
