/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::item_selection class.
 * \author Julien Jorge
 */
#include "bf/item_selection.hpp"

#include "bf/item_instance.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::item_selection::item_selection()
  : m_item(NULL)
{

} // item_selection::item_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert an item in the selection.
 * \param item The item to insert.
 * \param main_selection Tell if the main selection is set on the item.
 */
void bf::item_selection::insert( item_instance* item, bool main_selection )
{
  CLAW_PRECOND( item != NULL );

  m_group.insert(item);

  if ( main_selection || (m_item == NULL) )
    m_item = item;
} // item_selection::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inserts a selection in the selection. The main selection becomes the
 *        one of the argument.
 * \param s The items to insert.
 */
void bf::item_selection::insert( const item_selection& s )
{
  if ( !s.empty() )
    {
      m_group.insert(s.m_group.begin(), s.m_group.end());
      m_item = s.m_item;
    }
} // item_selection::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the selection.
 * \param item The item to insert.
 */
void bf::item_selection::remove( item_instance* item )
{
  CLAW_PRECOND( item != NULL );

  m_group.erase(item);

  if ( m_item == item )
    {
      if ( m_group.empty() )
        m_item = NULL;
      else
        m_item = *m_group.begin();
    }
} // item_selection::remove()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a selection from the selection.
 * \param s The items to remove.
 */
void bf::item_selection::remove( const item_selection& s )
{
  for (const_iterator it=s.begin(); it!=s.end(); ++it)
    m_group.erase(*it);

  if ( s.m_group.find(m_item) != s.m_group.end() )
    {
      if ( m_group.empty() )
        m_item = NULL;
      else
        m_item = *m_group.begin();
    }
} // item_selection::remove()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if an item is selected.
 * \param item The item to check.
 */
bool bf::item_selection::is_selected( item_instance const* item ) const
{
  CLAW_PRECOND( item != NULL );

  return m_group.find(const_cast<item_instance*>(item)) != m_group.end();
} // item_selection::is_selected()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if an item is the main selection.
 * \param item The item to check.
 */
bool bf::item_selection::is_main_selection( item_instance const* item ) const
{
  CLAW_PRECOND( item != NULL );

  return m_item == item;
} // item_selection::is_main_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Make the selection empty.
 */
void bf::item_selection::clear()
{
  m_item = NULL;
  m_group.clear();
} // item_selection::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the selection is empty.
 */
bool bf::item_selection::empty() const
{
  CLAW_PRECOND( (m_item != NULL) || m_group.empty() );

  return m_item == NULL;
} // item_selection::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the main selection.
 */
bf::item_instance* bf::item_selection::get_main_selection() const
{
  return m_item;
} // item_selection::get_main_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first item in the selection.
 */
bf::item_selection::const_iterator bf::item_selection::begin() const
{
  return m_group.begin();
} // item_selection::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator after the last item in the selection.
 */
bf::item_selection::const_iterator bf::item_selection::end() const
{
  return m_group.end();
} // item_selection::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the group of items in this selection is the same that the one
 *        in an other selection.
 * \param that The selection to compare to.
 */
bool bf::item_selection::same_group_than( const item_selection& that ) const
{
  return m_group == that.m_group;
} // item_selection::same_group_than()
