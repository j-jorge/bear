/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class loads and store informations from a game description file.
 * \author Julien Jorge
 */
#ifndef __ENGINE_GAME_DESCRIPTION_HPP__
#define __ENGINE_GAME_DESCRIPTION_HPP__

#include <string>
#include <map>
#include <list>

#include <claw/math.hpp>

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief This class stores some information about the initialization of the
     *        game.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT game_description
    {
    public:
      typedef std::list<std::string> string_list;

    public:
      game_description();

      const std::string& start_level() const;
      const std::string& game_name() const;
      const claw::math::coordinate_2d<unsigned int>& screen_size() const;
      bool dumb_rendering() const;
      double active_area_margin() const;
      const string_list& resources_path() const;
      const string_list& libraries() const;

      void set_game_name( const std::string& value );
      void set_screen_width( unsigned int value );
      void set_screen_height( unsigned int value );
      void set_dumb_rendering( bool v );
      void set_active_area_margin( unsigned int value );
      void add_resources_path( const string_list& value );
      void add_item_library( const string_list& value );
      void set_start_level( const std::string& value );

    private:
      /** \brief The name of the first level to load. */
      std::string m_start_level;

      /** \brief The name of the game. */
      std::string m_game_name;

      /** \brief The size of the screen. */
      claw::math::coordinate_2d<unsigned int> m_screen_size;

      /** \brief The margin of the active area around the screen. */
      double m_active_area_margin;

      /** \brief The paths to the forder containing the resources. */
      string_list m_resources_path;

      /** \brief The game libraries to link to. */
      string_list m_libraries;

      /** \brief Tells if we use dumb rendering by default. */
      bool m_use_dumb_rendering;

    }; // class game_description
  } // namespace engine
} // namespace bear

#endif // __ENGINE_GAME_DESCRIPTION_HPP__
