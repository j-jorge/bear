/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::transition_effect_message class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param p The prefered position of the effect if several effects are played
 *        simultaneously.
 * \param replace Tell if the new effect replaces the current ones.
 */
template<typename EffectType>
bear::engine::transition_effect_message<EffectType>::transition_effect_message
( int p, bool replace )
  : m_id(transition_layer::not_an_id), m_replace(replace), m_position(p)
{

} // transition_effect_message::transition_effect_message()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the message to thelayer.
 * \param that The layer to apply the message to.
 */
template<typename EffectType>
bool bear::engine::transition_effect_message<EffectType>::apply_to
( transition_layer& that )
{
  if ( m_replace )
    m_id = that.set_effect( new EffectType(m_effect), m_position );
  else
    m_id = that.push_effect( new EffectType(m_effect), m_position );

  return true;
} // transition_effect_message::apply_to()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the effect.
 */
template<typename EffectType>
EffectType& bear::engine::transition_effect_message<EffectType>::get_effect()
{
  return m_effect;
} // transition_effect_message::get_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the identifier of the effect added in the layer.
 */
template<typename EffectType>
std::size_t bear::engine::transition_effect_message<EffectType>::get_id() const
{
  return m_id;
} // transition_effect_message::get_id()
