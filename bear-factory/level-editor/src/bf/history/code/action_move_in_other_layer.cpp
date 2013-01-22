/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_move_in_other_layer class.
 * \author Sébastien Angibaud
 */
#include "bf/history/action_move_in_other_layer.hpp"

#include "bf/gui_level.hpp"
#include "bf/history/action_add_item.hpp"
#include "bf/history/action_remove_item.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_move_in_other_layer::action_move_in_other_layer
( const gui_level& lvl, unsigned int layer )
{
  if ( !lvl.empty() )
    if ( lvl.has_selection() )
      {
        m_new_layer = lvl.get_active_layer_index();

        if ( ( m_new_layer != layer ) && ( layer < lvl.layers_count() ) )
          {
	    m_last_layer = layer;
            item_selection::const_iterator it;
            const item_selection& selection( lvl.get_selection() );

            for (it=selection.begin(); it!=selection.end(); ++it)
              add_action( new action_remove_item( *it, m_new_layer ) );

            for (it=selection.begin(); it!=selection.end(); ++it)
              {
                item_instance* item = new item_instance(**it);
                add_action( new action_add_item( item, m_last_layer ) );
                m_new_items.insert
                  (item, (*it == selection.get_main_selection()) );
              }
          }
      }
} // action_move_in_other_layer::action_move_in_other_layer()

/*----------------------------------------------------------------------------*/
wxString bf::action_move_in_other_layer::get_description() const
{
  return _("Change the layer of the selection");
} // action_move_in_other_layer::get_description()

/*----------------------------------------------------------------------------*/
void bf::action_move_in_other_layer::execute( gui_level& lvl )
{
  CLAW_PRECOND( ( m_new_layer != m_last_layer ) && 
		( m_new_layer < lvl.layers_count() ) && 
		( m_last_layer < lvl.layers_count() ) );

  m_previous_items = lvl.get_selection(m_new_layer);
  m_previous_in_other_layer_items = lvl.get_selection(m_last_layer);

  action_group::execute(lvl);

  lvl.clear_selection(m_last_layer);
  lvl.add_to_selection(m_last_layer, m_new_items);
} // action_move_in_other_layer::execute()

/*----------------------------------------------------------------------------*/
void bf::action_move_in_other_layer::undo( gui_level& lvl )
{
  CLAW_PRECOND( ( m_new_layer != m_last_layer ) && 
		( m_new_layer < lvl.layers_count() ) && 
		( m_last_layer < lvl.layers_count() ) );

  action_group::undo(lvl);

  lvl.clear_selection(m_last_layer);
  lvl.add_to_selection(m_last_layer, m_previous_in_other_layer_items);
  lvl.clear_selection(m_new_layer);
  lvl.add_to_selection(m_new_layer, m_previous_items);
} // action_move_in_other_layer::undo()
