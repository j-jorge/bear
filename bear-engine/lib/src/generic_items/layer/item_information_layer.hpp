/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This layer displays informations on items when clicking on them.
 * \author Julien Jorge
 */
#ifndef __BEAR_ITEM_INFORMATION_LAYER_HPP__
#define __BEAR_ITEM_INFORMATION_LAYER_HPP__

#include "engine/layer/gui_layer.hpp"
#include "gui/static_text.hpp"
#include "universe/item_handle.hpp"
#include "universe/physical_item.hpp"

#include <list>

namespace bear
{
  /**
   * \brief This layer displays informations on items when clicking on them.
   *
   * When an item is left-clicked, a small window with instant informations
   * about the item appears. When right-clicked, the window is updated each time
   * that the scene is rendered.
   *
   * The windows can be dragged with the left button of the mouse. A right click
   * on a window put it in the background. Middle clicking on a window closes
   * it.
   *
   * A middle click in an empty part closes all windows.
   *
   * \author Julien Jorge
   */
  class item_information_layer:
    public bear::engine::gui_layer
  {
  public:
    /** \brief The type of a list of scene elements retrieved from the layer. */
    typedef bear::engine::gui_layer::scene_element_list scene_element_list;

  private:
    /**
     * \brief A box with the string representation of an item, for instant
     *        informations.
     */
    class info_box
    {
    public:
      info_box
      ( bear::universe::physical_item& item, const bear::visual::font& font );
      virtual ~info_box();

      bool includes( const claw::math::coordinate_2d<unsigned int>& pos ) const;
      bool has_item( const bear::universe::physical_item* item ) const;

      void add_delta( const claw::math::coordinate_2d<int>& delta );

      virtual void update
      ( const bear::universe::rectangle_type& camera_box,
        const claw::math::coordinate_2d<unsigned int>& layer_size);

      void render( scene_element_list& e ) const;

    protected:
      void get_informations();

    private:
      /** \brief The item to display. */
      bear::universe::item_handle m_item;

      /** \brief The distance between the m_position and the informations. */
      bear::universe::position_type m_delta;

      /** \brief Component containing the informations on the item. */
      bear::gui::static_text m_text;

    }; // class info_box

    /**
     * \brief A box with the string representation of an item, for followed
     *        informations.
     */
    class linked_info_box:
      public info_box
    {
    public:
      linked_info_box
      ( bear::universe::physical_item& item, const bear::visual::font& font );

      void update( const bear::universe::rectangle_type& camera_box,
       const claw::math::coordinate_2d<unsigned int>& layer_size );

    }; // class linked_info_box

    typedef std::list<info_box*> info_box_list;

  public:
    item_information_layer();
    ~item_information_layer();

    void clear();

    void progress( bear::universe::time_type elapsed_time );
    void render( scene_element_list& e ) const;

    bool mouse_pressed( bear::input::mouse::mouse_code key,
                        const claw::math::coordinate_2d<unsigned int>& pos );

    bool mouse_released( bear::input::mouse::mouse_code button,
                         const claw::math::coordinate_2d<unsigned int>& pos );

    bool mouse_maintained( bear::input::mouse::mouse_code button,
                           const claw::math::coordinate_2d<unsigned int>& pos );

  private:
    bool show_item( const bear::universe::position_type& pos,
                    const bear::universe::rectangle_type& camera_box );
    bool follow_item( const bear::universe::position_type& pos,
                      const bear::universe::rectangle_type& camera_box );
    bool close_info_box( const claw::math::coordinate_2d<unsigned int>& pos );
    bool grab_info_box( const claw::math::coordinate_2d<unsigned int>& pos );
    bool
    put_in_background( const claw::math::coordinate_2d<unsigned int>& pos );

    info_box_list::iterator
    find_info_box( const claw::math::coordinate_2d<unsigned int>& pos );

    bear::universe::physical_item*
    find_item( const bear::universe::position_type& pos ) const;

    bool is_handled( const bear::universe::physical_item* item ) const;

  private:
    /** \brief The information boxes. */
    info_box_list m_info_box;

    /** \brief An item box that is currently dragged. */
    info_box* m_dragged_item;

    /** \brief The position where the last mouse button has been pressed. */
    claw::math::coordinate_2d<unsigned int> m_drag_reference;

  }; // class item_information_layer
} // namespace bear

#endif // __BEAR_ITEM_INFORMATION_LAYER_HPP__
