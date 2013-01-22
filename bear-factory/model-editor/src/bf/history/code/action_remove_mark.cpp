/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_remove_mark class.
 * \author Sébastien Angibaud
 */
#include "bf/history/action_remove_mark.hpp"

#include "bf/mark_placement.hpp"
#include "bf/snapshot.hpp"
#include "bf/gui_model.hpp"

#include <claw/assert.hpp>
#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
bf::action_remove_mark::action_remove_mark
( const std::string& action_name, mark* m)
  : m_mark(m), m_action_name(action_name), m_done(false)
{

} // action_remove_mark::action_remove_mark()

/*----------------------------------------------------------------------------*/
bf::action_remove_mark::~action_remove_mark()
{
  /* If the action has not been done, the action must be deleted here.
     Otherwise, it will be deleted by the model. */
  if ( m_done )
    delete m_mark;
} // action_remove_mark::~action_remove_mark()

/*----------------------------------------------------------------------------*/
void bf::action_remove_mark::execute( gui_model& mdl )
{
 CLAW_PRECOND( !m_done );
 CLAW_PRECOND( mdl.has_action(m_action_name) );
 CLAW_PRECOND( mdl.get_action(m_action_name).has_mark(m_mark) );

 action::const_snapshot_iterator it;
 for ( it = mdl.get_action(m_action_name).snapshot_begin();
       it != mdl.get_action(m_action_name).snapshot_end(); ++it )
   m_placements.push_back
     ( std::pair<double, mark_placement>
       (it->get_date(), it->get_placement(m_mark)));

 mdl.remove_mark( m_action_name, m_mark );

 m_done = true;
} // action_remove_mark::execute()

/*----------------------------------------------------------------------------*/
void bf::action_remove_mark::undo( gui_model& mdl )
{
  CLAW_PRECOND( m_done );
  CLAW_PRECOND( mdl.has_action(m_action_name) );
  CLAW_PRECOND( !mdl.get_action(m_action_name).has_mark(m_mark) );

  mdl.add_mark( m_action_name, m_mark );

  std::list< std::pair<double, mark_placement> >::const_iterator it;
  for ( it = m_placements.begin(); it != m_placements.end(); ++it )
    {
      CLAW_PRECOND(mdl.get_action(m_action_name).has_snapshot_date(it->first));
      mark_placement& p =
        mdl.get_action(m_action_name).get_snapshot_before_or_at_date
        (it->first)->get_placement( m_mark );

      p.set_position
        ( it->second.get_x_position(), it->second.get_y_position() );
      p.set_depth_position( it->second.get_depth_position() );
      p.set_angle( it->second.get_angle() );
    }

  m_placements.clear();

  m_done = false;
} // action_remove_mark::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_remove_mark::is_identity( const gui_model& mdl ) const
{
  return false;
} // action_remove_mark::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_remove_mark::get_description() const
{
  return _("Remove mark");
} // action_remove_mark::get_description()
