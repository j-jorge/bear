/**
 * \file
 *
 * This example program creates a window and plays two sounds randomly.
 *
 * In order to make this example minimalistic, it will not use most of the
 * functionalities of the Bear Engine. Consequently, we will have manage the
 * exit of the program ourselves.
 *
 * The SDL is used for the subsystems of the engine. Thus we can use its event
 * system to detect when the user wants to quit the program.
 */

#include <bear/visual/screen.hpp>
#include <bear/audio/sound_manager.hpp>
#include <bear/time/time.hpp>

#include <SDL2/SDL.h>

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
 * Loads a sound from a given file name.
 * \param sound_manager The sound manager in which the sound is loaded.
 * \param file_name The path to the sound file to open.
 * \param sound_name The name to give to the sound.
 */
void load_sound
( bear::audio::sound_manager& sound_manager, const std::string& file_name,
  const std::string& sound_name )
{
  std::ifstream f( file_name );
  sound_manager.load_sound( sound_name, f );
}

/**
 * Creates a window and plays the sounds randomly until the quit signal is
 * received.
 */
void run_example()
{
  init();

  // A window must be created to initialize the SDL. We do not need it for the
  // sound but for the events that will allow to close the application.
  //
  // Here we create it; it will be immediately visible.
  // The engine does not support multiples windows yet, so be careful and be
  // sure to create only one of them.
  //
  // As soon as the screen is created, a render thread is created
  bear::visual::screen s( claw::math::coordinate_2d<unsigned int>(640, 480) );

  // Sounds must be accessed through a sound manager. It will handle the loading
  // of the files and the releasing of the resources when the sound is over.
  bear::audio::sound_manager sound_manager;

  // We need to load the sound into the sound manager before being able to play
  // it.
  load_sound( sound_manager, "lava.ogg", "lava" );
  load_sound( sound_manager, "acid_burn.ogg", "acid" );

  std::cout << "Sound 'lava.ogg' by Michel Baradari. See "
    "http://opengameart.org/content/lava-splash.\n"
    "Sound 'acid_burn.ogg' by mikhog, based on work by Micher Bardari. See "
    "http://opengameart.org/content/acid-burn-sounds.\n";

  while ( !g_quit )
    {
      SDL_PumpEvents();

      const double lottery( (double)std::rand() / RAND_MAX );

      // Here we play one of the sounds once.
      if ( lottery < 0.3 )
        sound_manager.play_sound( "lava" );
      else if ( lottery < 0.6 )
        sound_manager.play_sound( "acid" );

      // wait 1 second before the next sound.
      bear::systime::sleep( 1000 );
    }
}

/**
 * Initializes the Bear Engine's modules then run the example. The modules are
 * released before leaving.
 */
int main( int argc, char* argv[] )
{
  init();

  run_example();

  release();

  return 0;
}
