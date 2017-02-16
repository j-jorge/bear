/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The gl_renderer class manages the process of rendering with OpenGL.
 * \author Julien Jorge
 */
#ifndef __VISUAL_GL_RENDERER_HPP__
#define __VISUAL_GL_RENDERER_HPP__

#include "visual/gl_state.hpp"
#include "visual/shader_program.hpp"
#include "visual/types.hpp"

#include <claw/image.hpp>

#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/signals2/signal.hpp>

#include <SDL2/SDL.h>

namespace bear
{
  namespace visual
  {
    class gl_draw;
    class gl_fragment_shader;
    class gl_vertex_shader;

    /**
     * \brief The gl_renderer class manages the process of rendering with
     *        OpenGL.
     * \author Julien Jorge
     */
    class gl_renderer
    {
    public:
      /** \brief The type of a position on the screen, at pixel precision. */
      typedef claw::math::coordinate_2d<unsigned int> screen_position_type;

      /** \brief The type of a 2D size on the screen, at pixel level. */
      typedef claw::math::coordinate_2d<unsigned int> screen_size_type;
      
      /** \brief The type of the container in which the states to render are
          stored. */
      typedef std::vector<gl_state> state_list;

    private:
      typedef gl_renderer* renderer_pointer;

    public:
      static gl_renderer& get_instance();
      static void terminate();

      GLuint create_texture( screen_size_type& size );
      bool draw_texture
      ( GLuint texture_id, const claw::graphic::image& data,
        const screen_position_type& pos );

      claw::graphic::image
      read_texture( GLuint texture_id, const screen_size_type& size );

      void delete_texture( GLuint texture_id );

      GLuint create_fragment_shader( const std::string& p );
      GLuint create_vertex_shader( const std::string& p );
      void delete_shader( GLuint shader_id );

      GLuint create_shader_program
      ( const gl_fragment_shader& fragment, const gl_vertex_shader& vertex );
      void delete_shader_program( GLuint program_id );

      void shot( claw::graphic::image& img );
      boost::signals2::connection shot
      ( const boost::function< void( const claw::graphic::image& ) >& f );
      
      screen_size_type get_size();
      screen_size_type get_container_size();

      void set_title( const std::string& title );
      void set_video_mode( const screen_size_type& size, bool f );
      void set_fullscreen( bool f );

      void set_gl_states( state_list& states );

      color_type get_background_color();
      void set_background_color( const color_type& c );

    private:
      void stop();

      void loop();
      bool initialization_loop();
      void render_loop();

      void render_states();
      void draw_scene();
      void draw_states();
      void draw_screenshot();

      void set_background_color();

      void resize_view( const screen_size_type& viewport_size );
      
      void make_current();
      void release_context();
      
      void copy_texture_pixels
      ( GLuint texture_id, claw::graphic::rgba_pixel_8* pixels, std::size_t x,
        std::size_t y, std::size_t w, std::size_t h );

      bool ensure_window_exists();
      void assign_transform_matrix();

      screen_size_type get_best_screen_size() const;

      std::vector<SDL_DisplayMode> get_sdl_display_modes() const;

      screen_size_type
      get_best_screen_size
      ( const std::vector<SDL_DisplayMode>& modes ) const;

      void update_screenshot();
      void dispatch_screenshot();

      GLuint create_shader( GLenum type, const std::string& p );
      
    private:
      gl_renderer();
      gl_renderer( const gl_renderer& that ) = delete;

    private:
      /** \brief The single instance of this class. */
      static renderer_pointer s_instance;

      /** \brief Tells if we must stop the rendering process. */
      bool m_stop;
      
      /** \brief The window created by SDL. */
      SDL_Window* m_window;

      /** \brief The OpenGL context used for rendering. */
      SDL_GLContext m_gl_context;

      /** \brief The color of the background of the screen. */
      color_type m_background_color;

      /** \brief The size of the window in which we render. */
      screen_size_type m_window_size;

      /** \brief The size of the view on the rendered elements. */
      screen_size_type m_view_size;
      screen_size_type m_viewport_size;

      /** \brief The title of the window. */
      std::string m_title;

      /** \brief Tells if we are displaying a fullscreen window.*/
      bool m_fullscreen;

      /** \brief Tells if the window has been initialized. */
      bool m_video_mode_is_set;

      /** \brief The next elements to render. */
      state_list m_states;

      bool m_render_ready;
      boost::condition_variable m_render_condition;

      /** \brief A buffer in which we do the screenshots, to avoid an allocation
          at each call. */
      std::vector< claw::graphic::rgba_pixel_8 > m_screenshot_buffer;
      std::vector< claw::graphic::rgba_pixel_8 >
      m_progressive_screenshot_buffer;
      claw::graphic::image m_progressive_screenshot_image;
      boost::signals2::signal< void( const claw::graphic::image& ) >
      m_screenshot_signal;
      GLuint m_screenshot_frame_buffer;
      GLuint m_screenshot_render_buffer;
      std::size_t m_screenshot_line;
      bool m_ongoing_screenshot;
      
      gl_draw* m_draw;
      shader_program m_shader;
      
      /** \brief The various mutexes used to avoid simultaneous access to the
          fields of the class, and to the GL state. */
      struct
      {
        /** \brief This mutex is locked when a function calls OpenGL
            functions. */
        boost::mutex gl_access;

        /** \brief This mutex is locked when a function accesses m_states. */
        boost::mutex gl_set_states;

        /** \brief This mutex is locked when a function accesses
            m_background_color. */
        boost::mutex background_color;

        /** \brief This mutex is locked when a function accesses the property of
            the window, m_window or m_gl_context. */
        boost::mutex window;

        /** \brief This mutex is locked when the variables modifying the
            execution of the rendering loop is accessed. */
        boost::mutex loop_state;

      } m_mutex;

      /** \brief The thread running the render loop of this instance. */
      boost::thread* m_render_thread;

    }; // class gl_renderer

  } // namespace visual
} // namespace bear

#endif // __VISUAL_GL_RENDERER_HPP__
