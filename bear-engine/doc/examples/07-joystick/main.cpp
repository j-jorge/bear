/**
 * \file
 *
 * This example program creates a window and displays a sprite in the middle
 * of it. The user can move the sprite using the stick of the first joystick's.
 * Two buttons are used to rotate the sprite clockwise and anticlockwise. Two
 * other buttons flip the sprite horizontally and vertically. Again, two other
 * buttons are used to increase and decrease the size of the sprite.
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
#include <bear/input/joystick.hpp>

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
int event_filter( void* user_data, SDL_Event* event )
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
 * Reads the state of the joystick's buttons and applies the corresponding
 * transformations to a sprite. The axis moves the sprite, two buttons changes
 * the angle, two buttons change the scale and two buttons flip the sprite
 * horizontally and vertically.
 * \param sprite The sprite to transform.
 * \param center The position of the center of the sprite.
 */
void transform_sprite_with_joystick
( bear::visual::sprite& sprite, bear::visual::position_type& center )
{
  // The state of the joystick is supposed to be up to date.
  const bear::input::joystick& joystick
    ( bear::input::system::get_instance().get_joystick(0) );

  // The scale that will be applied to the sprite. By default the scale doesn't
  // change.
  double scale( 1 );

  // The length of the movement to be applied on each pressed axis.
  const double movement_length(5);

  // The angle of the rotation.
  const double rotation_delta(0.1);

  // bear::input::joystick has both the begin() and end() functions required to
  // use range-based loops. The value type of the returned iterator is a code
  // identifying a pressed button.
  for ( bear::input::joystick::joy_code k : joystick )
    switch (k)
      {
      case bear::input::joystick::jc_axis_left:
        center.x -= movement_length;
        break;
      case bear::input::joystick::jc_axis_right:
        center.x += movement_length;
        break;
      case bear::input::joystick::jc_axis_down:
        center.y -= movement_length;
        break;
      case bear::input::joystick::jc_axis_up:
        center.y += movement_length;
        break;
      case bear::input::joystick::jc_axis_up_right:
        center +=
          bear::visual::position_type( movement_length, movement_length );
        break;
      case bear::input::joystick::jc_axis_down_right:
        center +=
          bear::visual::position_type( movement_length, -movement_length );
        break;
      case bear::input::joystick::jc_axis_down_left:
        center +=
          bear::visual::position_type( -movement_length, -movement_length );
        break;
      case bear::input::joystick::jc_axis_up_left:
        center +=
          bear::visual::position_type( -movement_length, movement_length );
        break;
      case bear::input::joystick::jc_button_1:
        sprite.set_angle( sprite.get_angle() + rotation_delta );
        break;
      case bear::input::joystick::jc_button_2:
        sprite.set_angle( sprite.get_angle() - rotation_delta );
        break;
      case bear::input::joystick::jc_button_3:
        sprite.mirror( !sprite.is_mirrored() );
        break;
      case bear::input::joystick::jc_button_4:
        sprite.flip( !sprite.is_flipped() );
        break;
      case bear::input::joystick::jc_button_5:
        scale = 1.2;
        break;
      case bear::input::joystick::jc_button_6:
        scale = 0.8;
        break;
      }

  bear::visual::size_box_type s( sprite.get_size() * scale );

  if ( (s.x > 10) && (s.y > 10) )
    sprite.set_size( s );
}

/**
 * \brief Creates a window with a sprite in it, then polls the joystick and
 *        transforms the sprite according to the inputs.
 */
void run_example()
{
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
      transform_sprite_with_joystick( sprite, sprite_center );

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
 * Initializes the Bear Engine's modules then run the example. The modules are
 * released before leaving.
 */
int main( int argc, char* argv[] )
{
  init();

  if ( bear::input::joystick::number_of_joysticks() == 0 )
    {
      std::cout << "No joystick found. Leaving.\n";
      release();
      return 1;
    }

  run_example();

  release();

  return 0;
}
