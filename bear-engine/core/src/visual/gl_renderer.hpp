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
#include "visual/types.hpp"

#include <claw/image.hpp>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

#include <SDL2/SDL.h>

namespace bear
{
  namespace visual
  {
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
      void delete_texture( GLuint texture_id );

      void shot( claw::graphic::image& img );

      screen_size_type get_size();
      screen_size_type get_container_size();

      void set_title( const std::string& title );
      void set_video_mode( const screen_size_type& size, bool f );
      void set_fullscreen( bool f );

      void set_gl_states( state_list& states );

      color_type get_background_color();
      void set_background_color( const color_type& c );

      void set_pause();
      void unset_pause();

    private:
      void stop();

      void render_loop();

      void render_states();
      void set_background_color();

      void resize_view( const screen_size_type& viewport_size );

      void make_current();
      void release_context();
      
      void copy_texture_pixels
      ( GLuint texture_id, claw::graphic::rgba_pixel_8* pixels, std::size_t x,
        std::size_t y, std::size_t w, std::size_t h );

      void ensure_window_exists();

      screen_size_type get_best_screen_size() const;

      std::vector<SDL_DisplayMode> get_sdl_display_modes() const;

      screen_size_type
      get_best_screen_size
      ( const std::vector<SDL_DisplayMode>& modes ) const;

    private:
      gl_renderer();

      // not implemented
      gl_renderer( const gl_renderer& that );

    private:
      /** \brief The single instance of this class. */
      static renderer_pointer s_instance;

      /** \brief Tells if we must stop the rendering process. */
      bool m_stop;
      
      /** \brief Tells if we must pause the rendering process. */
      bool m_pause;
      
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

      /** \brief The title of the window. */
      std::string m_title;

      /** \brief Tells if we are displaying a fullscreen window.*/
      bool m_fullscreen;

      /** \brief Tells if the window has been initialized. */
      bool m_video_mode_is_set;

      /** \brief The next elements to render. */
      state_list m_states;

      /** \brief A buffer in which we do the screenshots, to avoid an allocation
          at each call. */
      claw::graphic::rgba_pixel_8* m_screenshot_buffer;

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
      boost::thread m_render_thread;

    }; // class gl_renderer

  } // namespace visual
} // namespace bear

#endif // __VISUAL_GL_RENDERER_HPP__
