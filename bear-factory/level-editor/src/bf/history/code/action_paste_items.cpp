/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_paste_items class.
 * \author Julien Jorge
 */
#include "bf/history/action_paste_items.hpp"

#include "bf/gui_level.hpp"
#include "bf/item_copy.hpp"
#include "bf/history/action_add_item.hpp"

#include <wx/intl.h>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
bf::action_paste_items::action_paste_items
( const gui_level& lvl, const item_copy& ref, double x, double y )
{
  if ( !ref.items.empty() && !lvl.empty() )
    {
      std::list<item_instance>::const_iterator it;

      m_layer = lvl.get_active_layer_index();

      for (it=ref.items.begin(); it!=ref.items.end(); ++it)
        {
          item_instance* item = new item_instance(*it);

          item->get_rendering_parameters().set_position
            ( x + (it->get_rendering_parameters().get_left() - ref.x),
              y + (it->get_rendering_parameters().get_bottom() - ref.y));

          add_action( new action_add_item( item, m_layer ) );

          if ( (it->get_rendering_parameters().get_left() == ref.x)
               && (it->get_rendering_parameters().get_bottom() == ref.y) )
            m_new_items.insert(item, true);
          else
            m_new_items.insert(item, false);
        }

      lvl.fix_identifiers
        ( std::vector<item_instance*>
          (m_new_items.begin(), m_new_items.end()) );
    }
} // action_paste_items::action_paste_items()

/*----------------------------------------------------------------------------*/
void bf::action_paste_items::execute( gui_level& lvl )
{
  CLAW_PRECOND( m_layer < lvl.layers_count() );

  m_previous_items = lvl.get_selection( m_layer );

  action_group::execute(lvl);

  lvl.clear_selection( m_layer );
  lvl.add_to_selection( m_new_items );
} // action_paste_items::execute()

/*----------------------------------------------------------------------------*/
void bf::action_paste_items::undo( gui_level& lvl )
{
  CLAW_PRECOND( m_layer < lvl.layers_count() );

  lvl.remove_from_selection( m_new_items );

  action_group::undo(lvl);

  lvl.add_to_selection( m_previous_items );
} // action_paste_items::undo()

/*----------------------------------------------------------------------------*/
wxString bf::action_paste_items::get_description() const
{
  return _("Paste the items in the layer");
} // action_paste_items::get_description()
