/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::balloon_layer_add_message class.
 * \author Sebastien Angibaud
 */
#include "bear/engine/comic/message/balloon_layer_add_message.hpp"

#include "bear/engine/comic/layer/balloon_layer.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::balloon_layer_add_message::balloon_layer_add_message()
  : m_speaker(NULL)
{

} // balloon_layer_add_message::balloon_layer_add_message()

/*----------------------------------------------------------------------------*/
/*
 * \brief Set the speaker.
 * \param speaker The speaker.
 */
void bear::engine::balloon_layer_add_message::set_speaker
( speaker_item* speaker)
{
  m_speaker = speaker;
} // balloon_layer_add_message::set_speaker()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the message to the balloon layer.
 * \param that The balloon layer to apply the message to.
 */
bool bear::engine::balloon_layer_add_message::apply_to( balloon_layer& that )
{
  if ( m_speaker == NULL )
    return false;

  that.add_speaker(m_speaker);
  return true;
} // balloon_layer_add_message::apply_to()
