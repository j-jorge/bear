/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::layer class.
 * \author Julien Jorge
 */
#include "bf/layer.hpp"

#include "level_code_value.hpp"

#include "bf/compilation_context.hpp"
#include "bf/item_class_pool.hpp"
#include "bf/layer_check_result.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param layer_type The type of the layer.
 * \param layer_name The name of the layer.
 * \param fit_level Tell if the size of the layer is the size of the level.
 * \param width The width of the layer.
 * \param height The height of the layer.
 * \param tag The tag of the layer.
 */
bf::layer::layer
( const std::string& layer_type, const std::string& layer_name,
  bool fit_level, unsigned int width, unsigned int height, 
  const std::string& tag)
  : m_fit_level(fit_level), m_width(width), m_height(height),
    m_layer_type(layer_type), m_layer_name(layer_name), m_tag(tag)
{
  CLAW_PRECOND( width > 0 );
  CLAW_PRECOND( height > 0 );
} // layer::layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The layer to copy from.
 */
bf::layer::layer( const layer& that )
{
  assign(that);
} // layer::layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::layer::~layer()
{
  clear();
} // layer::~layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the layer has to fit the level.
 * \param f Fit or not.
 */
void bf::layer::set_fit_level( bool f )
{
  m_fit_level = f;
} // layer::set_fit_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the layer has to fit the level.
 */
bool bf::layer::fits_level() const
{
  return m_fit_level;
} // layer::fits_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Change the size of the layer.
 * \param width The new width of the layer.
 * \param height The new height of the layer.
 */
void bf::layer::resize( unsigned int width, unsigned int height )
{
  CLAW_PRECOND( width > 0 );
  CLAW_PRECOND( height > 0 );

  m_width = width;
  m_height = height;
} // layer::resize()

/*----------------------------------------------------------------------------*/
/**
 * \brief Change the name of the class of the layer.
 * \param class_name The new name of the class.
 */
void bf::layer::set_class_name( const std::string& class_name )
{
  CLAW_PRECOND( !class_name.empty() );

  m_layer_type = class_name;
} // layer::set_class_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Change the name of the layer.
 * \param name The new name of the layer.
 */
void bf::layer::set_name( const std::string& name )
{
  m_layer_name = name;
} // layer::set_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the tag of the layer.
 * \param tag The new tag of the layer.
 */
void bf::layer::set_tag( const std::string& tag )
{
  m_tag = tag;
} // layer::set_tag()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the tag of the layer.
 */
std::string bf::layer::get_tag() const
{
  return m_tag;
} // layer::get_tag()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item in the layer.
 * \param item The item to add.
 */
void bf::layer::add_item( item_instance* item )
{
  m_item.insert(item);

  if ( evaluate_filters_on_item(*item) )
    m_filtered_item.insert(item);
} // layer::add_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the layer.
 * \param item The item to remove.
 * \remark \a item won't be <tt>delete</tt>'d.
 */
void bf::layer::remove_item( item_instance* item )
{
  CLAW_PRECOND( m_item.find(item) != m_item.end() );
  CLAW_PRECOND( !is_prioritized(item) );

  if ( evaluate_filters_on_item(*item) )
    m_filtered_item.erase(item);

  m_item.erase(item);
} // layer::remove_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find an item with a given id.
 * \param id The id of the searched item.
 */
bf::layer::item_iterator
bf::layer::find_item_by_id( const std::string& id ) const
{
  bool found = false;
  item_iterator it = item_begin_no_filter();
  item_iterator eit( item_end_no_filter() );

  while ( !found && (it!=eit) )
    if ( it->get_id() == id )
      found = true;
    else
      ++it;

  return it;
} // layer::find_item_by_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get all the items whose identifier is set.
 */
std::list<bf::item_instance*> bf::layer::get_identified_items() const
{
  std::list<item_instance*> result;

  for ( item_set_type::iterator it=m_item.begin(); it!=m_item.end(); ++it )
    if ( !(*it)->get_id().empty() )
      result.push_back(*it);

  return result;
} // layer::get_identified_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item at the end of the list of the prioritized items for the
 *        compilation.
 * \param item The item to add.
 */
void bf::layer::prioritize( item_instance* item )
{
  prioritize( item, m_priority.size() );
} // layer::prioritize()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item at the end of the list of the prioritized items for the
 *        compilation.
 * \param item The item to add.
 * \param p The position of the item in the list.
 */
void bf::layer::prioritize( item_instance* item, std::size_t p )
{
  if ( is_prioritized(item) )
    deprioritize(item);

  item_list_type::iterator it=m_priority.begin();
  std::advance(it, std::min(p, m_priority.size()));
  m_priority.insert( it, item );
} // layer::prioritize()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the list of the prioritized items.
 * \param item The item to remove.
 * \pre is_prioritized(item)
 */
void bf::layer::deprioritize( item_instance* item )
{
  CLAW_PRECOND( is_prioritized(item) );

  m_priority.erase( std::find( m_priority.begin(), m_priority.end(), item ) );
} // layer::()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if an item is prioritized for the compilation.
 * \param item The item to search.
 */
bool bf::layer::is_prioritized( item_instance* item ) const
{
  return std::find( m_priority.begin(), m_priority.end(), item )
    != m_priority.end();
} // layer::is_prioritized()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the priority of an item in the compilation.
 * \param item The item for which we want the priority.
 * \pre is_prioritized(item)
 */
std::size_t bf::layer::get_priority( item_instance* item ) const
{
  CLAW_PRECOND( is_prioritized(item) );

  item_list_type::const_iterator it =
    std::find( m_priority.begin(), m_priority.end(), item );
  return std::distance( m_priority.begin(), it );
} // layer::get_priority()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the prioritized items.
 */
std::vector<bf::item_instance*> bf::layer::get_prioritized_items() const
{
  return std::vector<item_instance*>(m_priority.begin(), m_priority.end());
} // layer::get_prioritized_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the type of the layer.
 */
const std::string& bf::layer::get_class_name() const
{
  return m_layer_type;
} // layer::get_class_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the layer.
 */
const std::string& bf::layer::get_name() const
{
  return m_layer_name;
} // layer::get_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get displayed name.
 */
std::string bf::layer::get_displayed_name() const
{
  return "{" + m_tag + "} " + m_layer_name + " [" + m_layer_type + "]";
} // layer::get_displayed_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of the layer.
 */
unsigned int bf::layer::get_width() const
{
  return m_width;
} // layer::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of the layer.
 */
unsigned int bf::layer::get_height() const
{
  return m_height;
} // layer::get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of items.
 */
std::size_t bf::layer::get_items_count() const
{
  return m_item.size();
} // layer::get_items_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is no item in the layer.
 */
bool bf::layer::empty() const
{
  return m_item.empty();
} // layer::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the layer contains a given item.
 * \param item The item to look for.
 */
bool bf::layer::contains( const item_instance& item ) const
{
  return m_item.find(const_cast<item_instance*>(&item)) != m_item.end();
} // layer::contains()

/*----------------------------------------------------------------------------*/
/**
 * \brief Test if the layer is valid.
 * \param result The result of the check.
 */
void bf::layer::check( layer_check_result& result ) const
{
  check_priorities(result);

  std::set<std::string> map_id;
  check_unique_identifiers( result, map_id );

  item_set_type::const_iterator it;

  for (it=m_item.begin(); it!=m_item.end(); ++it)
    result.check( *it, map_id );
} // layer::check()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the layer.
 * \param f The file in which we compile.
 * \param c The context in which the compilation is done.
 */
void bf::layer::compile( compiled_file& f, compilation_context& c ) const
{
  f << m_layer_type << m_width << m_height << m_tag;

  std::map<std::string, unsigned int> id_to_int;
  std::list<item_instance*> referenced, not_referenced;
  std::list<item_instance*>::const_iterator iti;
  item_set_type::const_iterator it;

  for (it=m_item.begin(); it!=m_item.end(); ++it)
    if ( (*it)->get_id().empty() )
      not_referenced.push_front(*it);
    else
      referenced.push_back(*it);

  sort_and_identify(referenced, c);

  if ( !referenced.empty() )
    {
      f << bear::level_code_value::item_declaration << referenced.size();

      // declaration of referenced items
      for (iti=referenced.begin(); iti!=referenced.end(); ++iti)
        f << (*iti)->get_class().get_class_name();

      // definition of referenced items
      for (iti=referenced.begin(); iti!=referenced.end(); ++iti)
        {
          f << bear::level_code_value::item_definition;
          (*iti)->compile( f, c );
        }
    }

  // not referenced items
  for (iti=not_referenced.begin(); iti!=not_referenced.end(); ++iti)
    {
      f << bear::level_code_value::base_item
        << (*iti)->get_class().get_class_name();
      (*iti)->compile( f, c );
    }
} // layer::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first filter.
 */
bf::layer::const_filter_iterator bf::layer::filter_begin() const
{
  return m_filters.begin();
} // layer::filter_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator just past the last filter.
 */
bf::layer::const_filter_iterator bf::layer::filter_end() const
{
  return m_filters.end();
} // layer::filter_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the set of items.
 * \remark Iterators are invalidated when inserting a filter if there was no
 *         filter before and when removing a filter if the pointed item does not
 *         verify any of the remaining filters or if there is no more filters.
 */
bf::layer::item_iterator bf::layer::item_begin() const
{
  if ( m_filters.empty() )
    return m_item.begin();
  else
    return m_filtered_item.begin();
} // layer::item_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the end of the set of items.
 */
bf::layer::item_iterator bf::layer::item_end() const
{
  if ( m_filters.empty() )
    return m_item.end();
  else
    return m_filtered_item.end();
} // layer::item_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the set of items.
 * \remark These iterators iterates over all items, whatever the filters are.
 */
bf::layer::item_iterator bf::layer::item_begin_no_filter() const
{
  return m_item.begin();
} // layer::item_begin_no_filter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator past the end of the set of items.
 */
bf::layer::item_iterator bf::layer::item_end_no_filter() const
{
  return m_item.end();
} // layer::item_end_no_filter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 * \param that The instance to copy from.
 */
bf::layer& bf::layer::operator=( const layer& that )
{
  if ( &that != this )
    {
      clear();
      assign( that );
    }

  return *this;
} // layer::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a filter on the items of the layer.
 * \param filter The filter to apply on the items.
 */
void bf::layer::add_filter( const item_filter& filter )
{
  m_filters.push_back(filter);

  item_set_type::iterator it;

  for (it=m_item.begin(); it!=m_item.end(); ++it)
    if ( filter.evaluate(**it) )
      m_filtered_item.insert(*it);
} // layer::add_filter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a filter on the items of the layer.
 * \param filter The filter to remove.
 */
void bf::layer::remove_filter( const item_filter& filter )
{
  const filter_list::iterator itf =
    std::find(m_filters.begin(), m_filters.end(), filter);

  if ( itf!=m_filters.end() )
    m_filters.erase( itf );

  item_set_type::iterator it(m_filtered_item.begin());

  while ( it!=m_filtered_item.end() )
    if ( !evaluate_filters_on_item(**it) )
      {
        item_set_type::iterator tmp(it);
        ++it;
        m_filtered_item.erase(tmp);
      }
    else
      ++it;
} // layer::remove_filter()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if items are in the layer.
 */
bool bf::layer::check_item_position() const
{
  bool result = true;

  layer::item_iterator it;

  for (it = item_begin(); it != item_end() && result; ++it)
    {
      const item_rendering_parameters& r( it->get_rendering_parameters() );
      
      result = 
        r.get_right() >= 0 && 
        r.get_top() >= 0 &&
        r.get_left() <= get_width() &&
        r.get_bottom() <= get_height();   
    }

  return result;
} // layer::check_item_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all items.
 */
void bf::layer::clear()
{
  item_set_type::iterator it;

  for (it=m_item.begin(); it!=m_item.end(); ++it)
    delete *it;

  m_item.clear();
} // layer::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assign a layer to this one.
 * \param that The instance to copy from.
 */
void bf::layer::assign( const layer& that )
{
  CLAW_PRECOND( m_item.empty() );
  CLAW_PRECOND( m_filters.empty() );
  CLAW_PRECOND( &that != this );

  m_fit_level = that.m_fit_level;
  m_width = that.m_width;
  m_height = that.m_height;
  m_layer_type = that.m_layer_type;
  m_layer_name = that.m_layer_name;
  m_tag = that.m_tag;

  for ( filter_list::const_iterator it=that.m_filters.begin();
        it!=that.m_filters.end(); ++it )
    m_filters.push_back( it->copy() );

  for ( item_set_type::const_iterator it=that.m_item.begin();
        it!=that.m_item.end(); ++it )
    m_item.insert( new item_instance( **it ) );
} // layer::assign()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort referenced items such that the referenced is before the
 *        referencer and give an identifier to each item.
 * \param referenced (in/out) The referenced items.
 * \param c The context in which the identifiers are stored.
 */
void bf::layer::sort_and_identify
( std::list<item_instance*>& referenced, compilation_context& c ) const
{
  std::map<std::string, item_instance*> items;
  std::list<item_instance*>::const_iterator it;

  for (it=referenced.begin(); it!=referenced.end(); ++it)
    if ( !is_prioritized(*it) )
      items[(*it)->get_id()] = *it;

  referenced = m_priority;

  while ( !items.empty() )
    sort_by_dependency( items.begin()->second, items, referenced );

  c.clear_compiled_identifiers();
  unsigned int index = 0;

  for (it=referenced.begin(); it!=referenced.end(); ++it, ++index)
    c.set_compiled_identifier( (*it)->get_id(), index );
} // layer::sort_and_identify()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort referenced items such that the referenced is before the
 *        referencer.
 * \param p Pointer on the item that references an other item.
 * \param items The set of referenced items associated with their name.
 * \param referenced (out) The ordered referenced items.
 */
void bf::layer::sort_by_dependency
( item_instance* p, std::map<std::string, item_instance*>& items,
  std::list<item_instance*>& referenced ) const
{
  const item_class& the_class(p->get_class());

  items.erase(p->get_id());

  std::list<std::string> f;
  the_class.get_field_names_in_hierarchy(f);
  std::list<std::string>::const_iterator it;

  for (it=f.begin(); it!=f.end(); ++it)
    {
      const bf::type_field& field(the_class.get_field(*it));

      if ( field.get_field_type() == type_field::item_reference_field_type )
        if ( p->has_value(field) )
          {
            if ( field.is_list() )
              {
                std::list<item_reference_type> v;
                p->get_value(field.get_name(), v);

                for( ; !v.empty(); v.pop_front() )
                  if ( items.find( v.front().get_value() ) != items.end() )
                    sort_by_dependency
                      (items[v.front().get_value()], items, referenced);
              }
            else
              {
                item_reference_type v;
                p->get_value(field.get_name(), v);
                if ( items.find(v.get_value()) != items.end() )
                  sort_by_dependency(items[v.get_value()], items, referenced);
              }
          }
    }

  referenced.push_back(p);
} // layer::sort_by_dependency()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if an item validates at least one filter.
 * \param item The item on which the filters are evaluated.
 */
bool bf::layer::evaluate_filters_on_item( const item_instance& item ) const
{
  bool result = false;
  filter_list::const_iterator it;

  for ( it=m_filters.begin(); !result && (it!=m_filters.end()); ++it )
    result = it->evaluate(item);

  return result;
} // layer::evaluate_filters_on_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check that the prioritized items are in a feasible order.
 * \param result The check result in which the errors are added.
 */
void bf::layer::check_priorities( layer_check_result& result ) const
{
  item_list_type::const_iterator it;

  for ( it=m_priority.begin(); it!=m_priority.end(); ++it )
    if ( (*it)->get_id().empty() )
      result.add( *it, check_error("Prioritized items must have an id.") );
    else
      {
        item_list_type::const_iterator next=it;

        for ( ++next; next!=m_priority.end(); ++next )
          if ( (*it)->has_a_reference_to((*next)->get_id()) )
            {
              const check_error error
                ( (*it)->get_id() + "/" + (*next)->get_id(),
                    "The build order is in conflict with the dependency based "
                    "on the fields.");

              result.add(*next, error);
              result.add(*it, error);
            }
      }
} // layer::check_priorities()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check that the identifiers of the item are unique.
 * \param result The check result in which the errors are added.
 * \param identifiers (out) The identifiers of the items.
 */
void bf::layer::check_unique_identifiers
( layer_check_result& result, std::set<std::string>& identifiers ) const
{
  std::map<std::string, item_instance*> map_id;

  item_set_type::const_iterator it;
  for ( it=m_item.begin(); it!=m_item.end(); ++it )
    if ( !(*it)->get_id().empty() )
      {
        if ( map_id.find((*it)->get_id()) != map_id.end() )
          {
            item_instance* other = map_id[(*it)->get_id()];
            const check_error error
              ( (*it)->get_id(), "This identifier is already in use." );

            result.add( other, error );
            result.add( *it, error );
          }
        else
          {
            map_id[(*it)->get_id()] = *it;
            identifiers.insert( (*it)->get_id() );
          }
      }
} // layer::check_unique_identifiers()
