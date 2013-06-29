/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_set_item_id class.
 * \author Julien Jorge
 */
#include "bf/history/action_set_item_id.hpp"

#include "bf/history/action_group.hpp"
#include "bf/history/action_set_item_field.hpp"

#include "bf/item_instance.hpp"
#include "bf/layer.hpp"
#include "bf/gui_level.hpp"
#include "bf/wx_facilities.hpp"

#include <wx/intl.h>
#include <claw/assert.hpp>
#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item for which the identifier changes.
 * \param id The new identifier.
 */
bf::action_set_item_id::action_set_item_id
( item_instance* item, std::string id )
  : m_id(id), m_layer_index(std::numeric_limits<unsigned int>::max()),
    m_clear_references_action(NULL)
{
  CLAW_PRECOND( item != NULL );

  m_item.push_back( item );
} // action_set_item_id::action_set_item_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param items The items for which the identifier changes.
 * \param id The new identifier.
 */
bf::action_set_item_id::action_set_item_id
( item_collection items, std::string id )
  : m_item(items), m_id(id),
    m_layer_index(std::numeric_limits<unsigned int>::max()),
    m_clear_references_action(NULL)
{

} // action_set_item_id::action_set_item_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::action_set_item_id::~action_set_item_id()
{
  delete m_clear_references_action;
} // action_set_item_id::~action_set_item_id()

/*----------------------------------------------------------------------------*/
void bf::action_set_item_id::execute( gui_level& lvl )
{
  CLAW_PRECOND( !m_item.empty() );

  if ( m_layer_index >= lvl.layers_count() )
    m_layer_index = lvl.get_layer_by_item( **m_item.begin() );

  drop_item_identifiers();

  if ( m_id.empty() )
    clear_identifiers( lvl );
  else
    rename_items( lvl );
} // action_set_item_id::execute()

/*----------------------------------------------------------------------------*/
void bf::action_set_item_id::undo( gui_level& lvl )
{
  CLAW_PRECOND( !m_item.empty() );

  if ( m_id.empty() )
    {
      m_clear_references_action->undo( lvl );
      delete m_clear_references_action;
      m_clear_references_action = NULL;
    }
  else
    {
      id_to_id_map id_map;

      for ( item_collection::const_iterator it=m_item.begin();
            it!=m_item.end(); ++it )
        id_map[ (*it)->get_id() ] = m_old_identifiers[ *it ];

      rename_item_references( lvl, id_map );
    }

  for ( item_collection::const_iterator it=m_item.begin();
        it!=m_item.end(); ++it )
    (*it)->set_id( m_old_identifiers[ *it ] );
} // action_set_item_id::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_set_item_id::is_identity( const gui_level& gui ) const
{
  return m_item.empty()
    || ( (m_item.size() == 1)
         && ( (*m_item.begin())->get_id() == m_id ) );
} // action_set_item_id::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_set_item_id::get_description() const
{
  if ( m_id.empty() )
    return _("Remove item identifier");
  else
    return
      wxString::Format
      ( _("Set item identifier to %s"), std_to_wx_string(m_id).c_str() );
} // action_set_item_id::get_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a generic description of the action.
 */
wxString bf::action_set_item_id::get_action_description()
{
  return _("Set item identifier");
} // action_set_item_id::get_action_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Saves the old identifiers for the undo and clear the items'
 * identifiers in order to avoid conflicts in the next calls to
 * lvl.generate_valid_id().
 */
void bf::action_set_item_id::drop_item_identifiers()
{
  m_old_identifiers.clear();

  for ( item_collection::const_iterator it=m_item.begin();
        it!=m_item.end(); ++it )
    {
      m_old_identifiers[ *it ] = (*it)->get_id();
      (*it)->set_id( std::string() );
    }
} // action_set_item_id::drop_item_identifiers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Removes all references to the renamed items from the fields of the
 *        items referencing them.
 * \param lvl The level from which the items are updated.
 */
void bf::action_set_item_id::clear_identifiers( gui_level& lvl )
{
  CLAW_PRECOND( m_id.empty() );
  CLAW_PRECOND( m_clear_references_action == NULL );

  m_clear_references_action = new action_group;

  layer::item_iterator it;
  const layer::item_iterator ite = lvl.get_layer(m_layer_index).item_end();

  for ( it = lvl.get_layer(m_layer_index).item_begin(); it != ite; ++it )
    create_clear_field_actions( *it );

  m_clear_references_action->execute( lvl );
} // action_set_item_id::clear_identifiers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds to m_clear_references_action the actions that clear the
 *        references to the items in m_item in a given item.
 * \param item The item whose fields are cleared.
 */
void
bf::action_set_item_id::create_clear_field_actions( item_instance& item ) const
{
  std::list<std::string> fields;
  std::list<std::string>::const_iterator it;

  item.get_class().get_field_names_in_hierarchy( fields );

  for ( it=fields.begin(); it!=fields.end(); ++it )
    {
      const type_field& f = item.get_class().get_field(*it);

      if( (f.get_field_type() == type_field::item_reference_field_type)
          && item.has_value(f) )
        {
          if ( f.is_list() )
            create_clear_list_field_actions( item, f.get_name() );
          else
            create_clear_field_actions( item, f.get_name() );
        }
    }
} // action_set_item_id::create_clear_field_actions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds to m_clear_references_action the actions that clear the
 *        references to the items in m_item from a field of a given item.
 * \param item The item whose fields are cleared.
 * \param field_name The name of the field to clear.
 */
void
bf::action_set_item_id::create_clear_field_actions
( item_instance& item, std::string field_name ) const
{
  item_reference_type field_value;
  item.get_value( field_name, field_value );

  for ( item_to_id_map::const_iterator it=m_old_identifiers.begin();
        it!=m_old_identifiers.end(); ++it )
    if ( field_value.get_value() == it->second )
      {
        m_clear_references_action->add_action
          ( new action_set_item_field<item_reference_type>
            ( &item, field_name ) );

        return; // There can be only one reference in non list fields.
      }
} // action_set_item_id::create_clear_field_actions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds to m_clear_references_action the actions that clear the
 *        references to the items in m_item from a field of a given item.
 * \param item The item whose fields are cleared.
 * \param field_name The name of the field to clear.
 */
void
bf::action_set_item_id::create_clear_list_field_actions
( item_instance& item, std::string field_name ) const
{
  typedef std::list<item_reference_type> value_type;

  value_type field_value;
  item.get_value( field_name, field_value );

  for ( item_to_id_map::const_iterator it=m_old_identifiers.begin();
        it!=m_old_identifiers.end(); ++it )
    {
      const std::string id( it->second );

      value_type::iterator value_it = field_value.begin();

      while( value_it != field_value.end() )
        if ( value_it->get_value() == id )
          {
            value_type::iterator tmp( value_it );
            ++value_it;
            field_value.erase( tmp );
          }
        else
          ++value_it;
    }

  if ( field_value.empty() )
    m_clear_references_action->add_action
      ( new action_set_item_field<value_type>( &item, field_name ) );
  else
    m_clear_references_action->add_action
      ( new action_set_item_field<value_type>
        ( &item, field_name, field_value ) );
} // action_set_item_id::create_clear_list_field_actions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Renames all the items and update all references to the renamed items
 *        from the fields of the items referencing them.
 * \param lvl The level from which the items are updated.
 */
void bf::action_set_item_id::rename_items( gui_level& lvl ) const
{
  CLAW_PRECOND( !m_id.empty() );

  id_to_id_map id_map;
  
  for ( item_collection::const_iterator it=m_item.begin();
        it!=m_item.end(); ++it )
    {
      std::string id( m_id );
      lvl.generate_valid_id(id);

      id_map[ m_old_identifiers.find( *it )->second ] = id;
      (*it)->set_id( id );
    }

  rename_item_references( lvl, id_map );
} // action_set_item_id::rename_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Restores the previous identifiers of the items.
 * \param lvl The level from which the items are updated.
 */
void bf::action_set_item_id::restore_identifiers( gui_level& lvl ) const
{
  CLAW_PRECOND( !m_id.empty() );

  std::map<std::string, std::string> id_map;
  
  for ( item_collection::const_iterator it=m_item.begin();
        it!=m_item.end(); ++it )
    id_map[ (*it)->get_id() ] = m_old_identifiers.find( *it )->second;

  rename_item_references( lvl, id_map );
} // action_set_item_id::restore_identifiers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Updates the references to the items in the fields of all item
 *        instances.
 * \param lvl The level from which the items are updated.
 * \param m The map associating the new identifier (as the value) with the
 *        current identifier (as the key).
 */
void
bf::action_set_item_id::rename_item_references
( gui_level& lvl, id_to_id_map m ) const
{
  layer::item_iterator it;
  const layer::item_iterator ite = lvl.get_layer(m_layer_index).item_end();

  for ( it = lvl.get_layer(m_layer_index).item_begin(); it != ite; ++it )
    it->rename_item_reference_fields(m);
} // action_set_item_id::restore_identifiers()
