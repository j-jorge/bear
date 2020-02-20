/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::trigger class.
 * \author Julien Jorge
 */
#include "bear/generic_items/trigger.hpp"
#include "bear/expr/boolean_constant.hpp"
#include "bear/visual/scene_line.hpp"
#include "bear/visual/scene_polygon.hpp"

BASE_ITEM_EXPORT( trigger, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param m The initial mode of the trigger.
 */
bear::trigger::trigger( mode m )
: m_mode(m)
{
  set_phantom(true);
  set_can_move_items(false);
} // trigger::trigger()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::trigger::set_string_field
( const std::string& name, const std::string& value )
{
  bool ok = true;

  if (name == "trigger.mode")
    {
      if (value == "one_way")
        m_mode = trigger_one_way;
      else if (value == "switch")
        m_mode = trigger_switch;
      else if (value == "condition")
        m_mode = trigger_condition;
      else
        {
          claw::logger << claw::log_error << '\'' << value
                       << "' is not a valid value for '" << name << '\''
                       << std::endl;
          ok = false;
        }
    }
  else
    ok = super::set_string_field(name, value);

  return ok;
} // trigger::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type item_list.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::trigger::set_item_list_field
( const std::string& name, const std::vector<engine::base_item*>& value )
{
  bool ok = true;

  if (name == "trigger.toggle")
    for( std::size_t i=0; i!=value.size(); ++i )
      add_toggle( value[i] );
  else
    ok = super::set_item_list_field(name, value);

  return ok;
} // trigger::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprite representing the item.
 * \param visuals (out) The sprites of the item, and their positions.
 */
void
bear::trigger::get_visual
( std::list<bear::engine::scene_visual>& visuals ) const
{
#ifndef NDEBUG
  if ( ( get_height() == 0 ) &&  ( get_width() == 0 ) )
    {
      std::vector<bear::universe::position_type> points;
      points.push_back(bear::universe::position_type(-5, -5));
      points.push_back(bear::universe::position_type(+5, -5));
      points.push_back(bear::universe::position_type(+5, +5));
      points.push_back(bear::universe::position_type(-5, +5));

      bear::visual::scene_element e
        ( bear::visual::scene_polygon
          (get_left(), get_bottom(), claw::graphic::red_pixel, points ));

      if ( get_condition() )
        e = bear::visual::scene_polygon
          (get_left(), get_bottom(), claw::graphic::green_pixel, points );

      e.get_rendering_attributes().set_opacity(0.6);

      visuals.push_front(e);
    }
#endif // NDEBUG
} // trigger::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Activate the trigger.
 */
void bear::trigger::activate()
{
  set_condition(expr::boolean_expression(expr::boolean_constant(true)));
} // trigger::activate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Deactivate the trigger.
 */
void bear::trigger::deactivate()
{
  set_condition(expr::boolean_expression(expr::boolean_constant(false)));
} // trigger::deactivate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a toggle.
 * \param t The item with the toggle (must inherit from with_toggle).
 */
void bear::trigger::add_toggle( base_item* t )
{
  m_toggle.push_back( handle_type((universe::physical_item*)t) );
} // trigger::add_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the items whose behavior depends upon this item.
 * \param d (out) The itemps depending on this item.
 */
void bear::trigger::get_dependent_items
( universe::physical_item::item_list& d ) const
{
  super::get_dependent_items( d );

  for ( handle_list::const_iterator it=m_toggle.begin(); it!=m_toggle.end();
        ++it )
    if ( *it != (universe::physical_item*)NULL )
      d.push_back( it->get_item() );
} // trigger::get_dependent_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief The condition is verified.
 * \param activator The item that activates the toggle.
 */
void bear::trigger::on_trigger_on( base_item* activator )
{
  switch( m_mode )
    {
    case trigger_one_way:
    case trigger_condition:
      set_toggles(activator, true);
      break;
    case trigger_switch: switch_toggles(activator); break;
    }
} // item_with_trigger::check_and_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief The condition is not verified.
 * \param activator The item that activates the toggle.
 */
void bear::trigger::on_trigger_off( base_item* activator )
{
  switch( m_mode )
    {
    case trigger_one_way:
    case trigger_switch:
      break;
    case trigger_condition:
      set_toggles(activator, false);
      break;
    }
} // trigger::on_trigger_off()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the status of the toggles.
 * \param activator The item that activates the toggle.
 * \param b The value applied to the toggles.
 */
void bear::trigger::set_toggles( base_item* activator, bool b )
{
  handle_list::iterator it;
  std::list<handle_list::iterator> dead;

  for (it=m_toggle.begin(); it!=m_toggle.end(); ++it)
    if ( *it == (engine::with_toggle*)NULL )
      dead.push_back(it);
    else
      (*it)->toggle(b, activator);

  for ( ; !dead.empty(); dead.pop_front() )
    m_toggle.erase(dead.front());
} // trigger::set_toggles()

/*----------------------------------------------------------------------------*/
/**
 * \brief Switch the status of the toggles.
 * \param activator The item that activates the toggle.
 */
void bear::trigger::switch_toggles( base_item* activator )
{
  handle_list::iterator it;
  std::list<handle_list::iterator> dead;

  for (it=m_toggle.begin(); it!=m_toggle.end(); ++it)
    if ( *it == (engine::with_toggle*)NULL )
      dead.push_back(it);
    else
      (*it)->toggle(activator);

  for ( ; !dead.empty(); dead.pop_front() )
    m_toggle.erase(dead.front());
} // trigger::switch_toggles()

/*----------------------------------------------------------------------------*/
/**
 * \brief Export the methods of the class.
 */
void bear::trigger::init_exported_methods()
{
  TEXT_INTERFACE_CONNECT_METHOD_0( trigger, activate, void );
  TEXT_INTERFACE_CONNECT_METHOD_0( trigger, deactivate, void );
} // trigger::init_exported_methods()

/*----------------------------------------------------------------------------*/
TEXT_INTERFACE_IMPLEMENT_METHOD_LIST( bear::trigger )
