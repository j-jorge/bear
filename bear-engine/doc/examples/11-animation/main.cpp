/**
 * \file
 *
 * This example program creates a window and displays an animation in the middle
 * of it.
 *
 * In order to make this example minimalistic, it will not use most of the
 * functionalities of the Bear Engine. Consequently, we will have manage the
 * exit of the program ourselves.
 *
 * The SDL is used for the subsystems of the engine. Thus we can use its event
 * system to detect when the user wants to quit the program.
 */

#include <bear/visual/animation.hpp>
#include <bear/visual/screen.hpp>
#include <bear/visual/scene_sprite.hpp>

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
 * Creates an eight-frames animation from the first raw of the file
 * top-down-green-dragon-animated.png located in the current directory.
 */
bear::visual::animation load_animation()
{
  const std::string file_name( "top-down-green-dragon-animated.png" );

  std::cout << "Animation by Warlock's Gauntlet team. "
    " See http://opengameart.org/content/top-down-green-dragon-animated\n";

  // Open the image file containing the frames of the animation.
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

  // The animation's constructor requires a sequence of sprites and their
  // corresponding display duration. We will build these sprites using this clip
  // rectangle which we will update for each of the height frames at the top of
  // the texture.
  bear::visual::sprite::clip_rectangle_type clip( 0, 0, 128, 128 );

  // The sequence of sprites in the animation.
  std::vector<bear::visual::sprite> frames;

  // The frames of the animation make the first row of the image. We create a
  // sprite for each position of clip incremented by its width.
  for ( ; clip.position.x + clip.width <= texture.width();
        clip.position.x += clip.width )
    frames.push_back
      ( bear::visual::sprite
        ( /* The texture of which the sprite is part of. */
         texture,
         /* The part of the image to use for the sprite. */
         clip ) );

  // Finally we can build the animation from the sprites and their
  // durations. Here the durations of the frames are equal (the 25th of a
  // second), but it is not necessary. By default the animation is played
  // indefinitely.
  return bear::visual::animation
    ( frames, std::vector<double>( frames.size(), 1.0 / 25 ) );
}

/**
 * Creates a window and plays an animation in it until the quit signal is
 * received.
 */
void run_example()
{
  // Here we create the window; it will be immediately visible.
  // The engine does not support multiples windows yet, so be careful and be
  // sure to create only one of them.
  //
  // As soon as the screen is created, a render thread is created
  bear::visual::screen s( claw::math::coordinate_2d<unsigned int>(640, 480) );

  // This is the animation we will display.
  bear::visual::animation animation( load_animation() );

  // We need to keep track of the elapsed time between each update in order to
  // synchronize the animation.
  bear::systime::milliseconds_type last_update( bear::systime::get_date_ms() );

  while ( !g_quit )
    {
      const bear::systime::milliseconds_type now
        ( bear::systime::get_date_ms() );

      // Update the animation forward of a given duration. We convert the time
      // to seconds to match the 25 fps expected at the construction of the
      // animation in load_animation().
      animation.next( double(now - last_update) / 1000 );
      last_update = now;

      // Display the current sprite of the animation.
      bear::visual::scene_sprite element
        ( 320 - animation.width() / 2, 240 - animation.height() / 2,
          // bear::visual::animation::get_sprite() returns the sprite reached by
          // the last update of the animation.
          animation.get_sprite() );

      s.begin_render();
      s.render( element );
      s.end_render();

      SDL_PumpEvents();

      // Wait a short time before the next update.
      bear::systime::sleep(1);
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
