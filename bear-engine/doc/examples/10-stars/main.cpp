/**
 * \file
 *
 * This example program creates a window and displays a star in the middle of
 * it.
 *
 * In order to make this example minimalistic, it will not use most of the
 * functionalities of the Bear Engine. Consequently, we will have manage the
 * exit of the program ourselves.
 *
 * The SDL is used for the subsystems of the engine. Thus we can use its event
 * system to detect when the user wants to quit the program.
 */

#include "visual/screen.hpp"
#include "visual/scene_star.hpp"

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
 * Releases the visual and input modules of the engine.
 */
void release()
{
  bear::visual::screen::release();
}

/**
 * Creates and returns a star. Its position is set suchat that it is centered on
 * the screen.
 */
bear::visual::scene_star create_star()
{
  // The bear::visual.:star object describes the branches of the star (number
  // and length).
  const bear::visual::star star
    ( // the number of branches on the star.
      5,
      // The ratio of the inside of the inner diameter and the outer diameter.
      // The lower it is, the longer the branches are.
      0.5 );

  // The size that will be assigned to the scene_star.
  const double width( 100 );
  const double height( 100 );

  // The bear::visual::scene_star object draws a bear::visual::star.
  bear::visual::scene_star result
    ( // The x-coordinate of the bottom-left of the bounding box of the star.
      320 - width / 2,
      // The y-coordinate of the bottom-left of the bounding box of the star.
      240 - height / 2,
      // The color to use to draw the border of the star.
      bear::visual::color( "#FAFAFA" ),
      // The star to draw
      star,
      // The width of the border of the star.
      3.5,
      // The color to use to fill the star.
      bear::visual::color( "#F0F000" ) );

  // The size of the star. If the width and the height are different, the star
  // will be stretched.
  result.get_rendering_attributes().set_size( width, height );

  return result;
}

/**
 * Creates a window and draws a star in it until the quit signal is received.
 */
void run_example()
{
  // Here we create the window; it will be immediately visible.
  // The engine does not support multiples windows yet, so be careful and be
  // sure to create only one of them.
  //
  // As soon as the screen is created, a render thread is created 
  bear::visual::screen s( claw::math::coordinate_2d<unsigned int>(640, 480) );

  const bear::visual::scene_star star( create_star() );

  while ( !g_quit )
    {
      SDL_PumpEvents();
        
      s.begin_render();
      s.render( star );
      s.end_render();
    }
}

/**
 * Initializes the engine then runs the example. The engine's modules will be
 * released before leaving.
 */
int main( int argc, char* argv[] )
{
  init();

  run_example();

  release();

  return 0;
}
