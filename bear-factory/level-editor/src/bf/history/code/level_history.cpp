/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::level_history class.
 * \author Julien Jorge
 */
#include "bf/history/level_history.hpp"

#include "bf/gui_level.hpp"
#include "bf/history/action_move_selection.hpp"
#include "bf/history/action_rotate_selection.hpp"
#include "bf/history/level_action.hpp"

#include <claw/assert.hpp>
#include <limits>
#include <wx/datetime.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param lvl The level on which we work, will be deleted in the desctructor.
 */
bf::level_history::level_history( gui_level* lvl )
  : m_level(lvl), m_max_history( std::numeric_limits<std::size_t>::max() ),
    m_saved_action(NULL), m_compiled_action(NULL), m_last_selection_move(NULL),
    m_last_selection_move_date(0)
{
  CLAW_PRECOND(lvl != NULL);
} // level_history::level_history()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::level_history::~level_history()
{
  clear_past();
  clear_future();
  delete m_level;
} // level_history::~level_history()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is undo information.
 */
bool bf::level_history::can_undo() const
{
  return !m_past.empty();
} // level_history::can_undo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is redo information.
 */
bool bf::level_history::can_redo() const
{
  return !m_future.empty();
} // level_history::can_redo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Undo the last change.
 */
void bf::level_history::undo()
{
  if ( !m_past.empty() )
    {
      level_action* action = m_past.back();
      m_past.pop_back();

      m_last_selection_move = NULL;
      m_last_selection_rotate = NULL;

      action->undo(*m_level);

      m_future.push_front( action );
    }
} // level_history::undo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Revert the last change.
 */
void bf::level_history::revert()
{
  if ( !m_past.empty() )
    {
      undo();
      clear_future();
    }
} // level_history::revert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Redo the last change.
 */
void bf::level_history::redo()
{
  if ( !m_future.empty() )
    {
      level_action* action = m_future.front();
      m_future.pop_front();

      action->execute( *m_level );

      m_past.push_back( action );
    }
} // level_history::redo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do an action on the level.
 * \param action The action to do.
 */
bool bf::level_history::do_action( level_action* action )
{
  m_last_selection_move = NULL;
  m_last_selection_rotate = NULL;

  if ( !action->is_identity(*m_level) )
    {
      clear_future();
      action->execute( *m_level );
      push_action(action);

      return true;
    }
  else
    {
      delete action;
      return false;
    }
} // level_history::do_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the action of moving the current selection in the level.
 * \param action The action to do.
 */
bool bf::level_history::do_action( action_move_selection* action )
{
  m_last_selection_rotate = NULL;

  if ( !action->is_identity(*m_level) )
    {
      clear_future();
      action->execute( *m_level );

      const bool same_selection =
        ( m_last_selection_move != NULL )
        && ( m_last_selection_move->get_selection().same_group_than
             ( action->get_selection() ) );

      // merge if the elapsed time since the last move is lower than 2 seconds
      if ( same_selection
           && (wxDateTime::GetTimeNow() - m_last_selection_move_date < 2) )
        {
          m_last_selection_move->move(*action);
          delete action;
        }
      else
        {
          m_last_selection_move = action;
          push_action(action);
        }

      m_last_selection_move_date = wxDateTime::GetTimeNow();
      return true;
    }
  else
    {
      delete action;
      return false;
    }
} // level_history::do_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the action of rotating the current selection in the level.
 * \param action The action to do.
 */
bool bf::level_history::do_action( action_rotate_selection* action )
{
  m_last_selection_move = NULL;

  if ( !action->is_identity(*m_level) )
    {
      clear_future();
      action->execute( *m_level );

      const bool same_selection =
        ( m_last_selection_rotate != NULL )
        && ( m_last_selection_rotate->get_selection().same_group_than
             ( action->get_selection() ) );

      // merge if the elapsed time since the last rotate is lower than 2 seconds
      if ( same_selection
           && (wxDateTime::GetTimeNow() - m_last_selection_rotate_date < 2) )
        {
          m_last_selection_rotate->move(*action);
          delete action;
        }
      else
        {
          m_last_selection_rotate = action;
          push_action(action);
        }

      m_last_selection_rotate_date = wxDateTime::GetTimeNow();
      return true;
    }
  else
    {
      delete action;
      return false;
    }
} // level_history::do_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the description of the most recent action in the past.
 */
wxString bf::level_history::get_undo_description() const
{
  CLAW_PRECOND( can_undo() );

  return m_past.back()->get_description();
} // level_history::get_undo_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the description of the most recent action in the future.
 */
wxString bf::level_history::get_redo_description() const
{
  CLAW_PRECOND( can_redo() );

  return m_future.front()->get_description();
} // level_history::get_undo_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mark the current level as saved.
 */
void bf::level_history::set_saved()
{
  if ( !m_past.empty() )
    m_saved_action = m_past.back();
} // level_history::set_saved()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we are on a saved state.
 */
bool bf::level_history::level_is_saved() const
{
  if ( !m_past.empty() )
    return m_saved_action == m_past.back();
  else
    return ( m_saved_action == NULL );
} // level_history::level_is_saved()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mark the current level as compiled.
 */
void bf::level_history::set_compiled()
{
  if ( !m_past.empty() )
    m_compiled_action = m_past.back();
} // level_history::set_compiled()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we are on a compiled state.
 */
bool bf::level_history::level_is_compiled() const
{
  if ( !m_past.empty() )
    return m_compiled_action == m_past.back();
  else
    return ( m_compiled_action == NULL );
} // level_history::level_is_compiled()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the level in the current state.
 */
bf::gui_level& bf::level_history::get_level()
{
  return *m_level;
} // level_history::get_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the level in the current state.
 */
const bf::gui_level& bf::level_history::get_level() const
{
  return *m_level;
} // level_history::get_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all instances of the past.
 */
void bf::level_history::clear_past()
{
  for ( ; !m_past.empty(); m_past.pop_front() )
    delete m_past.front();
} // level_history::clear_past()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all instances of the future.
 */
void bf::level_history::clear_future()
{
  for ( ; !m_future.empty(); m_future.pop_front() )
    delete m_future.front();
} // level_history::clear_future()

/*----------------------------------------------------------------------------*/
/**
 * \brief Push an action in the past.
 * \param action The action to push.
 */
void bf::level_history::push_action( level_action* action )
{
  if ( m_past.size() == m_max_history )
    {
      if ( m_max_history == 0 )
        delete action;
      else
        {
          if ( m_past.front() == m_saved_action )
            m_saved_action = NULL;

          if ( m_past.front() == m_compiled_action )
            m_compiled_action = NULL;

          delete m_past.front();
          m_past.push_back(action);
        }
    }
  else
    m_past.push_back(action);
} // level_history::push_action()
