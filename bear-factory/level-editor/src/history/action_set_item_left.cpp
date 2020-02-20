/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_set_item_left class.
 * \author Julien Jorge
 */
#include "bf/history/action_set_item_left.hpp"

#include "bf/item_instance.hpp"

#include <claw/assert.hpp>

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_set_item_left::action_set_item_left( item_instance* item, double p )
  : m_item(item), m_position(p)
{
  CLAW_PRECOND( item!=NULL );
} // action_set_item_left::action_set_item_left()

/*----------------------------------------------------------------------------*/
void bf::action_set_item_left::execute( gui_level& lvl )
{
  CLAW_PRECOND( m_item!=NULL );

  const double p( m_item->get_rendering_parameters().get_left() );
  m_item->get_rendering_parameters().set_left(m_position);
  m_position = p;
} // action_set_item_left::execute()

/*----------------------------------------------------------------------------*/
void bf::action_set_item_left::undo( gui_level& lvl )
{
  CLAW_PRECOND( m_item!=NULL );

  /* the first call to execute saved the initial position. */
  execute(lvl);
} // action_set_item_left::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_set_item_left::is_identity( const gui_level& gui ) const
{
  CLAW_PRECOND( m_item!=NULL );

  return m_position == m_item->get_rendering_parameters().get_left();
} // action_set_item_left::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_set_item_left::get_description() const
{
  return _("Set item's left position");
} // action_set_item_left::get_description()
