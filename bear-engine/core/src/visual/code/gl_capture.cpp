#include "visual/gl_capture.hpp"

#include "visual/gl_renderer.hpp"

bear::visual::gl_capture::gl_capture( const std::vector< gl_state >& states )
  : m_states( states )
{

}

bear::visual::gl_capture* bear::visual::gl_capture::clone() const
{
  return new gl_capture( *this );
}

boost::signals2::connection
bear::visual::gl_capture::render
( const capture_ready& ready, const capture_progress& progress )
{
  return gl_renderer::get_instance().queue_capture( m_states, ready, progress );
}
