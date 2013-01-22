/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::time_scale_on_input_toggle class.
 * \author Julien Jorge
 */
#include "generic_items/time_scale_on_input_toggle.hpp"

#include "engine/expr/get_toggle_status.hpp"
#include "expr/boolean_expression.hpp"

#include "generic_items/expr/any_input_pressed.hpp"
#include "generic_items/time_scale.hpp"
#include "generic_items/trigger.hpp"

BASE_ITEM_EXPORT( time_scale_on_input_toggle, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::time_scale_on_input_toggle::~time_scale_on_input_toggle()
{
  delete m_time_scale;
} // time_scale_on_input_toggle::~time_scale_on_input_toggle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::time_scale_on_input_toggle::build()
{
  super::build();

  time_scale* s = m_time_scale->clone();
  s->set_center_of_mass(get_center_of_mass());

  const std::vector<with_toggle*> linked( get_linked_toggles() );
  for ( std::size_t i=0; i!=linked.size(); ++i )
    s->add_linked_toggle( linked[i] );

  new_item(*s);

  trigger* activator = new trigger( trigger::trigger_condition );
  activator->set_center_of_mass( get_center_of_mass() );
  activator->set_global( is_global() );
  activator->add_toggle(s);
  new_item(*activator);

  any_input_pressed* input_check = new any_input_pressed();
  input_check->set_center_of_mass( get_center_of_mass() );
  input_check->set_global( is_global() );
  new_item(*input_check);

  activator->set_condition
    ( expr::boolean_expression( engine::get_toggle_status(*this) )
      && input_check->get_expression() );
} // time_scale_on_input_toggle::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "item".
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::time_scale_on_input_toggle::set_item_field
( const std::string& name, engine::base_item* value )
{
  bool result(true);

  if ( name == "time_scale_on_input_toggle.time_scale" )
    m_time_scale = dynamic_cast<time_scale*>(value->clone());
  else
    result = super::set_item_field( name, value );

  return result;
} // time_scale_on_input_toggle::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is well initialised.
 */
bool bear::time_scale_on_input_toggle::is_valid() const
{
  return (m_time_scale != NULL) && super::is_valid();
} // time_scale_on_input_toggle::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is well initialised.
 * \param The time_scale item to consider.
 */
void bear::time_scale_on_input_toggle::set_time_scale_item
(const time_scale* time_scale_item)
{
  m_time_scale = dynamic_cast<time_scale*>(time_scale_item->clone());
} // time_scale_on_input_toggle::set_time_scale_item()
