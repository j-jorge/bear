/**
 * \file
 * 
 * This program creates a game using the Bear Engine. The purpose of this
 * example is to show how to configure the game (path to the resources and the
 * items) and how to start the first level.
 */
#include "engine/game.hpp"
#include "engine/game_description.hpp"

int main( int argc, char* argv[] )
{
  bear::engine::game_description game_description;

  // This is the name of the game. It will be displayed in the title bar of the
  // window. Also, if the game needs to store user-specific files, this name
  // will be used to build the name of the directory to receive these files.
  game_description.set_game_name( "Sample project" );

  // Here we set up the size of the game's window.
  game_description.set_screen_width( 640 );
  game_description.set_screen_height( 480 );

  // The game will search the resources in the paths passed to
  // add_resources_path. The resources are searched by starting from the latest
  // added path, up to the first path.
  game_description.add_resources_path( "./resources" );

  // Custom item classes can be added to the game using modules. When loading a
  // level, the items used in it are searched in these libraries.
  game_description.add_item_library( "libbear_generic_items.so" );

  // Finally, the game needs to now which level must be started first.
  game_description.set_start_level( "level/start.cl" );

  // Now we can create the game corresponding to this description. This instance
  // is a singleton and can be retrieved by using
  // bear::engine::game::get_instance().
  bear::engine::game game( game_description );

  // And we can start the game loop. This function exits when
  // bear::engine::game::end() is called, for example when the user clicks the
  // close button of the window.
  game.run();

  return 0;
}
