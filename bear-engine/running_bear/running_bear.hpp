/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The main class of the application.
 * \author Julien Jorge
 */
#ifndef __BEAR_RUNNING_BEAR_HPP__
#define __BEAR_RUNNING_BEAR_HPP__

#include <claw/application.hpp>
#include "engine/game.hpp"

namespace bear
{
  /**
   * \brief The main class of the application.
   * \author Julien Jorge
   */
  class running_bear : public claw::application
  {
  public:
    running_bear( int& argc, char** &argv );
    virtual ~running_bear();

    virtual int run();

  private:
    void create_game( int& argc, char** &argv );
    void help() const;

  private:
    /** \brief The game we are running. */
    engine::game* m_game;

  }; // class running_bear
} // namespace bear

#endif // __BEAR_RUNNING_BEAR_HPP__
