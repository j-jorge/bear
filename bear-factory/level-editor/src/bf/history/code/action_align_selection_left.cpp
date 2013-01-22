/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_align_selection_left class.
 * \author Julien Jorge
 */
#include "bf/history/action_align_selection_left.hpp"

#include "bf/gui_level.hpp"
#include "bf/history/action_set_item_left.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_align_selection_left::action_align_selection_left
( const gui_level& lvl )
{
  if ( !lvl.empty() )
    if ( lvl.has_selection() )
      {
        item_selection::const_iterator it;
        const item_selection& selection( lvl.get_selection() );
        const item_instance& main_item( *lvl.get_main_selection() );

        for (it=selection.begin(); it!=selection.end(); ++it)
          add_action
            ( new action_set_item_left
              ( *it, main_item.get_rendering_parameters().get_left() ) );
      }
} // action_align_selection_left::action_align_selection_left()

/*----------------------------------------------------------------------------*/
wxString bf::action_align_selection_left::get_description() const
{
  return _("Align the selected items");
} // action_align_selection_left::get_description()
