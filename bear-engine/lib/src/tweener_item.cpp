/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::tweener_item class.
 * \author Sébastien Angibaud
 */
#include "bear/generic_items/tweener_item.hpp"

#include "bear/engine/export.hpp"

BASE_ITEM_IMPLEMENT( tweener_item, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param t The tweener managed by this item.
 * \param item An item used as reference for the validity of the tweener. If
 *        \a item is killed then the tweener is abandonned.
 * \param same_lifespan Tell if \a item must be killed with *this when the
 *        tweener is over.
 */
bear::tweener_item::tweener_item
( const claw::tween::tweener& t, engine::base_item* item, bool same_lifespan )
: m_item(item), m_same_lifespan(same_lifespan), m_follow_item(item != NULL),
  m_tweener(t)
{
  set_artificial(true);
  set_phantom(true);
  set_can_move_items(false);
} // tweener_item::tweener_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do post creation actions.
 */
void bear::tweener_item::on_enters_layer()
{
  super::on_enters_layer();

  if ( m_item.get_item() != NULL )
    set_center_of_mass( m_item->get_center_of_mass() );
} // tweener_item::on_enters_layer()

/*---------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::tweener_item::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  const bool item_is_valid = (m_item != (engine::base_item*)NULL);

  if ( m_follow_item && !item_is_valid )
    kill();
  else
    {
      m_tweener.update(elapsed_time); 
    
      if ( m_tweener.is_finished() )
        {
          kill();
	
          if ( m_same_lifespan && item_is_valid )
            m_item->kill();
        }
      else if ( item_is_valid )
        set_center_of_mass( m_item->get_center_of_mass() );
    }
} // tweener_item::progress()
