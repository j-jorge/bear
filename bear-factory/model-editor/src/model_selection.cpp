/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::model_selection class.
 * \author Julien Jorge
 */
#include "bf/model_selection.hpp"

#include "bf/mark.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::model_selection::model_selection()
  : m_mark(NULL), m_bounding_box_is_selected(false)
{

} // model_selection::model_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert an mark in the selection.
 * \param mark The mark to insert.
 * \param main_selection Tell if the main selection is set on the mark.
 */
void bf::model_selection::insert( mark* mark, bool main_selection )
{
  CLAW_PRECOND( mark != NULL );

  m_group.insert(mark);

  if ( main_selection || (m_mark == NULL) )
    m_mark = mark;
} // model_selection::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert a selection in the selection.
 * \param s The marks to insert.
 */
void bf::model_selection::insert( const model_selection& s )
{
  if ( !s.empty() )
    {
      m_group.insert(s.m_group.begin(), s.m_group.end());
      m_mark = s.m_mark;
    }
} // model_selection::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an mark from the selection.
 * \param mark The mark to insert.
 */
void bf::model_selection::remove( mark* mark )
{
  CLAW_PRECOND( mark != NULL );

  m_group.erase(mark);

  if ( m_mark == mark )
    {
      if ( m_group.empty() )
        m_mark = NULL;
      else
        m_mark = *m_group.begin();
    }
} // model_selection::remove()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a selection from the selection.
 * \param s The marks to remove.
 */
void bf::model_selection::remove( const model_selection& s )
{
  for (const_iterator it=s.begin(); it!=s.end(); ++it)
    m_group.erase(*it);

  if ( s.m_group.find(m_mark) != s.m_group.end() )
    {
      if ( m_group.empty() )
        m_mark = NULL;
      else
        m_mark = *m_group.begin();
    }
} // model_selection::remove()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if an mark is selected.
 * \param mark The mark to check.
 */
bool bf::model_selection::is_selected( mark* mark ) const
{
  CLAW_PRECOND( mark != NULL );

  return m_group.find(mark) != m_group.end();
} // model_selection::is_selected()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if an mark is the main selection.
 * \param mark The mark to check.
 */
bool bf::model_selection::is_main_selection( mark* mark ) const
{
  CLAW_PRECOND( mark != NULL );

  return m_mark == mark;
} // model_selection::is_main_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the selection on the bounding box.
 * \param s The new state of the selection.
 */
void bf::model_selection::set_bounding_box_selection( bool s )
{
  m_bounding_box_is_selected = s;
} // model_selection::set_bounding_box_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the bounding box is selected.
 */
bool bf::model_selection::bounding_box_is_selected() const
{
  return m_bounding_box_is_selected;
} // model_selection::bounding_box_is_selected()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the bounding box is the main selection.
 */
bool bf::model_selection::bounding_box_is_main_selection() const
{
  return m_bounding_box_is_selected && m_group.empty();
} // model_selection::bounding_box_is_main_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Make the selection empty.
 */
void bf::model_selection::clear()
{
  m_mark = NULL;
  m_group.clear();
} // model_selection::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the selection is empty.
 */
bool bf::model_selection::empty() const
{
  return mark_empty() && !bounding_box_is_selected();
} // model_selection::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is no mark in the selection.
 */
bool bf::model_selection::mark_empty() const
{
  CLAW_PRECOND( (m_mark != NULL) || m_group.empty() );

  return m_mark == NULL;
} // model_selection::mark_empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the main selection.
 */
bf::mark* bf::model_selection::get_main_mark_selection() const
{
  return m_mark;
} // model_selection::get_main_mark_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first mark in the selection.
 */
bf::model_selection::const_iterator bf::model_selection::begin() const
{
  return m_group.begin();
} // model_selection::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator after the last mark in the selection.
 */
bf::model_selection::const_iterator bf::model_selection::end() const
{
  return m_group.end();
} // model_selection::end()
