/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::script_context class.
 * \author Julien Jorge
 */
#include "bear/engine/script/script_context.hpp"

#include "bear/engine/base_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Set an actor of the script that inherits of base_item.
 * \param name The name of the actor.
 * \param item The actor.
 * \pre \a item inherit from bear::engine::with_text_interface
 */
void bear::engine::script_context::set_actor_item
( const std::string& name, base_item* item )
{
  handle_type h(item);

  CLAW_PRECOND(h != (text_interface::base_exportable*)NULL);

  m_actor_item[name] = h;
} // script_context::set_actor_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an actor from the script, that inherits of base_item.
 * \param name The name of the actor.
 * \return NULL if there is no actor with this name.
 */
bear::engine::base_item*
bear::engine::script_context::get_actor_item( const std::string& name ) const
{
  const actor_item_map_type::const_iterator it(m_actor_item.find(name));

  if (it == m_actor_item.end())
    return NULL;
  else
    return it->second.get_item();
} // script_context::get_actor_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set an actor of the script.
 * \param name The name of the actor.
 * \param item The actor.
 * \pre \a item inherit from text_interface::base_exportable
 */
void bear::engine::script_context::set_actor
( const std::string& name, text_interface::base_exportable* item )
{
  m_actor[name] = item;
} // script_context::set_actor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an actor from the script.
 * \param name The name of the actor.
 * \return NULL if there is no actor with this name.
 */
bear::text_interface::base_exportable*
bear::engine::script_context::get_actor( const std::string& name ) const
{
  const actor_map_type::const_iterator it(m_actor.find(name));

  if (it == m_actor.end())
    {
      const actor_item_map_type::const_iterator it2(m_actor_item.find(name));

      if (it2 == m_actor_item.end())
        return NULL;
      else
        return it2->second.get();
    }
  else
    return it->second;
} // script_context::get_actor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the iterator on the beginning of the actor map.
 */
bear::engine::script_context::actor_item_map_iterator_type
bear::engine::script_context::get_actors_item_begin()
{
  return m_actor_item.begin();
} // script_context::get_actors_item_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the iterator on the end of the actor map.
 */
bear::engine::script_context::actor_item_map_iterator_type
bear::engine::script_context::get_actors_item_end()
{
  return m_actor_item.end();
} // script_context::get_actors_item_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the iterator on the beginning of the actor map.
 */
bear::engine::script_context::actor_map_iterator_type
bear::engine::script_context::get_actors_begin()
{
  return m_actor.begin();
} // script_context::get_actors_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the iterator on the end of the actor map.
 */
bear::engine::script_context::actor_map_iterator_type
bear::engine::script_context::get_actors_end()
{
  return m_actor.end();
} // script_context::get_actors_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the const iterator on the beginning of the actor map.
 */
bear::engine::script_context::actor_item_map_const_iterator_type
bear::engine::script_context::get_actors_item_begin() const
{
  return m_actor_item.begin();
} // script_context::get_actors_item_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the const iterator on the end of the actor map.
 */
bear::engine::script_context::actor_item_map_const_iterator_type
bear::engine::script_context::get_actors_item_end() const
{
  return m_actor_item.end();
} // script_context::get_actors_item_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the const iterator on the beginning of the actor map.
 */
bear::engine::script_context::actor_map_const_iterator_type
bear::engine::script_context::get_actors_begin() const
{
  return m_actor.begin();
} // script_context::get_actors_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the const iterator on the end of the actor map.
 */
bear::engine::script_context::actor_map_const_iterator_type
bear::engine::script_context::get_actors_end() const
{
  return m_actor.end();
} // script_context::get_actors_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert an argument into the adequate type.
 * \param arg The argument to convert.
 * \param type The description of the expected type for the result value.
 */
bear::text_interface::converted_argument
bear::engine::script_context::do_convert_argument
( const std::string& arg, const std::type_info& type ) const
{
  base_item* item = get_actor_item(arg);

  if ( item != NULL )
    return item;

  text_interface::base_exportable* actor = get_actor(arg);

  if ( actor != NULL )
    return actor;

  throw text_interface::no_converter(arg, type);
} // script_context::do_convert_argument()
