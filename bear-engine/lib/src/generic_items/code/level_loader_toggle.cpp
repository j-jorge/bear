/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::level_loader_toggle class.
 * \author Julien Jorge
 */
#include "generic_items/level_loader_toggle.hpp"

#include "generic_items/delayed_level_loading.hpp"

#include "engine/game.hpp"
#include "engine/resource_pool.hpp"
#include "engine/variable/variable.hpp"

BASE_ITEM_EXPORT( level_loader_toggle, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::level_loader_toggle::level_loader_toggle()
  : m_fade_duration(1)
{

} // level_loader_toggle::level_loader_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type string.
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::level_loader_toggle::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "level_loader_toggle.level" )
    m_level_path = get_string_from_vars(value);
  else if ( name == "level_loader_toggle.transition_layer_name" )
    m_transition_layer_name = get_string_from_vars(value);
  else
    result = super::set_string_field( name, value );

  return result;
} // level_loader_toggle::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::level_loader_toggle::set_real_field
( const std::string& name, double value )
{
  bool result(true);

  if ( name == "level_loader_toggle.fade_duration" )
    m_fade_duration = value;
  else
    result = super::set_real_field( name, value );

  return result;
} // level_loader_toggle::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is well initialised.
 */
bool bear::level_loader_toggle::is_valid() const
{
  return engine::resource_pool::get_instance().exists(m_level_path)
    && super::is_valid();
} // level_loader_toggle::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play the sample.
 * \param activator (ignored) The item that activates the toggle, if any.
 */
void bear::level_loader_toggle::on_toggle_on( engine::base_item* activator )
{
  delayed_level_loading* loader =
    new delayed_level_loading
    (m_level_path, 0, false, m_fade_duration, m_transition_layer_name);

  loader->set_center_of_mass(get_center_of_mass());
  loader->set_global(is_global());

  new_item( *loader );
} // level_loader_toggle::on_toggle_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a string from the game variable.
 * \param name The name of the game variable.
 * \return \a name if there is such a variable.
 */
std::string bear::level_loader_toggle::get_string_from_vars
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
} // level_loader_toggle::get_string_from_vars()
