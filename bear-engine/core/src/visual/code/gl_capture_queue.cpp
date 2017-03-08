#include "visual/gl_capture_queue.hpp"

#include "visual/gl_draw.hpp"
#include "visual/gl_error.hpp"

#include <claw/logger.hpp>

bear::visual::gl_capture_queue::entry::entry( const state_list& s )
  : states( s )
{

}

bool bear::visual::gl_capture_queue::entry::connected() const
{
  return !ready_signal.empty();
}

bear::visual::gl_capture_queue::gl_capture_queue
( const claw::math::coordinate_2d< unsigned int >& window_size,
  const claw::math::coordinate_2d< unsigned int >& viewport_size )
  : m_window_size( window_size ),
    m_viewport_size( viewport_size ),
    m_ongoing_screenshot( false )
{
  std::fill( m_lines_per_duration.begin(), m_lines_per_duration.end(), 0 );
  
  m_screenshot_buffer.resize( viewport_size.x * viewport_size.y );
  m_image.set_size( viewport_size.x, viewport_size.y );

  setup_render_buffer();
  setup_frame_buffer();
}

boost::signals2::connection
bear::visual::gl_capture_queue::enqueue
( const state_list& states,
  const boost::function< void( const claw::graphic::image& ) >& ready,
  const boost::function< void( double ) >& progress )
{
  m_pending_captures.emplace_back( states );
  m_pending_captures.back().progress_signal.connect( progress );
  
  return m_pending_captures.back().ready_signal.connect( ready );
}

void bear::visual::gl_capture_queue::draw( gl_draw& output )
{
  if ( m_ongoing_screenshot || m_pending_captures.empty() )
    return;

  m_ongoing_screenshot = true;
  m_line_index = 0;
  
  glBindFramebuffer( GL_FRAMEBUFFER, m_frame_buffer );
  VISUAL_GL_ERROR_THROW();

  glBindRenderbuffer( GL_RENDERBUFFER, m_render_buffer );
  VISUAL_GL_ERROR_THROW();

  output.draw( m_pending_captures.front().states );
  
  glBindRenderbuffer( GL_RENDERBUFFER, 0 );
  VISUAL_GL_ERROR_THROW();

  glBindFramebuffer( GL_FRAMEBUFFER, 0 );
  VISUAL_GL_ERROR_THROW();
}
#include <iostream>
void bear::visual::gl_capture_queue::update
( systime::milliseconds_type allocated_time )
{
  if ( remove_obsolete_captures() )
    {
      m_ongoing_screenshot = false;
      return;
    }
  
  if ( !m_ongoing_screenshot )
    return;

  allocated_time =
    std::min< systime::milliseconds_type >
    ( m_lines_per_duration.size() - 1, allocated_time );
  const std::size_t line_count( m_lines_per_duration[ allocated_time ] + 1 );

  const systime::milliseconds_type start( systime::get_date_ms() );
  read_pixels( line_count );
  const systime::milliseconds_type end( systime::get_date_ms() );

  const std::size_t index
    ( std::min( std::size_t( end - start ), m_lines_per_duration.size() - 1 ) );
  
  for ( std::size_t i( index );
        ( i != m_lines_per_duration.size() )
          && ( m_lines_per_duration[ i ] < line_count );
        ++i )
    m_lines_per_duration[ i ] = line_count;

  m_pending_captures.front().progress_signal
    ( double( m_line_index ) / m_viewport_size.y );

  if ( m_line_index == m_viewport_size.y )
    dispatch_screenshot();
}

void bear::visual::gl_capture_queue::setup_render_buffer()
{
  glGenRenderbuffers( 1, &m_render_buffer );
  VISUAL_GL_ERROR_THROW();

  glBindRenderbuffer( GL_RENDERBUFFER, m_render_buffer );
  VISUAL_GL_ERROR_THROW();

  glRenderbufferStorage
    ( GL_RENDERBUFFER, GL_RGB565, m_window_size.x, m_window_size.y );
  VISUAL_GL_ERROR_THROW();

  glBindRenderbuffer( GL_RENDERBUFFER, 0 );
}

void bear::visual::gl_capture_queue::setup_frame_buffer()
{
  glGenFramebuffers( 1, &m_frame_buffer );
  VISUAL_GL_ERROR_THROW();
  
  glBindFramebuffer( GL_FRAMEBUFFER, m_frame_buffer );
  VISUAL_GL_ERROR_THROW();

  glFramebufferRenderbuffer
    ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_render_buffer );
  VISUAL_GL_ERROR_THROW();

  const GLenum status( glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
  VISUAL_GL_ERROR_THROW();

  switch( status )
    {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      claw::logger << claw::log_error
                   << "Framebuffer incomplete attachement.\n";
      assert( false );
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
      claw::logger << claw::log_error
                   << "Framebuffer incomplete dimensions.\n";
      assert( false );
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      claw::logger << claw::log_error
                   << "Framebuffer incomplete missing attachement.\n";
      assert( false );
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
      claw::logger << claw::log_error
                   << "Framebuffer unsupported.\n";
      assert( false );
      break;
    case  GL_FRAMEBUFFER_COMPLETE:
      break;
    }
  
  VISUAL_GL_ERROR_THROW();
  
  glBindFramebuffer( GL_FRAMEBUFFER, 0 );
  VISUAL_GL_ERROR_THROW();
}

bool bear::visual::gl_capture_queue::remove_obsolete_captures()
{
  if ( !m_pending_captures.empty() && m_pending_captures.front().connected() )
    return false;

  while ( !m_pending_captures.empty()
          && !m_pending_captures.front().connected() )
    m_pending_captures.pop_front();

  return true;
}

void bear::visual::gl_capture_queue::read_pixels( std::size_t line_count )
{
  glBindFramebuffer( GL_FRAMEBUFFER, m_frame_buffer );
  VISUAL_GL_ERROR_THROW();

  const std::size_t height
    ( std::min( m_viewport_size.y - m_line_index, line_count ) );

  claw::graphic::rgba_pixel_8* const output
    ( m_screenshot_buffer.data() + m_viewport_size.x * m_line_index );

  glReadPixels
    ( ( m_window_size.x - m_viewport_size.x ) / 2,
      m_line_index + ( m_window_size.y - m_viewport_size.y ) / 2,
      m_viewport_size.x, height, GL_RGBA, GL_UNSIGNED_BYTE, output );
  VISUAL_GL_ERROR_THROW();

  glBindFramebuffer( GL_FRAMEBUFFER, 0 );
  VISUAL_GL_ERROR_THROW();

  m_line_index += height;
}

void bear::visual::gl_capture_queue::dispatch_screenshot()
{
  m_ongoing_screenshot = false;

  auto begin( m_screenshot_buffer.begin() );
  auto end( begin + m_viewport_size.x );
  
  for ( unsigned int y( 0 ); y != m_viewport_size.y; ++y )
    {
      std::copy( begin, end, m_image[ m_viewport_size.y - y - 1 ].begin() );
      begin += m_viewport_size.x;
      end += m_viewport_size.x;
    }

  boost::signals2::signal< void( const claw::graphic::image& ) > signal;
  signal.swap( m_pending_captures.front().ready_signal );
  m_pending_captures.pop_front();
  
  signal( m_image );
}
