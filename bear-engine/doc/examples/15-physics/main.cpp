/**
 * \file
 *
 * This example program creates a window and displays an instance of each widget
 * available in bear::gui.
 */

#include "input/input_listener.hpp"
#include "input/input_status.hpp"
#include "input/mouse.hpp"
#include "input/system.hpp"
#include "time/time.hpp"
#include "universe/physical_item.hpp"
#include "universe/world.hpp"
#include "visual/scene_sprite.hpp"
#include "visual/screen.hpp"

#include <fstream>

/**
 * Creates a sprite given an image file name and a region in this image.
 * \param clip The part of the image to use for the sprite, relatively to the
 *        top-left corner of the texture.
 */
bear::visual::sprite load_sprite
( const bear::visual::sprite::clip_rectangle_type& clip )
{
  // Open the image file.
  std::ifstream f( "sprites.png" );

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

class ship:
  public bear::universe::physical_item,
  public bear::input::input_listener
{
private:
  bear::visual::sprite _ship_sprite;

  bool _engine_is_activated;

  bool _left_jet_is_activated;
  bool _right_jet_is_activated;

public:
  ship()
    : _engine_is_activated( false ), _left_jet_is_activated( false ),
      _right_jet_is_activated( false )
  {
    set_size( 45, 45 );
    set_mass( 1000 /* kg */ );
    set_friction( 1 );

    _ship_sprite =
      load_sprite( bear::visual::sprite::clip_rectangle_type( 0, 0, 38, 56 ) );
  }

  void time_step( bear::universe::time_type time_in_seconds ) override
  {
    if ( _engine_is_activated )
      {
        const bear::universe::force_type force_in_newtons( 5000000, 0 );
        add_internal_force( force_in_newtons * time_in_seconds );
      }

    if ( _left_jet_is_activated )
      add_angular_speed( -0.05 );

    if ( _right_jet_is_activated )
      add_angular_speed( 0.05 );
  }
 
  bear::visual::sprite get_display() const
  {
    bear::visual::sprite result( _ship_sprite );
    result.set_angle( get_system_angle() );
    return result;
  }

  bool key_pressed( const bear::input::key_info& key ) override
  {
    if ( key.is_up() )
      _engine_is_activated = true;
    else if ( key.is_left() )
      _right_jet_is_activated = true;
    else if ( key.is_right() )
      _left_jet_is_activated = true;
  }

  bool key_released( const bear::input::key_info& key ) override
  {
    if ( key.is_up() )
      _engine_is_activated = false;
    else if ( key.is_left() )
      _right_jet_is_activated = false;
    else if ( key.is_right() )
      _left_jet_is_activated = false;
  }

};

class asteroid:
  public bear::universe::physical_item
{

};

class game:
  public bear::input::input_listener
{
public:
  game()
    : m_quit( false ), m_screen_size( 1024, 575 ), m_screen( m_screen_size ),
      m_camera_position( 50, 50 ),
      m_world_size( m_screen_size + 2 * m_camera_position ),
      m_world( m_world_size )
  {
    m_world.set_gravity( bear::universe::force_type( 0, 0 ) );

    m_player_ship.set_center_of_mass( m_camera_position + m_screen_size / 2 );
    m_world.register_item( &m_player_ship );
  }

  void run()
  {
    bear::systime::milliseconds_type last_update
      ( bear::systime::get_date_ms() );
    const bear::systime::milliseconds_type time_step( 1000.0 / 30 );
 
    while ( !m_quit )
      {
        const bear::systime::milliseconds_type now
          ( bear::systime::get_date_ms() );
      
        if ( now - last_update < time_step )
          bear::systime::sleep( time_step - ( now - last_update ) );

        update_inputs();
        update_world( double( now - last_update ) / 1000 );
        
        last_update = now;

        render();
    }
  }

private:
  bool key_pressed( const bear::input::key_info& key ) override
  {
    if ( key.is_escape() )
      m_quit = true;
  }

  void update_inputs()
  {
    // update the state of the input devices.
    bear::input::system::get_instance().refresh();

    // check the changes in the inputs.
    m_input.read();

    // notify the listeners about the changes in the inputs.
    m_input.scan_inputs( m_player_ship );
    m_input.scan_inputs( *this );
  }

  void update_world( double time_step )
  {
    bear::universe::world::region_type region;
    region.push_back
      ( bear::universe::rectangle_type
        ( 0, 0, m_world.get_size().x, m_world.get_size().y ) );
    m_world.progress_entities( region, time_step );

    loop_entities_over_world();
  }

  void loop_entities_over_world()
  {
    const bear::universe::rectangle_type inside
      ( m_camera_position / 2, m_camera_position * 3 / 2 + m_screen_size );

    loop_entities_out_of_region( inside );
  }

  void loop_entities_out_of_region( bear::universe::rectangle_type region )
  {
    const bear::universe::size_box_type offset
      ( ( m_world_size - region.size() ) / 2 );

    if ( m_player_ship.get_horizontal_middle() < region.left() )
      m_player_ship.set_horizontal_middle
        ( region.right() - offset.x
          + m_player_ship.get_horizontal_middle() );
    else if ( m_player_ship.get_horizontal_middle() > region.right() )
      m_player_ship.set_horizontal_middle
        ( offset.x + m_player_ship.get_horizontal_middle() - region.right() );

    if ( m_player_ship.get_vertical_middle() < region.bottom() )
      m_player_ship.set_vertical_middle
        ( region.top() - offset.y
          + m_player_ship.get_vertical_middle() );
    else if ( m_player_ship.get_vertical_middle() > region.top() )
      m_player_ship.set_vertical_middle
        ( offset.y + m_player_ship.get_vertical_middle() - region.top() );
  }

  void render()
  {
    m_screen.begin_render();
    {
      const bear::visual::sprite ship_sprite( m_player_ship.get_display() );
      const bear::visual::position_type ship_position
        ( m_player_ship.get_center_of_mass() - m_camera_position
          - ship_sprite.get_size() / 2 );

      m_screen.render
        ( bear::visual::scene_sprite
          ( ship_position.x, ship_position.y, ship_sprite ) );
    }
    m_screen.end_render();
  }

private:
  bool m_quit;

  const claw::math::coordinate_2d<unsigned int> m_screen_size;

  bear::visual::screen m_screen;

  const bear::universe::size_box_type m_camera_position;

  bear::input::input_status m_input;

  const bear::universe::size_box_type m_world_size;

  bear::universe::world m_world;

  ship m_player_ship;

}; // class game

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
 * Launches the game.
 */
void run_example()
{
  game game_instance;
  game_instance.run();
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
