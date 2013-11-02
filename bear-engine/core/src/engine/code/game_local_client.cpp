/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::game_local_client class.
 * \author Julien Jorge
 */
#include "engine/game_local_client.hpp"

#include "input/system.hpp"
#include "engine/game_action/game_action.hpp"
#include "engine/game_action/game_action_load_level.hpp"
#include "engine/game_action/game_action_pop_level.hpp"
#include "engine/game_action/game_action_push_level.hpp"
#include "engine/game_action/game_action_set_current_level.hpp"
#include "engine/compiled_file.hpp"
#include "engine/game_initializer.hpp"
#include "engine/level.hpp"
#include "engine/level_globals.hpp"
#include "engine/level_loader.hpp"
#include "engine/resource_pool.hpp"
#include "engine/version.hpp"

#include "engine/system/android_game_filesystem.hpp"
#include "engine/system/android_system_event_manager.hpp"
#include "engine/system/default_game_filesystem.hpp"
#include "engine/system/default_system_event_manager.hpp"
#include "engine/system/freedesktop_game_filesystem.hpp"

#include "engine/variable/base_variable.hpp"
#include "engine/variable/variable_eraser.hpp"
#include "engine/variable/variable_copy.hpp"

#include "bear_gettext.hpp"

#include "debug/timing_log.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <claw/exception.hpp>
#include <claw/logger.hpp>
#include <claw/socket_traits.hpp>
#include <claw/string_algorithm.hpp>
#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Print the options of the program.
 */
void bear::engine::game_local_client::print_help()
{
  get_arguments_table().help();
} // game_local_client::print_help()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param argc Number of program arguments.
 * \param argv Program arguments.
 */
bear::engine::game_local_client::game_local_client( int& argc, char** &argv )
  : m_status(status_init), m_screen(NULL), m_fullscreen(false),
    m_current_level(NULL), m_level_in_abeyance(NULL), m_time_step(15),
    m_time_scale(1), m_frames_per_second(60), m_synchronized_render(false),
    m_level_paused_sync(false), m_event_manager(NULL)
{
  if ( !check_arguments(argc, argv) )
    m_status = status_quit;
  else
    {
      init_environment();
      init_game_filesystem();

      try
        {
          m_screen = new visual::screen
            ( m_game_description.screen_size(),
              m_game_description.game_name(), m_fullscreen );

          init_event_manager();

          set_dumb_rendering( m_game_description.dumb_rendering() );
        }
      catch(...)
        {
          clear();
          close_environment();
          throw;
        }
    }
} // game_local_client::game_local_client()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::game_local_client::~game_local_client()
{
  delete m_event_manager;

  clear();
  close_environment();

  base_item::print_allocated();
} // game_local_client::~game_local_client()

/*----------------------------------------------------------------------------*/
/**
 * \brief Run the game.
 */
void bear::engine::game_local_client::run()
{
  if ( m_status != status_quit )
    {
      init_stats();
      init_game();

      load_level( m_game_description.start_level() );
      m_screen->unset_pause();

      run_level();

      end_game();

      clear();
      m_stats.end();

      close_environment();
    }
} // game_local_client::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells the running game to stop its activities.
 */
void bear::engine::game_local_client::sleep()
{
  if ( m_status == status_sleep )
    return;

  m_sleep_status = m_status;
  m_status = status_sleep;

  m_screen->set_pause();

  if ( m_current_level != NULL )
    m_current_level->set_pause();
} // game_local_client::sleep()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells the running game to come back to work.
 */
void bear::engine::game_local_client::wake_up()
{
  if ( m_status != status_sleep )
    return;

  m_screen->unset_pause();

  if ( m_current_level != NULL )
    m_current_level->unset_pause();

  m_status = m_sleep_status;

  set_last_progress_date();
} // game_local_client::wake_up()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the time step between to progress on the level.
 */
bear::systime::milliseconds_type
bear::engine::game_local_client::get_time_step() const
{
  return m_time_step;
} // game_local_client::get_time_step()

/*----------------------------------------------------------------------------*/
/**
 * Set the scale of the time (ie. how many ingame seconds it takes for a real
 * second.
 * \param s The scale. If s is less or equal to zero, its value is ignored and
 *        the scale is set to 1.
 */
void bear::engine::game_local_client::set_time_scale( double s )
{
  if ( s <= 0 )
    m_time_scale = 0;
  else
    m_time_scale = s;
} // game_local_client::set_time_scale()

/*----------------------------------------------------------------------------*/
/**
 * Get the scale of the time.
 */
double bear::engine::game_local_client::get_time_scale() const
{
  return m_time_scale;
} // game_local_client::get_time_scale()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the access to the network.
 */
bear::engine::game_network& bear::engine::game_local_client::get_network()
{
  return m_network;
} // game_local_client::get_network()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the translator for the plugins.
 */
bear::engine::translator
bear::engine::game_local_client::get_translator() const
{
  return m_translator;
} // game_local_client::get_translator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the translator for the plugins.
 * \param t The new translator.
 */
void bear::engine::game_local_client::set_translator
( const bear::engine::translator& t )
{
  m_translator = t;
} // game_local_client::set_translator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn on/off the dumb rendering mode.
 * \param b Tell if we must activate the dumb rendering mode.
 */
void bear::engine::game_local_client::set_dumb_rendering( bool b )
{
  if ( m_screen == NULL )
    m_game_description.set_dumb_rendering( b );
  else
    m_screen->set_dumb_rendering(b);
} // game_local_client::set_dumb_rendering()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the dumb rendering mode is active.
 */
bool bear::engine::game_local_client::get_dumb_rendering() const
{
  if ( m_screen == NULL )
    return m_game_description.dumb_rendering();
  else
    return m_screen->get_dumb_rendering();
} // game_local_client::get_dumb_rendering()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn on/off the full screen mode.
 * \param full Tell if we must activate the fullscreen mode.
 */
void bear::engine::game_local_client::set_fullscreen( bool full )
{
  if ( m_fullscreen != full )
    {
      m_fullscreen = full;
      m_screen->fullscreen(m_fullscreen);
    }
} // game_local_client::set_fullscreen()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the full screen mode.
 */
bool bear::engine::game_local_client::get_fullscreen() const
{
  return m_fullscreen;
} // game_local_client::get_fullscreen()

/*----------------------------------------------------------------------------*/
/**
 * \brief Turn on/off the overview mode.
 * \param mode Tell if we must activate the overview mode.
 */
void bear::engine::game_local_client::set_overview_mode( bool mode )
{
  m_current_level->set_overview_activated(mode);
} // game_local_client::set_overview_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the state of the overview mode.
 */
bool bear::engine::game_local_client::get_overview_mode() const
{
  return m_current_level->get_overview_activated();
} // game_local_client::get_overview_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mute/unmute the sounds.
 * \param m The mute status.
 */
void bear::engine::game_local_client::set_sound_muted( bool m )
{
  if ( m_current_level == NULL )
    level_globals::global_set_sound_muted(m);
  else
    m_current_level->get_globals().mute_sound(m);
} // game_local_client::set_sound_muted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the sound is muted.
 */
bool bear::engine::game_local_client::get_sound_muted() const
{
  return level_globals::global_get_sound_muted();
} // game_local_client::get_sound_muted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the volume of the sounds.
 * \param v The volume.
 */
void bear::engine::game_local_client::set_sound_volume( double v )
{
  if ( m_current_level == NULL )
    level_globals::global_set_sound_volume(v);
  else
    m_current_level->get_globals().set_sound_volume(v);
} // game_local_client::set_sound_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the volume of the sounds.
 */
double bear::engine::game_local_client::get_sound_volume() const
{
  return level_globals::global_get_sound_volume();
} // game_local_client::get_sound_colume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mute/unmute the music.
 * \param m The mute status.
 */
void bear::engine::game_local_client::set_music_muted( bool m )
{
  if ( m_current_level == NULL )
    level_globals::global_set_music_muted(m);
  else
    m_current_level->get_globals().mute_music(m);
} // game_local_client::set_music_muted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the music is muted.
 */
bool bear::engine::game_local_client::get_music_muted() const
{
  return level_globals::global_get_music_muted();
} // game_local_client::get_music_muted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the volume of the musics.
 * \param v The volume.
 */
void bear::engine::game_local_client::set_music_volume( double v )
{
  if ( m_current_level == NULL )
    level_globals::global_set_music_volume(v);
  else
    m_current_level->get_globals().set_music_volume(v);
} // game_local_client::set_music_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the volume of the musics.
 */
double bear::engine::game_local_client::get_music_volume() const
{
  return level_globals::global_get_music_volume();
} // game_local_client::get_music_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Take a shot of the screen.
 * \param img The image in which we save the screen.
 */
void
bear::engine::game_local_client::screenshot( claw::graphic::image& img ) const
{
  m_screen->shot( img );
} // game_local_client::screenshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Take a shot of the whole level.
 * \param img The image in which we save the level.
 */
void
bear::engine::game_local_client::levelshot( claw::graphic::image& img ) const
{
  m_current_level->shot( *m_screen, img );
} // game_local_client::levelshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief End the game.
 */
void bear::engine::game_local_client::end()
{
  m_status = status_quit;
} // game_local_client::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Give the name of a level to load as soon as possible.
 * \param path The path of the level to load.
 */
void
bear::engine::game_local_client::set_waiting_level( const std::string& path )
{
  m_post_actions.push( new game_action_load_level(path) );
} // game_local_client::set_waiting_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the level to run as soon as possible.
 * \param the_level The level to run as soon as possible.
 */
void bear::engine::game_local_client::set_waiting_level( level* the_level )
{
  CLAW_PRECOND( the_level != NULL );

  m_post_actions.push( new game_action_set_current_level(the_level) );
} // game_local_client::set_waiting_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Give the name of a level to load as soon as possible but keep the
 *        current level in memory for future restoration.
 * \param path The path of the level to load.
 */
void bear::engine::game_local_client::push_level( const std::string& path )
{
  m_post_actions.push( new game_action_push_level(path) );
} // game_local_client::push_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Restore the level at the top of the stack.
 */
void bear::engine::game_local_client::pop_level()
{
  m_post_actions.push( new game_action_pop_level );
} // game_local_client::pop_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the screen.
 */
claw::math::coordinate_2d<unsigned int>
bear::engine::game_local_client::get_screen_size() const
{
  if ( m_screen == NULL )
    return m_game_description.screen_size();
  else
    return m_screen->get_size();
} // game_local_client::get_screen_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the main window.
 */
claw::math::coordinate_2d<unsigned int>
bear::engine::game_local_client::get_window_size() const
{
  if ( m_screen == NULL )
    return get_screen_size();
  else
    return m_screen->get_container_size();
} // game_local_client::get_window_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the margin of the active area around the camera.
 */
double bear::engine::game_local_client::get_active_area_margin() const
{
  return m_game_description.active_area_margin();
} // game_local_client::get_active_area_margin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets an abstraction of the filesystem that can be used by the game.
 */
bear::engine::game_filesystem
bear::engine::game_local_client::get_game_filesystem() const
{
  return m_game_filesystem;
} // game_local_client::get_game_filesystem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the abstraction of the filesystem that can be used by the game.
 * \param f The abstract file system.
 */
void bear::engine::game_local_client::set_game_filesystem
( const game_filesystem& f )
{
  m_game_filesystem = f;
} // game_filesystem::set_game_filesystem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of a global variable.
 * \param val (in/out) The variable for which we want the value.
 */
void
bear::engine::game_local_client::get_game_variable( base_variable& val ) const
{
  val.get_value_from(m_game_variables);
} // game_local_client::get_game_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the game variables whose name match a given regular
 *        expression;
 * \param vars The list of matched game variables.
 * \param pattern The expression that has to be matched by the variable names.
 */
void bear::engine::game_local_client::get_game_variables
( var_map& vars, const std::string& pattern )
{
  m_game_variables.for_each( variable_copy(vars, boost::regex(pattern)) );
} // game_local_client::get_game_variables()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of a global variable.
 * \param val (in/out) The variable for which we set the value.
 */
void bear::engine::game_local_client::set_game_variable
( const base_variable& val )
{
  val.assign_value_to(m_game_variables);
} // game_local_client::set_game_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set several game variables at once.
 * \param vars The variables.
 */
void bear::engine::game_local_client::set_game_variables( const var_map& vars )
{
  m_game_variables.set(vars);
} // game_local_client::set_game_variables()

/*----------------------------------------------------------------------------*/
/**
 * \brief Erase the variables whose name match a given pattern.
 * \param pattern The pattern to match.
 */
void bear::engine::game_local_client::erase_game_variables
( const std::string& pattern )
{
  m_game_variables.for_each
    ( variable_eraser( m_game_variables, boost::regex(pattern) ) );
} // game_local_client::erase_game_variables()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a given variable exists in the game.
 * \param val (in/out) The variable to check.
 */
bool bear::engine::game_local_client::game_variable_exists
( const base_variable& val ) const
{
  return val.exists(m_game_variables);
} // game_local_client::game_variable_exists()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get all the game variables.
 * \param vars (in/out) All the variables.
 */
void
bear::engine::game_local_client::get_all_game_variables( var_map& vars ) const
{
  vars = m_game_variables;
} // game_local_client::get_all_game_variables()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a listener to follow the change on a game variable of type int.
 * \param name The name of the variable.
 * \param f The listener.
 */
boost::signals::connection
bear::engine::game_local_client::listen_int_variable_change
( const std::string& name, const boost::function<void (int)>& f )
{
  return listen_variable_change<int>( name, f );
} // game_local_client::listen_int_variable_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a listener to follow the change on a game variable of type
 *        unsigned int.
 * \param name The name of the variable.
 * \param f The listener.
 */
boost::signals::connection
bear::engine::game_local_client::listen_uint_variable_change
( const std::string& name, const boost::function<void (unsigned int)>& f )
{
  return listen_variable_change<unsigned int>( name, f );
} // game_local_client::listen_uint_variable_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a listener to follow the change on a game variable of type bool.
 * \param name The name of the variable.
 * \param f The listener.
 */
boost::signals::connection
bear::engine::game_local_client::listen_bool_variable_change
( const std::string& name, const boost::function<void (bool)>& f )
{
  return listen_variable_change<bool>( name, f );
} // game_local_client::listen_bool_variable_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a listener to follow the change on a game variable of type double.
 * \param name The name of the variable.
 * \param f The listener.
 */
boost::signals::connection
bear::engine::game_local_client::listen_double_variable_change
( const std::string& name, const boost::function<void (double)>& f )
{
  return listen_variable_change<double>( name, f );
} // game_local_client::listen_double_variable_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a listener to follow the change on a game variable of type string.
 * \param name The name of the variable.
 * \param f The listener.
 */
boost::signals::connection
bear::engine::game_local_client::listen_string_variable_change
( const std::string& name, const boost::function<void (std::string)>& f )
{
  return listen_variable_change<std::string>( name, f );
} // game_local_client::listen_string_variable_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the game.
 */
const std::string& bear::engine::game_local_client::get_name() const
{
  return m_game_description.game_name();
} // game_local_client::get_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send a variable to the destination page.
 * \param operation The name of the operation stored in the stats.
 * \param name The name of the variable.
 * \param value The value of the variable.
 */
void bear::engine::game_local_client::send_data
( std::string operation, std::string name, std::string value ) const
{
  m_stats.send_data( operation, name, value );
} // game_local_client::send_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send some variables to the destination.
 * \param operation The name of the operation stored in the stats.
 * \param vars The variables to send.
 */
void bear::engine::game_local_client::send_data
( std::string operation, std::list<stat_variable> vars ) const
{
  m_stats.send_data(operation, vars);
} // game_local_client::send_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the game statistics.
 */
void bear::engine::game_local_client::init_stats()
{
  const std::string file_name
    ( get_game_filesystem().get_custom_config_file_name( "engine-stats" ) );
  std::ifstream f(file_name.c_str());
  boost::uuids::uuid id;

  if ( !f )
    {
      id = boost::uuids::random_generator()();
      std::ofstream of( file_name.c_str() );
      of << id;
    }
  else
    f >> id;

  std::ostringstream oss;
  oss << id;
  m_stats.start( get_formatted_game_name(), oss.str() );
} // game_local_client::init_stats()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call the game specific function for initialisation.
 */
void bear::engine::game_local_client::init_game() const
{
  claw::logger << claw::log_verbose << "Initializing game." << std::endl;

  game_initializer::get_instance().init();
} // game_local_client::init_game()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call the game specific function for ending the game.
 */
void bear::engine::game_local_client::end_game() const
{
  claw::logger << claw::log_verbose << "Ending game." << std::endl;

  game_initializer::get_instance().end();
} // game_local_client::end_game()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the name of the game in a string usable for a file name.
 */
std::string bear::engine::game_local_client::get_formatted_game_name() const
{
  std::string result( m_game_description.game_name() );

  std::transform( result.begin(), result.end(), result.begin(), tolower );

  for (unsigned int i=0; i!=result.size(); ++i)
    if ( !( ('a' <= result[i]) && (result[i] <= 'z') )
         && !( ('0' <= result[i]) && (result[i] <= '9') ) )
      result[i] = '_';

  claw::text::squeeze( result, "_" );

  return result;
} // game_local_client::get_formatted_game_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assigns the current date to m_last_progress.
 */
void bear::engine::game_local_client::set_last_progress_date()
{
  m_last_progress = systime::get_date_ms();
} // game_local_client::set_last_progress_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Run the current_level.
 */
void bear::engine::game_local_client::run_level()
{
  m_status = status_run;

  while (m_status != status_quit)
    {
      set_last_progress_date();
      m_last_render = m_last_progress;

      do
        {
          if ( m_event_manager != NULL )
            m_event_manager->refresh();

          if ( m_status == status_sleep )
            {
              systime::sleep( 1000 );
              set_last_progress_date();
            }
          else
            one_step_beyond();
        }
      while ( !do_post_actions() && (m_status != status_quit) );
    }
} // game_local_client::run_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one progress()/render() iteration.
 */
void bear::engine::game_local_client::one_step_beyond()
{
  systime::milliseconds_type current_time( systime::get_date_ms() );

  // The value of m_time_scale may be changed by an item during the progress
  const universe::time_type time_scale( m_time_scale );
  const universe::time_type time_range( current_time - m_last_progress );
  universe::time_type dt( time_range * time_scale );

  if ( dt >= m_time_step )
    {
      progress( current_time, dt, time_range, time_scale );
      render();

      current_time = systime::get_date_ms();
    }
  
  if ( current_time < m_last_progress + m_time_step )
    systime::sleep( m_last_progress + m_time_step - current_time );
} // game_local_client::one_step_beyond()

/*----------------------------------------------------------------------------*/
/**
 * \brief Try to synchronize the network and pause the level if it is not
 *        synchronized.
 */
bool bear::engine::game_local_client::synchronize_network()
{
  bool result(false);

  if ( m_network.synchronize() )
    {
      result = true;

      if ( m_level_paused_sync )
        {
          m_current_level->unset_pause();
          m_level_paused_sync = false;
        }
    }
  else if ( !m_level_paused_sync )
    {
      m_current_level->set_pause();
      m_level_paused_sync = true;
    }

  return result;
} // game_local_client::synchronize_network()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one progress of the game.
 * \param dt The elapsed time since the last progress, according to the game's
 *        point of view.
 * \return The amount of dt that was not used for in the progress.
 */
bear::universe::time_type
bear::engine::game_local_client::synchronous_progress( universe::time_type dt )
{
  // Read the messages arriving on the network. If some clients did not
  // receive a sync message then pause the level.
  synchronize_network();
          
  progress( (universe::time_type)m_time_step / 1000 ); // seconds

  // send a synchronization message on each server
  m_network.send_synchronization();

  const universe::time_type result = dt - m_time_step;

  return result;
} // game_local_client::synchronous_progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do as many progresses of the game as possible.
 * \param t The elapsed time since the last progress, according to the game's
 *        point of view.
 * \param current_time The real date at the beginning of the progress.
 * \param time_range The elapsed time since the last progress, according to the
 *        engine's point of view.
 * \return The amount of t that was not used for in the progress.
 */
bear::universe::time_type
bear::engine::game_local_client::asynchronous_progress
( universe::time_type t, systime::milliseconds_type current_time,
  universe::time_type time_range )
{
  bool overload = false;
  universe::time_type dt(t);

  do
    {
      // Read the messages arriving on the network. If some clients did
      // not receive a sync message then pause the level.
      synchronize_network();

      progress( (universe::time_type)m_time_step / 1000 ); // seconds
      
      // send a synchronization message on each server
      m_network.send_synchronization();

      dt -= m_time_step;

      overload = (systime::get_date_ms() - current_time > time_range);
    }
  while ( (dt >= m_time_step) && (m_time_step > 0) && !overload );

  if ( overload )
    return 0;
  else
    return dt;
} // game_local_client::asynchronous_progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the game.
 * \param current_time The date at which the progress is done.
 * \param dt The elapsed time since the last progress, according to the game's
 *        point of view.
 * \param time_range The elapsed time since the last progress, according to the
 *        engine's point of view.
 * \param time_scale The scale factor applied to the engine's time to obtain the
 *        game's time.
 */
void bear::engine::game_local_client::progress
( systime::milliseconds_type current_time, universe::time_type dt,
  universe::time_type time_range, universe::time_type time_scale )
{
  set_time_scale(1);
  m_last_progress = current_time;

  if ( m_synchronized_render )
    dt = synchronous_progress(dt);
  else
    dt = asynchronous_progress(dt, current_time, time_range);

  m_last_progress -= dt / time_scale;
} // game_local_client::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the game.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::engine::game_local_client::progress
( universe::time_type elapsed_time )
{
  // effective procedure
  input::system::get_instance().refresh();

  m_current_level->progress( elapsed_time );
} // game_local_client::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the current level.
 */
void bear::engine::game_local_client::render()
{
  if ( (m_frames_per_second != 0) && !m_synchronized_render )
    {
      const systime::milliseconds_type render_date
        ( m_last_render + 1000 / m_frames_per_second );
      const systime::milliseconds_type current_date( systime::get_date_ms() );

      if ( (current_date < render_date)
           && (render_date - current_date > m_time_step) )
        return;
    }

  // effective procedure
  m_screen->begin_render();
  m_current_level->render( *m_screen );
  m_screen->end_render();

  m_last_render = systime::get_date_ms();
} // game_local_client::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the environment (screen, inputs, sounds).
 */
void bear::engine::game_local_client::init_environment() const
{
  claw::logger << claw::log_verbose << "Initializing screen environment."
               << std::endl;

  visual::screen::initialize( visual::screen::screen_gl );

  claw::logger << claw::log_verbose << "Initializing input environment."
               << std::endl;

  input::system::initialize();

  claw::logger << claw::log_verbose << input::joystick::number_of_joysticks()
               << " joysticks found." << std::endl;

  claw::logger << claw::log_verbose << "Initializing sound environment."
               << std::endl;

  audio::sound_manager::initialize();

  if ( !claw::socket_traits::init() )
    claw::logger << claw::log_error << "Failed to initialize the network."
                 << std::endl;
} // game_local_client::init_environment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Close the environment (screen, inputs, sounds).
 */
void bear::engine::game_local_client::close_environment() const
{
  claw::logger << claw::log_verbose << "Closing sound environment."
               << std::endl;

  audio::sound_manager::release();

  claw::logger << claw::log_verbose << "Closing input environment."
               << std::endl;

  input::system::release();

  claw::logger << claw::log_verbose << "Closing screen environment."
               << std::endl;

  visual::screen::release();

  claw::socket_traits::release();
} // game_local_client::close_environment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initializes the abstraction of the filesystem as seen by the game.
 */
void bear::engine::game_local_client::init_game_filesystem()
{
#if defined( __ANDROID__ )

  m_game_filesystem =
    android_game_filesystem( m_game_description.game_name() );

#elif defined( BEAR_USES_FREEDESKTOP )

  m_game_filesystem =
    freedesktop_game_filesystem( m_game_description.game_name() );

#else

  m_game_filesystem = default_game_filesystem( m_game_description.game_name() );

#endif
} // game_local_client::init_game_filesystem()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initializes the abstraction of the event handler.
 */
void bear::engine::game_local_client::init_event_manager()
{
#ifdef __ANDROID__
  m_event_manager = event_manager_ptr( new android_system_event_manager() );
#else
  m_event_manager = event_manager_ptr( new default_system_event_manager() );
#endif
} // game_local_client::init_event_manager()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the libraries containing the items.
 * \param p A list of paths to libraries.
 */
void bear::engine::game_local_client::load_libraries
( const std::list<std::string>& p )
{
  std::list<std::string>::const_iterator it;

  for ( it=p.begin(); it!=p.end(); ++it )
    {
      claw::logger << claw::log_verbose << "Add library '" << *it << "'."
                   << std::endl;
      m_symbols.add_library(*it);
    }
} // game_local_client::load_libraries()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add paths in the resource_pool.
 * \param p A list of paths.
 */
void bear::engine::game_local_client::init_resource_pool
( const std::list<std::string>& p ) const
{
  std::list<std::string>::const_iterator it;

  for ( it=p.begin(); it!=p.end(); ++it )
    {
      claw::logger << claw::log_verbose
                   << "Adding resource path '" << *it << "'."
                   << std::endl;
      resource_pool::get_instance().add_path(*it);
    }
} // game_local_client::init_resource_pool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the pending actions.
 */
bool bear::engine::game_local_client::do_post_actions()
{
  bool result = false;

  while ( !m_post_actions.empty() )
    {
      game_action* a=m_post_actions.front();
      m_post_actions.pop();

      result = a->apply(*this);

      delete a;
    }

  return result;
} // game_local_client::do_post_actions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the current level (and delete the level currently running).
 * \param the_level The new current level.
 */
void bear::engine::game_local_client::set_current_level( level* the_level )
{
  CLAW_PRECOND( the_level != NULL );

  if ( m_current_level != NULL )
    close_level();

  m_current_level = the_level;
  m_stats.start_level( m_current_level->get_filename() );

  start_current_level();
} // game_local_client::set_current_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a level.
 * \param path The path of the level to load.
 */
void bear::engine::game_local_client::load_level( const std::string& path )
{
  claw::logger << claw::log_verbose << "------------ Loading level '"
               << path << "'... ------------" << std::endl;

  std::stringstream f;
  resource_pool::get_instance().get_file( path, f );

  if ( !f )
    throw claw::exception( "Can't open level file '" + path + "'." );

  /// \todo test the file to see if it's text or binary
  compiled_file level_file( f, true );

  level_globals* shared_resources = NULL;

  if ( m_level_in_abeyance == NULL )
    shared_resources = NULL;
  else
    shared_resources = &m_level_in_abeyance->get_globals();

  const systime::milliseconds_type loading_date( systime::get_date_ms() );

  const level_globals* resources_source
    ( (m_current_level == NULL) ? NULL : &m_current_level->get_globals() );

  level_loader loader( level_file, path, shared_resources, resources_source );
  loader.complete_run();

  claw::logger << "Level loaded in "
              << ( (double)(systime::get_date_ms() - loading_date) ) / 1000
              << " s." << std::endl;

  set_current_level( loader.drop_level() );
} // game_local_client::load_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Close the current level.
 * \pre m_current_level != NULL
 * \post m_current_level == NULL
 */
void bear::engine::game_local_client::close_level()
{
  CLAW_PRECOND( m_current_level != NULL );

  delete m_current_level;
  m_current_level = NULL;

  CLAW_POSTCOND( m_current_level == NULL );
} // game_local_client::close_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a level on the stack.
 * \param path The path of the level to load.
 */
void bear::engine::game_local_client::do_push_level( const std::string& path )
{
  claw::logger << claw::log_verbose
               << "------------ Pushing '" << path << "'. ------------"
               << std::endl;

  CLAW_PRECOND( m_level_in_abeyance == NULL );

  m_level_in_abeyance = m_current_level;
  m_level_in_abeyance->set_pause();
  m_current_level = NULL;

  load_level( path );

  CLAW_POSTCOND( m_level_in_abeyance != NULL );
} // game_local_client::do_push_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Restore the level on the top of the stack.
 */
void bear::engine::game_local_client::do_pop_level()
{
  claw::logger << claw::log_verbose
               << "------------ Popping. ------------" << std::endl;
  CLAW_PRECOND( m_level_in_abeyance != NULL );
  CLAW_PRECOND( m_current_level != NULL );

  close_level();

  m_current_level = m_level_in_abeyance;
  m_level_in_abeyance = NULL;

  m_stats.pop_level( m_current_level->get_filename() );

  m_current_level->unset_pause();
  set_music_muted(get_music_muted());
  set_sound_muted(get_sound_muted());
  set_music_volume(get_music_volume());
  set_sound_volume(get_sound_volume());
} // game_local_client::do_pop_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start the current level.
 */
void bear::engine::game_local_client::start_current_level()
{
  claw::logger << claw::log_verbose
               << "------------ Starting level. ------------" << std::endl;

  CLAW_PRECOND( m_current_level != NULL );

  m_current_level->start();

  m_current_level->set_pause();
  m_level_paused_sync = true;
} // game_local_client::start_current_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all layers, controllers and the screen.
 */
void bear::engine::game_local_client::clear()
{
  if ( m_current_level != NULL )
    {
      if ( m_level_in_abeyance != NULL )
        do_pop_level();

      close_level();
    }

  if (m_screen != NULL)
    {
      delete m_screen;
      m_screen = NULL;
    }

  // deleting pending actions
  while ( !m_post_actions.empty() )
    {
      delete m_post_actions.front();
      m_post_actions.pop();
    }
} // game_local_client::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check the arguments to initialise the game.
 * \param argc Number of program arguments.
 * \param argv Program arguments.
 */
bool bear::engine::game_local_client::check_arguments( int& argc, char** &argv )
{
  bool result = false;
  std::string help;
  bool auto_exit(false);
  claw::arguments_table arg( get_arguments_table() );
  char game_var_assignment('=');

  arg.parse( argc, argv );

  if ( arg.get_bool("--version") )
    {
      std::cout << BEAR_VERSION_STRING << std::endl;
      auto_exit = true;
    }

  if ( arg.has_value("--game-name") )
    m_game_description.set_game_name( arg.get_string("--game-name") );

  if ( arg.has_value( "--stats-destination" ) )
    m_stats.set_destination( arg.get_string("--stats-destination") );

  if ( arg.has_value("--tag") )
    m_stats.set_tag( arg.get_string("--tag") );

  if ( arg.has_value("--active-area") )
    {
      if ( arg.only_integer_values("--active-area") )
        m_game_description.set_active_area_margin
          ( arg.get_integer("--active-area") );
      else
        help = "--active-area=" + arg.get_string("--active-area");
    }

  m_fullscreen = arg.get_bool("--fullscreen") && !arg.get_bool("--windowed");
  
  m_game_description.set_dumb_rendering
    ( arg.get_bool( "--dumb-rendering" )
      && !arg.get_bool( "--no-dumb-rendering" ) );

  if ( arg.has_value("--screen-height") )
    {
      if ( arg.only_integer_values("--screen-height") )
        m_game_description.set_screen_height
          ( arg.get_integer("--screen-height") );
      else
        help = "--screen-height=" + arg.get_string("--screen-height");
    }

  if ( arg.has_value("--screen-width") )
    {
      if ( arg.only_integer_values("--screen-width") )
        m_game_description.set_screen_width
          ( arg.get_integer("--screen-width") );
      else
        help = "--screen-width=" + arg.get_string("--screen-width");
    }

  if ( arg.has_value("--network-horizon") )
    {
      if ( arg.only_integer_values("--network-horizon") )
        m_network.set_min_horizon( arg.get_integer("--network-horizon") );
      else
        help = "--network-horizon=" + arg.get_string("--network-horizon");
    }

  if ( arg.has_value("--game-var-assignment") )
    {
      const std::string v( arg.get_string("--game-var-assignment") );

      if ( v.length() == 1 )
        game_var_assignment = v[0];
      else
        help = bear_gettext("--game-var-assignment: not a character.");
    }

  if ( arg.has_value("--set-game-var-int") )
    if ( !set_game_variable_from_arg<int>
         ( arg.get_all_of_string("--set-game-var-int"), game_var_assignment ) )
      help = bear_gettext("--set-game-var-int: not an integer");

  if ( arg.has_value("--set-game-var-uint") )
    if ( !set_game_variable_from_arg<unsigned int>
         ( arg.get_all_of_string("--set-game-var-uint"), game_var_assignment ) )
      help = bear_gettext("--set-game-var-uint: not an unsigned integer");

  if ( arg.has_value("--set-game-var-bool") )
    if ( !set_game_variable_from_arg<bool>
         ( arg.get_all_of_string("--set-game-var-bool"), game_var_assignment ) )
      help = bear_gettext("--set-game-var-bool: not a boolean");

  if ( arg.has_value("--set-game-var-real") )
    if ( !set_game_variable_from_arg<double>
         ( arg.get_all_of_string("--set-game-var-real"), game_var_assignment ) )
      help = bear_gettext("--set-game-var-real: not a real number");

  if ( arg.has_value("--set-game-var-string") )
    set_game_variable_from_arg<std::string>
      ( arg.get_all_of_string("--set-game-var-string"), game_var_assignment );

  if ( arg.has_value("--start-level") )
    m_game_description.set_start_level( arg.get_string("--start-level") );
  else
    help = "--start-level";

  m_synchronized_render = arg.get_bool("--sync-render");

  if ( arg.has_value("--fps") )
    {
      if ( arg.only_integer_values("--fps") )
        m_frames_per_second = arg.get_integer("--fps");
      else
        help = "--fps=" + arg.get_string("--fps");
    }

  if ( !help.empty() )
    {
      std::cout << bear_gettext("Bad argument value: '") << help
                << bear_gettext("'\n");
      arg.help();
    }
  else if ( !auto_exit )
    {
      if ( arg.get_bool( "--auto-load-symbols" ) )
        m_symbols.add_library( std::string(), true );

      load_libraries( arg.get_all_of_string("--item-library") );
      init_resource_pool( arg.get_all_of_string("--data-path") );
      result = true;
    }

  return result;
} // game_local_client::check_arguments()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a game variable from a command line argument string formated
 *        "name<sep>value".
 * \param args The values assigned to the command line arguments.
 * \param sep The separator between the name and the value. Must not be
 *        present in the variable's name.
 */
template<typename T>
bool bear::engine::game_local_client::set_game_variable_from_arg
( const std::list<std::string>& args, const char sep )
{
  bool result(true);
  std::list<std::string>::const_iterator it;

  for (it=args.begin(); it!=args.end(); ++it)
    {
      const std::size_t pos( it->find_first_of(sep) );

      if ( pos == std::string::npos )
        result = false;
      else
        {
          const std::string name( it->substr(0, pos) );
          const std::string value( it->substr(pos+1) );

          if ( !claw::text::is_of_type<T>(value) )
            result = false;
          else
            {
              std::istringstream iss(value);
              T v;
              iss >> v;

              m_game_variables.set<T>(name, v);
              result = true;
            }
        }
    }

  return result;
} // game_local_client::set_game_variable_from_arg()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a listener on the changes of a game variable.
 * \param name The name of the variable to listen.
 * \param f The function to call when the value of the variable change.
 */
template<typename T>
boost::signals::connection
bear::engine::game_local_client::listen_variable_change
( const std::string& name, boost::function<void (T)> f )
{
  return m_game_variables.variable_changed<T>(name).connect(f);
} // game_local_client::listen_variable_change()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the arguments table with the arguments of the engine.
 */
claw::arguments_table bear::engine::game_local_client::get_arguments_table()
{
  claw::arguments_table arg( bear_gettext("Engine's options:") );

  arg.add_long
    ( "--game-name", bear_gettext("The name of the game."), true,
      bear_gettext("string") );
  arg.add_long
    ( "--active-area",
      bear_gettext
      ("The margin around the camera in which we check for activity."), true,
      bear_gettext("integer") );
  arg.add_long
    ( "--screen-width", bear_gettext("The width of the screen."), true,
      bear_gettext("integer") );
  arg.add_long
    ( "--screen-height", bear_gettext("The height of the screen."), true,
      bear_gettext("integer") );
  arg.add_long
    ( "--fullscreen", bear_gettext("Runs the game in fullscreen mode."), true );
  arg.add_long( "--windowed", bear_gettext("Run the game in a window."), true );
  arg.add_long
    ( "--data-path",
      bear_gettext("Path to the directory containing the data of the game."),
      false, bear_gettext("path") );
  arg.add_long
    ( "--dumb-rendering",
      bear_gettext("Tells to use the dumbest rendering procedure."), true );
  arg.add_long
    ( "--no-dumb-rendering",
      bear_gettext("Tells not to use the dumbest rendering procedure."), true );
  arg.add_long
    ( "--item-library",
      bear_gettext("Path to a library containing items for the game."), false,
      bear_gettext("path") );
  arg.add_long
    ( "--auto-load-symbols",
      bear_gettext("Search the items in the game launcher."), true );
  arg.add_long
    ( "--start-level", bear_gettext("The path of the first level to run."),
      false, bear_gettext("string") );
  arg.add_long
    ( "--network-horizon",
      bear_gettext("The delay to apply to the network messages in term of game"
                   " iterations. Default is 1."),
      false, bear_gettext("value") );
  arg.add_long
    ( "--set-game-var-int",
      bear_gettext("Sets the value of an integer game variable."), true,
      bear_gettext("name=value") );
  arg.add_long
    ( "--set-game-var-uint",
      bear_gettext("Sets the value of a non negative integer game variable."),
      true, bear_gettext("name=value") );
  arg.add_long
    ( "--set-game-var-bool",
      bear_gettext("Sets the value of a boolean game variable."), true,
      bear_gettext("name=value") );
  arg.add_long
    ( "--set-game-var-real",
      bear_gettext("Sets the value of a real number game variable."), true,
      bear_gettext("name=value") );
  arg.add_long
    ( "--set-game-var-string",
      bear_gettext("Sets the value of a string game variable."), true,
      bear_gettext("name=value") );
  arg.add_long
    ( "--game-var-assignment",
      bear_gettext
      ("Changes the delimiter used in --set-game-var-<type> to separate the"
       " name and the value of the variable."), true,
      bear_gettext("character") );

  arg.add_long
    ( "--stats-destination",
      bear_gettext
      ("Sets the value of the destination to which game statistics are sent."),
      true );

  arg.add_long
    ( "--tag",
      bear_gettext("Associates an identifier with this game."),
      true );

  arg.add_long
    ( "--fps",
      bear_gettext("Sets the limit of the number of frames per second."),
      true );

  arg.add_long
    ( "--sync-render",
      bear_gettext
      ("Tells to do a rendering of the scene for each progress of the game."),
      true );
  arg.add
    ( "-v", "--version",
      bear_gettext("Prints the version of the engine and exit."),
      true );

  return arg;
} // game_local_client::get_arguments_table()
