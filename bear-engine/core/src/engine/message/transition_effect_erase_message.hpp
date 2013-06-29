/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A message to be sent to the transition_layer to create a transition
 *        effect.
 * \author Julien Jorge
 */
#ifndef __BEAR_ENGINE_TRANSITION_EFFECT_ERASE_MESSAGE_HPP__
#define __BEAR_ENGINE_TRANSITION_EFFECT_ERASE_MESSAGE_HPP__

#include "communication/typed_message.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class transition_layer;

    /**
     * \brief A message to be sent to the transition_layer to erase a
     *        transition effect.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT transition_effect_erase_message:
      public communication::typed_message<transition_layer>
    {
    public:
      transition_effect_erase_message();
      transition_effect_erase_message( std::size_t id );

      bool apply_to( transition_layer& that );
      void set_id( std::size_t id );

    private:
      /** \brief The identifier returned by the layer. */
      std::size_t m_id;

    }; // class transition_effect_erase_message
  } // namespace engine
} // namespace bear

#endif // __BEAR_ENGINE_TRANSITION_EFFECT_ERASE_MESSAGE_HPP__
