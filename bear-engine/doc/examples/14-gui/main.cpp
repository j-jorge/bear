/**
 * \file
 *
 * This example program creates a window and displays an instance of each widget
 * available in bear::gui.
 */

#include "gui/button.hpp"
#include "gui/checkbox.hpp"
#include "gui/callback_function.hpp"
#include "gui/horizontal_flow.hpp"

#include "input/input_status.hpp"
#include "input/mouse.hpp"
#include "input/system.hpp"

#include "visual/font/font_manager.hpp"
#include "visual/screen.hpp"
#include "visual/scene_sprite.hpp"

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
 * \param clip The part of the image to use for the sprite, relatively to the
 *        top-left corner of the texture.
 */
bear::visual::sprite load_sprite
( const bear::visual::sprite::clip_rectangle_type& clip )
{
  const std::string file_name( "sprites.png" );
  
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
 * Applies the default border and fill colors to a widget.
 */
void apply_skin( bear::gui::visual_component& widget )
{
  widget.set_top_left_border_color( bear::gui::color_type( "#a0a0a0" ) );
  widget.set_bottom_right_border_color( bear::gui::color_type( "#1b1b1b" ) );
  widget.set_background_color( bear::gui::color_type( "#3b3b3b" ) );
}

/**
 * Loads a font given its name.
 * \param font_file_name The patht to the true type font file to load.
 */
void load_font( const std::string& font_file_name )
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
}

/**
 * Returns the font to use to display the texts in the widgets.
 */
bear::visual::font get_default_font()
{
  const std::string font_file_name("FreeSans.ttf");

  if ( !g_font_manager.exists( font_file_name ) )
    load_font( font_file_name );

  return g_font_manager.get_font( font_file_name, 16 );
}

/**
 * Creates a button which sets g_quit to true when it is clicked.
 */
bear::gui::visual_component* create_quit_button()
{
  // The widgets must be dynamically allocated when they are inserted into
  // another widget.
  bear::gui::button* result
    ( new bear::gui::button
      ( // The font to use to display the label of the button.
        get_default_font(),
        // The label in the button.
        "Quit" ) );

  // This sets the spacing between the label and the edges of the button.
  result->set_margin( 10 );

  apply_skin( *result );

  // When the button is clicked, it calls the callbacks passed to
  // bear::gui::button::add_callback(). Here we tell him to call quit().
  result->add_callback( bear::gui::callback_function_maker( &quit ) );

  return result;
}

/**
 * Creates a box with a tick mark which can be toggled. Also known as a
 * "checkbox".
 */
bear::gui::visual_component* create_checkbox()
{
  const bear::visual::sprite sprite_on
    ( load_sprite
      ( /* The part of the image to use for the sprite, relatively to the
           top-left corner of the texture. Values are: left, top, width,
           height. */
       bear::visual::sprite::clip_rectangle_type(55, 0, 23, 20) ) );

  const bear::visual::sprite sprite_off
    ( load_sprite
      ( bear::visual::sprite::clip_rectangle_type(32, 0, 23, 20) ) );

  bear::gui::checkbox* result
    ( new bear::gui::checkbox( sprite_off, sprite_on, get_default_font() ) );

  result->set_text("Click me!");

  return result;
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
      ( /* The part of the image to use for the sprite, relatively to the
           top-left corner of the texture. Values are: left, top, width,
           height. */
       bear::visual::sprite::clip_rectangle_type(0, 0, 32, 32) ) );

  bear::gui::horizontal_flow frame( 2, 2 );
  frame.set_bottom_left(10, 10);
  frame.set_size( s.get_size() - 2 * bear::visual::size_box_type(10, 10) );

  apply_skin( frame );

  frame.insert( create_quit_button() );
  frame.insert( create_checkbox() );

  // The bear::input::input_status class maintains a state of the inputs and can
  // notify instances of bear::input::input_listener of the changes.
  // Since bear::gui::visual_component is a bear::input::input_listener, it fits
  // perfectly.
  bear::input::input_status input;

  while ( !g_quit )
    {
      // update the state of the input devices.
      bear::input::system::get_instance().refresh();

      // check the changes in the inputs.
      input.read();

      // notify the frame about the state of the inputs.
      input.scan_inputs( frame );

      s.begin_render();
      {
        // bear::gui::visual_component::render builds the
        // bear::visual::scene_elements to display the widgets on the screen.
        bear::gui::visual_component::scene_element_list widget_display;
        frame.render( widget_display );

        // Once we have these elements, we can render them individually.
        for ( bear::gui::visual_component::scene_element_list::const_iterator it
                ( widget_display.begin() );
              it != widget_display.end();
              ++it )
          s.render( *it );

        // Then we need to render the mouse's cursor.
        const bear::visual::position_type cursor_position
          ( bear::input::system::get_instance().get_mouse().get_position() );

        s.render
          ( bear::visual::scene_sprite
            ( cursor_position.x, cursor_position.y - cursor.height(),
              cursor ) );
      }
      s.end_render();
    }

  g_font_manager.clear();
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
