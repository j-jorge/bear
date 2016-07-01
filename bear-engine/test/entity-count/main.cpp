/**
 * \file
 *
 * Performance test.
 */

#include "input/input_listener.hpp"
#include "input/input_status.hpp"
#include "input/system.hpp"
#include "time/time.hpp"
#include "universe/collision_info.hpp"
#include "universe/physical_item.hpp"
#include "universe/world.hpp"
#include "visual/scene_sprite.hpp"
#include "visual/screen.hpp"

#include <fstream>

double random_number()
{
  return (double)std::rand() / RAND_MAX;
}

bear::visual::sprite create_box_sprite()
{
  std::ifstream f( "box.png" );
  const claw::graphic::image image( f );
  const bear::visual::image texture( image );

  return bear::visual::sprite
    ( texture, bear::visual::sprite::clip_rectangle_type( 0, 0, 20, 20 ) );
}

class game_item:
  public bear::universe::physical_item
{
public:
  bear::visual::sprite m_display;
  
public:
  game_item( const bear::visual::sprite& visual )
    : m_display( visual )
  {
    set_size( 20, 20 );
    set_mass( 500 + 1000 * random_number() );
    set_friction( 1 );
    set_system_angle( 2 * 3.14159 * random_number() );
    set_speed( get_x_axis() * ( 200 + 200 * random_number() ) );
    set_density( random_number() * 10 );
    set_angular_speed( -0.05 + random_number() * 0.1 );
    set_elasticity( 0.5 + random_number() / 2 );
    
    m_display.set_intensity
      ( 0.5 + random_number() / 2, 0.5 + random_number() / 2,
        0.5 + random_number() / 2 );
  }

  const bear::visual::sprite& get_display() const
  {
    return m_display;
  }

private:
  void time_step( double elapsed_time ) override
  {
    bear::universe::physical_item::time_step( elapsed_time );
    m_display.set_opacity( 0.1 );
  }
  
  void collision( bear::universe::collision_info& info ) override
  {
    m_display.set_opacity( 1 );
  }
};

class arena_bound:
  public bear::universe::physical_item
{
private:
  void collision( bear::universe::collision_info& info ) override
  {
    switch( info.get_collision_side() )
      {
      case bear::universe::zone::bottom_zone:
        collision_align_bottom(info);
        break;
      case bear::universe::zone::top_zone:
        collision_align_top(info);
        break;
      case bear::universe::zone::middle_left_zone:
        collision_align_left(info);
        break;
      case bear::universe::zone::middle_right_zone:
        collision_align_right(info);
        break;
      default: { assert( false ); }
    }
  }
};

class game:
  public bear::input::input_listener
{
private:
  typedef std::vector< game_item* > item_collection;

private:
  bool m_quit;
  const bool m_unlimited;
  const claw::math::coordinate_2d<unsigned int> m_screen_size;
  bear::visual::screen m_screen;
  const bear::universe::size_box_type m_camera_position;
  bear::input::input_status m_input;
  const bear::universe::size_box_type m_world_size;
  bear::universe::world m_world;
  item_collection m_items;
  std::vector< arena_bound > m_bounds;
  bear::systime::milliseconds_type m_last_spawn;
  bear::visual::sprite m_item_visual;
  
public:
  explicit game( bool unlimited )
    : m_quit( false ),
      m_unlimited( unlimited ),
      m_screen_size( 1024, 575 ),
      m_screen( m_screen_size ),
      m_camera_position( 50, 50 ),
      m_world_size( m_screen_size + 2 * m_camera_position ),
      m_world( m_world_size ),
      m_bounds( 4 ),
      m_last_spawn( 0 ),
      m_item_visual( create_box_sprite() )
  {
    m_world.set_gravity( bear::universe::force_type( 0, 0 ) );

    add_bounds();
  }

  ~game()
  {
    for ( item_collection::value_type& item : m_items )
      delete item;
  }

  void run()
  {
    bear::systime::milliseconds_type last_update
      ( bear::systime::get_date_ms() );
    const bear::systime::milliseconds_type time_step( 1000.0 / 30 );

    std::size_t next_log( 0 );
    
    while ( !m_quit )
      {
        const bear::systime::milliseconds_type now
          ( bear::systime::get_date_ms() );
      
        if ( now - last_update < time_step )
          bear::systime::sleep( time_step - ( now - last_update ) );

        update_inputs();
        
        const bear::systime::milliseconds_type start
          ( bear::systime::get_date_ms() );

        update_world( double( now - last_update ) / 1000);
        
        const bear::systime::milliseconds_type end
          ( bear::systime::get_date_ms() );

        last_update = now;

        if ( m_items.size() == next_log )
          {
            std::cout << m_items.size() << '\t' << end - start << '\n';
            next_log += 10;
          }
        
        if ( end - start > time_step )
          {
            std::cout << "physics timeout: " << m_items.size() << " items, "
                      << end - start << " > " << time_step << " ms.\n";
            m_quit = !m_unlimited;
          }
        else if ( end - start > time_step / 2 )
          std::cout << "physics half timeout: " << m_items.size() << " items, "
                    << end - start << " > " << time_step << " / 2 ms.\n";
        
        render();
        
        spawn_item();
    }
  }

private:
  void add_bounds()
  {
    const double margin( m_camera_position.x );

    m_bounds[ 0 ].set_bottom_left( 0, 0 );
    m_bounds[ 0 ].set_size( m_world_size.x, margin );
    m_world.add_static( &m_bounds[ 0 ] );
    
    m_bounds[ 1 ].set_bottom_left( 0, m_world_size.y - margin );
    m_bounds[ 1 ].set_size( m_world_size.x, margin );
    m_world.add_static( &m_bounds[ 1 ] );

    m_bounds[ 2 ].set_bottom_left( 0, margin );
    m_bounds[ 2 ].set_size( margin, m_world_size.y - 2 * margin );
    m_world.add_static( &m_bounds[ 2 ] );

    m_bounds[ 3 ].set_bottom_left( m_world_size.x - margin, margin );
    m_bounds[ 3 ].set_size( margin, m_world_size.y - 2 * margin );
    m_world.add_static( &m_bounds[ 3 ] );
  }

  void spawn_item()
  {
    const bear::systime::milliseconds_type now( bear::systime::get_date_ms() );

    if ( now - m_last_spawn < 200 )
      return;

    m_last_spawn = now;
    const double margin( m_camera_position.x * 2 );
    game_item* item( new game_item( m_item_visual ) );

    item->set_center_of_mass
      ( margin + random_number() * ( m_world_size.x - 2 * margin ),
        margin + random_number() * ( m_world_size.y - 2 * margin ) );

    m_items.push_back( item );
    m_world.register_item( item );
  }

  bool key_pressed( const bear::input::key_info& key ) override
  {
    if ( key.is_escape() )
      {
        m_quit = true;
        return true;
      }

    return false;
  }

  void update_inputs()
  {
    bear::input::system::get_instance().refresh();
    m_input.read();
    m_input.scan_inputs( *this );
  }

  void update_world( double time_step )
  {
    bear::universe::world::region_type region;
    region.push_back
      ( bear::universe::rectangle_type
        ( 0, 0, m_world.get_size().x, m_world.get_size().y ) );
    m_world.progress_entities( region, time_step );
  }

  void render()
  {
    m_screen.begin_render();

    for ( game_item* item : m_items )
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

};

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
void run_example( bool unlimited )
{
  game game_instance( unlimited );
  game_instance.run();
}

/**
 * Initializes the engine then runs the example. The engine's modules will be
 * released before leaving.
 */
int main( int argc, char* argv[] )
{
  init();

  const bool unlimited
    ( ( argc == 2 ) && std::string( argv[ 1 ] ) == "--unlimited" );
  run_example( unlimited );

  release();

  return 0;
}
