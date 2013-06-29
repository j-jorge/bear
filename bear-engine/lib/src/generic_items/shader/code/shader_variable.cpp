/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::shader_variable class.
 * \author Julien Jorge
 */
#include "generic_items/shader/shader_variable.hpp"

#include "engine/item_brick/loader/single_tweener_loader.hpp"
#include "engine/export.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The instance loaded by this loaded.
 */
bear::shader_variable::loader::loader( shader_variable& item )
  : super("shader_variable"), m_item(item)
{

} // shader_variable::loader::loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a copy of this instance.
 */
bear::shader_variable::loader* bear::shader_variable::loader::clone() const
{
  return new loader( *this );
} // shader_variable::loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::shader_variable::loader::set_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "name" )
    m_item.set_name( value );
  else
    result = super::set_field(name, value);

  return result;
} // shader_variable::loader::set_field()




BASE_ITEM_EXPORT( shader_variable, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::shader_variable::shader_variable()
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // shader_variable::shader_variable()

/*---------------------------------------------------------------------------*/
/**
 * \brief Does one iteration in the progression of the item.
 * \param elapsed_time The elapsed time since the last call.
 */
void bear::shader_variable::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  m_tweener.update( elapsed_time );
} // shader_variable::progress()

/*---------------------------------------------------------------------------*/
/**
 * \brief Gets the name of the variable.
 */
std::string bear::shader_variable::get_name() const
{
  return m_name;
} // shader_variable::get_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the name of this variable.
 * \param n The new name.
 */
void bear::shader_variable::set_name( std::string n )
{
  m_name = n;
} // shader_variable::set_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the tweener used to compute the value of this variable.
 * \param t The tweener.
 */
void bear::shader_variable::set_tweener( claw::tween::single_tweener t )
{
  m_tweener = t;
} // shader_variable::set_tweener()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the current value of the variable.
 */
double bear::shader_variable::get_value() const
{
  return m_tweener.get_value();
} // shader_variable::get_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::shader_variable::populate_loader_map( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert
    ( engine::single_tweener_loader( "shader_variable.tweener", m_tweener ) );
  m.insert( loader( *this ) );
} // shader_variable::populate_loader_map()
