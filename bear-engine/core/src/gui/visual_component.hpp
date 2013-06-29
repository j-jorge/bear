/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Base class for all gui components.
 * \author Julien Jorge
 */
#ifndef __GUI_VISUAL_COMPONENT_HPP__
#define __GUI_VISUAL_COMPONENT_HPP__

#include "gui/types.hpp"
#include "visual/scene_element.hpp"
#include "input/input_listener.hpp"
#include "gui/callback_group.hpp"
#include "visual/bitmap_rendering_attributes.hpp"

#include <vector>

#include <claw/iterator.hpp>
#include <claw/functional.hpp>
#include <claw/non_copyable.hpp>

#include "gui/class_export.hpp"

namespace bear
{
  namespace gui
  {
    /**
     * \brief Base class for all gui components.
     * \author Julien Jorge
     */
    class GUI_EXPORT visual_component:
      public claw::pattern::non_copyable,
      public input::input_listener
    {
    public:
      typedef std::list<visual::scene_element> scene_element_list;

    private:
      typedef std::vector<visual_component*> component_list;

    public:
      /** The type of the iterators on the children components. */
      typedef claw::wrapped_iterator
        <
          visual_component, component_list::const_iterator,
          claw::dereference<visual_component>
        >::iterator_type iterator;

    public:
      visual_component();
      virtual ~visual_component();

      void insert( visual_component* child );
      void remove( visual_component* child );
      void remove_all();

      iterator begin() const;
      iterator end() const;

      void render( scene_element_list& e ) const;

      bool key_pressed( const input::key_info& key );
      bool char_pressed( const input::key_info& key );
      bool button_pressed
      ( input::joystick::joy_code button, unsigned int joy_index );
      bool mouse_pressed( input::mouse::mouse_code button,
                          const claw::math::coordinate_2d<unsigned int>& pos );
      bool mouse_released( input::mouse::mouse_code button,
                          const claw::math::coordinate_2d<unsigned int>& pos );
      bool mouse_maintained( input::mouse::mouse_code button,
                          const claw::math::coordinate_2d<unsigned int>& pos );

      bool mouse_move( const claw::math::coordinate_2d<unsigned int>& pos );

      void fit( size_type margin = 0 );

      void set_size_maximum();
      void set_size( const size_box_type& size );
      void set_size( size_type w, size_type h );
      void set_width( size_type w );
      void set_height( size_type h );
      void set_position( const position_type& pos );
      void set_position( coordinate_type x, coordinate_type y );
      void set_visible( bool b );
      void set_tab_order( unsigned int v );
      void set_input_priority( bool this_first );

      void set_left( coordinate_type x );
      void set_right( coordinate_type x );
      void set_top( coordinate_type y );
      void set_bottom( coordinate_type y );
      void set_top_left( coordinate_type x, coordinate_type y );
      void set_bottom_left( coordinate_type x, coordinate_type y );
      void set_top_right( coordinate_type x, coordinate_type y );
      void set_bottom_right( coordinate_type x, coordinate_type y );

      size_type width() const;
      size_type height() const;
      coordinate_type left() const;
      coordinate_type right() const;
      coordinate_type bottom() const;
      coordinate_type top() const;
      position_type top_left() const;
      position_type top_right() const;
      position_type bottom_left() const;
      position_type bottom_right() const;

      size_box_type get_size() const;
      position_type get_position() const;
      const rectangle_type& get_rectangle() const;
      bool get_visible() const;

      size_type get_border_size() const;

      void set_border_color( const color_type& clr );

      void set_top_left_border_color( const color_type& clr );
      const color_type& get_top_left_border_color() const;

      void set_bottom_right_border_color( const color_type& clr );
      const color_type& get_bottom_right_border_color() const;

      void set_background_color( const color_type& clr );
      const color_type& get_background_color() const;

      void disable();
      void enable();
      bool is_enabled() const;

      void set_focus();
      visual_component* get_focus() const;
      void add_focus_callback( const callback& c );

      void clear();
      
      visual::bitmap_rendering_attributes& get_rendering_attributes();
      const visual::bitmap_rendering_attributes&
        get_rendering_attributes() const;

    protected:
      void
      change_tab_position( const visual_component* that, unsigned int pos );

      virtual void on_resized();
      virtual void on_clear();

      virtual void on_child_inserted( visual_component* child );
      virtual void on_child_removed( visual_component* child );

      virtual void display( scene_element_list& e ) const;

      virtual bool on_key_press( const input::key_info& key );
      virtual bool on_char_pressed( const input::key_info& key );
      virtual bool on_button_press( input::joystick::joy_code button,
                                    unsigned int joy_index );
      virtual bool on_mouse_press
      ( input::mouse::mouse_code key,
        const claw::math::coordinate_2d<unsigned int>& pos );
      virtual
      bool on_mouse_move( const claw::math::coordinate_2d<unsigned int>& pos );
      virtual bool on_mouse_released
      ( input::mouse::mouse_code key,
        const claw::math::coordinate_2d<unsigned int>& pos );
      virtual bool on_mouse_maintained
      ( input::mouse::mouse_code key,
        const claw::math::coordinate_2d<unsigned int>& pos );

      virtual void on_focused();

    private:
      bool broadcast_mouse_move
      ( const claw::math::coordinate_2d<unsigned int>& pos );
      bool broadcast_mouse_press
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );
      bool broadcast_mouse_released
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );
      bool broadcast_mouse_maintained
      ( input::mouse::mouse_code button,
        const claw::math::coordinate_2d<unsigned int>& pos );

      void stay_in_owner();

      void set_focus( visual_component* c );
      
      scene_element_list get_scene_elements() const;
      void render_faces( scene_element_list& e ) const;

    private:
      /**
       * \brief The box in which the component will be. The position is relative
       *        to the bottom left corner of the owner.
       */
      rectangle_type m_box;

      /** \brief The component containing this component. */
      visual_component* m_owner;

      /** \brief The components in this component. */
      component_list m_components;

      /** \brief The component having the focus. */
      int m_focused_component;

      /** \brief Tell if the component is visible or not. */
      bool m_visible;

      /** \brief Tell if the component catches the inputs before its
          children. */
      bool m_input_priority;

      /** \brife Tell if this component is enabled or not. */
      bool m_enabled;

      /** \brief The color of the top and left borders of the control. */
      color_type m_top_left_border_color;

      /** \brief The color of the bottom and right borders of the control. */
      color_type m_bottom_right_border_color;

      /** \brief The color of the background of the control. */
      color_type m_background_color;

      /** \brief Callback executed when the component receives the focus. */
      callback_group m_focus_callback;

      /** \brief Global rendering attributes of the item. */
      visual::bitmap_rendering_attributes m_rendering_attributes;

    }; // class visual_component
  } // namespace gui
} // namespace bear

#endif // __GUI_VISUAL_COMPONENT_HPP__
