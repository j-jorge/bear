/**
 * \file
 *
 * This example program creates a window and displays some sprites, with a
 * fragment shader applied to some of them.
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
#include <bear/visual/scene_shader_push.hpp>
#include <bear/visual/scene_shader_pop.hpp>

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

  // The bear::visual::shader_program loads a shader from a GLSL source file.
  std::ifstream shader_source( "grayscale.frag" );
  const bear::visual::shader_program shader( shader_source );

  // This element will be rendered several times, and sometimes with an active
  // shader.
  bear::visual::scene_sprite sprite_element
    ( 320 - sprite.width() / 2, 240 - sprite.height() / 2, sprite );

  while ( !g_quit )
    {
      s.begin_render();

      // First rendering of the sprite, without the shader.
      sprite_element.set_position
        ( 160 - sprite.width() / 2, sprite_element.get_position().y );
      s.render( sprite_element );

      // The bear::visual::scene_shader_push object activates a shader for the
      // upcoming drawings. Several shaders can be pushed at once, in which case
      // the last shader on the stack is the one applied.
      s.render( bear::visual::scene_shader_push( shader ) );

      // This sprite will be thus displayed in grayscale.
      sprite_element.set_position
        ( 320 - sprite.width() / 2, sprite_element.get_position().y );
      s.render( sprite_element );

      // The bear::visual::scene_shader_pop() object removes the shader on the
      // top of the shader stack. Here the upcoming drawings will be done
      // without shaders.
      s.render( bear::visual::scene_shader_pop() );

      // This sprite will be rendered in normal colors, like the first one.
      sprite_element.set_position
        ( 480 - sprite.width() / 2, sprite_element.get_position().y );
      s.render( sprite_element );

      s.end_render();

      SDL_PumpEvents();
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
