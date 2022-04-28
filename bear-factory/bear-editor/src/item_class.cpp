/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::item_class class.
 * \author Julien Jorge
 */
#include "bf/item_class.hpp"

#include <claw/logger/logger.hpp>
#include <claw/assert.hpp>
#include <algorithm>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::item_class::item_class()
{

} // item_class::item_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The item to copy from.
 */
bf::item_class::item_class( const item_class& that )
{
  copy(that);
} // item_class::item_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::item_class::~item_class()
{
  clear();
} // item_class::~item_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment.
 * \param that The item to copy from.
 */
bf::item_class& bf::item_class::operator=( const item_class& that )
{
  if ( this != &that )
    {
      clear();
      copy(that);
    }

  return *this;
} // item_class::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first field.
 */
bf::item_class::field_iterator bf::item_class::field_begin() const
{
  return m_field.begin();
} // item_class::field_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator just past the last field.
 */
bf::item_class::field_iterator bf::item_class::field_end() const
{
  return m_field.end();
} // item_class::field_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first super class.
 */
bf::item_class::const_super_class_iterator
bf::item_class::super_class_begin() const
{
  return m_super_classes.begin();
} // item_class::super_class_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator just past the last super class.
 */
bf::item_class::const_super_class_iterator
bf::item_class::super_class_end() const
{
  return m_super_classes.end();
} // item_class::super_class_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a field to this item.
 * \param name The name of the field.
 * \param field Pointer on the field to add.
 * \remark \a field will be deleted by the destructor.
 */
void
bf::item_class::add_field( const std::string& name, const type_field& field )
{
  field_map_type::iterator it = m_field.find(name);

  if ( it == m_field.end() )
    m_field[name] = field.clone();
  else
    claw::logger << claw::log_error << "Field '" << name << "' already exists."
                 << std::endl;
} // item_class::add_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the description of the class.
 * \param desc The description of the class.
 */
void bf::item_class::set_description( const std::string& desc )
{
  m_description = desc;
} // item_class::set_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the url where a documentation of the class can be found.
 * \param url The url.
 */
void bf::item_class::set_url( const std::string& url )
{
  m_url = url;
} // item_class::set_url()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the class.
 * \param class_name The name of the class.
 */
void bf::item_class::set_class_name( const std::string& class_name )
{
  m_class_name = class_name;
} // item_class::set_class_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the category of the class.
 * \param category The category of the class.
 */
void bf::item_class::set_category( const std::string& category )
{
  m_category = category;
} // item_class::set_category()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the color used for displaying the item.
 * \param color The color to use.
 */
void bf::item_class::set_color( const std::string& color )
{
  m_color = color;
} // item_class::set_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item can be fixed.
 * \param fixable The value to set.
 */
void bf::item_class::set_fixable( bool fixable )
{
  m_fixable = fixable;
} // item_class::set_fixable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the class has a given class somewhere in its super classes.
 * \param super_class The name of the super class to search for.
 */
bool bf::item_class::inherits_from( const std::string& super_class ) const
{
  bool result = has_super_class(super_class);

  if (!result)
    {
      const_super_class_iterator it;

      for (it=super_class_begin(); !result && (it!=super_class_end()); ++it )
        result = it->inherits_from(super_class);
    }

  return result;
} // item_class::inherits_from()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the class has a given immediate super class.
 * \param super_class The name of the super class.
 */
bool bf::item_class::has_super_class( const std::string& super_class ) const
{
  bool result = false;

  const_super_class_iterator it;

  for (it=super_class_begin(); !result && (it!=super_class_end()); ++it )
    result = (it->get_class_name() == super_class);

  return result;
} // item_class::add_super_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a class from which this one inherits.
 * \param super_class The name of the super class.
 */
void bf::item_class::add_super_class( item_class const* super_class )
{
  m_super_classes.push_back(super_class);
} // item_class::add_super_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a class from the super classes.
 * \param super_class The name of the super class.
 */
void bf::item_class::remove_super_class( const std::string& super_class )
{
  bool found = false;

  super_class_list::iterator it(m_super_classes.begin());

  while ( !found && (it!=m_super_classes.end()) )
    if ( (*it)->get_class_name() == super_class )
      found = true;
    else
      ++it;

  if ( found )
    m_super_classes.erase(it);
} // item_class::remove_super_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the default value of a field of this class or a parent class.
 * \param f The name of the field.
 * \param v The new default value.
 */
void
bf::item_class::new_default_value( const std::string& f, const std::string& v )
{
  m_default_value[f] = v;
} // item_class::new_default_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the default value of a field of this class or a parent class.
 * \param f The name of the field.
 */
std::string bf::item_class::get_default_value( const std::string& f ) const
{
  std::string result;
  string_map_type::const_iterator it = m_default_value.find(f);

  if ( it!=m_default_value.end() )
    result = it->second;
  else
    {
      field_map_type::const_iterator itf = m_field.find(f);

      if ( itf!=m_field.end() )
        result = itf->second->get_default_value();
      else
        {
          const_super_class_iterator it_p = super_class_begin();

          for ( it_p=m_super_classes.begin();
                result.empty() && it_p!=super_class_end(); ++it_p )
            result = it_p->get_default_value(f);
        }
    }

  return result;
} // item_class::get_default_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indicate that a field of the parent classes is defined by this class.
 * \param f The name of the field.
 */
void bf::item_class::add_removed_field( const std::string& f )
{
  CLAW_PRECOND( has_field(f) );
  CLAW_PRECOND( std::find( m_removed_fields.begin(), m_removed_fields.end(), f )
                == m_removed_fields.end() );

  m_removed_fields.push_back(f);
} // item_class::add_removed_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a field of the parent classes is removed in this class.
 * \param f The name of the field.
 */
bool bf::item_class::is_removed_field( const std::string& f ) const
{
  CLAW_PRECOND( has_field(f) );

  return std::find( m_removed_fields.begin(), m_removed_fields.end(), f )
    != m_removed_fields.end();
} // item_class::is_removed_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get all the names of the fields of this class and its super classes.
 * \param f (out) The names of the fields.
 */
void bf::item_class::get_field_names_in_hierarchy
( std::list<std::string>& f ) const
{
  std::list<std::string> fields;
  std::list<std::string> removed;

  get_all_field_names_in_hierarchy(fields);
  get_removed_fields_names_in_hierarchy(removed);

  fields.sort();
  removed.sort();
  f.clear();

  std::set_difference
    ( fields.begin(), fields.end(), removed.begin(), removed.end(),
      std::back_inserter(f) );

  f.unique();
} // item_class::get_field_names_in_hierarchy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the description of a field, given its name.
 * \param field_name The name of the field.
 */
const bf::type_field&
bf::item_class::get_field( const std::string& field_name ) const
{
  CLAW_PRECOND( has_field(field_name) );
  const type_field* result = search_field(field_name);

  return *result;
} // item_class::get_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the description of the class.
 */
const std::string& bf::item_class::get_description() const
{
  return m_description;
} // item_class::get_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the url of the documentation of the class.
 */
const std::string& bf::item_class::get_url() const
{
  return m_url;
} // item_class::get_url()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the class.
 */
const std::string& bf::item_class::get_class_name() const
{
  return m_class_name;
} // item_class::get_class_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the category of the class.
 */
const std::string& bf::item_class::get_category() const
{
  return m_category;
} // item_class::get_category()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the color used for displaying the item.
 */
const std::string& bf::item_class::get_color() const
{
  return m_color;
} // item_class::get_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item can be fixable.
 */
bool bf::item_class::get_fixable() const
{
  bool result = m_fixable;

  const_super_class_iterator it;
  for ( it = super_class_begin(); ( it != super_class_end() ) && result; ++it)
    result = result && it->get_fixable();

  return result;
} // item_class::get_fixable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the whole class hierarchy of the class.
 * \param hierarchy (out) The classes from which \a class_name inherit.
 */
void
bf::item_class::find_hierarchy( std::list<item_class const*>& hierarchy ) const
{
  hierarchy.push_front( this );

  const_super_class_iterator it;
  const const_super_class_iterator eit = super_class_end();

  for ( it=super_class_begin(); it!=eit; ++it )
    it->find_hierarchy(hierarchy);
} // item_class::find_hierachy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Test the unicity of each field.
 * Return True if the class is valid.
 * \param error_msg The message of the error.
 */
bool bf::item_class::field_unicity_test(std::string& error_msg) const
{
  bool result = true;
  std::set<std::string> fields;

  std::list<item_class const*>::iterator super_class;
  std::list<item_class const*> hierarchy;

  find_hierarchy( hierarchy );

  for ( super_class = hierarchy.begin();
        ( super_class != hierarchy.end() ) && result; ++super_class)
    {
      field_iterator it;

      for ( it = (*super_class)->field_begin();
            ( it != (*super_class)->field_end() ) && result; ++it )
        if ( fields.find(it->get_name()) != fields.end() )
          {
            result = false;
            error_msg = "the field '" + it->get_name() +
              "' is already defined in the super class '"
              + (*super_class)->get_class_name() + "'";
          }
        else
          fields.insert(it->get_name());
    }

  return result;
} // item_class::field_unicity_test()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this class, or one of the parent classes, has a field with a
 *        given name.
 * \param name The name of the field we are looking for.
 */
bool bf::item_class::has_field( const std::string& name ) const
{
  bool result = false;
  field_map_type::const_iterator it = m_field.find( name );

  if ( it != m_field.end() )
    result = true;
  else
    {
      const_super_class_iterator it;

      for ( it=super_class_begin(); !result && (it!=super_class_end()); ++it)
        result = it->has_field( name );
    }

  return result;
} // item_class::has_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this class, or one of the parent classes, has a field of a
 *        given type with a given name.
 * \param name The name of the field we are looking for.
 * \param t The desired type.
 */
bool bf::item_class::has_field
( const std::string& name, type_field::field_type t ) const
{
  bool result = false;
  field_map_type::const_iterator it = m_field.find( name );

  if ( it != m_field.end() )
    result = it->second->get_field_type() == t;
  else
    {
      const_super_class_iterator it;

      for (it=super_class_begin(); !result && (it!=super_class_end()); ++it)
        result = it->has_field( name, t );
    }

  return result;
} // item_class::has_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all fields.
 */
void bf::item_class::clear()
{
  field_map_type::const_iterator it;

  for (it=m_field.begin(); it!=m_field.end(); ++it)
    delete it->second;

  m_field.clear();
} // item_class::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy all fields from an other item class.
 * \param that The item to copy from.
 */
void bf::item_class::copy( const item_class& that )
{
  m_class_name     = that.m_class_name;
  m_description    = that.m_description;
  m_url            = that.m_url;
  m_category       = that.m_category;
  m_color          = that.m_color;
  m_fixable        = that.m_fixable;
  m_super_classes  = that.m_super_classes;
  m_default_value  = that.m_default_value;
  m_removed_fields = that.m_removed_fields;

  field_map_type::const_iterator it;

  for (it=that.m_field.begin(); it!=that.m_field.end(); ++it)
    m_field[it->first] = it->second->clone();
} // item_class::copy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the description of a field, given its name.
 * \param field_name The name of the field.
 */
const bf::type_field*
bf::item_class::search_field( const std::string& field_name ) const
{
  field_map_type::const_iterator it = m_field.find(field_name);
  const bf::type_field* result = NULL;

  if ( it != m_field.end() )
    result = it->second;
  else
    {
      const_super_class_iterator it_p;

      for ( it_p=super_class_begin();
            (result == NULL) && it_p!=super_class_end(); ++it_p )
        result = it_p->search_field(field_name);
    }

  return result;
} // item_class::search_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get all the names of the fields whose values are set by this class and
 *        its super classes.
 * \param f (out) The names of the fields.
 */
void bf::item_class::get_removed_fields_names_in_hierarchy
( std::list<std::string>& f ) const
{
  std::copy
    ( m_removed_fields.begin(), m_removed_fields.end(),
      std::front_inserter(f) );

  const_super_class_iterator it;

  for ( it=super_class_begin(); it!=super_class_end(); ++it )
    it->get_removed_fields_names_in_hierarchy(f);
} // item_class::get_removed_fields_names_in_hierarchy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get all the names of the fields of this class and its super classes.
 * \param f (out) The names of the fields.
 */
void bf::item_class::get_all_field_names_in_hierarchy
( std::list<std::string>& f ) const
{
  std::transform( m_field.begin(), m_field.end(), std::front_inserter(f),
                  claw::const_pair_first<field_map_type::value_type>() );

  const_super_class_iterator it;

  for ( it=super_class_begin(); it!=super_class_end(); ++it )
    it->get_all_field_names_in_hierarchy(f);
} // item_class::get_all_field_names_in_hierarchy()
