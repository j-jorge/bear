/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::cursor class.
 * \author Sebastie Angibaud
 */
#include "bear/generic_items/cursor.hpp"

#include "bear/engine/export.hpp"
#include "bear/engine/level.hpp"

BASE_ITEM_EXPORT( cursor, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::cursor::cursor()
: m_screen_position(0, 0),
  m_visibility_duration( std::numeric_limits<universe::time_type>::max() ),
  m_inactive_duration( 0 )
{
  set_artificial(true);
  set_phantom(true);
  set_global(true);
} // cursor::cursor()

/*---------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 */
void bear::cursor::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  progress_input_reader(elapsed_time);
  set_center_of_mass( get_level().screen_to_level( m_screen_position ) );

  m_inactive_duration += elapsed_time;
} // cursor::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::cursor::set_real_field( const std::string& name, double value )
{
  bool ok = true;

  if (name == "cursor.visible_duration")
    {
      m_visibility_duration = value;
      m_inactive_duration = value;
    }
  else
    ok = super::set_real_field(name, value);

  return ok;
} // cursor::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprite representing the item.
 * \param visuals (out) The sprites of the item, and their positions.
 */
void bear::cursor::get_visual( std::list<engine::scene_visual>& visuals ) const
{
  if ( m_inactive_duration < m_visibility_duration )
    super::get_visual( visuals );
} // cursor::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when the position of the mouse changes.
 * \param pos The new position of the cursor.
 * \return true if the event has been processed.
 */
bool bear::cursor::mouse_move
( const claw::math::coordinate_2d<unsigned int>& pos )
{ 
  m_screen_position = pos;
  m_inactive_duration = 0;

  return false;
} // cursor::mouse_move()


