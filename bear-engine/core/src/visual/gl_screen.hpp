/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief OpenGL implementation of the screen.
 * \author Julien Jorge
 */
#ifndef __VISUAL_GL_SCREEN_HPP__
#define __VISUAL_GL_SCREEN_HPP__

#include "visual/base_screen.hpp"

#include <SDL/SDL.h>
#include <GL/gl.h>

namespace bear
{
  namespace visual
  {
    /**
     * \brief OpenGL implementation of the screen.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT gl_screen:
      public base_screen
    {
    private:
      /** \brief The type used to represent the size of a screen. */
      typedef claw::math::coordinate_2d<unsigned int> screen_size_type;

    public:
      static void initialize();
      static void release();

      gl_screen( const claw::math::coordinate_2d<unsigned int>& size,
                 const std::string& title="", bool full=false );
      ~gl_screen();

      void resize_view();
      void fullscreen( bool b );
      claw::math::coordinate_2d<unsigned int> get_size() const;
      claw::math::coordinate_2d<unsigned int> get_container_size() const;
      bool need_restoration() const;
      void set_restored();

      void set_background_color( const color_type& c );
      color_type get_background_color() const;

      void begin_render();
      void render( const position_type& pos, const sprite& s );
      bool end_render();

      void draw_line
      ( const color_type& color,
        const std::vector<position_type>& p, double w = 1.0, bool close=false );

      void draw_polygon
      ( const color_type& color,
        const std::vector<position_type>& p );

      void shot( claw::graphic::image& img ) const;

    private:
      void render_sprite( const position_type& pos, const sprite& s );
      
      void get_render_coord
        ( const position_type& pos, const sprite& s,
          claw::math::coordinate_2d<GLdouble>* result ) const;
      claw::math::box_2d<GLdouble> get_texture_clip( const sprite& s ) const;

      void render_image
      ( const claw::math::coordinate_2d<GLdouble> render_coord[],
        const claw::math::box_2d<GLdouble>& clip );

      claw::math::coordinate_2d<GLdouble>
      rotate( const claw::math::coordinate_2d<GLdouble>& pos, GLdouble a,
              const claw::math::coordinate_2d<GLdouble>& center ) const;

      void set_video_mode( unsigned int w, unsigned int y, bool f );

      screen_size_type get_best_screen_size
        ( unsigned int w, unsigned int y, bool f ) const;
      screen_size_type get_best_screen_size
        ( unsigned int w, unsigned int h, SDL_Rect** modes ) const;

      bool is_closed();

      void update_z_position();

    private:
      /** \brief The width and height of the screen. */
      screen_size_type m_size;

      /** \brief Tell if the screen needs to be restored. */
      bool m_need_restoration;

      /** \brief A buffer in which we do the screenshots, to avoid an allocation
          at each call. */
      claw::graphic::rgba_pixel_8* m_screenshot_buffer;

      /** \brief The z position of the next item to render. */
      GLdouble m_z_position;

      /** \brief The title of the window. */
      const std::string m_title;

      /** \brief The size of the screen at the initialization. */
      screen_size_type m_display_size;

      /** \brief The real size of the window. */
      screen_size_type m_window_size;

    }; // class gl_screen
  } // namespace visual
} // namespace bear

#endif // __VISUAL_GL_SCREEN_HPP__
