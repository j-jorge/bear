/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The balloon displays the balloons of the speaker_item.
 * \author Angibaud Sebastien
 */
#ifndef __BEAR_BALLOON_LAYER_HPP__
#define __BEAR_BALLOON_LAYER_HPP__

#include "communication/messageable.hpp"
#include "engine/layer/gui_layer.hpp"
#include "universe/derived_item_handle.hpp"

#include "engine/class_export.hpp"

#include <string>
#include <list>

namespace bear
{
  namespace engine
  {
    class speaker_item;

    /**
     * \brief The balloon displays the balloons of the speaker_item.
     * \author Angibaud Sebastien
     */
    class ENGINE_EXPORT balloon_layer:
      public communication::messageable,
      public engine::gui_layer
    {
    public:
      /** \brief The type of a list of scene elements retrieved from the
          layer. */
      typedef engine::gui_layer::scene_element_list scene_element_list;

      /** \brief The type of the pointers on the speaker items. */
      typedef universe::derived_item_handle<speaker_item> handle_type;

      /** \brief The type of the list of all speakers. */
      typedef std::list<handle_type> speaker_list;

    public:
      balloon_layer( const std::string& name );

      void build();
      void progress( universe::time_type elapsed_time );
      void render( scene_element_list& e ) const;

      void add_speaker( speaker_item* speaker );

    private:
      universe::rectangle_type
      get_bounding_box_on_screen(handle_type& speaker) const;

    private:
      /** \brief List of speakers. */
      speaker_list m_speakers;

      /** \brief  The size of the border. */
      unsigned int m_border;

    }; // class balloon_layer

  } // namespace engine
} // namespace bear

#endif // __BEAR_BALLOON_LAYER_HPP__
