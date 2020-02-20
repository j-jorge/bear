/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::gui_layer class.
 * \author Julien Jorge
 */
#include "bear/engine/layer/gui_layer.hpp"

#include "bear/engine/game.hpp"

#include "bear/input/finger_event.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::gui_layer::gui_layer()
  : m_size( game::get_instance().get_screen_size() ), m_root(NULL)
{

} // gui_layer::gui_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the media required by this class.
 */
void bear::engine::gui_layer::pre_cache()
{
  // nothing to do
} // gui_layer::pre_cache()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the layer.
 */
void bear::engine::gui_layer::build()
{
  // nothing to do
} // gui_layer::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the layer.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::engine::gui_layer::progress( universe::time_type elapsed_time )
{
  // nothing to do
} // gui_layer::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the scene elements of the layer.
 * \param e (out) The scene elements.
 */
void bear::engine::gui_layer::render( scene_element_list& e ) const
{
  if ( m_root != NULL )
    m_root->render( e );
} // gui_layer::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the layer.
 */
const claw::math::coordinate_2d<unsigned int>&
bear::engine::gui_layer::get_size() const
{
  return m_size;
} // gui_layer::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Converts screen coordinates to layer coordinates.
 */
claw::math::coordinate_2d<unsigned int>
bear::engine::gui_layer::adjust_screen_position
( const claw::math::coordinate_2d<unsigned int>& pos ) const
{
  return claw::math::coordinate_2d<unsigned int>
    ( get_size().x
      * pos.x
      / game::get_instance().get_window_size().x,
      get_size().y
      * pos.y
      / game::get_instance().get_window_size().y );
} // gui_layer::adjust_screen_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the component rendered by default and to which the inputs are
 *        sent.
 */
void bear::engine::gui_layer::set_root_component( gui::visual_component* c )
{
  m_root = c;
} // gui_layer::set_root_component()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a key has been pressed.
 * \param key The value of the pressed key.
 */
bool bear::engine::gui_layer::key_pressed( const bear::input::key_info& key )
{
  if ( m_root != NULL )
    return m_root->key_pressed(key);
  else
    return false;
} // gui_layer::key_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a character has been entered.
 * \param key The value of the pressed key.
 */
bool bear::engine::gui_layer::char_pressed( const bear::input::key_info& key )
{
  if ( m_root != NULL )
    return m_root->char_pressed(key);
  else
    return false;
} // gui_layer::char_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a joystick button had been pressed.
 * \param button The value of the pressed button.
 * \param joy_index The index of the joystick.
 */
bool bear::engine::gui_layer::button_pressed
( bear::input::joystick::joy_code button, unsigned int joy_index )
{
  if ( m_root != NULL )
    return m_root->button_pressed(button, joy_index);
  else
    return false;
} // gui_layer::button_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a mouse button has been pressed.
 * \param pos The current position of the cursor.
 * \param key The value of the pressed button.
 */
bool bear::engine::gui_layer::mouse_pressed
( bear::input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  if ( m_root != NULL )
    return
      m_root->mouse_pressed
      ( key, adjust_screen_position(pos) - m_root->get_position() );
  else
    return false;
} // gui_layer::mouse_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a mouse button has been released.
 * \param pos The current position of the cursor.
 * \param key The value of the pressed button.
 */
bool bear::engine::gui_layer::mouse_released
( bear::input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  if ( m_root != NULL )
    return
      m_root->mouse_released
      ( key, adjust_screen_position(pos) - m_root->get_position() );
  else
    return false;
} // gui_layer::mouse_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a mouse button has been maintained.
 * \param pos The current position of the cursor.
 * \param key The value of the pressed button.
 */
bool bear::engine::gui_layer::mouse_maintained
( bear::input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  if ( m_root != NULL )
    return
      m_root->mouse_maintained
      ( key, adjust_screen_position(pos) - m_root->get_position() );
  else
    return false;
} // gui_layer::mouse_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that the mouse has been moved.
 * \param pos The new position of the mouse.
 */
bool bear::engine::gui_layer::mouse_move
( const claw::math::coordinate_2d<unsigned int>& pos )
{
  if ( m_root != NULL )
    return
      m_root->mouse_move
      ( adjust_screen_position(pos) - m_root->get_position() );
  else
    return false;
} // gui_layer::mouse_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Process an event triggered by a finger.
 * \param event The event to process.
 */
bool bear::engine::gui_layer::finger_action
( const bear::input::finger_event& event )
{
  if ( m_root != NULL )
    return m_root->finger_action
      ( event.at_position
        ( adjust_screen_position(event.get_position())
          - m_root->get_position() ) );
  else
    return false;
} // gui_layer::finger_action()
