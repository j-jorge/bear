/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_set_x_easing class.
 * \author Julien Jorge
 */
#include "bf/history/action_set_x_easing.hpp"

#include "bf/gui_model.hpp"
#include "bf/snapshot.hpp"
#include "bf/mark.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param s The considered snapshot.
 * \param m The considered mark.
 * \param e The easing function.
 */
bf::action_set_x_easing::action_set_x_easing
( snapshot* s, const mark* m, const bear::easing& e )
  : m_snapshot(s), m_mark(m), m_easing(e)
{

} // action_set_x_easing::action_set_x_easing()

/*----------------------------------------------------------------------------*/
void bf::action_set_x_easing::execute( gui_model& mdl )
{
  CLAW_PRECOND( m_mark != NULL );
  CLAW_PRECOND( m_snapshot != NULL );
  CLAW_PRECOND( m_snapshot->has_mark(m_mark) );

  mark_placement& p = m_snapshot->get_placement(m_mark);

  const bear::easing e(p.get_x_easing());

  p.set_x_easing(m_easing);

  m_easing = e;
} // action_set_x_easing::execute()

/*----------------------------------------------------------------------------*/
void bf::action_set_x_easing::undo( gui_model& mdl )
{
  /* the first call to execute saved the initial label and animation. */
  execute(mdl);
} // action_set_x_easing::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_set_x_easing::is_identity
( const gui_model& mdl ) const
{
  const mark_placement& p = m_snapshot->get_placement(m_mark);

  return p.get_x_easing() == m_easing;
} // action_set_x_easing::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_set_x_easing::get_description() const
{
  return _("Set easing function");
} // action_set_x_easing::get_description()
