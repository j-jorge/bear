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
#include "universe/derived_item_handle.hpp"
#include "universe/physical_item.hpp"
#include "universe/world.hpp"
#include "visual/scene_sprite.hpp"
#include "visual/screen.hpp"

#include <fstream>

double random_number()
{
  return (double)std::rand() / RAND_MAX;
}

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

class game_item:
  public bear::universe::physical_item
{
public:
  bear::visual::sprite get_display() const
  {
    bear::visual::sprite result( get_display_sprite() );
    result.set_angle( get_system_angle() );
    return result;
  }

private:
  virtual bear::visual::sprite get_display_sprite() const = 0;
};

class ship:
  public game_item,
  public bear::input::input_listener
{
private:
  bear::visual::sprite m_ship_sprite;

  bool m_engine_is_activated;

  bool m_left_jet_is_activated;
  bool m_right_jet_is_activated;

public:
  ship()
    : m_engine_is_activated( false ), m_left_jet_is_activated( false ),
      m_right_jet_is_activated( false )
  {
    set_size( 45, 45 );
    set_mass( 1000 /* kg */ );
    set_friction( 1 );

    m_ship_sprite =
      load_sprite
      ( bear::visual::sprite::clip_rectangle_type( 412, 0, 38, 56 ) );
  }

private:
  void time_step( bear::universe::time_type time_in_seconds ) override
  {
    if ( m_engine_is_activated )
      {
        const bear::universe::force_type force_in_newtons( 5000000, 0 );
        add_internal_force( force_in_newtons * time_in_seconds );
      }

    if ( m_left_jet_is_activated )
      add_angular_speed( -0.05 );

    if ( m_right_jet_is_activated )
      add_angular_speed( 0.05 );
  }

  bear::visual::sprite get_display_sprite() const override
  {
    return m_ship_sprite;
  }

  bool key_pressed( const bear::input::key_info& key ) override
  {
    if ( key.is_up() )
      m_engine_is_activated = true;
    else if ( key.is_left() )
      m_right_jet_is_activated = true;
    else if ( key.is_right() )
      m_left_jet_is_activated = true;
  }

  bool key_released( const bear::input::key_info& key ) override
  {
    if ( key.is_up() )
      m_engine_is_activated = false;
    else if ( key.is_left() )
      m_right_jet_is_activated = false;
    else if ( key.is_right() )
      m_left_jet_is_activated = false;
  }

};

class asteroid:
  public game_item
{
private:
  const double m_scale;

  bear::visual::sprite m_asteroid_sprite;

public:
  asteroid()
    : asteroid( 1 )
  {
    
  }

private:
  explicit asteroid( double scale )
    : m_scale( scale )
  {
    pick_random_sprite();

    m_asteroid_sprite.set_size( m_asteroid_sprite.get_size() * scale );

    const bear::universe::size_type size
      ( std::min( m_asteroid_sprite.width(), m_asteroid_sprite.height() )
        - 20 );

    set_size( size, size );
    set_mass( scale * 10000 /* kg */ );
    set_friction( 1 );

    set_speed( -200 + random_number() * 500, -200 + random_number() * 500 );
  }

  bear::visual::sprite get_display_sprite() const override
  {
    return m_asteroid_sprite;
  }

  void pick_random_sprite()
  {
    bear::visual::sprite::clip_rectangle_type clip_rectangle;

    switch( std::rand() % 4 )
      {
      case 0:
        clip_rectangle =
          bear::visual::sprite::clip_rectangle_type( 0, 0, 120, 97 );
        break;
      case 1:
        clip_rectangle =
          bear::visual::sprite::clip_rectangle_type( 121, 0, 98, 95 );
        break;
      case 2:
        clip_rectangle =
          bear::visual::sprite::clip_rectangle_type( 220, 0, 101, 83 );
        break;
      case 3:
        clip_rectangle =
          bear::visual::sprite::clip_rectangle_type( 322, 0, 89, 81 );
        break;
      }

    m_asteroid_sprite = load_sprite( clip_rectangle );
  }

  void time_step( bear::universe::time_type time_in_seconds ) override
  {
    game_item::time_step( time_in_seconds );

    if ( ( m_scale <= 0.5 ) && ( get_age() > 2 ) )
      {
        if ( m_asteroid_sprite.get_opacity() == 0 )
          get_owner().release_item( this );
        else
          m_asteroid_sprite.set_opacity( ( 10 - get_age() ) / 8 );
      }
  }

  void collision( bear::universe::collision_info& info ) override
  {
    if ( get_age() < 1 )
      return;

    bear::universe::world& world( get_owner() );

    if ( m_scale > 0.5 )
      {
        world.register_item( create_small_asteroid() );
        world.register_item( create_small_asteroid() );
        world.release_item( this );
      }
    else
      default_collision( info );
  }

  asteroid* create_small_asteroid() const
  {
    asteroid* const result( new asteroid( 0.75 * m_scale ) );
    const bear::universe::position_type center
      ( get_left() + random_number() * get_width(),
        get_bottom() + random_number() * get_height() );
        
    result->set_center_of_mass( center ); 

    return result;
  }
};

class game:
  public bear::input::input_listener
{
private:
  typedef std::vector<game_item*> item_collection;

private:
  bool m_quit;

  const claw::math::coordinate_2d<unsigned int> m_screen_size;

  bear::visual::screen m_screen;

  const bear::universe::size_box_type m_camera_position;

  bear::input::input_status m_input;

  const bear::universe::size_box_type m_world_size;

  bear::universe::world m_world;

  ship* m_player_ship;

public:
  game()
    : m_quit( false ), m_screen_size( 1024, 575 ), m_screen( m_screen_size ),
      m_camera_position( 50, 50 ),
      m_world_size( m_screen_size + 2 * m_camera_position ),
      m_world( m_world_size ), m_player_ship( new ship )
  {
    m_world.set_gravity( bear::universe::force_type( 0, 0 ) );

    m_player_ship->set_center_of_mass( m_camera_position + m_screen_size / 2 );
    m_world.register_item( m_player_ship );

    add_asteroids();
  }

  ~game()
  {
    for ( game_item* item : get_game_items() )
      delete item;
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
  void add_asteroids()
  {
    for ( std::size_t i(0); i != 4; ++i )
      add_asteroid();
  }

  void add_asteroid()
  {
    asteroid* item( new asteroid );

    item->set_center_of_mass
      ( random_number() * m_world_size.x, random_number() * m_world_size.y );

    m_world.register_item( item );
  }

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
    m_input.scan_inputs( *m_player_ship );
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
    for ( game_item* item : get_game_items() )
      loop_entity_out_of_region( *item, region );
  }

  void loop_entity_out_of_region
  ( game_item& item, bear::universe::rectangle_type region )
  {
    const bear::universe::size_box_type offset
      ( ( m_world_size - region.size() ) / 2 );

    if ( item.get_horizontal_middle() < region.left() )
      item.set_horizontal_middle
        ( region.right() - offset.x + item.get_horizontal_middle() );
    else if ( item.get_horizontal_middle() > region.right() )
      item.set_horizontal_middle
        ( offset.x + item.get_horizontal_middle() - region.right() );

    if ( item.get_vertical_middle() < region.bottom() )
      item.set_vertical_middle
        ( region.top() - offset.y + item.get_vertical_middle() );
    else if ( item.get_vertical_middle() > region.top() )
      item.set_vertical_middle
        ( offset.y + item.get_vertical_middle() - region.top() );
  }

  void render()
  {
    m_screen.begin_render();

    for ( game_item* item : get_game_items() )
      if ( item != (game_item*)NULL )
        render_item( *item );

    m_screen.end_render();
  }

  void render_item( const game_item& item )
  {
    const bear::visual::sprite sprite( item.get_display() );
    const bear::visual::position_type position
      ( item.get_center_of_mass() - m_camera_position - sprite.get_size() / 2 );

    m_screen.render
      ( bear::visual::scene_sprite ( position.x, position.y, sprite ) );
  }

  item_collection get_game_items()
  {
    bear::universe::world::item_list items;
    const bear::universe::rectangle_type region
      ( 0, 0, m_world_size.x, m_world_size.y );

    m_world.pick_items_in_rectangle( items, region );

    item_collection result;

    for ( bear::universe::physical_item* item : items )
      {
        game_item* casted_item( dynamic_cast<game_item*>( item ) );

        if ( casted_item != NULL )
          result.push_back( casted_item );
      }

    return result;
  }

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
  std::srand( std::time( NULL ) );

  init();

  run_example();

  release();

  return 0;
}
