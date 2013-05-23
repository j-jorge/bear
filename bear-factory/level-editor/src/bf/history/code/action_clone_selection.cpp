/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_clone_selection class.
 * \author Julien Jorge
 */
#include "bf/history/action_clone_selection.hpp"

#include "bf/item_instance.hpp"
#include "bf/gui_level.hpp"
#include "bf/history/action_add_item.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a group action that applies this action for several layers.
 * \param lvl The level in which we take the selection.
 * \param layers The indices of the layers whose selection is cloned.
 * \param x_count Count of clones on the x-axis.
 * \param y_count Count of clones on the y-axis.
 * \param x_offset Offset on the x-axis.
 * \param y_offset Offset on the y-axis.
 * \param add Add the clones in the selection.
 */
bf::action_group*
bf::action_clone_selection::create_for_layers
( const gui_level& lvl, std::vector<std::size_t> layers,
  unsigned int x_count, unsigned int y_count, double x_offset, double y_offset,
  bool add )
{
  action_group* result( new action_group( _("Clone selected items") ) );

  for ( std::size_t i(0); i != layers.size(); ++i )
    result->add_action
      ( new action_clone_selection
        ( lvl, layers[i], x_count, y_count, x_offset, y_offset, add ) );

  return result;
} // action_clone_selection::create_for_layers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param lvl The level in which we take the selection.
 * \param layer_index The index of the layer whose selection is cloned.
 * \param x_count Count of clones on the x-axis.
 * \param y_count Count of clones on the y-axis.
 * \param x_offset Offset on the x-axis.
 * \param y_offset Offset on the y-axis.
 * \param add Add the clones in the selection.
 */
bf::action_clone_selection::action_clone_selection
( const gui_level& lvl, std::size_t layer_index, unsigned int x_count,
  unsigned int y_count, double x_offset, double y_offset, bool add )
  : m_layer( layer_index )
{
  const item_selection& selection( lvl.get_selection( layer_index ) );

  for ( item_selection::const_iterator it( selection.begin() );
        it != selection.end(); ++it )
    {
      double xo(x_offset);
      double yo(y_offset);

      if (xo < 0)
        xo -= 2 * (*it)->get_rendering_parameters().get_width();

      if (yo < 0)
        yo -= 2 * (*it)->get_rendering_parameters().get_height();

      clone_item(**it, x_count, y_count, xo, yo);
    }

  lvl.fix_identifiers
    ( std::vector<item_instance*>( m_new_items.begin(), m_new_items.end() ) );

  if (!add)
    m_new_items.clear();
} // action_clone_selection::action_clone_selection()

/*----------------------------------------------------------------------------*/
void bf::action_clone_selection::execute( gui_level& lvl )
{
  action_group::execute(lvl);

  lvl.add_to_selection( m_new_items );
} // action_clone_selection::execute()

/*----------------------------------------------------------------------------*/
void bf::action_clone_selection::undo( gui_level& lvl )
{
  lvl.remove_from_selection( m_new_items );

  action_group::undo(lvl);
} // action_clone_selection::undo()

/*----------------------------------------------------------------------------*/
wxString bf::action_clone_selection::get_description() const
{
  return _("Clone selected items");
} // action_clone_selection::get_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clone an item.
 * \param item The item to clone.
 * \param x_count Count of clones on the x-axis.
 * \param y_count Count of clones on the y-axis.
 * \param x_offset Offset on the x-axis.
 * \param y_offset Offset on the y-axis.
 */
void bf::action_clone_selection::clone_item
( const item_instance& item, unsigned int x_count, unsigned int y_count,
  double x_offset, double y_offset )
{
  double x = item.get_rendering_parameters().get_left()
    + item.get_rendering_parameters().get_width() + x_offset;
  double y = item.get_rendering_parameters().get_bottom();

  // fill the first line
  for (unsigned int x_copy=1; x_copy<x_count; ++x_copy)
    {
      item_instance* it = new item_instance(item);
      it->get_rendering_parameters().set_position(x, y);

      add_action( new action_add_item( it, m_layer ) );

      m_new_items.insert(it);
      x += item.get_rendering_parameters().get_width() + x_offset;
    }

  // then, fill a rectangle
  for (unsigned int y_copy=1; y_copy<y_count; ++y_copy)
    {
      y += item.get_rendering_parameters().get_height() + y_offset;
      x = item.get_rendering_parameters().get_left();

      for (unsigned int x_copy=0; x_copy!=x_count; ++x_copy)
        {
          item_instance* it = new item_instance(item);
          it->get_rendering_parameters().set_position(x, y);

          add_action( new action_add_item( it, m_layer ) );

          m_new_items.insert(it);
          x += item.get_rendering_parameters().get_width() + x_offset;
        }
    }
} // action_clone_selection::clone_item()
