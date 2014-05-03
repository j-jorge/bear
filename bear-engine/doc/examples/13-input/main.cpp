/**
 * \file
 *
 * This example program creates a window and prints a message in the terminal
 * for each input event.
 */

#include "input/input_listener.hpp"
#include "input/input_status.hpp"
#include "input/mouse.hpp"
#include "input/system.hpp"

#include "visual/scene_sprite.hpp"
#include "visual/screen.hpp"

#include <fstream>

/**
 * \brief Prints the details of a key event in a human readable way.
 * \param os The stream in which the details are printed.
 * \param key The event to print.
 */
std::ostream& operator<<( std::ostream& os, bear::input::key_info key )
{
  // The code of the key is its representation on the keyboard.
  // The symbol is what the user wants to see on the screen when he types.
  // The name is a human readable description of the code.
  return os << "code: " << key.get_code()
            << " symbol: " << (char)key.get_symbol()
            << " name: "
            << bear::input::keyboard::get_name_of( key.get_code() );
}

std::ostream& operator<<( std::ostream& os, bear::input::finger_event event )
{
  switch ( event.get_type() )
    {
    case bear::input::finger_event::finger_event_pressed:
      os << "pressed";
      break;
    case bear::input::finger_event::finger_event_released:
      os << "released";
      break;
    case bear::input::finger_event::finger_event_motion:
      os << "motion";
      break;
    }

  return os << " at position x=" << event.get_position().x << ", y="
            << event.get_position().y << ", and distance x="
            << event.get_distance().x << ", y=" << event.get_distance().y;
}

/**
 * \brief The input_process class processes the inputs of the user by displaying
 *        a description of them for each event.
 *
 * bear::input::input_listener is the interface of the objects that can be
 * passed to bear::input::input_status in order to be informed when the
 * keyboard, the joysticks, the mouses or the finger (on touch devices) are
 * used.
 *
 * For the button based devices, the interfaces provides methods called when the
 * buttons are either pressed, maintained (since the last call) and released.
 *
 * For the finger, a single function is provided, taking an argument telling if
 * the finger is pressed, released or if it is moving.
 *
 * We will use the input_processor to manage the position of the mouse, where
 * the cursor must be displayed.
 */
class input_processor:
  public bear::input::input_listener
{
public:
  /**
   * \brief Default constructor. The mouse is initially placed in (0, 0).
   */
  input_processor()
    : m_mouse_position( 0, 0 )
  { }

  /**
   * \brief Returns the last known position of the mouse.
   */
  claw::math::coordinate_2d<unsigned int> get_mouse_position() const
  {
    return m_mouse_position;
  }
  
protected:
  /**
   * \brief Displays the details of a pressed keyboard key.
   * \param key The details of the key.
   */
  virtual bool key_pressed( const bear::input::key_info& key )
  {
    std::cout << "keyboard key pressed: " << key << '\n';
  }

  /**
   * \brief Displays the details of a released keyboard key.
   * \param key The details of the key.
   */
  virtual bool key_released( const bear::input::key_info& key )
  {
    std::cout << "keyboard key released: " << key << '\n';
  }

  /**
   * \brief Displays the details of a text symbol entered by the user.
   * \param key The details of the symbol.
   */
  virtual bool char_pressed( const bear::input::key_info& key )
  {
    std::cout << "character pressed: " << key << '\n';
  }

  /**
   * \brief Displays the details of a pressed joystick button.
   * \param button The details of the button.
   * \param joy_index The index of the joystick emitting the event.
   */
  virtual bool button_pressed
  ( bear::input::joystick::joy_code button, unsigned int joy_index )
  {
    std::cout << "joystick #" << joy_index << " button pressed: "
              << bear::input::joystick::get_name_of(button) << '\n';
  }

  /**
   * \brief Displays the details of a released joystick button.
   * \param button The details of the button.
   * \param joy_index The index of the joystick emitting the event.
   */
  virtual bool button_released
  ( bear::input::joystick::joy_code button, unsigned int joy_index )
  {
    std::cout << "joystick #" << joy_index << " button released: "
              << bear::input::joystick::get_name_of(button) << '\n';
  }

  /**
   * \brief Displays the details of a pressed mouse button.
   * \param button The details of the button.
   * \param pos The position of the mouse when the button is pressed.
   */
  virtual
  bool mouse_pressed
  ( bear::input::mouse::mouse_code button,
    const claw::math::coordinate_2d<unsigned int>& pos )
  {
    std::cout << "mouse button pressed: "
              << bear::input::mouse::get_name_of(button) << " at x="
              << pos.x << " y=" << pos.y << '\n';
  }

  /**
   * \brief Displays the details of a released mouse button.
   * \param button The details of the button.
   * \param pos The position of the mouse when the button is released.
   */
  virtual
  bool mouse_released
  ( bear::input::mouse::mouse_code button,
    const claw::math::coordinate_2d<unsigned int>& pos )
  {
    std::cout << "mouse button released: "
              << bear::input::mouse::get_name_of(button) << " at x="
              << pos.x << " y=" << pos.y << '\n';
  }

  /**
   * \brief Displays and stores the position of the mouse.
   * \param pos The position where the mouse has moved.
   */
  virtual
  bool mouse_move( const claw::math::coordinate_2d<unsigned int>& pos )
  {
    std::cout << "mouse moving at x=" << pos.x << " y=" << pos.y << '\n';

    m_mouse_position = pos;
  }

  /**
   * \brief Displays the details of a touch event.
   * \param event The details of the event.
   */
  virtual bool finger_action( const bear::input::finger_event& event )
  {
    std::cout << "finger action: " << event << '\n';
  }

private:
  /** \brief The last known position of the mouse. */
  claw::math::coordinate_2d<unsigned int> m_mouse_position;

};

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
 * Sets g_quit to true.
 */
void quit()
{
  g_quit = true;
}

/**
 * Initializes the visual and input modules of the engine.
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
 * \param file_name The path to the image file of which the sprite is a part.
 * \param clip The part of the image to use for the sprite, relatively to the
 *        top-left corner of the texture.
 */
bear::visual::sprite load_sprite
( std::string file_name, const bear::visual::sprite::clip_rectangle_type& clip )
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
 * Creates a window with the widgets, then call the game loop.
 */
void run_example()
{
  // Here we create the window; it will be immediately visible.
  // The engine does not support multiples windows yet, so be careful and be
  // sure to create only one of them.
  //
  // As soon as the screen is created, a render thread is created 
  bear::visual::screen s( claw::math::coordinate_2d<unsigned int>(640, 480) );

  bear::visual::sprite cursor
    ( load_sprite
      ( "cursor.png",
        /* The part of the image to use for the sprite, relatively to the
           top-left corner of the texture. Values are: left, top, width,
           height. */
       bear::visual::sprite::clip_rectangle_type(0, 0, 32, 32) ) );

  // The bear::input::input_status class maintains a state of the inputs and can
  // notify instances of bear::input::input_listener of the changes.
  bear::input::input_status input;

  input_processor processed_input;

  while ( !g_quit )
    {
      // update the state of the input devices.
      bear::input::system::get_instance().refresh();

      // check the changes in the inputs.
      input.read();

      // notify the frame about the state of the inputs.
      input.scan_inputs( processed_input );

      s.begin_render();
      {
        // Then we need to render the mouse's cursor.
        const bear::visual::position_type cursor_position
          ( processed_input.get_mouse_position() );

        s.render
          ( bear::visual::scene_sprite
            ( cursor_position.x, cursor_position.y - cursor.height(),
              cursor ) );
      }
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
