/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::check_system_name_creator class.
 * \author Julien Jorge
 */
#include "bear/generic_items/expr/check_system_name_creator.hpp"

#include "bear/expr/boolean_constant.hpp"

#include "bear/engine/export.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The instance through which we set the parameters.
 */
bear::check_system_name_creator::loader::loader
( check_system_name_creator& item )
  : super("check_system_name_creator"), m_item(item)
{

} // check_system_name_creator::loader::loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a copy of this instance.
 */
bear::check_system_name_creator::loader*
bear::check_system_name_creator::loader::clone() const
{
  return new loader( *this );
} // check_system_name_creator::loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::check_system_name_creator::loader::set_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "system_name")
    m_item.m_system_name = value;
  else
    result = super::set_field(name, value);

  return result;
} // check_system_name_creator::loader::set_field()





BASE_ITEM_EXPORT( check_system_name_creator, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all required fields are initialized.
 */
bool bear::check_system_name_creator::is_valid() const
{
  return !m_system_name.empty();
} // check_system_name_creator::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::check_system_name_creator::build()
{
  kill();
} // check_system_name_creator::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the expression created by this item.
 */
bear::expr::boolean_expression
bear::check_system_name_creator::do_get_expression() const
{
  std::string system_name;

#ifdef __ANDROID__
  system_name = "android";
#elif defined(_WIN32)
  system_name = "windows";
#else
  system_name = "unix";
#endif

  return expr::boolean_expression
    ( expr::boolean_constant( system_name == m_system_name ) );
} // check_system_name_creator::do_get_expression()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::check_system_name_creator::populate_loader_map
( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( loader( *this ) );
} // check_system_name_creator::populate_loader_map()
