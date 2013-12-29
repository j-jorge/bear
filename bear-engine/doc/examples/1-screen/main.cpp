#include "visual/screen.hpp"
#include "SDL2/SDL.h"

bool g_quit(false);

int event_filter
( void* user_data, SDL_Event* event )
{
  int result(0);

  switch ( event->type )
    {
    case SDL_QUIT:
      g_quit = true;
      break;

    default:
      result = 1;
    }

  return result;
}

void init()
{
  bear::visual::screen::initialize( bear::visual::screen::screen_gl );
  SDL_EventState( SDL_QUIT, SDL_ENABLE );

  SDL_SetEventFilter( &event_filter, NULL );
}

int main( int argc, char* argv[] )
{
  init();

  bear::visual::screen s( claw::math::coordinate_2d<unsigned int>(1024, 600) );

  while( !g_quit )
    {
      SDL_PumpEvents();
    }

  bear::visual::screen::release();

  return 0;
}
