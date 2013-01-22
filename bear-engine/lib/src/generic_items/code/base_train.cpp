/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::base_train class.
 * \author Sebastien Angibaud
 */
#include "generic_items/base_train.hpp"

#include "universe/collision_info.hpp"

#include <algorithm>

/*----------------------------------------------------------------------------*/
/**
 * \brief Init the base train.
 */
void bear::base_train::init(const universe::position_type& position)
{
  m_last_position = position;
} // bear::base_train::init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update list of items.
 */
void bear::base_train::update_items()
{
  item_list::iterator it;
  std::list<item_list::iterator> dead;

  for (it=m_list_items.begin(); it!=m_list_items.end(); ++it)
    if ( *it == NULL )
      dead.push_front(it);

  for( ; !dead.empty(); dead.pop_front() )
    m_list_items.erase( dead.front() );
} // bear::base_train::update_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Give a string representation of the item.
 * \param str (out) The result of the convertion.
 */
void bear::base_train::to_string( std::string& str ) const
{
  std::ostringstream oss;
  oss << "nb_items: " << m_list_items.size() << "\n";

  str = str + oss.str();
} // base_train::to_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Proceed the collision.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::base_train::collision_as_base_train
( engine::base_item& that, universe::collision_info& info )
{
  if ( info.get_collision_side() == universe::zone::top_zone )
    m_list_items.push_front(that);
} // base_train::collision_as_base_train()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the movement of the item.
 * \param pos The position of the train.
 * \param speed The speed on the train.
 */
void bear::base_train::update_item_positions
( const universe::position_type& position, 
  const universe::speed_type& speed )
{
  item_list::iterator it;

  for(it=m_list_items.begin(); it!=m_list_items.end(); ++it)
    if ( *it != NULL )
      (*it)->set_left((*it)->get_left() + position.x - m_last_position.x);

  for(it=m_old_items.begin(); it!=m_old_items.end(); ++it)
    if ( *it != NULL )
      if ( std::find(m_list_items.begin(), m_list_items.end(), *it)
           == m_list_items.end() ) // item is not on me anymore
        (*it)->set_speed
          ( (*it)->get_speed() + universe::speed_type(speed.x, 0) );

  m_last_position = position;
  std::swap(m_old_items, m_list_items);
  m_list_items.clear();
} // base_train::update_item_positions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the items concerned by a progress/move of this one.
 * \param d (out) A list to which are added such items.
 */
void bear::base_train::get_items
( std::list<universe::physical_item*>& d ) const
{
  item_list::const_iterator it;

  for( it=m_list_items.begin(); it!=m_list_items.end(); ++it )
    if ( *it != NULL )
      d.push_front( it->get() );

  for( it=m_old_items.begin(); it!=m_old_items.end(); ++it )
    if ( *it != NULL )
      d.push_front( it->get() );
} // base_train::get_items()
