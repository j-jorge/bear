/**
 * \file
 *
 * This example program creates a window and displays a sprite in the middle
 * of it. The user can move the sprite using the mouse. A left or right click
 * respectively flips the sprite horizontally and vertically. Moving the wheel
 * up or down respectively increase and decrease the size of the sprite.
 *
 * In order to make this example minimalistic, it will not use most of the
 * functionalities of the Bear Engine. Consequently, we will have manage the
 * exit of the program ourselves.
 *
 * The SDL is used for the subsystems of the engine. Thus we can use its event
 * system to detect when the user wants to quit the program.
 */

#include "visual/screen.hpp"
#include "visual/scene_sprite.hpp"
#include "input/system.hpp"
#include "input/mouse.hpp"

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
 * \param file_name The path to the image used for the sprite.
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
 * Reads the state of the mouse's buttons and applies the corresponding
 * transformations to a sprite. A left or right click respectively flips the
 * sprite horizontally and vertically. Moving the wheel up or down respectively
 * increase and decrease the size of the sprite.
 * \param sprite The sprite to transform.
 */
void transform_sprite_with_mouse( bear::visual::sprite& sprite )
{
  // The state of the mouse is supposed to be up to date.
  const bear::input::mouse& mouse
    ( bear::input::system::get_instance().get_mouse() );

  // The scale that will be applied to the sprite. By default the scale doesn't
  // change.
  double scale( 1 );

  // bear::input::mouse has both the begin() and end() functions required to
  // use range-based loops. The value type of the returned iterator is a code
  // identifying a pressed button.
  for ( bear::input::mouse::mouse_code k : mouse )
    switch (k)
      {
      case bear::input::mouse::mc_left_button:
        sprite.mirror( !sprite.is_mirrored() );
        break;
      case bear::input::mouse::mc_right_button:
        sprite.flip( !sprite.is_flipped() );
        break;
      case bear::input::mouse::mc_wheel_up:
        scale = 1.2;
        break;
      case bear::input::mouse::mc_wheel_down:
        scale = 0.8;
        break;
      }

  bear::visual::size_box_type s( sprite.get_size() * scale );
  
  if ( (s.x > 10) && (s.y > 10) )
    sprite.set_size( s );
}

/**
 * Creates a window and draws a sprite in it until the quit signal is received.
 * The sprite is transformed in response to the mouse events.
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

  bear::visual::sprite sprite
    ( load_sprite
      ( /* The path to the image of which the sprite is a part. */
       "hourglass.png",
        /* The part of the image to use for the sprite, relatively to the
           top-left corner of the texture. Values are: left, top, width,
           height. */
        bear::visual::sprite::clip_rectangle_type(0, 0, 57, 65) ) );

  // The position of the center of the sprite in the screen.
  bear::visual::position_type sprite_center( s.get_size() / 2 );

  while ( !g_quit )
    {
      // update the state of the input devices.
      bear::input::system::get_instance().refresh();

      // compute the new center of the sprite.
      sprite_center =
        bear::input::system::get_instance().get_mouse().get_position();

      transform_sprite_with_mouse( sprite );

      // and build the scene element such that the sprite is centered on
      // sprite_center.
      bear::visual::scene_sprite element
        ( sprite_center.x - sprite.width() / 2,
          sprite_center.y - sprite.height() / 2, sprite );

      s.begin_render();
      s.render( element );
      s.end_render();
    }

  release();

  return 0;
}
