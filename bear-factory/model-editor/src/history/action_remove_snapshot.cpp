/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_remove_snapshot class.
 * \author Sébastien Angibaud
 */
#include "bf/history/action_remove_snapshot.hpp"

#include "bf/gui_model.hpp"

#include <claw/assert.hpp>
#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_remove_snapshot::action_remove_snapshot
( const std::string& action_name, snapshot* s)
  : m_snapshot(s), m_action_name(action_name), m_done(false)
{

} // action_remove_snapshot::action_remove_snapshot()

/*----------------------------------------------------------------------------*/
bf::action_remove_snapshot::~action_remove_snapshot()
{
  /* If the action has not been done, the action must be deleted here.
     Otherwise, it will be deleted by the model. */
  if ( m_done )
    delete m_snapshot;
} // action_remove_snapshot::~action_remove_snapshot()

/*----------------------------------------------------------------------------*/
void bf::action_remove_snapshot::execute( gui_model& mdl )
{
 CLAW_PRECOND( !m_done );
 CLAW_PRECOND( mdl.has_action(m_action_name) );
 CLAW_PRECOND( mdl.get_action(m_action_name).has_snapshot(m_snapshot) );

 mdl.remove_snapshot( m_action_name, m_snapshot );

 m_done = true;
} // action_remove_snapshot::execute()

/*----------------------------------------------------------------------------*/
void bf::action_remove_snapshot::undo( gui_model& mdl )
{
  CLAW_PRECOND( m_done );
  CLAW_PRECOND( mdl.has_action(m_action_name) );
  CLAW_PRECOND( !mdl.get_action(m_action_name).has_snapshot(m_snapshot) );

  mdl.add_snapshot( m_action_name, m_snapshot );

  m_done = false;
} // action_remove_snapshot::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_remove_snapshot::is_identity( const gui_model& mdl ) const
{
  return false;
} // action_remove_snapshot::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_remove_snapshot::get_description() const
{
  return _("Remove snapshot");
} // action_remove_snapshot::get_description()
