#include "engine/game.hpp"
#include "engine/game_description.hpp"

int main( int argc, char* argv[] )
{
  bear::engine::game_description game_description;

  game_description.set_game_name( "Sample project" );
  game_description.set_screen_width( 640 );
  game_description.set_screen_height( 480 );
  game_description.add_resources_path( "./data" );
  game_description.add_item_library( "libbear_generic_items.so" );
  game_description.set_start_level( "level/start.cl" );

  bear::engine::game game( game_description );

  game.run();

  return 0;
}
