/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The game_initializer class manages the functions used to initialize
 *        and to end the game.
 * \author Julien Jorge
 */
#ifndef __ENGINE_GAME_INITIALIZER_HPP__
#define __ENGINE_GAME_INITIALIZER_HPP__

#include <claw/basic_singleton.hpp>

#include "engine/class_export.hpp"

/**
 * \brief Sets the function to call when initializing the game.
 * \param f The function. Its signature must be void ().
 */
#define BEAR_ENGINE_GAME_INIT_FUNCTION( f )                             \
  static bool f ## _init_registered =                                   \
    bear::engine::game_initializer::get_instance().set_init( f );

/**
 * \brief Sets the function to call when ending the game.
 * \param f The function. Its signature must be void ().
 */
#define BEAR_ENGINE_GAME_END_FUNCTION( f )                             \
  static bool f ## _end_registered =                                   \
    bear::engine::game_initializer::get_instance().set_end( f );

namespace bear
{
  namespace engine
  {
    /**
     * \brief The game_initializer class manages the functions used to
     *        initialize and to end the game.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT game_initializer:
      public claw::pattern::basic_singleton<game_initializer>
    {
    private:
      /** \brief The type of the parent class. */
      typedef claw::pattern::basic_singleton<game_initializer> super;

      friend class claw::pattern::basic_singleton<game_initializer>;

    public:
      /** \brief Type of the game specific initialisation procedure. */
      typedef void (*init_game_function_type)();

      /** \brief Type of the game specific ending procedure. */
      typedef void (*end_game_function_type)();

    public:
      // Must be redefined to work correctly with dynamic libraries.
      // At least under Windows with MinGW.
      static game_initializer& get_instance();

      bool set_init( init_game_function_type f );
      bool set_end( end_game_function_type f );

      void init();
      void end();

    private:
      game_initializer();

    private:
      /** \brief The function to call when initializing the game. */
      init_game_function_type m_init;

      /** \brief The function to call when ending the game. */
      end_game_function_type m_end;

    }; // class game_initializer
  } // namespace engine
} // namespace bear

#endif // __ENGINE_GAME_INITIALIZER_HPP__
