/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::forced_movement_repeater class.
 * \author Julien Jorge
 */
#include "engine/item_brick/forced_movement_repeater.hpp"

#include "engine/base_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::forced_movement_repeater::forced_movement_repeater()
  : m_actor(NULL), m_count(0), m_delay(0)
{

} // forced_movement_repeater::forced_movement_repeater()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::forced_movement_repeater::~forced_movement_repeater()
{
  delete m_actor;
} // forced_movement_repeater::~forced_movement_repeater()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all required fields are initialized.
 */
bool bear::engine::forced_movement_repeater::is_valid() const
{
  return !m_movement.is_null() && (m_delay >= 0);
} // forced_movement_repeater::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the clones and apply the movement to them.
 */
void bear::engine::forced_movement_repeater::give_movement()
{
  CLAW_PRECOND( m_actor != NULL );

  m_actor->set_forced_movement( m_movement );

  for (unsigned int i=0; i!=m_count; ++i)
    {
      base_item* const c = m_actor->clone();
      m_actor->new_item( *c );
      c->set_forced_movement( m_movement );
      c->move( (i+1) * m_delay );
    }

  m_actor = NULL;
} // forced_movement_repeater::give_movement()
