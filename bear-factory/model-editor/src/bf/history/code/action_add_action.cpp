/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_add_action class.
 * \author Sébastien Angibaud
 */
#include "bf/history/action_add_action.hpp"

#include "bf/gui_model.hpp"

#include <claw/assert.hpp>
#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_add_action::action_add_action
( action* a )
  : m_action(a), m_action_name(a->get_name())
{

} // action_add_action::action_add_action()

/*----------------------------------------------------------------------------*/
bf::action_add_action::~action_add_action()
{
  /* If the action has not been done, the action must be deleted here.
     Otherwise, it will be deleted by the model. */
  if ( m_action != NULL )
    delete m_action;
} // action_add_action::~action_add_action()

/*----------------------------------------------------------------------------*/
void bf::action_add_action::execute( gui_model& mdl )
{
  CLAW_PRECOND( m_action != NULL );
  CLAW_PRECOND( !mdl.has_action(m_action_name) );

  mdl.add_action( m_action );
  m_action = NULL;
} // action_add_action::execute()

/*----------------------------------------------------------------------------*/
void bf::action_add_action::undo( gui_model& mdl )
{
  CLAW_PRECOND( m_action == NULL );
  CLAW_PRECOND( mdl.has_action(m_action_name) );

  m_action = mdl.remove_action( m_action_name );
} // action_add_action::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_add_action::is_identity( const gui_model& mdl ) const
{
  return false;
} // action_add_action::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_add_action::get_description() const
{
  return _("Add action");
} // action_add_action::get_description()
