/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_set_item_bottom class.
 * \author Julien Jorge
 */
#include "bf/history/action_set_item_bottom.hpp"

#include "bf/item_instance.hpp"

#include <wx/intl.h>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
bf::action_set_item_bottom::action_set_item_bottom
( item_instance* item, double p )
  : m_item(item), m_position(p)
{
  CLAW_PRECOND( item!=NULL );
} // action_set_item_bottom::action_set_item_bottom()

/*----------------------------------------------------------------------------*/
void bf::action_set_item_bottom::execute( gui_level& lvl )
{
  CLAW_PRECOND( m_item!=NULL );

  const double p( m_item->get_rendering_parameters().get_bottom() );
  m_item->get_rendering_parameters().set_bottom(m_position);
  m_position = p;
} // action_set_item_bottom::execute()

/*----------------------------------------------------------------------------*/
void bf::action_set_item_bottom::undo( gui_level& lvl )
{
  CLAW_PRECOND( m_item!=NULL );

  /* the first call to execute saved the initial position. */
  execute(lvl);
} // action_set_item_bottom::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_set_item_bottom::is_identity( const gui_level& gui ) const
{
  CLAW_PRECOND( m_item!=NULL );

  return m_position == m_item->get_rendering_parameters().get_bottom();
} // action_set_item_bottom::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_set_item_bottom::get_description() const
{
  return _("Set item bottom position");
} // action_set_item_bottom::get_description()
