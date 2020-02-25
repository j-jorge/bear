/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A message that adds a speaker in a balloon layer.
 * \author Angibaud S�bastien
 */
#ifndef __BEAR_BALLOON_LAYER_ADD_MESSAGE_HPP__
#define __BEAR_BALLOON_LAYER_ADD_MESSAGE_HPP__

#include "bear/communication/typed_message.hpp"

#include "bear/engine/class_export.hpp"
#include "bear/engine/comic/layer/balloon_layer.hpp"

namespace bear
{
  namespace engine
  {
    class balloon_layer;
    class speaker_item;

    /**
     * \brief A message that adds a speaker in a balloon layer.
     * \author S�bastien Angibaud
     */
    class ENGINE_EXPORT balloon_layer_add_message:
      public communication::typed_message<balloon_layer>
    {
    public:
      balloon_layer_add_message();
      void set_speaker( speaker_item* speaker );
      bool apply_to( balloon_layer& that );

    private:
      /** \brief Pointer to the speaker. */
      speaker_item* m_speaker;

    }; // class balloon_layer_add_message

  } // namespace engine
} // namespace bear

#endif // __BEAR_BALLOON_LAYER_ADD_MESSAGE_HPP__
