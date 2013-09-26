/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::forced_movement_applicator class.
 * \author Sebastien Angibaud
 */
#include "engine/item_brick/forced_movement_applicator.hpp"

#include "engine/base_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all required fields are initialized.
 */
bool bear::engine::forced_movement_applicator::is_valid() const
{
  return !m_movement.is_null();
} // forced_movement_applicator::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the movement.
 */
const bear::universe::forced_movement&
bear::engine::forced_movement_applicator::get_movement() const
{
  return m_movement;
} // forced_movement_applicator::get_movement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::engine::forced_movement_applicator::give_movement()
{
  for (unsigned int i=0; i!=m_actor.size(); ++i)
    if ( m_actor[i] != (handle_type)NULL )
      m_actor[i]->set_forced_movement(m_movement);
} // forced_movement_applicator::give_movement()
