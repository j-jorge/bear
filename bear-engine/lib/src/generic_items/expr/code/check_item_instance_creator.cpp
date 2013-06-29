/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::check_item_instance_creator class.
 * \author Julien Jorge
 */
#include "generic_items/expr/check_item_instance_creator.hpp"

#include "expr/boolean_constant.hpp"
#include "engine/expr/check_item_instance.hpp"
#include "engine/item_brick/with_trigger.hpp"

#include "engine/export.hpp"

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param creator The instance through which we set the parameters.
 */
bear::check_item_instance_creator::loader::loader
( check_item_instance_creator& creator )
  : super("check_item_instance_creator"), m_creator(creator)
{

} // check_item_instance_creator::loader::loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a copy of this instance.
 */
bear::check_item_instance_creator::loader*
bear::check_item_instance_creator::loader::clone() const
{
  return new loader( *this );
} // check_item_instance_creator::loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::check_item_instance_creator::loader::set_field
( const std::string& name, engine::base_item* value )
{
  bool result = true;

  if ( name == "collision_data")
    {
      engine::with_trigger* t( dynamic_cast<engine::with_trigger*>(value) );

      if ( t != NULL )
        m_creator.m_collision_data = t;
      else
        claw::logger << claw::log_error << name
                     << ": the item is not of type 'engine::with_trigger'."
                     << std::endl;
    }
  else
    result = super::set_field(name, value);

  return result;
} // check_item_instance_creator::loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c list of item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::check_item_instance_creator::loader::set_field
( const std::string& name, const std::vector<engine::base_item*>& value )
{
  bool result = true;

  if ( name == "instance")
    m_creator.m_instance = value;
  else
    result = super::set_field(name, value);

  return result;
} // check_item_instance_creator::loader::set_field()





BASE_ITEM_EXPORT( check_item_instance_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all required fields are initialized.
 */
bool bear::check_item_instance_creator::is_valid() const
{
  return !m_instance.empty() && (m_collision_data != NULL)
    && super::is_valid();
} // check_item_instance_creator::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::check_item_instance_creator::build()
{
  kill();
} // check_item_instance_creator::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the expression created by this item.
 */
bear::expr::boolean_expression
bear::check_item_instance_creator::do_get_expression() const
{
  expr::boolean_expression result( expr::boolean_constant(false) );

  for ( std::size_t i=0; i!=m_instance.size(); ++i )
    {
      engine::check_item_instance e;
      e.set_instance( m_instance[i] );
      e.set_collision_data( m_collision_data->get_collision_in_expression() );
      result = result || e;
    }

  return result;
} // check_item_instance_creator::do_get_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::check_item_instance_creator::populate_loader_map
( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( loader( *this ) );
} // check_item_instance_creator::populate_loader_map()
