/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::level_loader_item class.
 * \author Julien Jorge
 */
#include "generic_items/level_loader_item.hpp"

#include "engine/compiled_file.hpp"
#include "engine/game.hpp"
#include "engine/level.hpp"
#include "engine/resource_pool.hpp"
#include "engine/variable/variable.hpp"

#include "engine/export.hpp"

#include <limits>

BASE_ITEM_EXPORT( level_loader_item, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::level_loader_item::level_loader_item()
  : m_level_loader(NULL), m_level_file(NULL), m_level_stream(NULL),
    m_level(NULL), m_ratio(0.5)
{

} // level_loader_item::level_loader_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bear::level_loader_item::level_loader_item( const level_loader_item& that )
  : super(that), m_level_loader(NULL), m_level_file(NULL), m_level_stream(NULL),
    m_level(NULL), m_ratio(that.m_ratio)
{

} // level_loader_item::level_loader_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::level_loader_item::~level_loader_item()
{
  delete m_level_loader;
  delete m_level_file;
  delete m_level_stream;
  delete m_level;
} // level_loader_item::~level_loader_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item.
 */
void bear::level_loader_item::build()
{
  m_level_stream = new std::stringstream();

  engine::resource_pool::get_instance().get_file
    (m_level_path, *m_level_stream);

  m_level_file = new engine::compiled_file( *m_level_stream, true );
  m_level_loader =
    new engine::level_loader
    ( *m_level_file, m_level_path, NULL, &get_level_globals() );

  m_items_count = m_level_loader->get_items_count();
} // level_loader_item::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the item.
 * \param elapsed_time The elapsed time since the last call.
 */
void bear::level_loader_item::progress( bear::universe::time_type elapsed_time )
{
  super::progress(elapsed_time);
  progress_preload(elapsed_time);
} // level_loader_item::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type string.
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::level_loader_item::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if ( name == "level_loader_item.level" )
    {
      m_level_path = value;

      engine::variable<std::string> var(value);

      if ( engine::game::get_instance().game_variable_exists(var) )
        {
          engine::game::get_instance().get_game_variable(var);

          if ( !var.get_value().empty() )
            m_level_path = var.get_value();
        }
    }
  else
    result = super::set_string_field( name, value );

  return result;
} // level_loader_item::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::level_loader_item::set_real_field( const std::string& name, double value )
{
  bool result(true);

  if ( name == "level_loader_item.ratio" )
    m_ratio = value;
  else
    result = super::set_real_field( name, value );

  return result;
} // level_loader_item::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is well initialised.
 */
bool bear::level_loader_item::is_valid() const
{
  return engine::resource_pool::get_instance().exists(m_level_path)
    && super::is_valid();
} // level_loader_item::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the index of the currently loaded item.
 */
unsigned int bear::level_loader_item::get_item_index() const
{
  return m_item_index;
} // level_loader_item::get_item_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the total number of items to load.
 */
unsigned int bear::level_loader_item::get_items_count() const
{
  return m_items_count;
} // level_loader_item::get_items_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the level has been completely loaded.
 */
bool bear::level_loader_item::level_is_loaded() const
{
  return m_level != NULL;
} // level_loader_item::level_is_loaded()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start the level, if it is loaded, even if the delay is not elapsed.
 */
void bear::level_loader_item::start_level()
{
  if ( m_level != NULL )
    {
      engine::game::get_instance().set_waiting_level( m_level );
      m_level = NULL;
    }
} // level_loader_item::start_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Continue to preload the level.
 * \param elapsed_time The elapsed time since the last call.
 */
void bear::level_loader_item::progress_preload
( bear::universe::time_type elapsed_time )
{
  if ( m_level_loader != NULL )
    progress_loading();
  else
    start_level();
} // level_loader_item::progress_preload()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the level loading.
 */
void bear::level_loader_item::progress_loading()
{
  CLAW_PRECOND( m_level_loader != NULL );
  CLAW_PRECOND( m_level_file != NULL );
  CLAW_PRECOND( m_level_stream != NULL );
  CLAW_PRECOND( m_level == NULL );

  bool stop = false;
  systime::milliseconds_type start_time, current_time;

  start_time = systime::get_date_ms();
  const systime::milliseconds_type max_t =
    engine::game::get_instance().get_time_step() * m_ratio;

  do
    {
      stop = m_level_loader->one_step();
      current_time = systime::get_date_ms();
    }
  while ( !stop && (current_time - start_time < max_t) );

  m_item_index = m_level_loader->get_item_index();

  if (stop)
    clear_loading_data();
} // level_loader_item::progress_loading()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the loaded level and delete the data used to load it.
 */
void bear::level_loader_item::clear_loading_data()
{
  CLAW_PRECOND( m_level == NULL );

  m_level = m_level_loader->drop_level();

  delete m_level_loader;
  m_level_loader = NULL;

  delete m_level_file;
  m_level_file = NULL;

  delete m_level_stream;
  m_level_stream = NULL;
} // level_loader_item::clear_loading_data()
