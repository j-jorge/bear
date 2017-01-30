/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::game class.
 * \author Julien Jorge
 */
#include "engine/game.hpp"

#include "engine/game_local_client.hpp"
#include "engine/variable/variable_saver.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
bear::engine::game* bear::engine::game::s_instance(NULL);

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the instance of the game.
 * \pre s_instance != NULL
 */
bear::engine::game& bear::engine::game::get_instance()
{
  CLAW_PRECOND( s_instance != NULL );

  return *s_instance;
} // game::get_instance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Print the options of the program.
 */
void bear::engine::game::print_help()
{
  game_local_client::print_help();
} // game::print_help()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs the game from the command line arguments.
 * \param argc Number of program arguments.
 * \param argv Program arguments.
 */
bear::engine::game::game( int& argc, char** &argv )
{
  CLAW_PRECOND( s_instance == NULL );
  s_instance = this;

  m_game = new game_local_client(argc, argv);
} // game::game()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs the game from a pre built description.
 * \param descripton The description of the game to run.
 */
bear::engine::game::game( const game_description& description )
{
  CLAW_PRECOND( s_instance == NULL );
  s_instance = this;

  m_game = new game_local_client( description );
} // game::game()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::game::~game()
{
  delete m_game;
  s_instance = NULL;
} // game::~game()

/*----------------------------------------------------------------------------*/
/**
 * \brief Run the game.
 */
void bear::engine::game::run()
{
  m_game->run();
} // game::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells the running game to stop its activities.
 */
void bear::engine::game::sleep()
{
  m_game->sleep();
} // game::sleep()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells the running game to come back to work.
 */
void bear::engine::game::wake_up()
{
  m_game->wake_up();
} // game::wake_up()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the time step between to progress on the level.
 */
bear::systime::milliseconds_type bear::engine::game::get_time_step() const
{
  return m_game->get_time_step();
} // game::get_time_step()

/*----------------------------------------------------------------------------*/
/**
 * Set the scale of the time (ie. how many ingame seconds it takes for a real
 * second.
 * \param s The scale. If s is less or equal to zero, its value is ignored and
 *        the scale is set to 1.
 */
void bear::engine::game::set_time_scale( double s )
{
  m_game->set_time_scale(s);
} // game::set_time_scale()

/*----------------------------------------------------------------------------*/
/**
 * Get the scale of the time.
 */
double bear::engine::game::get_time_scale() const
{
  return m_game->get_time_scale();
} // game::get_time_scale()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the access to the network.
 */
bear::engine::game_network& bear::engine::game::get_network()
{
  return m_game->get_network();
} // game::get_network()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the translator for the plugins.
 */
bear::engine::translator bear::engine::game::get_translator() const
{
  return m_game->get_translator();
} // game::get_translator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the translator for the plugins.
 * \param t The new translator.
 */
void bear::engine::game::set_translator( const bear::engine::translator& t )
{
  return m_game->set_translator( t );
} // game::set_translator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn on/off the dumb rendering mode.
 * \param b Tell if we must activate the dumb rendering mode.
 */
void bear::engine::game::set_dumb_rendering( bool b )
{
  m_game->set_dumb_rendering(b);
} // game::set_dumb_rendering()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the dumb rendering mode is active.
 */
bool bear::engine::game::get_dumb_rendering() const
{
  return m_game->get_dumb_rendering();
} // game::get_dumb_rendering()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn on/off the full screen mode.
 * \param full Tell if we must activate the fullscreen mode.
 */
void bear::engine::game::set_fullscreen( bool full )
{
  m_game->set_fullscreen(full);
} // game::set_fullscreen()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the full screen mode.
 */
bool bear::engine::game::get_fullscreen() const
{
  return m_game->get_fullscreen();
} // game::get_fullscreen()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the status of the fullscreen mode.
 */
void bear::engine::game::toggle_fullscreen()
{
  set_fullscreen( !get_fullscreen() );
} // game::toggle_fullscreen()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn on/off the overview mode.
 * \param mode Tell if we must activate the overview mode.
 */
void bear::engine::game::set_overview_mode( bool mode )
{
  m_game->set_overview_mode(mode);
} // game::set_overview_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the state of the overview mode.
 */
bool bear::engine::game::get_overview_mode() const
{
  return m_game->get_overview_mode();
} // game::get_overview_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Toggle the status of the overview mode.
 */
void bear::engine::game::toggle_overview_mode()
{
  set_overview_mode( !get_overview_mode() );
} // game::toggle_overview_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mute/unmute the sounds.
 * \param m The mute status.
 */
void bear::engine::game::set_sound_muted( bool m )
{
  m_game->set_sound_muted(m);
} // game::set_sound_muted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the sound is muted.
 */
bool bear::engine::game::get_sound_muted() const
{
  return m_game->get_sound_muted();
} // game::get_sound_muted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Change the "mute" status of the sounds.
 */
void bear::engine::game::toggle_sound_muted()
{
  set_sound_muted( !get_sound_muted() );
} // game::toggle_sound_muted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the volume of the sounds.
 * \param v The volume.
 */
void bear::engine::game::set_sound_volume( double v )
{
  m_game->set_sound_volume(v);
} // game::set_sound_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the volume of the sounds.
 */
double bear::engine::game::get_sound_volume() const
{
  return m_game->get_sound_volume();
} // game::get_sound_colume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mute/unmute the music.
 * \param m The mute status.
 */
void bear::engine::game::set_music_muted( bool m )
{
  m_game->set_music_muted(m);
} // game::set_music_muted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the music is muted.
 */
bool bear::engine::game::get_music_muted() const
{
  return m_game->get_music_muted();
} // game::get_music_muted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Change the "mute" status of the music.
 */
void bear::engine::game::toggle_music_muted()
{
  set_music_muted( !get_music_muted() );
} // game::toggle_music_muted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the volume of the musics.
 * \param v The volume.
 */
void bear::engine::game::set_music_volume( double v )
{
  m_game->set_music_volume(v);
} // game::set_music_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the volume of the musics.
 */
double bear::engine::game::get_music_volume() const
{
  return m_game->get_music_volume();
} // game::get_music_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Take a shot of the screen.
 * \param img The image in which we save the screen.
 */
void bear::engine::game::screenshot( claw::graphic::image& img ) const
{
  m_game->screenshot( img );
} // game::screenshot()

boost::signals2::connection bear::engine::game::schedule_screenshot
( const boost::function< void( const claw::graphic::image& ) >& f )
{
  return m_game->schedule_screenshot(f);
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Take a shot of the whole level.
 * \param img The image in which we save the level.
 */
void bear::engine::game::levelshot( claw::graphic::image& img ) const
{
  m_game->levelshot( img );
} // game::levelshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief End the game.
 */
void bear::engine::game::end()
{
  m_game->end();
} // game::end()

#include "engine/variable/variable_list_reader.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Give the name of a level to load as soon as possible.
 * \param path The path of the level to load.
 */
void bear::engine::game::set_waiting_level( const std::string& path )
{
  m_game->set_waiting_level(path);
} // game::set_waiting_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the level to run as soon as possible.
 * \param the_level The level to run as soon as possible.
 */
void bear::engine::game::set_waiting_level( level* the_level )
{
  m_game->set_waiting_level(the_level);
} // game::set_waiting_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Give the name of a level to load as soon as possible but keep the
 *        current level in memory for future restoration.
 * \param path The path of the level to load.
 */
void bear::engine::game::push_level( const std::string& path )
{
  m_game->push_level(path);
} // game::push_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Restore the level at the top of the stack.
 */
void bear::engine::game::pop_level()
{
  m_game->pop_level();
} // game::pop_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the screen.
 */
claw::math::coordinate_2d<unsigned int>
 bear::engine::game::get_screen_size() const
{
  return m_game->get_screen_size();
} // game::get_screen_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the main window.
 */
claw::math::coordinate_2d<unsigned int>
bear::engine::game::get_window_size() const
{
  return m_game->get_window_size();
} // game::get_window_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the margin of the active area around the camera.
 */
double bear::engine::game::get_active_area_margin() const
{
  return m_game->get_active_area_margin();
} // game::get_active_area_margin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets an abstraction of the filesystem that can be used by the game.
 */
bear::engine::game_filesystem bear::engine::game::get_game_filesystem() const
{
  return m_game->get_game_filesystem();
} // game::get_game_filesystem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the abstraction of the filesystem that can be used by the game.
 * \param f The abstract file system.
 */
void bear::engine::game::set_game_filesystem( const game_filesystem& f )
{
  m_game->set_game_filesystem(f);
} // game::set_game_filesystem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of a global variable.
 * \param val (in/out) The variable for which we want the value.
 */
void bear::engine::game::get_game_variable( base_variable& val ) const
{
  m_game->get_game_variable(val);
} // game::get_game_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the game variables whose name match a given regular
 *        expression;
 * \param vars The list of matched game variables.
 * \param pattern The expression that has to be matched by the variable names.
 */
void bear::engine::game::get_game_variables
( var_map& vars, const std::string& pattern )
{
  m_game->get_game_variables(vars, pattern);
} // game::get_game_variables()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of a global variable.
 * \param val (in/out) The variable for which we set the value.
 */
void bear::engine::game::set_game_variable( const base_variable& val )
{
  m_game->set_game_variable(val);
} // game::set_game_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set several game variables at once.
 * \param vars The variables.
 */
void bear::engine::game::set_game_variables( const var_map& vars )
{
  m_game->set_game_variables(vars);
} // game::set_game_variables()

/*----------------------------------------------------------------------------*/
/**
 * \brief Erase the variables whose name match a given pattern.
 * \param pattern The pattern to match.
 */
void bear::engine::game::erase_game_variables( const std::string& pattern )
{
  m_game->erase_game_variables(pattern);
} // game::erase_game_variables()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a given variable exists in the game.
 * \param val (in/out) The variable to check.
 */
bool bear::engine::game::game_variable_exists( const base_variable& val ) const
{
  return m_game->game_variable_exists(val);
} // game::game_variable_exists()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save all the game variables whose name match a given regular
 *        expression;
 * \param os The stream in which the variables are saved.
 * \param pattern The expression that has to be matched by the variable names.
 */
void bear::engine::game::save_game_variables
( std::ostream& os, const std::string& pattern )
{
  var_map vars;
  m_game->get_all_game_variables(vars);

  vars.for_each( variable_saver(os, boost::regex(pattern)) );
} // game::save_game_variables()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a listener to follow the change on a game variable of type int.
 * \param name The name of the variable.
 * \param f The listener.
 */
boost::signals2::connection
bear::engine::game::listen_int_variable_change
( const std::string& name, const boost::function<void (int)>& f )
{
  return m_game->listen_int_variable_change( name, f );
} // game::listen_int_variable_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a listener to follow the change on a game variable of type
 *        unsigned int.
 * \param name The name of the variable.
 * \param f The listener.
 */
boost::signals2::connection
bear::engine::game::listen_uint_variable_change
( const std::string& name, const boost::function<void (unsigned int)>& f )
{
  return m_game->listen_uint_variable_change( name, f );
} // game::listen_uint_variable_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a listener to follow the change on a game variable of type bool.
 * \param name The name of the variable.
 * \param f The listener.
 */
boost::signals2::connection
bear::engine::game::listen_bool_variable_change
( const std::string& name, const boost::function<void (bool)>& f )
{
  return m_game->listen_bool_variable_change( name, f );
} // game::listen_bool_variable_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a listener to follow the change on a game variable of type double.
 * \param name The name of the variable.
 * \param f The listener.
 */
boost::signals2::connection
bear::engine::game::listen_double_variable_change
( const std::string& name, const boost::function<void (double)>& f )
{
  return m_game->listen_double_variable_change( name, f );
} // game::listen_double_variable_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a listener to follow the change on a game variable of type string.
 * \param name The name of the variable.
 * \param f The listener.
 */
boost::signals2::connection
bear::engine::game::listen_string_variable_change
( const std::string& name, const boost::function<void (std::string)>& f )
{
  return m_game->listen_string_variable_change( name, f );
} // game::listen_string_variable_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the game.
 */
const std::string& bear::engine::game::get_name() const
{
  return m_game->get_name();
} // game::get_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send a variable to the destination page.
 * \param operation The name of the operation stored in the stats.
 * \param name The name of the variable.
 * \param value The value of the variable.
 */
void bear::engine::game::send_data
( std::string operation, std::string name, std::string value ) const
{
  m_game->send_data( operation, name, value );
} // game::send_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send some variables to the destination.
 * \param operation The name of the operation stored in the stats.
 * \param vars The variables to send.
 */
void bear::engine::game::send_data
( std::string operation, std::list<stat_variable> vars ) const
{
  m_game->send_data(operation, vars);
} // game::send_data()
