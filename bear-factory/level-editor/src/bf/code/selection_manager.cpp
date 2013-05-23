/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::selection_manager class.
 * \author Julien Jorge
 */
#include "bf/selection_manager.hpp"

#include "bf/gui_level.hpp"
#include "bf/properties_frame.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs a new instance for a given level and a properties_frame.
 * \param The level in which we select the items.
 * \param The frame in which the properties of the items are displayed.
 */
bf::selection_manager::selection_manager
( gui_level& lvl, properties_frame& properties )
  : m_level( lvl ), m_properties( properties )
{

} // selection_manager::selection_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells to which items the operations are applied.
 */
bf::edit_mode bf::selection_manager::get_edit_mode() const
{
  return m_edit_mode;
} // selection_manager::get_edit_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells to which items the operations are applied.
 * \param m The new edit mode.
 */
void bf::selection_manager::set_edit_mode( edit_mode m )
{
  m_edit_mode = m;

  refresh_properties();
} // selection_manager::set_edit_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Refreshes the properties frame with the selected items.
 * \param m The new edit mode.
 */
void bf::selection_manager::refresh_properties() const
{
  m_properties.clear();

  const item_selection selection( m_edit_mode.get_selection( m_level ) );
  m_properties.add_items
    ( std::vector<item_instance*>( selection.begin(), selection.end() ) );
} // selection_manager::refresh_properties()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if there is an item selected.
 */
bool bf::selection_manager::has_selection() const
{
  return !m_edit_mode.get_selection( m_level ).empty();
} // selection_manager::has_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the selection to nothing.
 */
void bf::selection_manager::clear_selection()
{
  const std::vector<std::size_t> layers
    ( m_edit_mode.get_edit_layers( m_level ) );

  for ( std::size_t i(0); i!=layers.size(); ++i )
    m_level.clear_selection( layers[i] );

  m_properties.clear();
} // selection_manager::clear_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Selects all items.
 */
void bf::selection_manager::select_all()
{
  const std::vector<std::size_t> layers
    ( m_edit_mode.get_edit_layers( m_level ) );

  for ( std::size_t i(0); i!=layers.size(); ++i )
    {
      const std::size_t layer_index( layers[i] );
      const layer& the_layer( m_level.get_layer( layer_index ) );
      std::vector<item_instance*> selection;

      for ( layer::item_iterator it( the_layer.item_begin() );
            it != the_layer.item_end(); ++it )
        selection.push_back( &(*it) );
      
      add_selection( selection );
    }
} // selection_manager::select_all()

/*----------------------------------------------------------------------------*/
/**
 * \brief Changes the selection on an item.
 * \param item The item to process.
 */
void bf::selection_manager::toggle_selection( item_instance* item )
{
  if ( (item == NULL) || m_edit_mode.is_in_edited_layer( m_level, item ) )
    return;

  if ( m_level.item_is_selected( item ) )
    {
      m_level.remove_from_selection( item );
      m_properties.remove_item( item );
    }
  else
    {
      m_level.add_to_selection( item, true );
      m_properties.add_item( item );
    }
} // selection_manager::toggle_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds an item in the selection.
 * \param item The item to select.
 */
void bf::selection_manager::add_selection( item_instance* item )
{
  if ( m_edit_mode.is_in_edited_layer( m_level, item ) )
    return;

  m_level.add_to_selection(item, true);
  m_properties.add_item(item);
} // selection_manager::add_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds some items in the selection.
 * \param items The items to select.
 */
void bf::selection_manager::add_selection
( const std::vector<item_instance*>& items )
{
  std::vector<item_instance*> candidates;
  
  for ( std::vector<item_instance*>::const_iterator it( items.begin() );
        it != items.end(); ++it )
    if ( m_edit_mode.is_in_edited_layer( m_level, *it ) )
      candidates.push_back( *it );
  
  if ( candidates.empty() )
    return;

  for ( std::vector<item_instance*>::const_iterator it( candidates.begin() );
        it != candidates.end(); ++it )
    m_level.add_to_selection( *it );

  m_properties.add_items( candidates );
  m_level.add_to_selection( candidates.front(), true );
} // selection_manager::add_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the selected items.
 * \param item The items to select.
 */
void bf::selection_manager::set_selection
( const std::vector<item_instance*>& item )
{
  if ( item.empty() )
    clear_selection();
  else
    set_selection( item, item.front(), false );
} // selection_manager::set_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the selected items.
 * \param item The items to select.
 * \param selected Set this item as the selected item.
 * \param add Indicates if the items are added at the selection.
 */
void bf::selection_manager::set_selection
( const std::vector<item_instance*>& item, item_instance* selected, bool add )
{
  CLAW_PRECOND( std::find( item.begin(), item.end(), selected ) != item.end() );

  if ( !add )
    clear_selection();

  add_selection( item );
  m_level.add_to_selection( selected, true );
} // selection_manager::set_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the selected item.
 * \param item The item to select.
 */
void bf::selection_manager::set_selection( item_instance* item )
{
  if ( item == NULL )
    clear_selection();
  else
    {
      std::vector<item_instance*> item_list;
      item_list.push_back( item );
      set_selection( item_list );
    }
} // selection_manager::set_selection()
