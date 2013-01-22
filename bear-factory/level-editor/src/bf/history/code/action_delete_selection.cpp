/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
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
bf::action_delete_selection::action_delete_selection( const gui_level& lvl )
{
  if ( !lvl.empty() )
    if ( lvl.has_selection() )
      {
        item_selection::const_iterator it;
        const item_selection& selection( lvl.get_selection() );

        for (it=selection.begin(); it!=selection.end(); ++it)
          add_action
            ( new action_remove_item(*it, lvl.get_active_layer_index()) );
      }
} // action_delete_selection::action_delete_selection()

/*----------------------------------------------------------------------------*/
wxString bf::action_delete_selection::get_description() const
{
  return _("Delete selected items");
} // action_delete_selection::get_description()
