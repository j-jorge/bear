/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_copy_selection class.
 * \author Julien Jorge
 */
#include "bf/history/action_copy_selection.hpp"

#include "bf/item_instance.hpp"
#include "bf/gui_level.hpp"
#include "bf/history/action_add_item.hpp"

#include <claw/assert.hpp>
#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a group of actions that copies the items selected in a
 *        collection of layers.
 * \param lvl The level in which we take the selection.
 * \param layers The indices of the layers whose selection is cloned.
 * \param dx The distance of the copy on the x-axis.
 * \param dy The distance of the copy on the y-axis.
 * \param add Tell if the copy has to be added to the current selection.
 */
bf::action_group*
bf::action_copy_selection::create_for_layers
( const gui_level& lvl, std::vector<std::size_t> layers, double dx, double dy,
  bool add )
{
  action_group* const result
    ( new action_group( _("Copy the selected items") ) );

  for ( std::size_t i(0); i != layers.size(); ++i )
    result->add_action
      ( new action_copy_selection( lvl, layers[i], dx, dy, add ) );

  return result;
} // action_copy_selection::create_for_layers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param lvl The level in which we take the selection.
 * \param layer_index The index of the layer whose selection is cloned.
 * \param dx The distance of the copy on the x-axis.
 * \param dy The distance of the copy on the y-axis.
 * \param add Tell if the copy has to be added to the current selection.
 */
bf::action_copy_selection::action_copy_selection
( const gui_level& lvl, std::size_t layer_index, double dx, double dy,
  bool add )
  : m_add_to_selection(add), m_layer(layer_index)
{
  const item_selection& selection( lvl.get_selection( layer_index ) );

  for (item_selection::const_iterator it( selection.begin() );
       it != selection.end(); ++it )
    {
      double x = (*it)->get_rendering_parameters().get_left() + dx;
      double y = (*it)->get_rendering_parameters().get_bottom() + dy;

      item_instance* item = new item_instance(**it);
      item->get_rendering_parameters().set_position(x, y);

      add_action( new action_add_item(item, m_layer) );

      m_new_items.insert(item, (*it == selection.get_main_selection()) );
    }

  lvl.fix_identifiers
    ( std::vector<item_instance*>( m_new_items.begin(), m_new_items.end() ) );
} // action_copy_selection::action_copy_selection()

/*----------------------------------------------------------------------------*/
void bf::action_copy_selection::execute( gui_level& lvl )
{
  CLAW_PRECOND( m_layer < lvl.layers_count() );

  m_previous_items = lvl.get_selection(m_layer);

  action_group::execute(lvl);

  if (!m_add_to_selection)
    lvl.clear_selection(m_layer);

  lvl.add_to_selection( m_new_items );
} // action_copy_selection::execute()

/*----------------------------------------------------------------------------*/
void bf::action_copy_selection::undo( gui_level& lvl )
{
  CLAW_PRECOND( m_layer < lvl.layers_count() );

  lvl.remove_from_selection( m_new_items );

  action_group::undo(lvl);

  lvl.add_to_selection( m_previous_items );
} // action_copy_selection::undo()

/*----------------------------------------------------------------------------*/
wxString bf::action_copy_selection::get_description() const
{
  return _("Copy selected items");
} // action_copy_selection::get_description()
