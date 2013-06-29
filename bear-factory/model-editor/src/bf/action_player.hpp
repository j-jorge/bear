/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class helps getting the sprites of an action at a given date and
 *        moving through an action when playing it.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_PLAYER_HPP__
#define __BF_ACTION_PLAYER_HPP__

#include "bf/animation_player.hpp"

#include <map>

namespace bf
{
  class action;
  class mark;

  /**
   * \brief This class helps getting the sprites of an action at a given date
   *        and moving through an action when playing it.
   * \author Julien Jorge
   */
  class action_player
  {
  private:
    /** \brief A player for each mark. */
    typedef std::map<const mark*, animation_player> player_map;

    /** \brief The sprites displayed at a given date. */
    typedef std::map<const mark*, sprite> sprite_map;

  public:
    action_player( const action* a = NULL );

    void set_action( const action* a );

    void reset();
    void next();
    void next( double d );
    double get_date() const;
    void set_date( double d );
    double get_duration_until_next() const;

    sprite get_current_sprite_for( const mark* m ) const;

    bool is_finished() const;

  private:
    void fill_sprites();

  private:
    /** \brief The action played. */
    const action* m_action;

    /** \brief The players for the action. */
    player_map m_player;

    /** \brief The current date. */
    double m_date;

    /** \brief The sprite of the marks at the current date. */
    sprite_map m_sprite;

  }; // class action_player
} // namespace bf

#endif // __BF_ACTION_PLAYER_HPP__
