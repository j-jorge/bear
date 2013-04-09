/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::layer_shader class.
 * \author Julien Jorge
 */
#include "generic_items/shader/layer_shader.hpp"

#include "engine/item_brick/loader/single_tweener_loader.hpp"
#include "engine/level.hpp"
#include "engine/level_globals.hpp"
#include "engine/layer/layer.hpp"

#include "engine/export.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The instance loaded by this loaded.
 */
bear::layer_shader::loader::loader( layer_shader& item )
  : super("layer_shader"), m_item(item)
{

} // layer_shader::loader::loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a copy of this instance.
 */
bear::layer_shader::loader* bear::layer_shader::loader::clone() const
{
  return new loader( *this );
} // layer_shader::loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::layer_shader::loader::set_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "shader" )
    m_item.set_shader_file( value );
  else
    result = super::set_field(name, value);

  return result;
} // layer_shader::loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::layer_shader::loader::set_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "kill_delay" )
    m_item.set_kill_delay( value );
  else
    result = super::set_field(name, value);

  return result;
} // layer_shader::loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c list of item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::layer_shader::loader::set_field
( const std::string& name, const std::vector<engine::base_item*>& value )
{
  bool result = true;

  if ( name == "variables" )
    for ( std::size_t i(0); i != value.size(); ++i )
      {
        const shader_variable* v( dynamic_cast<shader_variable*>( value[i] ) );

        if ( v == NULL )
          claw::logger << claw::log_warning << "Item is not a shader_variable."
                       << std::endl;
        else
          m_item.add_variable( *v );
      }
  else
    result = super::set_field(name, value);

  return result;
} // layer_shader::loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c list of string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::layer_shader::loader::set_field
( const std::string& name, const std::vector<std::string>& value )
{
  bool result = true;

  if ( name == "layer_tags" )
    for ( std::size_t i(0); i != value.size(); ++i )
      m_item.add_layer_tag( value[i] );
  else
    result = super::set_field(name, value);

  return result;
} // layer_shader::loader::set_field()




BASE_ITEM_EXPORT( layer_shader, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::layer_shader::layer_shader()
: m_kill_delay( std::numeric_limits<universe::time_type>::infinity() )
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
  set_global(true);
} // layer_shader::layer_shader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bear::layer_shader::layer_shader( const layer_shader& that )
  : super( that ), m_shader( that.m_shader ), m_kill_delay( that.m_kill_delay ),
    m_layer_tags( that.m_layer_tags )
{
  for ( std::size_t i(0); i != that.m_variables.size(); ++i )
    if ( that.m_variables[i] != (shader_variable_pointer)NULL )
      add_variable( *that.m_variables[i] );
} // layer_shader::layer_shader()

/*---------------------------------------------------------------------------*/
/**
 * \brief Does one iteration in the progression of the item.
 * \param elapsed_time The elapsed time since the last call.
 */
void bear::layer_shader::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  if ( get_age() >= m_kill_delay )
    kill();
  else
    {
      for ( std::size_t i(0); i != m_variables.size(); ++i )
        if ( m_variables[i] != (shader_variable_pointer)NULL )
          m_shader.set_variable
            ( m_variables[i]->get_name(), m_variables[i]->get_value() );

      if ( m_layer_tags.empty() )
        get_layer().set_shader( m_shader );
      else
        for ( engine::level::layer_iterator it( get_level().layer_begin() );
              it != get_level().layer_end(); ++it )
          {
            const bool apply_shader
              ( std::find
                ( m_layer_tags.begin(), m_layer_tags.end(), it->get_tag() )
                != m_layer_tags.end() );

            if ( apply_shader )
              it->set_shader( m_shader );
          }
    }
} // layer_shader::progress()

/*---------------------------------------------------------------------------*/
/**
 * \brief Sets the delay after which the item must kill itself.
 * \param t The delay.
 */
void bear::layer_shader::set_kill_delay( universe::time_type t )
{
  m_kill_delay = t;
} // layer_shader::set_kill_delay()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the shader file to pass to the layer.
 * \param n The path to the shader file.
 */
void bear::layer_shader::set_shader_file( std::string n )
{
  m_shader = get_level_globals().get_shader( n );
} // layer_shader::set_shader_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds an item to compute the values of the variables in the shader.
 * \param v The variable to add.
 */
void bear::layer_shader::add_variable( const shader_variable& v )
{
  m_variables.push_back( v.clone() );
} // layer_shader::add_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the tag of the layers to which the shader must be applied.
 * \param tag The tag to add.
 */
void bear::layer_shader::add_layer_tag( std::string tag )
{
  m_layer_tags.push_back( tag );
} // layer_shader::add_layer_tag()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::layer_shader::populate_loader_map( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( loader( *this ) );
} // layer_shader::populate_loader_map()
