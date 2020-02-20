/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::level_loader class.
 * \author Julien Jorge
 */
#include "bear/engine/level_loader.hpp"

#include "bear/easing.hpp"
#include "bear/level_code_value.hpp"

#include "bear/engine/base_item.hpp"
#include "bear/engine/compiled_file.hpp"
#include "bear/engine/export_function.hpp"
#include "bear/engine/game.hpp"
#include "bear/engine/item_factory.hpp"
#include "bear/engine/level.hpp"
#include "bear/engine/level_globals.hpp"
#include "bear/engine/libraries_pool.hpp"
#include "bear/engine/sprite_loader.hpp"
#include "bear/engine/i18n/translator.hpp"
#include "bear/engine/layer/layer_creator.hpp"
#include "bear/engine/layer/layer_factory.hpp"
#include "bear/engine/loader/item_loader_map.hpp"

#include <claw/exception.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The file from which we load the level.
 * \param path The path of the level to load.
 * \param shared_resources A level_globals from which we can take the resources
 *        instead of building new ones in the new level.
 * \param resource_source A level_globals from which we can temporarily take
 *        the resources instead of building new ones in the new level.
 */
bear::engine::level_loader::level_loader
( compiled_file& f, const std::string& path,
  const level_globals* shared_resources,
  const level_globals* resource_source )
  : m_level(NULL), m_layer(NULL), m_file(f), m_current_item(NULL),
    m_current_loader(NULL), m_items_count(0), m_item_index(0),
    m_maj(0), m_min(0), m_rel(0)
{
  if ( !(m_file >> m_maj >> m_min >> m_rel) )
    throw claw::exception( "Can't read the version of the level file." );

  if ( (m_maj != 0) || (m_min <= 4) )
    throw claw::exception( "This version of the level file is not supported." );

  unsigned int layers_count;
  std::string level_music;
  universe::size_box_type level_size;
  std::string name("Anonymous");

  if ( (m_maj == 0) && (m_min >= 5) )
    m_file >> name;

  m_file >> level_size.x >> level_size.y >> level_music >> m_items_count
         >> layers_count >> m_next_code;

  m_level =
    new level
    ( name, path, level_size, level_music, shared_resources, resource_source );
} // level_loader::level_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::level_loader::~level_loader()
{
  delete m_level;
  delete m_current_item;
  delete m_current_loader;
} // level_loader::~level_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the index of the currently loaded item.
 */
unsigned int bear::engine::level_loader::get_item_index() const
{
  return m_item_index;
} // level_loader::get_item_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the total number of items in the currently loaded layer.
 */
unsigned int bear::engine::level_loader::get_items_count() const
{
  return m_items_count;
} // level_loader::get_items_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Extract the level.
 * \pre m_level != NULL.
 */
bear::engine::level* bear::engine::level_loader::drop_level()
{
  CLAW_PRECOND( m_level != NULL );

  bear::engine::level* result( m_level );
  m_level = NULL;

  return result;
} // level_loader::drop_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the whole level at once.
 */
void bear::engine::level_loader::complete_run()
{
  bool stop = false;

  do
    {
      stop = one_step();
    }
  while( !stop );
} // level_loader::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the next thing to load.
 * \return true if the level is completely loaded.
 */
bool bear::engine::level_loader::one_step()
{
  bool result = true;

  if ( m_current_item != NULL )
    result = one_step_item();

  if ( result ) // no item or completed item
    result = one_step_level();

  return result;
} // level_loader::one_step()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the next thing to load in the item (fields).
 * \return true if the item is completely loaded.
 */
bool bear::engine::level_loader::one_step_item()
{
  bool result = false;

  switch( m_next_code )
    {
    case level_code_value::field_list      : load_item_field_list(); break;
    case level_code_value::field_int       : load_item_field_int(); break;
    case level_code_value::field_u_int     : load_item_field_u_int(); break;
    case level_code_value::field_real      : load_item_field_real(); break;
    case level_code_value::field_bool      : load_item_field_bool(); break;
    case level_code_value::field_string    : load_item_field_string(); break;
    case level_code_value::field_sprite    : load_item_field_sprite(); break;
    case level_code_value::field_animation : load_item_field_animation(); break;
    case level_code_value::field_item      : load_item_field_item(); break;
    case level_code_value::field_sample    : load_item_field_sample(); break;
    case level_code_value::field_font      : load_item_field_font(); break;
    case level_code_value::field_color     : load_item_field_color(); break;
    case level_code_value::field_easing    : load_item_field_easing(); break;
    default:
      {
        validate_current_item();
        result = true;
      }
    }

  return result;
} // level_loader::one_step_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the next thing to load in the level.
 * \return true if the level is completely loaded.
 */
bool bear::engine::level_loader::one_step_level()
{
  bool result = false;

  switch( m_next_code )
    {
    case level_code_value::layer            : load_layer(); break;
    case level_code_value::item_declaration : load_item_declaration(); break;
    case level_code_value::item_definition  : load_item_definition(); break;
    case level_code_value::base_item        : load_item(); break;
    case level_code_value::eof: result = true;
    }

  return result;
} // level_loader::one_step_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the action layer.
 */
void bear::engine::level_loader::load_layer()
{
  claw::math::coordinate_2d<unsigned int> size;
  std::string class_name;
  std::string tag;

  m_file >> class_name >> size.x >> size.y;

  if ( (m_maj == 0) && (m_min >= 9) )
    m_file >> tag;

  m_file >> m_next_code;
  m_referenced_index = 0;
  m_referenced.clear();

  m_layer = create_layer_from_string( class_name, size );
  if ( (m_maj == 0) && (m_min >= 9) )
    m_layer->set_tag(tag);
  m_level->push_layer( m_layer );
} // level_loader::load_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Validate the current item and add it in the level.
 */
void bear::engine::level_loader::validate_current_item()
{
  assert( m_current_item != NULL );

  if ( !m_current_item->is_valid() )
    throw claw::exception
      ( std::string("Invalid item: ") + m_current_item->get_class_name() );

  m_layer->add_item( *m_current_item );

  m_current_item = NULL;

  delete m_current_loader;
  m_current_loader = NULL;
} // level_loader::validate_current_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate an item inheriting from bear::engine::base_item.
 */
void bear::engine::level_loader::load_item_declaration()
{
  CLAW_PRECOND( m_current_item == NULL );
  CLAW_PRECOND( m_current_loader == NULL );

  unsigned int n;
  std::string class_name;

  m_file >> n;
  m_referenced.resize(n);

  claw::logger << claw::log_verbose << "Preparing " << n << " items."
               << std::endl;

  for (unsigned int i=0; i!=n; ++i)
    {
      m_file >> class_name;
      m_referenced[i] = create_item_from_string(class_name);
    }

  m_file >> m_next_code;
} // level_loader::load_item_declaration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the fields of a referenced item.
 */
void bear::engine::level_loader::load_item_definition()
{
  CLAW_PRECOND( m_current_item == NULL );
  CLAW_PRECOND( m_current_loader == NULL );

  claw::logger << claw::log_verbose << "Defining item #" << m_referenced_index
               << ' ' << m_referenced[m_referenced_index]->get_class_name()
               << std::endl;

  bool fixed;

  m_current_item = m_referenced[m_referenced_index];

  m_current_loader = new item_loader_map( m_current_item->get_loaders() );

  m_file >> fixed >> m_next_code;
  ++m_referenced_index;

  if (fixed)
    m_current_item->set_insert_as_static();
} // level_loader::load_item_definition()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an item inheriting from bear::engine::base_item.
 */
void bear::engine::level_loader::load_item()
{
  CLAW_PRECOND( m_current_item == NULL );
  CLAW_PRECOND( m_current_loader == NULL );

  std::string class_name;

  bool fixed;

  m_file >> class_name >> fixed >> m_next_code;
  ++m_item_index;

  m_current_item = create_item_from_string(class_name);

  m_current_loader = new item_loader_map( m_current_item->get_loaders() );

  if (fixed)
    m_current_item->set_insert_as_static();
} // level_loader::load_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the a field of type list.
 */
void bear::engine::level_loader::load_item_field_list()
{
  CLAW_PRECOND(m_next_code == level_code_value::field_list);

  m_file >> m_next_code;

  switch( m_next_code )
    {
    case level_code_value::field_int:
      load_item_field_int_list();
      break;
    case level_code_value::field_u_int:
      load_item_field_u_int_list();
      break;
    case level_code_value::field_real:
      load_item_field_real_list();
      break;
    case level_code_value::field_bool:
      load_item_field_bool_list();
      break;
    case level_code_value::field_string:
      load_item_field_string_list();
      break;
    case level_code_value::field_sprite:
      load_item_field_sprite_list();
      break;
    case level_code_value::field_animation:
      load_item_field_animation_list();
      break;
    case level_code_value::field_item:
      load_item_field_item_list();
      break;
    case level_code_value::field_sample:
      load_item_field_sample_list();
      break;
    case level_code_value::field_font:
      load_item_field_font_list();
      break;
    case level_code_value::field_color:
      load_item_field_color_list();
      break;
    case level_code_value::field_easing:
      load_item_field_easing_list();
      break;
    }
} // level_loader::load_item_field_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type integer to an item.
 */
void bear::engine::level_loader::load_item_field_int()
{
  std::string field_name;
  int val;
  m_file >> field_name >> val >> m_next_code;

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' of item '" << m_current_item->get_class_name()
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_int()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type unsigned integer to an item.
 */
void bear::engine::level_loader::load_item_field_u_int()
{
  std::string field_name;
  unsigned int val;
  m_file >> field_name >> val >> m_next_code;

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' of item '" << m_current_item->get_class_name()
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_u_int()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type real to an item.
 */
void bear::engine::level_loader::load_item_field_real()
{
  std::string field_name;
  double val;
  m_file >> field_name >> val >> m_next_code;

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' of item '" << m_current_item->get_class_name()
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_real()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type boolean to an item.
 */
void bear::engine::level_loader::load_item_field_bool()
{
  std::string field_name;
  bool val;
  m_file >> field_name >> val >> m_next_code;

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' of item '" << m_current_item->get_class_name()
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_bool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type string to an item.
 */
void bear::engine::level_loader::load_item_field_string()
{
  std::string field_name;
  std::string val;

  m_file >> field_name >> val >> m_next_code;

  val = game::get_instance().get_translator().get( val );
  escape(val);

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' of item '" << m_current_item->get_class_name()
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type sprite to an item.
 */
void bear::engine::level_loader::load_item_field_sprite()
{
  std::string field_name;

  m_file >> field_name;

  visual::sprite val
    ( sprite_loader::load_sprite( m_file, m_level->get_globals() ) );

  m_file >> m_next_code;

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' of item '" << m_current_item->get_class_name()
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type animation to an item.
 */
void bear::engine::level_loader::load_item_field_animation()
{
  std::string field_name;

  m_file >> field_name;

  visual::animation val =
    sprite_loader::load_any_animation(m_file, m_level->get_globals());
  m_file >> m_next_code;

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' of item '" << m_current_item->get_class_name()
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type item to an item.
 */
void bear::engine::level_loader::load_item_field_item()
{
  std::string field_name;
  unsigned int index;

  m_file >> field_name >> index >> m_next_code;

  if ( !m_current_loader->set_field( field_name, m_referenced[index] ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' of item '" << m_current_item->get_class_name()
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type sample to an item.
 */
void bear::engine::level_loader::load_item_field_sample()
{
  std::string field_name;

  m_file >> field_name;
  audio::sample* s = load_sample_data();
  m_file >> m_next_code;

  if ( !m_current_loader->set_field( field_name, s ) )
    {
      delete s;
      claw::logger << claw::log_warning << "field '" << field_name
                   << "' of item '" << m_current_item->get_class_name()
                   << "' has not been set." << std::endl;
    }
} // level_loader::load_item_field_sample()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type font to an item.
 */
void bear::engine::level_loader::load_item_field_font()
{
  std::string field_name;

  m_file >> field_name;
  visual::font f( load_font_data() );
  m_file >> m_next_code;

  if ( !m_current_loader->set_field( field_name, f ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' of item '" << m_current_item->get_class_name()
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type color to an item.
 */
void bear::engine::level_loader::load_item_field_color()
{
  std::string field_name;

  m_file >> field_name;
  visual::color f( load_color_data() );
  m_file >> m_next_code;

  if ( !m_current_loader->set_field( field_name, f ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' of item '" << m_current_item->get_class_name()
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type easing to an item.
 */
void bear::engine::level_loader::load_item_field_easing()
{
  std::string field_name;

  m_file >> field_name;
  easing_function e( load_easing_data() );
  m_file >> m_next_code;

  if ( !m_current_loader->set_field( field_name, e ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' of item '" << m_current_item->get_class_name()
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type list of integer to an item.
 */
void bear::engine::level_loader::load_item_field_int_list()
{
  std::vector<int> val;
  std::string field_name = load_list<int>(val);

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' of item '" << m_current_item->get_class_name()
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_int_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type list of unsigned integer to an item.
 */
void bear::engine::level_loader::load_item_field_u_int_list()
{
  std::vector<unsigned int> val;
  std::string field_name = load_list<unsigned int>(val);

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' of item '" << m_current_item->get_class_name()
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_u_int_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type list of real to an item.
 */
void bear::engine::level_loader::load_item_field_real_list()
{
  std::vector<double> val;
  std::string field_name = load_list<double>(val);

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' of item '" << m_current_item->get_class_name()
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_real_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type list of bool to an item.
 */
void bear::engine::level_loader::load_item_field_bool_list()
{
  std::vector<bool> val;
  std::string field_name = load_list<bool>(val);

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' of item '" << m_current_item->get_class_name()
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_bool_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type list of string to an item.
 */
void bear::engine::level_loader::load_item_field_string_list()
{
  unsigned int n;
  std::string v;
  std::string field_name;

  m_file >> field_name >> n;

  std::vector<std::string> val(n);

  for (unsigned int i=0; i!=n; ++i)
    {
      m_file >> v;

      v = game::get_instance().get_translator().get( v );
      escape(v);
      val[i] = v;
    }

  m_file >> m_next_code;

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_string_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type list of sprite to an item.
 */
void bear::engine::level_loader::load_item_field_sprite_list()
{
  std::string field_name;
  unsigned int n;

  m_file >> field_name >> n;

  std::vector<visual::sprite> val(n);

  for (unsigned int i=0; i!=n; ++i)
    val[i] = sprite_loader::load_sprite( m_file, m_level->get_globals() );

  m_file >> m_next_code;

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_sprite_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type list of animation to an item.
 */
void bear::engine::level_loader::load_item_field_animation_list()
{
  std::string field_name;
  unsigned int n;

  m_file >> field_name >> n;

  std::vector<visual::animation> val(n);

  for (unsigned int i=0; i!=n; ++i)
    val[i] = sprite_loader::load_any_animation(m_file, m_level->get_globals());

  m_file >> m_next_code;

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_animation_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type list of item to an item.
 */
void bear::engine::level_loader::load_item_field_item_list()
{
  std::string field_name;
  unsigned int n;

  m_file >> field_name >> n;

  std::vector<base_item*> val(n);

  for (unsigned int i=0; i!=n; ++i)
    {
      unsigned int index;
      m_file >> index;

      CLAW_ASSERT( index < m_referenced.size(), "bad reference index" );
      val[i] = m_referenced[index];
    }

  m_file >> m_next_code;

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_item_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type sample to an item.
 */
void bear::engine::level_loader::load_item_field_sample_list()
{
  std::string field_name;
  unsigned int n;

  m_file >> field_name >> n;

  std::vector<audio::sample*> val(n);

  for (unsigned int i=0; i!=n; ++i)
    val[i] = load_sample_data();

  m_file >> m_next_code;

  if ( !m_current_loader->set_field( field_name, val ) )
    {
      for (unsigned int i=0; i!=n; ++i)
        delete val[i];

      claw::logger << claw::log_warning << "field '" << field_name
                   << "' has not been set." << std::endl;
    }
} // level_loader::load_item_field_sample_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type font to an item.
 */
void bear::engine::level_loader::load_item_field_font_list()
{
  std::string field_name;
  unsigned int n;

  m_file >> field_name >> n;

  std::vector<visual::font> val(n);

  for (unsigned int i=0; i!=n; ++i)
    val[i] = load_font_data();

  m_file >> m_next_code;

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_font_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type color to an item.
 */
void bear::engine::level_loader::load_item_field_color_list()
{
  std::string field_name;
  unsigned int n;

  m_file >> field_name >> n;

  std::vector<visual::color> val(n);

  for (unsigned int i=0; i!=n; ++i)
    val[i] = load_color_data();

  m_file >> m_next_code;

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_color_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type easing to an item.
 */
void bear::engine::level_loader::load_item_field_easing_list()
{
  std::string field_name;
  unsigned int n;

  m_file >> field_name >> n;

  std::vector<easing_function> val(n);

  for (unsigned int i=0; i!=n; ++i)
    val[i] = load_easing_data();

  m_file >> m_next_code;

  if ( !m_current_loader->set_field( field_name, val ) )
    claw::logger << claw::log_warning << "field '" << field_name
                 << "' has not been set." << std::endl;
} // level_loader::load_item_field_easing_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an item using the create_* method exported by the item class.
 * \param name The name of the class to instanciate.
 */
bear::engine::base_item* bear::engine::level_loader::create_item_from_string
( const std::string& name ) const
{
  claw::logger << claw::log_verbose << "Creating item '" << name << "'..."
               << std::endl;

  base_item* result(NULL);

  if ( item_factory::get_instance().is_known_type(name) )
    {
      result = item_factory::get_instance().create(name);
      result->set_level(*m_level);
      claw::logger << claw::log_verbose << "Item '" << name << "' id="
                   << result->get_id() << std::endl;
    }
  else
    {
      claw::logger << claw::log_error << "Can't find item class '" << name
                   << "'." << std::endl;
      throw claw::exception("Can't find item class '" + name + "'.");
    }

  return result;
} // level_loader::create_item_from_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a layer using the create_layer_* method exported by the layer
 *        class.
 * \param name The name of the class to instanciate.
 * \param s The size of the layer.
 */
bear::engine::layer* bear::engine::level_loader::create_layer_from_string
( const std::string& name, const universe::size_box_type& s ) const
{
  claw::logger << claw::log_verbose << "Creating layer '" << name << "'"
               << std::endl;

  layer* result(NULL);

  if ( layer_factory::get_instance().is_known_type(name) )
    {
      layer_creator* creator = layer_factory::get_instance().create(name);
      result = creator->create(s);
      delete creator;
    }
  else
    {
      claw::logger << claw::log_error << "Can't find layer class '" << name
                   << "'." << std::endl;
      throw claw::exception("Can't find layer class '" + name + "'.");
    }

  return result;
} // level_loader::create_layer_from_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert the escaped "\n", "\t", etc. substrings of a string into their
 *        equivalent in the C language.
 * \param str (in/out) The string to convert.
 */
void bear::engine::level_loader::escape( std::string& str ) const
{
  std::string result;
  result.reserve(str.size());
  claw::text::c_escape( str.begin(), str.end(), std::back_inserter(result) );
  str.swap(result);
} // level_loader::escape()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads a sample.
 */
bear::audio::sample* bear::engine::level_loader::load_sample_data() const
{
  std::string sample_path;
  int loops;
  double volume;

  m_file >> sample_path >> loops >> volume;
  audio::sound_effect e(loops, volume);
  audio::sample* s = m_level->get_globals().new_sample(sample_path);

  s->set_effect(e);

  return s;
} // level_loader::load_sample_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads a font.
 */
bear::visual::font bear::engine::level_loader::load_font_data() const
{
  std::string font_path;
  double size;

  m_file >> font_path >> size;
  return m_level->get_globals().get_font( font_path, size );
} // level_loader::load_font_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads a color.
 */
bear::visual::color bear::engine::level_loader::load_color_data() const
{
  double red, green, blue, opacity;

  m_file >> opacity >> red >> green >> blue;

  bear::visual::color result;
  result.set_opacity(opacity);
  result.set_red_intensity(red);
  result.set_green_intensity(green);
  result.set_blue_intensity(blue);

  return result;
} // level_loader::load_color_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads an easing function.
 */
bear::engine::easing_function
bear::engine::level_loader::load_easing_data() const
{
  std::string s;
  m_file >> s;

  easing result;
  result.from_string( s );

  return result.to_claw_easing_function();
} // level_loader::load_easing_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a list of values.
 * \param v (out) The values read from the file.
 * \return The name of the corresponding field.
 */
template<typename T>
std::string bear::engine::level_loader::load_list( std::vector<T>& v )
{
  std::string field_name;
  unsigned int n;
  T val;

  m_file >> field_name >> n;

  v.resize(n);

  for (unsigned int i=0; i!=n; ++i)
    {
      m_file >> val;
      v[i] = val;
    }

  m_file >> m_next_code;

  return field_name;
} // level_loader::load_list()
