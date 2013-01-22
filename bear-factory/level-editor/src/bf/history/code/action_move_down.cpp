/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_move_down class.
 * \author Sébastien Angibaud
 */
#include "bf/history/action_move_down.hpp"

#include "bf/gui_level.hpp"
#include "bf/history/action_add_item.hpp"
#include "bf/history/action_remove_item.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_move_down::action_move_down( const gui_level& lvl )
{
  if ( !lvl.empty() )
    if ( lvl.has_selection() )
      {
        m_layer = lvl.get_active_layer_index();

        if ( m_layer < lvl.layers_count()-1 )
          {
            item_selection::const_iterator it;
            const item_selection& selection( lvl.get_selection() );

            for (it=selection.begin(); it!=selection.end(); ++it)
              add_action( new action_remove_item( *it, m_layer ) );

            for (it=selection.begin(); it!=selection.end(); ++it)
              {
                item_instance* item = new item_instance(**it);
                add_action( new action_add_item( item, m_layer+1 ) );
                m_new_items.insert
                  (item, (*it == selection.get_main_selection()) );
              }
          }
      }
} // action_move_down::action_move_down()

/*----------------------------------------------------------------------------*/
wxString bf::action_move_down::get_description() const
{
  return _("Change the layer of the selection");
} // action_move_down::get_description()

/*----------------------------------------------------------------------------*/
void bf::action_move_down::execute( gui_level& lvl )
{
  CLAW_PRECOND( m_layer < lvl.layers_count() - 1 );

  m_previous_items = lvl.get_selection(m_layer);
  m_previous_down_items = lvl.get_selection(m_layer+1);

  action_group::execute(lvl);

  lvl.clear_selection(m_layer+1);
  lvl.add_to_selection(m_layer+1, m_new_items);
} // action_move_down::execute()

/*----------------------------------------------------------------------------*/
void bf::action_move_down::undo( gui_level& lvl )
{
  CLAW_PRECOND( m_layer < lvl.layers_count() - 1 );

  action_group::undo(lvl);

  lvl.clear_selection(m_layer+1);
  lvl.add_to_selection(m_layer+1, m_previous_down_items);
  lvl.clear_selection(m_layer);
  lvl.add_to_selection(m_layer, m_previous_items);
} // action_move_down::undo()
