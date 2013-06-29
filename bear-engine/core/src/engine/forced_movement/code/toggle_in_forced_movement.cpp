/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::toggle_in_forced_movement class.
 * \author Julien Jorge
 */
#include "engine/forced_movement/toggle_in_forced_movement.hpp"

#include "universe/physical_item.hpp"
#include "engine/item_brick/with_toggle.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::toggle_in_forced_movement::toggle_in_forced_movement()
  : m_use_toggle_delay(false), m_elapsed_time(0)
{

} // toggle_in_forced_movement::toggle_in_forced_movement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this movement.
 */
bear::universe::base_forced_movement*
bear::engine::toggle_in_forced_movement::clone() const
{
  return new toggle_in_forced_movement(*this);
} // toggle_in_forced_movement::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the movement is finished.
 */
bool bear::engine::toggle_in_forced_movement::is_finished() const
{
  return !m_use_toggle_delay || (get_toggle() == NULL)
    || !get_toggle()->is_on();
} // toggle_in_forced_movement::is_finished()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the duration of the forced movement in the duration of
 *        the toggle.
 * \param b True if the movement use the delay of the toggle.
 */
void bear::engine::toggle_in_forced_movement::use_toggle_delay( bool b )
{
  m_use_toggle_delay = b;
} // toggle_in_forced_movement::use_toggle_delay()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the movement.
 */
void bear::engine::toggle_in_forced_movement::do_init()
{
  m_elapsed_time = 0;

  with_toggle* const t = get_toggle();

  if ( t != NULL )
    t->toggle_on( NULL );
} // toggle_in_forced_movement::do_init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do an iteration of the movement.
 * \param elapsed_time Elapsed time since the last call.
 * \return The remaining time if the movement is finished.
 */
bear::universe::time_type
bear::engine::toggle_in_forced_movement::do_next_position
( universe::time_type elapsed_time )
{
  if ( !m_use_toggle_delay )
    return elapsed_time;

  universe::time_type remaining_time = 0;

  with_toggle* const t = get_toggle();

  if ( t == NULL )
    remaining_time = elapsed_time;
  else if ( t->get_delay() >= elapsed_time )
    remaining_time = 0;
  else
    remaining_time = m_elapsed_time + elapsed_time - t->get_delay();

  m_elapsed_time += elapsed_time - remaining_time;

  return remaining_time;
} // toggle_in_forced_movement::do_next_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the toggle observed by this movement.
 */
bear::engine::with_toggle*
bear::engine::toggle_in_forced_movement::get_toggle() const
{
  if ( !has_reference_item() )
    return NULL;

  return dynamic_cast<with_toggle*>( &get_reference_item() );
} // toggle_in_forced_movement::get_toggle()
