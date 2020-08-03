/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::visual::gl_renderer class.
 * \author Julien Jorge
 */
#include "visual/gl_renderer.hpp"

#include "visual/gl_capture_queue.hpp"
#include "visual/gl_draw.hpp"
#include "visual/gl_fragment_shader.hpp"
#include "visual/gl_error.hpp"
#include "visual/gl_vertex_shader.hpp"
#include "visual/sdl_error.hpp"
#include "visual/detail/get_default_fragment_shader_code.hpp"
#include "visual/detail/get_default_vertex_shader_code.hpp"
#include "visual/detail/gl_vertex_attribute_index.hpp"

#include "time/time.hpp"

#include <claw/logger.hpp>

namespace bear
{
  namespace visual
  {
    namespace detail
    {
      static GLuint create_shader( GLenum type, const std::string& source );
      static void log_shader_errors( GLuint shader_id );

      static GLuint create_program( GLuint f, GLuint v );
      static void log_program_errors
      ( const std::string& step, GLuint program_id );
    }
  }
}

/*----------------------------------------------------------------------------*/
bear::visual::gl_renderer::renderer_pointer
bear::visual::gl_renderer::s_instance( (gl_renderer*)nullptr );

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the single instance of this class and starts the rendering
 *        process if it has not been started yet.
 */
bear::visual::gl_renderer& bear::visual::gl_renderer::get_instance()
{
  if ( s_instance == nullptr )
    s_instance = renderer_pointer( new gl_renderer() );

  return *s_instance;
} // gl_renderer::get_instance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Terminates the rendering process.
 */
void bear::visual::gl_renderer::terminate()
{
  if ( s_instance != nullptr )
    {
      s_instance->stop();
      delete s_instance;
      s_instance = nullptr;
    }
} // gl_renderer::terminate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a new texture.
 * \param size The size of the texture.
 */
GLuint bear::visual::gl_renderer::create_texture( screen_size_type& size )
{
  boost::mutex::scoped_lock lock( m_mutex.gl_access );
 
  GLuint texture_id;

  make_current();
  
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  VISUAL_GL_ERROR_THROW();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, nullptr );
  VISUAL_GL_ERROR_THROW();

  release_context();

  return texture_id;
} // gl_renderer::create_texture()

/*----------------------------------------------------------------------------*/
/**
 * \brief Replaces a portion of a texture with a given data.
 * \param texture_id The identifier of the texture in which we write the pixels.
 * \param data The pixels to copy in the image.
 * \param pos The position in the image where data must be copied.
 * \return true If there is a transparent pixel in the copied data.
 */
bool bear::visual::gl_renderer::draw_texture
( GLuint texture_id, const claw::graphic::image& data,
  const screen_position_type& pos )
{
  const claw::graphic::rgba_pixel_8::component_type opaque =
    std::numeric_limits<claw::graphic::rgba_pixel_8::component_type>::max();

  const std::size_t pixels_count( data.width() * data.height() );
  claw::graphic::rgba_pixel_8* const pixels =
    new claw::graphic::rgba_pixel_8[ pixels_count ];

  std::copy( data.begin(), data.end(), pixels );

  copy_texture_pixels
    ( texture_id, pixels, pos.x, pos.y, data.width(), data.height() );

  bool has_transparency( false );

  for ( claw::graphic::rgba_pixel_8* p = pixels;
        (p != pixels + pixels_count) && !has_transparency; ++p )
    has_transparency = p->components.alpha != opaque;
  
  delete[] pixels;

  return has_transparency;
} // gl_renderer::draw_texture()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reads the content of a texture in an image.
 * \param texture_id The identifier of the texture to read.
 * \param size The size of the texture. On systems where the texture's size
 *        cannot be retrieved from the texture itself (i.e. when using GLES),
 *        this value is used to allocate the buffer receiving the
 *        image. Otherwise it is ignored.
 * \return An image made from the texture's data.
 */
claw::graphic::image
bear::visual::gl_renderer::read_texture
( GLuint texture_id, const screen_size_type& size )
{
#ifdef GL_TEXTURE_WIDTH

  boost::mutex::scoped_lock lock( m_mutex.gl_access );

  make_current();

  glBindTexture( GL_TEXTURE_2D, texture_id );
  VISUAL_GL_ERROR_THROW();

  GLint width(size.x);
  GLint height(size.y);

  glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width );
  VISUAL_GL_ERROR_THROW();

  glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height );
  VISUAL_GL_ERROR_THROW();

  const std::size_t pixels_count( width * height );
  claw::graphic::rgba_pixel_8* const pixels =
    new claw::graphic::rgba_pixel_8[ pixels_count ];

  glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

  release_context();

  claw::graphic::image result( width, height );
  std::copy( pixels, pixels + pixels_count, result.begin() );

  delete[] pixels;

  return result;

#else
  return claw::graphic::image( size.x, size.y );
#endif
} // gl_renderer::read_texture()

/*----------------------------------------------------------------------------*/
/**
 * \brief Deletes a texture.
 * \param texture_id The identifier of the texture to delete.
 */
void bear::visual::gl_renderer::delete_texture( GLuint texture_id )
{
  boost::mutex::scoped_lock lock( m_mutex.gl_access );

  make_current();

  if ( glIsTexture( texture_id ) )
    glDeleteTextures(1, &texture_id);

  release_context();
} // gl_renderer::delete_texture()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a new fragment shader.
 * \param p The stream from which we read the shader's code.
 */
GLuint bear::visual::gl_renderer::create_fragment_shader( const std::string& p )
{
  return create_shader( GL_FRAGMENT_SHADER, p );
}

GLuint bear::visual::gl_renderer::create_vertex_shader( const std::string& p )
{
  return create_shader( GL_VERTEX_SHADER, p );
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Deletes a fragment shader previously created with
 *        create_fragment_shader().
 * \param shader_id The identifier of the shader to delete.
 */
void bear::visual::gl_renderer::delete_shader( GLuint shader_id )
{
  boost::mutex::scoped_lock lock( m_mutex.gl_access );
  make_current();

  if ( glIsShader( shader_id ) )
    glDeleteShader( shader_id );

  release_context();
} // gl_renderer::delete_fragment_shader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a new shader program from an existing fragment shader.
 * \param shader The shader to link in the program.
 */
GLuint bear::visual::gl_renderer::create_shader_program
( const gl_fragment_shader& fragment, const gl_vertex_shader& vertex )
{
  boost::mutex::scoped_lock lock( m_mutex.gl_access );
  make_current();

  const GLuint result
    ( detail::create_program( fragment.shader_id(), vertex.shader_id() ) );

  release_context();

  return result;
} // gl_renderer::create_shader_program()

/*----------------------------------------------------------------------------*/
/**
 * \brief Deletes a shader program previously created with
 *        create_shader_program().
 * \param program_id The identifier of the program to delete.
 */
void bear::visual::gl_renderer::delete_shader_program( GLuint program_id )
{
  boost::mutex::scoped_lock lock( m_mutex.gl_access );
  make_current();

  if ( glIsProgram( program_id ) )
    {
      GLint shader_count;

      glGetProgramiv( program_id, GL_ATTACHED_SHADERS, &shader_count );

      if ( shader_count != 0 )
        {
          GLuint* shaders = new GLuint[ shader_count ];
          glGetAttachedShaders( program_id, shader_count, nullptr, shaders );

          for ( GLint i(0); i != shader_count; ++i )
            glDetachShader( program_id, shaders[i] );

          delete[] shaders;
        }
    }

  glDeleteProgram( program_id );

  release_context();
} // gl_renderer::delete_shader_program()

/*----------------------------------------------------------------------------*/
/**
 * \brief Does a screen shot.
 * \param img The image in which we save the content of the gl_renderer.
 */
void bear::visual::gl_renderer::shot( claw::graphic::image& img )
{
  boost::mutex::scoped_lock lock( m_mutex.gl_access );

  make_current();

  GLint p[4];
  glGetIntegerv( GL_VIEWPORT, p );
  const unsigned int w = p[2];
  const unsigned int h = p[3];

  img.set_size( w, h );

  glReadPixels
    ( 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, m_screenshot_buffer.data() );
  VISUAL_GL_ERROR_THROW();

  for ( claw::graphic::rgba_pixel_8& p : m_screenshot_buffer )
    p.components.alpha = 255;

  auto begin( m_screenshot_buffer.begin() );
  auto end( begin + w );

  for ( unsigned int y( 0 ); y != h; ++y )
    {
      std::copy( begin, end, img[ h - y - 1 ].begin() );
      begin += w;
      end += w;
    }

  release_context();
} // gl_renderer::shot()

bear::visual::gl_capture bear::visual::gl_renderer::capture_scene()
{
  return gl_capture( m_previous_states );
}

boost::signals2::connection bear::visual::gl_renderer::queue_capture
( const state_list& states,
  const boost::function< void( const claw::graphic::image& ) >& ready,
  const boost::function< void( double ) >& progress )
{
  boost::mutex::scoped_lock gl_lock( m_mutex.gl_access );
  
  return m_capture_queue->enqueue( states, ready, progress );
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the size of the view on the rendered elements.
 */
bear::visual::gl_renderer::screen_size_type
bear::visual::gl_renderer::get_size()
{
  boost::mutex::scoped_lock lock( m_mutex.window );

  return m_view_size;
} //gl_renderer::get_size()

bear::visual::gl_renderer::screen_size_type
bear::visual::gl_renderer::get_viewport_size()
{
  boost::mutex::scoped_lock lock( m_mutex.window );

  return m_viewport_size;
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the size of the window in which we render.
 */
bear::visual::gl_renderer::screen_size_type
bear::visual::gl_renderer::get_container_size()
{
  boost::mutex::scoped_lock lock( m_mutex.window );

  if ( m_window == nullptr )
    return m_window_size;

  int w, h;
  SDL_GetWindowSize( m_window, &w, &h );

  return screen_size_type( w, h );
} //gl_renderer::get_container_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the title of the window.
 * \param title The new title of the window.
 */
void bear::visual::gl_renderer::set_title( const std::string& title )
{
  boost::mutex::scoped_lock lock( m_mutex.window );

  m_title = title;

  if ( m_window != nullptr )
    SDL_SetWindowTitle( m_window, m_title.c_str() );
} // gl_renderer::set_title()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the size of the screen.
 * \param w The width of the screen.
 * \param h The height of the screen.
 * \param f Tell if we want a fullscreen mode.
 */
void bear::visual::gl_renderer::set_video_mode
( const screen_size_type& size, bool f )
{
  {
    boost::mutex::scoped_lock lock( m_mutex.window );

    m_view_size = size;
    m_window_size = size;
    m_fullscreen = f;

    const std::size_t buffer_size( m_view_size.x * m_view_size.y );
    m_screenshot_buffer.resize( buffer_size );

    m_video_mode_is_set = true;
  }

  if ( m_render_thread == nullptr )
    ensure_window_exists();
} // gl_renderer::set_video_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turns the fullscreen mode on or off.
 * \param f Tell if we want a fullscreen mode.
 */
void bear::visual::gl_renderer::set_fullscreen( bool f )
{
  boost::mutex::scoped_lock lock( m_mutex.window );

  if ( m_fullscreen == f )
    return;

  m_fullscreen = f;

  if ( m_window != nullptr )
    {
      make_current();

      if ( f )
        SDL_SetWindowFullscreen( m_window, SDL_WINDOW_FULLSCREEN );
      else
        SDL_SetWindowFullscreen( m_window, 0 );

      int w;
      int h;
      SDL_GetWindowSize( m_window, &w, &h );
      m_window_size.set( w, h );

      boost::mutex::scoped_lock gl_lock( m_mutex.gl_access );
      resize_view();

      release_context();
    }
} // gl_renderer::set_fullscreen()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the elements to render.
 * \param states States to render. The function steals the states and leaves
 *        this parameter empty when returning.
 */
void bear::visual::gl_renderer::set_gl_states( state_list& states )
{
  {
    boost::mutex::scoped_lock lock( m_mutex.gl_set_states );

    m_states.clear();
    m_states.swap( states );
    m_render_ready = true;
  }

  if ( m_render_thread == nullptr )
    render_states();
  else
    m_render_condition.notify_one();
} // gl_renderer::set_gl_states()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the color of the background.
 */
bear::visual::color_type bear::visual::gl_renderer::get_background_color()
{
  return m_background_color;
} // gl_renderer::get_background_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the color of the background, used to clear the screen.
 * \param c The color.
 */
void bear::visual::gl_renderer::set_background_color( const color_type& c )
{
  m_background_color = c;
  
  boost::mutex::scoped_lock lock( m_mutex.gl_access );
  m_draw->set_background_color( c );
} // gl_renderer::set_background_color()

void bear::visual::gl_renderer::pause()
{
}

void bear::visual::gl_renderer::unpause()
{
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells to stop the rendering process.
 */
void bear::visual::gl_renderer::stop()
{
  {
    boost::mutex::scoped_lock lock( m_mutex.loop_state );
    m_stop = true;
  }

  {
    boost::mutex::scoped_lock lock( m_mutex.gl_set_states );
    m_render_ready = true;
    m_render_condition.notify_one();
  }
  
  delete m_draw;
  
  if ( m_render_thread != nullptr )
    {
      m_render_thread->join();
      delete m_render_thread;
    }

  SDL_GL_DeleteContext( m_gl_context );
  SDL_DestroyWindow( m_window );
} // gl_renderer::stop()

void bear::visual::gl_renderer::loop()
{
  if ( !initialization_loop() )
    return;
  
  render_loop();
}

bool bear::visual::gl_renderer::initialization_loop()
{
  while ( true )
    {
      {
        // lock m_stop to ensure that stop() will block if called during the
        // loop.
        boost::mutex::scoped_lock states_lock( m_mutex.loop_state );
        
        if ( m_stop )
          return false;

        if ( ensure_window_exists() )
          return true;
      }

      systime::sleep( systime::milliseconds_type( 100 ) );
    }
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Executes the rendering process. The function returns when m_stop
 *        becomes true. Otherwise it renders the states from m_states.
 */
void bear::visual::gl_renderer::render_loop()
{
  while ( true )
    {
      {
        boost::mutex::scoped_lock states_lock( m_mutex.gl_set_states );

        while( !m_render_ready )
          m_render_condition.wait( states_lock );
      }

      // lock m_stop to ensure that stop() will block if called during the loop.
      m_mutex.loop_state.lock();

      if ( m_stop )
        {
          m_mutex.loop_state.unlock();
          break;
        }

      const systime::milliseconds_type start( systime::get_date_ms() );
      render_states();
      const systime::milliseconds_type end( systime::get_date_ms() );
      update_screenshot( end - start );

      // Release the mutex while we sleep so other threads can request to stop
      // the loop.
      m_mutex.loop_state.unlock();
    }
} // gl_renderer::render_loop()

/*----------------------------------------------------------------------------*/
/**
 * \brief Renders the elements of m_states then removes them.
 */
void bear::visual::gl_renderer::render_states()
{
  boost::mutex::scoped_lock states_lock( m_mutex.gl_set_states );

  m_render_ready = false;
  
  assert ( m_gl_context != nullptr );
  draw_scene();

  // The destruction of the states may call a delete_something() which will need
  // the gl_access mutex. Thus we have to ensure it is unlocked here.
  m_previous_states.swap( m_states );
  m_states.clear();
} // gl_renderer::render_states()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clears the view and calls draw() on each state of m_states.
 */
void bear::visual::gl_renderer::draw_scene()
{
  boost::mutex::scoped_lock gl_lock( m_mutex.gl_access );
  make_current();

  m_draw->draw( m_states );
  m_capture_queue->draw( *m_draw );
  
  SDL_GL_SwapWindow( m_window );
  VISUAL_GL_ERROR_THROW();

  release_context();
}

void bear::visual::gl_renderer::update_screenshot
( systime::milliseconds_type render_time )
{
  boost::mutex::scoped_lock gl_lock( m_mutex.gl_access );
  make_current();

  const systime::milliseconds_type allocated_time
    ( render_time >= 15 ? 0 : 15 - render_time );
  m_capture_queue->update( allocated_time );

  release_context();
}

void bear::visual::gl_renderer::resize_view()
{
  const float scale
    ( std::min
      ( float( m_window_size.x ) / m_view_size.x,
        float( m_window_size.y ) / m_view_size.y ) );

  const float w( m_view_size.x * scale );
  const float h( m_view_size.y * scale );

  m_viewport_size.set( w, h );

  glViewport( ( m_window_size.x - w ) / 2, ( m_window_size.y - h ) / 2, w, h );
  VISUAL_GL_ERROR_THROW();
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Makes m_gl_context the current context of the thread.
 */
void bear::visual::gl_renderer::make_current()
{
  if ( SDL_GL_MakeCurrent( m_window, m_gl_context ) != 0 )
    claw::logger << claw::log_error
                 << SDL_GetError() << std::endl;
} // gl_renderer::make_current()

/*----------------------------------------------------------------------------*/
/**
 * \brief Releases the context associated with this thread, in order to let it
 *        available for other threads.
 */
void bear::visual::gl_renderer::release_context()
{
  if ( SDL_GL_MakeCurrent( m_window, nullptr ) != 0 )
    claw::logger << claw::log_error
                 << SDL_GetError() << std::endl;
} // gl_renderer::release_context()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the pixel of a part of an existing texture.
 * \param texture_id The identifier of the texture.
 * \param pixels The pixels to copy into the texture.
 * \param x The x-coordinate of the top-left region where the pixels are copied
 *        in the texture.
 * \param y The y-coordinate of the top-left region where the pixels are copied
 *        in the texture.
 * \param w The width of the region where the pixels are copied in the texture.
 * \param h The height of the region where the pixels are copied in the texture.
 */
void bear::visual::gl_renderer::copy_texture_pixels
( GLuint texture_id, claw::graphic::rgba_pixel_8* pixels, std::size_t x,
  std::size_t y, std::size_t w, std::size_t h )
{
  boost::mutex::scoped_lock lock( m_mutex.gl_access );

  make_current();

  glBindTexture(GL_TEXTURE_2D, texture_id);
    
  glTexSubImage2D
    ( GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

  release_context();
} // gl_renderer::copy_texture_pixels()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the window and the GL context if it has not been created yet.
 */
bool bear::visual::gl_renderer::ensure_window_exists()
{
  boost::lock(m_mutex.window, m_mutex.gl_access);

  if ( !m_video_mode_is_set || (m_gl_context != nullptr) )
    return false;

#ifdef __ANDROID__
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
#endif
  
  Uint32 flags = SDL_WINDOW_OPENGL;

  m_window_size = get_best_screen_size();

  if ( m_fullscreen )
    flags |= SDL_WINDOW_FULLSCREEN;

  claw::logger << claw::log_verbose << "Setting video mode to "
               << m_window_size.x << 'x' << m_window_size.y
               << ' ' << (m_fullscreen ? "fullscreen" : "windowed")
               << std::endl;

  m_window =
    SDL_CreateWindow
    ( m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      m_window_size.x, m_window_size.y, flags );

  if ( m_window == nullptr )
    VISUAL_SDL_ERROR_THROW();

  m_gl_context = SDL_GL_CreateContext( m_window );

  if ( m_gl_context == nullptr )
    VISUAL_SDL_ERROR_THROW();

  claw::logger << claw::log_verbose << "OpenGL version is "
               << glGetString( GL_VERSION )
               << ", vendor is " << glGetString(GL_VENDOR)
               << std::endl;

  SDL_ShowCursor(0);

#ifdef _WIN32
  const GLenum err = glewInit();

  if ( err != GLEW_OK )
    claw::logger << claw::log_error << "Failed to initialize Glew: "
                 << glewGetErrorString(err) << std::endl;
#endif
  
  glEnable(GL_BLEND);
  VISUAL_GL_ERROR_THROW();

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  VISUAL_GL_ERROR_THROW();

  resize_view();

  create_drawing_helper();
  create_capture_queue();

  release_context();

  m_mutex.window.unlock();
  m_mutex.gl_access.unlock();

  return true;
} // gl_renderer::ensure_window_exists()

void bear::visual::gl_renderer::create_drawing_helper()
{
  GLuint texture;
  glGenTextures( 1, &texture );
  VISUAL_GL_ERROR_THROW();

  glBindTexture( GL_TEXTURE_2D, texture );
  VISUAL_GL_ERROR_THROW();

  glTexImage2D
    ( GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
      &claw::graphic::white_pixel );
  VISUAL_GL_ERROR_THROW();

  const GLuint shader
    ( detail::create_program
      ( detail::create_shader
        ( GL_FRAGMENT_SHADER, detail::get_default_fragment_shader_code() ),
        detail::create_shader
        ( GL_VERTEX_SHADER, detail::get_default_vertex_shader_code() ) ) );
  
  m_draw = new gl_draw( texture, shader, m_view_size );
}
    
void bear::visual::gl_renderer::create_capture_queue()
{
  m_capture_queue = new gl_capture_queue( m_window_size, m_viewport_size );
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Finds a screen resolution that has is the nearest of a given size.
 * \param w The width of the screen.
 * \param h The height of the screen.
 * \param f Tell if we want a fullscreen mode.
 */
bear::visual::gl_renderer::screen_size_type
bear::visual::gl_renderer::get_best_screen_size() const
{
  screen_size_type result( m_view_size );

  claw::logger << claw::log_verbose << "Requested screen resolution is "
               << result.x << 'x' << result.y << '.' << std::endl;

#ifdef __ANDROID__
  const bool use_fullscreen(true);
#else
  const bool use_fullscreen(m_fullscreen);
#endif

  if ( use_fullscreen )
    {
      claw::logger << claw::log_verbose << "Available screen resolutions:"
                   << std::endl;
      
      const std::vector<SDL_DisplayMode> modes( get_sdl_display_modes() );

      for ( std::size_t i(0); i != modes.size(); ++i )
        claw::logger << claw::log_verbose << modes[i].w << 'x' << modes[i].h
                     << std::endl;

      result = get_best_screen_size( modes );
    }
  else
    {
      claw::logger << claw::log_verbose
                   << "Setting resolution in windowed mode."
                   << std::endl;
      
      SDL_DisplayMode m;
      SDL_GetDesktopDisplayMode(0, &m);

      const double r_x = (double)m.w / m_view_size.x;
      const double r_y = (double)m.h / m_view_size.y;
      double resize_ratio = std::min( r_x, r_y );

      if ( resize_ratio < 1 )
        result =
          screen_size_type
          ( resize_ratio * m_view_size.x, resize_ratio * m_view_size.y );
    }

  claw::logger << claw::log_verbose << "Selected screen resolution is "
               << result.x << 'x' << result.y << '.' << std::endl;

  return result;
} // gl_renderer::get_best_screen_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the display modes as returned by SDL.
 */
std::vector<SDL_DisplayMode>
bear::visual::gl_renderer::get_sdl_display_modes() const
{
  const int count( SDL_GetNumDisplayModes(0) );

  if ( count < 1 )
    VISUAL_SDL_ERROR_THROW();

  std::vector<SDL_DisplayMode> result( count );

  for ( int i(0); i!=count; ++i )
    SDL_GetDisplayMode( 0, i, &result[i] );

  return result;
} // gl_renderer::get_sdl_display_modes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Finds a screen resolution that has is the nearest of a given size.
 * \param modes The available resolutions.
 */
bear::visual::gl_renderer::screen_size_type
bear::visual::gl_renderer::get_best_screen_size
( const std::vector<SDL_DisplayMode>& modes ) const
{
  SDL_DisplayMode m;
  SDL_GetDesktopDisplayMode(0, &m);

  const double requested_ratio( (double)m_view_size.x / m_view_size.y );
  screen_size_type result( m.w, m.h );
  double display_ratio( (double)result.x / result.y );
  double best_scale_factor_distance;
  
  if ( (display_ratio >= 1) && (requested_ratio >= 1) )
    best_scale_factor_distance = std::abs( (double)m.w / m_view_size.x - 1 );
  else
    best_scale_factor_distance = std::abs( (double)m.h / m_view_size.y - 1 );

  for ( std::size_t i(0); i!=modes.size(); ++i )
    {
      const unsigned int width( modes[i].w );
      const unsigned int height( modes[i].h );

      const double scale_factor
        ( std::min
          ( (double)height / m_view_size.y, (double)width / m_view_size.x ) );

      const double scale_factor_distance( std::abs( scale_factor - 1 ) );

      if ( scale_factor_distance < best_scale_factor_distance )
        {
          best_scale_factor_distance = scale_factor_distance;
          result = screen_size_type( width, height );
        }
    }

  return result;
} // gl_renderer::get_best_screen_size()

GLuint
bear::visual::gl_renderer::create_shader( GLenum type, const std::string& p )
{
  boost::mutex::scoped_lock lock( m_mutex.gl_access );
  make_current();

  const GLuint result( detail::create_shader( type, p ) );

  release_context();

  return result;
}

/*----------------------------------------------------------------------------*/
/**
 * Constructs a gl_renderer instance with no rendering informations.
 */
bear::visual::gl_renderer::gl_renderer()
  : m_stop( false ), m_window( nullptr ), m_gl_context( nullptr ),
    m_background_color(0, 0, 0),
    m_window_size( 640, 480 ),
    m_view_size( m_window_size ),
    m_viewport_size( m_view_size ),
    m_fullscreen( false ),
    m_video_mode_is_set( false ),
    m_render_ready( false ),
    m_draw( nullptr ),
    m_capture_queue( nullptr )
{

#ifdef WIN32
  m_render_thread = nullptr;
#else
  m_render_thread =
    new boost::thread( boost::bind(&gl_renderer::loop, this) );
#endif
} // gl_renderer::gl_renderer()

GLuint
bear::visual::detail::create_shader( GLenum type, const std::string& source )
{
  const GLuint result( glCreateShader( type ) );
  VISUAL_GL_ERROR_THROW();

  const char *fragmentText = source.c_str();

  glShaderSource( result, 1, &fragmentText, 0 );
  VISUAL_GL_ERROR_THROW();

  glCompileShader( result );
  log_shader_errors( result );

  return result;
}

void bear::visual::detail::log_shader_errors( GLuint shader_id )
{
  GLint buffer_size;
  glGetShaderiv( shader_id, GL_INFO_LOG_LENGTH, &buffer_size );

  if ( buffer_size <= 1 )
    return;

  char* const buffer = new char[ buffer_size ];

  glGetShaderInfoLog( shader_id, buffer_size, nullptr, buffer );

  claw::logger << claw::log_error << "Shader " << shader_id
               << " compile error: " << buffer << std::endl;

  delete[] buffer;
}

GLuint bear::visual::detail::create_program( GLuint f, GLuint v )
{
  const GLuint result( glCreateProgram() );
  VISUAL_GL_ERROR_THROW();

  glBindAttribLocation( result, detail::position_attribute, "in_position");
  VISUAL_GL_ERROR_THROW();
  glBindAttribLocation( result, detail::color_attribute, "in_color");
  VISUAL_GL_ERROR_THROW();
  glBindAttribLocation
    ( result, detail::texture_coordinate_attribute, "in_texture_coordinates");
  VISUAL_GL_ERROR_THROW();

  glAttachShader( result, f );
  VISUAL_GL_ERROR_THROW();

  glAttachShader( result, v );
  VISUAL_GL_ERROR_THROW();

  glLinkProgram( result );
  log_program_errors( "link", result );

  glValidateProgram( result );
  log_program_errors( "validation", result );

  return result;
}

void bear::visual::detail::log_program_errors
( const std::string& step, GLuint program_id )
{
  GLint buffer_size;
  glGetProgramiv( program_id, GL_INFO_LOG_LENGTH, &buffer_size );

  if ( buffer_size <= 1 )
    return;

  char* const buffer = new char[ buffer_size ];

  glGetProgramInfoLog( program_id, buffer_size, nullptr, buffer );

  claw::logger << claw::log_error << "Program " << program_id << ' ' << step
               << " errors: " << buffer << std::endl;

  delete[] buffer;
}
