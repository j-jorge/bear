/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief OpenGL implementation of the screen.
 * \author Julien Jorge
 */
#ifndef __VISUAL_GL_SCREEN_HPP__
#define __VISUAL_GL_SCREEN_HPP__

#include "visual/base_screen.hpp"
#include "visual/shader_program.hpp"

#include <SDL/SDL.h>
#include "visual/gl.hpp"

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

      /**
       * \brief The visitor that sets the variables of a shader program.
       */
      class uniform_setter
      {
      public:
        explicit uniform_setter( GLuint program );

        void operator()( std::string name, int value ) const;
        void operator()( std::string name, double value ) const;
        void operator()( std::string name, bool value ) const;

      private:
        /** \brief The identifier of the shader program in which the variables
            are set. */
        const GLuint m_program;

      }; // class uniform_setter

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

      void push_shader( const shader_program& p );
      void pop_shader();

      void shot( claw::graphic::image& img ) const;

    private:
      void render_sprite( const position_type& pos, const sprite& s );
      
      void get_render_coord
        ( const position_type& pos, const sprite& s,
          std::vector<position_type>& result ) const;
      claw::math::box_2d<GLdouble> get_texture_clip( const sprite& s ) const;

      void render_image
        ( const std::vector<position_type>& render_coord,
          const claw::math::box_2d<GLdouble>& clip,
          const color_type& color );

      position_type
      rotate( const position_type& pos, GLdouble a,
              const position_type& center ) const;

      void set_video_mode( unsigned int w, unsigned int y, bool f );

      screen_size_type get_best_screen_size
        ( unsigned int w, unsigned int y, bool f ) const;
      screen_size_type get_best_screen_size
        ( unsigned int w, unsigned int h,
          const std::vector<SDL_DisplayMode>& modes ) const;
      std::vector<SDL_DisplayMode> get_sdl_display_modes() const;

      bool is_closed();

      std::vector<GLfloat>
        fill_gl_colors( const color_type& c, std::size_t count ) const;
      std::vector<GLfloat>
        fill_gl_positions( const std::vector<position_type>& p ) const;
      std::vector<GLfloat>
        fill_gl_texture_positions
        ( const claw::math::box_2d<GLdouble>& clip ) const;

      void use_program( const shader_program& p ) const;

    private:
      /** \brief The window created by SDL. */
      SDL_Window* m_window;

      /** \brief The width and height of the screen. */
      screen_size_type m_size;

      /** \brief Tell if the screen needs to be restored. */
      bool m_need_restoration;

      /** \brief A buffer in which we do the screenshots, to avoid an allocation
          at each call. */
      claw::graphic::rgba_pixel_8* m_screenshot_buffer;

      /** \brief The title of the window. */
      const std::string m_title;

      /** \brief The real size of the window. */
      screen_size_type m_window_size;

      /** \brief The shaders to apply to the next rendering commands. */
      std::vector<shader_program> m_shader;

    }; // class gl_screen
  } // namespace visual
} // namespace bear

#endif // __VISUAL_GL_SCREEN_HPP__
