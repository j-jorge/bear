/**
 * \file
 * 
 * This program creates a game using the Bear Engine. The purpose of this
 * example is to show how to configure the game (path to the resources and the
 * items) and how to start the first level.
 */
#include "engine/game.hpp"

#include "engine/game_description.hpp"
#include "engine/game_initializer.hpp"

#include "engine/level.hpp"

#include "generic_items/decorative_item.hpp"
#include "generic_items/layer/decoration_layer.hpp"

/**
 * Creates and returns a level item displaying a text.
 *
 * The item is dynamically allocated. It's the caller's responsibility to delete
 * it.
 *
 * \param level_globals The resources to use for the item. The font will be
 *        loaded from it.
 */
bear::decorative_item*
create_decorative_text( bear::engine::level_globals& level_globals )
{
  bear::decorative_item* const result( new bear::decorative_item );
  result->set_text( "It's time to code!" );
  result->set_font( level_globals.get_font( "font/FreeSansBold.ttf", 40 ) );
  result->set_bottom_left( 100, 400 );

  return result;
}

/**
 * Creates a level containing a layer with a text inside then put it in the
 * game's instance.
 */
void init_game()
{
  // A level is mostly a set of resources and a container of layer. Here we
  // create our initial level with the minimum information.
  bear::engine::level* const level
    ( new bear::engine::level
      ( // The name of the level. You can use any name you want.
        "Example level",
        // The file from which the level is loaded. It is totally optional and
        // would be set if we had used the level loader. Here we build the level
        // in the code so there is no name.
        std::string(),
        // The size of the level. If some layers do not have the same size, they
        // will be displayed using a parallax effect.
        bear::universe::size_box_type(640, 480),
        // The path of the music resource to play in the level. The music is
        // played indefinitely and paused when the level is paused.
        std::string(),
        // A pointer to a set of already loaded resources we can use. When two
        // levels are in memory simultaneously, they can share their resources
        // (musics, sounds, textures, etc.) It is necessary that these resources
        // outlive this level. This argument is typically used when loading a
        // sublevel of a level. Here we only have one level, so there is nothing
        // to share.
        NULL,
        // A pointer to a set of already loaded resources we can copy. The idea
        // is the same than for the previous argument except than there is no
        // requirement on the lifespan of these resources. This argument is
        // typically used when starting a new level reusing some resources of
        // the previous one. Here we only have one level, so there is nothing to
        // share.
        NULL ) );

  // The decoration_layer displays some decorations (sprites, texts, animations)
  // without movements. We assign the same size than the level.
  bear::engine::layer* const layer
    ( new bear::decoration_layer( bear::universe::size_box_type(640, 480) ) );

  // It is necessary that the layer is part of a level before adding items to
  // it.
  level->push_layer( layer );

  // Here we add our item. The layer takes the ownership of the items added to
  // it.
  layer->add_item( *create_decorative_text( level->get_globals() ) );

  // Finally we can tell the game to start this level as soon as possible.
  bear::engine::game::get_instance().set_waiting_level( level );
}

// The game can call a custom function before starting the main loop. We are
// going to use it to load our first level.
BEAR_ENGINE_GAME_INIT_FUNCTION( init_game )

/**
 * Creates a game instance with a simple level and runs it.
 */
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

  // Now we can create the game corresponding to this description. This instance
  // is a singleton and can be retrieved by using
  // bear::engine::game::get_instance().
  bear::engine::game game( game_description );

  // And we can start the game loop, which will call the init function above.
  // This loop ends when bear::engine::game::end() is called, for example when
  // the user clicks the close button of the window.
  game.run();

  return 0;
}
