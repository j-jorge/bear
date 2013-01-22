/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::population class.
 * \author Julien Jorge
 */
#include "engine/population.hpp"

#include <claw/assert.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::population::~population()
{
  clear();
} // population::~population()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert an item in the population.
 * \param item The item to insert.
 */
void bear::engine::population::insert( base_item* item )
{
  CLAW_PRECOND( item != NULL );
  CLAW_PRECOND( !exists( item->get_id() )
                || ( m_dropped_items.find(item->get_id())
                    != m_dropped_items.end() ) );

  if ( m_dropped_items.find(item->get_id()) != m_dropped_items.end() )
    m_dropped_items.erase(item->get_id());

  m_items[ item->get_id() ] = item;
} // population::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Kill an item.
 * \param item The item to delete.
 */
void bear::engine::population::kill( const base_item* item )
{
  CLAW_PRECOND( item != NULL );

  m_dead_items.insert( item->get_id() );
  m_dropped_items.erase( item->get_id() );
} // population::kill()

/*----------------------------------------------------------------------------*/
/**
 * \brief Drop an item.
 * \param item The item to drop.
 */
void bear::engine::population::drop( const base_item* item )
{
  CLAW_PRECOND( item != NULL );

  if ( m_dead_items.find( item->get_id() ) == m_dead_items.end() )
    m_dropped_items.insert( item->get_id() );
} // population::drop()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if an item exists.
 * \param id The identifier of the item.
 */
bool bear::engine::population::exists( base_item::id_type id ) const
{
  return m_items.find(id) != m_items.end();
} // population::exists()

/*----------------------------------------------------------------------------*/
/**
 * \brief Delete items on the dead-items list.
 */
void bear::engine::population::remove_dead_items()
{
  std::set<base_item::id_type>::const_iterator it;

  for (it=m_dead_items.begin(); it!=m_dead_items.end(); ++it)
    if ( exists(*it) )
      {
        delete m_items[*it];
        m_items.erase(*it);
      }

  m_dead_items.clear();

  for ( it=m_dropped_items.begin(); it!=m_dropped_items.end(); ++it )
    m_items.erase(*it);

  m_dropped_items.clear();
} // population::remove_dead_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Delete all items.
 */
void bear::engine::population::clear()
{
  remove_dead_items();

  std::map< base_item::id_type, base_item* >::const_iterator it;

  for (it=m_items.begin(); it!=m_items.end(); ++it)
    {
      // The item may have been dropped by the deletion of another item.
      if ( m_dropped_items.find( it->first ) == m_dropped_items.end() )
        delete it->second;
    }

  m_items.clear();
} // population::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the items.
 */
bear::engine::population::const_iterator bear::engine::population::begin() const
{
  return m_items.begin();
} // population::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the end of the items.
 */
bear::engine::population::const_iterator bear::engine::population::end() const
{
  return m_items.end();
} // population::end()
