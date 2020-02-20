/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::model_history class.
 * \author Angibaud Sébastien
 */
#include "bf/history/model_history.hpp"

#include "bf/gui_model.hpp"
#include "bf/history/model_action.hpp"

#include <claw/assert.hpp>
#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param mdl The model on which we work, will be deleted in the desctructor.
 */
bf::model_history::model_history( gui_model* mdl )
  : m_model(mdl), m_max_history( std::numeric_limits<std::size_t>::max() ),
    m_saved_action(NULL)
{
  CLAW_PRECOND(mdl != NULL);
} // model_history::model_history()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::model_history::~model_history()
{
  clear_past();
  clear_future();
  delete m_model;
} // model_history::~model_history()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is undo information.
 */
bool bf::model_history::can_undo() const
{
  return !m_past.empty();
} // model_history::can_undo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is redo information.
 */
bool bf::model_history::can_redo() const
{
  return !m_future.empty();
} // model_history::can_redo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Undo the last change.
 */
void bf::model_history::undo()
{
  if ( !m_past.empty() )
    {
      model_action* action = m_past.back();
      m_past.pop_back();

      action->undo(*m_model);

      m_future.push_front( action );
    }
} // model_history::undo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Redo the last change.
 */
void bf::model_history::redo()
{
  if ( !m_future.empty() )
    {
      model_action* action = m_future.front();
      m_future.pop_front();

      action->execute( *m_model );

      m_past.push_back( action );
    }
} // model_history::redo()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do an action on the model.
 */
bool bf::model_history::do_action( model_action* action )
{
  if ( !action->is_identity(*m_model) )
    {
      clear_future();

      action->execute( *m_model );

      if ( m_past.size() == m_max_history )
        {
          if ( m_max_history == 0 )
            delete action;
          else
            {
              if ( m_past.front() == m_saved_action )
                m_saved_action = NULL;

              delete m_past.front();
              m_past.push_back(action);
            }
        }
      else
        m_past.push_back(action);

      return true;
    }
  else
    {
      delete action;
      return false;
    }
} // model_history::do_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the description of the most recent action in the past.
 */
wxString bf::model_history::get_undo_description() const
{
  CLAW_PRECOND( can_undo() );

  return m_past.back()->get_description();
} // model_history::get_undo_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the description of the most recent action in the future.
 */
wxString bf::model_history::get_redo_description() const
{
  CLAW_PRECOND( can_redo() );

  return m_future.front()->get_description();
} // model_history::get_undo_description()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mark the current model as saved.
 */
void bf::model_history::set_saved()
{
  if ( !m_past.empty() )
    m_saved_action = m_past.back();
} // model_history::set_saved()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if we are on a saved state.
 */
bool bf::model_history::model_is_saved() const
{
  if ( !m_past.empty() )
    return m_saved_action == m_past.back();
  else
    return false;
} // model_history::model_is_saved()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the model in the current state.
 */
bf::gui_model& bf::model_history::get_model()
{
  return *m_model;
} // model_history::get_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the model in the current state.
 */
const bf::gui_model& bf::model_history::get_model() const
{
  return *m_model;
} // model_history::get_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all instances of the past.
 */
void bf::model_history::clear_past()
{
  for ( ; !m_past.empty(); m_past.pop_front() )
    delete m_past.front();
} // model_history::clear_past()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all instances of the future.
 */
void bf::model_history::clear_future()
{
  for ( ; !m_future.empty(); m_future.pop_front() )
    delete m_future.front();
} // model_history::clear_future()

