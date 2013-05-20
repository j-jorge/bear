/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::edit_mode class.
 * \author Julien Jorge
 */
#include "bf/edit_mode.hpp"

#include "bf/gui_level.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs a new instance initialized to apply the operations to the
 *        active layer.
 */
bf::edit_mode::edit_mode()
  : m_current_mode( active_layer )
{

} // edit_mode::edit_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells to which items the operations are applied.
 */
bf::edit_mode::value_type bf::edit_mode::get_value() const
{
  return m_current_mode;
} // edit_mode::get_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets to which items the operations are applied.
 * \param v The new edit mode.
 */
void bf::edit_mode::set_value( value_type v )
{
  m_current_mode = v;
} // edit_mode::set_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the items to which the operations must be done.
 * \param lvl The level from which the items are taken.
 */
bf::item_selection
bf::edit_mode::get_selection( const gui_level& lvl ) const
{
  return get_selections_by_layer_index
    ( lvl, get_edit_layers( lvl ), lvl.get_active_layer_index() );
} // edit_mode::get_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the selection of the active layer.
 * \param lvl The level from which the items are taken.
 */
std::vector<std::size_t>
bf::edit_mode::get_edit_layers( const gui_level& lvl ) const
{
  std::vector<std::size_t> result;

  switch( m_current_mode )
    {
    case active_layer:
      result.push_back( lvl.get_active_layer_index() );
      break;

    case layers_by_tag:
      result = get_layer_indices_by_tag( lvl );
      break;

    case all_layers:
      result = get_all_layers_indices( lvl );
    }

  return result;
} // edit_mode::get_edit_layers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the indices of all layers having the same tag than the active
 *        layer.
 * \param lvl The level from which the layers are taken.
 */
std::vector<std::size_t>
bf::edit_mode::get_layer_indices_by_tag( const gui_level& lvl ) const
{
  std::vector<std::size_t> layers;

  const std::string ref_tag( lvl.get_active_layer().get_tag() );

  for ( std::size_t i(0); i!=lvl.layers_count(); ++i )
    if ( lvl.get_layer( i ).get_tag() == ref_tag )
      layers.push_back( i );

  return layers;
} // edit_mode::get_layer_indices_by_tag()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the indices of all layers.
 * \param lvl The level from which the layers are taken.
 */
std::vector<std::size_t>
bf::edit_mode::get_all_layers_indices( const gui_level& lvl ) const
{
  std::vector<std::size_t> layers;

  for ( std::size_t i(0); i!=lvl.layers_count(); ++i )
    layers.push_back( i );

  return layers;
} // edit_mode::get_all_layers_indices()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the selection in several layers.
 * \param lvl The level from which the items are taken.
 * \param layers The indices of the layers.
 * \param main_index The index of the layer from which the main selection is
 *        taken. If there is no selection in this layer then the main selection
 *        is taken from the first layer having a selection.
 */
bf::item_selection bf::edit_mode::get_selections_by_layer_index
( const gui_level& lvl, std::vector<std::size_t> layers,
  std::size_t main_index ) const
{
  item_selection result;

  for ( std::size_t i(0); i!=layers.size(); ++i )
    result.insert( lvl.get_selection( layers[i] ) );

  // This will for the main selection to be the one of main_index, if this layer
  // has a selection.
  result.insert( lvl.get_selection( main_index ) );

  return result;
} // edit_mode::get_selections_by_layer_index()
