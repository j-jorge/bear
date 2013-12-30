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
#ifndef __ENGINE_GAME_HPP__
#define __ENGINE_GAME_HPP__

#include "engine/class_export.hpp"
#include "engine/stat_variable.hpp"
#include "engine/variable/var_map.hpp"

#include "time/time.hpp"

#include <claw/image.hpp>
#include <claw/coordinate_2d.hpp>

#include <boost/function.hpp>

namespace bear
{
  namespace engine
  {
    class base_variable;
    class game_description;
    class game_filesystem;
    class game_local_client;
    class game_network;
    class level;
    class libraries_pool;
    class translator;

    /**
     * \brief The class managing the levels and the evolution of the game.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT game
    {
    public:
      static game& get_instance();
      static void print_help();

      game( int& argc, char** &argv );
      explicit game( const game_description& description );

      ~game();

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
      void toggle_fullscreen();

      void set_overview_mode( bool m );
      bool get_overview_mode() const;
      void toggle_overview_mode();

      void set_sound_muted( bool m );
      bool get_sound_muted() const;
      void toggle_sound_muted();
      void set_sound_volume( double v );
      double get_sound_volume() const;

      void set_music_muted( bool m );
      bool get_music_muted() const;
      void toggle_music_muted();
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
      (var_map& vars, const std::string& pattern = ".*" );
      void set_game_variable( const base_variable& val );
      void set_game_variables( const var_map& vars );
      void erase_game_variables( const std::string& pattern );
      bool game_variable_exists( const base_variable& val ) const;

      void save_game_variables
        ( std::ostream& os, const std::string& pattern = ".*" );

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
          const boost::function<void (const std::string)>& f );

      const std::string& get_name() const;
      void send_data
        ( std::string operation, std::string name, std::string value ) const;
      void send_data
        ( std::string operation, std::list<stat_variable> vars ) const;

    private:
      /** \brief The instance of the game on this computer. */
      static game* s_instance;

      /** \brief The effective implementation of the game. */
      game_local_client* m_game;

    }; // class game
  } // namespace engine
} // namespace bear

#endif // __ENGINE_GAME_HPP__
