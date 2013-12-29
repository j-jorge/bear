/**
 * \file
 *
 * This example program creates a window and displays a text in the middle of
 * it.
 *
 * In order to make this example minimalistic, it will not use most of the
 * functionalities of the Bear Engine. Consequently, we will have manage the
 * exit of the program ourselves.
 *
 * The SDL is used for the subsystems of the engine. Thus we can use its event
 * system to detect when the user wants to quit the program.
 */

#include "visual/font/font_manager.hpp"
#include "visual/scene_writing.hpp"
#include "visual/screen.hpp"

#include "SDL2/SDL.h"

#include <fstream>

/**
 * This global variable is observed in the main loop in order to exit the
 * program as it becomes true.
 */
bool g_quit(false);

/**
 * The font manager handles the loading of the fonts and their use at various
 * sizes. One can do without it but it is really easier with.
 */
bear::visual::font_manager g_font_manager;

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
 * Creates a writing given a font file name, a text and the size at which the
 * text must be rendered.
 * \param font_file_name The path to the font.
 * \param writing_text The text to display.
 * \param font_size The size at which the text must be displayed.
 */
bear::visual::writing create_writing
( const std::string& font_file_name, const std::string& writing_text,
  double font_size )
{
  // Open the font file.
  std::ifstream f( font_file_name.c_str() );

  if ( !f )
    std::cerr << "Cannot open file \"" << font_file_name
              << "\" from current directory.\n";

  // We pass the font file to the font manager and let it handle the loading.
  // The first argument is a unique identifier for the font. Here we use the
  // file's name.
  g_font_manager.load_font( font_file_name, f );

  // And now we can get the font to render a text at a given size.
  bear::visual::font font
    ( g_font_manager.get_font( font_file_name, font_size ) );

  // Once that we have the font, it is easy to create the writing. By default,
  // the text is aligned on the top-left. See the documentation to limit the
  // text to a given rectangle, with automatic word wrapping, and apply other
  // alignments horizontally and vertically.
  return bear::visual::writing( font, writing_text );
}

/**
 * Creates a window and draws a text in it until the quit signal is received.
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

  const bear::visual::writing writing
    ( create_writing
      ( /* The path to the font to use. */
       "FreeSansBold.ttf",
        /* The text to display. */
       "Some text,\non\nmultiple\nlines.",
       /* The size of the font. */
       24 ) );

  // The scene_writing is the scene element which places a writing on the
  // screen. Here we set its position such that the text is centered in the
  // window.
  bear::visual::scene_writing element
    ( 320 - writing.get_width() / 2, 240 - writing.get_height() / 2, writing );

  while ( !g_quit )
    {
      s.begin_render();
      s.render( element );
      s.end_render();

      SDL_PumpEvents();
    }

  return 0;
}
