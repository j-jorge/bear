#pragma once

#include <claw/coordinate_2d.hpp>

namespace bear
{
  namespace input
  {
    class display_projection
    {
    public:
      display_projection();
      display_projection
      ( const claw::math::coordinate_2d< int >& window_size,
        const claw::math::coordinate_2d< int >& view_size,
        const claw::math::coordinate_2d< int >& viewport_size );

      claw::math::coordinate_2d< unsigned int > window_to_display
      ( int x, int y ) const;
      claw::math::coordinate_2d< unsigned int > window_to_display
      ( double x, double y ) const;

    private:
      claw::math::coordinate_2d< int > m_window_size;
      claw::math::coordinate_2d< int > m_view_size;
      claw::math::coordinate_2d< int > m_viewport_size;
    };
  }
}

