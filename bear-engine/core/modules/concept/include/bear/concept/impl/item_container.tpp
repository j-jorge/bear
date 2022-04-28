/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementaiton of the bear::concept::item_container class.
 * \author Julien Jorge.
 */

#include <claw/logger/logger.hpp>

/*---------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template <class ItemType>
bear::concept::item_container<ItemType>::item_container()
  : m_locked(false)
{

} // item_container::item_container() [constructor]

/*---------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 *
 * Nothing is copied.
 */
template <class ItemType>
bear::concept::item_container<ItemType>::item_container
( const item_container<item_type>& that )
  : m_locked(false)
{

} // item_container::item_container() [copy constructor]

/*---------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 * \pre The container is unlocked.
 */
template <class ItemType>
bear::concept::item_container<ItemType>::~item_container()
{
  if ( m_locked )
    {
      claw::logger << claw::log_warning
       << "bear::concept::item_container::~item_container(): "
       << "destructor is called but the instance is locked."
       << std::endl;
    }
} // item_container::~item_container() [destructor]

/*---------------------------------------------------------------------------*/
/**
 * \brief Add an item in the container.
 * \param who The item to add.
 */
template <class ItemType>
void
bear::concept::item_container<ItemType>::register_item(const item_type& who)
{
  if (m_locked)
    m_life_queue.push_back(who);
  else
    add(who);
} // item_container::register()

/*---------------------------------------------------------------------------*/
/**
 * \brief Remove an item of the container.
 * \param who The item to remove.
 */
template <class ItemType>
void bear::concept::item_container<ItemType>::release_item(const item_type& who)
{
  if (m_locked)
    m_death_queue.push_back(who);
  else
    remove(who);
} // item_container::release()

/*---------------------------------------------------------------------------*/
/**
 * \brief Tell if the instance is locked.
 */
template <class ItemType>
bool bear::concept::item_container<ItemType>::locked() const
{
  return m_locked;
} // item_container::locked()

/*---------------------------------------------------------------------------*/
/**
 * \brief Lock the container : items added and removed are now buffered.
 */
template <class ItemType>
void bear::concept::item_container<ItemType>::lock()
{
  m_locked = true;
} // item_container::lock()

/*---------------------------------------------------------------------------*/
/**
 * \brief Unlock the buffer, add and remove the corresponding pending items.
 */
template <class ItemType>
void bear::concept::item_container<ItemType>::unlock()
{
  m_locked = false;

  // add items
  for(; !m_life_queue.empty(); m_life_queue.pop_front())
    add( m_life_queue.front() );

  // delete items
  for(; !m_death_queue.empty(); m_death_queue.pop_front())
    remove( m_death_queue.front() );
} // item_container::unlock()

