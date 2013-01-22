/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_move_selection class.
 * \author Julien Jorge
 */
#include "bf/history/action_move_selection.hpp"

#include "bf/gui_level.hpp"
#include "bf/history/action_set_item_position.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_move_selection::action_move_selection
( const gui_level& lvl, double dx, double dy )
{
  if ( !lvl.empty() )
    if ( lvl.has_selection() )
      {
        item_selection::const_iterator it;
        const item_selection& selection( lvl.get_selection() );

        for (it=selection.begin(); it!=selection.end(); ++it)
          add_action
            ( new action_set_item_position
              ( *it,
                (*it)->get_rendering_parameters().get_left() + dx,
                (*it)->get_rendering_parameters().get_bottom() + dy ) );

      }
} // action_move_selection::action_move_selection()

/*----------------------------------------------------------------------------*/
wxString bf::action_move_selection::get_description() const
{
  return _("Move selected items");
} // action_move_selection::get_description()
