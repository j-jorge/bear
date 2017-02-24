#include "input/display_projection.hpp"

#include <algorithm>

bear::input::display_projection::display_projection()
  : m_window_size( 1, 1 ),
    m_view_size( 1, 1 ),
    m_viewport_size( 1, 1 )
{

}

bear::input::display_projection::display_projection
( const claw::math::coordinate_2d< int >& window_size,
  const claw::math::coordinate_2d< int >& view_size,
  const claw::math::coordinate_2d< int >& viewport_size )
  : m_window_size( window_size ),
    m_view_size( view_size ),
    m_viewport_size( viewport_size )
{

}

claw::math::coordinate_2d< unsigned int >
bear::input::display_projection::window_to_display( int x, int y ) const
{
  const int rx
    ( ( x - ( m_window_size.x - m_viewport_size.x ) / 2 ) * m_view_size.x
      / m_viewport_size.x );
  const int ry
    ( ( ( m_window_size.y - y ) - ( m_window_size.y - m_viewport_size.y ) / 2 )
      * m_view_size.y / m_viewport_size.y );

  return
    claw::math::coordinate_2d< unsigned int >
    ( std::min( std::max( 0, rx ), m_view_size.x ),
      std::min( std::max( 0, ry ), m_view_size.y ) );
}

claw::math::coordinate_2d< unsigned int >
bear::input::display_projection::window_to_display( double x, double y ) const
{
  return
    window_to_display( int( x * m_window_size.x ), int( y * m_window_size.y ) );
}
