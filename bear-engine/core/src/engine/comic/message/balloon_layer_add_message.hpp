/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A message that adds a speaker in a balloon layer.
 * \author Angibaud Sébastien
 */
#ifndef __BEAR_BALLOON_LAYER_ADD_MESSAGE_HPP__
#define __BEAR_BALLOON_LAYER_ADD_MESSAGE_HPP__

#include "communication/typed_message.hpp"

#include "engine/class_export.hpp"


namespace bear
{
  namespace engine
  {
    class balloon_layer;
    class speaker_item;

    /**
     * \brief A message that adds a speaker in a balloon layer.
     * \author Sébastien Angibaud
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
