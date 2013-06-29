/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::level_popper_toggle class.
 * \author Sebastien Angibaud
 */
#include "generic_items/level_popper_toggle.hpp"

#include "generic_items/delayed_level_loading.hpp"

#include "engine/game.hpp"
#include "engine/level_globals.hpp"
#include "engine/resource_pool.hpp"
#include "engine/message/transition_effect_message.hpp"
#include "engine/transition_effect/fade_effect.hpp"
#include "engine/variable/variable.hpp"

BASE_ITEM_EXPORT( level_popper_toggle, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::level_popper_toggle::level_popper_toggle()
: m_fade_duration(1), m_applied(false), m_time(0), m_start_fade_out(false)
{

} // level_popper_toggle::level_popper_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 */
void bear::level_popper_toggle::progress
( bear::universe::time_type elapsed_time )
{
  if ( m_start_fade_out )
    m_time += elapsed_time;

  if ( ! m_applied && m_time >= m_fade_duration )
    {
      m_applied = true;
      bear::engine::game::get_instance().pop_level();
    }
} // level_popper_toggle::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type string.
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::level_popper_toggle::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "level_popper_toggle.transition_layer_name" )
    m_transition_layer_name = get_string_from_vars(value);
  else
    result = super::set_string_field( name, value );

  return result;
} // level_popper_toggle::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::level_popper_toggle::set_real_field
( const std::string& name, double value )
{
  bool result(true);

  if ( name == "level_popper_toggle.fade_duration" )
    m_fade_duration = value;
  else
    result = super::set_real_field( name, value );

  return result;
} // level_popper_toggle::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play the sample.
 * \param activator (ignored) The item that activates the toggle, if any.
 */
void bear::level_popper_toggle::on_toggle_on( engine::base_item* activator )
{
  if ( ! m_start_fade_out )
    {
      m_start_fade_out = true;
      start_fading();
    }
} // level_popper_toggle::on_toggle_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start fading before loading the level.
 */
void bear::level_popper_toggle::start_fading()
{
  get_level().stop_music( m_fade_duration );

  if ( (m_fade_duration > 0) && !m_transition_layer_name.empty() )
    {
      engine::transition_effect_message<engine::fade_effect> msg;
      msg.get_effect().set_duration(m_fade_duration, 1, 0);
      msg.get_effect().set_color( 0, 0, 0 );
      get_level_globals().send_message( m_transition_layer_name, msg );
    }    
} // level_popper_toggle::start_fading()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a string from the game variable.
 * \param name The name of the game variable.
 * \return \a name if there is such a variable.
 */
std::string bear::level_popper_toggle::get_string_from_vars
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
} // level_popper_toggle::get_string_from_vars()
