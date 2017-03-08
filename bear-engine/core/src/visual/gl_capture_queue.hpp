#pragma once

#include "visual/gl_state.hpp"

#include "time/time.hpp"

#include <claw/image.hpp>

#include <boost/signals2/signal.hpp>

#include <deque>
#include <vector>

namespace bear
{
  namespace visual
  {
    class gl_draw;
    
    class gl_capture_queue
    {
    public:
      typedef std::vector< gl_state > state_list;

    public:
      explicit gl_capture_queue
      ( const claw::math::coordinate_2d< unsigned int >& window_size,
        const claw::math::coordinate_2d< unsigned int >& viewport_size );
      
      boost::signals2::connection enqueue
      ( const state_list& states,
        const boost::function< void( const claw::graphic::image& ) >& ready,
        const boost::function< void( double ) >& progress );
      void draw( gl_draw& output );
      void update( systime::milliseconds_type allocated_time );
      
    private:
      struct entry
      {
      public:
        explicit entry( const state_list& s );

        bool connected() const;
        
      public:
        state_list states;
        boost::signals2::signal< void( double ) > progress_signal;
        boost::signals2::signal< void( const claw::graphic::image& ) >
        ready_signal;
      };
      
    private:
      void setup_render_buffer();
      void setup_frame_buffer();

      bool remove_obsolete_captures();

      void read_pixels( std::size_t line_count );
      void dispatch_screenshot();
      
    private:
      claw::math::coordinate_2d< unsigned int > m_window_size;
      claw::math::coordinate_2d< unsigned int > m_viewport_size;
      std::deque< entry > m_pending_captures;

      std::vector< claw::graphic::rgba_pixel_8 > m_screenshot_buffer;
      claw::graphic::image m_image;

      GLuint m_frame_buffer;
      GLuint m_render_buffer;
      std::size_t m_line_index;
      bool m_ongoing_screenshot;

      std::array< std::size_t, 16 > m_lines_per_duration;
    };
  }
}
