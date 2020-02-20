/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::level class.
 * \author Julien Jorge
 */
#include "bf/level.hpp"

#include "bf/level_check_result.hpp"
#include "bf/version.hpp"

#include "bear/level_code_value.hpp"

#include <claw/assert.hpp>
#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the level.
 * \param width The width of the level.
 * \param height The height of the level.
 * \param mus The music played in this level.
 */
bf::level::level
( const std::string& name, unsigned int width, unsigned int height,
  const std::string& mus )
  : m_name(name), m_width(width), m_height(height), m_music(mus)
{

} // level::level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 */
bf::level::level( const level& that )
{
  assign(that);
} // level::level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::level::~level()
{
  clear();
} // level::~level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a layer given its position in the stack.
 * \param index The position of the layer.
 */
bf::layer& bf::level::get_layer( unsigned int index ) const
{
  CLAW_PRECOND( index < m_layer.size() );
  return *m_layer[index];
} // level::get_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a layer given its position in the stack.
 * \param index The position of the layer.
 * \return The layer at position \a index before the suppression.
 */
bf::layer* bf::level::remove_layer( unsigned int index )
{
  CLAW_PRECOND( index < m_layer.size() );

  layer* result = m_layer[index];
  m_layer.erase(m_layer.begin() + index);
  return result;
} // level::remove_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a layer.
 * \param class_name The name of the class of the layer.
 * \param name The name of the layer.
 * \param fit_level Tell if the layer size is the same than the level.
 * \param width The width of the layer.
 * \param height The height of the layer.
 * \remark The layer is added at the end (index layers_count()).
 */
bf::layer& bf::level::add_layer
( const std::string& class_name, const std::string& name,
  bool fit_level, unsigned int width,
  unsigned int height )
{
  add_layer
    ( new layer( class_name, name, fit_level, width, height ),
      m_layer.size() );
  return *m_layer.back();
} // level::add_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert a layer.
 * \param lay The layer to insert.
 * \param index The position where \a lay is inserted.
 */
void bf::level::add_layer( layer* lay, unsigned int index )
{
  CLAW_PRECOND( index <= m_layer.size() );

  if ( lay->fits_level() )
    lay->resize( get_width(), get_height() );

  m_layer.insert( m_layer.begin() + index, lay );
} // level::add_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move a layer toward the background.
 * \param layer_index The position of the layer.
 */
void bf::level::move_backward( unsigned int layer_index )
{
  if ( layer_index > 0 )
    std::swap( m_layer[layer_index], m_layer[layer_index-1] );
} // level::move_backward()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move a layer toward the foreground.
 * \param layer_index The position of the layer.
 */
void bf::level::move_forward( unsigned int layer_index )
{
  if ( layer_index + 1 < m_layer.size() )
    std::swap( m_layer[layer_index], m_layer[layer_index+1] );
} // level::move_forward()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the level has no layer.
 */
bool bf::level::empty() const
{
  return m_layer.empty();
} // level::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of layers.
 */
std::size_t bf::level::layers_count() const
{
  return m_layer.size();
} // level::layers_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of the level.
 */
unsigned int bf::level::get_width() const
{
  return m_width;
} // level::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of the level.
 */
unsigned int bf::level::get_height() const
{
  return m_height;
} // level::get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the configuration of the level.
 * \param width The width of the level.
 * \param height The height of the level.
 */
void bf::level::set_size( unsigned int width, unsigned int height )
{
  m_width = width;
  m_height = height;

  for ( std::size_t i=0; i!=m_layer.size(); ++i )
    if ( m_layer[i]->fits_level() )
      m_layer[i]->resize( m_width, m_height );
} // level::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the music of the level.
 */
const std::string& bf::level::get_music() const
{
  return m_music;
} // level::get_music()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the music of the level.
 * \param path The path to the music file.
 */
void bf::level::set_music( const std::string& path )
{
  m_music = path;
} // level::set_music()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the level.
 */
const std::string& bf::level::get_name() const
{
  return m_name;
} // level::get_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the level.
 * \param name The path to the name file.
 */
void bf::level::set_name( const std::string& name )
{
  m_name = name;
} // level::set_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find an item with a given id.
 * \param id The id of the searched item.
 */
std::pair<bool, bf::layer::item_iterator>
bf::level::find_item_by_id( const std::string& id ) const
{
  bool found = false;
  layer::item_iterator it;

  for (unsigned int i=0; !found && (i!=m_layer.size()); ++i)
    {
      it = m_layer[i]->find_item_by_id(id);
      found = (it != m_layer[i]->item_end_no_filter());
    }

  return std::pair<bool, layer::item_iterator>(found, it);
} // level::find_item_by_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the layer in which an item is.
 * \param item The item for which we want the layer.
 * \return The index of the layer containing the item, or layers_count() if none
 *         of the layers contain him.
 */
std::size_t bf::level::get_layer_by_item( const item_instance& item ) const
{
  std::size_t result(0);
  bool found(false);

  while ( !found && (result!=layers_count()) )
    if ( get_layer(result).contains(item) )
      found = true;
    else
      ++result;

  return result;
} // level::get_layer_by_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Generates a new identifier.
 * \param id (in) The desired identifier, (out) a valid identifier.
 */
void bf::level::generate_valid_id( std::string& id ) const
{
  generate_valid_id( id, std::set<std::string>() );
} // generate_valid_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Generates a new identifier.
 * \param id (in) The desired identifier, (out) a valid identifier.
 * \param avoid A set of names to avoid.
 */
void bf::level::generate_valid_id
( std::string& id, const std::set<std::string>& avoid ) const
{
  if ( find_item_by_id(id).first || ( avoid.find(id) != avoid.end() ) )
    {
      std::string prefix(id);
      std::string::value_type sep('-');
      id.clear();

      std::string::size_type p = prefix.find_last_not_of("0123456789");

      if ( p != std::string::npos )
        if ( std::string("_-").find_first_of(prefix[p]) != std::string::npos )
          {
            sep = prefix[p];
            prefix = prefix.substr(0, p);
          }

      std::size_t index(0);

      while ( id.empty() )
        {
          ++index;

          std::ostringstream oss;
          oss << prefix << sep << index;

          if ( !find_item_by_id(oss.str()).first
               && ( avoid.find(oss.str()) == avoid.end() ) )
            id = oss.str();
        }
    }
} // level::generate_valid_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Change the idenfiers of a bunch of items in order to avoid name
 *        collisions.
 * \param items The list of the items to update.
 */
void bf::level::fix_identifiers
( const std::vector<item_instance*>& items ) const
{
  std::map<std::string, std::string> id_map;
  std::vector<item_instance*>::const_iterator it;
  std::set<std::string> avoid;

  for (it=items.begin(); it!=items.end(); ++it)
    if ( !(*it)->get_id().empty() )
      {
        std::string id( (*it)->get_id() );
        generate_valid_id(id, avoid);
        avoid.insert(id);

        id_map[(*it)->get_id()] = id;
        (*it)->set_id(id);
      }

  for (it=items.begin(); it!=items.end(); ++it)
    (*it)->rename_item_reference_fields(id_map);
} // level::fix_identifiers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 * \param that The instance to copy from.
 */
bf::level& bf::level::operator=( const level& that )
{
  if ( &that != this )
    {
      clear();
      assign(that);
    }

  return *this;
} // level::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Test if the level is valid.
 * \param result The result of the check.
 */
void bf::level::check( level_check_result& result )
{
  for (unsigned int i=0; i!=m_layer.size(); ++i)
    result.check( m_layer[i] );
} // level::check()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the level.
 * \param f The file in which we compile.
 * \param c The context in which the compilation is done.
 */
void bf::level::compile( compiled_file& f, compilation_context& c ) const
{
  std::size_t items_count = 0;

  for (unsigned int i=0; i!=m_layer.size(); ++i)
    items_count += m_layer[i]->get_items_count();

  f << BF_MAJOR_VERSION << BF_MINOR_VERSION << BF_RELEASE_NUMBER
    << m_name << m_width << m_height << m_music << items_count
    << m_layer.size();

  for (unsigned int i=0; i!=m_layer.size(); ++i)
    {
      f << bear::level_code_value::layer;
      m_layer[i]->compile(f, c);
    }

  f << bear::level_code_value::eof;
} // level::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a filter on the items of all layers.
 * \param filter The filter to apply on the layers.
 */
void bf::level::add_filter( const item_filter& filter )
{
  for ( std::size_t i=0; i!=m_layer.size(); ++i )
    m_layer[i]->add_filter(filter);
} // level::add_filter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a filter on the items of all layers.
 * \param filter The filter to remove.
 */
void bf::level::remove_filter( const item_filter& filter )
{
  for ( std::size_t i=0; i!=m_layer.size(); ++i )
    m_layer[i]->remove_filter(filter);
} // level::remove_filter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all layers.
 */
void bf::level::clear()
{
  for (unsigned int i=0; i!=m_layer.size(); ++i)
    delete m_layer[i];

  m_layer.clear();
} // level::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assign a level to this one.
 * \param that The instance to copy from.
 */
void bf::level::assign( const level& that )
{
  CLAW_PRECOND( m_layer.empty() );
  CLAW_PRECOND( &that != this );

  m_name = that.m_name;
  m_width = that.m_width;
  m_height = that.m_height;
  m_music = that.m_music;

  m_layer.resize( that.m_layer.size() );

  for (unsigned int i=0; i!=that.m_layer.size(); ++i)
    m_layer[i] = new layer( *that.m_layer[i] );
} // level::assign()
