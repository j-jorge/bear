/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A horizontal flow displays his children from the left to the right and
 *        from the top to the bottom, keeping them aligned on a grid.
 * \author Julien Jorge
 */
#ifndef __GUI_HORIZONTAL_FLOW_HPP__
#define __GUI_HORIZONTAL_FLOW_HPP__

#include "gui/visual_component.hpp"
#include "gui/class_export.hpp"

#include "input/keyboard.hpp"

namespace bear
{
  namespace gui
  {
    /**
     * \brief A horizontal flow displays his children from the left to the right
     *        and from the top to the bottom, keeping them aligned on a grid.
     * \author Julien Jorge
     */
    class GUI_EXPORT horizontal_flow:
      public visual_component
    {
    public:
      explicit horizontal_flow
      ( size_type horizontal_margin = 0, size_type vertical_margin = 0,
        bear::visual::color_type color = bear::visual::color_type());

      void set_horizontal_margin( size_type m );
      void set_vertical_margin( size_type m );
      void set_margins( size_type horizontal, size_type vertical );
      void display( std::list<visual::scene_element>& e ) const;

    private:
      bool process_mouse_move
      ( const claw::math::coordinate_2d<unsigned int>& pos );
      bool on_key_press( const input::key_info& key );
      bool on_mouse_move( const claw::math::coordinate_2d<unsigned int>& pos );
      bool highlight_control_at
     ( const claw::math::coordinate_2d<unsigned int>& pos );
      bool special_code( const input::key_info& key );
      bool children_at_top(unsigned int line, unsigned int column);
      bool children_at_bottom(unsigned int line, unsigned int column);
      bear::gui::visual_component::iterator get_selected_children();
      bool get_selected_children_in_array
      (unsigned int & line, unsigned int & column);
      bool move_left();
      bool move_right();
      bool move_up();
      bool move_down();
      virtual void on_resized();
      virtual void on_clear();
      virtual void on_child_inserted( visual_component* child );
      virtual void on_child_removed( visual_component* child );
      virtual void on_focused();

      void adjust_children_positions();

    private:
      /** \brief The horizontal margin between the components. */
      size_type m_horizontal_margin;

      /** \brief The vertical margin between the components. */
      size_type m_vertical_margin;

      /** \brief The selected children. */
      visual_component* m_selected_children;

      /** \brief Color used to display rectangle around selected childrens. */
      bear::visual::color_type m_selected_color;

      /** \brief Array of visible childrens. */
      std::vector< std::vector< visual_component* > > m_childrens_array;
    }; // class horizontal_flow
  } // namespace gui
} // namespace bear

#endif // __GUI_HORIZONTAL_FLOW_HPP__
