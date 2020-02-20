/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::windows_layout class.
 * \author Julien Jorge
 */
#include "bf/windows_layout.hpp"

#include "bf/main_frame.hpp"
#include "bf/model_frame.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param mf The main window.
 */
bf::windows_layout::windows_layout( main_frame& mf )
  : m_main_frame(mf), m_current_model_frame(NULL)
{

} // windows_layout::windows_layout()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the main frame.
 */
wxWindow* bf::windows_layout::get_root_window() const
{
  return &m_main_frame;
} // windows_layout::get_root_window()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current model view.
 */
bf::model_frame* bf::windows_layout::get_current_model_frame()
{
  return m_current_model_frame;
} // windows_layout::get_current_model_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the current model.
 */
bool bf::windows_layout::set_current_model_frame( model_frame& m )
{
  bool result = true;

  // the close event can happen before the activate event
  if ( m_model_frame.find(&m) != m_model_frame.end() )
    {
      m_current_model_frame = &m;
      m_main_frame.set_active_model(m_current_model_frame);
    }
  else
    result = false;

  return result;
} // windows_layout::set_current_model_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a model view.
 * \param m The view to add.
 */
void bf::windows_layout::add_model_frame( model_frame& m )
{
  if ( m_model_frame.empty() )
    m_main_frame.set_active_model(&m);

  m_model_frame.insert(&m);
} // windows_layout::add_model_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a model view.
 * \param m The view to remove.
 */
void bf::windows_layout::remove_model_frame( model_frame& m )
{
  m_model_frame.erase(&m);

  if (m_current_model_frame == &m)
    {
      m_current_model_frame = NULL;
      m_main_frame.set_active_model(m_current_model_frame);
    }
} // windows_layout::remove_model_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update windows when the current action is modified.
 */
void bf::windows_layout::update_action()
{
  m_main_frame.update_action();
} // windows_layout::update_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update windows when the current snapshot is modified.
 */
void bf::windows_layout::update_snapshot()
{
  m_main_frame.update_snapshot();
} // windows_layout::update_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update windows when the current mark is modified.
 */
void bf::windows_layout::update_mark()
{
  m_main_frame.update_mark();
} // windows_layout::update_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the list of model view.
 */
bf::windows_layout::iterator bf::windows_layout::begin()
{
  return iterator( m_model_frame.begin() );
} // windows_layout::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the end of the list of model view.
 */
bf::windows_layout::iterator bf::windows_layout::end()
{
  return iterator( m_model_frame.end() );
} // windows_layout::end()
