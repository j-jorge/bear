/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_add_mark class.
 * \author Sébastien Angibaud
 */
#include "bf/history/action_add_mark.hpp"

#include "bf/gui_model.hpp"

#include <claw/assert.hpp>
#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_add_mark::action_add_mark
(  const std::string& action_name, mark* m )
  : m_action_name(action_name), m_mark(m), m_done(false)
{

} // action_add_mark::action_add_mark()

/*----------------------------------------------------------------------------*/
bf::action_add_mark::~action_add_mark()
{
  /* If the action has not been done, the mark must be deleted here. Otherwise,
     it will be deleted by the action. */
  if ( !m_done )
    delete m_mark;
} // action_add_mark::~action_add_mark()

/*----------------------------------------------------------------------------*/
void bf::action_add_mark::execute( gui_model& mdl )
{
  CLAW_PRECOND( !m_done );
  CLAW_PRECOND( mdl.has_action(m_action_name) );
  CLAW_PRECOND( !mdl.get_action(m_action_name).has_mark(m_mark) );

  mdl.add_mark( m_action_name, m_mark );
  m_done = true;
} // action_add_mark::execute()

/*----------------------------------------------------------------------------*/
void bf::action_add_mark::undo( gui_model& mdl )
{
  CLAW_PRECOND( m_done );
  CLAW_PRECOND( mdl.has_action(m_action_name) );
  CLAW_PRECOND( mdl.get_action(m_action_name).has_mark(m_mark) );

  mdl.remove_mark( m_action_name, m_mark );
  m_done = false;
} // action_add_mark::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_add_mark::is_identity( const gui_model& mdl ) const
{
  return false;
} // action_add_mark::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_add_mark::get_description() const
{
  return _("Add mark");
} // action_add_mark::get_description()
