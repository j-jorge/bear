/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_delete_selection class.
 * \author Julien Jorge
 */
#include "bf/history/action_delete_selection.hpp"

#include "bf/gui_level.hpp"
#include "bf/history/action_remove_item.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs the action to remove the items selected in a given
 *        collection of layers.
 * \param lvl The level from which the items are removed.
 * \param layers The layers from the selection of which the items are taken.
 */
bf::action_delete_selection::action_delete_selection
( const gui_level& lvl, std::vector<std::size_t> layers )
  : action_group( _("Delete selected items") )
{
  for ( std::size_t i(0); i!=layers.size(); ++i )
    add_actions_for_layer( lvl, layers[i] );
} // action_delete_selection::action_delete_selection()


/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the actions that remove the items selected in a given layer.
 * \param lvl The level from which the items are removed.
 * \param layer_index The index of the  layer from the selection of which the
 *        items are taken.
 */
void bf::action_delete_selection::add_actions_for_layer
( const gui_level& lvl, std::size_t layer_index )
{
  const item_selection& selection( lvl.get_selection( layer_index ) );

  for ( item_selection::const_iterator it( selection.begin() );
        it != selection.end(); ++it )
    add_action( new action_remove_item( *it, layer_index ) );
} // action_delete_selection::add_actions_for_layer()
