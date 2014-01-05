/**
 * \file
 *
 * This example program creates a window and displays a rectangle in the middle
 * of it.
 *
 * In order to make this example minimalistic, it will not use most of the
 * functionalities of the Bear Engine. Consequently, we will have manage the
 * exit of the program ourselves.
 *
 * The SDL is used for the subsystems of the engine. Thus we can use its event
 * system to detect when the user wants to quit the program.
 */

#include "visual/screen.hpp"
#include "visual/scene_rectangle.hpp"

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
 * Creates a window and draws a rectangle in it until the quit signal is
 * received.
 */
int main( int argc, char* argv[] )
{
  init();

  // Here we create the window; it will be immediately visible.
  // The engine does not support multiples windows yet, so be careful and be
  // sure to create only one of them.
  //
  // As soon as the screen is created, a render thread is created 
  bear::visual::screen s( claw::math::coordinate_2d<unsigned int>(640, 480) );

  // Rendering is done using scene elements, representing something to be drawn
  // on the screen. The scene_rectangle class represents a rectangle, either
  // filled or outlined.
  //
  // In the coordinate system of the screen, the bottom-left corner is the zero.
  // The top right corner is equal to the size passed to the constructor.
  bear::visual::scene_rectangle r
    ( /* x-origin relatively to which the element is placed in the screen. */
      320,
      /* y-origin relatively to which the element is placed in the screen. */
      240,
      /* the color of the rectangle */
      bear::visual::color_type( 0, 255, 0, 255 ),
      /* the coordinates of the rectangle, relatively to the first two
         arguments. Values are: left, bottom, right, top. */
      bear::visual::rectangle_type( -50, -50, 50, 50 ) );

  // This simple render loop draws repeatedly the rectangle on the screen, until
  // the quit signal is received.
  while( !g_quit )
    {
      s.begin_render();
      s.render( r );
      s.end_render();

      SDL_PumpEvents();
    }

  return 0;
}
