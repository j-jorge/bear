/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::gui_layer_stack class.
 * \author Julien Jorge
 */
#include "engine/layer/gui_layer_stack.hpp"

#include "engine/layer/gui_layer.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::gui_layer_stack::~gui_layer_stack()
{
  clear();
} // gui_layer_stack::~gui_layer_stack()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a layer in front of the others.
 * \param the_layer The layer to add (it will be deleted in the destructor).
 * \pre the_layer != NULL
 */
void bear::engine::gui_layer_stack::push_layer( gui_layer* the_layer )
{
  CLAW_PRECOND( the_layer != NULL );

  m_sub_layers.push_back(the_layer);
} // gui_layer_stack::push_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the sub layers.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::engine::gui_layer_stack::progress( universe::time_type elapsed_time )
{
  m_input_status.read();
  m_input_status.scan_inputs( *this );

  for (unsigned int i=0; i!=m_sub_layers.size(); ++i)
    m_sub_layers[i]->progress(elapsed_time);
} // gui_layer_stack::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the visibles components of the sub layers on a screen.
 * \param e The container in which we store the visuals.
 */
void bear::engine::gui_layer_stack::render( scene_element_list& e ) const
{
  for (unsigned int i=0; i!=m_sub_layers.size(); ++i)
    {
      scene_element_list new_e;
      m_sub_layers[i]->render( new_e );
      e.insert( e.end(), new_e.begin(), new_e.end() );
    }
} // gui_layer_stack::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Delete all layers.
 */
void bear::engine::gui_layer_stack::clear()
{
  for (unsigned int i=0; i!=m_sub_layers.size(); ++i)
    delete m_sub_layers[i];

  m_sub_layers.clear();
} // gui_layer_stack::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a key has been pressed.
 * \param key The value of the pressed key.
 * \remark The message is sent to the sub layers until one processes it.
 */
bool bear::engine::gui_layer_stack::key_pressed( const input::key_info& key )
{
  bool result = false;
  unsigned int i = m_sub_layers.size();

  while ( (i != 0) && !result )
    {
      --i;
      result = m_sub_layers[i]->key_pressed(key);
    }

  return result;
} // gui_layer_stack::key_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a key is maintained
 * \param key The value of the pressed key.
 * \remark The message is sent to the sub layers until one processes it.
 */
bool bear::engine::gui_layer_stack::key_maintained( const input::key_info& key )
{
  bool result = false;
  unsigned int i = m_sub_layers.size();

  while ( (i != 0) && !result )
    {
      --i;
      result = m_sub_layers[i]->key_maintained(key);
    }

  return result;
} // gui_layer_stack::key_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a key has been released.
 * \param key The value of the pressed key.
 * \remark The message is sent to the sub layers until one processes it.
 */
bool bear::engine::gui_layer_stack::key_released( const input::key_info& key )
{
  bool result = false;
  unsigned int i = m_sub_layers.size();

  while ( (i != 0) && !result )
    {
      --i;
      result = m_sub_layers[i]->key_released(key);
    }

  return result;
} // gui_layer_stack::key_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a character has been entered.
 * \param key The value of the pressed key.
 * \remark The message is sent to the sub layers until one processes it.
 */
bool bear::engine::gui_layer_stack::char_pressed( const input::key_info& key )
{
  bool result = false;
  unsigned int i = m_sub_layers.size();

  while ( (i != 0) && !result )
    {
      --i;
      result = m_sub_layers[i]->char_pressed(key);
    }

  return result;
} // gui_layer_stack::char_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a joystick button had been pressed.
 * \param button The value of the pressed button.
 * \param joy_index The index of the joystick.
 * \remark The message is sent to the sub layers until one processes it.
 */
bool bear::engine::gui_layer_stack::button_pressed
( input::joystick::joy_code button, unsigned int joy_index )
{
  bool result = false;
  unsigned int i = m_sub_layers.size();

  while ( (i != 0) && !result )
    {
      --i;
      result = m_sub_layers[i]->button_pressed(button, joy_index);
    }

  return result;
} // gui_layer_stack::button_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a joystick button is maintained.
 * \param button The value of the pressed button.
 * \param joy_index The index of the joystick.
 * \remark The message is sent to the sub layers until one processes it.
 */
bool bear::engine::gui_layer_stack::button_maintained
( input::joystick::joy_code button, unsigned int joy_index )
{
  bool result = false;
  unsigned int i = m_sub_layers.size();

  while ( (i != 0) && !result )
    {
      --i;
      result = m_sub_layers[i]->button_maintained(button, joy_index);
    }

  return result;
} // gui_layer_stack::button_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a joystick button has been released.
 * \param button The value of the pressed button.
 * \param joy_index The index of the joystick.
 * \remark The message is sent to the sub layers until one processes it.
 */
bool bear::engine::gui_layer_stack::button_released
( input::joystick::joy_code button, unsigned int joy_index )
{
  bool result = false;
  unsigned int i = m_sub_layers.size();

  while ( (i != 0) && !result )
    {
      --i;
      result = m_sub_layers[i]->button_released(button, joy_index);
    }

  return result;
} // gui_layer_stack::button_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a mouse button has been pressed.
 * \param pos The current position of the cursor.
 * \param key The value of the pressed button.
 * \remark The message is sent to the sub layers until one processes it.
 */
bool bear::engine::gui_layer_stack::mouse_pressed
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  bool result = false;
  unsigned int i = m_sub_layers.size();

  while ( (i != 0) && !result )
    {
      --i;
      result = m_sub_layers[i]->mouse_pressed(key, pos);
    }

  return result;
} // gui_layer_stack::mouse_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a mouse button is maintained.
 * \param pos The current position of the cursor.
 * \param key The value of the maintained button.
 * \remark The message is sent to the sub layers until one processes it.
 */
bool bear::engine::gui_layer_stack::mouse_maintained
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  bool result = false;
  unsigned int i = m_sub_layers.size();

  while ( (i != 0) && !result )
    {
      --i;
      result = m_sub_layers[i]->mouse_maintained(key, pos);
    }

  return result;
} // gui_layer_stack::mouse_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a mouse button has been released.
 * \param pos The current position of the cursor.
 * \param key The value of the released button.
 * \remark The message is sent to the sub layers until one processes it.
 */
bool bear::engine::gui_layer_stack::mouse_released
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  bool result = false;
  unsigned int i = m_sub_layers.size();

  while ( (i != 0) && !result )
    {
      --i;
      result = m_sub_layers[i]->mouse_released(key, pos);
    }

  return result;
} // gui_layer_stack::mouse_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that the mouse has been moved.
 * \param pos The new position of the mouse.
 * \remark The message is sent to the sub layers until one processes it.
 */
bool bear::engine::gui_layer_stack::mouse_move
( const claw::math::coordinate_2d<unsigned int>& pos )
{
  bool result = false;
  unsigned int i = m_sub_layers.size();

  while ( (i != 0) && !result )
    {
      --i;
      result = m_sub_layers[i]->mouse_move(pos);
    }

  return result;
} // gui_layer_stack::mouse_move()
