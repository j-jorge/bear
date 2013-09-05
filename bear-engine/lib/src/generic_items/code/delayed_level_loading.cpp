/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::delayed_level_loading class.
 * \author Julien Jorge
 */
#include "generic_items/delayed_level_loading.hpp"

#include "engine/game.hpp"
#include "engine/level_globals.hpp"
#include "engine/resource_pool.hpp"
#include "engine/message/transition_effect_message.hpp"
#include "engine/message/transition_effect_erase_message.hpp"
#include "engine/transition_effect/fade_effect.hpp"
#include "engine/variable/variable.hpp"

#include <limits>

BASE_ITEM_EXPORT( delayed_level_loading, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::delayed_level_loading::delayed_level_loading()
  : m_time(0), m_delay(std::numeric_limits<universe::time_type>::infinity()),
    m_fade_duration(1), m_loading(false), m_load_on_input(false),
  m_push_mode(false), m_effect_id(engine::transition_layer::not_an_id)
{

} // delayed_level_loading::delayed_level_loading()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param level_name The name of the game variable containing the name of the
 *        level to load or the path of the level itself.
 * \param delay number of seconds of delay.
 * \param load_on_input Tell if the level must be loaded if the user press a key
 *        before the end of the delay.
 * \param fade_duration The duration of the fading effect before starting the
 *        level
 * \param transition_layer_name The name of the transition layer
 *        receiving the fade effect
 */
bear::delayed_level_loading::delayed_level_loading
( const std::string& level_name, universe::time_type delay,
  bool load_on_input, universe::time_type fade_duration,
  const std::string& transition_layer_name )
  : m_time(0), m_delay(delay), m_fade_duration(fade_duration), m_loading(false),
    m_load_on_input(load_on_input), m_push_mode(false),
    m_effect_id(engine::transition_layer::not_an_id)
{
  m_level_path = get_string_from_vars(level_name);
  m_transition_layer_name = get_string_from_vars(transition_layer_name);
} // delayed_level_loading::delayed_level_loading()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destroy the item.
 */
void bear::delayed_level_loading::destroy()
{
  engine::transition_effect_erase_message msg(m_effect_id);
  get_level_globals().send_message( m_transition_layer_name, msg );
} // delayed_level_loading::destroy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type string.
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::delayed_level_loading::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "delayed_level_loading.level" )
    m_level_path = get_string_from_vars(value);
  else if ( name == "delayed_level_loading.transition_layer_name" )
    m_transition_layer_name = get_string_from_vars(value);
  else
    result = super::set_string_field( name, value );

  return result;
} // delayed_level_loading::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::delayed_level_loading::set_real_field
( const std::string& name, double value )
{
  bool result(true);

  if ( name == "delayed_level_loading.fade_duration" )
    m_fade_duration = value;
  else if ( name == "delayed_level_loading.delay" )
    m_delay = value;
  else
    result = super::set_real_field( name, value );

  return result;
} // delayed_level_loading::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::delayed_level_loading::set_bool_field
( const std::string& name, bool value )
{
  bool result(true);

  if ( name == "delayed_level_loading.load_on_input" )
    m_load_on_input = value;
  else if ( name == "delayed_level_loading.push_mode" )
    m_push_mode = value;
  else
    result = super::set_bool_field( name, value );

  return result;
} // delayed_level_loading::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is well initialised.
 */
bool bear::delayed_level_loading::is_valid() const
{
  return engine::resource_pool::get_instance().exists(m_level_path)
    && super::is_valid();
} // delayed_level_loading::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::delayed_level_loading::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);
  progress_input_reader(elapsed_time);

  const universe::time_type fade_date(m_delay - m_fade_duration);

  if ( (m_time + elapsed_time >= fade_date) && !m_loading )
    start_fading();

  m_time += elapsed_time;

  if ( (m_time >= m_delay) && !m_level_path.empty() )
    {
      if ( ! m_push_mode ) 
        engine::game::get_instance().set_waiting_level( m_level_path );
      else
        bear::engine::game::get_instance().push_level( m_level_path );
      
      m_level_path.clear();
    }
} // delayed_level_loading::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that the level must be pushed.
 * \param push_mode The new mode.
 */
void bear::delayed_level_loading::set_push_mode(bool push_mode)
{
  m_push_mode = push_mode;
} // delayed_level_loading::set_push_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start fading before loading the level.
 */
void bear::delayed_level_loading::start_fading()
{
  if ( !m_loading )
    {
      get_level().stop_music( m_fade_duration );

      if ( (m_fade_duration > 0) && !m_transition_layer_name.empty() )
        {
          engine::transition_effect_message<engine::fade_effect> msg;
          msg.get_effect().set_duration(m_fade_duration, 1, 0);
          msg.get_effect().set_color( 0, 0, 0 );
          get_level_globals().send_message( m_transition_layer_name, msg );

          m_effect_id = msg.get_id();

          // start the level at the end of the fading
          m_delay = m_time + m_fade_duration;
        }
      else
        m_delay = m_time;
    }

  m_loading = true;
} // delayed_level_loading::start_fading()

/*----------------------------------------------------------------------------*/
/**
 * \brief A keyboard key has been pressed.
 * \param key The code of the key.
 */
bool bear::delayed_level_loading::key_pressed
( const bear::input::key_info& key )
{
  if ( m_load_on_input )
    {
      start_fading();
      return true;
    }
  else
    return false;
} // delayed_level_loading::key_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief A joystick button has been pressed.
 * \param button The code of the button.
 * \param joy_index The index of the joystick.
 */
bool bear::delayed_level_loading::button_pressed
( input::joystick::joy_code button, unsigned int joy_index )
{
  if ( m_load_on_input )
    {
      start_fading();
      return true;
    }
  else
    return false;
} // delayed_level_loading::button_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief A mouse button has been pressed.
 * \param button The code of the button.
 * \param pos The position of the cursor on the screen.
 */
bool bear::delayed_level_loading::mouse_pressed
( input::mouse::mouse_code button,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  if ( m_load_on_input )
    {
      start_fading();
      return true;
    }
  else
    return false;
} // delayed_level_loading::mouse_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief A finger has been used.
 * \param event The event dispatched by the finger.
 */
bool bear::delayed_level_loading::finger_action
( const input::finger_event& event )
{
  if ( m_load_on_input )
    {
      start_fading();
      return true;
    }
  else
    return false;
} // delayed_level_loading::finger_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a string from the game variable.
 * \param name The name of the game variable.
 * \return \a name if there is such a variable.
 */
std::string bear::delayed_level_loading::get_string_from_vars
( const std::string& name ) const
{
  // The parameter is the default result.
  std::string result(name);

  engine::variable<std::string> var(name, name);

  if ( engine::game::get_instance().game_variable_exists(var) )
    {
      engine::game::get_instance().get_game_variable(var);

      if ( !var.get_value().empty() )
        result = var.get_value();
    }

  return result;
} // delayed_level_loading::get_string_from_vars()
