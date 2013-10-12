/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The class managing the levels and the development of the game.
 * \author Julien Jorge
 */
#ifndef __ENGINE_GAME_LOCAL_CLIENT_HPP__
#define __ENGINE_GAME_LOCAL_CLIENT_HPP__

#include <fstream>
#include <queue>

#include "engine/class_export.hpp"
#include "engine/game_description.hpp"
#include "engine/game_network.hpp"
#include "engine/game_stats.hpp"
#include "engine/i18n/translator.hpp"
#include "engine/libraries_pool.hpp"
#include "engine/stat_variable.hpp"
#include "engine/system/base_system_event_manager.hpp"
#include "engine/system/game_filesystem.hpp"
#include "engine/variable/var_map.hpp"
#include "time/time.hpp"
#include "visual/screen.hpp"
#include "universe/types.hpp"

#include <claw/arguments_table.hpp>

#include <boost/function.hpp>

namespace bear
{
  namespace engine
  {
    class base_variable;
    class game_action;
    class game_action_load_level;
    class game_action_pop_level;
    class game_action_push_level;
    class game_action_set_current_level;
    class game_action_set_current_level;
    class level;

    /**
     * \brief The class managing the levels and the development of the game.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT game_local_client
    {
      friend class game_action;
      friend class game_action_load_level;
      friend class game_action_pop_level;
      friend class game_action_push_level;
      friend class game_action_set_current_level;

    private:
      /**
       * \brief Game status.
       */
      enum status
        {
          /** \brief The game is under initialization. */
          status_init,

          /** \brief The game is running. */
          status_run,

          /** \brief The game is sleeping. */
          status_sleep,

          /** \brief We're quiting. */
          status_quit

        }; // enum status

      /** \brief The type of the pointer on the instance of the event
          manager. */
      typedef base_system_event_manager* event_manager_ptr;

    public:
      static void print_help();

      game_local_client( int& argc, char** &argv );
      ~game_local_client();

      void run();
      void sleep();
      void wake_up();

      systime::milliseconds_type get_time_step() const;
      void set_time_scale( double s );
      double get_time_scale() const;

      game_network& get_network();

      translator get_translator() const;
      void set_translator( const translator& t );

      void set_dumb_rendering( bool b );
      bool get_dumb_rendering() const;

      void set_fullscreen( bool full );
      bool get_fullscreen() const;

      void set_overview_mode( bool m );
      bool get_overview_mode() const;

      void set_sound_muted( bool m );
      bool get_sound_muted() const;
      void set_sound_volume( double v );
      double get_sound_volume() const;

      void set_music_muted( bool m );
      bool get_music_muted() const;
      void set_music_volume( double v );
      double get_music_volume() const;

      void screenshot( claw::graphic::image& img ) const;
      void levelshot( claw::graphic::image& img ) const;

      void end();
      void set_waiting_level( const std::string& path );
      void set_waiting_level( level* the_level );
      void push_level( const std::string& path );
      void pop_level();

      claw::math::coordinate_2d<unsigned int> get_screen_size() const;
      claw::math::coordinate_2d<unsigned int> get_window_size() const;
      double get_active_area_margin() const;

      game_filesystem get_game_filesystem() const;
      void set_game_filesystem( const game_filesystem& f );

      void get_game_variable( base_variable& val ) const;
      void get_game_variables
      ( var_map& vars, const std::string& pattern = ".*");
      void set_game_variable( const base_variable& val );
      void set_game_variables( const var_map& vars );
      void erase_game_variables( const std::string& pattern );
      bool game_variable_exists( const base_variable& val ) const;
      void get_all_game_variables( var_map& vars ) const;

      boost::signals::connection
        listen_int_variable_change
        ( const std::string& name, const boost::function<void (int)>& f );
      boost::signals::connection
        listen_uint_variable_change
        ( const std::string& name,
          const boost::function<void (unsigned int)>& f );
      boost::signals::connection
        listen_bool_variable_change
        ( const std::string& name, const boost::function<void (bool)>& f );
      boost::signals::connection
        listen_double_variable_change
        ( const std::string& name, const boost::function<void (double)>& f );
      boost::signals::connection
        listen_string_variable_change
        ( const std::string& name,
          const boost::function<void (std::string)>& f );

      const std::string& get_name() const;

      void send_data
        ( std::string operation, std::string name, std::string value ) const;
      void send_data
        ( std::string operation, std::list<stat_variable> vars ) const;

    private:
      void init_stats();

      void init_game() const;
      void end_game() const;

      std::string get_formatted_game_name() const;

      void set_last_progress_date();

      void run_level();
      void one_step_beyond();

      bool synchronize_network();

      bear::universe::time_type synchronous_progress( universe::time_type dt );
      bear::universe::time_type asynchronous_progress
        ( universe::time_type dt, systime::milliseconds_type current_time,
          universe::time_type time_range );

      void progress
        ( systime::milliseconds_type current_time, universe::time_type dt,
          universe::time_type time_range, universe::time_type time_scale );
      void progress( universe::time_type elapsed_time );
      void render();

      void update_inputs();

      void init_environment() const;
      void close_environment() const;

      void init_game_filesystem();
      void init_event_manager();

      void load_libraries( const std::list<std::string>& p );
      void init_resource_pool( const std::list<std::string>& p ) const;

      bool do_post_actions();

      void set_current_level( level* the_level );
      void load_level( const std::string& path );
      void close_level();
      void do_push_level( const std::string& path );
      void do_pop_level();

      void start_current_level();

      void clear();

      bool check_arguments( int& argc, char** &argv );

      template<typename T>
      bool set_game_variable_from_arg
        ( const std::list<std::string>& args, const char sep );

      template<typename T>
        boost::signals::connection
        listen_variable_change
        ( const std::string& name, boost::function<void (T)> f );

      static claw::arguments_table get_arguments_table();

    private:
      // must be declared before m_game_description
      /** \brief The libraries in which we take custom functions. */
      libraries_pool m_symbols;

      /** \brief The current status of the game. */
      status m_status;

      /** \brief The status of the game before it was sent to sleep. */
      status m_sleep_status;

      /** \brief Description of the game. */
      game_description m_game_description;

      /** \brief Global variables of the game. */
      var_map m_game_variables;

      /** \brief The screen. */
      visual::screen* m_screen;

      /** \brief Tell if we are fullscreen or not. */
      bool m_fullscreen;

      /** \brief The current level. */
      level* m_current_level;

      /** \brief A level in abeyance. */
      level* m_level_in_abeyance;

      /** \brief The name of the next level to load, if any. */
      std::string m_waiting_level;

      /** \brief Actions to do once an iteration is done. */
      std::queue<game_action*> m_post_actions;

      /** \brief Number of milliseconds between two iterations. */
      systime::milliseconds_type m_time_step;

      /** \brief The scale of the time in the game. */
      universe::time_type m_time_scale;

      /** \brief The expected number of frames per seconds. */
      std::size_t m_frames_per_second;

      /** \brief The date of the last render. */
      systime::milliseconds_type m_last_render;

      /** \brief The date of the last call to progress. */
      systime::milliseconds_type m_last_progress;

      /** \brief Tell to do one render for each progress. */
      bool m_synchronized_render;

      /** \brief The statistics sent at the end of the game. */
      game_stats m_stats;

      /** \brief The instance that manages the connections to the network. */
      game_network m_network;

      /** \brief Tell if the current level is paused due to a desynchronization
          of the network. */
      bool m_level_paused_sync;

      /** \brief The translator for the plugins. */
      translator m_translator;

      /** \brief The abstraction of the filesystem, as seen by the game. */
      game_filesystem m_game_filesystem;

      /** \brief The abstraction of the system event handler. */
      event_manager_ptr m_event_manager;

    }; // class game_local_client
  } // namespace engine
} // namespace bear

#endif // __ENGINE_GAME_LOCAL_CLIENT_HPP__
