/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_set_placement_function class.
 * \author Julien Jorge
 */
#include "bf/history/action_set_placement_function.hpp"

#include "bf/gui_model.hpp"
#include "bf/snapshot.hpp"
#include "bf/mark.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param s The considered snapshot.
 * \param m The considered mark.
 * \param f The new collision function of the mark.
 */
bf::action_set_placement_function::action_set_placement_function
( snapshot* s, const mark* m, const std::string& f )
  : m_snapshot(s), m_mark(m), m_function(f)
{

} // action_set_placement_function::action_set_placement_function()

/*----------------------------------------------------------------------------*/
void bf::action_set_placement_function::execute( gui_model& mdl )
{
  CLAW_PRECOND( m_mark != NULL );
  CLAW_PRECOND( m_snapshot != NULL );
  CLAW_PRECOND( m_snapshot->has_mark(m_mark) );

  mark_placement& p = m_snapshot->get_placement(m_mark);

  std::string f(p.get_collision_function());
  p.set_collision_function( m_function );
  m_function.swap(f);
} // action_set_placement_function::execute()

/*----------------------------------------------------------------------------*/
void bf::action_set_placement_function::undo( gui_model& mdl )
{
  /* the first call to execute saved the initial label and animation. */
  execute(mdl);
} // action_set_placement_function::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_set_placement_function::is_identity
( const gui_model& mdl ) const
{
  const mark_placement& p = m_snapshot->get_placement(m_mark);

  return p.get_collision_function() == m_function;
} // action_set_placement_function::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_set_placement_function::get_description() const
{
  return _("Set collision function");
} // action_set_placement_function::get_description()
