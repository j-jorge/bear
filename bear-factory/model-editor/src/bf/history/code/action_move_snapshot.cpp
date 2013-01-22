/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_move_snapshot class.
 * \author Sébastien Angibaud
 */
#include "bf/history/action_move_snapshot.hpp"

#include "bf/gui_model.hpp"
#include "bf/snapshot.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param s The snapshot.
 * \param date The new date of the snapshot.
 */
bf::action_move_snapshot::action_move_snapshot
( snapshot* s, double date )
  : m_snapshot(s), m_date(date)
{

} // action_move_snapshot::action_move_snapshot()

/*----------------------------------------------------------------------------*/
void bf::action_move_snapshot::execute( gui_model& mdl )
{
  double d(m_snapshot->get_date());

  m_snapshot->set_date( m_date );

  m_date = d;
} // action_move_snapshot::execute()

/*----------------------------------------------------------------------------*/
void bf::action_move_snapshot::undo( gui_model& mdl )
{
  /* the first call to execute saved the initial position. */
  execute(mdl);
} // action_move_snapshot::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_move_snapshot::is_identity( const gui_model& mdl ) const
{
  return ( m_snapshot->get_date() == m_date );
} // action_move_snapshot::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_move_snapshot::get_description() const
{
  return _("Move snapshot");
} // action_move_snapshot::get_description()
