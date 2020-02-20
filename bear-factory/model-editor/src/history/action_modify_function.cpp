/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_modify_action class.
 * \author Sébastien Angibaud
 */
#include "bf/history/action_modify_function.hpp"

#include "bf/gui_model.hpp"
#include "bf/snapshot.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param s The snapshot.
 * \param function The new function of the snapshot.
 */
bf::action_modify_function::action_modify_function
( snapshot* s, const std::string& function )
  : m_snapshot(s), m_function(function)
{

} // action_modify_function::action_modify_function()

/*----------------------------------------------------------------------------*/
/**
 * \remark Calling execute() two times will restore the initial size.
 */
void bf::action_modify_function::execute( gui_model& mdl )
{
  const std::string f(m_snapshot->get_function());

  m_snapshot->set_function( m_function );

  m_function = f;
} // action_modify_function::execute()

/*----------------------------------------------------------------------------*/
void bf::action_modify_function::undo( gui_model& mdl )
{
  /* the first call to execute saved the initial label and animation. */
  execute(mdl);
} // action_modify_function::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_modify_function::is_identity( const gui_model& mdl ) const
{
  return ( m_snapshot->get_function() == m_function );
} // action_modify_function::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_modify_function::get_description() const
{
  return _("Set function called in a snapshot");
} // action_modify_function::get_description()
