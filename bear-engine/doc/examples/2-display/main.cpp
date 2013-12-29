#include "visual/screen.hpp"
#include "visual/scene_rectangle.hpp"
#include "time/time.hpp"

int main( int argc, char* argv[] )
{
  bear::visual::screen::initialize( bear::visual::screen::screen_gl );

  bear::visual::screen s( claw::math::coordinate_2d<unsigned int>(1024, 600) );
  s.unset_pause();

  bear::visual::scene_rectangle r
    ( 0, 0, bear::visual::color_type( 0, 255, 0, 255 ),
      bear::visual::rectangle_type( 500, 300, 600, 400 ) );

  while( true )
    {
      s.begin_render();
      s.render( r );
      s.end_render();

      bear::systime::sleep( 1000 );
    }

  return 0;
}
