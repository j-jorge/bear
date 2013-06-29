/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::transition_effect_erase_message
 *        class.
 * \author Julien Jorge
 */
#include "engine/message/transition_effect_erase_message.hpp"

#include "engine/layer/transition_layer.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::transition_effect_erase_message::transition_effect_erase_message()
{
  set_id(transition_layer::not_an_id);
} // transition_effect_erase_message::transition_effect_erase_message()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param id The identifier of the effect to erase.
 */
bear::engine::transition_effect_erase_message::transition_effect_erase_message
( std::size_t id )
  : m_id(transition_layer::not_an_id)
{
  set_id(id);
} // transition_effect_erase_message::transition_effect_erase_message()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the message to thelayer.
 * \param that The layer to apply the message to.
 */
bool bear::engine::transition_effect_erase_message::apply_to
( transition_layer& that )
{
  that.erase_effect( m_id );

  return true;
} // transition_effect_erase_message::apply_to()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the identifier of the effect.
 * \param id The identifier.
 */
void bear::engine::transition_effect_erase_message::set_id( std::size_t id )
{
  m_id = id;
} // transition_effect_erase_message::set_id()
