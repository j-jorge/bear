/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::action_modify_snapshot_sound class.
 * \author Sébastien Angibaud
 */
#include "bf/history/action_modify_snapshot_sound.hpp"

#include "bf/gui_model.hpp"
#include "bf/action.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param s The snapshot.
 * \param sound The new sound of the snapshot.
 */
bf::action_modify_snapshot_sound::action_modify_snapshot_sound
( snapshot* s, const sound_description& sound )
  : m_snapshot(s), m_sound(sound)
{

} // action_modify_snapshot_sound::action_modify_snapshot_sound()

/*----------------------------------------------------------------------------*/
void bf::action_modify_snapshot_sound::execute( gui_model& mdl )
{
  const sound_description sound(m_snapshot->get_sound());

  m_snapshot->set_sound(m_sound);

  m_sound = sound;
} // action_modify_snapshot_sound::execute()

/*----------------------------------------------------------------------------*/
void bf::action_modify_snapshot_sound::undo( gui_model& mdl )
{
  const sound_description sound(m_snapshot->get_sound());

  m_snapshot->set_sound(m_sound);

  m_sound = sound;
} // action_modify_snapshot_sound::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_modify_snapshot_sound::is_identity
( const gui_model& mdl ) const
{
  return m_snapshot->get_sound() == m_sound;
} // action_modify_snapshot_sound::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_modify_snapshot_sound::get_description() const
{
  return _("Set snapshot sound");
} // action_modify_snapshot_sound::get_description()
