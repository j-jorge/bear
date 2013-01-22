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
bf::action_clone_selection::action_clone_selection
( const gui_level& lvl, unsigned int x_count, unsigned int y_count,
  double x_offset, double y_offset, bool add )
{
  if ( !lvl.empty() )
    if ( lvl.has_selection() )
      {
        m_layer = lvl.get_active_layer_index();

        item_selection::const_iterator it;
        const item_selection& selection( lvl.get_selection() );

        for (it=selection.begin(); it!=selection.end(); ++it)
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
          ( std::vector<item_instance*>
            (m_new_items.begin(), m_new_items.end()) );

        if (!add)
          m_new_items.clear();
      }
} // action_clone_selection::action_clone_selection()

/*----------------------------------------------------------------------------*/
void bf::action_clone_selection::execute( gui_level& lvl )
{
  action_group::execute(lvl);

  lvl.add_to_selection(m_layer, m_new_items);
} // action_clone_selection::execute()

/*----------------------------------------------------------------------------*/
void bf::action_clone_selection::undo( gui_level& lvl )
{
  lvl.remove_from_selection(m_layer, m_new_items);

  action_group::undo(lvl);
} // action_clone_selection::undo()

/*----------------------------------------------------------------------------*/
wxString bf::action_clone_selection::get_description() const
{
  return _("Clone selected items");
} // action_clone_selection::get_description()

/*----------------------------------------------------------------------------*/
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
