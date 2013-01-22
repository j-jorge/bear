/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::any_input_pressed class.
 * \author Julien Jorge
 */
#include "generic_items/expr/any_input_pressed.hpp"

#include "expr/boolean_variable.hpp"

BASE_ITEM_EXPORT(any_input_pressed, bear)

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::any_input_pressed::any_input_pressed()
: m_pressed(false)
{

} // any_input_pressed::any_input_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progress of the item.
 * \param elapsed_time The elapsed time since the last call.
 */
void bear::any_input_pressed::progress( universe::time_type elapsed_time )
{
  m_pressed = false;
  progress_input_reader(elapsed_time);
} // any_input_pressed::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the expression created by this item.
 */
bear::expr::boolean_expression
bear::any_input_pressed::do_get_expression() const
{
  return expr::boolean_variable( m_pressed );
} // any_input_pressed::do_get_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief A keyboard key has been pressed.
 * \param key The code of the key.
 */
bool bear::any_input_pressed::key_pressed
( const bear::input::key_info& key )
{
  m_pressed = true;
  return false;
} // any_input_pressed::key_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief A joystick button has been pressed.
 * \param button The code of the button.
 * \param joy_index The index of the joystick.
 */
bool bear::any_input_pressed::button_pressed
( input::joystick::joy_code button, unsigned int joy_index )
{
  m_pressed = true;
  return false;
} // any_input_pressed::button_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief A mouse button has been pressed.
 * \param button The code of the button.
 * \param pos The position of the cursor on the screen.
 */
bool bear::any_input_pressed::mouse_pressed
( input::mouse::mouse_code button,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  m_pressed = true;
  return false;
} // any_input_pressed::mouse_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief A keyboard key has been maintained.
 * \param key The code of the key.
 */
bool bear::any_input_pressed::key_maintained
( const bear::input::key_info& key )
{
  m_pressed = true;
  return false;
} // any_input_pressed::key_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief A joystick button has been maintained.
 * \param button The code of the button.
 * \param joy_index The index of the joystick.
 */
bool bear::any_input_pressed::button_maintained
( input::joystick::joy_code button, unsigned int joy_index )
{
  m_pressed = true;
  return false;
} // any_input_pressed::button_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief A mouse button has been maintained.
 * \param button The code of the button.
 * \param pos The position of the cursor on the screen.
 */
bool bear::any_input_pressed::mouse_maintained
( input::mouse::mouse_code button,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  m_pressed = true;
  return false;
} // any_input_pressed::mouse_maintained()
