/**
 * \file
 *
 * This example program creates a window and plays a music.
 *
 * In order to make this example minimalistic, it will not use most of the
 * functionalities of the Bear Engine. Consequently, we will have manage the
 * exit of the program ourselves.
 *
 * The SDL is used for the subsystems of the engine. Thus we can use its event
 * system to detect when the user wants to quit the program.
 */

#include "visual/screen.hpp"
#include "audio/sound_manager.hpp"

#include "SDL2/SDL.h"

#include <fstream>

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
 * Initializes the visual and audio modules of the engine and registers the
 * event filter to the SDL.
 */
void init()
{
  bear::visual::screen::initialize( bear::visual::screen::screen_gl );
  bear::audio::sound_manager::initialize();

  SDL_EventState( SDL_QUIT, SDL_ENABLE );

  SDL_SetEventFilter( &event_filter, NULL );
}

/**
 * Releases the visual and audio modules of the engine.
 */
void release()
{
  bear::audio::sound_manager::release();
  bear::visual::screen::release();
}

/**
 * Creates a window and draws a sprite in it until the quit signal is received.
 * The sprite is transformed in response to the mouse events.
 */
int main( int argc, char* argv[] )
{
  init();

  // A window must be created to initialize the SDL. We do not need it for the
  // music bud for the events that will allow to close the application.
  //
  // Here we create it; it will be immediately visible.
  // The engine does not support multiples windows yet, so be careful and be
  // sure to create only one of them.
  //
  // As soon as the screen is created, a render thread is created 
  bear::visual::screen s( claw::math::coordinate_2d<unsigned int>(640, 480) );

  // Musics must be accessed through a sound manager. It will handle the loading
  // of the files and the releasing of the resources when the music is over.
  bear::audio::sound_manager sound_manager;

  // We need to load the music into the sound manager before being able to play
  // it.
  std::ifstream f( "mus_1.ogg" );
  sound_manager.load_sound("music", f);

  std::cout << "Music by Alexandr Zhelanov. See "
    "http://opengameart.org/content/mus.\n";

  // Here we start the music. The second argument is the number of times it must
  // be played. Zero means to play it forever.
  sound_manager.play_music( "music", 0 );

  while ( !g_quit )
    SDL_PumpEvents();

  release();

  return 0;
}
