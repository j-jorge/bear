/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
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
bf::action_copy_selection::action_copy_selection
( const gui_level& lvl, double dx, double dy, bool add )
  : m_add_to_selection(add)
{
  if ( !lvl.empty() )
    if ( lvl.has_selection() )
      {
        m_layer = lvl.get_active_layer_index();

        item_selection::const_iterator it;
        const item_selection& selection( lvl.get_selection() );

        for (it=selection.begin(); it!=selection.end(); ++it)
          {
            double x = (*it)->get_rendering_parameters().get_left() + dx;
            double y = (*it)->get_rendering_parameters().get_bottom() + dy;

            item_instance* item = new item_instance(**it);
            item->get_rendering_parameters().set_position(x, y);

            add_action( new action_add_item(item, m_layer) );

            m_new_items.insert(item, (*it == selection.get_main_selection()) );
          }

        lvl.fix_identifiers
          ( std::vector<item_instance*>
            (m_new_items.begin(), m_new_items.end()) );
      }
} // action_copy_selection::action_copy_selection()

/*----------------------------------------------------------------------------*/
void bf::action_copy_selection::execute( gui_level& lvl )
{
  CLAW_PRECOND( m_layer < lvl.layers_count() );

  m_previous_items = lvl.get_selection(m_layer);

  action_group::execute(lvl);

  if (!m_add_to_selection)
    lvl.clear_selection(m_layer);

  lvl.add_to_selection(m_layer, m_new_items);
} // action_copy_selection::execute()

/*----------------------------------------------------------------------------*/
void bf::action_copy_selection::undo( gui_level& lvl )
{
  CLAW_PRECOND( m_layer < lvl.layers_count() );

  lvl.remove_from_selection(m_layer, m_new_items);

  action_group::undo(lvl);

  lvl.add_to_selection(m_layer, m_previous_items);
} // action_copy_selection::undo()

/*----------------------------------------------------------------------------*/
wxString bf::action_copy_selection::get_description() const
{
  return _("Copy selected items");
} // action_copy_selection::get_description()
