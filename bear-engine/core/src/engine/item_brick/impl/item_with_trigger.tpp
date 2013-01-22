/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::item_with_trigger class.
 * \author Julien Jorge
 */

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<class Base>
bear::engine::item_with_trigger<Base>::item_with_trigger()
  : m_check_on_progress(true), m_check_on_collision(false)
{
  this->set_phantom(true);
} // item_with_trigger::item_with_trigger()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::item_with_trigger<Base>::set_bool_field
( const std::string& name, bool value )
{
  bool ok = true;

  if (name == "item_with_trigger.check_on_collision")
    m_check_on_collision = value;
  else if (name == "item_with_trigger.check_on_progress")
    m_check_on_progress = value;
  else
    ok = super::set_bool_field(name, value);

  return ok;
} // item_with_trigger::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
template<class Base>
void bear::engine::item_with_trigger<Base>::progress
( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);
  progress_trigger(elapsed_time);
} // item_with_trigger::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check the condition and modify the toggle if true.
 * \param activator The item that activates the toggle.
 */
template<class Base>
void
bear::engine::item_with_trigger<Base>::check_and_toggle( base_item* activator )
{
  check_condition(activator);
} // item_with_trigger::check_and_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
template<class Base>
void bear::engine::item_with_trigger<Base>::progress_trigger
( universe::time_type elapsed_time )
{
  if ( m_check_on_progress )
    check_and_toggle(this);
} // item_with_trigger::progress_trigger()

/*----------------------------------------------------------------------------*/
/**
 * \brief A collision occured with an other item.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
template<class Base>
void bear::engine::item_with_trigger<Base>::collision_trigger
( base_item& that, universe::collision_info& info )
{
  if ( m_check_on_collision )
    {
      set_collision_data(that, info);
      check_condition(&that);
      clear_collision_data();
    }
} // item_with_trigger::collision_trigger()

/*----------------------------------------------------------------------------*/
/**
 * \brief A collision occured with an other item.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
template<class Base>
void bear::engine::item_with_trigger<Base>::collision
( base_item& that, universe::collision_info& info )
{
  collision_trigger(that, info);
} // item_with_trigger::collision()
