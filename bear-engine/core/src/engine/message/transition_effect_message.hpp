/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A message to be sent to the transition_layer to create a transition
 *        effect.
 * \author Julien Jorge
 */
#ifndef __BEAR_ENGINE_TRANSITION_EFFECT_MESSAGE_HPP__
#define __BEAR_ENGINE_TRANSITION_EFFECT_MESSAGE_HPP__

#include "communication/typed_message.hpp"
#include "engine/layer/transition_layer.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief A message to be sent to the transition_layer to create a
     *        transition effect.
     * \author Julien Jorge
     */
    template<typename EffectType>
    class transition_effect_message:
      public communication::typed_message<transition_layer>
    {
    public:
      explicit transition_effect_message( int p = 0, bool replace = false );

      bool apply_to( transition_layer& that );
      EffectType& get_effect();
      std::size_t get_id() const;

    private:
      /** \brief The effect to add in the layer. */
      EffectType m_effect;

      /** \brief The identifier returned by the layer. */
      std::size_t m_id;

      /** \brief Tell if the new effect replaces the current ones. */
      bool m_replace;

      /** \brief The prefered position of the effect if several effects are
          played simultaneously. */
      int m_position;

    }; // class transition_effect_message

  } // namespace engine
} // namespace bear

#include "engine/message/impl/transition_effect_message.tpp"

#endif // __BEAR_ENGINE_TRANSITION_EFFECT_MESSAGE_HPP__
