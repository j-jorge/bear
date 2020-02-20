/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::script_director class.
 * \author Julien Jorge
 */
#include "bear/generic_items/script/script_director.hpp"
#include "bear/generic_items/script/add_script_actor.hpp"

BASE_ITEM_EXPORT( script_director, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::script_director::script_director()
  : m_time_scale_toggle(NULL), m_time_scale(NULL),
    m_can_be_accelerated(false), m_scale(2)
{
  set_global(true);
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // script_director::script_director()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c <string>.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::script_director::set_string_field
( const std::string& name, const std::string& value )
{
  bool ok = true;

  if (name == "script_director.script")
    {
      claw::logger << claw::log_verbose << "Loading script '" << value << '\''
                   << std::endl;
      ok = load_script(value);
    }
  else
    ok = super::set_string_field(name, value);

  return ok;
} // script_director::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::script_director::set_item_list_field
( const std::string& name, const std::vector<base_item*>& value )
{
  bool ok = true;

  if (name == "script_director.actors")
    {
      m_actors.resize(value.size());

      for ( std::size_t i=0; i!=value.size(); ++i )
        m_actors[i] = value[i];
    }
  else
    ok = super::set_item_list_field(name, value);

  return ok;
} // script_director::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::script_director::set_real_field
( const std::string& name, double value )
{
  bool result(true);

  if ( name == "script_director.scale" )
    m_scale = value;
  else
    result = super::set_real_field( name, value );

  return result;
} // script_director::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "boolean".
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::script_director::set_bool_field
( const std::string& name, bool value )
{
  bool result(true);

  if ( name == "script_director.can_be_accelerated" )
    m_can_be_accelerated = value;
  else
    result = super::set_bool_field( name, value );

  return result;
} // script_director::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the items concerned by a progress/move of this one.
 * \param d (out) A list to which are added such items.
 */
void bear::script_director::get_dependent_items
( universe::physical_item::item_list& d ) const
{
  engine::script_context::actor_item_map_const_iterator_type it;

  for ( it = get_context().get_actors_item_begin();
        it != get_context().get_actors_item_end(); ++it )
    if ( it->second != (base_item*)NULL )
      d.push_back( it->second.get_item() );
} // script_director::get_dependent_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Extra processing by subclasses at the beginning of the script.
 */
void bear::script_director::on_script_started()
{
  script_runner::on_script_started();

  remove_time_scale_items();
  if ( m_can_be_accelerated )
    create_time_scale_items();
} // script_director::on_script_started()

/*----------------------------------------------------------------------------*/
/**
 * \brief Extra processing by subclasses at the end of the script.
 */
void bear::script_director::on_script_stopped()
{
  script_runner::on_script_stopped();

  remove_time_scale_items();
} // script_director::on_script_stopped()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn the item on.
 * \param activator The item that activates this item.
 */
void bear::script_director::on_toggle_on(engine::base_item *activator)
{
  reset();

  for ( std::size_t i=0; i!=m_actors.size(); ++i )
    if ( m_actors[i] != (add_script_actor*)NULL )
      m_actors[i]->set_actor(*this);
} // script_director::on_toggle_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Continue the progression of the item, while active.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::script_director::progress_on(universe::time_type elapsed_time)
{
  play(elapsed_time);
} // script_director::progress_on()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create items to allow time acceleration.
 */
void bear::script_director::create_time_scale_items()
{
  time_scale* item1 = new time_scale;
  item1->set_global(true);
  item1->set_center_of_mass(get_center_of_mass());
  item1->set_scale(m_scale);
  new_item( *item1 );
  CLAW_ASSERT
    ( item1->is_valid(),
      "The time_scale created by bear::script_director isn't correctly "
      "initialized" );
  m_time_scale =
    universe::derived_item_handle< time_scale >(item1);

  time_scale_on_input_toggle* item2 = new time_scale_on_input_toggle;
  item2->set_global(true);
  item2->set_center_of_mass(get_center_of_mass());
  item2->set_time_scale_item(item1);
  new_item( *item2 );
  CLAW_ASSERT
    ( item2->is_valid(),
      "The time_scale_on_input_toggle created by bear::script_director isn't "
      "correctly initialized" );
  m_time_scale_toggle =
    universe::derived_item_handle< time_scale_on_input_toggle >(item2);
  item2->toggle(this);
} // script_director::create_time_scale_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove items that enables time acceleration.
 */
void bear::script_director::remove_time_scale_items()
{
  if ( m_time_scale.get() != NULL )
    m_time_scale->kill();
  m_time_scale = NULL;

  if ( m_time_scale_toggle.get() != NULL )
    m_time_scale_toggle->kill();

  m_time_scale_toggle = NULL;
} // script_director::remove_time_scale_items()
