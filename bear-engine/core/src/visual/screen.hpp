/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The screen surface, where drawing & blitting is done.
 * \author Julien Jorge
 */
#ifndef __VISUAL_SCREEN_HPP__
#define __VISUAL_SCREEN_HPP__

#include "visual/scene_element.hpp"

#include "visual/class_export.hpp"

#include <claw/image.hpp>

#include <boost/signals2/connection.hpp>

namespace bear
{
  namespace visual
  {
    /**
     * \brief The screen surface, where drawing & blitting is done.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT screen
    {
    public:
      /** \brief The subsystem selected for rendering. */
      enum sub_system
        {
          screen_gl,
          screen_undef
        }; // enum_sub_system

    private:
      /** \brief A list of elements of the scene. */
      typedef std::list<scene_element> scene_element_list;

      /** \brief A list of rectangles. */
      typedef std::list<rectangle_type> rectangle_list;

      /** \brief Defined the current screen process. */
      enum screen_status
        {
          SCREEN_IDLE,
          SCREEN_RENDER
        }; // enum screen_status

    public:
      static void initialize( sub_system sub_sys );
      static void release();
      static sub_system get_sub_system();

      screen( const claw::math::coordinate_2d<unsigned int>& size,
              const std::string& title="", bool full=false );
      ~screen();

      void fullscreen( bool b );
      claw::math::coordinate_2d<unsigned int> get_size() const;
      claw::math::coordinate_2d<unsigned int> get_container_size() const;

      void set_opaque_box_visible( bool b );
      void set_dumb_rendering( bool b );
      bool get_dumb_rendering() const;

      void set_background_color( const color_type& c );
      color_type get_background_color() const;

      void begin_render();
      void render( const scene_element& e );
      void end_render();

      void shot( const std::string& bitmap_name ) const;
      void shot( claw::graphic::image& img ) const;
      boost::signals2::connection schedule_shot
      ( const boost::function< void( const claw::graphic::image& ) >& f );

    private:
      void render_opaque_box( const scene_element& e ) const;
      void render_element( const scene_element& e ) const;

      void render_elements();

      bool intersects_any
      ( const rectangle_type& r, const rectangle_list& boxes ) const;

      void split
      ( const scene_element& e, scene_element_list& output,
        rectangle_list& boxes ) const;

      void subtract
      ( const rectangle_type& a, const rectangle_type& b,
        rectangle_list& result ) const;

    private:
      /** \brief True if we are rendering. */
      screen_status m_mode;

      /** \brief The width and height of the screen. */
      base_screen* m_impl;

      /** \brief The elements to render. */
      scene_element_list m_scene_element;

      /** \brief This flag turns on the rendering of the opaque boxes. */
      bool m_render_opaque_box;

      /** \brief This flag turns on the use of the simple rendering
          procedure. */
      bool m_dumb_rendering;

      /** \brief The subsystem used for rendering. */
      static sub_system s_sub_system;

    }; // class screen
  } // namespace visual
} // namespace bear

#endif // __VISUAL_SCREEN_HPP__
