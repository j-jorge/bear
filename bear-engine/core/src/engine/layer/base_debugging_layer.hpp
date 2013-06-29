/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Base layer for the ones that give debugging information about the
 *        visible items.
 * \author Julien Jorge
 */
#ifndef __ENGINE_BASE_DEBUGGING_LAYER_HPP__
#define __ENGINE_BASE_DEBUGGING_LAYER_HPP__

#include "engine/class_export.hpp"

#include "engine/layer/gui_layer.hpp"
#include "engine/base_item.hpp"

#include "input/key_info.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief Base layer for the ones that give debugging information about the
     *        visible items.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT base_debugging_layer:
      public gui_layer
    {
    public:
      /** \brief The type of a list of scene elements retrieved from the
          layer. */
      typedef gui_layer::scene_element_list scene_element_list;

    protected:
      /** \brief The type of a list of items. */
      typedef std::list<universe::physical_item*> item_list;

    public:
      base_debugging_layer( input::key_code toggle_key );

      void progress( universe::time_type elapsed_time );
      void render( scene_element_list& e ) const;

      bool key_pressed( const input::key_info& key );

    protected:
      virtual void render( scene_element_list& e,
                           const visual::position_type& delta ) const = 0;

      virtual void progress
        ( const item_list& items, universe::time_type elapsed_time ) = 0;

    private:
      void find_items( item_list& items ) const;

      virtual void on_show();

    private:
      /** \brief Tell if the bow must be drawn. */
      bool m_visible;

      /** \brief The value of the key that changes the visibility of the
          layer. */
      const input::key_code m_toggle_key;

    }; // class base_debugging_layer
  } // namespace engine
} // namespace bear

#endif // __ENGINE_BASE_DEBUGGING_LAYER_HPP__
