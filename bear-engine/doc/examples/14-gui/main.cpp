/**
 * \file
 *
 * This example program creates a window and displays an instance of each widget
 * available in bear::gui.
 */

#include <bear/gui/button.hpp>
#include <bear/gui/checkbox.hpp>
#include <bear/gui/callback_function.hpp>
#include <bear/gui/horizontal_flow.hpp>
#include <bear/gui/multi_page.hpp>
#include <bear/gui/picture.hpp>
#include <bear/gui/radio_button.hpp>
#include <bear/gui/radio_group.hpp>
#include <bear/gui/slider.hpp>
#include <bear/gui/static_text.hpp>
#include <bear/gui/text_input.hpp>

#include <bear/input/input_status.hpp>
#include <bear/input/mouse.hpp>
#include <bear/input/system.hpp>

#include <bear/visual/font/font_manager.hpp>
#include <bear/visual/screen.hpp>
#include <bear/visual/scene_sprite.hpp>

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
 * Creates a sprite from the widgets sprite sheed given a region in this image.
 * \param clip The part of the image to use for the sprite, relatively to the
 *        top-left corner of the texture.
 */
bear::visual::sprite load_widget_sprite
( const bear::visual::sprite::clip_rectangle_type& clip )
{
  return load_sprite( "sprites.png", clip );
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
 * Applies the border and fill colors to a widget such that it appears with a
 * hollow effect.
 */
void apply_hollow_skin( bear::gui::visual_component& widget )
{
  widget.set_top_left_border_color( bear::gui::color_type( "#1b1b1b" ) );
  widget.set_bottom_right_border_color( bear::gui::color_type( "#a0a0a0" ) );
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
 * Creates a button with a given label.
 * \param label The label to display in the button.
 */
bear::gui::button* create_button( const std::string& label )
{
  // The widgets must be dynamically allocated when they are inserted into
  // another widget.
  bear::gui::button* result
    ( new bear::gui::button
      ( // The font to use to display the label of the button.
        get_default_font(),
        // The label in the button.
        label ) );

  // This sets the spacing between the label and the edges of the button.
  result->set_margin( 10 );

  apply_skin( *result );

  return result;
}

/**
 * Creates a button which sets g_quit to true when it is clicked.
 */
bear::gui::visual_component* create_quit_button()
{
  bear::gui::button* const result( create_button( "Quit" ) );

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
    ( load_widget_sprite
      ( /* The part of the image to use for the sprite, relatively to the
           top-left corner of the texture. Values are: left, top, width,
           height. */
       bear::visual::sprite::clip_rectangle_type(55, 0, 23, 20) ) );

  const bear::visual::sprite sprite_off
    ( load_widget_sprite
      ( bear::visual::sprite::clip_rectangle_type(32, 0, 23, 20) ) );

  bear::gui::checkbox* result
    ( new bear::gui::checkbox( sprite_off, sprite_on, get_default_font() ) );

  result->set_text("You can click me!");

  return result;
}

/**
 * \brief Creates radio button: a box with a tick mark which can be exclusively
 *        selected among the radio buttons of the group in which it will be
 *        inserted.
 * \param label The label to display next to the button.
 */
bear::gui::radio_button* create_radio_button( std::string label )
{
  const bear::visual::sprite sprite_on
    ( load_widget_sprite
      ( /* The part of the image to use for the sprite, relatively to the
           top-left corner of the texture. Values are: left, top, width,
           height. */
       bear::visual::sprite::clip_rectangle_type(88, 15, 15, 15) ) );

  const bear::visual::sprite sprite_off
    ( load_widget_sprite
      ( bear::visual::sprite::clip_rectangle_type(88, 0, 15, 15) ) );

  bear::gui::radio_button* const result
    ( new bear::gui::radio_button
      ( sprite_off, sprite_on, get_default_font() ) );

  result->set_text(label);

  return result;
}

/**
 * \brief Creates a group of mutually exclusive choices.
 */
bear::gui::visual_component* create_radio_group()
{
  bear::gui::radio_group* result( new bear::gui::radio_group );
  result->set_size
    ( std::numeric_limits<bear::gui::size_type>::max(),
      std::numeric_limits<bear::gui::size_type>::max() );

  const bear::gui::size_type margin(10);

  result->add_button( create_radio_button("Option 3"), margin );
  result->add_button( create_radio_button("Option 2"), margin );
  result->add_button( create_radio_button("Option 1"), margin );

  result->fit( 10 );
  apply_skin( *result );

  return result;
}

/**
 * \brief Creates a component which displays a sprite.
 */
bear::gui::visual_component* create_picture()
{
    const bear::visual::sprite sprite
    ( load_sprite
      ( "hourglass.png",
        /* The part of the image to use for the sprite, relatively to the
           top-left corner of the texture. Values are: left, top, width,
           height. */
        bear::visual::sprite::clip_rectangle_type(0, 0, 57, 65) ) );

  bear::gui::picture* const result( new bear::gui::picture( sprite ) );

  apply_skin( *result );

  return result;
}

/**
 * This function object calls bear::gui::multi_page::next() on a given
 * multi_page instance.
 */
class next_page
{
private:
  /** The instance on which we call the function. */
  bear::gui::multi_page& multi_page;

public:
  explicit next_page( bear::gui::multi_page& m )
    : multi_page(m)
  {}

  void operator()() { multi_page.next(); }
};

/**
 * This function object calls bear::gui::multi_page::previous() on a given
 * multi_page instance.
 */
struct previous_page
{
private:
  /** The instance on which we call the function. */
  bear::gui::multi_page& multi_page;

public:
  explicit previous_page( bear::gui::multi_page& m )
    : multi_page(m)
  {}

  void operator()() { multi_page.previous(); }
};

/**
 * Creates an object which displays a long text and which can be scrolled up and
 * down with two buttons.
 */
bear::gui::visual_component* create_multi_page()
{
  const bear::gui::size_type margin( 10 );

  bear::gui::button* const previous( create_button( "Previous" ) );
  previous->set_bottom_left( 0, 0 );

  bear::gui::button* const next( create_button( "Next" ) );
  next->set_bottom_left( previous->right() + margin, previous->bottom() );

  bear::gui::visual_component* const result( new bear::gui::visual_component );
  result->set_size
    ( previous->width() + next->width() + margin, 800 );

  result->insert( previous );
  result->insert( next );

  bear::gui::multi_page* const multi_page
    ( new bear::gui::multi_page( get_default_font() ) );

  apply_hollow_skin( *multi_page );
  multi_page->set_bottom_left( previous->left(), previous->top() + margin );
  const bear::gui::size_type page_size( next->right() - previous->left() );

  multi_page->set_size( page_size, page_size / 2 );
  multi_page->set_text
    ( "Click the buttons below to scroll this very long text. "
      "This component will help you to present long textual descriptions to the"
      " player." );

  result->insert( multi_page );
  result->fit();

  previous->add_callback
    ( bear::gui::callback_function_maker( previous_page( *multi_page ) ) );
  next->add_callback
    ( bear::gui::callback_function_maker( next_page( *multi_page ) ) );

  return result;
}

/**
 * \brief Creates radio button: a box with a tick mark which can be exclusively
 *        selected among the radio buttons of the group in which it will be
 *        inserted.
 * \param label The label to display next to the button.
 */
bear::gui::visual_component* create_slider()
{
  const bear::visual::sprite bar
    ( load_widget_sprite
      ( /* The part of the image to use for the sprite, relatively to the
           top-left corner of the texture. Values are: left, top, width,
           height. */
       bear::visual::sprite::clip_rectangle_type(78, 22, 1, 3) ) );

  const bear::visual::sprite marker
    ( load_widget_sprite
      ( bear::visual::sprite::clip_rectangle_type(78, 0, 10, 22) ) );

  bear::gui::slider<double>* const result
    ( new bear::gui::slider<double>( bar, marker, -1, 2, 0 ) );

  result->set_size( 100, 24 );

  return result;
}

/**
 * \brief Creates a component displaying a non editable text.
 */
bear::gui::visual_component* create_label()
{
  bear::gui::static_text* const result
    ( new bear::gui::static_text( get_default_font() ) );

  result->set_auto_size( true );
  result->set_text( "This text cannot be edited" );

  return result;
}

/**
 * \brief Creates a component in which the user can enter some text.
 */
bear::gui::visual_component* create_text_input()
{
  bear::gui::text_input* const result
    ( new bear::gui::text_input
      ( get_default_font(), bear::gui::color_type("#C00000") ) );

  result->set_width( 200 );
  result->set_text( "Enter some text here" );

  apply_hollow_skin( *result );

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
    ( load_widget_sprite
      ( /* The part of the image to use for the sprite, relatively to the
           top-left corner of the texture. Values are: left, top, width,
           height. */
       bear::visual::sprite::clip_rectangle_type(0, 0, 32, 32) ) );

  bear::gui::visual_component root;
  root.set_size( s.get_size() );

  bear::gui::horizontal_flow* frame( new bear::gui::horizontal_flow( 20, 20 ) );
  root.insert( frame );

  frame->set_bottom_left(40, 115);
  frame->set_size( s.get_size() - 2 * bear::visual::size_box_type(40, 115) );

  apply_skin( *frame );

  frame->insert( create_multi_page() );
  frame->insert( create_radio_group() );
  frame->insert( create_picture() );
  frame->insert( create_checkbox() );
  frame->insert( create_slider() );
  frame->insert( create_label() );
  frame->insert( create_text_input() );
  frame->insert( create_quit_button() );

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
      input.scan_inputs( root );

      s.begin_render();
      {
        // bear::gui::visual_component::render builds the
        // bear::visual::scene_elements to display the widgets on the screen.
        bear::gui::visual_component::scene_element_list widget_display;
        root.render( widget_display );

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
