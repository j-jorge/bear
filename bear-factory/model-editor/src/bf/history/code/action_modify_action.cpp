/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_modify_action class.
 * \author Sébastien Angibaud
 */
#include "bf/history/action_modify_action.hpp"

#include "bf/gui_model.hpp"
#include "bf/action.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param a The action.
 * \param name The new name of the action.
 * \param duration The new duration of the action.
 * \param sound The new sound of the action.
 * \param auto_next The new next action.
 */
bf::action_modify_action::action_modify_action
( action* a, const std::string& name,
  double duration, const sound_description& sound,
  const std::string& auto_next)
  : m_action(a), m_name(name), m_duration(duration),
    m_sound(sound), m_auto_next(auto_next)
{

} // action_modify_action::action_modify_action()

/*----------------------------------------------------------------------------*/
bf::action_modify_action::~action_modify_action()
{
  std::list<snapshot*>::iterator it;
  for ( it = m_snapshots.begin(); it != m_snapshots.end(); ++it)
    delete *it;
} // action_modify_action::~action_modify_action()

/*----------------------------------------------------------------------------*/
void bf::action_modify_action::execute( gui_model& mdl )
{
  CLAW_PRECOND( m_snapshots.empty() );

  const std::string n(m_action->get_name());
  const double d(m_action->get_duration());
  const sound_description sound(m_action->get_sound());
  const std::string a(m_action->get_auto_next());

  m_action->get_all_snapshots_after_or_at_date( m_duration, m_snapshots );

  std::list<snapshot*>::iterator it;
  for ( it = m_snapshots.begin(); it != m_snapshots.end(); ++it)
    m_action->remove_snapshot(*it);

  m_action->set_name( m_name );
  m_action->set_duration( m_duration );
  m_action->set_sound(m_sound);
  m_action->set_auto_next(m_auto_next);

  m_name = n;
  m_duration = d;
  m_sound = sound;
  m_auto_next = a;
} // action_modify_action::execute()

/*----------------------------------------------------------------------------*/
void bf::action_modify_action::undo( gui_model& mdl )
{
  CLAW_PRECOND( mdl.has_action(m_action->get_name()) );

  const std::string n(m_action->get_name());
  const double d(m_action->get_duration());
  const sound_description sound(m_action->get_sound());
  const std::string a(m_action->get_auto_next());

  std::list<snapshot*>::iterator it;
  for ( it = m_snapshots.begin(); it != m_snapshots.end(); ++it)
    m_action->add_snapshot(*it);

  m_snapshots.clear();

  m_action->set_name( m_name );
  m_action->set_duration( m_duration );
  m_action->set_sound(m_sound);
  m_action->set_auto_next(m_auto_next);

  m_name = n;
  m_duration = d;
  m_sound = sound;
  m_auto_next = a;
} // action_modify_action::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_modify_action::is_identity( const gui_model& mdl ) const
{
  return (m_action->get_name() == m_name) &&
    (m_action->get_duration() == m_duration) &&
    (m_action->get_sound() == m_sound) &&
    (m_action->get_auto_next() == m_auto_next);
} // action_modify_action::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_modify_action::get_description() const
{
  return _("Set action properties");
} // action_modify_action::get_description()
