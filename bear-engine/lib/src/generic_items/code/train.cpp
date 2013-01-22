/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::train class.
 * \author Sebastien Angibaud
 */
#include "generic_items/train.hpp"

#include "universe/collision_info.hpp"

#include "engine/export.hpp"

BASE_ITEM_EXPORT( train, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::train::train()
{
  set_global(true);
} // train::train()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do post creation actions.
 */
void bear::train::on_enters_layer()
{
  super::on_enters_layer();

  init(get_top_left());
} // bear::train::on_enters_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the item.
 * \param elapsed_time Elasped time since the last progress.
 */
void bear::train::progress( universe::time_type elapsed_time )
{
  super::progress( elapsed_time );

  update_items();
} // bear::train::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Give a string representation of the item.
 * \param str (out) The result of the convertion.
 */
void bear::train::to_string( std::string& str ) const
{
  super::to_string(str);
  base_train::to_string(str);
} // train::to_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call collision_as_train().
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::train::collision
( engine::base_item& that, universe::collision_info& info )
{
  bool align = collision_check_and_align(that, info);
  
  if ( align )
    collision_as_base_train(that, info);
} // train::collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the movement of the item.
 * \param elapsed_time Elasped time since the last call.
 */
void bear::train::move( universe::time_type elapsed_time )
{
  super::move(elapsed_time);

  update_item_positions(get_top_left(), get_speed());
} // train::move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the items concerned by a progress/move of this one.
 * \param d (out) A list to which are added such items.
 */
void bear::train::get_dependent_items( std::list<physical_item*>& d ) const
{
  super::get_dependent_items(d);

  get_items(d);
} // train::get_dependent_items()
