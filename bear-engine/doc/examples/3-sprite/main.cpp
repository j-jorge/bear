#include "visual/screen.hpp"
#include "visual/scene_sprite.hpp"
#include "time/time.hpp"

#include <fstream>

int main( int argc, char* argv[] )
{
  bear::visual::screen::initialize( bear::visual::screen::screen_gl );

  bear::visual::screen s( claw::math::coordinate_2d<unsigned int>(1024, 600) );
  s.unset_pause();

  std::ifstream f( "hourglass.png" );
  claw::graphic::image img( f );
  bear::visual::image texture( img );
  bear::visual::sprite spr( texture );
  bear::visual::scene_sprite element( 100, 200, spr );

  for ( int i(0); i!=5; ++i )
    {
      s.begin_render();
      s.render( element );
      s.end_render();

      bear::systime::sleep( 1000 );
    }

  return 0;
}
