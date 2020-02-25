/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::action_modify_action class.
 * \author S�bastien Angibaud
 */
#include "bf/history/action_modify_mark.hpp"

#include "bf/gui_model.hpp"
#include "bf/mark.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param m The mark.
 * \param label The new label of the mark.
 * \param animation The new animation of the mark.
 * \param apply_angle_to_animation The new angle application statut.
 * \param pause_hidden Tells if the animation must be paused when the mark is
 *        hidden.
 * \param reset_with_action Tells if the animation must be reset when the action
 *        starts.
 */
bf::action_modify_mark::action_modify_mark
( mark* m, const std::string& label, const bf::any_animation& animation,
  bool apply_angle_to_animation, bool pause_hidden, bool reset_with_action )
  : m_mark(m), m_label(label), m_animation(animation),
    m_apply_angle_to_animation(apply_angle_to_animation),
    m_pause_animation_when_hidden(pause_hidden),
    m_reset_animation_with_action(reset_with_action)
{

} // action_modify_mark::action_modify_mark()

/*----------------------------------------------------------------------------*/
/**
 * \remark Calling execute() two times will restore the initial size.
 */
void bf::action_modify_mark::execute( gui_model& mdl )
{
  const std::string label( m_mark->get_label() );
  const bf::any_animation a( m_mark->get_animation() );
  const bool apply_angle_to_animation( m_mark->apply_angle_to_animation() );
  const bool pause_animation_when_hidden
    ( m_mark->pause_animation_when_hidden() );
  const bool reset_animation_with_action
    ( m_mark->reset_animation_with_action() );

  m_mark->set_label( m_label );
  m_mark->set_animation( m_animation );
  m_mark->apply_angle_to_animation( m_apply_angle_to_animation );
  m_mark->pause_animation_when_hidden( m_pause_animation_when_hidden );
  m_mark->reset_animation_with_action( m_reset_animation_with_action );

  m_label = label;
  m_animation = a;
  m_apply_angle_to_animation = apply_angle_to_animation;
  m_pause_animation_when_hidden = pause_animation_when_hidden;
  m_reset_animation_with_action = reset_animation_with_action;
} // action_modify_mark::execute()

/*----------------------------------------------------------------------------*/
void bf::action_modify_mark::undo( gui_model& mdl )
{
  /* the first call to execute saved the initial label and animation. */
  execute(mdl);
} // action_modify_mark::undo()

/*----------------------------------------------------------------------------*/
bool bf::action_modify_mark::is_identity( const gui_model& mdl ) const
{
  return (m_mark->get_label() == m_label)
    && (m_mark->get_animation() == m_animation )
    && (m_mark->apply_angle_to_animation() == m_apply_angle_to_animation)
    && (m_mark->pause_animation_when_hidden() == m_pause_animation_when_hidden)
    && (m_mark->reset_animation_with_action() == m_reset_animation_with_action);
} // action_modify_mark::is_identity()

/*----------------------------------------------------------------------------*/
wxString bf::action_modify_mark::get_description() const
{
  return _("Set mark properties");
} // action_modify_mark::get_description()
