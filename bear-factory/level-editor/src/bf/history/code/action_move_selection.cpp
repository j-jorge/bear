/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_move_selection class.
 * \author Julien Jorge
 */
#include "bf/history/action_move_selection.hpp"

#include "bf/item_instance.hpp"
#include "bf/item_selection.hpp"
#include "bf/history/action_set_item_position.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param selection The items to move.
 * \param dx The movement on the x-axis.
 * \param dy The movement on the y-axis.
 */
bf::action_move_selection::action_move_selection
( const item_selection& selection, double dx, double dy )
  : action_group( _("Move the selected items") )
{
  m_selection = selection;

  for ( item_selection::const_iterator it( selection.begin() );
        it != selection.end(); ++it )
    add_action
      ( new action_set_item_position
        ( *it,
          (*it)->get_rendering_parameters().get_left() + dx,
          (*it)->get_rendering_parameters().get_bottom() + dy ) );
} // action_move_selection::action_move_selection()

/*----------------------------------------------------------------------------*/
/**
 * Gets the selection moved by this action.
 */
bf::item_selection bf::action_move_selection::get_selection() const
{
  return m_selection;
} // action_move_selection::get_selection()
