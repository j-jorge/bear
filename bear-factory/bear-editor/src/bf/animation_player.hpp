/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class stores the status of an animation when reading it.
 * \author Julien Jorge
 */
#ifndef __BF_ANIMATION_PLAYER_HPP__
#define __BF_ANIMATION_PLAYER_HPP__

#include "bf/animation.hpp"
#include "bf/libeditor_export.hpp"

namespace bf
{
  /**
   * \brief This class stores the status of an animation when reading it.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT animation_player
  {
  public:
    animation_player( const animation& anim = animation() );

    void set_animation( const animation& anim );
    const animation& get_animation() const;

    void reset();
    void next();
    void next( double d );

    double get_duration_until_next() const;
    double get_time() const;

    sprite get_sprite() const;

    void set_current_index( std::size_t index );
    std::size_t get_current_index() const;

    bool is_finished() const;

  private:
    bool sequence_is_finished() const;

    void next_index();
    void next_forward();
    void next_backward();

  private:
    /** \brief The displayed animation. */
    animation m_animation;

    /** \brief Current frame. */
    std::size_t m_index;

    /** \brief Are we playing in the normal order ? */
    bool m_forward;

    /** \brief How many full playing have we done ? */
    unsigned int m_play_count;

    /** \brief Time spent on the current frame. */
    double m_time;

  }; // class animation_player
} // namespace bf

#endif // __BF_ANIMATION_PLAYER_HPP__
