/**
 * \file
 *
 * This example program creates a window and displays a sprite in the middle
 * of it. The sprite moves when the keyboard arrows are pressed.
 *
 * In order to make this example minimalistic, it will not use most of the
 * functionalities of the Bear Engine. Consequently, we will have manage the
 * exit of the program ourselves.
 *
 * The SDL is used for the subsystems of the engine. Thus we can use its event
 * system to detect when the user wants to quit the program.
 */

#include <bear/visual/screen.hpp>
#include <bear/visual/scene_sprite.hpp>
#include <bear/time/time.hpp>
#include <bear/input/system.hpp>
#include <bear/input/keyboard.hpp>

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
 * Initializes the visual and input modules of the engine and registers the
 * event filter to the SDL.
 */
void init()
{
  bear::visual::screen::initialize( bear::visual::screen::screen_gl );
  bear::input::system::initialize();

  SDL_EventState( SDL_QUIT, SDL_ENABLE );

  SDL_SetEventFilter( &event_filter, NULL );
}

/**
 * Releases the visual and input modules of the engine.
 */
void release()
{
  bear::input::system::release();
  bear::visual::screen::release();
}

/**
 * Creates a sprite given an image file name and a region in this image.
 * \param file_name The path to the image use for the sprite.
 * \param clip The part of the image to use for the sprite, relatively to the
 *        top-left corner of the texture.
 */
bear::visual::sprite load_sprite
( const std::string& file_name,
  const bear::visual::sprite::clip_rectangle_type& clip )
{
  // Open the image file.
  std::ifstream f( file_name.c_str() );

  if ( !f )
    std::cerr << "Cannot open file \"" << file_name
              << "\" from current directory.\n";

  // the engine uses the graphic module of libclaw to load the images. This
  // constructor builds a 32 bits color image from files such as PNG, JPEG, TGA,
  // Bitmap.
  claw::graphic::image image( f );

  // Then we create a texture from the loaded image.
  bear::visual::image texture( image );

  // And we build a sprite from this texture.
  return bear::visual::sprite
    ( /* The texture of which the sprite is part of. */
      texture,
      /* The part of the image to use for the sprite. */
      clip );
}

/**
 * Returns the movement distance to apply to the sprite according to the arrows
 * currently pressed on the keyboard.
 */
bear::visual::position_type read_sprite_movement()
{
  // the distance of the movement in the direction of the pressed arrows.
  const bear::visual::coordinate_type delta(1);

  // the result of the function. If no arrow is pressed, the sprite does not
  // move. Otherwise this value will be updated accordingly.
  bear::visual::position_type result(0, 0);

  // The state of the keyboard is supposed to be up to date.
  const bear::input::keyboard& keyboard
    ( bear::input::system::get_instance().get_keyboard() );

  // bear::input::keyboard has both the begin() and end() functions required to
  // use range-based loops. The value type of the returned iterator is a code
  // identifying a pressed key.
  for ( bear::input::key_code k : keyboard )
    switch (k)
      {
      case bear::input::keyboard::kc_up:
        result.y += delta;
        break;
      case bear::input::keyboard::kc_down:
        result.y -= delta;
        break;
      case bear::input::keyboard::kc_left:
        result.x -= delta;
        break;
      case bear::input::keyboard::kc_right:
        result.x += delta;
        break;
      }

  return result;
}

/**
 * Creates a window and draws a sprite in it until the quit signal is received.
 */
void run_example()
{
  // Here we create the window; it will be immediately visible.
  // The engine does not support multiples windows yet, so be careful and be
  // sure to create only one of them.
  //
  // As soon as the screen is created, a render thread is created
  bear::visual::screen s( claw::math::coordinate_2d<unsigned int>(640, 480) );

  const bear::visual::sprite sprite
    ( load_sprite
      ( /* The path to the image of which the sprite is a part. */
       "hourglass.png",
        /* The part of the image to use for the sprite, relatively to the
           top-left corner of the texture. Values are: left, top, width,
           height. */
        bear::visual::sprite::clip_rectangle_type(0, 0, 57, 65) ) );

  bear::visual::position_type sprite_center( s.get_size() / 2 );

  while ( !g_quit )
    {
      // update the state of the input devices.
      bear::input::system::get_instance().refresh();

      // compute the new center of the sprite.
      sprite_center += read_sprite_movement();

      // and build the scene element such that the sprite is centered on
      // sprite_center.
      bear::visual::scene_sprite element
        ( sprite_center.x - sprite.width() / 2,
          sprite_center.y - sprite.height() / 2, sprite );

      s.begin_render();
      s.render( element );
      s.end_render();

      // wait 15 milliseconds before the next update. It is necessary to avoid
      // the sprite to move too fast, in this example.
      bear::systime::sleep( 15 );
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
