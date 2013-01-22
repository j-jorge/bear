/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_move_layer_forward class.
 * \author Julien Jorge
 */
#include "bf/history/action_move_layer_forward.hpp"

#include "bf/gui_level.hpp"
#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_move_layer_forward::action_move_layer_forward( unsigned int index )
  : m_original_index(index)
{

} // action_move_layer_forward::action_move_layer_forward()

/*----------------------------------------------------------------------------*/
void bf::action_move_layer_forward::execute( gui_level& lvl )
{
  lvl.move_forward(m_original_index);
} // action_move_layer_forward::execute()

/*----------------------------------------------------------------------------*/
void bf::action_move_layer_forward::undo( gui_level& lvl )
{
  lvl.move_backward(m_original_index+1);
} // action_move_layer_forward::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_move_layer_forward::is_identity( const gui_level& lvl ) const
{
  return m_original_index + 1 == lvl.layers_count();
} // action_move_layer_forward::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_move_layer_forward::get_description() const
{
  return wxString::Format(_("Move layer %d forward"), m_original_index);
} // action_move_layer_forward::get_description()
