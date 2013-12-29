/**
 * \file
 *
 * This example program creates a window and wait for the user to close it.
 *
 * In order to make this example minimalistic, it will not use most of the
 * functionalities of the Bear Engine. Consequently, we will have manage the
 * exit of the program ourselves.
 *
 * The SDL is used for the subsystems of the engine. Thus we can use its event
 * system to detect when the user wants to quit the program.
 */

#include "visual/screen.hpp"
#include "SDL2/SDL.h"

/**
 * This global variable is observed in the main loop in order to exit the
 * program as it becomes true.
 */
bool g_quit(false);

/**
 * This function processes an event of the SDL. If the quit signal has been
 * sent, then g_quit is set to true. Otherwise the function does nothing.
 * \param user_data Ignored.
 * \param event The event to process.
 */
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

/**
 * Initializes the visual module of the engine and registers the event filter to
 * the SDL.
 */
void init()
{
  bear::visual::screen::initialize( bear::visual::screen::screen_gl );
  SDL_EventState( SDL_QUIT, SDL_ENABLE );

  SDL_SetEventFilter( &event_filter, NULL );
}

/**
 * Creates a window and waits for the quit signal to be received.
 */
int main( int argc, char* argv[] )
{
  init();

  // Here we create the window; it will be immediately visible.
  // The engine does not support multiples windows yet, so be careful and be
  // sure to create only one of them.
  bear::visual::screen s( claw::math::coordinate_2d<unsigned int>(640, 480) );

  // And we can wait for the quit signal. The window won't display anything
  // since we never call the rendering functions.
  while( !g_quit )
    SDL_PumpEvents();

  // Turn the subsystems off.
  bear::visual::screen::release();

  return 0;
}
