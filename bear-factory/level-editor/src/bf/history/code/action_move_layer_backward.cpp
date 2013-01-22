/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_move_layer_backward class.
 * \author Julien Jorge
 */
#include "bf/history/action_move_layer_backward.hpp"

#include "bf/gui_level.hpp"
#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_move_layer_backward::action_move_layer_backward( unsigned int index )
  : m_original_index(index)
{

} // action_move_layer_backward::action_move_layer_backward()

/*----------------------------------------------------------------------------*/
void bf::action_move_layer_backward::execute( gui_level& lvl )
{
  lvl.move_backward(m_original_index);
} // action_move_layer_backward::execute()

/*----------------------------------------------------------------------------*/
void bf::action_move_layer_backward::undo( gui_level& lvl )
{
  lvl.move_forward(m_original_index-1);
} // action_move_layer_backward::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_move_layer_backward::is_identity( const gui_level& lvl ) const
{
  return m_original_index == 0;
} // action_move_layer_backward::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_move_layer_backward::get_description() const
{
  return wxString::Format(_("Move layer %d backward"), m_original_index);
} // action_move_layer_backward::get_description()
