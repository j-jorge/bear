/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_arrange_selection_vertically class.
 * \author Julien Jorge
 */
#include "bf/history/action_arrange_selection_vertically.hpp"

#include "bf/arrange.hpp"
#include "bf/item_instance.hpp"
#include "bf/item_selection.hpp"
#include "bf/history/action_set_item_bottom.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param selection The items to align.
 */
bf::action_arrange_selection_vertically::action_arrange_selection_vertically
( const item_selection& selection )
  : action_group( _("Arrange the items vertically") )
{
  if ( selection.empty() )
    return;

  std::set<double> initial_position;
  item_selection::const_iterator it;

  for ( it=selection.begin(); it!=selection.end(); ++it )
    initial_position.insert
      ( (*it)->get_rendering_parameters().get_vertical_middle() );

  const std::map<double, double> old_to_new( arrange( initial_position ) );

  for ( it=selection.begin(); it!=selection.end(); ++it )
    {
      const double ref_pos =
        (*it)->get_rendering_parameters().get_vertical_middle();
      const double new_bottom =
        old_to_new.find(ref_pos)->second
        - (*it)->get_rendering_parameters().get_height() / 2;

      add_action( new action_set_item_bottom( *it, new_bottom ) );
    }
} // action_arrange_selection_…::action_arrange_selection_vertically()
